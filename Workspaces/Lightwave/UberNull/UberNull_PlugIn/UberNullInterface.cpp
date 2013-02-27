//
// UberNullInterface.cpp
//
// This file contains the implementation fo the UberNull Interface
// Class, responsible for creating and maintaining the UberNull UI.
//

#include <stdio.h>

#include "UberNullInterface.h"
#include "UberNullLogo.h"

UberNullInterface *un_int = NULL;
UberNull          *un     = NULL;
UberNullLogo       logo;

// Constructor
UberNullInterface::UberNullInterface( GlobalFunc *_global,
                                      void * local,
                                      int &retval )
  : main_panel(NULL), about_panel(NULL),
    Global(*_global), message(NULL), panel_funcs(NULL), scene_info(NULL),
    content_dir(NULL), selected_name(-1), name_top(0) {

  // Set the global UberNullInterface pointer so the draw functions and controls can use it
  un_int = this;

  // Get the Panel Functions and Scene Info from Lightwave
  message     = CMessage::New( (MessageFuncs *) Global( "Info Messages", GFUSE_TRANSIENT) );
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global( PANEL_SERVICES_NAME, GFUSE_TRANSIENT ), _global );
  scene_info  = CLWSceneInfo::New( (LWSceneInfo *) Global( "LW Scene Info", GFUSE_TRANSIENT ) );

  if( !message ) {
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !panel_funcs ) {
    message->Error( "ERROR: Unable to find lwpanels.p to create the GUI",
                    "Add it and try again." );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !scene_info ) {
    message->Error( "ERROR: Unable to obtain Scene Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  // Get the Dir Info Function so we can get the Content Dir
  DirInfoFunc *DirInfo = *( (DirInfoFunc *) Global( "Directory Info", GFUSE_TRANSIENT ) );
  if( DirInfo == NULL ) {
    message->Error( "ERROR: Unable to obtain Directory Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  content_dir = (*DirInfo)( "Content" );

  // Create the UberNull instance and initialize it to scene data
  if( un == NULL ) {
    un = new UberNull( scene_info->filename );
    if( !un ) {
      message->Error( "ERROR: Unable to create UberNull class; aborting" );
      retval = AFUNC_BADGLOBAL;
      return;
    }
  }

  if( stricmp( scene_info->filename, "(unnamed)" ) != 0 ) {
    // Create the input scene's filename
    char default_source[ MAX_PATH_LENGTH ];
    strcpy( default_source, content_dir );
    DirStrings::AddPathPart( default_source, scene_info->filename );
    un->SetSource( default_source );
  }
  un->GenerateNames();

  // Create the panels
  if( MakeMainPanel() ) {                  // Create the main panel
    if( MakeAboutPanel() ) {               // Create the about panel
      if( OpenMainPanel() ) {
        retval = AFUNC_OK;
      } else {
        retval = AFUNC_BADLOCAL;
      }
    } else {
      retval = AFUNC_BADLOCAL;
    }
  } else {
    retval = AFUNC_BADLOCAL;
  }
}

// Destructor
UberNullInterface::~UberNullInterface() {
  panel_funcs->Destroy( main_panel  );
  panel_funcs->Destroy( about_panel );

  delete panel_funcs;
  delete scene_info;
  delete message;
}

// MakeMainPanel():
bool UberNullInterface::MakeMainPanel() {
  // Set up Main Title String
  sprintf( main_title, "UberNull V%d.%d.%d from TM Productions (%s)",
           UberNull_Version, UberNull_Revision, UberNull_SubRevision, __DATE__ );

  // Create the Panel
  main_panel = panel_funcs->Create( main_title );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  int x, y, w;
try {
  if( !( load_settings = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                         "Load Settings", 100 ) ) )
    throw false;

  if( !( save_settings = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                         "Save Settings", 100 ) ) )
    throw false;

  if( !( source_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                  "Source Scene", 68 ) ) )
    throw false;

  if( !( output_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                  "Output Scene", 68 ) ) )
    throw false;

  if( !( ubernull_master_name = STR_CTL( panel_funcs->orig_struct, main_panel,
                                         "Master Null Name", 50 ) ) )
    throw false;

  if( !( ubernull_master_enable = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                            "Use Master Null" ) ) )
    throw false;

  if( !( ubernull_master_location = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                                "Master Null Location", ubernull_location_strings, 200 ) ) )
    throw false;

  x = CON_X( load_settings );
  y = CON_Y( load_settings );
  MOVE_CON( load_settings, x, 66 );
  PanelTools::PutOnLineWith( load_settings, save_settings );

  PanelTools::PutUnder( load_settings, source_scene, 5 );
  x = CON_X( source_scene );
  y = CON_Y( source_scene );
  MOVE_CON( source_scene, x+16, y );

  LWControl *align_array[] =
    { source_scene, output_scene, ubernull_master_name, ubernull_master_location, NULL };
  PanelTools::AlignLabels( align_array, 4 );

  PanelTools::PutOnLineWith( ubernull_master_name, ubernull_master_enable );

  if( !( about = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                              "About", 80 ) ) )
    throw false;

  PanelTools::PutOnLineWith( ubernull_master_location, about );
  int pw = PAN_GETW( panel_funcs->orig_struct, main_panel );
  w = CON_HOTW( about );
  y = CON_Y( about );
  MOVE_CON( about, pw - w + 8, y );

  // Default Settings for String/File and Popup Controls
  SET_STR( load_settings,            "*.ubn",                         4 );
  SET_STR( save_settings,            "*.ubn",                         4 );
  SET_STR( source_scene,             (char *)un->GetSource(),         strlen( un->GetSource()         ) );
  SET_STR( output_scene,             (char *)un->GetOutput(),         strlen( un->GetOutput()         ) );
  SET_STR( ubernull_master_name,     (char *)un->GetMasterNullName(), strlen( un->GetMasterNullName() ) );
  SET_INT( ubernull_master_location, un->GetMasterLocation() );
  SET_INT( ubernull_master_enable,   (int)un->GetUseMasterNull() );

  // Lister Controls
  if( !( name_listbox = AREA_CTL( panel_funcs->orig_struct, main_panel,
                                  "", 400, 84 ) ) )
    throw false;

  if( !( name_scroll = VSLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                    "", 88, 0, 0 ) ) )
    throw false;

  PanelTools::PutUnder( ubernull_master_location, name_listbox );
  x = CON_X( name_listbox );
  y = CON_Y( name_listbox );
  MOVE_CON( name_listbox, x, y+15 );

  PanelTools::PutOnLineWith( name_listbox, name_scroll, -20 );

  // Name List Controls
  if( !( name_use = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                              "Use Named Nulls" ) ) )
    throw false;

  if( !( name_add = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                 "Add Null", 132 ) ) )
    throw false;

  if( !( name_remove = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                   "Remove Null", 132 ) ) )
    throw false;

  if( !( name_clone = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                  "Clone Null", 132 ) ) )
    throw false;

  // Name Entry Controls
  if( !( name_enable = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                 "Enable Null" ) ) )
    throw false;

  if( !( name_null = STR_CTL( panel_funcs->orig_struct, main_panel,
                              "Null Name", 45 ) ) )
    throw false;

  if( !( name_location = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                     "Location", UberNull_NullLocations, 125 ) ) )
    throw false;

  if( !( name_match = STR_CTL( panel_funcs->orig_struct, main_panel,
                               "Match With", 45 ) ) )
    throw false;

  if( !( name_match_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                       "Match Mode", UberNull_MatchModes, 125 ) ) )
    throw false;

  SET_INT( name_use,        (bool)un->GetUseNames() );
  SET_STR( name_null,       "", 0 );
  SET_STR( name_match,      "", 0 );
  SET_INT( name_enable,     false );
  SET_INT( name_location,   0     );
  SET_INT( name_match_mode, 0     );

  PanelTools::PutOnLineWith( name_scroll,  name_use, 3 );
  PanelTools::PutOnLineWith( name_scroll,  name_add, 3 );
  PanelTools::PutUnder(      name_add,     name_add    );
  PanelTools::AlignWith(     name_add,     name_remove );
  PanelTools::PutUnder(      name_add,     name_remove );
  PanelTools::AlignWith(     name_remove,  name_clone  );
  PanelTools::PutUnder(      name_remove,  name_clone );

  PanelTools::PutUnder( name_listbox, name_enable );
  x = CON_X( name_enable );
  y = CON_Y( name_enable );
  MOVE_CON(  name_enable, x+71, y+5 );

  PanelTools::PutUnder( name_enable, name_null );
  PanelTools::PutOnLineWith( name_null, name_location, 5 );
  PanelTools::AlignLabels( name_null, name_match );
  PanelTools::PutOnLineWith( name_match, name_match_mode );
  PanelTools::AlignLabels( name_location, name_match_mode );

  // Ubernullify Button
  if( !( ubernullify = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                   "UberNullify", 200 ) ) )
    throw false;

  PanelTools::PutUnder( name_match, ubernullify, 12 );
  w = CON_HOTW( ubernullify );
  y = CON_Y( ubernullify );
  MOVE_CON( ubernullify, pw/2 - w/2 + 15, y );

  // Align the Load/Save Settings buttons
  w = CON_W( save_settings );
  y = CON_Y( save_settings );
  MOVE_CON( save_settings, pw - w + 35, y );
  x = CON_X( save_settings );
  MOVE_CON( load_settings, x - w + 5, y );

  // Set the Panel Size
  y = CON_Y( ubernullify );
  int h = CON_H( ubernullify );
  PAN_SETH( panel_funcs->orig_struct, main_panel, y+h+30 );

  // Set the Handler Functions
  CON_SETEVENT( source_scene,                OnSourceFile,         this );
  CON_SETEVENT( output_scene,                OnOutputFile,         this );
  CON_SETEVENT( ubernull_master_name,        OnUberNullName,       this );
  CON_SETEVENT( ubernull_master_location,    OnUberNullLocation,   this );
  CON_SETEVENT( ubernullify,                 OnUberNullify,        this );
  CON_SETEVENT( about,                       OnAbout,              this );
  CON_SETEVENT( name_scroll,                 OnNameScroll,         this );
  CON_SETEVENT( name_listbox,                OnNameListbox,        this );
  CON_SETEVENT( name_add,                    OnNameAdd,            this );
  CON_SETEVENT( name_remove,                 OnNameRemove,         this );
  CON_SETEVENT( name_clone,                  OnNameClone,          this );
  CON_SETEVENT( name_null,                   OnNameNull,           this );
  CON_SETEVENT( name_match,                  OnNameMatch,          this );
  CON_SETEVENT( name_match_mode,             OnNameMatchMode,      this );
  CON_SETEVENT( name_location,               OnNameLocation,       this );
  CON_SETEVENT( name_enable,                 OnNameEnable,         this );
  CON_SETEVENT( name_use,                    OnNameUse,            this );
  CON_SETEVENT( ubernull_master_enable,      OnEnableMaster,       this );
  CON_SETEVENT( save_settings,               OnSaveSettings,       this );
  CON_SETEVENT( load_settings,               OnLoadSettings,       this );

  // Set the Draw Functions
  panel_funcs->Set( main_panel, PAN_USERDRAW, DrawMainPanel );

  ival.intv.value = (int)&DrawNameListbox;
  (*name_listbox->set)( name_listbox, CTL_USERDRAW, &ival );

  // Update the Name List
  UpdateName( selected_name );
  OnEnableMaster( ubernull_master_enable, un_int );

  return true;
} catch( bool a ) {
  if( !a ) {
    message->Error("Error creating interface controls; aborting" );
    return false;
  }
}

