//
// ContentDirector_Interface.cpp
//

#include "ContentDirector-Interface.h"
#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>
#include <stdio.h>

#include <portable/pfstream/pfstream.h>

static ContentDirector_Interface *cint = NULL;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Content Director",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Content Directory Switcher/Manager for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2000-2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

const char *dir_lister_columns[] = {
  "QCD", "Comment", "From", "At", "Exists", NULL };

const int dir_lister_column_widths[] = {
  30, 120, 40, 180, 30, 0 };

const char *path_from_strings[] = {
  "Specific Dir", "Env Variable", NULL };

const char *config_from_strings[] = {
  "Local Settings", "Shared Settings", NULL };

const char *create_base_popup_strings[] = {
  "Default", "Root Only", "Template", NULL };

const char *default_subdirs[] = {
  "Scenes", "Objects", "Images", "Motions", "Envelopes", "Surfaces", "Previews", NULL };

// Constructor():
ContentDirector_Interface::ContentDirector_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), create_panel(NULL), cur_entry( -1 ), cur_content_entry( -1 ),
    status_string(NULL), create_status_string(NULL), status_ok(true), create_progress(1.0),
    default_entry(-1) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  sceneinfo   = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,    GFUSE_TRANSIENT );
  dirinfo     = (LWDirInfoFunc   *)global( LWDIRINFOFUNC_GLOBAL,  GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,   GFUSE_TRANSIENT );

  unsigned long prodinfo = (unsigned long) global( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
  unsigned long major    = LWINF_GETMAJOR( prodinfo );
  unsigned long minor    = LWINF_GETMINOR( prodinfo );

  if( ((major >= 6) && (minor >= 1)) && (major < 7) )
    checkmark_string = "\03(i:MIMG_CHECKMARK)";
  else
    checkmark_string = "Yes";

  cint = this;

  InitEnvVarList();
}

// Destructor():
ContentDirector_Interface::~ContentDirector_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  if( create_panel != NULL )
    (*panel_funcs->destroy)( create_panel );

  entries.Flush();
  env_vars.Flush();

  StringTools::strdup2( status_string, NULL );
  StringTools::strdup2( create_status_string, NULL );

  cint = NULL;
}

// InitEnvVarList():
//  Fills in env_vars with the list of environment variables
void ContentDirector_Interface::InitEnvVarList() {
  const char * envs = GetEnvironmentStrings();
  int offset = 0;
  char * equal;

  env_vars.Flush();
  char buffer[ 1024 ];

  while( true ) {
    if( envs[ offset ] == '\0' )
      break;

    // Find the equal; the variable name is to the left of it
    equal = strchr( &(envs[ offset ]), '=' );
    if( equal != NULL ) {
      if( equal != &(envs[ offset ]) ) {
        strncpy( buffer, &(envs[ offset ]), equal - &(envs[ offset ]) );
        buffer[ equal - &(envs[ offset ]) ] = '\0';
        env_vars.Add( StringTools::strdup( buffer ) );
      }
    }

    offset += strlen( &(envs[ offset ]) ) + 1;
  };

  FreeEnvironmentStrings( (char *)envs );
}

// CreateUI():
bool ContentDirector_Interface::CreateUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Content Director -- %c2001 Joe Angell, TM Productions", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Current Content Dir
  current_dir_comment = STRRO_CTL(   panel_funcs, panel, "Current Content", 30 );
  current_dir_path    = STRRO_CTL(   panel_funcs, panel, "",                43 );
  current_dir_select  = WBUTTON_CTL( panel_funcs, panel, "Select",          42 );

  int w = CON_LW( current_dir_comment );
  MOVE_CON( current_dir_comment, -(w/2), 0 );
  PanelTools::PutOnLineWith( current_dir_comment, current_dir_path, -10 );
  PanelTools::PutOnLineWith( current_dir_path, current_dir_select );

  // Content Dir List
  dir_list = MULTILIST_CTL( panel_funcs, panel, "Available Content Dirs",
                            420, 14, DirListNames, DirListCount, DirListColumnWidths );

  PanelTools::PutUnder( current_dir_comment, dir_list, 18 );

  // List Management
  add_button         = WBUTTON_CTL( panel_funcs, panel, "Add",         68 );
  add_current_button = WBUTTON_CTL( panel_funcs, panel, "Add Current", 68 );
  duplicate_button   = WBUTTON_CTL( panel_funcs, panel, "Duplicate",   68 );
  remove_button      = WBUTTON_CTL( panel_funcs, panel, "Remove",      68 );

  create_button      = WBUTTON_CTL( panel_funcs, panel, "Create",      68 );

  shift_up_button    = WBUTTON_CTL( panel_funcs, panel, "Shift Up",    68 );
  shift_down_button  = WBUTTON_CTL( panel_funcs, panel, "Shift Down",  68 );

  PanelTools::PutOnLineWith( dir_list, add_button );
  PanelTools::AlignLabels( add_button, add_current_button );
  PanelTools::AlignLabels( add_current_button, duplicate_button );
  PanelTools::AlignLabels( duplicate_button, remove_button );

  PanelTools::AlignLabels( remove_button, create_button,   15 );
  PanelTools::AlignLabels( create_button, shift_up_button, 15 );
  PanelTools::AlignLabels( shift_up_button, shift_down_button );

  edit_button        = BOOLBUTTON_CTL( panel_funcs, panel, "Modify Dirs", );
  about_button       = WBUTTON_CTL(    panel_funcs, panel, "About", 68 );

  int x  = CON_X( add_button );
  int y  = CON_Y( dir_list );
  int h  = CON_H( dir_list );
  int h2 = CON_H( about_button );
  MOVE_CON( about_button, x, (y + h) - h2 );
  MOVE_CON( edit_button,  x, (y + h) - (h2 * 2) + 5 );

  // Content Dir Edit
  comment_string     = STR_CTL(       panel_funcs, panel, "Comment",   40 );
  default_dir_toggle = BOOL_CTL(      panel_funcs, panel, "Mark as QuickCD\'s path" );
  path_string        = DIR_CTL(       panel_funcs, panel, "",          63 );
  test_path_button   = WBUTTON_CTL(   panel_funcs, panel, "Test Path", 65 );

  path_from_popup    = WPOPUP_CTL(    panel_funcs, panel, "", path_from_strings, 90 );
  env_string         = STR_CTL(       panel_funcs, panel, "", 63 );
  env_popup          = CUSTPOPUP_CTL( panel_funcs, panel, "", 20, EnvName, EnvCount );

  // Config From
  config_from_popup   = WPOPUP_CTL( panel_funcs, panel, "Config From", config_from_strings, 100 );
  config_filebutton   = LOAD_CTL( panel_funcs, panel, "Shared Config Path", 30 );
  config_reload       = WBUTTON_CTL( panel_funcs, panel, "Reload", 65 );

  // Status Field
  status_field = CANVAS_CTL( panel_funcs, panel, "", 380, (CON_HOTH( config_reload )) );

  // Change Content Dir
  change_content_button = WBUTTON_CTL( panel_funcs, panel, "Set Content Directory", 120 );

  // Default States
  SET_INT( edit_button, 0 );
  const char * cur_content_dir = dirinfo( "Content" );
  if( cur_content_dir != NULL )
    SET_STR( current_dir_path, (char *)cur_content_dir, strlen( cur_content_dir ) );

  SET_STR( config_filebutton, "ContentDirector-Shared.cfg", strlen( "ContentDirector-Shared.cfg" ) );

  // Set Event Handlers
  CON_SETEVENT( current_dir_select, HandleSelectCurrentButton, NULL );

  CON_SETEVENT( about_button,       HandleAboutButton,         NULL );
  CON_SETEVENT( edit_button,        HandleEditButton,          NULL );

  CON_SETEVENT( dir_list,           HandleDirList,          NULL );

  CON_SETEVENT( add_button,         HandleAddButton,        NULL );
  CON_SETEVENT( add_current_button, HandleAddCurrentButton, NULL );
  CON_SETEVENT( duplicate_button,   HandleDuplicateButton,  NULL );
  CON_SETEVENT( remove_button,      HandleRemoveButton,     NULL );

  CON_SETEVENT( create_button,      HandleCreateButton,     NULL );

  CON_SETEVENT( shift_up_button,    HandleShiftUpButton,    NULL );
  CON_SETEVENT( shift_down_button,  HandleShiftDownButton,  NULL );

  CON_SETEVENT( comment_string,     HandleCommentString,    NULL );
  CON_SETEVENT( default_dir_toggle, HandleDefaultDirToggle, NULL );
  CON_SETEVENT( path_string,        HandlePathString,       NULL );
  CON_SETEVENT( test_path_button,   HandleTestPathButton,   NULL );

  CON_SETEVENT( path_from_popup,    HandlePathFromPopup,    NULL );
  CON_SETEVENT( env_string,         HandleEnvString,        NULL );
  CON_SETEVENT( env_popup,          HandleEnvPopup,         NULL );

  CON_SETEVENT( config_from_popup,  HandleConfigFromPopup,  NULL );
  CON_SETEVENT( config_filebutton,  HandleConfigFileButton, NULL );
  CON_SETEVENT( config_reload,      HandleReloadButton,     NULL );

  CON_SETEVENT( change_content_button, HandleChangeContentDirButton, NULL );

  // Set the Draw Callbacks
  (*panel_funcs->set)( panel, PAN_USERDRAW, DrawPanel );

  ival.intv.value = (int)&DrawStatusField;
  (*status_field->set)( status_field, CTL_USERDRAW, &ival );

  // Init Panel Size
  HandleEditButton( edit_button, NULL );

  // Create the Create Panel
  CreateCreatePanel();

  // Load Settings
  const char *settings_path = dirinfo( "Settings" );
  char config_path[ MAX_PATH_LENGTH ];

  if( settings_path != NULL ) {
    strcpy( config_path, settings_path );
    DirStrings::AddPathPart( config_path, "ContentDirector.cfg" );
    if( DirInfo::Exists( config_path ) )
      LoadSettings( config_path );
  }

  HandleSelectCurrentButton( current_dir_select, NULL );

  return true;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
