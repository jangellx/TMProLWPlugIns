//
// FreeKey-Interface.cpp
//

#include <stdio.h>

#include "FreeKey-Interface.h"
#include "FreeKeyLogo.h"
#include "FreeKeyLogo_Small.h"

#include <freq.h>

#define DEBUG_RAM_USAGE

// Misc. UI Strings
char *about_text[] = {
  "FreeKey",
  "for Lightwave 3D",
  "",
  "Copyright 1999-2000 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// Globals
FreeKeyInterface *fk_int = NULL;
FreeKey          *fk     = NULL;

FreeKeyLogo        logo;
FreeKeyLogo_Small  logo_small;
ImageClass        *decoded_logo_small;

// Constructor
FreeKeyInterface::FreeKeyInterface( GlobalFunc    *_global,
                                    LayoutGeneric *_local,
                                    int           &retval )
  : main_panel(NULL), about_panel(NULL), keys_panel(NULL), local(_local),
    Global(*_global), message(NULL), panel_funcs(NULL), scene_info(NULL), content_dir(NULL),
    shift_down(false), ctrl_down(false), alt_down(false), last_key_down(-1),
    processing(false), stop_processing(false), fkd(NULL), mouse_state(MSTATE_UP),
    mouse_delta_x(0), mouse_delta_y(0), mouse_count(0), mouse_down_x(0), mouse_down_y(0),
    draw_bbox(false), now_scrolling(false), status_mode(STATUSMODE_INFO),
    quickdraw_item_list(false) {

  // Set the global FreeKeyInterface pointer so the draw functions
  //  and controls can use it
  fk_int = this;

  // Get the Panel Functions and Scene Info from Lightwave
  message     = CMessage::New( (MessageFuncs *) Global( "Info Messages", GFUSE_TRANSIENT) );
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global( PANEL_SERVICES_NAME, GFUSE_TRANSIENT ), _global );
  scene_info  = CLWSceneInfo::New( (LWSceneInfo *) Global( "LW Scene Info", GFUSE_TRANSIENT ) );

  if( !message ) {
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !panel_funcs ) {
    message->Error( "FreeKey Error: Unable to find lwpanels.p to create the GUI",
                    "Add it and try again." );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !scene_info ) {
    message->Error( "FreeKey Error: Unable to obtain Scene Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  // Get the Dir Info Function so we can get the Content Dir
  DirInfoFunc *DirInfo = *( (DirInfoFunc *) Global( "Directory Info", GFUSE_TRANSIENT ) );
  if( DirInfo == NULL ) {
    message->Error( "FreeKey Error: Unable to obtain Directory Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }
  content_dir = (*DirInfo)( "Content" );

  // See if a FreeKey instance already exists
  if( fk == NULL ) {
    // Create the FreeKey instance and initialize it to scene data
    fk = new FreeKey( scene_info->filename );
    if( !fk ) {
      message->Error( "FreeKey Error: Unable to create FreeKey class; aborting" );
      retval = AFUNC_BADGLOBAL;
      return;
    }

    if( stricmp( scene_info->filename, "(unnamed)" ) != 0 ) {
      // Create the input scene's filename
      char default_source[ MAX_PATH_LENGTH ];
      strcpy( default_source, content_dir );
      DirStrings::AddPathPart( default_source, scene_info->filename );
      fk->SetSource( default_source );
    }
    fk->GenerateNames( "-FreeKeyed" );

    // Create Default Group
    fk->AddGroup();
  }

  // Create the Register Panel
  fkd = new FreeKeyDongle;
  if( !fkd->TestKey( fkd->ReadKey(), fk_int->fkd->GetDongleID() ) )
    fkd->OpenRegisterPanel();

  SetStatusText( "Ready!" );

  // Decode the title image
  decoded_logo_small = logo_small.Decode();

  // Create the panels
  if( MakeMainPanel() ) {                  // Create the main panel
    if( MakeAboutPanel() ) {               // Create the about panel
      if( MakeKeysPanel() ) {              // Create the keyboard help panel

        // Load the scene from Layout if none is loaded yet
        if( (stricmp( fk->GetSource(), "(unnamed)" ) != 0) &&                     // Make sure the scene is named
            (fk->GetScene().GetBaseItem().GetChildren().NumElements() == 0) )     //  and that nothing has been loaded yet
          fk_int->RefreshSource( fk->GetSource(), true );

        // Open the Main Panel
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
  } else {
    retval = AFUNC_BADLOCAL;
  }
}

// Destructor
FreeKeyInterface::~FreeKeyInterface() {
  if( main_panel  != NULL )           panel_funcs->Destroy( main_panel  );
  if( about_panel != NULL )           panel_funcs->Destroy( about_panel );
  if( keys_panel  != NULL )           panel_funcs->Destroy( keys_panel  );

  if( decoded_logo_small != NULL )    delete decoded_logo_small;

  delete fkd;

  delete panel_funcs;
  delete scene_info;
  delete message;
}

// MakeMainPanel():
bool FreeKeyInterface::MakeMainPanel() {
  // Set up Main Title String
  sprintf( main_title, "FreeKey V%d.%d from TM Productions",
           FreeKey_Version, FreeKey_Revision );

  // Create the Panel
  main_panel = panel_funcs->Create( main_title );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  // Create the controls
  try {
    if( !MakeMainControls() )
      throw false;

    if( !MakeItemSelectionControls() )
      throw false;

    if( !MakeKeySelectionControls() )
      throw false;

    if( !MakeGroupControls() )
      throw false;

    if( !MakeOptionControls() )
      throw false;

    if( !MakeToolControls() )
      throw false;

    // Set the default tabs
    fk_int->ShowToolOptionsTab( fk->GetToolOptionsTab() );

    // Set the Panel Size
    int y = CON_Y( freekey );
    int h = CON_H( freekey );
    int ph = PAN_GETH( panel_funcs->orig_struct, main_panel );
    PAN_SETH( panel_funcs->orig_struct, main_panel, y + h + 30 );

    // Set Panel-Level Functions
    panel_funcs->Set( main_panel, PAN_USERDRAW,     DrawMainPanel );
    panel_funcs->Set( main_panel, PAN_USERKEYS,     OnKeyDown     );
    panel_funcs->Set( main_panel, PAN_USERKEYUPS,   OnKeyUp       );
    panel_funcs->Set( main_panel, PAN_MOUSEBUTTON,  OnMouse       );
    panel_funcs->Set( main_panel, PAN_MOUSEMOVE,    OnMouse       );

    // Do some post-creation setup
    if( fk != NULL ) {
      // Set some default values
      SET_STR( source_scene, (char *)fk->GetSource(), strlen( fk->GetSource() ) );
      SET_STR( output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput() ) );
 
      SET_FLOAT( item_first_frame,   fk->GetScene().GetPreviewFirstFrame() );
      SET_FLOAT( item_last_frame,    fk->GetScene().GetPreviewLastFrame()  );

      // Initialize the item list
      OnDisplayMode( fk_int->display_item_mode, fk_int );
 
      // Set the selection
      if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 ) {
        fk->GetScene().GetBaseItem().BuildSelectedItemList( fk->GetSelectedItems(), true, true, false );
        SET_INT( select_count, fk->GetSelectedItems().NumElements() );
      }

      // Update the group controls
      FillWithFreeKeyGroup( fk->GetCurrentGroup() );
    }

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
bool FreeKeyInterface::OpenMainPanel() {
  if( !panel_funcs->Open( main_panel, PANF_BLOCKING | PANF_MOUSETRAP | PANF_PASSALLKEYS ) )
    return false;

  return true;
}

// DrawMainPanel() {
void DrawMainPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( fk_int->panel_funcs->orig_struct, fk_int->main_panel );
  int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->main_panel );

  // Horizontal Dividers
  int y = CON_Y( fk_int->source_scene );
  PanelDraw::HorizontalDivider( fk_int->main_panel, df, 0, y - 2, pw, 1 );

      y = CON_Y( fk_int->output_scene );
  int h = CON_H( fk_int->output_scene );
  PanelDraw::HorizontalDivider( fk_int->main_panel, df, 0, y+h+3, pw, 1 );

  y = CON_Y( fk_int->scale_amount );
  h = CON_H( fk_int->scale_amount );
  PanelDraw::HorizontalDivider( fk_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( fk_int->item_multiscroll );
  h = CON_H( fk_int->item_multiscroll );
  PanelDraw::HorizontalDivider( fk_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( fk_int->range_low );
  PanelDraw::HorizontalDivider( fk_int->main_panel, df, 10, y-5, pw-30, 1 );

  // Tab Underline
  y = CON_HOTY( fk_int->tool_options_tabs );
  h = CON_HOTH( fk_int->tool_options_tabs );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, 0, y+h, pw, 1 );

  DrawToolsPane();

  // Title Image
  if( !fk->GetUseOpenGLLogo() )
    logo_small.DrawInLWPanels( fk_int->main_panel, df, 0, 1 );
}

// DrawTitleImage()
void DrawTitleImage( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  int h = CON_HOTH( ctl );

  glOrtho( 0, decoded_logo_small->GetWidth(),
           decoded_logo_small->GetHeight(), 0,
           -1.0, 1.0 );

  glRasterPos2i( 0, 0 );
  glPixelZoom(  1, -1 );
  glDrawPixels( decoded_logo_small->GetWidth(), decoded_logo_small->GetHeight(),
                GL_RGB, GL_UNSIGNED_BYTE, decoded_logo_small->GetBuffer() );
  glFlush();
}

// MakeAboutPanel():
bool FreeKeyInterface::MakeAboutPanel() {
  // Create the Panel
  about_panel = panel_funcs->Create( main_title );

  if( !about_panel ) {                    // Make sure the panel was created
    message->Error("Error creating about panel, aborting" );
    return false;
  }

  PAN_SETW( panel_funcs->orig_struct, about_panel, 380 );
  PAN_SETH( panel_funcs->orig_struct, about_panel, 270 );

  // Set the Draw Functions
  panel_funcs->Set( about_panel, PAN_USERDRAW, DrawAboutPanel );

  return true;
}

// OpenAboutPanel()
//  Opens the about panel in blocking mode
bool FreeKeyInterface::OpenAboutPanel() {
  if( !panel_funcs->Open( about_panel, PANF_BLOCKING ) )
    return false;

  return true;
}

// OnAbout():
//  Called to open the About Box.
void OnAbout(LWControl *ctl, FreeKeyInterface *panel) {
  panel->OpenAboutPanel();
}

// DrawAbout():
//  Called to draw the About box
void DrawAboutPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  // Title Image
  logo.DrawInLWPanels( fk_int->about_panel, df, -280, 0 );

  int ph = PAN_GETH( fk_int->panel_funcs->orig_struct, fk_int->about_panel );
  int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->about_panel );

  int y = 70;

  char buffer[256];
  sprintf( buffer, "%s V%d.%d", about_text[0], FreeKey_Version, FreeKey_Revision );

  df->WriteText( fk_int->about_panel, buffer, COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(fk_int->about_panel, buffer))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( fk_int->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(fk_int->about_panel, (about_text[i]) ))/2,
             y );
    y += 15;
  }
}

// OnSourceFile():
//  Called to handle a change in the Source File's name. All source
//   filename checks are done at Process time.  When the source name
//   changes from (unnamed) to anything else, the rest of the fields
//   are auto-generated.
void OnSourceFile(LWControl *ctl, FreeKeyInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( fk_int->source_scene, buffer, MAX_PATH_LENGTH );

  fk_int->RefreshSource( buffer, false );

  // Update the output filename
  fk->SetOutput( NULL );
  fk->GenerateNames( "-FreeKeyed" );
  SET_STR( fk_int->output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput() ) );
}

