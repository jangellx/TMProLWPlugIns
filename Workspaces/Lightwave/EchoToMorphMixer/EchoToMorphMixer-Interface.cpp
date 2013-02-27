//
// EchoToMorphMixer-Interface.cpp
//

#include "EchoToMorphMixer-Interface.h"

#include <stdio.h>

#include <portable/bfstream/bfstream.h>
#include <portable/pfstream/pfstream.h>

#include <lwdisplce.h>

#include <lightwave/lwaboutpanel/lwaboutpanel.h>

const unsigned long LWObjectChunk_FORM     = ('F'<<24) | ('O'<<16) | ('R'<<8) | 'M';
const unsigned long LWObjectChunk_LWO2     = ('L'<<24) | ('W'<<16) | ('O'<<8) | '2';
const unsigned long LWObjectChunk_VMAP     = ('V'<<24) | ('M'<<16) | ('A'<<8) | 'P';
const unsigned long LWObject_VMapType_MORF = ('M'<<24) | ('O'<<16) | ('R'<<8) | 'F';

static EchoToMM_Interface *eint = NULL;

int    EchoToMM_Interface::object_state                          = 0;
char   EchoToMM_Interface::viseme_freq_state[  MAX_PATH_LENGTH ] = "";
char   EchoToMM_Interface::project_freq_state[ MAX_PATH_LENGTH ] = "";
double EchoToMM_Interface::shift_state                           = 0.0;
double EchoToMM_Interface::scale_state                           = 1.0;
int    EchoToMM_Interface::clip_values_state                     = 0;

extern DynArray< EchoMapping * > mappings;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Echo To Morph Mixer",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Converts LIPSinc. Echo viseme sync output into",
  "Morph Mixer envelopes",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Foundation Imaging",
  "http://www.foundation-i.com",
  LWABOUT_VTAB,
  "Echo Copyright 1999-2001 LIPSinc.",
  LWABOUT_HDIV,
  "Written by Joe Angell",
  "jangell@tmproductions.com",
  "http://www.tmproductions.com",
  NULL };

const char *keys_text[] = {
  "Arrow Up/Down",        "Previous/Next Item",
  "Shift-Arrow Up/Down",  "First/Last Item",
  "Ctrl-Arrow Up/Down",   "Previous/Next Mapping",
  "*",                    "New Mapping",
  "=/+",                  "Add Synonym",
  "-",                    "Remove Synonym",
  "Enter/Return",         "Link",
  "Del",                  "Unlink",
  "Tab",                  "Activate First Field",
  "M",                    "Toggle Setup Mappings",
  "C",                    "Toggle Clip Values Below 0",
  "V",                    "Load Viseme Sync File",
  "U",                    "Toggle Synonym Usage",
  "L",                    "Load Project Mapping",
  "Shift-S",              "Save Project Mapping",
  "H",                    "Toggle Hide Linked",
  "Space/F",              "Find Linked",
  "T",                    "Test Links",
  "F1",                   "About/Keys",
  NULL };

const char *syn_columns[] = {
  "Use", "Object", "Full Path", NULL };
const int syn_column_widths[] = {
  25, 175, 150, 0 };

const char *endo_columns[] = {
  "Link", "Endomorph Target", NULL };
const int endo_column_widths[] = {
  25, 120, 0 };

const char *vis_columns[] = {
  "Link", "Viseme Identifier", NULL };
const int vis_column_widths[] = {
  25, 120, 0 };

const char *assigned_columns[] = {
  "Endomorph Target", "<- ->", "Viseme Identifier", NULL };
const int assigned_column_widths[] = {
  188, 24, 188, 0 };

// Constructor():
EchoToMM_Interface::EchoToMM_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), ctrl_down(false), shift_down(false),
    progress_panel(NULL), progress_total(1), progress_current(0),
    current_mapping(-1), current_synonym(-1), current_target(-1),
    current_viseme(-1), current_assigned(-1),
    last_clicked_list( ECHOLIST_ENDO ), last_clicked_linkable_list( ECHOLIST_TARGET ) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  iteminfo    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  objectinfo  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  objectfuncs = (LWObjectFuncs   *)global( LWOBJECTFUNCS_GLOBAL,   GFUSE_TRANSIENT );
  sceneinfo   = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );
  chaninfo    = (LWChannelInfo   *)global( LWCHANNELINFO_GLOBAL,   GFUSE_TRANSIENT );
  env_funcs   = (LWEnvelopeFuncs *)global( LWENVELOPEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  freq_func   = (LWFileReqFunc   *)global( LWFILEREQFUNC_GLOBAL,   GFUSE_TRANSIENT );

  eint = this;

  unsigned long prodinfo = (unsigned long) global( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
  unsigned long major    = LWINF_GETMAJOR( prodinfo );
  unsigned long minor    = LWINF_GETMINOR( prodinfo );

  if( ((major >= 6) && (minor >= 1)) && (major < 7) )
    checkmark_string = "\03(i:MIMG_CHECKMARK)";
  else
    checkmark_string = "Yes";

  BuildEndomorphList();
}

// Destructor():
EchoToMM_Interface::~EchoToMM_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  if( progress_panel != NULL )
    (*panel_funcs->destroy)( progress_panel );

  objects.Flush();

  eint = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool EchoToMM_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Echo To Morph Mixer -- %c2001 Foundation Imaging, written by Joe Angell", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Endomorph Object
  object_popup         = CUSTPOPUP_CTL( panel_funcs, panel, "Endomorph Object", 150, ObjectPopup_Names, ObjectPopup_Count );

  // Viseme File Requester
  viseme_freq          = LOAD_CTL( panel_funcs, panel, "Viseme Sync File", 58 );

  // Project Mapping File Requester
  project_mapping_freq = LOAD_CTL( panel_funcs, panel, "Project Mapping", 58 );

  // Shift Viseme Keys By
  shift_keys           = FLOAT_CTL( panel_funcs, panel, "Shift Viseme Keys By" );

  // Scale Viseme Values By
  scale_values         = FLOAT_CTL( panel_funcs, panel, "Scale Viseme Values By" );
  SET_FLOAT( scale_values, 1.0 );

  LWControl *aligner[] = { object_popup, viseme_freq, project_mapping_freq, shift_keys, scale_values, NULL };
  PanelTools::AlignLabels( aligner );

  // Clip values below 0
  clip_values          = BOOL_CTL( panel_funcs, panel, "Clip Values Below 0" );
  PanelTools::PutOnLineWith( scale_values, clip_values );

  // Setup
  setup                = BOOLBUTTON_CTL( panel_funcs, panel, "Setup Mappings" );
  PanelTools::PutOnLineWith( clip_values, setup );

  // About
  int w = CON_W( setup );
  about                = WBUTTON_CTL( panel_funcs, panel, "About", 50 );
  PanelTools::PutOnLineWith( object_popup, about );

  // - Setup Controls -

  // Mappings Popup
  mapping_name         = STR_CTL( panel_funcs, panel, "Current Mapping", 58 );
  PanelTools::AlignLabels( object_popup, mapping_name );
  PanelTools::PutUnder( setup, mapping_name, 20 );

  mapping_popup        = CUSTPOPUP_CTL( panel_funcs, panel, "", 18,
                                        MappingPopup_Names, MappingPopup_Count );

  // Synonym Objects
  synonym_list         = MULTILIST_CTL( panel_funcs, panel, "Synonym Objects",
                                        350, 3, SynNames, SynCount, SynColumnWidths );

  // - Add
  synonym_add          = LOADBUTTON_CTL( panel_funcs, panel, "Add", 60 );

  // - Remove
  synonym_remove       = WBUTTON_CTL( panel_funcs, panel, "Remove", 60 );


  // Assignment Lists

  // - Endomorphs
  endomorph_list       = MULTILIST_CTL( panel_funcs, panel, "Endomorph Targets",
                                        156, 10, EndoNames, EndoCount, EndoColumnWidths );

  // - Load New Endomorphs
  endomorph_reload     = FILEBUTTON_CTL( panel_funcs, panel, "Load New Endomorphs", 156 );

  // - Link
  link                 = WBUTTON_CTL( panel_funcs, panel, "<-   Link   ->", 73 );

  // - Viseme Identifier
  viseme_list          = MULTILIST_CTL( panel_funcs, panel, "Visime Identifier",
                                        156, 10, VisNames, VisCount, VisColumnWidths );

  // - Load New Sync File
  viseme_reload        = LOADBUTTON_CTL( panel_funcs, panel, "Load New Sync File", 156 );

  // - Hide Linked
  hide_linked          = BOOLBUTTON_CTL( panel_funcs, panel, "Hide Linked" );

  // - Find Link
  find_link            = WBUTTON_CTL( panel_funcs, panel, "Find Link", 73 );

  // - Unlink
  unlink               = WBUTTON_CTL( panel_funcs, panel, "Unlink", 73 );

  // - Clear Links
  clear_links          = WBUTTON_CTL( panel_funcs, panel, "Clear Links", 73 );

  // - Test
  test_links           = WBUTTON_CTL( panel_funcs, panel, "Test Links", 73 );

  // - Assigned Mappings
  assigned_list        = MULTILIST_CTL( panel_funcs, panel, "Assigned Mappings",
                                        410, 3, AssNames, AssCount, AssColumnWidths );

  // - Save Project Mapping
  save_project_mapping = SAVEBUTTON_CTL( panel_funcs, panel, "Save Project Mapping", 130 );

  // Right-align some controls
  int x  = CON_X( viseme_freq );
  int w2 = CON_W( viseme_freq );
  int y  = CON_Y( about );
      w  = CON_W( about );
  MOVE_CON( about, x + w2 - w, y );

  y = CON_Y( setup );
  w = CON_W( setup );
  MOVE_CON( setup, x + w2 - w, y );

  y = CON_Y( save_project_mapping );
  w = CON_W( save_project_mapping );
  MOVE_CON( save_project_mapping, x + w2 - w, y );

  // Initialize the controls
  if( object_state < (int)objects.NumElements() )
    SET_INT( object_popup,         object_state );
  else
    SET_INT( object_popup,         0 );

  SET_STR(   viseme_freq,          viseme_freq_state,  strlen( viseme_freq_state  ) );
  SET_STR(   project_mapping_freq, project_freq_state, strlen( project_freq_state ) );
  SET_FLOAT( shift_keys,           shift_state );
  SET_FLOAT( scale_values,         scale_state );
  SET_INT(   clip_values,          clip_values_state );

  if( project_freq_state[0] != '\0' )
    HandleProjectMappingFReq( eint->project_mapping_freq, NULL );

  // Update Ghosts and all that
  HandleSetup( setup, NULL );

  // Event Handlers
  CON_SETEVENT( about,                         HandleAbout,                      NULL );
  CON_SETEVENT( project_mapping_freq,          HandleProjectMappingFReq,         NULL );
  CON_SETEVENT( setup,                         HandleSetup,                      NULL );
  CON_SETEVENT( mapping_name,                  HandleMappingName,                NULL );
  CON_SETEVENT( mapping_popup,                 HandleMappingPopup,               NULL );
  CON_SETEVENT( synonym_list,                  HandleSynonymList,                NULL );
  CON_SETEVENT( synonym_add,                   HandleSynonymAdd,                 NULL );
  CON_SETEVENT( synonym_remove,                HandleSynonymRemove,              NULL );
  CON_SETEVENT( endomorph_list,                HandleEndomorphList,              NULL );
  CON_SETEVENT( endomorph_reload,              HandleEndomorphReload,            NULL );
  CON_SETEVENT( viseme_list,                   HandleVisemeList,                 NULL );
  CON_SETEVENT( viseme_reload,                 HandleVisemeReload,               NULL );
  CON_SETEVENT( assigned_list,                 HandleAssignedList,               NULL );
  CON_SETEVENT( link,                          HandleLink,                       NULL );
  CON_SETEVENT( unlink,                        HandleUnlink,                     NULL );
  CON_SETEVENT( find_link,                     HandleFindLink,                   NULL );
  CON_SETEVENT( hide_linked,                   HandleHideLinked,                 NULL );
  CON_SETEVENT( clear_links,                   HandleClearLinks,                 NULL );
  CON_SETEVENT( test_links,                    HandleTestLinks,                  NULL );
  CON_SETEVENT( save_project_mapping,          HandleSaveProjectMapping,         NULL );

  // Set some panel-level callbacks
  (*panel_funcs->set)( panel, PAN_USERDRAW,   DrawPanel    );
  (*panel_funcs->set)( panel, PAN_USERKEYS,   HandleKeys   );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS, HandleKeyUps );

  // Open the Panel repeatedly until the user has decided to cancel or processing
  //  completed successfully.
  do {
    if( (*panel_funcs->open)( panel, PANF_BLOCKING | PANF_CANCEL | PANF_PASSALLKEYS ) == 0 )
      break;

    if( Process() )
      break;
  } while( true );

  // Store some UI variables for later sessions
  GET_INT(   object_popup,         object_state );
  GET_STR(   viseme_freq,          viseme_freq_state,  MAX_PATH_LENGTH );
  GET_STR(   project_mapping_freq, project_freq_state, MAX_PATH_LENGTH );
  GET_FLOAT( shift_keys,           shift_state );
  GET_FLOAT( scale_values,         scale_state );
  GET_INT(   clip_values,          clip_values_state );
  
  return true;
}