int ContentDirector_Interface::OpenUI() {
  const char *settings_path = dirinfo( "Settings" );
  char config_path[ MAX_PATH_LENGTH ];
  strcpy( config_path, settings_path );
  DirStrings::AddPathPart( config_path, "ContentDirector.cfg" );

  if( settings_path == NULL ) {
    (*message->error)( "ContentDirector Error:  Settings path invalid; unable to load config",
                       "Any changes to content directory entries will not be saved." );
  }

  // Open the Panel
  int retval = (*panel_funcs->open)( panel, PANF_BLOCKING );
  HandleChangeContentDirButton( cint->change_content_button, NULL );

  // Save Settings
  int use_remote;
  GET_INT( config_from_popup, use_remote );

  if( settings_path != NULL ) {
    if( use_remote == 1 ) {
      char buffer[ MAX_PATH_LENGTH ];
      GET_STR( config_filebutton, buffer, MAX_PATH_LENGTH );
      SaveSettings( config_path, buffer );
    } else {
      SaveSettings( config_path, NULL );
    }
  }

  return retval;
}

// DrawMainPanel() {
void DrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( cint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( cint->panel_funcs, cint->panel );
  int pw = PAN_GETW( cint->panel_funcs, cint->panel );

  // Horizontal Dividers
  int y = CON_Y( cint->current_dir_comment );
  int h = CON_H( cint->current_dir_comment );
  PanelDraw::HorizontalDivider( cint->panel, &df, 0, y + h + 3, pw, 1 );

  int state;
  GET_INT( cint->edit_button, state );

  if( state == 1 ) {
    y = CON_Y( cint->dir_list );
    h = CON_H( cint->dir_list );
    PanelDraw::HorizontalDivider( cint->panel, &df, 0, y + h + 3, pw, 1 );

    y = CON_Y( cint->path_string );
    h = CON_H( cint->path_string );
    PanelDraw::HorizontalDivider( cint->panel, &df, 0, y + h + 3, pw, 1 );

    y = CON_Y( cint->config_from_popup );
    h = CON_H( cint->config_from_popup );
    PanelDraw::HorizontalDivider( cint->panel, &df, 0, y + h + 3, pw, 1 );
  }
}

//
// --- Multilist Handlers --------------------
// 
int DirListCount( LWPanelID panel ) {
  return cint->entries.NumElements();
}

char * DirListNames( LWPanelID panel, int item, int col ) {
  if( item == -1 )  // Column Headers
    return (char*)dir_lister_columns[ col ];

  if( item >= (int)cint->entries.NumElements() )
    return "";

  static char buffer[ 1024 ] = "";

  switch( col ) {
    case 0:
      return ((item == cint->default_entry) ? (char *)cint->checkmark_string : "");

    case 1:         // Comment
      return ((cint->entries[ item ]->GetName() == NULL) ? "(none)" : (char *)cint->entries[ item ]->GetName());

    case 2:         // From
      return ((cint->entries[ item ]->GetMode() == CDFM_SPECIFIC_DIR) ? "Dir" : "Env Var" );

    case 3:         // At
      if( cint->entries[ item ]->GetMode() == CDFM_SPECIFIC_DIR ) {
        return ((cint->entries[ item ]->GetPath() == NULL) ? "(none)" : (char *)cint->entries[ item ]->GetPath());
      } else {
        if( cint->entries[ item ]->GetEnvVar() == NULL ) {
          return "(none)";
        } else {
          const char * var = getenv( cint->entries[ item ]->GetEnvVar() );
          sprintf( buffer, "%s (%s)", cint->entries[ item ]->GetEnvVar(), (var == NULL) ? "env variable not set" : var );
          return buffer;
        }
      }

    case 4:         // Exists
      return (cint->entries[ item ]->GetExists() ? (char *)cint->checkmark_string : "");

    default:
      return "";
  }
}

int DirListColumnWidths( LWPanelID panel, int col ) {
  return dir_lister_column_widths[ col ];
}

