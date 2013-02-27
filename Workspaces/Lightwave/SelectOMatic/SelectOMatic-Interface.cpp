//
// SelectOMatic-Interface.h
//

#include "SelectOMatic-Interface.h"

#include <stdio.h>
#include <time.h>
#include <assert.h>

#include <lightwave/lwaboutpanel_tmpro/lwaboutpanel_tmpro.h>

GlobalFunc * SelectOMatic_Interface::global = NULL;

extern SelectOMatic_Data      *som_data;
extern SelectOMatic_Interface *som_int;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Select-O-Matic",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Item Selection Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001-2002 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "SelectOMatic,",
  NULL };

const char *select_mode_strings[] = {
  "Select", "Add To Selection", "Deselect", "Invert Selection", NULL };

const char *match_popup_strings[] = {
  "All Items",
  "-- -- -- -- -- -- -- -- -- ",
  "Exact Match",
  "Not Exact Match",
  "-- -- -- -- -- -- -- -- -- ",
  "Starting With",
  "Ending With",
  "Containing",
  "-- -- -- -- -- -- -- -- -- ",
  "Not Starting With",
  "Not Ending With",
  "Not Containing",
  NULL };

const char *display_mode_strings[] = {
  "Selection",
  "Groupings",
  NULL };

enum SOMMatchModeIDs {
  SELMATCH_ALL          = 0,
  SELMATCH_EXACT        = 2,
  SELMATCH_NOT_EXACT,
  SELMATCH_START        = 5,
  SELMATCH_END,
  SELMATCH_CONTAIN,
  SELMATCH_NOT_START    = 9,
  SELMATCH_NOT_END,
  SELMATCH_NOT_CONTAIN };

const char *sort_mode_strings[] = {
  "Natural", "Alphabetical", "Manual", NULL };

// Constructor
SelectOMatic_Interface::SelectOMatic_Interface( GlobalFunc *_global ) 
  : panel(NULL), is_open(false), panel_x(-1), panel_y(-1), list_height(250), list_width(203),
    ctrl_down(false), shift_down(false), list(NULL), match_case_state(0),
    select_tools_state(0), select_mode_state(0), sort_mode(0),
    display_groups(false), prev_focus(NULL), sort_mode_state(0), sort_reverse_state(0) {

  assert( som_int == NULL );

  if( _global != NULL )   // global may have become non-null externally.
    global = _global;

  panel_funcs    = (LWPanelFuncs      *)global( LWPANELFUNCS_GLOBAL,      GFUSE_ACQUIRE );
  xpanels        = (LWXPanelFuncs     *)global( LWXPANELFUNCS_GLOBAL,     GFUSE_ACQUIRE );
  raster_funcs   = (LWRasterFuncs     *)global( LWRASTERFUNCS_GLOBAL,     GFUSE_ACQUIRE );
  message        = (LWMessageFuncs    *)global( LWMESSAGEFUNCS_GLOBAL,    GFUSE_ACQUIRE );
  inst_update    = (LWInstUpdate      *)global( LWINSTUPDATE_GLOBAL,      GFUSE_ACQUIRE );
  panel_controls = (TMP_PanelControls *)global( TMP_PANELCONTROLS_GLOBAL, GFUSE_ACQUIRE );
  raster_draw    = (TMP_RasterDraw    *)global( TMP_RASTERDRAW_GLOBAL,    GFUSE_ACQUIRE );
  command        = (LWCommandFunc     *)global( "LW Command Interface",   GFUSE_ACQUIRE );
  context_funcs  = (ContextMenuFuncs  *)global( LWCONTEXTMENU_GLOBAL,     GFUSE_ACQUIRE );

  if( !raster_draw || !panel_controls ) {
    message->error( "Cuttlefish Error:  Unable to find the PanelTools globals, which are required for the interface",
                    "Add the plug-in PanelTools.p and try again" );
    return;
  }

  AquireVariableGlobals();

  som_int = this;

  list_menu = CONTEXT_MENU( context_funcs, ListMenuName, ListMenuCount, NULL );

  HandleReset( NULL, NULL );
  match_string_state[0] = '\0';

  CreateUI();
}

// Destructor
SelectOMatic_Interface::~SelectOMatic_Interface() {
  global( LWPANELFUNCS_GLOBAL,      GFUSE_RELEASE );
  global( LWXPANELFUNCS_GLOBAL,     GFUSE_RELEASE );
  global( LWRASTERFUNCS_GLOBAL,     GFUSE_RELEASE );
  global( LWMESSAGEFUNCS_GLOBAL,    GFUSE_RELEASE );
  global( LWINSTUPDATE_GLOBAL,      GFUSE_RELEASE );
  global( TMP_PANELCONTROLS_GLOBAL, GFUSE_RELEASE );
  global( TMP_RASTERDRAW_GLOBAL,    GFUSE_RELEASE );
  global( "LW Command Interface",   GFUSE_RELEASE );
  global( LWCONTEXTMENU_GLOBAL,     GFUSE_RELEASE );

  ReleaseVariableGlobals();

  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Destroy the list
  if( panel_controls != NULL )
    panel_controls->multiTreeDestroy( list );

  som_int = NULL;
}

// AquireVariableGlobals():
SelectOMatic_Interface::AquireVariableGlobals() {
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_ACQUIRE );
  scene_info  = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_ACQUIRE );
  item_info   = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_ACQUIRE );
}