// DrawMainPanel() {
void DrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( eint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( eint->panel_funcs, eint->panel );
  int pw = PAN_GETW( eint->panel_funcs, eint->panel );

  int state;
  GET_INT( eint->setup, state );

  if( state == 1 ) {
    // Horizontal Dividers
    int y = CON_Y( eint->setup );
    int h = CON_H( eint->setup );
    PanelDraw::HorizontalDivider( eint->panel, &df, 0, y + h + 3, pw, 1 );

    y = CON_Y( eint->synonym_list );
    h = CON_H( eint->synonym_list );
    PanelDraw::HorizontalDivider( eint->panel, &df, 20, y + h + 3, pw - 40, 1 );

    y = CON_Y( eint->mapping_name );
    h = CON_H( eint->mapping_name );
    PanelDraw::HorizontalDivider( eint->panel, &df, 20, y + h + 3, pw - 40, 1 );

    y = CON_Y( eint->assigned_list );
    h = CON_H( eint->assigned_list );
    PanelDraw::HorizontalDivider( eint->panel, &df, 20, y + h + 3, pw - 40, 1 );

    // Text
    y = CON_Y( eint->synonym_list );
    int x = CON_X( eint->synonym_list );
    int w = CON_W( eint->synonym_list );
    int tw = df.TextWidth( eint->panel, "Synonynm Objects" );
    df.WriteText( eint->panel, "Synonynm Objects", COLOR_BLACK, (x + (pw/2 - tw/2)), (y-12) );

    y = CON_Y( eint->endomorph_list );
    x = CON_X( eint->endomorph_list );
    w = CON_W( eint->endomorph_list );
    tw = df.TextWidth( eint->panel, "Endomorph Targets" );
    df.WriteText( eint->panel, "Endomorph Targets", COLOR_BLACK, (x + (w/2 - tw/2)), (y-12) );

    y = CON_Y( eint->viseme_list );
    x = CON_X( eint->viseme_list );
    w = CON_W( eint->viseme_list );
    tw = df.TextWidth( eint->panel, "Viseme Identifiers" );
    df.WriteText( eint->panel, "Viseme Identifiers", COLOR_BLACK, (x + (w/2 - tw/2)), (y-12) );

    y = CON_Y( eint->assigned_list );
    x = CON_X( eint->assigned_list );
    w = CON_W( eint->assigned_list );
    tw = df.TextWidth( eint->panel, "Assigned Targets and Identifiers" );
    df.WriteText( eint->panel, "Assigned Targets and Identifiers", COLOR_BLACK, (x + (w/2 - tw/2)), (y-12) );
  }
}

//
// -- Callbacks Handlers ---------------------------------------------------------
//

// ObjectPopup_Names():
char * ObjectPopup_Names( void * data, int index ) {
  if( index < 0 )
    return "";

  if( index < (int)eint->objects.NumElements() ) {
    const char * name = eint->objects[ index ]->GetName();
    return (char *)((name == NULL) ? "(unknown)" : name );
  }

  return NULL;
}

// ObjectPopup_Count():
int ObjectPopup_Count( void * data ) {
  return eint->objects.NumElements();
}

// MappingPopup_Names():
char * MappingPopup_Names( void * data, int index ) {
  if( index < 0 )
    return NULL;

  if( index < (int)mappings.NumElements() + 6 ) {
    if( index < (int)mappings.NumElements() ) {
      const char * name = mappings[ index ]->GetName();
      return (char *)((name == NULL) ? "(unnamed)" : name );
    }

    switch( index - mappings.NumElements() ) {
      case 0:
        return " --- --- --- --- --- --- --- --- ";

      case 1:
        return "New Mapping";

      case 2:
        return "Clone Mapping";
      
      case 3:
        return "Delete Mapping";

      case 4:
        return " --- --- --- --- --- --- --- --- ";

      case 5:
        return "New Project";
    }
  }

  return NULL;
}

// MappingPopup_Count():
int MappingPopup_Count( void * data ) {
  return mappings.NumElements() + 6;
}

//
// --- Multilist Handlers --------------------
// 
int SynCount( LWPanelID panel ) {
  if( eint->current_mapping == -1 )
    return 0;

  return mappings[ eint->current_mapping ]->GetSynonyms().NumElements();
}

char * SynNames( LWPanelID panel, int item, int col ) {
  if( item == -1 )  // Column Headers
    return (char*)syn_columns[ col ];

  if( eint->current_mapping == -1 )
    return "";

  EchoMapping *map = mappings[ eint->current_mapping ];
  if( item >= (int)map->GetSynonyms().NumElements() )
    return "";

  switch( col ) {
    case 0:         // Use
      return (map->GetSynonyms()[ item ]->GetIsEnabled() ? (char *)eint->checkmark_string : "");

    case 1:         // Filename
      return ((map->GetSynonyms()[ item ]->GetFilename() == NULL) ? "(none)" : (char *)map->GetSynonyms()[ item ]->GetFilename());

    case 2:         // Full Path
      return ((map->GetSynonyms()[ item ]->GetFullPath() == NULL) ? "(none)" : (char *)map->GetSynonyms()[ item ]->GetFullPath());

    default:
      return "";
  }
}

int SynColumnWidths( LWPanelID panel, int col ) {
  return syn_column_widths[ col ];
}


int EndoCount( LWPanelID panel ) {
  if( eint->current_mapping == -1 )
    return 0;

  int hide_linked;
  GET_INT( eint->hide_linked, hide_linked );
  if( !hide_linked )
    return mappings[ eint->current_mapping ]->GetTargets().NumElements();

  EchoMapping *map = mappings[ eint->current_mapping ];
  int index = 0;
  for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
    if( !map->GetTargets()[i]->IsLinked() )
      index++;
  }

  return index;
}

char * EndoNames( LWPanelID panel, int item, int col ) {
  static char buffer[10];

  if( item == -1 )  // Column Headers
    return (char*)endo_columns[ col ];

  if( eint->current_mapping == -1 )
    return "";

  int hide_linked;
  GET_INT( eint->hide_linked, hide_linked );
  if( hide_linked )
    item = eint->FindLinkedEndomorphAtIndex( item, false );

  EchoMapping *map = mappings[ eint->current_mapping ];
  if( item >= (int)map->GetTargets().NumElements() )
    return "";

  switch( col ) {
    case 0: {       // Linked
      if( map->GetTargets()[ item ]->IsLinked() ) {
        sprintf( buffer, "%d", map->GetTargets()[ item ]->GetLinkedTo() );
        return buffer;
      } else {
        return "";
      }
    }

    case 1:         // Target
      return ((map->GetTargets()[ item ]->GetName() == NULL) ? "(none)" : (char *)map->GetTargets()[ item ]->GetName());

    default:
      return "";
  }
}

int EndoColumnWidths( LWPanelID panel, int col ) {
  return endo_column_widths[ col ];
}



int VisCount( LWPanelID panel ) {
  if( eint->current_mapping == -1 )
    return 0;

  int hide_linked;
  GET_INT( eint->hide_linked, hide_linked );
  if( !hide_linked )
    return mappings[ eint->current_mapping ]->GetVisemes().NumElements();

  EchoMapping *map = mappings[ eint->current_mapping ];
  int index = 0;
  for( unsigned long i=0; i < map->GetVisemes().NumElements(); i++ ) {
    if( !map->GetVisemes()[i]->IsLinked() )
      index++;
  }

  return index;
}

char * VisNames( LWPanelID panel, int item, int col ) {
  static char buffer[10];

  if( item == -1 )  // Column Headers
    return (char*)vis_columns[ col ];

  if( eint->current_mapping == -1 )
    return "";

  int hide_linked;
  GET_INT( eint->hide_linked, hide_linked );
  if( hide_linked )
    item = eint->FindLinkedVisemeAtIndex( item, false );

  EchoMapping *map = mappings[ eint->current_mapping ];
  if( item >= (int)map->GetVisemes().NumElements() )
    return "";

  switch( col ) {
    case 0: {       // Linked
      if( map->GetVisemes()[ item ]->IsLinked() ) {
        sprintf( buffer, "%d", map->GetVisemes()[ item ]->GetLinkedTo() );
        return buffer;
      } else {
        return "";
      }
    }

    case 1:         // Viseme
      return ((map->GetVisemes()[ item ]->GetName() == NULL) ? "(none)" : (char *)map->GetVisemes()[ item ]->GetName());

    default:
      return "";
  }
}