// Env Popup
int EnvCount( void *userdata ) {
  return cint->env_vars.NumElements();
}

char * EnvName(  void *userdata, int index ) {
  if( (index >= (int)cint->env_vars.NumElements()) || (index < 0) )
    return "";

  return cint->env_vars[ index ];
}

//
// --- Event Handlers --------------------
//

// HandleModifyDirs():
void HandleEditButton( LWControl *con, void *data ) {
  int state;
  GET_INT( cint->edit_button, state );

  if( state == 0 ) {
    // Edit Mode Off
//    PanelTools::PutUnder( cint->dir_list, cint->status_field, 20 );
//    PanelTools::PutUnder( cint->dir_list, cint->change_content_button, 20 );

    MOVE_CON( cint->comment_string, 4, -100 );          // Controls are moved off the top of
    MOVE_CON( cint->default_dir_toggle, 4, -100 );      //  the window so that it will open
    MOVE_CON( cint->path_string, 4, -100 );             //  at the height we tell it to instead
    MOVE_CON( cint->test_path_button, 4, -100 );        //  of the height of those controls

    MOVE_CON( cint->path_from_popup, 4, -100 );
    MOVE_CON( cint->env_string, 4, -100 );
    MOVE_CON( cint->env_popup, 4, -100 );

    MOVE_CON( cint->config_from_popup, 4, -100 );
    MOVE_CON( cint->config_filebutton, 4, -100 );
    MOVE_CON( cint->config_reload, 4, -100 );

    MOVE_CON( cint->status_field, 4, -100 );
    MOVE_CON( cint->change_content_button, 4, -100 );

    GHOST_CON( cint->comment_string );
    GHOST_CON( cint->default_dir_toggle );
    GHOST_CON( cint->path_string );
    GHOST_CON( cint->test_path_button );

    GHOST_CON( cint->config_from_popup );
    GHOST_CON( cint->config_filebutton );
    GHOST_CON( cint->config_reload );

    GHOST_CON( cint->path_from_popup);
    GHOST_CON( cint->env_string );
    GHOST_CON( cint->env_popup );

    GHOST_CON( cint->status_field );
    GHOST_CON( cint->change_content_button );

    GHOST_CON( cint->add_button );
    GHOST_CON( cint->add_current_button );
    GHOST_CON( cint->remove_button );
    GHOST_CON( cint->duplicate_button );

    GHOST_CON( cint->create_button );

    GHOST_CON( cint->shift_up_button );
    GHOST_CON( cint->shift_down_button );

    ERASE_CON( cint->comment_string );
    ERASE_CON( cint->default_dir_toggle );
    ERASE_CON( cint->path_string );
    ERASE_CON( cint->test_path_button );

    ERASE_CON( cint->path_from_popup );
    ERASE_CON( cint->env_string );
    ERASE_CON( cint->env_popup );

    ERASE_CON( cint->config_from_popup );
    ERASE_CON( cint->config_filebutton );
    ERASE_CON( cint->config_reload );

    ERASE_CON( cint->status_field );
    ERASE_CON( cint->change_content_button );

    ERASE_CON( cint->add_button );
    ERASE_CON( cint->add_current_button );
    ERASE_CON( cint->remove_button );
    ERASE_CON( cint->duplicate_button );

    ERASE_CON( cint->create_button );

    ERASE_CON( cint->shift_up_button );
    ERASE_CON( cint->shift_down_button );

    int y = CON_Y( cint->dir_list );
    int h = CON_H( cint->dir_list );

    PAN_SETH( (cint->panel_funcs), (cint->panel), (y + h + 37) );
    (*cint->panel_funcs->draw)( cint->panel, DR_REFRESH );
  } else {
    // Edit Mode On
    PanelTools::PutUnder( cint->dir_list, cint->comment_string, 18 );
    PanelTools::AlignWith( cint->comment_string, cint->path_from_popup );

    int x = CON_X( cint->comment_string );
    int y = CON_Y( cint->comment_string );
    MOVE_CON( cint->comment_string, x + 53, y );

    PanelTools::PutOnLineWith( cint->comment_string, cint->default_dir_toggle );

    PanelTools::PutOnLineWith( cint->path_from_popup, cint->path_string, -3 );
    PanelTools::PutOnLineWith( cint->path_string, cint->test_path_button );

    PanelTools::PutOnLineWith( cint->path_from_popup, cint->env_string, -3 );
    PanelTools::PutOnLineWith( cint->env_string, cint->env_popup, -10 );

    PanelTools::PutOnLineWith( cint->path_string, cint->test_path_button );

    PanelTools::PutUnder( cint->path_string, cint->config_from_popup, 18 );
    PanelTools::PutOnLineWith( cint->config_from_popup, cint->config_filebutton, 4 );
    PanelTools::PutOnLineWith( cint->config_filebutton, cint->config_reload );

    PanelTools::PutUnder( cint->config_from_popup, cint->status_field, 20 );
    PanelTools::PutUnder( cint->config_from_popup, cint->change_content_button, 20 );

    int w  = CON_W( cint->change_content_button );
        y  = CON_Y( cint->change_content_button );
        x  = CON_X( cint->add_button );
    int w2 = CON_W( cint->add_button );
    MOVE_CON( cint->change_content_button, (x + w2) - w, y );

    UNGHOST_CON( cint->comment_string );
    UNGHOST_CON( cint->path_from_popup );

    int from_value;
    GET_INT( cint->path_from_popup, from_value );

    UNGHOST_CON( cint->test_path_button );

    UNGHOST_CON( cint->config_from_popup );
    UNGHOST_CON( cint->config_filebutton );
    UNGHOST_CON( cint->config_reload );

    UNGHOST_CON( cint->status_field );
    UNGHOST_CON( cint->change_content_button );

    UNGHOST_CON( cint->add_button );
    UNGHOST_CON( cint->add_current_button );
    UNGHOST_CON( cint->remove_button );
    UNGHOST_CON( cint->duplicate_button );

    UNGHOST_CON( cint->create_button );

    UNGHOST_CON( cint->shift_up_button );
    UNGHOST_CON( cint->shift_down_button );

    y = CON_Y( cint->status_field );
    int h = CON_H( cint->status_field );

    PAN_SETH( (cint->panel_funcs), (cint->panel), (y + h + 37) );
    (*cint->panel_funcs->draw)( cint->panel, DR_REFRESH );
  }

  // Update ghosts
  HandleDirList( cint->dir_list, NULL );
  HandleConfigFromPopup( cint->config_from_popup, NULL );
}

// HandleAboutButton():
void HandleAboutButton( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( cint->panel_funcs, "Content Director", about_text, cint->panel, true );
}

