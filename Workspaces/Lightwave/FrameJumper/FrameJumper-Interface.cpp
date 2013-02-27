//
// FrameJumper-Interface.cpp
//

#include "FrameJumper-Interface.h"

#include "framejumper.h"
#include "framejumper-data.h"

#include <stdio.h>
#include <math.h>
#include <windows.h>

#include <lightwave/lwaboutpanel_tmpro/lwaboutpanel_tmpro.h>

FrameJumper_Data        fj_data;
FrameJumper_Interface * fj_int = NULL;

GlobalFunc * FrameJumper_Interface::global = NULL;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Frame Jumper",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Frame Marking Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "FrameJumper,",
  "JumpToRenderFirstFrame, JumpToRenderLastFrame,",
  "JumpToPreviewFirstFrame, JumpToPreviewLastFrame,",
  "NextFrameByRenderStep, PreviousFrameByRenderStep,",
  "NextFrameByPreivewStep, PreviousFrameByPreviewStep,",
  "SetRenderRangeToPreviewRange, SetPreviewRangeToRenderRange,",
  "JumpToNextMarker, JumpToPreviousMarker,",
  "JumpToNextMarkerGroup, JumpToPreviousMarkerGroup,",
  "MarkFrame, UnmarkFrame, MarkFrameAs??, JumpToMarker??,",
  "FrameJumperProperties",
  NULL };

const char *keys_text[] = {
  "Esc",                  "Close Panel",
  "Arrow Up/Down",        "Previous/Next Marker",
  "Shift-Arrow Up/Down",  "First/Last Marker",
  "Ctrl-Arrow Up/Down",   "Previous/Next Group",
  "*",                    "New Group",
  "=/+",                  "Add Marker",
  "Del/-",                "Remove Marker",
  "Enter/Return",         "Go To Marker's Time",
  "Tab",                  "Activate Group Name Field",
  "m",                    "Toggle Modify Mode",
  "Shift-F1 to Shift-F12","Go To Marker At Key 1-12",
  "Ctrl-F1 to Ctrl-F12",  "Set Marker's Key To 1-12",
  "t/w",                  "Activate When Field",
  "L",                    "Set Marker to Current Layout Time",
  "Space",                "Toggle Enable Marker",
  "n",                    "Activate Name Field",
  "f",                    "Display Time In Frames",
  "s",                    "Display Time In Seconds",
  "F1",                   "About/Keys",
  NULL };

const char *time_unit_strings[] = {
  "Seconds", "Frames", NULL };

const char *key_strings[] = {
  "Off", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", NULL };

// Constructor():
FrameJumper_Interface::FrameJumper_Interface( GlobalFunc *_global ) 
  : panel(NULL), is_open(false), panel_x(-1), panel_y(-1),
    modify_state(0), units_state(1), current_marker(-1), list_height(11),
    return_true_count(false), ctrl_down(false), shift_down(false) {

  assert( fj_int == NULL );

  if( _global != NULL )   // global may have become non-null externally.
    global = _global;
  
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_ACQUIRE );
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_ACQUIRE );
  inst_update = (LWInstUpdate    *)global( LWINSTUPDATE_GLOBAL,    GFUSE_ACQUIRE );
  AquireVariableGlobals();

  unsigned long prodinfo = (unsigned long) global( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
  unsigned long major    = LWINF_GETMAJOR( prodinfo );
  unsigned long minor    = LWINF_GETMINOR( prodinfo );
  
  if( ((major >= 6) && (minor >= 1)) && (major < 7) )
    checkmark_string = "\03(i:MIMG_CHECKMARK)";
  else
    checkmark_string = "Yes";

  fj_int = this;

  CreateUI();
}

// Destructor():
FrameJumper_Interface::~FrameJumper_Interface() {
  global( LWPANELFUNCS_GLOBAL,    GFUSE_RELEASE );
  global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_RELEASE );
  global( LWINSTUPDATE_GLOBAL,    GFUSE_RELEASE );
  ReleaseVariableGlobals();

  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
    return_true_count = false;
  }

  fj_int = NULL;
}

// AquireVariableGlobals():
FrameJumper_Interface::AquireVariableGlobals() {
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_ACQUIRE );
  scene_info  = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_ACQUIRE );
}