return true;
}

// OpenMain():
bool UberNullInterface::OpenMainPanel() {
  if( !panel_funcs->Open( main_panel, PANF_BLOCKING | PANF_MOUSETRAP ) )
    return false;

  return true;
}

// DrawMainPanel():
void DrawMainPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs *df = un_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( un_int->panel_funcs->orig_struct, un_int->main_panel );
  int pw = PAN_GETW( un_int->panel_funcs->orig_struct, un_int->main_panel );

  int y = CON_Y( un_int->about );
  int h = CON_H( un_int->about );
  PanelDraw::HorizontalDivider( un_int->main_panel, df, 0, y+h+1, pw, 1 );

  y = CON_Y( un_int->ubernullify );
  PanelDraw::HorizontalDivider( un_int->main_panel, df, 0, y, pw, 1 );

  logo.DrawInLWPanels( un_int->main_panel, df, -15, 0 );
}

// MakeAboutPanel():
bool UberNullInterface::MakeAboutPanel() {
  // Create the Panel
  about_panel = panel_funcs->Create( main_title );

  if( !about_panel ) {                    // Make sure the panel was created
    message->Error("Error creating about panel, aborting" );
    return false;
  }

  PAN_SETW( panel_funcs->orig_struct, about_panel, 415 );
  PAN_SETH( panel_funcs->orig_struct, about_panel, 250 );

  // Set the Draw Functions
  panel_funcs->Set( about_panel, PAN_USERDRAW, DrawAboutPanel );

  return true;
}