// HandleDirList():
void HandleDirList( LWControl *con, void *data ) {
  GET_INT( cint->dir_list, cint->cur_entry );

  CDEntryFromModes from_value = CDFM_SPECIFIC_DIR;
  if( (cint->cur_entry == -1) || ( ((unsigned long)cint->cur_entry) >= cint->entries.NumElements()) ) {
    SET_STR( cint->comment_string, "", 0 );
    SET_STR( cint->path_string, "", 0 );
    SET_STR( cint->env_string, "", 0 );
    SET_INT( cint->env_popup, 0 );
    SET_INT( cint->path_from_popup, 0 );
    SET_INT( cint->default_dir_toggle, 0 );
  } else {
    SET_INT( cint->path_from_popup, cint->entries[ cint->cur_entry ]->GetMode() );
    from_value = cint->entries[ cint->cur_entry ]->GetMode();

    SET_INT( cint->default_dir_toggle, ((cint->default_entry == cint->cur_entry) ? 1 : 0) );

    if( cint->entries[ cint->cur_entry ]->GetName() == NULL )
      SET_STR( cint->comment_string, "", 0 );
    else
      SET_STR( cint->comment_string, (char *)cint->entries[ cint->cur_entry ]->GetName(), strlen( cint->entries[ cint->cur_entry ]->GetName() ) );

    if( cint->entries[ cint->cur_entry ]->GetPath() == NULL )
      SET_STR( cint->path_string, "", 0 );
    else
      SET_STR( cint->path_string, (char *)cint->entries[ cint->cur_entry ]->GetPath(), strlen( cint->entries[ cint->cur_entry ]->GetPath() ) );

    if( cint->entries[ cint->cur_entry ]->GetEnvVar() == NULL )
      SET_STR( cint->env_string, "", 0 );
    else
      SET_STR( cint->env_string, (char *)cint->entries[ cint->cur_entry ]->GetEnvVar(), strlen( cint->entries[ cint->cur_entry ]->GetEnvVar() ) );
  }

  // Update Control Ghosts
  int state;
  GET_INT( cint->edit_button, state );

  if( from_value == CDFM_SPECIFIC_DIR ) {
    GHOST_CON( cint->env_string );
    GHOST_CON( cint->env_popup );

    ERASE_CON( cint->env_string );
    ERASE_CON( cint->env_popup );

    UNGHOST_CON( cint->path_string );
  } else {
    GHOST_CON( cint->path_string );
    ERASE_CON( cint->path_string );

    UNGHOST_CON( cint->env_string );
    UNGHOST_CON( cint->env_popup );
  }

  if( state ) {
    if( (cint->cur_entry == -1) || ( ((unsigned long)cint->cur_entry) >= cint->entries.NumElements()) ) {
      GHOST_CON( cint->duplicate_button );
      GHOST_CON( cint->remove_button );

      GHOST_CON( cint->path_from_popup );
      GHOST_CON( cint->comment_string );
      GHOST_CON( cint->default_dir_toggle );
      GHOST_CON( cint->test_path_button );

      if( from_value == CDFM_SPECIFIC_DIR ) {
        GHOST_CON( cint->path_string );
      } else {
        GHOST_CON( cint->env_string );
        GHOST_CON( cint->env_popup );
      }
    } else {
      UNGHOST_CON( cint->duplicate_button );
      UNGHOST_CON( cint->remove_button );

      UNGHOST_CON( cint->path_from_popup );
      UNGHOST_CON( cint->comment_string );
      UNGHOST_CON( cint->default_dir_toggle );
      UNGHOST_CON( cint->test_path_button );

      if( from_value == CDFM_SPECIFIC_DIR ) {
        UNGHOST_CON( cint->path_string );
      } else {
        UNGHOST_CON( cint->env_string );
        UNGHOST_CON( cint->env_popup );
      }
    }
  }
}

// HandleAddButton():
void HandleAddButton( LWControl *con, void *data ) {
  cint->entries.Add( new ContentDirector_Entry );
  SET_INT( cint->dir_list, (cint->entries.NumElements() - 1) );
  HandleDirList( cint->dir_list, NULL );
  REDRAW_CON( cint->dir_list );
}

// HandleAddCurrentButton():
void HandleAddCurrentButton( LWControl *con, void *data ) {
  if( cint->cur_content_entry == -1 ) {
    // Current Content doesn't match any existing content dirs; make a new one
    cint->entries.Add( new ContentDirector_Entry );
    unsigned long index = cint->entries.NumElements() - 1;

    const char * cur_content_dir = cint->dirinfo( "Content" );
    if( cur_content_dir == NULL )
      cur_content_dir = "";

    cint->entries[ index ]->SetName( "" );
    cint->entries[ index ]->SetPath( cur_content_dir );

    SET_INT( cint->dir_list, (int)index );
    HandleDirList( cint->dir_list, NULL );
    REDRAW_CON( cint->dir_list );
  } else {
    // Entry already exists; duplicate it
    HandleDuplicateButton( cint->duplicate_button, NULL );
  }

  HandleSelectCurrentButton( cint->current_dir_select, (void *)-1 );
}

// HandleDuplicateButton():
void HandleDuplicateButton( LWControl *con, void *data ) {
  cint->entries.Add( new ContentDirector_Entry );

  unsigned long index = cint->entries.NumElements() - 1;
  cint->entries[ index ]->SetName( cint->entries[ cint->cur_entry ]->GetName() );
  cint->entries[ index ]->SetPath( cint->entries[ cint->cur_entry ]->GetPath() );

  SET_INT( cint->dir_list, (int)index );
  HandleDirList( cint->dir_list, NULL );
  REDRAW_CON( cint->dir_list );
}

// HandleRemoveButton():
void HandleRemoveButton( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  delete cint->entries[ cint->cur_entry ];
  cint->entries.Remove( cint->cur_entry );
  cint->cur_entry--;

  SET_INT( cint->dir_list, cint->cur_entry );
  HandleDirList( cint->dir_list, NULL );
  REDRAW_CON( cint->dir_list );

  if( cint->cur_content_entry == cint->cur_entry )
    HandleSelectCurrentButton( cint->current_dir_select, (void *)-1 );
}

// HandleShiftUpButton():
void HandleShiftUpButton( LWControl *con, void *data ) {
  if( cint->cur_entry < 1 )
    return;

  cint->entries.Swap( cint->cur_entry, cint->cur_entry - 1 );

  if( cint->cur_content_entry == cint->cur_entry )
    cint->cur_content_entry--;
  else if( cint->cur_content_entry == (cint->cur_entry - 1) )
    cint->cur_content_entry++;

  if( cint->default_entry == cint->cur_entry )
    cint->default_entry--;
  else if( cint->default_entry == (cint->cur_entry - 1) )
    cint->default_entry++;

  cint->cur_entry--;
  SET_INT( cint->dir_list, cint->cur_entry );
  REDRAW_CON( cint->dir_list );
}

// HandleShiftDownButton():
void HandleShiftDownButton( LWControl *con, void *data ) {
  if( (unsigned long)cint->cur_entry > (cint->entries.NumElements() - 2) )
    return;

  cint->entries.Swap( cint->cur_entry, cint->cur_entry + 1 );

  if( cint->cur_content_entry == cint->cur_entry )
    cint->cur_content_entry++;
  else if( cint->cur_content_entry == (cint->cur_entry + 1) )
    cint->cur_content_entry--;

  if( cint->default_entry == cint->cur_entry )
    cint->default_entry++;
  else if( cint->default_entry == (cint->cur_entry + 1) )
    cint->default_entry--;

  cint->cur_entry++;
  SET_INT( cint->dir_list, cint->cur_entry );
  REDRAW_CON( cint->dir_list );
}