// ReleaseVariableGlobals():
FrameJumper_Interface::ReleaseVariableGlobals() {
  global( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
  global( LWSCENEINFO_GLOBAL,     GFUSE_RELEASE );

  ui         = NULL;
  scene_info = NULL;
}

// ReaquireVariableGlobals():
FrameJumper_Interface::ReaquireVariableGlobals() {
  ReleaseVariableGlobals();
  AquireVariableGlobals();
}

// CreateUI():
//  Creates the UI, but doesn't open it.  Called by the constructor.
void FrameJumper_Interface::CreateUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
    return_true_count = false;
  }

  char title_string[256] = "FrameJumper";
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return;
  }

  // Hide Disabled
  hide_disabled = BOOLBUTTON_CTL( panel_funcs, panel, "Hide Disabled" );
  GHOST_CON( hide_disabled );
  ERASE_CON( hide_disabled );

  // Time Units
  units         = WPOPUP_CTL( panel_funcs, panel, "Time As", time_unit_strings, 65 );
  PanelTools::PutOnLineWith( hide_disabled, units, 10 );
  SET_INT( units, units_state );

  // Group
//  group_popup = CUSTPOPUP_CTL( panel_funcs, panel, "Group", 170, GroupName, GroupCount );

  group_name  = STR_CTL( panel_funcs, panel, "Group", 29 );
  group_popup = CUSTPOPUP_CTL( panel_funcs, panel, "", 18, GroupName, GroupCount );
  PanelTools::PutOnLineWith( group_name, group_popup, -7 );

  // List
  list = MULTILIST_CTL( panel_funcs, panel, "Markers",
                        190, list_height, ListNames, ListCount, ListColumnWidths );
  SET_INT( list, fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetCurrentMarker() );

  // Modify
  modify = BOOLBUTTON_CTL( panel_funcs, panel, "Modify" );
  SET_INT( modify, modify_state );

  // Add/Remove
  add    = WBUTTON_CTL( panel_funcs, panel, "Add", 50 );
  remove = WBUTTON_CTL( panel_funcs, panel, "Remove", 50 );
  PanelTools::PutOnLineWith( modify, add );
  PanelTools::PutOnLineWith( add, remove );

  // About
  about = WBUTTON_CTL( panel_funcs, panel, "About", 50 );
  PanelTools::PutOnLineWith( remove, about );

  // Name
  name = STR_CTL( panel_funcs, panel, "Name", 20 );

  // Key
  key = WPOPUP_CTL( panel_funcs, panel, "Key", key_strings, 40 );

  // When
  time = FLOAT_CTL( panel_funcs, panel, "When" );

  // Enable
  enable = BOOL_CTL( panel_funcs, panel, "Enable" );

  // In Group
  in_group = CUSTPOPUP_CTL( panel_funcs, panel, "In Group", 140, InGroupName, InGroupCount );

  // Adjust Hidden Controls
  HandleModify( modify, NULL );

  // Set some panel callbacks
  (*panel_funcs->set)( panel, PAN_USERCLOSE,     HandlePanelClose    );
  (*panel_funcs->set)( panel, PAN_USEROPEN,      HandlePanelOpen     );
  (*panel_funcs->set)( panel, PAN_USERDRAW,      HandleDrawPanel     );
  (*panel_funcs->set)( panel, PAN_MOUSEBUTTON,   HandlePanelMouse    );
  (*panel_funcs->set)( panel, PAN_MOUSEMOVE,     HandlePanelMouse    );
  (*panel_funcs->set)( panel, PAN_USERKEYS,      HandleKeys          );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS,    HandleKeyUps        );
  (*panel_funcs->set)( panel, PAN_USERACTIVATE,  HandlePanelActivate );

  // Event Handlers
  CON_SETEVENT( units,                      HandleUnits,             NULL );
  CON_SETEVENT( group_popup,                HandleGroupPopup,        NULL );
  CON_SETEVENT( group_name,                 HandleGroupName,         NULL );

  CON_SETEVENT( list,                       HandleList,              NULL );
  CON_SETEVENT( about,                      HandleAbout,             NULL );
  CON_SETEVENT( modify,                     HandleModify,            NULL );

  CON_SETEVENT( add,                        HandleAdd,               NULL );
  CON_SETEVENT( remove,                     HandleRemove,            NULL );

  CON_SETEVENT( name,                       HandleName,              NULL );
  CON_SETEVENT( key,                        HandleKey,               NULL );
  CON_SETEVENT( time,                       HandleTime,              NULL );
  CON_SETEVENT( enable,                     HandleEnable,            NULL );
  CON_SETEVENT( in_group,                   HandleInGroup,           NULL );

  // Default Values
  SET_INT( group_popup, fj_data.GetCurrentGroup() );

  const char * name = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetName();
  if( name == NULL )
    SET_STR( group_name, "", 0 );
  else
    SET_STR( group_name, (char *)name, strlen( name ) );
}

// OpenUI():
void FrameJumper_Interface::OpenUI() {
  assert( fj_int != NULL );

  if( !fj_int->is_open ) {
    (*fj_int->panel_funcs->open)( fj_int->panel, PANF_MOUSETRAP | PANF_PASSALLKEYS );
    HandleModify( modify, NULL );
  }
}