// OnSourcePopup():
//  Reloads the source scene
void OnSourcePopup(LWControl *ctl, FreeKeyInterface *panel) {
  int value;
  GET_INT( fk_int->source_popup, value );

  char buffer[ MAX_PATH_LENGTH ];

  // Save Layout Scene
  if( value == 2 ) {
    char freq_path[ MAX_PATH_LENGTH ];
    char freq_base_name[ MAX_PATH_LENGTH ];
    char freq_full_name[ MAX_PATH_LENGTH ];

    FileReqFunc * filereq = (FileReqFunc *) fk_int->Global( "File Request", GFUSE_TRANSIENT );
    if( filereq == NULL ) {
      fk_int->message->Error( "FreeKey Error:  Unable to obtain File Request from Lightwave; aborting" );
      return;
    }

    strcpy( freq_full_name, fk_int->scene_info->filename );
    DirStrings::GetPathPart( freq_path, freq_full_name );
    DirStrings::GetFilePart( freq_base_name, freq_full_name );

    int error = (*filereq)( "Save Layout's Scene As...", freq_base_name, freq_path,
                            freq_full_name, MAX_PATH_LENGTH );
    if( error < 0 ) {
      sprintf( buffer, "FreeKey Error:  File Requester returned error code %d", error );
      fk_int->message->Error( buffer);
    } else if ( error == 1  ) {
      fk_int->local->saveScene( freq_full_name );
      fk_int->message->Info( "FreeKey:  Layout's scene has been saved to", freq_full_name );
    }

    // Update our scene
    fk_int->RefreshSource( freq_full_name, false );
  }

  // Handle all other options
  switch( value ) {
  case 0:                              // Refresh
    GET_STR( fk_int->source_scene, buffer, MAX_PATH_LENGTH );
    fk_int->RefreshSource( buffer, true );
    break;

  case 1:                              // Get Layout's Scene Path
    fk_int->RefreshSource( fk_int->scene_info->filename, true );
    break;

  case 2:                              // Save Scene In Layout
    // Handled above, where we can allocate new variables.
    break;

  case 3:                              // Load Scene Into Layout
    GET_STR( fk_int->source_scene, buffer, MAX_PATH_LENGTH );
    fk_int->local->loadScene( buffer, buffer );
    fk_int->message->Info( "FreeKey:  The following scene has been loaded into Layout:", buffer );
    break;
  }

  SET_INT( fk_int->source_popup, 0 );
  REDRAW_CON( fk_int->source_popup );
}