// OpenAboutPanel()
//  Opens the about panel in blocking mode
bool UberNullInterface::OpenAboutPanel() {
  if( !panel_funcs->Open( about_panel, PANF_BLOCKING ) )
    return false;

  return true;
}

// OnAbout():
//  Called to open the About Box.
void OnAbout(LWControl *ctl, UberNullInterface *panel) {
  panel->OpenAboutPanel();
}

// DrawAboutPanel():
//  Called to draw the About box
void DrawAboutPanel( LWPanelID panel, void *data, DrMode mdoe ) {
  CDrawFuncs *df = un_int->panel_funcs->DrawFuncs;

  int y = 70;

  int ph = PAN_GETH( un_int->panel_funcs->orig_struct, un_int->about_panel );
  int pw = PAN_GETW( un_int->panel_funcs->orig_struct, un_int->about_panel );

  char buffer[256];
  sprintf( buffer, "%s V%d.%d.%d (%s)", about_text[0],
           UberNull_Version, UberNull_Revision, UberNull_SubRevision, __DATE__ );

  df->WriteText( un_int->about_panel, buffer, COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(un_int->about_panel, buffer))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( un_int->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(un_int->about_panel, (about_text[i]) ))/2,
             y );
    y += 15;
  }

  logo.DrawInLWPanels( un_int->about_panel, df, -190, 0 );
}