// ReleaseVariableGlobals():
SelectOMatic_Interface::ReleaseVariableGlobals() {
  global( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
  global( LWSCENEINFO_GLOBAL,     GFUSE_RELEASE );
  global( LWITEMINFO_GLOBAL,      GFUSE_RELEASE );

  ui         = NULL;
  scene_info = NULL;
}

// ReaquireVariableGlobals():
SelectOMatic_Interface::ReaquireVariableGlobals() {
  ReleaseVariableGlobals();
  AquireVariableGlobals();
}

// CreateUI():
//  Creates the UI, but doesn't open it.  Called by the constructor.
void SelectOMatic_Interface::CreateUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Destroy the list if it still exists
  if( list != NULL )
    panel_controls->multiTreeDestroy( list );

  char title_string[256] = "Select-O-Matic";
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return;
  }

  // Display Mode
  display_mode_choice = HCHOICE_CTL( panel_funcs, panel, "Display", display_mode_strings );

  // Sort Pop-up
  sort_mode    = WPOPUP_CTL( panel_funcs, panel, "Sort", sort_mode_strings, 100 );
  PanelTools::AlignLabels( display_mode_choice, sort_mode );

  // Reverse
  sort_reverse = BOOLBUTTON_CTL( panel_funcs, panel, "Reverse");
  PanelTools::PutOnLineWith( sort_mode, sort_reverse );

  // Lister
  int list_column_widths[] = { 20, 1000, 0 };
  list = panel_controls->multiTree( global, panel, list_width, list_height, list_column_widths, 1, 1, 1 );
  panel_controls->multiTreeQuery(         list, List_Query         );
  panel_controls->multiTreeNthChild(      list, List_NthChild      );
  panel_controls->multiTreeCustomDraw(    list, List_CustomDraw    );
  panel_controls->multiTreeItemEvent(     list, List_ItemEvent     );
  panel_controls->multiTreeClickEvent(    list, List_ClickEvent    );
  panel_controls->multiTreeDragDropEvent( list, List_DragDropEvent );

  panel_controls->multiTreeRefresh( list );

  // Selection Tools
  select_tools = BOOLBUTTON_CTL( panel_funcs, panel, "Selection Tools" );

  // Fetch
  fetch        = WBUTTON_CTL( panel_funcs, panel, "Fetch", 50 );
  PanelTools::PutOnLineWith( select_tools, fetch, 3 );

  // About
  about        = WBUTTON_CTL( panel_funcs, panel, "About", 50 );
  PanelTools::PutOnLineWith( fetch, about );

  // - Select Modes
  select_mode  = WPOPUP_CTL( panel_funcs, panel, "Mode", select_mode_strings, 105 );

  // - Reset
  reset        = WBUTTON_CTL( panel_funcs, panel, "Reset", 45 );

  // - Match Mode
  match_popup  = WPOPUP_CTL( panel_funcs, panel, "Match", match_popup_strings, 105 );

  // - Match Case
  match_case   = BOOLBUTTON_CTL( panel_funcs, panel, "Case" );

  // - Match String
  match_string = STR_CTL( panel_funcs, panel, "With", 30 );

  // - Step Amount
  skip_amount  = INT_CTL( panel_funcs, panel, "Skip" );

  // - Step Mode
  keep_amount  = INT_CTL( panel_funcs, panel, "Keep" );

  // - Random Percent
  random       = PERCENT_CTL( panel_funcs, panel, "Random" );

  // - Select/Deselect
  execute      = WBUTTON_CTL( panel_funcs, panel, "Execute", 60 );

  // Right align some controls
  int y = CON_Y( about );
  int w = CON_W( about );
  int pw = CON_W( ((LWControl *)(panel_controls->multiTreeGetControl( list ))) );
  MOVE_CON( about, (pw - w + 3), y );

  // Initialize some values
  SET_INT(   som_int->display_mode_choice, (som_int->display_groups ? 1 : 0) );
  SET_INT(   som_int->sort_mode,           som_int->sort_mode_state );
  SET_INT(   som_int->sort_reverse,        som_int->sort_reverse_state );
  SET_INT(   som_int->select_tools,        som_int->select_tools_state );
  SET_INT(   som_int->select_mode,         som_int->select_mode_state );
  SET_INT(   som_int->match_popup,         som_int->match_popup_state );
  SET_INT(   som_int->match_case,          som_int->match_case_state );
  SET_STR(   som_int->match_string,        som_int->match_string_state, strlen( som_int->match_string_state ) );
  SET_INT(   som_int->skip_amount,         som_int->skip_state );
  SET_INT(   som_int->keep_amount,         som_int->keep_state );
  SET_FLOAT( som_int->random,              (som_int->random_state*100.0) );

  // Adjust Hidden Controls
  HandleSelectTools( select_tools, NULL );

  // Update the selected items list
  HandleFetch( fetch, NULL );

  // Event Handlers
  CON_SETEVENT( display_mode_choice, HandleDisplayMode, NULL );
  CON_SETEVENT( sort_mode,           HandleSortMode,    NULL );
  CON_SETEVENT( sort_reverse,        HandleSortMode,    NULL );
  CON_SETEVENT( select_tools,        HandleSelectTools, NULL );
  CON_SETEVENT( fetch,               HandleFetch,       NULL );
  CON_SETEVENT( about,               HandleAbout,       NULL );
  CON_SETEVENT( reset,               HandleReset,       NULL );
  CON_SETEVENT( match_popup,         HandleMatchPopup,  NULL );
  CON_SETEVENT( execute,             HandleExecute,     NULL );

  // Set some panel callbacks
  (*panel_funcs->set)( panel, PAN_USERCLOSE,     HandlePanelClose    );
  (*panel_funcs->set)( panel, PAN_USEROPEN,      HandlePanelOpen     );
  (*panel_funcs->set)( panel, PAN_USERDRAW,      HandleDrawPanel     );
  (*panel_funcs->set)( panel, PAN_MOUSEBUTTON,   HandlePanelMouse    );
  (*panel_funcs->set)( panel, PAN_MOUSEMOVE,     HandlePanelMouse    );
  (*panel_funcs->set)( panel, PAN_USERKEYS,      HandleKeys          );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS,    HandleKeyUps        );
  (*panel_funcs->set)( panel, PAN_USERACTIVATE,  HandlePanelActivate );
}

// OpenUI():
void SelectOMatic_Interface::OpenUI() {
  assert( som_int != NULL );

  if( !som_int->is_open )
    (*som_int->panel_funcs->open)( som_int->panel, PANF_MOUSETRAP | PANF_PASSALLKEYS );
}


// Group Popup Callbacks
int GroupCount( void * data ) {
  return som_data->GetGroups().NumElements() + 5;
}

char * GroupName( void * data, int index ) {
  int count = som_data->GetGroups().NumElements();

  switch( (count - 1 - index) ) {
     case -1:              // Divider
       return " -- -- -- -- -- -- -- -- -- -- ";

     case -2:              // Current Selection
       return "Current Selection";

     case -3:              // Divider
       return " -- -- -- -- -- -- -- -- -- -- ";

     case -4:              // Create Group
       return "Create Group";

     case -5:              // Delete Group
       return "Delete Group";

    default:
      return ((som_data->GetGroups()[ index ]->GetName() == NULL) ? "(unnamed)" : (char *)som_data->GetGroups()[ index ]->GetName() );
  }
}

// HandlePanelClose():
//  Called when the panel is closed by the user.
void HandlePanelClose( LWPanelID pan, void *data ) {
  if( som_int != NULL )
    som_int->is_open = false;

  som_int->panel_x = PAN_GETX( som_int->panel_funcs, som_int->panel );
  som_int->panel_y = PAN_GETY( som_int->panel_funcs, som_int->panel );

  int state;
  GET_INT( som_int->display_mode_choice, state );
  som_int->display_groups = (state != 0);

  GET_INT(   som_int->sort_mode,    som_int->sort_mode_state         );
  GET_INT(   som_int->sort_reverse, som_int->sort_reverse_state      );
  GET_INT(   som_int->select_tools, som_int->select_tools_state      );
  GET_INT(   som_int->select_mode,  som_int->select_mode_state       );
  GET_INT(   som_int->match_popup,  som_int->match_popup_state       );
  GET_INT(   som_int->match_case,   som_int->match_case_state        );
  GET_STR(   som_int->match_string, som_int->match_string_state, 256 );
  GET_INT(   som_int->skip_amount,  som_int->skip_state              );
  GET_INT(   som_int->keep_amount,  som_int->keep_state              );
  GET_FLOAT( som_int->random,       som_int->random_state            );

  som_int->random_state /= 100.0;
}