// MultiList Callbacks
static char * list_names[]         = { "On", "When", "Name", "Key", NULL };
static int    list_column_widths[] = { 20, 50, 80, 20, 0 };

int ListCount( LWPanelID panel ) {
  if( fj_int->return_true_count == false )
    return 0;

  return fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().NumElements();
}

char * ListNames( LWPanelID panel, int item, int col ) {
  static char buffer[ 100 ];
  if( item == -1 )  // Column Headers
    return list_names[ col ];

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  if( item > (int)group->GetMarkers().NumElements() )
    return "";

  switch( col ) {
    case 0:         // Enabled
      return (char *)(group->GetMarkers()[ item ]->GetEnabled() ? fj_int->checkmark_string : "" );

    case 1: {       // When
      double time = group->GetMarkers()[ item ]->GetTime();
      int mode = GET_INT( fj_int->units, mode );
      if( mode == 1 ) {  // Convert from seconds to frames
        time *= fj_int->scene_info->framesPerSecond;
        time = ceil( time * 1000.0 )/1000.0;
      }

      if( fj_int->ui->generalFlags & LWGENF_FRACTIONALFRAME )
        sprintf( buffer, "%g", time );
      else
        sprintf( buffer, "%d", (int)(time + 0.5001) );

      return buffer;
    }

    case 2:         // Name
      return ((group->GetMarkers()[ item ]->GetName() == NULL) ? "" : (char *)group->GetMarkers()[ item ]->GetName());

    case 3:         // Key
      if( group->GetMarkers()[ item ]->GetKey() == 0 ) {
        return "";
      } else {
        sprintf( buffer, "%d", group->GetMarkers()[ item ]->GetKey() );
        return buffer;
      }
      break;

    default:
      return "";
  }
}

int ListColumnWidths( LWPanelID panel, int col ) {  // pixel width of column i, up to 8 columns terminate with 0
  return list_column_widths[ col ];
}

// Group Popup Callbacks
int GroupCount( void * data ) {
  return fj_data.GetGroups().NumElements() + 3;
}

char * GroupName( void * data, int index ) {
  int count = fj_data.GetGroups().NumElements();

  switch( (count - 1 - index) ) {
     case -1:              // Divider
       return " -- -- -- -- -- -- -- -- -- -- ";

     case -2:              // Create Group
       return "Create Group";

     case -3:              // Delete Group
       return "Delete Group";

    default:
      return ((fj_data.GetGroups()[ index ]->GetName() == NULL) ? "(unnamed)" : (char *)fj_data.GetGroups()[ index ]->GetName() );
  }
}

// In Group Popup Callbacks
int InGroupCount( void * data ) {
  return fj_data.GetGroups().NumElements();
}

char * InGroupName( void * data, int index ) {
  return ((fj_data.GetGroups()[ index ]->GetName() == NULL) ? "(unnamed)" : (char *)fj_data.GetGroups()[ index ]->GetName() );
}


// HandlePanelClose():
//  Called when the panel is closed by the user.
void HandlePanelClose( LWPanelID pan, void *data ) {
  if( fj_int != NULL )
    fj_int->is_open = false;

  fj_int->panel_x = PAN_GETX( fj_int->panel_funcs, fj_int->panel );
  fj_int->panel_y = PAN_GETY( fj_int->panel_funcs, fj_int->panel );

  GET_INT( fj_int->modify, fj_int->modify_state   );
  GET_INT( fj_int->units,  fj_int->units_state    );
  GET_INT( fj_int->list,   fj_int->current_marker );
}

// HandlePanelOpen():
//  Called when the panel is opened by the user.
void HandlePanelOpen( LWPanelID pan, void *data ) {
  if( fj_int != NULL )
    fj_int->is_open = true;

  if( fj_int->panel_x != -1 )
    MOVE_PAN( fj_int->panel_funcs, fj_int->panel, fj_int->panel_x, fj_int->panel_y );

  fj_int->return_true_count = true;
}

// HandlePanelActivate():
//  Called when the panel is opened by the user.
void HandlePanelActivate( LWPanelID pan, void *data ) {
  fj_int->ReaquireVariableGlobals();
}