// OnSourceFile():
//  Called to handle a change in the Source File's name.
void OnSourceFile(LWControl *ctl, UberNullInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  if( buffer[0] == '\0' ) {
    SET_STR( un_int->source_scene,         (char *)un->GetSource(),         strlen( un->GetSource()         )  );
  } else {
    char new_source[ MAX_PATH_LENGTH ];
    strcpy( new_source, un_int->content_dir );
    DirStrings::AddPathPart( new_source, buffer );
    SET_STR( un_int->source_scene, new_source, strlen( new_source ) );

    // We just changed the source name, so re-generate the names as needed
    if( stricmp( un->GetMasterNullName(), "--UberNull--(unnamed)") == 0 )
      un->SetMasterNullName( NULL );

    un->SetSource( new_source );
    un->SetOutput( NULL );

    un->GenerateNames();

    SET_STR( un_int->output_scene,         (char *)un->GetOutput(),         strlen( un->GetOutput()         )  );
    SET_STR( un_int->ubernull_master_name, (char *)un->GetMasterNullName(), strlen( un->GetMasterNullName() )  );
  }
}

// OnOutputFile():
//  Called to handle a change in the Output File's name.  Currently,
//   this does nothing, and all output filename checks are done
//   at Process time.
void OnOutputFile(LWControl *ctl, UberNullInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  char new_output[ MAX_PATH_LENGTH ];
  strcpy( new_output, un_int->content_dir );
  DirStrings::AddPathPart( new_output, buffer );
  SET_STR( un_int->output_scene, new_output, strlen( new_output ) );
  un->SetOutput( new_output );
}

// OnUberNullName():
//  Called to handle a change in the UberNull's name.
void OnUberNullName(LWControl *ctl, UberNullInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );
  if( buffer[0] == '\0' )
    SET_STR( ctl, (char *)un->GetMasterNullName(), strlen( un->GetMasterNullName() ) );
  else
    un->SetMasterNullName( buffer );
}

// OnUberNullLocation():
//  Called to handle a change in the UberNull's location.  Currently,
//   this does nothing.
void OnUberNullLocation( LWControl *ctl, UberNullInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  un->SetMasterLocation( value );
}