// HandlePanelOpen():
//  Called when the panel is opened by the user.
void HandlePanelOpen( LWPanelID pan, void *data ) {
  if( som_int != NULL )
    som_int->is_open = true;

  if( som_int->panel_x != -1 )
    MOVE_PAN( som_int->panel_funcs, som_int->panel, som_int->panel_x, som_int->panel_y );
}

// HandlePanelActivate():
//  Called when the panel is opened by the user.
void HandlePanelActivate( LWPanelID pan, void *data ) {
  som_int->ReaquireVariableGlobals();
}

// HandlePanelMouse():
//  Handles resizing the panel when the user clicks within the
//   resize area.
void HandlePanelMouse( LWPanelID, void *data, int flags, int x, int y ) {
  static bool was_down = false;
  static bool down_to_resize = false;
  static int  down_x, down_y;
  static int  down_pan_w, down_pan_h;
  static int  down_list_width, down_list_height;

  som_int->prev_focus = som_int->panel_controls->multiTreeHandle( som_int->list, som_int->prev_focus, flags, x, y );
  if( (som_int->prev_focus != NULL) && (som_int->prev_focus != (void *)-1) )
    return;

  if( flags & MOUSE_DOWN ) {
    if( was_down == false ) {
      was_down = true;
      down_x = x;
      down_y = y;
      down_pan_w = PAN_GETW( som_int->panel_funcs, som_int->panel );
      down_pan_h = PAN_GETH( som_int->panel_funcs, som_int->panel );
      down_list_height = som_int->list_height;
      down_list_width  = som_int->list_width;

      if( (down_x > (down_pan_w - 20)) && (down_y > (down_pan_h - 20)) ) {
        down_to_resize = true;
        som_int->prev_focus = (void *)-1;
      }
    }
  } else {
    if( down_to_resize ) {
      (*som_int->panel_funcs->close)( som_int->panel );
      som_int->prev_focus = NULL;
      som_int->CreateUI();
      som_int->OpenUI();
    }

    was_down = false;
    down_to_resize = false;
  }

  if( was_down ) {
    if( down_to_resize ) {
      // Vertical Resizing
      int new_h = (y - down_y + down_pan_h);

      int state;
      GET_INT( som_int->select_tools, state );
      if( state == 0 ) {
        if( new_h < 170 )
          new_h = 170;
      } else {
        if( new_h < 300 )
          new_h = 300;
      }

      som_int->list_height = down_list_height + (y - down_y);
      if( som_int->list_height < 50 )
        som_int->list_height = 50;

      // Horizontal Resizing
      int new_w = (x - down_x + down_pan_w);
      if( new_w < 210 )
        new_w = 210;

      som_int->list_width = down_list_width + (x - down_x);
      if( som_int->list_width < 203 )
        som_int->list_width = 203;

      // Resize the panel
      PAN_SETH( som_int->panel_funcs, som_int->panel, new_h );
      PAN_SETW( som_int->panel_funcs, som_int->panel, new_w );
    }
  }
}

// HandleDrawMainPanel() {
void HandleDrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( som_int->panel_funcs->drawFuncs );

  int ph = PAN_GETH( som_int->panel_funcs, som_int->panel );
  int pw = PAN_GETW( som_int->panel_funcs, som_int->panel );

  // Horizontal Dividers
  int state;
  GET_INT( som_int->select_tools, state );

  if( state == 1 ) {
    int y = CON_Y( som_int->select_tools );
    int h = CON_H( som_int->select_tools );
    PanelDraw::HorizontalDivider( som_int->panel, &df, 0, y + h + 3, pw, 1 );

    y = CON_Y( som_int->select_mode );
    h = CON_H( som_int->select_mode );
    PanelDraw::HorizontalDivider( som_int->panel, &df, 20, y + h + 3, pw - 40, 1 );

    y = CON_Y( som_int->match_string );
    h = CON_H( som_int->match_string );
    PanelDraw::HorizontalDivider( som_int->panel, &df, 20, y + h + 3, pw - 40, 1 );

    y = CON_Y( som_int->skip_amount );
    h = CON_H( som_int->skip_amount );
    PanelDraw::HorizontalDivider( som_int->panel, &df, 20, y + h + 3, pw - 40, 1 );

    y = CON_Y( som_int->random );
    h = CON_H( som_int->random );
    PanelDraw::HorizontalDivider( som_int->panel, &df, 20, y + h + 3, pw - 40, 1 );
  }

  // Resize Button
  df.DrawLine( som_int->panel, LWP_WHITE, pw-18, ph-2, pw-2, ph-18 );
  df.DrawLine( som_int->panel, LWP_WHITE, pw-14, ph-2, pw-2, ph-14 );
  df.DrawLine( som_int->panel, LWP_WHITE, pw-10, ph-2, pw-2, ph-10 );
  df.DrawLine( som_int->panel, LWP_WHITE, pw-6,  ph-2, pw-2, ph-6  );

  df.DrawLine( som_int->panel, LWP_BLACK, pw-17, ph-2, pw-2, ph-17 );
  df.DrawLine( som_int->panel, LWP_BLACK, pw-13, ph-2, pw-2, ph-13 );
  df.DrawLine( som_int->panel, LWP_BLACK, pw-9,  ph-2, pw-2, ph-9  );
  df.DrawLine( som_int->panel, LWP_BLACK, pw-5,  ph-2, pw-2, ph-5  );
}

// HandleKeys():
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  int state = 0;

  switch( key ) {
    case LWDK_CTRL:             // Control
      som_int->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      som_int->shift_down = true;
      break;

    case 27:                    // Cancel (ESC)
      (*som_int->panel_funcs->close)( som_int->panel );
      break;
  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  switch( key ) {
    case LWDK_CTRL:            // Control
      som_int->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      som_int->shift_down = false;
      break;
  }
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( som_int->panel_funcs, "Select-O-Matic", about_text, som_int->panel, true, NULL );
}

// HandleDisplayMode():
void HandleDisplayMode( LWControl *con, void *data ) {
  int state;
  GET_INT( som_int->display_mode_choice, state );
  som_int->display_groups = (state != 0);

  if( som_int->display_groups )
    som_int->panel_controls->multiTreeRefresh( som_int->list );
  else
    HandleFetch( som_int->fetch, NULL );
}

// HandleFetch():
//  Re-fetches the selected items from Layout, since things like
//   Add/Remove Selection aren't sent to Masters
void HandleFetch( LWControl *con, void *data ) {
  som_data->GetSelected().Flush();

  LWInterfaceInfo *ui        = (LWInterfaceInfo *)som_data->global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    som_data->GetSelected().Add( new SelectOMatic_Item( ui->selItems[i], i, NULL ) );

  HandleSortMode( som_int->sort_mode, data );
}