// OnOutputPopup():
//  Auto-generates a name for the output scene
void OnOutputPopup(LWControl *ctl, FreeKeyInterface *panel) {
  int value;
  GET_INT( fk_int->output_popup, value );

  char buffer[ MAX_PATH_LENGTH ];

  switch( value ) {
  case 0:                              // Auto-Generate Name
    fk->SetOutput( NULL );
    fk->GenerateNames( "-FreeKeyed" );
    SET_STR( fk_int->output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput() ) );
    break;

  case 1:                              // Load Scene Into Layout
    GET_STR( fk_int->output_scene, buffer, MAX_PATH_LENGTH );
    fk_int->local->loadScene( buffer, buffer );
    fk_int->message->Info( "FreeKey:  The following scene has been loaded into Layout:", buffer );
    break;
  }

  SET_INT( fk_int->output_popup, 0 );
  REDRAW_CON( fk_int->output_popup );
}

// RefreshSource():
//  Reads the lastest data from the source scene file specified.
//   If reload is true, the source scene will be reloaded even
//   if the names are identical.
void FreeKeyInterface::RefreshSource( const char * buffer, bool reload ) {
  if( buffer == NULL )
    return;

  if( stricmp( buffer, "(unnamed)" ) == 0 ) {
    // Return to defaults
    fk->SetSource( buffer );
    fk->SetOutput( NULL );

    fk->GenerateNames( "-FreeKeyed" );

    SET_STR( output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput()   )  );
  } else {
    if( fk->GetSource()[0] == '\0' )
      fk->SetOutput( NULL );

    char new_source[ MAX_PATH_LENGTH ];
    strcpy( new_source, content_dir );
    DirStrings::AddPathPart( new_source, buffer );
    SET_STR( source_scene, new_source, strlen( new_source ) );

    // See if the two strings are equal
    if( (stricmp( new_source, fk->GetSource() ) == 0) && !reload )
      return;

    fk_int->SetStatusText( "Loading scene; please wait...", STATUSMODE_NOTICE, true );

    // We just changed the source name, so re-generate the names if needed
    fk->SetSource( new_source );
    fk->GenerateNames( "-FreeKeyed" );

    SET_STR( output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput()   )  );

    // Scan the source scene file for the hierarchy
    fk->Init( fk->GetSource(), fk->GetOutput() );
    fk->GetScene().SetSceneName( fk->GetSource() );
    int error = fk->BuildHierarchy();
    if( error != LWSERROR_OK ) {
      if( error == LWSERROR_UNSUPPORTED_VERSION ) {
        fk_int->message->Error( "FreeKey Error:  Unsupported scene version; only version 1 and 2",
                                "(LW 5.7 and earlier) scenes are supported at this time; aborting" );
      } else
        fk_int->message->Error( "FreeKey Error:", LWSceneErrorStrings[error] );

      fk_int->SetStatusText( "Error occured loading scene file data!", STATUSMODE_ERROR, true );
    } else {
      fk_int->SetStatusText( "Scene loaded successfully!", STATUSMODE_INFO, true );
    }

    fk->AddGroup();
  }

  // Re-init some things
  sorted_items.Reset();
  fk->GetSelectedItems().Reset();
  SET_INT( item_scroll, 0 );
  SET_INT( select_count, 0 );

  // Refresh the groups
  fk->SetCurrentGroup(0);
  SET_INT( group_list, fk->GetCurrentGroup() );
  FillWithFreeKeyGroup( fk->GetCurrentGroup() );

  // Update some scene-specific controls
  fk->SetDisplayMin( fk->GetScene().GetPreviewStartTime() );
  fk->SetDisplayMax( fk->GetScene().GetPreviewEndTime() );

  SET_FLOAT( item_first_frame, fk->GetDisplayMin() * default_fps );
  SET_FLOAT( item_last_frame,  fk->GetDisplayMax() * default_fps );

  // Reset the selected list and re-sort
  DeselectAll();
  OnDisplayMode( display_item_mode, fk_int );