// OnUberNullify():
//  Actually process the source scene file using the UberNull class
void OnUberNullify( LWControl *ctl, UberNullInterface *panel ) {
  // Open the Source and Output files and make sure the null name is OK
  int error = un->OpenFiles();
  if( error != UBERNULL_OK ) {
    un_int->message->Error( ubernull_error_strings[ error ] );
    return;
  }

  SET_STR( un_int->ubernullify, "Processing...", strlen( "Processing..." ) );

  // Process the source scene into the UberNull'ed output scene;
  error = un->Process();

  // Close the files when finished
  un->CloseFiles();

  SET_STR( un_int->ubernullify, "UberNullify", strlen( "UberNullify" ) );

  // Check for errors during processing
  if( error != UBERNULL_OK ) {  // Error
    un_int->message->Error( ubernull_error_strings[ error ] );
    return;
  } else {                      // All OK
    un_int->message->Info( "Source scene sucessfully UberNull'ed into",
                           un->GetOutput() );
    return;
  }
}


// OnNameListbox():
//  Handles changing which Name we're editing.
void OnNameListbox( LWControl *ctl, UberNullInterface *panel ) {
  // Figure out which item was clicked
  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int mx = CON_MOUSEX( ctl );
  int my = CON_MOUSEY( ctl );
  
  int index = (my / item_height) + un_int->name_top;
  if( index < (int)un->names.NumElements() ) {
    un_int->selected_name = index;

    // Test for an Enable/Disable click
    int cx = CON_HOTX( ctl );
    int mx = CON_MOUSEX( ctl );

    if( mx < 15 ) {
      SET_INT( un_int->name_enable, !un->names[ un_int->selected_name ]->GetEnable() );
      OnNameEnable( un_int->name_enable, un_int );
    }

    REDRAW_CON( un_int->name_listbox );
    un_int->UpdateName( un_int->selected_name );
  }
}

// OnNameScroll():
//  Handles adjusting the visible items via the scroll bar.
void OnNameScroll( LWControl *ctl, UberNullInterface *panel ) {
  GET_INT( ctl, un_int->name_top );
  REDRAW_CON( un_int->name_listbox );
}

// DrawNameListbox():
//  Called to draw the Palette Control
void DrawNameListbox( LWControl *ctl, UberNullInterface *si, DrMode mode ) {
  CDrawFuncs *df = un_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int ch = CON_HOTH( ctl );
  int cw = CON_HOTW( ctl );

  int item_text_color;

  // Clear the control
  df->DrawRGBBox( un_int->main_panel, 132, 130, 132, cx, cy, cw, ch );

  // Rows
  for( int j=0; j < ch/item_height; j++ ) {
    df->DrawRGBBox( un_int->main_panel, 112, 112, 112,
                    cx, cy+(j*item_height), cw, 1 );
  }

  // Main Columns
  df->DrawRGBBox( un_int->main_panel, 0, 0, 0,
                  cx+item_text_offset-5, cy, 1, ch );

  // Items in the list
  int   baseline;                                // Base of the current line
  char  name_buffer[1024];

  for( unsigned int i=un_int->name_top; i < un->names.NumElements(); i++ ) {
    baseline = cy + ((i - un_int->name_top) * item_height );

    // Figure out what color the text should be drawn in
    ctl->get( ctl, CTL_FLAGS, &ival );
    if( (mode == DR_GHOST) || ( ival.intv.value & CTLF_GHOST) )
      item_text_color = i == un_int->selected_name ? COLOR_DK_YELLOW : COLOR_LT_GREY;
    else
      item_text_color = i == un_int->selected_name ? COLOR_LT_YELLOW : COLOR_WHITE;

    // Make sure the name fits in the lister
    if( un->names[i]->GetName() == NULL )
      strcpy( name_buffer, "<Unnamed Null>" );
    else 
      strcpy( name_buffer, un->names[i]->GetName() );

    int len = strlen( name_buffer );
    while( df->TextWidth( un_int->main_panel, name_buffer ) > (cw - item_text_offset) )
      name_buffer[--len] = '\0';

    // Display the item name
    df->WriteText( un_int->main_panel, name_buffer,
                   item_text_color, cx+item_text_offset, baseline );

    // Enable Checkmark Icons
    if( un->names[i]->GetEnable() ) {
      PanelDraw::DrawLWIcon( un_int->main_panel, df,
                             cx + 1, baseline + 1,
                             PanelDraw::check, PanelDraw::grey );
    }

    // Check for the end of the list
    if( ch/item_height < (int)i - un_int->name_top + 2 )
      break;
  }
}

