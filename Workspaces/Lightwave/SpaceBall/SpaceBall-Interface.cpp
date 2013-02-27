//
// SpaceBall-Interface.cpp
//

#include <stdio.h>

#include "spaceball-interface.h"
#include <lightwave/lwaboutpanel_tmpro/lwaboutpanel_tmpro.h>

extern SpaceBall_Interface *sb_int;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Spaceball Driver",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Basic Spaceball Input Device Driver",
  "for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2002 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// Constructor():
SpaceBall_Interface::SpaceBall_Interface() 
  : panel(NULL), is_open(false), creation_ok(false), current_function(0) {

  assert( sb_int == NULL );

  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_ACQUIRE );
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_ACQUIRE );

  sb_int = this;

  CreateUI();
}

// Destructor():
SpaceBall_Interface::~SpaceBall_Interface() {
  global( LWPANELFUNCS_GLOBAL,    GFUSE_RELEASE );
  global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_RELEASE );

  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  sb_int = NULL;
}

// CreateUI():
//  Creates the UI, but doesn't open it.  Called by the constructor.
void SpaceBall_Interface::CreateUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  char title_string[256] = "Spaceball Input Device Driver";
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return;
  }

  // Configure
  configure = WBUTTON_CTL( panel_funcs, panel, "Configure", 70 );

  // Open Config On Startup
  open_config_on_startup_toggle = BOOL_CTL( panel_funcs, panel, "Open on activation" );
  PanelTools::PutOnLineWith( configure, open_config_on_startup_toggle );

  // About
  about = WBUTTON_CTL( panel_funcs, panel, "About", 70 );
  PanelTools::PutOnLineWith( configure, about );

  // List
  list = MULTILIST_CTL( panel_funcs, panel, "Functions",
                        300, 15, ListNames, ListCount, ListColumnWidths );
  PanelTools::PutUnder( configure, list );

  // Command
  command = STR_CTL( panel_funcs, panel, "Command", 41 );
  PanelTools::PutUnder( list, command );

  // Capture
  capture = BOOLBUTTON_CTL( panel_funcs, panel, "Capture" );
  PanelTools::PutOnLineWith( command, capture );

  creation_ok = true;

  CON_SETEVENT( about,                         HandleAbout,               NULL );
  CON_SETEVENT( list,                          HandleList,                NULL );
  CON_SETEVENT( command,                       HandleCommand,             NULL );
  CON_SETEVENT( capture,                       HandleCapture,             NULL );
  CON_SETEVENT( configure,                     HandleConfigure,           NULL );
  CON_SETEVENT( open_config_on_startup_toggle, HandleOpenConfigOnStartup, NULL );

  int y = CON_Y( command );
  int h = CON_H( command );
  int ph = y + h;
  PAN_SETH( panel_funcs, panel, ph );

  PanelTools::RightAlignWithPanel( panel_funcs, panel, about, -4 );
}

// OpenUI():
void SpaceBall_Interface::OpenUI() {
  assert( sb_int != NULL );

  if( !sb_int->is_open ) {
    is_open = true;
    SET_INT( sb_int->capture, 0 );
    (*sb_int->panel_funcs->open)( sb_int->panel, PANF_MOUSETRAP | PANF_PASSALLKEYS | PANF_NOBUTT );
  }
}

// Multilist Callbacks
static char * list_names[]         = { "Func", "Command", NULL };
static int    list_column_widths[] = { 50, 250, 0 };

int ListCount( LWPanelID panel ) {
  return num_functions;
}

char * ListNames( LWPanelID panel, int item, int col ) {
  static char buffer[ 100 ];
  if( item == -1 )  // Column Headers
    return list_names[ col ];

  if( (item < 0) || (item >= num_functions) )
    return "";

  switch( col ) {
    case 0:         // Func
      sprintf( buffer, "%d", item + 1 );
      return buffer;

    case 1:         // Command
      if( function_map[ item ] == NULL )
        return "(none)";
      else
        return function_map[ item ];

    default:
      return "";
  }
}

int ListColumnWidths( LWPanelID panel, int col ) {  // pixel width of column i, up to 8 columns terminate with 0
  return list_column_widths[ col ];
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( sb_int->panel_funcs, "Spaceball Input Device Drivers", about_text, sb_int->panel, true, NULL );
}

// HandleList():
void HandleList( LWControl *con, void *data ) {
  GET_INT( sb_int->list, sb_int->current_function );
  if( (sb_int->current_function < 0) || (sb_int->current_function > num_functions) )
    return;

  char * string = ((function_map[ sb_int->current_function ] == '\0') ? "" : function_map[ sb_int->current_function ] );
  SET_STR( sb_int->command, string, strlen( string ) );
  SET_INT( sb_int->capture, 0 );
}

// HandleCommand():
void HandleCommand( LWControl *con, void *data ) {
  if( (sb_int->current_function < 0) || (sb_int->current_function > num_functions) )
    return;

  static char buffer[8192];
  GET_STR( sb_int->command, buffer, 8192 );
  StringTools::strdup2( function_map[ sb_int->current_function ], buffer );

  REDRAW_CON( sb_int->list );
}

// HandleCapture():
void HandleCapture( LWControl *con, void *data ) {
  if( (sb_int->current_function < 0) || (sb_int->current_function > num_functions) )
    return;
}

// HandleConfigure():
void HandleConfigure( LWControl *con, void *data ) {
  SiSetUiMode(hdl, SI_UI_ALL_CONTROLS);
}

// HandleOpenConfigOnStartup():
void HandleOpenConfigOnStartup( LWControl *con, void *data ) {
  ;
}