// HandleCommentString():
void HandleCommentString( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( cint->comment_string, buffer, MAX_PATH_LENGTH );
  cint->entries[ cint->cur_entry ]->SetName( buffer );

  if( cint->cur_content_entry == cint->cur_entry )
    HandleSelectCurrentButton( cint->current_dir_select, (void *)-1 );

  REDRAW_CON( cint->dir_list );
}

// HandleDefaultDirToggle():
void HandleDefaultDirToggle( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  int state;
  GET_INT( cint->default_dir_toggle, state );

  cint->default_entry = (state == 0) ? -1 : cint->cur_entry;

  REDRAW_CON( cint->dir_list );
}

// HandlePathString():
void HandlePathString( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( cint->path_string, buffer, MAX_PATH_LENGTH );

  DirStrings::RemoveTrailingDirSeperator( buffer );
  cint->entries[ cint->cur_entry ]->SetPath( buffer );

  SET_STR( cint->path_string, buffer, MAX_PATH_LENGTH );

  cint->entries[ cint->cur_entry ]->CheckExists();

  if( cint->cur_content_entry == cint->cur_entry )
    HandleSelectCurrentButton( cint->current_dir_select, (void *)-1 );

  REDRAW_CON( cint->dir_list );
}

// HandleTestPath():
void HandleTestPathButton( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  if( cint->entries[ cint->cur_entry ]->CheckExists() )
    cint->SetStatus( "Test Passed:  Directory found", true );
  else
    cint->SetStatus( "Test Failed:  Directory doesn't exist", false );
}

// HandlePathFromPopup():
void HandlePathFromPopup( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  int value;
  GET_INT( cint->path_from_popup, value );
  cint->entries[ cint->cur_entry ]->SetMode( (CDEntryFromModes)value );

  cint->entries[ cint->cur_entry ]->CheckExists();

  HandleDirList( cint->dir_list, NULL );
  REDRAW_CON( cint->dir_list );
}

// HandleEnvString():
void HandleEnvString( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( cint->env_string, buffer, MAX_PATH_LENGTH );
  cint->entries[ cint->cur_entry ]->SetEnvVar( buffer );

  cint->entries[ cint->cur_entry ]->CheckExists();

  REDRAW_CON( cint->dir_list );
}

// HandleEnvPopup():
void HandleEnvPopup( LWControl *con, void *data ) {
  if( cint->cur_entry == -1 )
    return;

  int value;
  GET_INT( cint->env_popup, value );

  cint->entries[ cint->cur_entry ]->SetEnvVar( cint->env_vars[ value ] );
  SET_STR( cint->env_string, (char*)cint->entries[ cint->cur_entry ]->GetEnvVar(), strlen( cint->entries[ cint->cur_entry ]->GetEnvVar() ) );

  cint->entries[ cint->cur_entry ]->CheckExists();

  REDRAW_CON( cint->dir_list );
}

// SetStatus():
void ContentDirector_Interface::SetStatus( const char *string, bool is_ok ) {
  StringTools::strdup2( cint->status_string, string );
  cint->status_ok = is_ok;

  DrawStatusField( cint->status_field, NULL, DR_REFRESH );
}

// DrawStatusField():
void DrawStatusField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = cint->panel_funcs->drawFuncs;

  if( cint->status_string == NULL ) {
    (*df->drawRGBBox)( cint->panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );
    (*df->drawText)( cint->panel, "Welcome to Content Director!", COLOR_WHITE, cx + 3, cy + 2 );
  } else if( cint->status_ok ) {
    (*df->drawRGBBox)( cint->panel, 0, 60, 150, cx, cy + 1, cw, ch-2 );
    (*df->drawText)( cint->panel, cint->status_string, COLOR_WHITE, cx + 3, cy + 2 );
  } else {
    (*df->drawRGBBox)( cint->panel, 175, 0, 0, cx, cy + 1, cw, ch-2 );
    (*df->drawText)( cint->panel, cint->status_string, COLOR_LT_YELLOW, cx + 3, cy + 2 );
  }
}

// SaveSettings():
//  Saves content dir settings to the path specified.  If either file can't
//   be opened, the function returns false.  If remote_path is not NULL,
//   then the file at local_path will contain a remote_path and the file at
//   remote_path will contain all the data.
bool ContentDirector_Interface::SaveSettings( const char *local_path, char *remote_path ) {
  ofstream out( local_path );
  if( !out )
    return false;

  out << "TMP_CONDIR 0"   << endl;
  out << endl;

  if( remote_path != NULL ) {
    out << "RemotePath " << remote_path << endl;
    out << "UseRemotePath" << endl;
    out.close();

    out.open( remote_path );
    if( !out )
      return false;

    out << "TMP_CONDIR 0"   << endl;
    out << endl;
  }

  if( default_entry != -1 )
    out << "QuickCDEntry " << default_entry << endl;

  int value;
  GET_INT( create_base_popup, value );
  out << "CreateBase " << value << endl;

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( cint->create_template_freq, buffer, MAX_PATH_LENGTH );
  if( buffer[0] != '\0' )
    out << "CreateTemplate " << buffer << endl;

  for( unsigned long i=0; i < entries.NumElements(); i++ )
    entries[i]->OutputToStream( out );

  return ((!out) ? false : true);
}

// LoadSettings():
//  Loads settings from the path specified.  If there is an error loading the
//   settings, an error will pop up and the function will return false.  If
//   recursed is true, the existing settings won't be cleared.  This is used
//   internally when loading a shared config from a local one.
bool ContentDirector_Interface::LoadSettings( const char *path, bool recursed ) {
  if( path == NULL )
    return false;

  if( path[0] == '\0' )
    return false;

  if( !DirInfo::Exists( path ) ) {
    (*message->error)( "ContentDirector Error:  Config file doesn't exist", path );
    return false;
  }

  ifstream in( path );
  if( !in ) {
    (*message->error)( "ContentDirector Error:  Unable to open config file", path );
    return false;
  }

  char buffer[ 1024 ];
  in >> buffer;
  if( stricmp( buffer, "TMP_CONDIR" ) != NULL ) {
    (*message->error)( "ContentDirector Error:  File is not a ContentDirector config", path );
    return false;
  }

  int value;
  in >> value;
  if( value != 0 ) {
    (*message->error)( "ContentDirector Error:  Unsupported config file version", path );
    return false;
  }

  // Setup
  if( !recursed ) {
    cur_entry = -1;
    cur_content_entry = -1;
    entries.Flush();

    SET_INT( cint->dir_list, -1 );
    SET_INT( cint->config_from_popup, 0 );
  }

  // Read in the data
  while( !in.eof() ) {
    buffer[0] = '\0';
    in >> buffer;

    if( (in.fail() || in.bad()) && !(in.eof()) ) {
      (*message->error)( "ContentDirector Error:  File I/O error occured while reading config", path );
      return false;
    }

    if( stricmp( buffer, "RemotePath" ) == 0 ) {
      in.getline( buffer, MAX_PATH_LENGTH );
      StringTools::RemoveCappingWhiteSpace( buffer );
      SET_STR( cint->config_filebutton, buffer, strlen( buffer ) );
    } else if( stricmp( buffer, "UseRemotePath" ) == 0 ) {
      SET_INT( cint->config_from_popup, 1 );
      GET_STR( cint->config_filebutton, buffer, MAX_PATH_LENGTH ) ;
      return LoadSettings( buffer, true );
    } else if( stricmp( buffer, "Entry" ) == 0 ) {
      ContentDirector_Entry *new_entry = new ContentDirector_Entry;
      if( new_entry->InputFromStream( in ) )
        entries.Add( new_entry );
    } if( stricmp( buffer, "QuickCDEntry" ) == 0 ) {
      in >> default_entry;
      in.getline( buffer, MAX_PATH_LENGTH );
    } if( stricmp( buffer, "CreateBase" ) == 0 ) {
      int temp;
      in >> temp;
      SET_INT( create_base_popup, temp );
      in.getline( buffer, MAX_PATH_LENGTH );
    } if( stricmp( buffer, "CreateTemplate" ) == 0 ) {
      in.getline( buffer, MAX_PATH_LENGTH );
      StringTools::RemoveCappingWhiteSpace( buffer );
      SET_STR( create_template_freq, buffer, strlen(buffer) );
    }
  }

  // Update the UI
  HandleDirList( dir_list, NULL );
  HandleConfigFromPopup( config_from_popup, NULL );
  HandleSelectCurrentButton( current_dir_select, NULL );
  HandleCreateBasePopup( create_base_popup, NULL );
  return true;
}