// UpdateName():
//  Updates the Name controls with the index provided into
//   the instance->name array.
void UberNullInterface::UpdateName( int index ) {
  // Handle unavailable name
  if( (index < 0) || (un->names.NumElements() == 0) ) {
    SET_STR( name_null,       "", 0 );
    SET_STR( name_match,      "", 0 );
    SET_INT( name_enable,     false );
    SET_INT( name_location,   0     );
    SET_INT( name_match_mode, 0     );
  } else {
    // Handle valid names
    if( un->names[ index ]->GetName() == NULL )
      SET_STR( name_null, "", 0 );
    else
      SET_STR( name_null,     (char *)un->names[ index ]->GetName(),        strlen( un->names[ index ]->GetName()        ) );

    if( un->names[ index ]->GetMatchString() == NULL )
      SET_STR( name_match, "", 0 );
    else
      SET_STR( name_match,    (char *)un->names[ index ]->GetMatchString(), strlen( un->names[ index ]->GetMatchString() ) );

    SET_INT( name_location,   (int)un->names[ index ]->GetLocation()  );
    SET_INT( name_match_mode, (int)un->names[ index ]->GetMatchMode() );
    SET_INT( name_enable,     (int)un->names[ index ]->GetEnable()    );
  }

  // Ghost main list controls, if applicable
  if( !un->GetUseNames() ) {
    GHOST_CON( name_listbox    );
    GHOST_CON( name_add        );
    GHOST_CON( name_remove     );
    GHOST_CON( name_clone      );
  } else {
    UNGHOST_CON( name_listbox  );
    UNGHOST_CON( name_add      );
    UNGHOST_CON( name_remove   );
    UNGHOST_CON( name_clone    );
  }

  // Ghost uneditable controls if apropriate
  if( (index < 0) || !un->GetUseNames() || (un->names.NumElements() == 0) ) {
    GHOST_CON( name_enable     );
    GHOST_CON( name_null       );
    GHOST_CON( name_match      );
    GHOST_CON( name_location   );
    GHOST_CON( name_match_mode );
  } else {
    UNGHOST_CON( name_enable   );
    UNGHOST_CON( name_null       );
    UNGHOST_CON( name_match      );
    UNGHOST_CON( name_location   );
    UNGHOST_CON( name_match_mode );
  }
}

// OnNameAdd():
//  Adds a new Name.
void OnNameAdd( LWControl *ctl, UberNullInterface *panel ) {
  // Create a new Name instance
  UberNullNames * new_name = new UberNullNames;
  un->names.Add( new_name );

  // Set the new instance in the Name list
  un_int->selected_name = un->names.NumElements() - 1;

  // Scroll the list, if needed
  int ch = CON_HOTH( un_int->name_listbox );
  unsigned int index = ( ch / item_height) + un_int->name_top;
  if( un_int->selected_name >= index )
    un_int->name_top++;

  // Update the controls
  ival.intv.value = un_int->name_top;
  un_int->name_scroll->set( un_int->name_scroll, CTL_RANGEMAX, &ival );
  SET_INT( un_int->name_scroll, un_int->name_top );

  REDRAW_CON( un_int->name_scroll );
  REDRAW_CON( un_int->name_listbox );

  un_int->UpdateName( un_int->selected_name );
}

// OnNameRemove():
//  Removes an existing Name.
void OnNameRemove( LWControl *ctl, UberNullInterface *panel ) {
  if( un_int->selected_name == -1 )
    return;

  delete un->names[ un_int->selected_name ];
  un->names.Remove( un_int->selected_name );

  if( un_int->selected_name >= un->names.NumElements() )
    un_int->selected_name--;

  // Scroll the list, if needed
  if( un_int->selected_name < (unsigned int)un_int->name_top )
    un_int->name_top = un_int->selected_name;

  if( un_int->name_top < 0 )
    un_int->name_top = 0;

  // Update the controls
  ival.intv.value = un_int->name_top;
  un_int->name_scroll->set( un_int->name_scroll, CTL_RANGEMAX, &ival );
  SET_INT( un_int->name_scroll, un_int->name_top );

  REDRAW_CON( un_int->name_scroll );
  REDRAW_CON( un_int->name_listbox );

  un_int->UpdateName( un_int->selected_name );
}