// HandlePanelMouse():
//  Handles resizing the panel when the user clicks within the
//   resize area.
void HandlePanelMouse( LWPanelID, void *data, int flags, int x, int y ) {
  static bool was_down = false;
  static bool down_to_resize = false;
  static int  down_x, down_y;
  static int  down_pan_w, down_pan_h;
  static int  down_list_height;

  if( flags & MOUSE_DOWN ) {
    if( was_down == false ) {
      was_down = true;
      down_x = x;
      down_y = y;
      down_pan_w = PAN_GETW( fj_int->panel_funcs, fj_int->panel );
      down_pan_h = PAN_GETH( fj_int->panel_funcs, fj_int->panel );
      down_list_height = fj_int->list_height;

      if( (down_x > (down_pan_w - 20)) && (down_y > (down_pan_h - 20)) )
        down_to_resize = true;
    }
  } else {
    if( down_to_resize ) {
      (*fj_int->panel_funcs->close)( fj_int->panel );
      fj_int->CreateUI();
      fj_int->OpenUI();
    }

    was_down = false;
    down_to_resize = false;
  }

  if( was_down ) {
    if( down_to_resize ) {
      int new_h = (y - down_y + down_pan_h);

      int state;
      GET_INT( fj_int->modify, state );
      if( state == 0 ) {
        if( new_h < 170 )
          new_h = 170;
      } else {
        if( new_h < 247 )
          new_h = 247;
      }

      fj_int->list_height = down_list_height + ((y - down_y)/15);
      if( fj_int->list_height < 2 )
        fj_int->list_height = 2;

      // Resize the panel
//      PAN_SETW( fj_int->panel_funcs, fj_int->panel, (x - down_x + down_pan_w) );
      PAN_SETH( fj_int->panel_funcs, fj_int->panel, new_h );

/*
      // Move/Resize the affected controls
      ival.intv.value = down_list_height + (y-down_y);
      (*fj_int->list->set)( fj_int->list, CTL_H, &ival );

      PanelTools::PutUnder( fj_int->list, fj_int->modify );
      PanelTools::PutUnder( fj_int->list, fj_int->add    );
      PanelTools::PutUnder( fj_int->list, fj_int->remove );
      PanelTools::PutUnder( fj_int->list, fj_int->about  );

      HandleModify( fj_int->modify, NULL );
*/
    }
  }
}

// HandleDrawMainPanel() {
void HandleDrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( fj_int->panel_funcs->drawFuncs );

  int ph = PAN_GETH( fj_int->panel_funcs, fj_int->panel );
  int pw = PAN_GETW( fj_int->panel_funcs, fj_int->panel );

  // Horizontal Dividers
  int state;
  GET_INT( fj_int->modify, state );

  if( state == 1 ) {
    int y = CON_Y( fj_int->modify );
    int h = CON_H( fj_int->modify );
    PanelDraw::HorizontalDivider( fj_int->panel, &df, 0, y + h + 3, pw, 1 );
  }

  // Resize Button
  df.DrawLine( fj_int->panel, LWP_WHITE, pw-18, ph-2, pw-2, ph-18 );
  df.DrawLine( fj_int->panel, LWP_WHITE, pw-14, ph-2, pw-2, ph-14 );
  df.DrawLine( fj_int->panel, LWP_WHITE, pw-10, ph-2, pw-2, ph-10 );
  df.DrawLine( fj_int->panel, LWP_WHITE, pw-6,  ph-2, pw-2, ph-6  );

  df.DrawLine( fj_int->panel, LWP_BLACK, pw-17, ph-2, pw-2, ph-17 );
  df.DrawLine( fj_int->panel, LWP_BLACK, pw-13, ph-2, pw-2, ph-13 );
  df.DrawLine( fj_int->panel, LWP_BLACK, pw-9,  ph-2, pw-2, ph-9  );
  df.DrawLine( fj_int->panel, LWP_BLACK, pw-5,  ph-2, pw-2, ph-5  );
  }