// HandleConfigFromPopup():
void HandleConfigFromPopup( LWControl *con, void *data ) {
  int state;
  GET_INT( cint->config_from_popup, state );

  if( state == 0 )
    GHOST_CON( cint->config_filebutton );
  else
    UNGHOST_CON( cint->config_filebutton );
}

// HandleConfigFileButton():
void HandleConfigFileButton( LWControl *con, void *data ) {
  char buffer[ 2 ];
  GET_STR( cint->config_filebutton, buffer, 1 );
  if( buffer[0] == '\0' )
    SET_STR( cint->config_filebutton, "ContentDirector-Shared.cfg", strlen( "ContentDirector-Shared.cfg" ) );
}

// HandleReloadButton():
void HandleReloadButton( LWControl *con, void *data ) {
  int state;
  GET_INT( cint->config_from_popup, state );

  char config_path[ MAX_PATH_LENGTH ];
  if( state == 0 ) {
    // Local Config
    const char *settings_path = cint->dirinfo( "Settings" );

    if( settings_path == NULL ) {
      (*cint->message->error)( "ContentDirector Error:  Settings path invalid; unable to load config",
                               "Any changes to content directory entries will not be saved." );
      cint->SetStatus( "Error loading settings", false );
    } else {
      strcpy( config_path, settings_path );
      DirStrings::AddPathPart( config_path, "ContentDirector.cfg" );
    }
  } else {
    // Shared Config
    GET_STR( cint->config_filebutton, config_path, MAX_PATH_LENGTH );
  }

  if( cint->LoadSettings( config_path ) )
    cint->SetStatus( "Settings Loaded Successfully", true );
  else
    cint->SetStatus( "Error loading settings", false );

  // Set the pop-up back to it's original state
  SET_INT( cint->config_from_popup, state );
}

// HandleSelectCurrentButton():
void HandleSelectCurrentButton( LWControl *con, void *data ) {
  const char *comment = "(none)";

  const char * cur_content_dir = cint->dirinfo( "Content" );
  cint->cur_content_entry = -1;

  const char *path;
  if( cur_content_dir != NULL ) {
    for( unsigned long i=0; i < cint->entries.NumElements(); i++ ) {
      path = NULL;
      if( cint->entries[ i ]->GetMode() == CDFM_SPECIFIC_DIR ) {
        path = cint->entries[ i ]->GetPath();
      } else {
        if( cint->entries[ i ]->GetEnvVar() != NULL )
          path = getenv( cint->entries[ i ]->GetEnvVar() );
      }

      if( path != NULL ) {
        if( stricmp( cur_content_dir, path ) == 0 ) {
          if( cint->entries[i]->GetName() != NULL )
            comment = cint->entries[i]->GetName();

          cint->cur_content_entry = i;
          break;
        }
      }
    }
  }

  SET_STR( cint->current_dir_comment, (char *)comment, strlen( comment ) );

  if( data != (void *)-1 ) {
    SET_INT( cint->dir_list, cint->cur_content_entry );
    HandleDirList( cint->dir_list, NULL );
    REDRAW_CON( cint->dir_list );
  }
}

// HandleChangeContentDirButton():
void HandleChangeContentDirButton( LWControl *con, void *data ) {
  if( cint->entries.NumElements() == 0 )    // Return silently if there are no entries
    return;

  if( cint->cur_entry == -1 ) {
    (*cint->message->error)( "ContentDirector Error:  You must first select an content dir to switch to.", NULL );
    cint->SetStatus( "No content directory selected", false );
    return;
  }

  const char *path = NULL;
  if( cint->entries[ cint->cur_entry ]->GetMode() == CDFM_SPECIFIC_DIR ) {
    path = cint->entries[ cint->cur_entry ]->GetPath();
  } else {
    if( cint->entries[ cint->cur_entry ]->GetEnvVar() != NULL )
      path = getenv( cint->entries[ cint->cur_entry ]->GetEnvVar() );
  }

  if( path == NULL ) {
    (*cint->message->error)( "ContentDirector Error:  Can't change content dir; the path is invalid", NULL );
    cint->SetStatus( "Can't change content dir to invalid path", false );
    return;
  }

  if( !DirInfo::Exists( path ) ) {
    (*cint->message->error)( "ContentDirector Error:  Can't change content dir; the path doesn't exist", path );
    cint->SetStatus( "Can't change content dir to invalid path", false );
    return;
  }

  char buffer[ MAX_PATH_LENGTH + 100 ];
  sprintf( buffer, "ContentDirectory %s", path );
  (*cint->generic->evaluate)( cint->generic->data, buffer );

  sprintf( buffer, "Content Dir is now \"%s\"", path );
  cint->SetStatus( buffer, true );

  const char * cur_content_dir = cint->dirinfo( "Content" );
  if( cur_content_dir == NULL )
    SET_STR( cint->current_dir_path, "(none)", strlen( "(none)" ) );
  else
    SET_STR( cint->current_dir_path, (char *)cur_content_dir, strlen( cur_content_dir ) );

  HandleSelectCurrentButton( cint->current_dir_select, (void *)-1 );
}