// OnNameClone():
//  Clones an existing Name.
void OnNameClone( LWControl *ctl, UberNullInterface *panel ) {
  if( un_int->selected_name < 0 )
    return;

  // Create a new Name instance
  UberNullNames * new_name = new UberNullNames;

  // Copy the data
  new_name->SetName(        un->names[ un_int->selected_name ]->GetName()        );
  new_name->SetMatchString( un->names[ un_int->selected_name ]->GetMatchString() );
  new_name->SetMatchMode(   un->names[ un_int->selected_name ]->GetMatchMode()   );
  new_name->SetLocation(    un->names[ un_int->selected_name ]->GetLocation()    );

  // Insert the name into the list
  if( un_int->selected_name == un->names.NumElements()-1 ) {
    un->names.Add( new_name );
  } else {
    un->names.InsertAt( un_int->selected_name + 1, new_name );
  }

  // Set the new instance in the Name list
  SET_INT( un_int->name_listbox, ++un_int->selected_name );
  REDRAW_CON( un_int->name_listbox );

  un_int->UpdateName( un_int->selected_name );
}


// OnNameEnable():
//  Toggles an Name's Enabled state.
void OnNameEnable( LWControl *ctl, UberNullInterface *panel ) {
  UberNullNames *name = un->names[ un_int->selected_name ];

  int state;
  GET_INT( ctl, state );

  if( state == true ) {
    if( name->GetName() == NULL ) {   // Still an Invalid path; disable preview
      un_int->message->Info("You must enter a valid Null Name before enabling it");
      SET_INT( un_int->name_enable, (int)false );
      name->SetEnable( false );
      return;
    }

    if( name->GetMatchString() == NULL ) {   // Still an Invalid path; disable preview
      un_int->message->Info("You must enter a valid Match String before enabling it");
      SET_INT( un_int->name_enable, (int)false );
      name->SetEnable( false );
      return;
    }
  }

  name->SetEnable( (bool) state );

  // Refresh the Name Listbox
  REDRAW_CON( un_int->name_listbox );
}

// OnNameLocation():
void OnNameLocation( LWControl *ctl, UberNullInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  un->names[ un_int->selected_name ]->SetLocation( (ubernullnames_location) value );
}

// OnNameMatchMode():
void OnNameMatchMode( LWControl *ctl, UberNullInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  un->names[ un_int->selected_name ]->SetMatchMode( (ubernullnames_match_modes) value );
}

// OnNameNull():
void OnNameNull( LWControl *ctl, UberNullInterface *panel ) {
  char buffer[ 1024 ];
  GET_STR( ctl, buffer, 1024 );

  if( buffer[0] == '\0' ) {
    if( un->names[ un_int->selected_name ]->GetName() != NULL ) {
      SET_STR( ctl, (char *)un->names[ un_int->selected_name ]->GetName(),
               strlen( un->names[ un_int->selected_name ]->GetName() ) );
    }
  } else {
    un->names[ un_int->selected_name ]->SetName( buffer );
    REDRAW_CON( un_int->name_listbox );
  }
}

// OnNameMatch():
void OnNameMatch( LWControl *ctl, UberNullInterface *panel ) {
  char buffer[ 1024 ];
  GET_STR( ctl, buffer, 1024 );

  if( buffer[0] == '\0' ) {
    if( un->names[ un_int->selected_name ]->GetMatchString() != NULL ) {
      SET_STR( ctl, (char *)un->names[ un_int->selected_name ]->GetMatchString(),
               strlen( un->names[ un_int->selected_name ]->GetMatchString() ) );
    }
  } else {
    un->names[ un_int->selected_name ]->SetMatchString( buffer );
    if( un->names[ un_int->selected_name ]->GetName() == NULL ) {
      un->names[ un_int->selected_name ]->SetName( buffer );
      SET_STR( un_int->name_null, (char *)un->names[ un_int->selected_name ]->GetName(),
               strlen( un->names[ un_int->selected_name ]->GetName() ) );

      un->names[ un_int->selected_name ]->SetEnable( true );
      SET_INT( un_int->name_enable, true );

      REDRAW_CON( un_int->name_listbox );
    }
  }
}

// OnNameUse():
void OnNameUse( LWControl *ctl, UberNullInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  un->SetUseNames( (bool)value );
  un_int->UpdateName( un_int->selected_name );

  REDRAW_CON( un_int->name_listbox );
}