// HandleKeys():
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  int state;

  switch( key ) {
    case LWDK_CTRL:             // Control
      fj_int->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      fj_int->shift_down = true;
      break;

    case 27:                    // Cancel (ESC)
      (*fj_int->panel_funcs->close)( fj_int->panel );
      break;

    case LWDK_SC_UP:
      if( fj_int->ctrl_down ) {           // Previous Group
        GET_INT( fj_int->group_popup, state );
        if( state > 0 ) {
          SET_INT( fj_int->group_popup, state-1 );
          HandleGroupPopup( fj_int->group_popup, NULL );
        }
      } else if( fj_int->shift_down ) {   // First Marker
        SET_INT( fj_int->list, 0 );
        HandleList( fj_int->list, (void *)-1 );
      } else {                            // Previous Marker
        GET_INT( fj_int->list, state );
        if( state > 0 ) {
          SET_INT( fj_int->list, state-1 );
          HandleList( fj_int->list, (void *)-1 );
        }
      }
      break;

    case LWDK_SC_DOWN:
      if( fj_int->ctrl_down ) {           // Next Group
        GET_INT( fj_int->group_popup, state );
        if( state < (int)(fj_data.GetGroups().NumElements() - 1) ) {
          SET_INT( fj_int->group_popup, state+1 );
          HandleGroupPopup( fj_int->group_popup, NULL );
        }
      } else if( fj_int->shift_down ) {   // Last Marker
        SET_INT( fj_int->list, fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().NumElements() - 1 );
        HandleList( fj_int->list, (void *)-1 );
      } else {                            // Next Marker
        GET_INT( fj_int->list, state );
        if( state < (int)(fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().NumElements() - 1) ) {
          SET_INT( fj_int->list, state+1 );
          HandleList( fj_int->list, (void *)-1 );
        }
      }
      break;

    case '=':
    case '+':                   // New Marker
      HandleAdd( fj_int->add, NULL );
      break;

    case '-':                   // Remove Marker
    case LWDK_DELETE:
      HandleRemove( fj_int->add, NULL );
      break;

    case '*':                   // New Group
      SET_INT( fj_int->group_popup, fj_data.GetGroups().NumElements() + 1 );
      HandleGroupPopup( fj_int->add, NULL );
      break;

    case '\r': {                // Jump To Marker
      char buffer[ 100 ];
      FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

      if( group->GetCurrentMarker() != -1 ) {
        sprintf( buffer, "GoToFrame %g", (group->GetMarkers()[ group->GetCurrentMarker() ]->GetTime() * fj_int->scene_info->framesPerSecond) );
        fj_data.SetCommand( buffer );
        (*fj_int->inst_update)( LWMASTER_HCLASS, &fj_data );
      }
    } break;

    case 9:                     // Activate First Field
      ACTIVATE_CON( fj_int->group_name );
      break;

    case 'm':                   // Toggle Modify Mode
    case 'M':
      GET_INT( fj_int->modify, state );
      SET_INT( fj_int->modify, ((state == 0) ? 1 : 0 ) );
      HandleModify( fj_int->modify, NULL );
      break;

    case 'n':                   // Activate Marker Name Field
    case 'N':
      ACTIVATE_CON( fj_int->name );
      break;

    case 's':                   // Display Time In Seconds
    case 'S':
      SET_INT( fj_int->units, 0 );
      HandleUnits( fj_int->units, NULL );
      break;

    case 'f':                   // Display Time In Frames
    case 'F':
      SET_INT( fj_int->units, 1 );
      HandleUnits( fj_int->units, NULL );
      break;

    case 'l':                   // Set Marker To Current Layout Time
    case 'L': {
      FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
      if( group->GetCurrentMarker() != -1 ) {
        FrameJumper_Marker *marker = group->GetMarkers()[ group->GetCurrentMarker() ];
        if( marker != NULL ) {
          marker->SetTime( fj_int->ui->curTime );
          HandleList( fj_int->list, NULL );
        }
      }
    } break;

    case 't':                   // Activate Time Field                   
    case 'T':
    case 'w':
    case 'W':
      if( !fj_int->ctrl_down )
        ACTIVATE_CON( fj_int->time );
      break;

    case ' ':                   // Toggle Marker Enable
      GET_INT( fj_int->enable, state );
      SET_INT( fj_int->enable, ((state == 0) ? 1 : 0 ) );
      HandleEnable( fj_int->enable, NULL );
      break;

    case LWDK_F1:               // Set/Get Marker 1
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 1 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 1 );
        HandleKey( fj_int->key, NULL );
      } else {                  // About
        HandleAbout( fj_int->about, NULL );
      }
      break;

    case LWDK_F2:               // Set/Get Marker 2
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 2 );
        if( marker == NULL )
          break;

        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 2 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F3:               // Set/Get Marker 3
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 3 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 3 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F4:               // Set/Get Marker 4
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 4 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 4 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F5:               // Set/Get Marker 5
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 5 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 5 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F6:               // Set/Get Marker 6
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 6 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 6 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F7:               // Set/Get Marker 7
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 7 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 7 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F8:               // Set/Get Marker 8
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 8 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 8 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F9:               // Set/Get Marker 9
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 9 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 9 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F10:               // Set/Get Marker 10
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 10 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 10 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F11:               // Set/Get Marker 11
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 11 );
        if( marker == NULL )
          break;
    
        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 11 );
        HandleKey( fj_int->key, NULL );
      }
      break;

    case LWDK_F12:               // Set/Get Marker 12
      if( fj_int->shift_down ) {
        FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( 12 );
        if( marker == NULL )
          break;

        FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
        int index = group->GetMarkers().FindIndexOf( marker );
        group->SetCurrentMarker( index );

        SET_INT( fj_int->list, index );
        HandleList( fj_int->list, (void *) -1 );
      } else if( fj_int->ctrl_down ) {
        SET_INT( fj_int->key, 12 );
        HandleKey( fj_int->key, NULL );
      }
      break;

  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  switch( key ) {
    case LWDK_CTRL:            // Control
      fj_int->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      fj_int->shift_down = false;
      break;
  }
}