// CreateCreatePanel():
void ContentDirector_Interface::CreateCreatePanel() {
  // Create the panel
  create_panel = (*panel_funcs->create)( "Create Content Directory Structure", panel_funcs );
  if( !create_panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating Create Content Directory Structure panel, aborting", NULL );
    return;
  }

  // Add
  create_add_button = BOOL_CTL( panel_funcs, create_panel, "Add To List" );
  SET_INT( create_add_button, 1 );

  // Root
  create_root_freq = DIR_CTL( panel_funcs, create_panel, "Content Root", 40 );
  PanelTools::PutOnLineWith( create_add_button, create_root_freq, 10 );
  PanelTools::PutOnLineWith( create_root_freq, create_add_button, 10 );       // Yeah, it's weird, but it works

  // Base Mode
  create_base_popup = WPOPUP_CTL( panel_funcs, create_panel, "Base", create_base_popup_strings, 70 );

  // Template
  create_template_freq = LOAD_CTL( panel_funcs, create_panel, "Template", 40 );
  PanelTools::AlignLabels( create_root_freq, create_template_freq );

  SET_STR( create_template_freq, "*.txt", strlen( "*.txt" ) );

  // Save Example
  create_save_example = SAVEBUTTON_CTL( panel_funcs, create_panel, "Save Example", 85 );
  PanelTools::PutOnLineWith( create_template_freq, create_save_example );
  PanelTools::AlignLabels( create_add_button, create_save_example );
  SET_STR( create_save_example, "ContentDirectorExample.txt", strlen( "ContentDirectorExample.txt" ) );

  // Status Line
  create_status = CANVAS_CTL( panel_funcs, create_panel, "", 389, (CON_HOTH( create_base_popup )) );
  PanelTools::PutUnder( create_template_freq, create_status, 20 );

  // Create Button
  create_now = WBUTTON_CTL( panel_funcs, create_panel, "Create Dirs", 85 );
  PanelTools::PutOnLineWith( create_status, create_now, 5 );

  // Set the Draw Callbacks
  (*panel_funcs->set)( create_panel, PAN_USERDRAW, DrawCreatePanel );

  ival.intv.value = (int)&DrawCreateStatusField;
  (*create_status->set)( create_status, CTL_USERDRAW, &ival );

  // Set the Event Handlers
  CON_SETEVENT( create_base_popup,   HandleCreateBasePopup, NULL );
  CON_SETEVENT( create_save_example, HandleSaveExample,     NULL );
  CON_SETEVENT( create_now,          HandleCreateNow,       NULL );

  (*panel_funcs->set)( create_panel, PAN_USEROPEN, (void *)HandleCreatePanelOpen );

  HandleCreateBasePopup( create_base_popup, NULL );
};


// HandleCreateButton():
void HandleCreateButton( LWControl *con, void *data ) {
  cint->SetCreateStatus( NULL, 0.0f );
  (*cint->panel_funcs->open)( cint->create_panel, PANF_BLOCKING );
}

// DrawCreatePanel() {
void DrawCreatePanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( cint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( cint->panel_funcs, cint->create_panel );
  int pw = PAN_GETW( cint->panel_funcs, cint->create_panel );

  // Horizontal Dividers
  int y = CON_Y( cint->create_template_freq );
  int h = CON_H( cint->create_template_freq );
  PanelDraw::HorizontalDivider( cint->create_panel, &df, 0, y + h + 3, pw, 1 );
}

// HandleCreateBasePopup():
void HandleCreateBasePopup( LWControl *con, void *data ) {
  int state;
  GET_INT( con, state );

  if( state == 0 ) {
    GHOST_CON( cint->create_template_freq );
  } else {
    UNGHOST_CON( cint->create_template_freq );
  }
}

// DrawCreateStatusField():
void DrawCreateStatusField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = cint->panel_funcs->drawFuncs;

  if( cint->create_status_string == NULL ) {
    (*df->drawRGBBox)( cint->create_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );
    (*df->drawText)( cint->create_panel, "Click \"Create Dirs\" to build the content directory structure.", COLOR_WHITE, cx + 3, cy + 2 );
  } else {
    if( cint->create_progress < 0.0f ) {
      (*df->drawRGBBox)( cint->create_panel, 175, 0, 0, cx, cy + 1, cw, ch-2 );
      (*df->drawText)( cint->create_panel, cint->create_status_string, COLOR_LT_YELLOW, cx + 3, cy + 2 );
    } else {
      // Progress Bars/Status
      (*df->drawRGBBox)( cint->create_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

      // Draw the progress bar
      (*df->drawRGBBox)( cint->create_panel, 0, 60, 150, cx+1, cy+1,
                        (int)((cw-2) * cint->create_progress), ch-2 );

      // Write some text
      (*df->drawText)( cint->create_panel, cint->create_status_string, COLOR_WHITE, cx + 3, cy + 2 );
    }
  }
}

// HandleCreatePanelOpen():
void HandleCreatePanelOpen( LWPanelID pan, void *data ) {
  int pw = PAN_GETW( cint->panel_funcs, cint->create_panel );
  int ph = PAN_GETH( cint->panel_funcs, cint->create_panel );

  int main_x = PAN_GETX( cint->panel_funcs, cint->panel );
  int main_y = PAN_GETY( cint->panel_funcs, cint->panel );
  int main_w = PAN_GETW( cint->panel_funcs, cint->panel );
  int main_h = PAN_GETH( cint->panel_funcs, cint->panel );

  MOVE_PAN( cint->panel_funcs, cint->create_panel, main_x + (main_w/2 - pw/2),
                                                   main_y + (main_h/2 - ph/2) );
}