#ifdef DEBUG_RAM_USAGE
  fk_int->SetStatusText( "Outputting RAM Usage data i:\\FKTest\\RamUsage.txt...", STATUSMODE_NOTICE, true );
  fk->GetScene().RAMUsageToFile( "i:\\FKTest\\RamUsage.txt" );
  fk_int->SetStatusText( "RAM Usage data output sucessfully!", STATUSMODE_INFO, true );
#endif
}

// OnOutputFile():
//  Called to handle a change in the Output File's name.  Currently,
//   this does nothing, and all output filename checks are done
//   at Process time.
void OnOutputFile(LWControl *ctl, FreeKeyInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  fk->SetOutput( buffer );
  fk->GenerateNames( "-FreeKeyed" );

  strcpy( buffer, fk_int->content_dir );
  DirStrings::AddPathPart( buffer, fk->GetOutput() );
  fk->SetOutput( buffer );

  SET_STR( fk_int->output_scene, (char *)fk->GetOutput(), strlen( fk->GetOutput() ) );
}

// OnMouse()
//  This is called whenever the state of the mouse buttons changes.
void OnMouse( LWPanelID panel, void *data, int click, int mx, int my) {
  // Figure out the mouse state
  bool do_event = true;

//  ofstream out( "i:\\TestMouse.txt", ios::app );

//  click = click & IQ_CTRL & IQ_SHIFT & IQ_ALT;

  if( click & MOUSE_DOWN ) {
    // Mouse Pressed
    fk_int->mouse_delta_x =  0;
    fk_int->mouse_delta_y =  0;
    fk_int->mouse_down_x  = mx;
    fk_int->mouse_down_y  = my;
    fk_int->mouse_count   =  0;
    fk_int->mouse_state = MSTATE_DOWN;
  } else if( (click & MOUSE_LEFT) || (click & MOUSE_RIGHT) || (click & MOUSE_MID) ) {
    // Mouse Released
    fk_int->mouse_state = MSTATE_UP;
    do_event = false;
  } else {
    // Mouse Moving
    fk_int->mouse_state = MSTATE_MOVING;
    int new_delta_x = fk_int->mouse_delta_x;
    int new_delta_y = fk_int->mouse_delta_y;

    fk_int->mouse_delta_x = mx - fk_int->mouse_down_x;
    fk_int->mouse_delta_y = my - fk_int->mouse_down_y;

    if( (new_delta_x == fk_int->mouse_delta_x) &&
        (new_delta_y == fk_int->mouse_delta_y) )
      do_event = false;
    else
      fk_int->mouse_count++;

//    out << fk_int->mouse_count << "  Click: " << click << "    MX/MY: " << mx << "/" << my << "    dX/dY: " << fk_int->mouse_delta_x << "/" << fk_int->mouse_delta_y << "    Do: " << (int)do_event << endl;
  }

//  SET_INT( fk_int->range_low, click );

  if( RCLICK( click ) )        fk_int->mouse_button = MBUTTON_RIGHT;
  else if( MCLICK( click ) )   fk_int->mouse_button = MBUTTON_MIDDLE;
  else if( LCLICK( click ) )   fk_int->mouse_button = MBUTTON_LEFT;

  // Do events that also want Mouse Up calls
  int cx = CON_HOTX( fk_int->item_list );
  int cy = CON_HOTY( fk_int->item_list );
  int cw = CON_HOTW( fk_int->item_list );
  int ch = CON_HOTH( fk_int->item_list );

/*
  char buffer[ 128 ];
  sprintf( buffer, "Count: %d    Click: %d    MX/MY: %d/%d    dX/dY: %d/%d", fk_int->mouse_count, click, mx, my, fk_int->mouse_delta_x, fk_int->mouse_delta_y );
  SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );
*/

  // Check for clicks in the Item List
  if( ( mx > cx ) && ( mx < cx + cw ) ) {
    if( ( my > cy ) && ( my < cy + ch ) ) {
      OnItemList( fk_int->item_list, fk_int );
    }
  }

  // Check for clicks within the logo
  cx = CON_HOTX( fk_int->title_image );
  cy = CON_HOTY( fk_int->title_image );
  cw = CON_HOTW( fk_int->title_image );
  ch = CON_HOTH( fk_int->title_image );

  if( ( mx > cx ) && ( mx < cx + cw ) ) {
    if( ( my > cy ) && ( my < cy + ch ) ) {
      if( fk_int->mouse_state == MSTATE_DOWN ) {
        fk->SetUseOpenGLLogo( !fk->GetUseOpenGLLogo() );
        if( fk->GetUseOpenGLLogo() )
          UNGHOST_CON( fk_int->title_image );
        else {
          ERASE_CON( fk_int->title_image );
          DrawMainPanel( fk_int->main_panel, NULL, DR_REFRESH );
        }
      }
    }
  }

  // Do events that don't want Mouse Up calls
  if( do_event ) {

  }
}