// HandleUnits():
void HandleUnits( LWControl *con, void *data ) {
  HandleList( fj_int->list, (void *)-1 );
}

// HandleGroupPopup():
void HandleGroupPopup( LWControl *con, void *data ) {
  int index;
  GET_INT( fj_int->group_popup, index );

  int count = fj_data.GetGroups().NumElements();

  switch( (count - 1 - index) ) {
    case -1:              // Divider
      SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );
      break;

    case -2: {            // Create Group
      FrameJumper_Group * new_group = new FrameJumper_Group;
      fj_data.GetGroups().Add( new_group );
      fj_data.SetCurrentGroup( fj_data.GetGroups().NumElements() - 1 );
      SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );
      SET_STR( fj_int->group_name, "", 0 );

      SET_INT( fj_int->list, fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetCurrentMarker() );
      HandleList( fj_int->list, (void *)-1 );
    } break;


    case -3: {            // Delete Group
      delete fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
      fj_data.GetGroups().Remove( fj_data.GetCurrentGroup() );

      if( fj_data.GetGroups().NumElements() == 0 ) {
        fj_data.GetGroups().Add( new FrameJumper_Group );
        fj_data.GetGroups()[ 0 ]->SetName( "Default" );
        fj_data.SetCurrentGroup( 0 );
      } else {
        fj_data.SetCurrentGroup( (fj_data.GetCurrentGroup() == 0) ? 0 : (fj_data.GetCurrentGroup() - 1) );
      }

      SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );

      FrameJumper_Group * group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
      if( group->GetName() == NULL ) 
        SET_STR( fj_int->group_name, "", 0 );
      else
        SET_STR( fj_int->group_name, (char *)group->GetName(), strlen( group->GetName() ) );

      SET_INT( fj_int->list, fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetCurrentMarker() );
      HandleList( fj_int->list, (void *)-1 );
    } break;

    default: {
      fj_data.SetCurrentGroup( index );
      FrameJumper_Group * group = fj_data.GetGroups()[ index ];
      SET_INT( fj_int->group_popup, index );

      if( group->GetName() == NULL ) 
        SET_STR( fj_int->group_name, "", 0 );
      else
        SET_STR( fj_int->group_name, (char *)group->GetName(), strlen( group->GetName() ) );

      SET_INT( fj_int->list, fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetCurrentMarker() );
      HandleList( fj_int->list, (void *)-1 );
    }
  }
}

// HandleGroupName():
void HandleGroupName( LWControl *con, void *data ) {
  char buffer[ 512 ];
  GET_STR( fj_int->group_name, buffer, 512 );

  fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->SetName( buffer );
}