int VisColumnWidths( LWPanelID panel, int col ) {
  return vis_column_widths[ col ];
}



int AssCount( LWPanelID panel ) {
  if( eint->current_mapping == -1 )
    return 0;

  unsigned long count = 0;
  EchoMapping *map = mappings[ eint->current_mapping ];
  for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
    if( map->GetTargets()[i]->IsLinked() )
      count++;
  }

  return count;
}

char * AssNames( LWPanelID panel, int item, int col ) {
  if( item == -1 )  // Column Headers
    return (char *)assigned_columns[ col ];

  if( eint->current_mapping == -1 )
    return "";

  EchoMapping *map = mappings[ eint->current_mapping ];
  if( item >= (int)map->GetVisemes().NumElements() )
    return "";

  // Figure what item this should be
  int index = -1;
  for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
    if( map->GetTargets()[i]->IsLinked() ) {
      if( ++index == item )
        break;
    }
  }

  if( index < item )
    return "";

  switch( col ) {
    case 0:         // Target
      return ((map->GetTargets()[ i ]->GetName() == NULL) ? "(none)" : (char *)map->GetTargets()[ i ]->GetName());

    case 1:
      return (char *)assigned_columns[ 1 ];

    case 2:         // Viseme
      return ((map->GetVisemes()[ map->GetTargets()[ i ]->GetLinkedTo() ]->GetName() == NULL) ? "(none)" : (char *)map->GetVisemes()[ map->GetTargets()[ i ]->GetLinkedTo() ]->GetName());

    default:
      return "";
  }
}

int AssColumnWidths( LWPanelID panel, int col ) {
  return assigned_column_widths[ col ];
}

//
// -- Event Handlers ---------------------------------------------------------
//

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  FI_Logo logo;
  LWAboutPanel( eint->panel_funcs, "Echo To Morph Mixer", about_text, &logo, eint->panel, true, keys_text );
}

// HandleSynonymList():
void HandleSynonymList( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  int index, column, other ;
  GET_IVEC( eint->synonym_list, index, column, other );
  if( index != -1 ) {
    if( column == 0 ) {
      EchoMapping *map = mappings[ eint->current_mapping ];
      map->GetSynonyms()[ index ]->SetIsEnabled( !map->GetSynonyms()[ index ]->GetIsEnabled() );
      REDRAW_CON( eint->synonym_list );
    }
  }

  eint->last_clicked_linkable_list = ECHOLIST_SYNONYM;
}

// HandleEndomorphList():
void HandleEndomorphList( LWControl *con, void *data ) {
  eint->last_clicked_list          = ECHOLIST_TARGET;
  eint->last_clicked_linkable_list = ECHOLIST_TARGET;
}

// HandleVisemeList():
void HandleVisemeList( LWControl *con, void *data ) {
  eint->last_clicked_list          = ECHOLIST_VISEME;
  eint->last_clicked_linkable_list = ECHOLIST_VISEME;
}

// HandleAssignedList():
void HandleAssignedList( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  int item;
  GET_INT( eint->assigned_list, item );

  EchoMapping *map = mappings[ eint->current_mapping ];

  int index = eint->FindLinkedEndomorphAtIndex( item, true );
  if( item == -1 )
    return;

  SET_INT( eint->endomorph_list, index );
  SET_INT( eint->viseme_list, map->GetTargets()[ index ]->GetLinkedTo() );

  eint->last_clicked_list = ECHOLIST_ASSIGNED;
}

// HandleFindLink():
void HandleFindLink( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );
  EchoMapping *map = mappings[ eint->current_mapping ];

  int index, target;
  unsigned long link = -1;

  if( eint->last_clicked_linkable_list == ECHOLIST_TARGET ) {
    // Find Endomorph's Link
    GET_INT( eint->endomorph_list, index );
  
    if( index == -1 )
      return;

    link = map->GetTargets()[ index ]->GetLinkedTo();
    if( link != -1 ) {
      SET_INT( eint->viseme_list, link );
      target = index;
    }
  } else {
    // Find Vieseme's Link
    GET_INT( eint->viseme_list, index );
    if( index == -1 )
      return;

    link = map->GetVisemes()[ index ]->GetLinkedTo();
    if( link != -1 ) {
      SET_INT( eint->endomorph_list, link );
      target = link;
    }
  }

  // Update the Assigned List
  if( link != -1 ) {
    index = -1;
    for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
      if( map->GetTargets()[i]->IsLinked() ) {
        index++;
        if( i == (unsigned long)target )
          break;
      }
    }

    SET_INT( eint->assigned_list, index );
  }
}

//  HandleUnlink():
void HandleUnlink( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );
  EchoMapping *map = mappings[ eint->current_mapping ];

  int index, hide_linked;
  unsigned long link = -1;

  GET_INT( eint->hide_linked, hide_linked );

  if( eint->last_clicked_list == ECHOLIST_ASSIGNED ) {
    GET_INT( eint->assigned_list, index );
    index = eint->FindLinkedEndomorphAtIndex( index, true );

    if( index != -1 )
      link = map->GetTargets()[ index ]->GetLinkedTo();

    if( link != -1 ) {
      map->GetTargets()[ index ]->SetLinkedTo( -1 );
      map->GetVisemes()[ link ]->SetLinkedTo( -1 );
    }

  } else if( eint->last_clicked_linkable_list == ECHOLIST_TARGET ) {
    if( hide_linked )
      return;

    GET_INT( eint->endomorph_list, index );
    if( index != -1 )
      link = map->GetTargets()[ index ]->GetLinkedTo();

    if( link != -1 ) {
      map->GetTargets()[ index ]->SetLinkedTo( -1 );
      map->GetVisemes()[ link ]->SetLinkedTo( -1 );
    }
  } else {
    if( hide_linked )
      return;

    GET_INT( eint->viseme_list, index );
    if( index != -1 )
      link = map->GetVisemes()[ index ]->GetLinkedTo();

    if( link != -1 ) {
      map->GetTargets()[ link ]->SetLinkedTo( -1 );
      map->GetVisemes()[ index ]->SetLinkedTo( -1 );
    }
  }

  eint->UpdateWithCurrentMapping();
}

// HandleHideLinked():
void HandleHideLinked( LWControl *con, void *data ) {
  eint->UpdateWithCurrentMapping();
}

// FindLinkedEndomorphAtIndex():
//  Finds the item at the selected index, skipping all unlinked items.
//   Returns -1 if no items are linked.  The linked bool toggles this,
//   returning only unlinked objects.
int EchoToMM_Interface::FindLinkedEndomorphAtIndex( int _index, bool linked ) {
  assert( eint->current_mapping != -1 );

  if( _index == -1 )
    return -1;

  EchoMapping *map = mappings[ eint->current_mapping ];

  int index = -1;
  for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
    if( linked ) {
      if( map->GetTargets()[i]->IsLinked() )
        index++;
    } else {
      if( !map->GetTargets()[i]->IsLinked() )
        index++;
    }

    if( index == _index )
      return i;
  }

  return -1;
}

// FindLinkedVisemeAtIndex():
//  Finds the item at the selected index, skipping all unlinked items.
//   Returns -1 if no items are linked.  The linked bool toggles this,
//   returning only unlinked objects.
int EchoToMM_Interface::FindLinkedVisemeAtIndex( int _index, bool linked ) {
  assert( eint->current_mapping != -1 );

  if( _index == -1 )
    return -1;

  EchoMapping *map = mappings[ eint->current_mapping ];

  int index = -1;
  for( unsigned long i=0; i < map->GetVisemes().NumElements(); i++ ) {
    if( linked ) {
      if( map->GetVisemes()[i]->IsLinked() )
        index++;
    } else {
      if( !map->GetVisemes()[i]->IsLinked() )
        index++;
    }

    if( index == _index )
      return i;
  }

  return -1;
}

// BuildEndomorphList():
//  Builds a list of objects using endomorphs.
int EchoToMM_Interface::BuildEndomorphList() {
  EchoVMapScanData data;
  int num_vmaps = (*objectfuncs->numVMaps)( LWVMAP_MORF );
  for( int i=0; i < num_vmaps; i++ )
    data.vmap_names.Add( StringTools::strdup( (*objectfuncs->vmapName)( LWVMAP_MORF, i ) ) );

  for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
    data.item = item;
    data.mesh = (*objectinfo->meshInfo)( item, 0 );
    if( data.mesh != NULL )
      (*data.mesh->scanPoints)( data.mesh, ScanPointsForEndomorphs, (void *)&data );
  }

  data.vmap_names.Flush();
  return objects.NumElements();
}

// ScanPointsForEndomorphs():
int ScanPointsForEndomorphs( void *_data, LWPntID point ) {
  EchoVMapScanData &data = *(EchoVMapScanData *)_data;

  void  * vmap_id;
  float   val[256];

  for( unsigned long i=0; i < data.vmap_names.NumElements(); i++ ) {
    vmap_id = data.mesh->pntVLookup( data.mesh, LWVMAP_MORF, eint->objectfuncs->vmapName( LWVMAP_MORF, i ) );
    if( vmap_id != NULL ) {
      (*data.mesh->pntVSelect)( data.mesh, vmap_id );
      if( (*data.mesh->pntVGet)( data.mesh, point, val ) != 0 ) {
        // Endomorph found; add the object and exit
        EchoLWItem *new_item = new EchoLWItem( data.item, (*eint->iteminfo->name)( data.item ) );
        eint->objects.Add( new_item );
        return 1;
      }
    }
  }

  // No Endomorphs; keep looking
  return 0;
}