// HandleSortMode():
void HandleSortMode( LWControl *con, void *data ) {
  int reverse, mode;
  GET_INT( som_int->sort_mode,    mode    );
  GET_INT( som_int->sort_reverse, reverse );

  if( som_int->display_groups ) {
    switch( mode ) {
      case 0:  // Natural (Item ID)
        som_data->GetRootGroup().QSortChildren( reverse ? QSortItemsByIDReverse : QSortItemsByID );
        break;

      case 1:  // Alphabetical (By Name)
        som_data->GetRootGroup().QSortChildren( reverse ? QSortItemsByNameReverse : QSortItemsByName );
        break;

      case 2:  // Manual/Selection Order (do nothing)
        break;
    }
  } else {
    switch( mode ) {
      case 0:  // Natural (Item ID)
        som_data->GetSelected().QSort( reverse ? QSortItemsByIDReverse : QSortItemsByID );
        break;

      case 1:  // Alphabetical (By Name)
        som_data->GetSelected().QSort( reverse ? QSortItemsByNameReverse : QSortItemsByName );
        break;

      case 2:  // Manual/Selection Order
        som_data->GetSelected().QSort( reverse ? QSortItemsByOrderReverse : QSortItemsByOrder );
        break;
    }
  }

  if( data == NULL )
    som_int->panel_controls->multiTreeRefresh( som_int->list );
}