// HandleList():
void HandleList( LWControl *con, void *data ) {
  static __int64 prev_click_time = 0;
  static int     prev_index = -1;

  // Figure out which item was selected
  int index, state;
  GET_INT( fj_int->list, index );
  GET_INT( fj_int->modify, state );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  if( fj_data.GetGroups().NumElements() != 0 ) {
    // Get the current time to test for double clicks
    __int64 this_click_time;
    SYSTEMTIME sys_time;
    GetSystemTime( &sys_time );
    SystemTimeToFileTime( &sys_time, (LPFILETIME)&this_click_time );

    // Get the mouse coordinates
    int x = CON_MOUSEX( con );
    int y = CON_MOUSEY( con );

    if( index != -1 ) {
      group->SetCurrentMarker( index );

      if( data == NULL ) {  // Only check for clicks if data is NULL
        // See if the user clicked inside the control
        if( (y < CON_H( fj_int->list )) && (x < CON_W( fj_int->list )) ) {
          if( index < (int)group->GetMarkers().NumElements() ) {
            if( x < list_column_widths[0] + 8 ) {
              // Test for an Enable click
              group->GetMarkers()[ index ]->SetEnabled( !group->GetMarkers()[ index ]->GetEnabled() );
            } else {
              // Handle double-clicks (only if not in the Enable column)
              if( prev_index == index ) {
                if( (this_click_time - prev_click_time) < 5000000 ) {  // Time is in nanoseconds (1/2 second)
                  char buffer[ 100 ];
                  sprintf( buffer, "GoToFrame %g", (group->GetMarkers()[ index ]->GetTime() * fj_int->scene_info->framesPerSecond) );
                  fj_data.SetCommand( buffer );
                  (*fj_int->inst_update)( LWMASTER_HCLASS, &fj_data );
                }
              }
            }
          }
        }

        prev_click_time = this_click_time;
      }

      prev_index      = index;
    }
  }

  // Update Marker Details
  if( state == 1 ) {
    if( (index < 0) || (index >= (int)group->GetMarkers().NumElements()) ) {
      SET_STR(   fj_int->name,   "", 0 );
      SET_INT(   fj_int->key,    0     );
      SET_FLOAT( fj_int->time,   0     );
      SET_INT(   fj_int->enable, 0     );

      GHOST_CON( fj_int->name   );
      GHOST_CON( fj_int->key    );
      GHOST_CON( fj_int->time   );
      GHOST_CON( fj_int->enable );
      GHOST_CON( fj_int->in_group );
    } else {
      const char * name = (group->GetMarkers()[ index ]->GetName() == NULL) ? "" : group->GetMarkers()[ index ]->GetName();
      SET_STR( fj_int->name,     (char *)name, strlen( name ) );
      SET_INT( fj_int->key,      group->GetMarkers()[ index ]->GetKey() );
      SET_INT( fj_int->enable,   (group->GetMarkers()[ index ]->GetEnabled() ? 1 : 0 ) );
      SET_INT( fj_int->in_group, fj_data.GetCurrentGroup() );

      double time = group->GetMarkers()[ index ]->GetTime();
      int mode = GET_INT( fj_int->units, mode );
      if( mode == 1 )     // Convert from seconds to frames
        time *= fj_int->scene_info->framesPerSecond;

      SET_FLOAT( fj_int->time, time );

      UNGHOST_CON( fj_int->name   );
      UNGHOST_CON( fj_int->key    );
      UNGHOST_CON( fj_int->time   );
      UNGHOST_CON( fj_int->enable );
      UNGHOST_CON( fj_int->in_group );
    }
  }

  REDRAW_CON( con );
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( fj_int->panel_funcs, "FrameJumper", about_text, fj_int->panel, true, keys_text );
}

// HandleModify():
void HandleModify( LWControl *con, void *data ) {
  int state;
  GET_INT( con, state );

  int ph = 0;
  if( state == 0 ) {
    MOVE_CON( fj_int->name,     4, -100 );
    MOVE_CON( fj_int->key,      4, -100 );
    MOVE_CON( fj_int->time,     4, -100 );
    MOVE_CON( fj_int->enable,   4, -100 );
    MOVE_CON( fj_int->in_group, 4, -100 );

    GHOST_CON( fj_int->name   );
    GHOST_CON( fj_int->key    );
    GHOST_CON( fj_int->time   );
    GHOST_CON( fj_int->enable );
    GHOST_CON( fj_int->in_group );

    GHOST_CON( fj_int->add    );
    GHOST_CON( fj_int->remove );

    ERASE_CON( fj_int->name   );
    ERASE_CON( fj_int->key    );
    ERASE_CON( fj_int->time   );
    ERASE_CON( fj_int->enable );
    ERASE_CON( fj_int->in_group );

    ERASE_CON( fj_int->add    );
    ERASE_CON( fj_int->remove );

    ph  = CON_Y( fj_int->modify );
    ph += CON_H( fj_int->modify );
  } else {
    PanelTools::PutUnder( fj_int->modify, fj_int->name, 18 );
    PanelTools::PutOnLineWith( fj_int->name, fj_int->key, 3 );

    PanelTools::PutUnder( fj_int->name, fj_int->time );
    PanelTools::PutOnLineWith( fj_int->time, fj_int->enable );

    PanelTools::PutUnder( fj_int->time, fj_int->in_group );

    UNGHOST_CON( fj_int->name   );
    UNGHOST_CON( fj_int->key    );
    UNGHOST_CON( fj_int->time   );
    UNGHOST_CON( fj_int->enable );
    UNGHOST_CON( fj_int->in_group );

    UNGHOST_CON( fj_int->add    );
    UNGHOST_CON( fj_int->remove );

    ph  = CON_Y( fj_int->in_group );
    ph += CON_H( fj_int->in_group );
  }

  PAN_SETH( fj_int->panel_funcs, fj_int->panel, ph + 34 );

  if( fj_int->is_open ) {
    (*fj_int->panel_funcs->draw)( fj_int->panel, DR_REFRESH );
    HandleList( fj_int->list, (void *)-1 );
  }
}

// HandleAdd():
void HandleAdd( LWControl *con, void *data ) {
  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  FrameJumper_Marker *marker = new FrameJumper_Marker;
  group->GetMarkers().Add( marker );

  double now = fj_int->ui->curTime;

  marker->SetTime( now );
  marker->SetEnabled( true );

  fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().QSort( SortMarkersByTime );
  int new_index = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().FindIndexOf( marker );

  group->SetCurrentMarker( new_index );

  SET_INT( fj_int->list, group->GetCurrentMarker() );
  HandleList( fj_int->list, (void *)-1 );
}