// HandleCreateNow():
void HandleCreateNow( LWControl *con, void *data ) {
  char root_path[ MAX_PATH_LENGTH ], path[ MAX_PATH_LENGTH ];
  GET_STR( cint->create_root_freq, path, MAX_PATH_LENGTH );

  strcpy( root_path, cint->dirinfo( "Content" ) );
  DirStrings::AddPathPart( root_path, path );

  // See if this path already is a content dir
  for( unsigned long i=0; i < cint->entries.NumElements(); i++ ) {
    if( stricmp( root_path, cint->entries[i]->GetPath() ) == 0 ) {
      char error_buffer[ MAX_PATH_LENGTH + 50 ];
      sprintf( error_buffer, "\"%s\".  Create dirs anyway?", root_path );
      if( (*cint->message->okCancel)( "Warning:  Content Dir Exists",
                                      "Content Director Warning:  A content directory already exists at",
                                      error_buffer ) == 0 ) {
        cint->SetCreateStatus( "User Abort:  Content Directory already exists at root", -1.0f );
        return;
      }
      break;
    }
  }

  // Determine the base
  int base_state;
  GET_INT( cint->create_base_popup, base_state );

  DynArray< char * > subdirs;
  if( base_state == 0 ) {                          // Default Mode
    for( int i=0; default_subdirs[i] != NULL; i++ )
      subdirs.Add( (char *)default_subdirs[i] );
  } else if( base_state == 2 ) {                   // Template File
    char template_path[ MAX_PATH_LENGTH ];
    GET_STR( cint->create_template_freq, template_path, MAX_PATH_LENGTH );
    if( !ReadTemplate( template_path, subdirs ) )
      return;
  }

  // Create the directory structure
  if( !DirInfo::Exists( root_path ) ) {
    if( !DirInfo::MakeTree( root_path ) ) {
      cint->SetCreateStatus( "Unable to create root of content directroy; creation aborted", -1.0f );
      (*cint->message->error)( "Content Director Error:  Unable to create root of content directory; aborting:", root_path );

      if( base_state == 2 )      // Do NOT free strings in default mode; they're not dynamically allocated
        subdirs.Flush();
      return;
    }
  }

  int add_state;
  GET_INT( cint->create_add_button, add_state );

  char progress_string[ MAX_PATH_LENGTH + 50 ];
  for( i=0; i < subdirs.NumElements(); i++ ) {
    strcpy( path, root_path );
    DirStrings::AddPathPart( path, subdirs[i] );

    sprintf( progress_string, "Creating \"%s\"...", subdirs[i] );
    cint->SetCreateStatus( progress_string, ((float)i) / ((float)subdirs.NumElements() + (add_state * 2) ) );

    if( !DirInfo::MakeTree( path ) ) {
      (*cint->message->error)( "Content Director Error:  Unable to create sub-directory; aborting:", path );
      cint->SetCreateStatus( "Unable to create sub-directory; creation aborted", -1.0f );

      if( base_state == 2 )      // Do NOT free strings in default mode; they're not dynamically allocated
        subdirs.Flush();
      return;
    }
  }

  // Free the subdir list
  if( base_state == 2 )      // Do NOT free strings in default mode; they're not dynamically allocated
    subdirs.Flush();

  // Add the item to the list, if enabled
  if( add_state == 1 ) {
    cint->SetCreateStatus( "Adding new content directory to list...", ((float)i+1) / ((float)subdirs.NumElements() + 2) );

    ContentDirector_Entry *entry = new ContentDirector_Entry;
    cint->entries.Add( entry );

    DirStrings::RemoveTrailingDirSeperator( root_path );

    entry->SetName( DirStrings::HasFile( root_path ) );
    entry->SetPath( root_path );
    entry->CheckExists();

    SET_INT( cint->dir_list, (cint->entries.NumElements() - 1) );
    HandleDirList( cint->dir_list, NULL );
  }

  // Report that everything is OK
  cint->SetCreateStatus( "Content Directory Creation Complete!", 1.0f );
  (*cint->message->error)( "Content Directory Structure successfully created in", root_path );
}

// SetCreateStatus():
void ContentDirector_Interface::SetCreateStatus( const char *string, float progress ) {
  StringTools::strdup2( cint->create_status_string, string );
  cint->create_progress = progress;
  DrawCreateStatusField( cint->create_status, NULL, DR_REFRESH );
}

// ReadTemplate():
bool ReadTemplate( const char *template_path, DynArray< char * > &subdirs ) {
  if( template_path[0] == '\0' ) {
    cint->SetCreateStatus( "Invalid template specified", -1.0f );
    (*cint->message->error)( "Content Director Error:  You must supply a valid template; aborting.", NULL );
    return false;
  }

  if( !DirInfo::Exists( template_path ) ) {
    cint->SetCreateStatus( "Template doesn't exist", -1.0f );
    (*cint->message->error)( "Content Director Error:  The following template file doesn't exist; aborting:", template_path );
    return false;
  }

  pifstream in( template_path );
  if( !in ) {
    cint->SetCreateStatus( "Error occured while opening template", -1.0f );
    (*cint->message->error)( "Content Director Error:  An error occured opening the template file; aborting:", template_path );
    return false;
  }

  char buffer[ 4096 ];
  in.GuessEOLType();

  in >> buffer;
  if( stricmp( buffer, "ContentDirectorTemplate" ) != 0 ) {
    cint->SetCreateStatus( "File is not a Content Director Template", -1.0f );
    (*cint->message->error)( "Content Director Error:  File is not a Content Director Template:", template_path );
    return false;
  }

  int version;
  in >> version;
  if( version != 1 ) {
    cint->SetCreateStatus( "Unsupported template version", -1.0f );
    (*cint->message->error)( "Content Director Error:  Unsupported version found in template:", template_path );
    return false;
  }

  // Skip the rest of the line
  in.getline( buffer, 4096 );

  char *temp;
  do {
    in.getline( buffer, 4096 );

    if( (in.bad() || in.fail()) && !in.eof() ) {
      cint->SetCreateStatus( "File I/O error while reading from template", -1.0f );
      (*cint->message->error)( "Content Director Error:  File I/O error occured reading from template file", template_path );
      subdirs.Flush();
    }

    // Remove any capping white space
    StringTools::RemoveCappingWhiteSpace( buffer );

    // Test for blank lines
    if( buffer[0] == '\0' )
      continue;

    // Test for comment lines
    if( buffer[0] == ';' )
      continue;

    // See if it has a drive part (it shouldn't)
    if( DirStrings::HasDrive( buffer ) ) {
      cint->SetCreateStatus( "Template cannot contain absolute paths", -1.0f );
      (*cint->message->error)( "Content Director Error:  Template cannot contain absolute paths; aborting", template_path );
      subdirs.Flush();
    }

    // Add the new directory
    temp = NULL;
    subdirs.Add( StringTools::strdup2( temp, buffer ) );
  } while( !in.eof() );

  return true;
}

// HandleSaveExample():
void HandleSaveExample( LWControl *con, void *data ) {
  char path[ MAX_PATH_LENGTH ];
  GET_STR( cint->create_save_example, path, MAX_PATH_LENGTH );

  // See if the user aborted (ie: empty string)
  if( path[0] == '\0' )
    return;

  pofstream out( path );
  if( !out ) {
    (*cint->message->error)( "Content Director Error:  Error creating example template; aborting:", path );
      cint->SetCreateStatus( "Error creating example template; save aborted", -1.0f );
    return;
  }

  out << "ContentDirectorTemplate 1" << out.endl();
  out << "; This is a comment.  Any line that starts with a semi-colon is a comment." << out.endl();
  out << ";  The first line of a Content Director Template must be exactly as above," << out.endl();
  out << ";  containing first the word ContentDirectorTemplate, and then the number"  << out.endl();
  out << ";  1 (one).  The number is the version of the template, which may change"   << out.endl();
  out << ";  as more features are added in the future." << out.endl();
  out << ";" << out.endl();
  out << "; Insert the names of the directories you want to create after these"         << out.endl();
  out << ";  comments.  The paths should be relative to the content directory, so no"   << out.endl();
  out << ";  drive letters.  Sub-directories can also be specified, such as"            << out.endl();
  out << ";  Scenes/Test.  All blank and commented lines will be skipped. All leading"  << out.endl();
  out << ";  and trailing white space will be removed before creating any directories." << out.endl();
  out << ";  Each directory must otherwise be on it's own line." << out.endl();
  out << "" << out.endl();

  out << "; Default Directories" << out.endl();
  for( int i=0; default_subdirs[i] != NULL; i++ )
    out << default_subdirs[i] << out.endl();

  if( !out ) {
    cint->SetCreateStatus( "File I/O error writing example template, save aborted", -1.0f );
    (*cint->message->error)( "Content Director Error:  File I/O error occured writing example template; aborting:", path );
  }
}