// QSortItemsByID():
int (__cdecl QSortItemsByID)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( (sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP) ) {
    if( (sel1->GetName() == NULL) || (sel2->GetName() == NULL) )
      return -1;

    return stricmp( sel1->GetName(), sel2->GetName() );
  } else if( (sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP) ) {
    return ((sel1->GetID() < sel2->GetID()) ? -1 : 1);
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// QSortItemsByIDReverse():
int (__cdecl QSortItemsByIDReverse)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( (sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP) ) {
    if( (sel1->GetName() == NULL) || (sel2->GetName() == NULL) )
      return -1;

    return stricmp( sel1->GetName(), sel2->GetName() );
  } else if( (sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP) ) {
    return ((sel1->GetID() < sel2->GetID()) ? 1 : -1);
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// QSortItemsByName():
int (__cdecl QSortItemsByName)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( (sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP) ) {
    if( (sel1->GetName() == NULL) || (sel2->GetName() == NULL) )
      return -1;

    return stricmp( sel1->GetName(), sel2->GetName() );
  } else if( (sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP) ) {
    char name1[512], name2[512];
    strcpy( name1, som_int->item_info->name( sel1->GetID() ) );
    strcpy( name2, som_int->item_info->name( sel2->GetID() ) );
    return stricmp( name1, name2 );
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// QSortItemsByNameReverse():
int (__cdecl QSortItemsByNameReverse)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( (sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP) ) {
    if( (sel1->GetName() == NULL) || (sel2->GetName() == NULL) )
      return -1;

    return -stricmp( sel1->GetName(), sel2->GetName() );
  } else if( (sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP) ) {
    char name1[512], name2[512];
    strcpy( name1, som_int->item_info->name( sel1->GetID() ) );
    strcpy( name2, som_int->item_info->name( sel2->GetID() ) );
    return -stricmp( name1, name2 );
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// QSortItemsByOrder():
int (__cdecl QSortItemsByOrder)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( ((sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP)) ||
      ((sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP)) ) {
    return ((sel1->GetOriginalIndex() < sel2->GetOriginalIndex()) ? -1 : 1);
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// QSortItemsByOrderReverse():
int (__cdecl QSortItemsByOrderReverse)( const void *arg1, const void *arg2 ) {
  SelectOMatic_Item *sel1 = *(SelectOMatic_Item **)(arg1);
  SelectOMatic_Item *sel2 = *(SelectOMatic_Item **)(arg2);

  if( ((sel1->GetID() == SOM_GROUP) && (sel2->GetID() == SOM_GROUP)) ||
      ((sel1->GetID() != SOM_GROUP) && (sel2->GetID() != SOM_GROUP)) ) {
    return ((sel1->GetOriginalIndex() < sel2->GetOriginalIndex()) ? 1 : -1);
  }

  return ((sel1->GetID() == SOM_GROUP) ? -1 : 1);
}

// HandleSelectTools():
void HandleSelectTools( LWControl *con, void *data ) {
  int state;
  GET_INT( con, state );

  int ph = 0;
  if( state == 0 ) {
    MOVE_CON( som_int->select_mode,  4, -100 );
    MOVE_CON( som_int->reset,        4, -100 );
    MOVE_CON( som_int->match_string, 4, -100 );
    MOVE_CON( som_int->match_case,   4, -100 );
    MOVE_CON( som_int->match_popup,  4, -100 );
    MOVE_CON( som_int->skip_amount,  4, -100 );
    MOVE_CON( som_int->keep_amount,  4, -100 );
    MOVE_CON( som_int->random,       4, -100 );
    MOVE_CON( som_int->execute,      4, -100 );

    GHOST_CON( som_int->select_mode  );
    GHOST_CON( som_int->reset        );
    GHOST_CON( som_int->match_string );
    GHOST_CON( som_int->match_case   );
    GHOST_CON( som_int->match_popup  );
    GHOST_CON( som_int->skip_amount  );
    GHOST_CON( som_int->keep_amount  );
    GHOST_CON( som_int->random       );
    GHOST_CON( som_int->execute      );

    ERASE_CON( som_int->select_mode  );
    ERASE_CON( som_int->reset        );
    ERASE_CON( som_int->match_popup  );
    ERASE_CON( som_int->match_case   );
    ERASE_CON( som_int->match_string );
    ERASE_CON( som_int->skip_amount  );
    ERASE_CON( som_int->keep_amount  );
    ERASE_CON( som_int->random       );
    ERASE_CON( som_int->execute      );

    ph  = CON_Y( som_int->select_tools );
    ph += CON_H( som_int->select_tools );
  } else {
    LWControl *align[] = { som_int->select_mode, som_int->match_popup,
                           som_int->match_string, som_int->skip_amount,
                           som_int->random, NULL };
    PanelTools::AlignLabels( align );

    PanelTools::PutUnder( som_int->select_tools, som_int->select_mode, 18 );
    PanelTools::PutOnLineWith( som_int->select_mode, som_int->reset );

    PanelTools::PutUnder( som_int->select_mode, som_int->match_popup, 18 );
    PanelTools::PutOnLineWith( som_int->match_popup, som_int->match_case );
    PanelTools::PutUnder( som_int->match_popup, som_int->match_string );

    PanelTools::PutUnder( som_int->match_string, som_int->skip_amount, 18 );
    PanelTools::PutOnLineWith( som_int->skip_amount, som_int->keep_amount );

    PanelTools::PutUnder( som_int->skip_amount, som_int->random, 18 );

    PanelTools::PutUnder( som_int->random, som_int->execute, 18 );
    int y = CON_Y( som_int->execute );
    int w = CON_W( som_int->execute );
    int pw = PAN_GETW( som_int->panel_funcs, som_int->panel );
    MOVE_CON( som_int->execute, (pw/2 - w/2), y );

    UNGHOST_CON( som_int->select_mode  );
    UNGHOST_CON( som_int->reset        );
    UNGHOST_CON( som_int->match_popup  );
    UNGHOST_CON( som_int->match_case   );
    UNGHOST_CON( som_int->match_string );
    UNGHOST_CON( som_int->skip_amount  );
    UNGHOST_CON( som_int->keep_amount  );
    UNGHOST_CON( som_int->random       );
    UNGHOST_CON( som_int->execute      );

    HandleMatchPopup( som_int->match_popup, NULL );

    ph  = CON_Y( som_int->execute );
    ph += CON_H( som_int->execute );
  }

  PAN_SETH( som_int->panel_funcs, som_int->panel, ph + 34 );

  if( som_int->is_open ) {
    (*som_int->panel_funcs->draw)( som_int->panel, DR_REFRESH );
    som_int->panel_controls->multiTreeRefresh( som_int->list );
  }
}

// HandleReset():
void HandleReset( LWControl *con, void *data ) {
  som_int->match_popup_state = 0;
  som_int->skip_state        = 0;
  som_int->keep_state        = 1;
  som_int->random_state      = 1.0;

  if( con != NULL ) {
    SET_INT(   som_int->match_popup,  som_int->match_popup_state );
    SET_INT(   som_int->skip_amount,  som_int->skip_state );
    SET_INT(   som_int->keep_amount,  som_int->keep_state );
    SET_FLOAT( som_int->random,       (som_int->random_state*100.0) );

    HandleMatchPopup( som_int->match_popup, NULL );

    som_int->panel_controls->multiTreeRefresh( som_int->list );
  }
}

// HandleMatchPopup():
void HandleMatchPopup( LWControl *con, void *data ) {
  int state;
  GET_INT( con, state );

  // NOTE:  There are _NO_ breaks below
  switch( state ) {
    case 1:             // Dividers
    case 4:
    case 8:
      state--;

    default:
      if( state == 0 )
        GHOST_CON( som_int->match_string );
      else
        UNGHOST_CON( som_int->match_string );

      SET_INT( con, state );
  }
}

// HandleExecute():
void HandleExecute( LWControl *con, void *data ) {
  // See if any items are selected so we know what item type we're dealing with
  if( som_data->GetSelected().NumElements() == 0 ) {
    (*som_int->message->error)( "Select-O-Matic Error:  There aren't any items of this type in the scene to select", NULL );
    return;
  }

  // Walk the items and go selecting
  LWItemType type = (*som_int->item_info->type)( som_data->GetSelected()[0]->GetID() );
  LWItemID owner = NULL;

  if( type == LWI_BONE ) {
    for( owner = (*som_int->item_info->parent)( som_data->GetSelected()[0]->GetID() ); owner != NULL; owner = (*som_int->item_info->parent)( owner ) ) {
      if( (*som_int->item_info->type)( owner ) == LWI_OBJECT )
        break;
    }

    if( owner == NULL ) {
      (*som_int->message->error)( "Select-O-Matic Error:  Unable to find bone's owner object; aborting", NULL );
      return;
    }
  }

  // Grab the interface state for matching
  int mode, match_mode, match_case, skip_amount, keep_amount;
  double random;
  GET_INT(   som_int->select_mode, mode        );
  GET_INT(   som_int->match_popup, match_mode  );
  GET_INT(   som_int->match_case,  match_case  );
  GET_INT(   som_int->skip_amount, skip_amount );
  GET_INT(   som_int->keep_amount, keep_amount );
  GET_FLOAT( som_int->random,      random      );
  random /= 100.0;

  char match_with[ 256 ];
  GET_STR( som_int->match_string, match_with, 256 );

  int (__cdecl *compare)(  const char *string1, const char *string2 )               = &stricmp;
  int (__cdecl *comparen)( const char *string1, const char *string2, size_t count ) = &strnicmp;
  if( match_case != 0 ) {
    compare  = &strcmp;
    comparen = &strncmp;
  }

  srand( (unsigned int)time( NULL ) << 4 );

  // Start selecting
  int  cur_skip = -1;
  int  cur_keep = -1;
  int  count    =  0;
  char buffer[ 256 ];

  som_data->SetEnableUpdate( false );

  for( LWItemID item = (*som_int->item_info->first)( type, owner ); item != NULL; item = (*som_int->item_info->next)( item ) ) {
    const char *name = (*som_int->item_info->name)( item );

    // Pattern Matching
    switch( match_mode ) {
      case SELMATCH_ALL:
        break;

      case SELMATCH_EXACT:
        if( (*compare)( name, match_with ) != 0 )
          continue;
        break;

      case SELMATCH_NOT_EXACT:
        if( (*compare)( name, match_with ) == 0 )
          continue;
        break;

      case SELMATCH_START:
        if( (*comparen)( name, match_with, strlen( match_with ) ) != 0 )
          continue;
        break;

      case SELMATCH_END: {
        int len1 = strlen( name );
        int len2 = strlen( match_with );
        if( len1 < len2 )
          break;

        if( (*comparen)( &(name[ (len1-len2) ]), match_with, len2 ) != 0 ) 
          continue;
      } break;

      case SELMATCH_CONTAIN:
        if( match_case == 0 ) {
          if( StringTools::stristr( name, match_with ) == 0 )
            continue;
        } else {
          if( strstr( name, match_with ) == 0 )
            continue;
        }
        break;

      case SELMATCH_NOT_START:
        if( (*comparen)( name, match_with, strlen( match_with ) ) == 0 )
          continue;
        break;

      case SELMATCH_NOT_END: {
        int len1 = strlen( name );
        int len2 = strlen( match_with );
        if( len1 < len2 )
          break;

        if( (*comparen)( &(name[ (len1-len2) ] ), match_with, len2 ) == 0 ) 
          continue;
      } break;

      case SELMATCH_NOT_CONTAIN:
        if( match_case == 0 ) {
          if( StringTools::stristr( name, match_with ) != 0 )
            continue;
        } else {
          if( strstr( name, match_with ) != 0 )
            continue;
        }
        break;
    }

    // Skip/Keep Step
    cur_skip++;
    if( cur_skip < skip_amount )
      continue;

    cur_keep++;
    if( cur_keep >= (keep_amount - 1) ) {
      cur_skip = -1;
      cur_keep = -1;
    }

    // Random
    if( ((double)rand() / (double)RAND_MAX) > random ) 
      continue;

    // If we got this far, then we're going to change the selection.
    count++;

    switch( mode ) {
      case 0:          // Select
        if( count == 1 ) {
          sprintf( buffer, "SelectItem %x", item );
          break;       // - If count != 1, then continue on to Add To Selection
        }

      case 1:          // Add To Selection
        sprintf( buffer, "AddToSelection %x", item );
        break;

      case 2:          // Remove From Selection
        sprintf( buffer, "RemoveFromSelection %x", item );
        break;

      case 3:          // Invert Selection
        if( (*som_int->ui->itemFlags)( item ) & LWITEMF_SELECTED )
          sprintf( buffer, "RemoveFromSelection %x", item );
        else
          sprintf( buffer, "AddToSelection %x", item );
        break;

      default:
        assert( false );
    }

    som_int->command( buffer );
  }

  if( count == 0 ) {
    (*som_int->message->error)( "Select-O-Matic Error:  No items match the search",
                                "criteria;  the selection remains unchanged" );
  }

  // Update the selected list
  HandleFetch( som_int->fetch, NULL );
  som_data->SetEnableUpdate( true );
}

// List_Query():
int List_Query( MultiTreeID mt, void *user_data, void *item, MultiTree_Queries query ) {
  if( query == MULTITREEQUERY_IS_SELECTED ) {
    if( som_int->display_groups ) {
      // Groups
      SelectOMatic_Item *group = (SelectOMatic_Item *)item;
      if( group != NULL )
        return (group->GetIsSelected() ? 1 : 0);
    } else {
      // Selection
      return 1;
    }
  }

  if( som_int->display_groups ) {
    SelectOMatic_Item *group = (SelectOMatic_Item *)item;
    if( group == NULL )
      return 0;

    if( query == MULTITREEQUERY_IS_FOLDABLE ) {
      if( group->GetID() == SOM_GROUP )
        return 1;
    } else if( query == MULTITREEQUERY_IS_FOLDED ) {
      if( group->GetID() == SOM_GROUP )
        return (group->GetIsFolded() ? 1 : 0);
    }
  }

  return 0;
}

// List_NthChild():
void * List_NthChild( MultiTreeID mt, void *user_data, void *item, int index ) {
  if( som_int->display_groups ) {
    // Groups
    if( item == NULL ) {
      if( index < (int)som_data->GetGroups().NumElements() )
        return som_data->GetGroups()[ index ];
    } else {
      SelectOMatic_Item *group = (SelectOMatic_Item *)item;
      assert( group->GetID() == SOM_GROUP );
      if( index < (int)group->GetChildren().NumElements() )
        return group->GetChildren()[ index ];
    }
  } else {
    // Selection
    if( item == NULL ) {
      if( index < (int)som_data->GetSelected().NumElements() )
        return som_data->GetSelected()[ index ];
    }
  }

  return NULL;
}

/* Some Icon Colors */
const int ItemColors[] = {
  RGB_(  0,   0,   0),    /* Black       */
  RGB_(  0,  48, 128),    /* Dark Blue   */
  RGB_(  0,  96,   0),    /* Dark Green  */
  RGB_( 32,  96, 112),    /* Dark Cyan   */
  RGB_(112,   0,   0),    /* Dark Red    */
  RGB_(112,  32, 112),    /* Purple      */
  RGB_(112,  80,   0),    /* Brown       */
  RGB_(176, 176, 176),    /* Grey        */
  RGB_( 32, 160, 240),    /* Blue        */
  RGB_( 32, 224,  32),    /* Green       */
  RGB_( 96, 224, 240),    /* Cyan        */
  RGB_(240,  32,  32),    /* Red         */
  RGB_(240,  96, 240),    /* Magenta     */
  RGB_(240, 192,  32),    /* Orange      */
  RGB_(240, 240, 240)     /* White       */
};

const int GroupColors[] = {
  RGB_( 225, 225, 191 ),
  RGB_( 225, 225, 191 ),
  RGB_( 225, 225, 191 ),

  RGB_( 32, 150,  32),    /* Green       */
  RGB_( 96, 150, 170),    /* Cyan        */
  RGB_(170,  96, 170)     /* Magenta     */

};

// List_CustomDraw():
const char * List_CustomDraw( MultiTreeID mt, void *user_data, void *item, int column, LWRasterID raster, MultiTree_DrawStages stage, int x, int y, int w, int h ) {
  static char buffer[20];

  if( stage == MULTITREEDRAW_ICON ) {
    // Item Type Icon
    if( item == NULL )
      return NULL;

    if( column == 0 )
      return NULL;

    SelectOMatic_Item *the_item = (SelectOMatic_Item *)item;
    if( the_item->GetID() == SOM_GROUP ) {
      som_int->raster_funcs->drawBox( raster, COLOR_BLACK, x,     y+h/3-1, 5, h/3-1 );
      som_int->raster_funcs->drawBox( raster, COLOR_BLACK, x + 6, y+2,     5, h/3-1 );
      som_int->raster_funcs->drawBox( raster, COLOR_BLACK, x + 8, y+8,     5, h/3-1 );

      som_int->raster_funcs->drawBox( raster, GroupColors[0], x + 1, y+h/3, 3, h/3-3 );
      som_int->raster_funcs->drawBox( raster, GroupColors[1], x + 7, y+3,   3, h/3-3 );
      som_int->raster_funcs->drawBox( raster, GroupColors[2], x + 9, y+9,   3, h/3-3 );
      return (const char *)(som_int->raster_draw->iconWidth( TMPICON_OBJECT ) + 2);
    }

    LWItemID id = the_item->GetID();

    int color = som_int->ui->itemColor( id );
    TMP_IconTypes shape = TMPICON_OBJECT;
    
    switch( som_int->item_info->type( id ) ) {
      case LWI_OBJECT:  shape = TMPICON_OBJECT;  break;
      case LWI_BONE:    shape = TMPICON_BONE;    break;
      case LWI_LIGHT:   shape = TMPICON_LIGHT;   break;
      case LWI_CAMERA:  shape = TMPICON_CAMERA;  break;
    }

    int icon_w  = som_int->raster_draw->iconWidth( shape );
    int shift_w = som_int->display_groups ? 0 : icon_w;
    som_int->raster_draw->icon( raster, som_int->raster_funcs, x - shift_w, y+1, shape, COLOR_BLACK, ItemColors[ color ] );

    if( som_int->display_groups )
      return (const char *)(icon_w + 2);

    return (const char *)2;
  }

  if( stage == MULTITREEDRAW_TEXT ) {
    // Column Headings
    if( item == NULL ) {
      if( column == 0 ) {
        if( som_int->display_groups )
          return MULTITREE_CHECKMARK;
        else
          return NULL;
      } else {
        if( som_int->display_groups )
          return "Grouped Item Tree";
        else
          return "Selected Item List";
      }
    }

    // List Items
    if( column == 0 ) {
      if( item == som_data->GetActiveGroup() ) {
        som_int->raster_funcs->drawBox( raster, RGB_( 255, 255, 191 ), x+1, y+3, w-8, h-7 );
        return NULL;

      } else if( item == som_data->GetActiveItem() ) {
        int icon_w = som_int->raster_draw->iconWidth( TMPICON_DOT );
        som_int->raster_draw->icon( raster, som_int->raster_funcs, x + w/2 - icon_w/2 - 3, y+1, TMPICON_DOT, COLOR_BLACK, COLOR_BLACK );
        return NULL;
      }

    } else {
      SelectOMatic_Item *the_item = (SelectOMatic_Item *)item;
      if( som_int->display_groups ) {
        if( the_item->GetID() == SOM_GROUP )
          return the_item->GetName();
      }

      return som_int->item_info->name( the_item->GetID() );
    }
  }

  return NULL;
}

// List_ItemEvent():
MulitTree_EventStates List_ItemEvent( MultiTreeID mt, void *user_data, void *item, int column, MultiTree_ItemEvents item_event ) {
  SelectOMatic_Item *the_item = (SelectOMatic_Item *)item;
  static char buffer[ 1024 ];

  if( item == NULL ) {
    if( item_event == MULTITREEITEM_DONE_SELECTING ) {
      // Done selecting; refresh list
      HandleFetch( som_int->fetch, (void *)1 );
      if( !som_int->display_groups )
        return MULTITREEEVENT_HANDLED;
    }

    return MULTITREEEVENT_CONTINUE;
  }

  unsigned long i;
  if( som_int->display_groups ) {
    // Groups
    if( item == NULL ) {
      return MULTITREEEVENT_CONTINUE;
    } else {
      switch( item_event ) {
        case MULTITREEITEM_DESELECT:
        // Deselect Item
        the_item->SetIsSelected( false );
        ChangeSelectionByItem( "RemoveFromSelection", "RemoveFromSelection", the_item );
        break;

        case MULTITREEITEM_SELECT: {
          // Select Item
          if( column == 0 ) {
            // Active Group Toggle
            som_data->SetActiveGroup( (the_item->GetID() == SOM_GROUP) ? the_item : the_item->GetParent() );

            // Active Item Toggle
            if( the_item->GetID() == SOM_GROUP ) {
              som_data->SetActiveItem( NULL );
              for( i=0; i < the_item->GetChildren().NumElements(); i++ ) {
                if( the_item->GetChildren()[i]->GetID() != SOM_GROUP ) {
                  som_data->SetActiveItem( the_item->GetChildren()[i] );
                  break;
                }
              }
            } else {
              som_data->SetActiveItem( the_item );
            }

            return MULTITREEEVENT_FINISHED;
          }

          DynArray< SelectOMatic_Item * > items;
          som_data->GetRootGroup().FindSelected( items );

          for( i=0; i < items.NumElements(); i++ )                  // Deselect all items
            items[i]->SetIsSelected( false );

          the_item->SetIsSelected( true );                          // Select this one item
          ChangeSelectionByItem( "SelectItem", "AddToSelection", the_item );
        } break;

        case MULTITREEITEM_ADD_TO_SELECTION:
          // Add item to selection
          the_item->SetIsSelected( true );
          ChangeSelectionByItem( "AddToSelection", "AddToSelection", the_item );
          break;

        case MULTITREEITEM_FOLD:
          // Fold Group
          if( the_item->GetID() == SOM_GROUP )
            the_item->SetIsFolded( true );
          break;

        case MULTITREEITEM_UNFOLD:
          // Unfold Group
          if( the_item->GetID() == SOM_GROUP )
            the_item->SetIsFolded( false );
          break;
      }

      return MULTITREEEVENT_CONTINUE;
    }

  } else {
    // Selection
    som_data->SetEnableUpdate( false );
    switch( item_event ) {
      case MULTITREEITEM_DESELECT:
        // Deselect Item
        the_item->SetIsSelected( false );
        sprintf( buffer, "RemoveFromSelection %x", the_item->GetID() );
        som_int->command( buffer );
        break;

      case MULTITREEITEM_SELECT:
        // Select Item
        for( i=0; i < som_data->GetSelected().NumElements(); i++ )  // Deselect all items
          som_data->GetSelected()[i]->SetIsSelected( false );

        the_item->SetIsSelected( true );                          // Select this one item
        sprintf( buffer, "SelectItem %x", the_item->GetID() );
        som_int->command( buffer );
        break;

      case MULTITREEITEM_ADD_TO_SELECTION:
        // Add item to selection
        the_item->SetIsSelected( true );
        sprintf( buffer, "AddToSelection %x", the_item->GetID() );
        som_int->command( buffer );
        break;
    }

    som_data->SetEnableUpdate( true );
    HandleFetch( som_int->fetch, NULL );

    return MULTITREEEVENT_CONTINUE;
  }
}

// ChangeSelectionByItem():
void ChangeSelectionByItem( const char *first_command, const char *successive_command, SelectOMatic_Item *the_item ) {
  som_data->SetEnableUpdate( false );
  static char buffer[ 1024 ];

  if( the_item->GetID() != SOM_GROUP ) {
    // - Single Item
    sprintf( buffer, "%s %x", first_command, the_item->GetID() );
    som_int->command( buffer );

  } else {
    // - Group
    DynArray< SelectOMatic_Item * > items;
    the_item->FindAllItems( items );
    if( items.NumElements() > 0 ) {
      int type = som_int->item_info->type( items[0]->GetID() );
      if( som_data->GetSelected().NumElements() > 0 ) {
        type = som_int->item_info->type( som_data->GetSelected()[0]->GetID() );

        som_data->SetEnableUpdate( false );
        int count = 0;

        // - First try the currently selected item's type
        unsigned long i;
        for( i=0; i < items.NumElements(); i++ ) {
          if( type == som_int->item_info->type( items[i]->GetID() ) ) {
            sprintf( buffer, "%s %x", ((count == 0) ? first_command : successive_command), items[i]->GetID() );
            som_int->command( buffer );
            count++;
          }
        }

        if( count == 0 ) {
          // - That failed, so try the first item type in the item array instead
          type = som_int->item_info->type( items[0]->GetID() );
          for( i=0; i < items.NumElements(); i++ ) {
            if( type == som_int->item_info->type( items[i]->GetID() ) ) {
              sprintf( buffer, "%s %x", ((count == 0) ? first_command : successive_command), items[i]->GetID() );
              som_int->command( buffer );
              count++;
            }
          }
        }
      }
    }
  }

  som_data->SetEnableUpdate( true );
}

// List_ClickEvent():
MulitTree_EventStates List_ClickEvent( MultiTreeID mt, void *user_data, void *item, int column, int mflags, int mx, int my, int double_click ) {
  if( som_int->display_groups ) {
    if( RCLICK( mflags ) ) {
      int index = som_int->context_funcs->cmenuDeploy( som_int->list_menu, som_int->panel, -1 );
      if( (index != -1) || (index != 3) ) {
        SelectOMatic_Item *the_item = NULL;

        if( (item != NULL) && (item != (void *)-1) ) {
          the_item = (SelectOMatic_Item *)item;
          if( the_item->GetID() == SOM_GROUP ) {
            // - Group Specific
            if( index == 1 ) {         // Rename Group
              // Create an XPanel for the name
              LWXPanelControl controls[] = { 0x8000, "Name", "string", NULL }; 
              LWXPanelDataDesc data[]    = { 0x8000, "Name", "string", NULL };
              LWXPanelHint panel_hints[] = { XpLABEL( 0, "Rename Group" ), XpEND };

              LWXPanelID xpanel = som_int->xpanels->create( LWXP_FORM, controls );
              som_int->xpanels->describe( xpanel, data, NULL, NULL );
              som_int->xpanels->hint( xpanel, 0, panel_hints );

              som_int->xpanels->formSet( xpanel, 0x8000, (char *)((the_item->GetName() == NULL) ? "" : the_item->GetName()) );

              // Open the panel
              if( som_int->xpanels->post( xpanel ) == 1 )  {
                char * string = (char *)som_int->xpanels->formGet( xpanel, 0x8000 );
                if( string[0] != '\0' ) {
                  the_item->SetName( string );
                }
              }

              som_int->xpanels->destroy( xpanel );

              HandleSortMode( som_int->sort_mode, NULL );
              som_int->panel_controls->multiTreeRefresh( som_int->list );

            } else if( index == 2 ) {  // Delete Group
              SelectOMatic_Item *parent = the_item->GetParent();
              if( som_int->message->yesNo( "Delete Group?", "Do you really want to delete this group?", NULL ) == 1 ) {
                if( parent == NULL ) {
                  unsigned long index = som_data->GetGroups().FindIndexOf( the_item );
                  assert( index != DA_NO_MATCH );
                  som_data->GetGroups().Remove( index );
                } else {
                  unsigned long index = parent->GetChildren().FindIndexOf( the_item );
                  assert( index != DA_NO_MATCH );
                  parent->GetChildren().Remove( index );
                }

                delete the_item;

                HandleSortMode( som_int->sort_mode, NULL );
                som_int->panel_controls->multiTreeRefresh( som_int->list );
              }
            }
          }

          if( index == 4 ) {           // Add to groups
            DynArray< SelectOMatic_Item * > groups;
            som_data->GetRootGroup().FindSelectedGroups( groups );

            unsigned long i, g;
            for( i=0; i < som_data->GetSelected().NumElements(); i++ ) {
              for( g=0; g < groups.NumElements(); g++ ) {
                SelectOMatic_Item *child = new SelectOMatic_Item( som_data->GetSelected()[ i ] );
                child->SetParent( groups[g] );
                groups[g]->GetChildren().Add( child );
              }
            }

            HandleSortMode( som_int->sort_mode, NULL );
            som_int->panel_controls->multiTreeRefresh( som_int->list );

          } else if( index == 5 ) {    // Remove from groups
            DynArray< SelectOMatic_Item * > items;
            som_data->GetRootGroup().FindSelectedItems( items );

            for( unsigned long i=0; i < items.NumElements(); i++ ) {
              unsigned long index = items[i]->GetParent()->GetChildren().FindIndexOf( items[i] );
              items[i]->GetParent()->GetChildren().Remove( index );
              delete items[i];
            }

            som_int->panel_controls->multiTreeRefresh( som_int->list );
          }
        }

        if( index == 0 ) {             // Create Group
          // Create an XPanel for the name
          LWXPanelControl controls[] = { 0x8000, "Name", "string", NULL };
          LWXPanelDataDesc data[]    = { 0x8000, "Name", "string", NULL };
          LWXPanelHint panel_hints[] = { XpLABEL( 0, "Create Group" ), XpEND };

          LWXPanelID xpanel = som_int->xpanels->create( LWXP_FORM, controls );
          som_int->xpanels->describe( xpanel, data, NULL, NULL );
          som_int->xpanels->hint( xpanel, 0, panel_hints );

          som_int->xpanels->formSet( xpanel, 0x8000, "" );

          // Open the panel
          if( som_int->xpanels->post( xpanel ) == 1 )  {
            char * string = (char *)som_int->xpanels->formGet( xpanel, 0x8000 );
            if( string[0] != '\0' ) {
              SelectOMatic_Item *parent = &som_data->GetRootGroup();
              if( the_item != NULL )
                parent = (the_item->GetID() == SOM_GROUP) ? the_item : the_item->GetParent();

              SelectOMatic_Item *group = new SelectOMatic_Item( string, parent );
              parent->GetChildren().Add( group );
              HandleSortMode( som_int->sort_mode, NULL );
            }
          }

          som_int->xpanels->destroy( xpanel );

          HandleSortMode( som_int->sort_mode, NULL );
          som_int->panel_controls->multiTreeRefresh( som_int->list );
        }
      }

      return MULTITREEEVENT_LOSE_FOCUS;
    }
  }

  return MULTITREEEVENT_CONTINUE;
}

// List_DragDropEvent():
void * List_DragDropEvent( MultiTreeID mt, void *user_data, void *item, void *target, MultiTree_DragDrops *drop_as, int apply ) {
  if( item == NULL ) {
    *drop_as = MULTITREEDRAGDROP_NONE;
    return NULL;
  }

  SelectOMatic_Item *the_item   = (SelectOMatic_Item *)item;
  SelectOMatic_Item *the_target = (SelectOMatic_Item *)target;

  // Replace child item with insertion after; only groups can have children
  if( the_target->GetID() != SOM_GROUP ) {
    if( *drop_as == MULTITREEDRAGDROP_AS_CHILD )
      *drop_as = MULTITREEDRAGDROP_AS_INSERTION_AFTER;
  }

  // If the target is selected, don't allow drop here
  if( the_target->GetIsSelected() )
    *drop_as = MULTITREEDRAGDROP_NONE;

  if( *drop_as == MULTITREEDRAGDROP_NONE )
    return target;

  // Perform drag/drop operation
  if( apply ) {
    // Find the insertion point and parent group
    unsigned long target_index = 0;
    SelectOMatic_Item *parent_group = the_target;

    if( *drop_as != MULTITREEDRAGDROP_AS_CHILD ) {
      parent_group = the_target->GetParent();
      assert( parent_group != NULL );

      target_index = 0;
      for( unsigned long i = 0; i < parent_group->GetChildren().NumElements(); i++ ) {
        if( parent_group->GetChildren()[i] == the_target )
          break;

        if( parent_group->GetChildren()[i]->GetIsSelected() )
          continue;

        target_index++;
      }

      if( *drop_as == MULTITREEDRAGDROP_AS_INSERTION_AFTER ) {
        if( !the_target->GetIsSelected() )
          target_index++;
      }
    }

    // See if it's OK to do the drag/drop operation
    if( som_data->GetRootGroup().IsDragDropOK( parent_group ) ) {

      // Build a list of items to drag, removing them from the existing list
      DynArray< SelectOMatic_Item * > dragged_items;
      som_data->GetRootGroup().BuildDragDropList( dragged_items, (parent_group->GetParent() == NULL) );

      // Insert the items at the new point in the list
      for( unsigned long i=0; i < dragged_items.NumElements(); i++ ) {
        parent_group->GetChildren().InsertAt( target_index++, dragged_items[i] );
        dragged_items[i]->SetParent( parent_group );
      }

      // Resort the groups
      HandleSortMode( som_int->sort_mode, (void *)1 );
    }
  }

  return target;
}



// Context Menu Callbacks
const char *list_menu_strings[] = {
  "Create Group",
  "Rename Group",
  "Delete Group",
  "\03(c:Nc_BG)--------------------------------------",
  "Add items selected in Layout to groups",
  "Remove selected items from groups",
  NULL };

int ListMenuCount( void *userdata ) {
  int i=0; 
  while( list_menu_strings[i] != NULL )
    i++;

  return i;
}

char * ListMenuName( void *userdata, int index ) {
  return (char *)list_menu_strings[ index ];
}