// HandleRemove():
void HandleRemove( LWControl *con, void *data ) {
  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  int index;
  GET_INT( fj_int->list, index );
  if( index < 0 )
    return;

  if( group->GetMarkers().NumElements() == 0 )
    return;

  delete group->GetMarkers()[ index ];
  group->GetMarkers().Remove( index );

  group->SetCurrentMarker( (index > 0) ? index - 1 : index );
  SET_INT( fj_int->list, group->GetCurrentMarker() );
  HandleList( fj_int->list, (void *)-1 );
}

// HandleName():
void HandleName( LWControl *con, void *data ) {
  int index;
  GET_INT( fj_int->list, index );

  if( index == -1 )
    return;

  char new_name[ 512 ];
  GET_STR( fj_int->name, new_name, 512 );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  FrameJumper_Marker * marker = group->GetMarkers()[ index ];
  marker->SetName( new_name );

  HandleList( fj_int->list, (void *)-1 );
}

// HandleKey():
void HandleKey( LWControl *con, void *data ) {
  int index;
  GET_INT( fj_int->list, index );

  if( index == -1 )
    return;

  int key;
  GET_INT( fj_int->key, key );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  FrameJumper_Marker * marker = group->GetMarkers()[ index ];

  if( key != 0 ) {
    // Make sure no one else is using this key
    for( unsigned long i=0; i < group->GetMarkers().NumElements(); i++ ) {
      if( group->GetMarkers()[i]->GetKey() == key )
        group->GetMarkers()[i]->SetKey( 0 );
    }
  }

  marker->SetKey( key );
  HandleList( fj_int->list, (void *)-1 );
}

// HandleTime():
void HandleTime( LWControl *con, void *data ) {
  int index;
  GET_INT( fj_int->list, index );

  if( index == -1 )
    return;

  double time;
  GET_FLOAT( fj_int->time, time );

  int mode = GET_INT( fj_int->units, mode );
  if( mode == 1 )     // Convert from frames to seconds
    time /= fj_int->scene_info->framesPerSecond;

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  FrameJumper_Marker * marker = group->GetMarkers()[ index ];

  marker->SetTime( time );

  fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().QSort( SortMarkersByTime );
  int new_index = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().FindIndexOf( marker );

  group->SetCurrentMarker( new_index );

  SET_INT( fj_int->list, group->GetCurrentMarker() );
  HandleList( fj_int->list, (void *)-1 );
}

// HandleEnable():
void HandleEnable( LWControl *con, void *data ) {
  int index;
  GET_INT( fj_int->list, index );

  if( index == -1 )
    return;

  int enable;
  GET_INT( fj_int->enable, enable );

  FrameJumper_Group  *group  = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  FrameJumper_Marker *marker = group->GetMarkers()[ index ];

  marker->SetEnabled( (enable == 0) ? false : true );

  HandleList( fj_int->list, (void *)-1 );
}

// HandleInGroup():
void HandleInGroup( LWControl *con, void *data ) {
  int group_index, marker_index;
  GET_INT( fj_int->in_group, group_index  );
  GET_INT( fj_int->list,     marker_index );

  if( marker_index == -1 )
    return;

  FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers()[ marker_index ];
  FrameJumper_Group  *group  = fj_data.GetGroups()[ group_index ];  // New Group, NOT Current Group

  if( marker->GetKey() != 0 ) {
    // Make sure no one else is using this key
    for( unsigned long i=0; i < group->GetMarkers().NumElements(); i++ ) {
      if( group->GetMarkers()[i]->GetKey() == marker->GetKey() )
        group->GetMarkers()[i]->SetKey( 0 );
    }
  }

  fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->GetMarkers().Remove( marker_index );
  fj_data.GetGroups()[ group_index ]->GetMarkers().Add( marker );

  group->SetCurrentMarker( (marker_index > 0) ? marker_index - 1 : marker_index );
  SET_INT( fj_int->list, group->GetCurrentMarker() );
  HandleList( fj_int->list, (void *)-1 );
}

// SortMarkersByTime():
int (__cdecl SortMarkersByTime)( const void *arg1, const void *arg2 ) {
  FrameJumper_Marker *m1 = *((FrameJumper_Marker **)(arg1));
  FrameJumper_Marker *m2 = *((FrameJumper_Marker **)(arg2));

  if( m1->GetTime() == m2->GetTime() )
    return 0;

  if( m1->GetTime() > m2->GetTime() )
    return 1;

  return -1;
}