// OnEnableMaster():
void OnEnableMaster( LWControl *ctl, UberNullInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  un->SetUseMasterNull( (bool)value );

  if( un->GetUseMasterNull() ) {
    UNGHOST_CON( un_int->ubernull_master_name     );
    UNGHOST_CON( un_int->ubernull_master_location );
  } else {
    GHOST_CON( un_int->ubernull_master_name     );
    GHOST_CON( un_int->ubernull_master_location );
  }
}

// OnSaveSettings():
void OnSaveSettings( LWControl *ctl, UberNullInterface *panel ) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  // Open the output file
  ofstream out( buffer );
  if( !out ) {
    un_int->message->Info( "UberNull Error:  File error occured creating settings file",
                           buffer );
    return;
  }

  // Output the header
  out << "UberNull "   << UberNull_Settings_Version << endl;
  if( un->GetUseMasterNull() )
    out << "UseMaster" << endl;
  out << "MasterName " << un->GetMasterNullName() << endl;
  out << "MasterLoc "  << un->GetMasterLocation() << endl;

  if( un->GetUseNames() )
    out << "UseNamedNulls" << endl;

  if( !out ) {
    un_int->message->Info( "UberNull Error:  File error occured writing settings file",
                           buffer );
    return;
  }

  for( unsigned int i=0; i < un->names.NumElements(); i++ ) {
    if( !un->names[i]->SaveSettings( out ) ) {
      un_int->message->Info( "UberNull Error:  File error occured writing Named Nulls to settings file",
                             buffer );
      return;
    }
  }
}

// OnLoadSettings():
void OnLoadSettings( LWControl *ctl, UberNullInterface *panel ) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  if( buffer[0] == '\0' )
    return;

  // Open the output file
  ifstream in( buffer );
  if( !in ) {
    un_int->message->Info( "UberNull Error:  File error occured opening the settings file",
                           buffer );
    return;
  }

  // Header Test
  char line[ 8192 ];
  in.getline( line, 8192 );
  if( strnicmp( line, "UberNull", 8 ) != 0 ) {
    un_int->message->Info( "UberNull Error:  UberNull header not found in settings file",
                           buffer );
    return;
  }

  if( atoi( &line[9] ) > UberNull_Settings_Version ) {
    un_int->message->Info( "UberNull Error:  The settings file version is newer than UberNull in",
                           buffer );
    return;
  }    

  // Setup
  un->SetUseMasterNull( false );
  un->SetMasterNullName( NULL );
  un->SetMasterLocation( UBERNULL_FIRST );
  un->SetUseNames( false );
  un->names.Flush();

  un_int->name_top = 0;
  un_int->selected_name = -1;

  // Keyword Test
  do {
    in.getline( line, 8192 );
    if( strnicmp( line, "UseMaster", 9 ) == 0 )
      un->SetUseMasterNull( true );
    else if( strnicmp( line, "MasterName", 10 ) == 0 )
      un->SetMasterNullName( &line[11] );
    else if( strnicmp( line, "MasterLoc", 9 ) == 0 )
      un->SetMasterLocation( atoi( &line[10] ) );
    else if( strnicmp( line, "UseNamedNulls", 13 ) == 0 )
      un->SetUseNames( true );
    else if( strnicmp( line, "NamedNull", 9 ) == 0 ) {
      UberNullNames * new_name = new UberNullNames;
      if( new_name->LoadSettings( in ) )
        un->names.Add( new_name );
      else {
        un_int->message->Info( "UberNull Error:  A file error occured reading Named Null settings from",
                               buffer );
      }
    }

    if( in.eof() )
      break;

    if( in.bad() || in.fail() ) {
      un_int->message->Info( "UberNull Error:  A file error occured reading the settings from",
                             buffer );
      break;
    }
  } while( true );

  // Update the UI
  SET_STR( un_int->ubernull_master_name,     (char *)un->GetMasterNullName(), strlen( un->GetMasterNullName() ) );
  SET_INT( un_int->ubernull_master_location, un->GetMasterLocation() );
  SET_INT( un_int->ubernull_master_enable,   (int)un->GetUseMasterNull() );
  SET_INT( un_int->name_use,                 (bool)un->GetUseNames() );

  OnEnableMaster( un_int->ubernull_master_enable, un_int );

  un_int->UpdateName( -1 );
}