// HandleSetup():
//  Toggles the Setup options' visibility.
void HandleSetup( LWControl *con, void *data ) {
  int state;
  GET_INT( eint->setup, state );

  int x, y, w, w2, h;

  if( state == 0 ) {
    // Hide Controls
    GHOST_CON( eint->mapping_name );
    ERASE_CON( eint->mapping_name );
    MOVE_CON(  eint->mapping_name, 5, -1000 );

    GHOST_CON( eint->mapping_popup );
    ERASE_CON( eint->mapping_popup );
    MOVE_CON(  eint->mapping_popup, 5, -1000 );

    GHOST_CON( eint->synonym_list );
    ERASE_CON( eint->synonym_list );
    MOVE_CON(  eint->synonym_list, 5, -1000 );

    GHOST_CON( eint->synonym_add );
    ERASE_CON( eint->synonym_add );
    MOVE_CON(  eint->synonym_add, 5, -1000 );

    GHOST_CON( eint->synonym_remove );
    ERASE_CON( eint->synonym_remove );
    MOVE_CON(  eint->synonym_remove, 5, -1000 );

    GHOST_CON( eint->endomorph_list );
    ERASE_CON( eint->endomorph_list );
    MOVE_CON(  eint->endomorph_list, 5, -1000 );

    GHOST_CON( eint->endomorph_reload );
    ERASE_CON( eint->endomorph_reload );
    MOVE_CON(  eint->endomorph_reload, 5, -1000 );

    GHOST_CON( eint->link );
    ERASE_CON( eint->link );
    MOVE_CON(  eint->link, 5, -1000 );

    GHOST_CON( eint->hide_linked );
    ERASE_CON( eint->hide_linked );
    MOVE_CON(  eint->hide_linked, 5, -1000 );

    GHOST_CON( eint->find_link );
    ERASE_CON( eint->find_link );
    MOVE_CON(  eint->find_link, 5, -1000 );

    GHOST_CON( eint->unlink );
    ERASE_CON( eint->unlink );
    MOVE_CON(  eint->unlink, 5, -1000 );

    GHOST_CON( eint->clear_links );
    ERASE_CON( eint->clear_links );
    MOVE_CON(  eint->clear_links, 5, -1000 );

    GHOST_CON( eint->test_links );
    ERASE_CON( eint->test_links );
    MOVE_CON(  eint->test_links, 5, -1000 );

    GHOST_CON( eint->viseme_list );
    ERASE_CON( eint->viseme_list );
    MOVE_CON(  eint->viseme_list, 5, -1000 );

    GHOST_CON( eint->viseme_reload );
    ERASE_CON( eint->viseme_reload );
    MOVE_CON(  eint->viseme_reload, 5, -1000 );

    GHOST_CON( eint->assigned_list );
    ERASE_CON( eint->assigned_list );
    MOVE_CON(  eint->assigned_list, 5, -1000 );

    GHOST_CON( eint->save_project_mapping );
    ERASE_CON( eint->save_project_mapping );
    MOVE_CON(  eint->save_project_mapping, 5, -1000 );

    y = CON_Y( eint->setup );
    h = CON_H( eint->setup );
  } else {
    // Show Controls
    UNGHOST_CON( eint->mapping_name );
    UNGHOST_CON( eint->mapping_popup );
    UNGHOST_CON( eint->synonym_list );
    UNGHOST_CON( eint->synonym_add );
    UNGHOST_CON( eint->synonym_remove );
    UNGHOST_CON( eint->endomorph_list );
    UNGHOST_CON( eint->endomorph_reload );
    UNGHOST_CON( eint->link );
    UNGHOST_CON( eint->hide_linked );
    UNGHOST_CON( eint->find_link );
    UNGHOST_CON( eint->unlink );
    UNGHOST_CON( eint->clear_links );
    UNGHOST_CON( eint->test_links );
    UNGHOST_CON( eint->viseme_list );
    UNGHOST_CON( eint->viseme_reload );
    UNGHOST_CON( eint->assigned_list );
    UNGHOST_CON( eint->save_project_mapping );

    PanelTools::AlignLabels( eint->object_popup, eint->mapping_name );
    PanelTools::PutUnder( eint->setup, eint->mapping_name, 20 );

    PanelTools::PutOnLineWith( eint->mapping_name, eint->mapping_popup, -10 );

    PanelTools::PutUnder( eint->mapping_name, eint->synonym_list, 33 );
    PanelTools::PutOnLineWith( eint->synonym_list, eint->synonym_add );
    PanelTools::AlignLabels( eint->synonym_add, eint->synonym_remove );

    PanelTools::PutUnder( eint->synonym_list, eint->endomorph_list, 33 );
    PanelTools::PutOnLineWith( eint->endomorph_list, eint->link );
    PanelTools::PutOnLineWith( eint->link, eint->viseme_list );
    PanelTools::AlignLabels( eint->endomorph_list, eint->endomorph_reload );

    x = CON_X( eint->link );
    y = CON_Y( eint->link );
    MOVE_CON( eint->link, x, y + 15 );

    PanelTools::AlignLabels( eint->link, eint->hide_linked, 20 );
    PanelTools::AlignLabels( eint->hide_linked, eint->find_link );
    PanelTools::AlignLabels( eint->find_link, eint->unlink, 20 );
    PanelTools::AlignLabels( eint->unlink, eint->clear_links );
    PanelTools::AlignLabels( eint->clear_links, eint->test_links, 20 );

    PanelTools::PutUnder( eint->endomorph_reload, eint->assigned_list, 23 );
    PanelTools::AlignLabels( eint->viseme_list, eint->viseme_reload );

    PanelTools::PutUnder( eint->assigned_list, eint->save_project_mapping, 20 );

    x  = CON_X( eint->viseme_freq );
    y  = CON_Y( eint->save_project_mapping );
    w  = CON_W( eint->save_project_mapping );
    w2 = CON_W( eint->viseme_freq );

    MOVE_CON( eint->save_project_mapping, x + w2 - w, y );

    y = CON_Y( eint->save_project_mapping );
    h = CON_H( eint->save_project_mapping );
  }

  h = y + h + 40;
  PAN_SETH( eint->panel_funcs, eint->panel, h );
  (*eint->panel_funcs->draw)( eint->panel, DR_REFRESH );

  eint->UpdateWithCurrentMapping();
}

// UpdateWithCurrentMapping():
//  Updates the interface with the values from the current mapping,
//   if enabled.  Otherwise it will ghost the controls.  Unless setup
//   is off, in which case it does nothing at all.
void EchoToMM_Interface::UpdateWithCurrentMapping() {
  int state;
  GET_INT( eint->setup, state );
  if( setup == 0 )
    return;

  if( eint->current_mapping == -1 ) {
    GHOST_CON( synonym_list );
    GHOST_CON( synonym_add );
    GHOST_CON( synonym_remove );
    GHOST_CON( endomorph_list );
    GHOST_CON( endomorph_reload );
    GHOST_CON( link );
    GHOST_CON( hide_linked );
    GHOST_CON( unlink );
    GHOST_CON( clear_links );
    GHOST_CON( test_links );
    GHOST_CON( viseme_list );
    GHOST_CON( viseme_reload );
    GHOST_CON( assigned_list );

    SET_STR( eint->mapping_name, "", 0 );
  } else {
    UNGHOST_CON( synonym_list );
    UNGHOST_CON( synonym_add );
    UNGHOST_CON( synonym_remove );
    UNGHOST_CON( endomorph_list );
    UNGHOST_CON( endomorph_reload );
    UNGHOST_CON( link );
    UNGHOST_CON( hide_linked );
    UNGHOST_CON( unlink );
    UNGHOST_CON( clear_links );
    UNGHOST_CON( test_links );
    UNGHOST_CON( viseme_list );
    UNGHOST_CON( viseme_reload );
    UNGHOST_CON( assigned_list );

    // Mapping Name
    EchoMapping *map = mappings[ eint->current_mapping ];
    if( map->GetName() == NULL ) {
      SET_STR( eint->mapping_name, "(unnamed)", 9 );
    } else {
      SET_STR( eint->mapping_name, (char *)map->GetName(), strlen( map->GetName() ) );
    }
  }

  int state2;
  GET_INT( eint->hide_linked, state2 );
  if( (state == 1) && (state2 == 0) && (eint->current_mapping != -1) )
    UNGHOST_CON( find_link );
  else
    GHOST_CON( find_link );

  SET_INT( synonym_list,   current_synonym  );
  SET_INT( endomorph_list, current_target   );
  SET_INT( viseme_list,    current_viseme   );
  SET_INT( assigned_list,  current_assigned );

  REDRAW_CON( synonym_list );
  REDRAW_CON( endomorph_list );
  REDRAW_CON( viseme_list );
  REDRAW_CON( assigned_list );
}

// HandleMappingPopup():
//  Toggles the Setup options' visibility.
void HandleMappingPopup( LWControl *con, void *data ) {
  int state;
  GET_INT( eint->mapping_popup, state );

  // See of the user clicked on the seperator
  if( (state == (int)mappings.NumElements() + 4) || 
      (state == (int)mappings.NumElements() ) ) {
    SET_INT( eint->mapping_popup, eint->current_mapping );
    return;
  }

  // New Mapping
  if( state == (int)mappings.NumElements() + 1 ) {
    eint->current_mapping = mappings.NumElements();
    mappings.Add( new EchoMapping );
  } else if( state == (int)mappings.NumElements() + 2 ) {
    // Clone Mapping
    EchoMapping *map = mappings[ eint->current_mapping ];
    EchoMapping *new_map = new EchoMapping;
    *new_map = *map;

    char buffer[ 512 ] = "";
    if( new_map->GetName() != NULL )
      strcpy( buffer, new_map->GetName() );

    strcat( buffer, " (Clone)" );
    new_map->SetName( buffer );

    eint->current_mapping = mappings.NumElements();
    mappings.Add( new_map );
  } else if( state == (int)mappings.NumElements() + 3 ) {
    // Delete Mapping
    if( eint->current_mapping != -1 ) {
      if( (*eint->message->yesNo)( "Delete Mapping?", "Do you really want to delete this mapping, clearing", NULL ) == 1 ) {
        delete mappings[ eint->current_mapping ];
        mappings.Remove( eint->current_mapping );
        if( eint->current_mapping >= (int)mappings.NumElements() )
          eint->current_mapping--;
      }
    }
  } else if( state == (int)mappings.NumElements() + 5 ) {
    if( mappings.NumElements() > 0 ) {
      if( (*eint->message->yesNo)( "New Project?", "Do you want to create a new project, clearing",
                                   "all of your existing mappings from memory?" ) == 1 ) {
        mappings.Flush();
        eint->current_mapping = -1;
      }
    }
  } else {
    eint->current_mapping = state;
  }

  SET_INT( eint->mapping_popup, ((eint->current_mapping == -1) ? 0 : eint->current_mapping) );
  eint->UpdateWithCurrentMapping();
}

// HandleMappingName():
//  Handles renaming the selected map.
void HandleMappingName( LWControl *con, void *data ) {
  int state;
  GET_INT( eint->mapping_name, state );

  if( eint->current_mapping == -1 ) {
    SET_STR( eint->mapping_name, "", 0 );
    (*eint->message->error)( "Echo To Morph Mixer Error:  You must first select a mapping to",
                             "rename via the pop-up to the right of the Current Mapping field." );
    return;
  }

  EchoMapping *map = mappings[ eint->current_mapping ];
  char buffer[ 1024 ];

  GET_STR( eint->mapping_name, buffer, 1024 );
  map->SetName( buffer );
}

// HandleEndomorphReload():
//  Lost the endomorphs from the object into the target list.
void HandleEndomorphReload( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  char path[ MAX_PATH_LENGTH ];
  GET_STR( eint->endomorph_reload, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  // See if there are any endomorphs and warn the user about clearing the links
  EchoMapping *map = mappings[ eint->current_mapping ];
  if( map->GetTargets().NumElements() > 0 ) {
    if( (*eint->message->okCancel)( "Load New Endomorphs?",
                                    "Echo To Morph Mixer: Are you sure you want to load new endomorphs?",
                                    "This will remove any existing links in this mapping." ) == 0 ) {
      return;
    }
  }

  // Test for path existance
  if( !DirInfo::Exists( path ) ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File doesn't exist:", path );
    return;
  }

  // Open the object so we can scan for a list targets
  bifstream in( path );
  if( !in ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error opening object file for endomorph scan:", path );
    return;
  }

  // Make sure this is an LWO2
  in.SetEndian( ENDIAN_MOTOROLA );
  unsigned long chunk_id;

  in >> chunk_id;
  if( chunk_id != LWObjectChunk_FORM ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", path );
    return;
  }

  in >> chunk_id;  // Skip the file size
  in >> chunk_id;
  if( chunk_id != LWObjectChunk_LWO2 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", path );
    return;
  }

  // Find all the endomorphs in the object
  DynArray< EchoTarget * > targets;
  unsigned long            chunk_size;
  streampos                chunk_start;
  short                    temps;
  char                     buffer[ 1024 ];

  try {
    while( !in.eof() ) {
      if( in.bad() || in.fail() ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error reading object file while scanning for endomorphs:", path );
        throw false;
      }

      // Get the basic chunk data
      chunk_start = in.tellg();
      in >> chunk_id >> chunk_size;

      // Figure out what kind of chunk this is
      switch( chunk_id ) {
        case LWObjectChunk_VMAP:                    // VMAP
          in >> chunk_id;
          if( chunk_id == LWObject_VMapType_MORF ) {          // - Only scan endomorphs
            in >> temps;                                      // - Skip the dimensions; we only want the name
            in.GetString( buffer, 1024 );                     // - Fetch the map name
            targets.Add( new EchoTarget( buffer ) );          // - Add the new target to our list
          }

          in.seekg( chunk_start + chunk_size + 8, ios::beg ); // - Skip the rest of the chunk
          break;

        default:                                    // Any Other Chunk; skip it
          in.seekg( chunk_size, ios::cur );
      }

      // Skip another byte if the chunk size is odd (chunks must be even sized)
      if( (chunk_size % 2) > 0 )
        in.seekg( 1, ios::cur );
    }
  } catch ( bool ) {
    targets.Flush();
    return;
  }

  // Close the file and add the targets to the mapping list
  in.close();

  if( targets.NumElements() == 0 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  There are no endomorphs in the object:", path );
    return;
  }

  // Unlink the Visemes
  for( unsigned long i=0; i < map->GetVisemes().NumElements(); i++ )
    map->GetVisemes()[i]->SetLinkedTo(-1);

  map->GetTargets() = targets;

  // Resort the list
  map->GetTargets().QSort( SortTargetsAlpha );

  // Add the object as a synonym if it isn't already there
  eint->AddSynonym( path );
}

// HandleSynonymAdd():
//  Adds a new synonym object.
void HandleSynonymAdd( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  char path[ MAX_PATH_LENGTH ];
  GET_STR( eint->synonym_add, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  // Test for path existance
  if( !DirInfo::Exists( path ) ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File doesn't exist:", path );
    return;
  }

  // Open the object so we can scan for a list targets
  bifstream in( path );
  if( !in ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error opening object file for endomorph scan:", path );
    return;
  }

  // Make sure this is an LWO2
  in.SetEndian( ENDIAN_MOTOROLA );
  unsigned long chunk_id;

  in >> chunk_id;
  if( chunk_id != LWObjectChunk_FORM ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", path );
    return;
  }

  in >> chunk_id;  // Skip the file size
  in >> chunk_id;
  if( chunk_id != LWObjectChunk_LWO2 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", path );
    return;
  }

  // Make sure the object is valid and has at least one endomorph
  unsigned long            chunk_size;
  streampos                chunk_start;

  while( !in.eof() ) {
    if( in.bad() || in.fail() ) {
      (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error reading object file while scanning for endomorphs:", path );
      throw false;
    }

    // Get the basic chunk data
    chunk_start = in.tellg();
    in >> chunk_id >> chunk_size;

    // Figure out what kind of chunk this is
    switch( chunk_id ) {
      case LWObjectChunk_VMAP:                    // VMAP
        in >> chunk_id;
        if( chunk_id == LWObject_VMapType_MORF )  // - Only test endomorphs
          break;
        break;

      default:                                    // Any Other Chunk; skip it
        in.seekg( chunk_size, ios::cur );
    }

    // Skip another byte if the chunk size is odd (chunks must be even sized)
    if( (chunk_size % 2) > 0 )
      in.seekg( 1, ios::cur );
  }

  // Make sure the object isn't already in this map
  EchoMapping *map = mappings[ eint->current_mapping ];
  if( !eint->AddSynonym( path ) ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  Object already belongs to this mapping:", path );
    return;
  }

  eint->UpdateWithCurrentMapping();
}

// AddSynonym():
//  Returns true if the synonym was added, and false if the synonym already exists.
bool EchoToMM_Interface::AddSynonym( const char *path ) {
  assert( eint->current_mapping != -1 );
  assert( path != NULL );

  EchoMapping *map = mappings[ eint->current_mapping ];

  // Make sure it isn't already there
  for( unsigned long i=0; i < map->GetSynonyms().NumElements(); i++ ) {
    if( stricmp( path, map->GetSynonyms()[i]->GetFullPath() ) == 0 )
      return false;
  }

  // Create the new synonym and add it to the list
  EchoSynonym *synonym = new EchoSynonym;
  synonym->SetFullPath( path );
  synonym->SetIsEnabled( true );

  map->GetSynonyms().Add( synonym );
  eint->current_synonym = map->GetSynonyms().NumElements() - 1;

  return true;
}

// HandleSynonymRemove():
//  Removes the selected synonym object.
void HandleSynonymRemove( LWControl *con, void *data ) {
  int index;
  GET_INT( eint->synonym_list, index );

  if( index == -1 )
    return;

  EchoMapping *map = mappings[ eint->current_mapping ];
  map->GetSynonyms().Remove( index );
  SET_INT( eint->synonym_list, ((index >= (int)map->GetSynonyms().NumElements()) ? index - 1 : index) );
}

// HandleVisemeReload():
//  Changes the current viseme sync file for mapping purposes.
void HandleVisemeReload( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  char path[ MAX_PATH_LENGTH ];
  GET_STR( eint->viseme_reload, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  // See if there are any visemes and warn the user about clearing the links
  EchoMapping *map = mappings[ eint->current_mapping ];
  if( map->GetVisemes().NumElements() > 0 ) {
    if( (*eint->message->okCancel)( "Load New Visemes?",
                                    "Echo To Morph Mixer: Are you sure you want to load new visemes?",
                                    "This will remove any existing links in this mapping." ) == 0 ) {
      return;
    }
  }

  // See if this is a valid sync filename
  char *ext = DirStrings::HasExtension( path );
  if( ext == NULL ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a viseme sync file (extension must be .sync or .phon)", path );
    return;
  }

  if( (stricmp( ext, ".sync" ) != 0) && (stricmp( ext, ".phon" ) != 0) )  {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a viseme sync file (extension must be .sync or .phon)", path );
    return;
  }

  // Test for path existance
  if( !DirInfo::Exists( path ) ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File doesn't exist:", path );
    return;
  }

  // Open the sync file so we can scan for a list visemes
  pifstream in( path );
  if( !in ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error opening sync file for viseme scan:", path );
    return;
  }

  in.GuessEOLType();

  // Skip the wave file part
  char buffer[ 2048 ];
  in.getline( buffer, 2048 );

  // Skip the frame rate and frame count
  in.getline( buffer, 2048 );

  // Start reading in vesemes
  DynArray< EchoViseme * > visemes;

  int keys, i;
  while( !in.eof() ) {
    if( in.bad() || in.fail() ) {
      (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error reading sync file while scanning for viseme:", path );
      visemes.Flush();
      return;
    }

    // Read the viseme name
    in >> buffer;
    if( in.eof() )
      break;

    visemes.Add( new EchoViseme( buffer ) );

    // Skip the keys
    in >> keys;
    in.getline( buffer, 2048 );    // Skip the EOL

    for( i=0; i < keys; i++ )
      in.getline( buffer, 2048 );
  }

  in.close();

  // Unlink the Visemes
  for( unsigned long j=0; j < map->GetTargets().NumElements(); j++ )
    map->GetTargets()[j]->SetLinkedTo(-1);

  map->GetVisemes() = visemes;

  // Note:  Do NOT flush the Target List; this is taken
  //        care of (if needed) in the above loop

  // Resort the list
  map->GetVisemes().QSort( SortVisemesAlpha );

  // All done; update the UI
  eint->UpdateWithCurrentMapping();
}

// SortVisemesAlpha():
//  Sorts the viseme list alphabetically
int (__cdecl SortVisemesAlpha)( const void *arg1, const void *arg2 ) {
  EchoViseme *v1 = *((EchoViseme **)(arg1));
  EchoViseme *v2 = *((EchoViseme **)(arg2));

  return stricmp( v1->GetName(), v2->GetName() );
}

// SortTargetsAlpha():
//  Sorts the target list alphabetically
int (__cdecl SortTargetsAlpha)( const void *arg1, const void *arg2 ) {
  EchoTarget *v1 = *((EchoTarget **)(arg1));
  EchoTarget *v2 = *((EchoTarget **)(arg2));

  return stricmp( v1->GetName(), v2->GetName() );
}

// HandleLink():
//  Links the selected Viseme and Target.
void HandleLink( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  int viseme, target, hidden;
  GET_INT( eint->viseme_list,    viseme );
  GET_INT( eint->endomorph_list, target );
  GET_INT( eint->hide_linked,    hidden );

  if( (viseme == -1) || (target == -1) ) {
    (eint->message->warning)( "Echo To Morph Mixer Warning:  You must select an",
                              "Endomorph and a Viseme to link." );
    return;
  }

  EchoMapping *map = mappings[ eint->current_mapping ];

  // Update the Viseme
  if( hidden ) {
    int index = -1;
    for( unsigned long i=0; i < map->GetVisemes().NumElements(); i++ ) {
      if( !map->GetVisemes()[i]->IsLinked() ) {
        if( ++index == viseme )
          break;
      }
    }
    viseme = i;
  }

  if( map->GetVisemes()[ viseme ]->IsLinked() )
    map->GetTargets()[ map->GetVisemes()[ viseme ]->GetLinkedTo() ]->SetLinkedTo( -1 );

  // Update the Target
  if( hidden ) {
    int index = -1;
    for( unsigned long i=0; i < map->GetTargets().NumElements(); i++ ) {
      if( !map->GetTargets()[i]->IsLinked() ) {
        if( ++index == target )
          break;
      }
    }
    target = i;
  }

  if( map->GetTargets()[ target ]->IsLinked() )
    map->GetVisemes()[ map->GetTargets()[ target ]->GetLinkedTo() ]->SetLinkedTo( -1 );

  map->GetVisemes()[ viseme ]->SetLinkedTo( target );
  map->GetTargets()[ target ]->SetLinkedTo( viseme );

  // Deselected any selected items in the lists
  SET_INT( eint->viseme_list,    -1 );
  SET_INT( eint->endomorph_list, -1 );

  // Update the UI
  eint->UpdateWithCurrentMapping();
}

// HandleClearLinks():
void HandleClearLinks( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  EchoMapping *map = mappings[ eint->current_mapping ];
  for( unsigned long i=0; i < map->GetVisemes().NumElements(); i++ )
    map->GetVisemes()[i]->SetLinkedTo(-1);

  for( i=0; i < map->GetTargets().NumElements(); i++ )
    map->GetTargets()[i]->SetLinkedTo(-1);

  eint->UpdateWithCurrentMapping();
}

// HandleTestLinks():
void HandleTestLinks( LWControl *con, void *data ) {
  assert( eint->current_mapping != -1 );

  EchoMapping *map = mappings[ eint->current_mapping ];

  if( map->GetSynonyms().NumElements() == 0 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  No synonyms to test against; nothing to do!", NULL );
    return;
  }

  DynArray< EchoTarget * > targets;
  unsigned long            chunk_size;
  streampos                chunk_start;
  short                    temps;
  char                     buffer[ 1024 ];

  // Open Progress Panel
  eint->progress_total = map->GetSynonyms().NumElements();
  eint->OpenProgressPanel();
  eint->progress_current = 0;
  
  for( unsigned long i=0; i < map->GetSynonyms().NumElements(); i++ ) {
    try {
      // Update the panel and see if the user closed it (ie: cancel operation)
      sprintf( eint->progress_string, "Testing \"%s\"...", map->GetSynonyms()[i]->GetFilename() );
      eint->progress_current++;
      REDRAW_CON( eint->progress_field );

      if( (*eint->panel_funcs->handle)( eint->progress_panel, 0 ) == -1) {
        if( (*eint->message->yesNo)( "Cancel Link Test?",
                                     "Echo To Morph Mixer:  Do you really want to abort the link test?", NULL ) == 1 ) {
          (*eint->message->info)( "Echo To Morph Mixer:  Test link scan aborted by user.", NULL );
          return;
        } else {
          (*eint->panel_funcs->open)( eint->progress_panel, PANF_PASSALLKEYS );
        }
      }

      // Test for path existance
      if( !DirInfo::Exists( map->GetSynonyms()[i]->GetFullPath() ) ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File doesn't exist:", map->GetSynonyms()[i]->GetFullPath() );
        throw false;
      }

      // Open the object so we can scan for a list targets
      bifstream in( map->GetSynonyms()[i]->GetFullPath() );
      if( !in ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error opening object file for endomorph scan:", map->GetSynonyms()[i]->GetFullPath() );
        throw false;
      }

      // Make sure this is an LWO2
      in.SetEndian( ENDIAN_MOTOROLA );
      unsigned long chunk_id;

      in >> chunk_id;
      if( chunk_id != LWObjectChunk_FORM ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", map->GetSynonyms()[i]->GetFullPath() );
        throw false;
      }

      in >> chunk_id;  // Skip the file size
      in >> chunk_id;
      if( chunk_id != LWObjectChunk_LWO2 ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File is not a Version 2 Lightwave Object:", map->GetSynonyms()[i]->GetFullPath() );
        continue;
      }

      // Find all the endomorphs in the object
      while( !in.eof() ) {
        if( in.bad() || in.fail() ) {
          (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error reading object file while scanning for endomorphs:", map->GetSynonyms()[i]->GetFullPath() );
          throw false;
        }

        // Get the basic chunk data
        chunk_start = in.tellg();
        in >> chunk_id >> chunk_size;

        // Figure out what kind of chunk this is
        switch( chunk_id ) {
          case LWObjectChunk_VMAP:                    // VMAP
            in >> chunk_id;
            if( chunk_id == LWObject_VMapType_MORF ) {          // - Only scan endomorphs
              in >> temps;                                      // - Skip the dimensions; we only want the name
              in.GetString( buffer, 1024 );                     // - Fetch the map name
              targets.Add( new EchoTarget( buffer ) );          // - Add the new target to our list
            }

            in.seekg( chunk_start + chunk_size + 8, ios::beg ); // - Skip the rest of the chunk
            break;

          default:                                    // Any Other Chunk; skip it
            in.seekg( chunk_size, ios::cur );
        }

        // Skip another byte if the chunk size is odd (chunks must be even sized)
        if( (chunk_size % 2) > 0 )
          in.seekg( 1, ios::cur );
      }

      // Close the file and add the targets to the mapping list
      in.close();

      if( targets.NumElements() == 0 ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  There are no endomorphs in the synonym:", map->GetSynonyms()[i]->GetFullPath() );
        throw false;
      }

      // Make sure the targets in the object match those linked
      for( unsigned long j=0; j < map->GetTargets().NumElements(); j++ ) {
        if( map->GetTargets()[j]->IsLinked() ) {
          for( unsigned long k=0; k < targets.NumElements(); k++ ) {
            if( stricmp( map->GetTargets()[j]->GetName(), targets[k]->GetName() ) == 0 )
              break;
          }

          if( k == targets.NumElements() ) {
            sprintf( buffer, "Echo To Morph Mixer Error:  There is no match for the linked target \"%s\" in", map->GetTargets()[j]->GetName() );
            (*eint->message->error)( buffer, map->GetSynonyms()[i]->GetFullPath() );
            throw false;
          }
        }
      }
    } catch ( bool ) {
      targets.Flush();
      if( (*eint->message->okCancel)( "Continue Testing?", "Echo To Morph Mixer:  Continue testing links?", NULL ) == 0 ) {
        (*eint->panel_funcs->close)( eint->progress_panel );
        return;
      }

      continue;
    }

    targets.Flush();
  }

  (*eint->message->info)( "Echo To Morph Mixer:  All synonyms contain linked targets; test OK!", NULL );
  (*eint->panel_funcs->close)( eint->progress_panel );
}

// HandleSaveProjectMapping():
void HandleSaveProjectMapping( LWControl *con, void *data ) {
  char path[ MAX_PATH_LENGTH ];
  GET_STR( eint->save_project_mapping, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  // Make sure there is at least one mapping
  if( mappings.NumElements() == 0 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  You must have at least one mapping before saving.", NULL );
    return;
  }

  // Add .epm to the path, if needed
  DirStrings::ChangeExtension( path, ".epm" );

  // Open the output file
  pofstream out( path );
  if( !out ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  Unable to open project mapping output file", path );
    return;
  }

  // Output the header
  out << "EchoToMMProject 1" << out.endl();

  // Save data
  unsigned long i, j;
  for( i=0; i < mappings.NumElements(); i++ ) {
    EchoMapping *map = mappings[i];

    // Save Core Data
    out << "Mapping {" << out.endl();
    if( map->GetName() != NULL )
      out << "  Name " << map->GetName() << out.endl();

    // Save Synonyms
    for( j=0; j < map->GetSynonyms().NumElements(); j++ ) {
      out << "  Synonym {" << out.endl();

      if( map->GetSynonyms()[j]->GetFullPath() != NULL )
        out << "    FullPath " << map->GetSynonyms()[j]->GetFullPath() << out.endl();

      if( map->GetSynonyms()[j]->GetIsEnabled() )
        out << "    Enabled" << out.endl();

      out << "  }" << out.endl();
    }

    // Save Targets
    for( j=0; j < map->GetTargets().NumElements(); j++ ) {
      out << "  Target {" << out.endl();

      if( map->GetTargets()[j]->GetName() != NULL )
        out << "    Name " << map->GetTargets()[j]->GetName() << out.endl();

      if( map->GetTargets()[j]->IsLinked() )
        out << "    LinkedTo " << map->GetTargets()[j]->GetLinkedTo() << out.endl();

      out << "  }" << out.endl();
    }

    // Save Visemes
    for( j=0; j < map->GetVisemes().NumElements(); j++ ) {
      out << "  Viseme {" << out.endl();

      if( map->GetVisemes()[j]->GetName() != NULL)
        out << "    Name " << map->GetVisemes()[j]->GetName() << out.endl();

      if( map->GetVisemes()[j]->IsLinked() )
        out << "    LinkedTo " << map->GetVisemes()[j]->GetLinkedTo() << out.endl();

      out << "  }" << out.endl();
    }

    out << "}" << out.endl();
  }

  if( !out ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error occured saving project mapping to", path );
    return;
  }

  out.close();
  (*eint->message->info)( "Echo To Morph Mixer Info:  Successfully saved Project Mapping to", path );
}

// HandleProjectMappingFReq():
void HandleProjectMappingFReq( LWControl *con, void *data ) {
  char path[ MAX_PATH_LENGTH ];
  GET_STR( eint->project_mapping_freq, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  if( !DirInfo::Exists( path ) ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  Unable to load project mapping; file not found:", path );
    return;
  }

  pifstream in( path, ios::nocreate );
  if( !in ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error occured while opeing project mapping from", path );
    return;
  }

  char buffer[ 2048 ];

  // Check the header
  in >> buffer;
  if( stricmp( buffer, "EchoToMMProject" ) != 0 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  This is not a project mapping file:", path );
    return;
  }

  // Check the version
  int version;
  in >> version;
  if( version != 1 ) {
    (*eint->message->error)( "Echo To Morph Mixer Error:  Unsupported project mapping version found in", path );
    return;
  }

  // Clear out any existing mappings
  mappings.Flush();

  enum EchoBlockID { EBLOCK_ROOT = 0, EBLOCK_MAPPING, EBLOCK_SYNONYM, EBLOCK_TARGET, EBLOCK_VISEME };

  // Read in the file
  EchoBlockID  block = EBLOCK_ROOT;
  char        *word;
  EchoMapping *map     = NULL;
  EchoSynonym *synonym = NULL;
  EchoTarget  *target  = NULL;
  EchoViseme  *viseme  = NULL;
  int          tempi;

  try {
    while( !in.eof() ) {
      in >> buffer;

      if( in.eof() ) {
        if( block != EBLOCK_ROOT ) {
          (*eint->message->error)( "Echo To Morph Mixer Error:  Unexpected End Of File reached while reading project mapping from", path );
          throw false;
        } else {
          break;
        }
      }

      if( in.bad() || in.fail() ) {
        (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error occured while reading project mapping from", path );
        throw false;
      }

      word = StringTools::FindWord( buffer, 0 );
      if( word == NULL )
        continue;

      switch( block ) {
        case EBLOCK_ROOT:
          if( word[0] == '}' ) {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unbalanced closing brace found at root level of project mapping", path );
            throw false;

          } else if( strnicmp( word, "Mapping", 7 ) == 0 ) {
            block = EBLOCK_MAPPING;
            map = new EchoMapping;
            mappings.Add( map );
            in >> buffer;  // Skip the opening brace

          } else {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unknown keyword found at root level of project mapping", path );
            mappings.Flush();
            throw false;
          }
          break;

        case EBLOCK_MAPPING:
          assert( map != NULL );
          if( word[0] == '}' ) {
            block = EBLOCK_ROOT;
            map = NULL;
          } else if( strnicmp( word, "Name", 4 ) == 0 ) {
            in >> buffer;
            map->SetName( buffer );

          } else if( strnicmp( word, "Synonym", 7 ) == 0 ) {
            block = EBLOCK_SYNONYM;
            synonym = new EchoSynonym;
            map->GetSynonyms().Add( synonym );
            in >> buffer;  // Skip the opening brace

          } else if( strnicmp( word, "Target", 6 ) == 0 ) {
            block = EBLOCK_TARGET;
            target = new EchoTarget;
            map->GetTargets().Add( target );
            in >> buffer;  // Skip the opening brace

          } else if( strnicmp( word, "Viseme", 6 ) == 0 ) {
            block = EBLOCK_VISEME;
            viseme = new EchoViseme;
            map->GetVisemes().Add( viseme );
            in >> buffer;  // Skip the opening brace

          } else {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unknown keyword found at mapping level of project", path );
            throw false;
          }
          break;

        case EBLOCK_SYNONYM:
          assert( synonym != NULL );
          if( word[0] == '}' ) {
            block = EBLOCK_MAPPING;
            synonym = NULL;

          } else if( strnicmp( word, "FullPath", 8 ) == 0 ) {
            in >> buffer;
            synonym->SetFullPath( buffer );

          } else if( strnicmp( word, "Enabled", 7 ) == 0 ) {
            synonym->SetIsEnabled( true );

          } else {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unknown keyword found at synonym level of project", path );
            throw false;
          }
          break;

        case EBLOCK_TARGET:
          assert( target != NULL );
          if( word[0] == '}' ) {
            block = EBLOCK_MAPPING;
            target = NULL;

          } else if( strnicmp( word, "Name", 4 ) == 0 ) {
            in >> buffer;
            target->SetName( buffer );

          } else if( strnicmp( word, "LinkedTo", 8 ) == 0 ) {
            in >> tempi;
            target->SetLinkedTo( tempi );

          } else {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unknown keyword found at target level of project", path );
            throw false;
          }
          break;

        case EBLOCK_VISEME:
          assert( viseme != NULL );
          if( word[0] == '}' ) {
            block = EBLOCK_MAPPING;
            viseme = NULL;

          } else if( strnicmp( word, "Name", 4 ) == 0 ) {
            in >> buffer;
            viseme->SetName( buffer );

          } else if( strnicmp( word, "LinkedTo", 8 ) == 0 ) {
            in >> tempi;
            viseme->SetLinkedTo( tempi );

          } else {
            (*eint->message->error)( "Echo To Morph Mixer Error:  Unknown keyword found at viseme level of project", path );
            throw false;
          }
          break;
      }
    }
  } catch( bool ) {
    mappings.Flush();
    return;
  }

  if( mappings.NumElements() > 0 )
    eint->current_mapping = 0;

  eint->UpdateWithCurrentMapping();
}

// Process():
bool EchoToMM_Interface::Process() {
  // Make sure at least one endomorph is in the scene
  if( objects.NumElements() < 1 ) {
    (*message->error)( "Echo To Morph Mixer Error:  No endomorph objects exist in the scene; nothing to do!", NULL );
    return false;
  }

  // Make sure there is at least one mapping
  if( mappings.NumElements() < 1 ) {
    (*message->error)( "Echo To Morph Mixer Error:  No project mapping is currently loaded; nothing to do!", NULL );
    return false;
  }

  // Figure out which object to apply to
  int index;
  GET_INT( object_popup, index );
  EchoLWItem *item = objects[ index ];

  // Find out which mapping the object belongs to, if any
  if( mappings.NumElements() < 1 ) {
    (*message->error)( "Echo To Morph Mixer Error:  No project mapping is currently loaded; nothing to do!", NULL );
    return false;
  }

  unsigned long i, j;
  EchoMapping *map = NULL;
  for( i=0; i < mappings.NumElements(); i++ ) {
    for( j=0; j < mappings[i]->GetSynonyms().NumElements(); j++ ) {
      if( mappings[i]->GetSynonyms()[j]->GetIsEnabled() ) {
        if( mappings[i]->GetSynonyms()[j]->GetFilename(), item->GetName() ) {
          map = mappings[i];
        }
      }
    }
  }

  if( map == NULL ) {
    (*message->error)( "Echo To Morph Mixer Error:  The selected obejct doesn't match any enabled",
                       "synonyms in the currently loaded project mapping; aborting");
    return false;
  }

  // See if the sync file is valid
  char sync_path[ 1024 ];
  GET_STR( viseme_freq, sync_path, 1024 );

  if( sync_path[0] == '\0' ) {
    (*message->error)( "Echo To Morph Mixer Error:  No viseme sync file selected; nothing to do!", NULL );
    return false;
  }

  // See if this is a valid sync filename
  char *ext = DirStrings::HasExtension( sync_path );
  if( ext == NULL ) {
    (*message->error)( "Echo To Morph Mixer Error:  File is not a viseme sync file.", "(filename extension must be .sync or .phon)" );
    return false;
  }

  if( (stricmp( ext, ".sync" ) != 0) && (stricmp( ext, ".phon" ) != 0) )  {
    (*message->error)( "Echo To Morph Mixer Error:  File is not a viseme sync file.", "(filename extension must be .sync or .phon)" );
    return false;
  }

  // See if the sync file exists
  if( !DirInfo::Exists( sync_path ) ) {
    (*message->error)( "Echo To Morph Mixer Error:  Viseme sync file doesn't exist.", NULL );
    return false;
  }

  // Open the sync file
  pifstream in( sync_path );
  if( !in ) {
    (*message->error)( "Echo To Morph Mixer Error:  File I/O error occured opening viseme sync file; aborting.", NULL );
    return false;
  }

  // Begin processing, step one:  Add Morph Mixer if it isn't already applied
  const char *server_name;
  char buffer[ 2048 ];

  // - Select this item
  sprintf( buffer, "SelectItem %x", item->GetID() );
  (*generic->evaluate)( generic->data, buffer );

  // - Look for Morph Mixer
  server_name = (const char *)-1;
  for( i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( item->GetID(), LWDISPLACEMENT_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "LW_MorphMixer" ) == 0 ) {
      break;
    }
  }

  // - Get some more interface settings
  double scale;
  int    clip;
  GET_FLOAT( scale_values, scale );
  GET_INT( clip_values, clip );

  // - Apply Morph Mixer if it isn't already there
  if( server_name == NULL ) {
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWDISPLACEMENT_HCLASS, "LW_MorphMixer" );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Get the item's channel group
  LWChanGroupID item_group = iteminfo->chanGroup( item->GetID() );

  // Start reading data from the sync file
  in.getline( buffer, 2048 );    // Skip the wave file

  int sync_fps, total_frames;
  in >> sync_fps;
  in >> total_frames;

  double layout_fps = (int)sceneinfo->framesPerSecond;

  // - Loop through reading the visemes
  LWChannelID    channel, temp_channel;
  LWChanGroupID  temp_group;
  EchoTarget    *target;
  int            keys;
  double         frame, value, slope, prev_value;

  const int      linear_shape = 3;

  // Open Progress Panel
  eint->OpenProgressPanel();
  eint->progress_current = 0;

  eint->progress_current = in.tellg();
  in.seekg( 0, ios::end );
  eint->progress_total = in.tellg();
  in.clear();
  in.seekg( eint->progress_current, ios::beg );
  
  while( !in.eof() ) {
    if( (*eint->panel_funcs->handle)( eint->progress_panel, 0 ) == -1) {
      if( (*eint->message->yesNo)( "Cancel Viseme Translation?",
                                   "Echo To Morph Mixer:  Do you really want to abort translating the visemes?",
                                   "Note that some visemes may already have been loaded into Morph Mixer channels.") == 1 ) {
        (*eint->message->info)( "Echo To Morph Mixer:  Viseme translation aborted by user.", NULL );
        (*panel_funcs->close)( progress_panel );
        return false;
      } else {
        (*eint->panel_funcs->open)( eint->progress_panel, PANF_PASSALLKEYS );
      }
    }

    // Test for errors
    if( in.bad() || in.fail() ) {
      (*eint->message->error)( "Echo To Morph Mixer Error:  File I/O error reading sync file while scanning for viseme:", sync_path );
      return false;
    }

    // Read the viseme name
    in >> buffer;
    in >> keys;
    if( in.eof() )
      break;

    // Update the panel and see if the user closed it (ie: cancel operation)
    sprintf( eint->progress_string, "Processing \"%s\"...", buffer );
    eint->progress_current = in.tellg();
    REDRAW_CON( eint->progress_field );

    // Figure out which target is mapped to this viseme, if any
    target  = NULL;
    channel = NULL;
    for( i=0; i < map->GetVisemes().NumElements(); i++ ) {
      if( stricmp( buffer, map->GetVisemes()[i]->GetName() ) == 0 ) {
        if( !map->GetVisemes()[i]->IsLinked() )
          break;

        target = map->GetTargets()[ map->GetVisemes()[i]->GetLinkedTo() ];
      }
    }

    // Figure out which channel this target points to
    if( target != NULL ) {
      // Handle grouped channels
      temp_group = item_group;
      char *dot = strchr( target->GetName(), '.' );
      if( dot != NULL ) {
        strncpy( buffer, target->GetName(), dot - target->GetName() );
        buffer[ dot-target->GetName() ] = '\0';

        for( temp_group = (*chaninfo->nextGroup)( item_group, NULL ); temp_group != NULL; temp_group = (*chaninfo->nextGroup)( item_group, temp_group ) ) {
          if( stricmp( buffer, (*chaninfo->groupName)( temp_group ) ) == 0 )
            break;
        }
      } else {
        dot = buffer;
      }

      // Find the actual channel
      for( temp_channel = (*chaninfo->nextChannel)( temp_group, NULL ); temp_channel != NULL; temp_channel = (*chaninfo->nextGroup)( temp_group, temp_channel ) ) {
        if( stricmp( &(dot[1]), (*chaninfo->channelName)( temp_channel ) ) == 0 ) {
          channel = temp_channel;
          break;
        }
      }
    }

    // No match; skip the keys
    if( channel == NULL ) {
      in.getline( buffer, 2048 );  // Skip the EOL
      for( i=0; i < (unsigned long)keys; i++ )
        in.getline( buffer, 2048 );
    } else {
      // Channel matches; clear it's existing keys
      LWEnvKeyframeID key, new_key;
      const LWEnvelopeID env = chaninfo->channelEnvelope( channel );

      key = (*env_funcs->nextKey)( env, NULL );
      while( key != NULL ) {
        new_key = (*env_funcs->nextKey)( env, key );          // Get the next key now, since we might be deleting this one
        (*env_funcs->destroyKey)( env, key );
        key = new_key;
      }

      // Read keys from the viseme and create new ones in the envelope
      for( i=0; i < (unsigned long)keys; i++ ) {
        in >> frame >> value >> slope;
        key = (*env_funcs->createKey)( env, frame/layout_fps, (value/100.0) * scale );

        // Clip values below 0, if enabled
        if( clip != 0 ) {
          if( i > 0 ) {
            if( (prev_value <= 0.0) && (value <= 0.0 ) )
              (*env_funcs->keySet)( env, key, LWKEY_SHAPE, (void *)&linear_shape );
          }
          prev_value = value;
        }
      }
    }
  } 

  in.close();
  (*panel_funcs->close)( progress_panel );

  (*eint->message->info)( "Echo To Morph Mixer:  Viseme Translation Complete!", NULL );

  return true;
}

// HandleKeys():
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  int state;

  LWControl *list;
  int item_count = 0;

  int setup;
  GET_INT( eint->setup, setup );

  if( (key == LWDK_SC_UP) || (key == LWDK_SC_DOWN) ) {
    if( eint->current_mapping != -1 ) {
      EchoMapping *map = mappings[ eint->current_mapping ];

      switch( eint->last_clicked_list ) {
        case ECHOLIST_ENDO:
          list = eint->object_popup;
          item_count = eint->objects.NumElements();
          break;

        case ECHOLIST_SYNONYM:
          list = eint->synonym_list;
          item_count = map->GetSynonyms().NumElements();
          break;

        case ECHOLIST_TARGET:
          list = eint->endomorph_list;
          item_count = map->GetTargets().NumElements();
          break;

        case ECHOLIST_VISEME:
          list = eint->viseme_list;
          item_count = map->GetVisemes().NumElements();
          break;

        case ECHOLIST_ASSIGNED:
          list = eint->assigned_list;
          item_count = AssCount( eint->panel );
          break;
      }
    }
  }

  switch( key ) {
    case LWDK_CTRL:             // Control
      eint->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      eint->shift_down = true;
      break;

    case LWDK_SC_UP:
      if( eint->ctrl_down ) {           // Previous Mapping
        GET_INT( eint->mapping_popup, state );
        if( state > 0 ) {
          SET_INT( eint->mapping_popup, state-1 );
          HandleMappingPopup( eint->mapping_popup, NULL );
        }
      } else {
        if( setup && (eint->current_mapping != -1) ) {
          if( eint->shift_down ) {   // First Item
            if( eint->last_clicked_list == ECHOLIST_ASSIGNED ) {
              HandleAssignedList( list, NULL );
            } else {
              SET_INT( list, 0 );
              REDRAW_CON( list );
            }
          } else {                          // Previous Item
            GET_INT( list, state );
            if( state > 0 ) {
              if( eint->last_clicked_list == ECHOLIST_ASSIGNED ) {
                HandleAssignedList( list, NULL );
              } else {
                SET_INT( list, state-1 );
                REDRAW_CON( list );
              }
            }
          }
        }
      }
      break;

    case LWDK_SC_DOWN:
      if( eint->ctrl_down ) {           // Next Mapping
        GET_INT( eint->mapping_popup, state );
        if( state < (int)mappings.NumElements()-2 ) {
          SET_INT( eint->mapping_popup, state+1 );
          HandleMappingPopup( eint->mapping_popup, NULL );
        }
      } else {
        if( setup && (eint->current_mapping != -1) ) {
          if( eint->shift_down ) {      // Last Item
            if( eint->last_clicked_list == ECHOLIST_ASSIGNED ) {
              HandleAssignedList( list, NULL );
            } else {
              SET_INT( list, item_count-1 );
              REDRAW_CON( list );
            }
          } else {                      // Next Item
            GET_INT( list, state );
            if( state < item_count-2 ) {
              if( eint->last_clicked_list == ECHOLIST_ASSIGNED ) {
                HandleAssignedList( list, NULL );
              } else {
                SET_INT( list, state+1 );
                REDRAW_CON( list );
              }
            }
          }
        }
      }
      break;

    case 9:                     // Activate first field
      ACTIVATE_CON( eint->viseme_freq );
      break;

    case ' ':                   // Find
    case 'f':
    case 'F':
      HandleFindLink( eint->find_link, NULL );
      break;

    case LWDK_RETURN:           // Link
    case LWDK_PAD_ENTER:
    case '\r':
      HandleLink( eint->link, NULL );
      break;

    case LWDK_DELETE:
      HandleUnlink( eint->link, NULL );
      break;

    case '*':                   // New Group
      SET_INT( eint->mapping_popup, mappings.NumElements() + 1 );
      HandleMappingPopup( eint->mapping_popup, NULL );
      break;

    case '+':                   // Add Synonym
    case '=': {
      char path[ MAX_PATH_LENGTH ] = "<";
      GET_STR( eint->synonym_add, &(path[1]), MAX_PATH_LENGTH );

      char filename[ MAX_PATH_LENGTH ] = "";
      char *file_here = DirStrings::HasFile( path );
      if( file_here != NULL ) {
        strcpy( filename, file_here );
        file_here[0] = '\0';
      }

      char buffer[ MAX_PATH_LENGTH ] = "";
      if( (*eint->freq_func)( "Add Synonym", filename, path, buffer, MAX_PATH_LENGTH ) ) {
        SET_STR( eint->synonym_add, buffer, strlen( buffer ) );
        HandleSynonymAdd( eint->synonym_add, NULL );
      }
    } break;

    case '-':                   // Remove Synonym
      HandleSynonymRemove( eint->synonym_remove, NULL );
      break;

    case 'm':
    case 'M':
      GET_INT( eint->setup, state );
      SET_INT( eint->setup, !state );
      HandleSetup( eint->setup, NULL );
      break;

    case 'h':
    case 'H':
      GET_INT( eint->setup, state );
      SET_INT( eint->setup, !state );
      HandleHideLinked( eint->hide_linked, NULL );
      break;

    case 'u':
    case 'U':
      if( eint->current_mapping != -1 ) {
        GET_INT( eint->synonym_list, state );
        if( state != -1 ) {
          EchoMapping *map = mappings[ eint->current_mapping ];
          map->GetSynonyms()[ state ]->SetIsEnabled( map->GetSynonyms()[ state ]->GetIsEnabled() );
        }
      }
      break;

    case 'c':
    case 'C':
      GET_INT( eint->clip_values, state );
      SET_INT( eint->clip_values, !state );
      break;

    case 'l':
    case 'L': {
      char path[ MAX_PATH_LENGTH ] = "<";
      GET_STR( eint->project_mapping_freq, &(path[1]), MAX_PATH_LENGTH );

      char filename[ MAX_PATH_LENGTH ] = "";
      char *file_here = DirStrings::HasFile( path );
      if( file_here != NULL ) {
        strcpy( filename, file_here );
        file_here[0] = '\0';
      }

      char buffer[ MAX_PATH_LENGTH ] = "";
      if( (*eint->freq_func)( "Load Project Mapping File", filename, path, buffer, MAX_PATH_LENGTH ) ) {
        SET_STR( eint->project_mapping_freq, buffer, strlen( buffer ) );
        HandleProjectMappingFReq( eint->project_mapping_freq, NULL );
      }
    } break;

    case 's':
    case 'S':
      if( eint->shift_down ) {
        char path[ MAX_PATH_LENGTH ] = ">";
        GET_STR( eint->save_project_mapping, &(path[1]), MAX_PATH_LENGTH );

        char filename[ MAX_PATH_LENGTH ] = "";
        char *file_here = DirStrings::HasFile( path );
        if( file_here != NULL ) {
          strcpy( filename, file_here );
          file_here[0] = '\0';
        }

        char buffer[ MAX_PATH_LENGTH ] = "";
        if( (*eint->freq_func)( "Save Project Mapping", filename, path, buffer, MAX_PATH_LENGTH ) ) {
          SET_STR( eint->save_project_mapping, buffer, strlen( buffer ) );
          if( DirInfo::Exists( buffer ) ) {
            if( (*eint->message->yesNo)( "Overwrite Existing Mapping?",
                                         "The project mapping file already exists.  Overwrite?", NULL ) == 1 )
              HandleSaveProjectMapping( eint->save_project_mapping, NULL );
          }
        }
      }
      break;

    case 'v':
    case 'V': {
      char path[ MAX_PATH_LENGTH ] = "<";
      GET_STR( eint->viseme_freq, &(path[1]), MAX_PATH_LENGTH );

      char filename[ MAX_PATH_LENGTH ] = "";
      char *file_here = DirStrings::HasFile( path );
      if( file_here != NULL ) {
        strcpy( filename, file_here );
        file_here[0] = '\0';
      }

      char buffer[ MAX_PATH_LENGTH ] = "";
      if( (*eint->freq_func)( "Load Viseme Sync File", filename, buffer, buffer, MAX_PATH_LENGTH ) )
        SET_STR( eint->viseme_freq, buffer, strlen( buffer ) );
    } break;

    case 't':
    case 'T':
      HandleTestLinks( eint->test_links, NULL );
      break;

    case LWDK_F1:                  // About
      HandleAbout( eint->about, NULL );
      break;
  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  switch( key ) {
    case LWDK_CTRL:            // Control
      eint->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      eint->shift_down = false;
      break;
  }
}

// OpenProgressPanel():
int EchoToMM_Interface::OpenProgressPanel() {
  // Destroy the panel if it hasn't been destroyed yet
  if( progress_panel != NULL ) {
    (*panel_funcs->destroy)( progress_panel );
    progress_panel = NULL;
  }

  // Create the panel
  progress_panel = (*panel_funcs->create)( "Echo To Morph Mixer Progress...", panel_funcs );
  if( !progress_panel ) {              // Make sure the panel was created
    (*message->error)("Error creating progress panel, aborting", NULL );
    return false;
  }

  // Progress Field
  progress_field = CANVAS_CTL( panel_funcs, progress_panel, "", 400, (CON_HOTH( object_popup )) );

  // Draw Callbacks
  ival.intv.value = (int)&DrawProgressField;
  (*progress_field->set)( progress_field, CTL_USERDRAW, &ival );

  // Set up some variables
  strcpy( progress_string, "Processing..." );

  // Open the Panel
  int retval = (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );

  return retval;
}

// DrawProgressField():
void DrawProgressField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = eint->panel_funcs->drawFuncs;

  // Clear the bar
  (*df->drawRGBBox)( eint->progress_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

  // Fill the progress portion of the bar
  (*df->drawRGBBox)( eint->progress_panel, 0, 60, 150, cx+1, cy+1,
                    (int)((cw-2) * ( ((double)eint->progress_current) / ((double)eint->progress_total) ) ), ch-2 );

  // Write some text
  (*df->drawText)( eint->progress_panel, eint->progress_string, COLOR_WHITE, cx + 3, cy + 2 );
}

