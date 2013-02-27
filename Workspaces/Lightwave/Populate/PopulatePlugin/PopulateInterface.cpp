//
// PopulateInterface.cpp
//

#include <stdio.h>

#include "PopulateInterface.h"

PopulateInterface *pop_int = NULL;

// Constructor
PopulateInterface::PopulateInterface( GlobalFunc *_global,
                                      void * local,
                                      int &retval )
  : main_panel(NULL), about_panel(NULL),
    Global(*_global), message(NULL), panel_funcs(NULL), scene_info(NULL),
    populate(NULL), content_dir(NULL) {

  // Set the global PopulateInterface pointer so the draw functions and controls can use it
  pop_int = this;

  // Get the Panel Functions and Scene Info from Lightwave
  message     = CMessage::New( (MessageFuncs *) Global( "Info Messages", GFUSE_TRANSIENT) );
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global( PANEL_SERVICES_NAME, GFUSE_TRANSIENT ), _global );
  scene_info  = CLWSceneInfo::New( (LWSceneInfo *) Global( "LW Scene Info", GFUSE_TRANSIENT ) );

  if( !message ) {
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !panel_funcs ) {
    message->Error( "Populate Error: Unable to find lwpanels.p to create the GUI",
                    "Add it and try again." );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !scene_info ) {
    message->Error( "Populate Error: Unable to obtain Scene Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  // Get the Dir Info Function so we can get the Content Dir
  DirInfoFunc *DirInfo = *( (DirInfoFunc *) Global( "Directory Info", GFUSE_TRANSIENT ) );
  if( DirInfo == NULL ) {
    message->Error( "Populate Error: Unable to obtain Directory Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  content_dir = (*DirInfo)( "Content" );

  // Create the Populate instance and initialize it to scene data
  pop = new Populate( scene_info->filename );
  if( !pop ) {
    message->Error( "Populate Error: Unable to create Populate class; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( stricmp( scene_info->filename, "(unnamed)" ) != 0 ) {
    // Create the input scene's filename
    char default_source[ MAX_PATH_LENGTH ];
    strcpy( default_source, content_dir );
    DirStrings::AddPathPart( default_source, scene_info->filename );
    pop->SetSource( default_source );
  }
  pop->GenerateNames( "-Populated" );

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
PopulateInterface::~PopulateInterface() {
  panel_funcs->Destroy( main_panel  );
  panel_funcs->Destroy( about_panel );

  delete panel_funcs;
  delete scene_info;
  delete message;
}

// MakeMainPanel():
bool PopulateInterface::MakeMainPanel() {
  // Set up Main Title String
  sprintf( main_title, "Populate V%g.2 from TM Productions", Populate_Version );

  // Create the Panel
  main_panel = panel_funcs->Create( main_title );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  int x, y, w, h, y2;
try {
  if( !( source_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                  "Source Scene", 45 ) ) )
    throw false;

  if( !( output_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                  "Output Scene", 45 ) ) )
    throw false;


  // Default Settings for String/File and Popup Controls
  SET_STR( source_scene,  (char *)pop->GetSource(),   strlen( pop->GetSource()   ) );
  SET_STR( output_scene,  (char *)pop->GetOutput(),   strlen( pop->GetOutput()   ) );

  // Set the Panel Size
  int ph = PAN_GETH( panel_funcs->orig_struct, main_panel );
  PAN_SETH( panel_funcs->orig_struct, main_panel, ph + 4 );


  // Set the Handler Functions
  CON_SETEVENT( source_scene,         OnSourceFile,         this );
  CON_SETEVENT( output_scene,         OnOutputFile,         this );
//  CON_SETEVENT( populate,             OnPopulate,           this );
//  CON_SETEVENT( about,                OnAbout,              this );

  // Set the Draw Functions
//  ival.intv.value = (int)&DrawAbout;

  panel_funcs->Set( main_panel, PAN_USERDRAW, DrawMainPanel );

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
bool PopulateInterface::OpenMainPanel() {
  if( !panel_funcs->Open( main_panel, PANF_BLOCKING | PANF_MOUSETRAP ) )
    return false;

  return true;
}

// DrawMainPanel() {
void DrawMainPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs *df = pop_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( pop_int->panel_funcs->orig_struct, pop_int->main_panel );
  int pw = PAN_GETW( pop_int->panel_funcs->orig_struct, pop_int->main_panel );

}

// MakeAboutPanel():
bool PopulateInterface::MakeAboutPanel() {
  // Create the Panel
  about_panel = panel_funcs->Create( main_title );

  if( !about_panel ) {                    // Make sure the panel was created
    message->Error("Error creating about panel, aborting" );
    return false;
  }

try {
  // About Box Area
  if( !( about_area = CANVAS_CTL( panel_funcs->orig_struct, about_panel,                     // Compare Mode pop-up
                                  "", 320, 20 ) ) )
    throw false;

  PAN_SETW( panel_funcs->orig_struct, about_panel, 300 );
  PAN_SETH( panel_funcs->orig_struct, about_panel, 200 );

  MOVE_CON( about_area, (CON_X( about_area )) + 10,
                        (CON_Y( about_area )) );

  // Set the Draw Functions
  ival.intv.value = (int)&DrawAbout;
  (*about_area->set)( about_area, CTL_USERDRAW, &ival );

  return true;

} catch( bool a ) {
  if( !a ) {
    message->Error("Error creating interface controls; aborting" );
    return false;
  }
}

return true;
}

// OpenAboutPanel()
//  Opens the about panel in blocking mode
bool PopulateInterface::OpenAboutPanel() {
  if( !panel_funcs->Open( about_panel, PANF_BLOCKING ) )
    return false;

  return true;
}

// OnAbout():
//  Called to open the About Box.
void OnAbout(LWControl *ctl, PopulateInterface *panel) {
  panel->OpenAboutPanel();
}

// DrawAbout():
//  Called to draw the About box
void DrawAbout(LWControl *ctl, PopulateInterface *ui, DrMode mode) {
  CDrawFuncs *df = pop_int->panel_funcs->DrawFuncs;

  int x = CON_X( ctl );
  int y = CON_Y( ctl );
  int w = CON_W( ctl );
  int h = CON_H( ctl );

  int ph = PAN_GETH( pop_int->panel_funcs->orig_struct, pop_int->about_panel );
  int pw = PAN_GETW( pop_int->panel_funcs->orig_struct, pop_int->about_panel );

  char buffer[256];
  sprintf( buffer, "%s V%g.2", about_text[0], Populate_Version );

  df->WriteText( pop_int->about_panel, buffer, COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(pop_int->about_panel, buffer))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( pop_int->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(pop_int->about_panel, (about_text[i]) ))/2,
             y );
    y += 15;
  }
}

// OnSourceFile():
//  Called to handle a change in the Source File's name. All source
//   filename checks are done at Process time.  When the source name
//   changes from (unnamed) to anything else, the rest of the fields
//   are auto-generated.
void OnSourceFile(LWControl *ctl, PopulateInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  if( stricmp( buffer, "(unnamed)" ) == 0 ) {
    // Return to defaults
    pop_int->pop->SetSource(   NULL );
    pop_int->pop->SetOutput(   NULL );

    pop_int->pop->GenerateNames( "-Populated" );

    SET_STR( pop_int->output_scene,  (char *)pop_int->pop->GetOutput(),   strlen( pop_int->pop->GetOutput()   )  );
  } else {
    if( pop_int->pop->GetSource()[0] == '\0' ) {
      pop_int->pop->SetOutput(   NULL );
    }

    char new_source[ MAX_PATH_LENGTH ];
    strcpy( new_source, pop_int->content_dir );
    DirStrings::AddPathPart( new_source, buffer );
    SET_STR( pop_int->source_scene, new_source, strlen( new_source ) );

    // We just changed the source name, so re-generate the names as needed
    pop_int->pop->SetSource( new_source );

    pop_int->pop->GenerateNames( "-Populated" );

    SET_STR( pop_int->output_scene,  (char *)pop_int->pop->GetOutput(),   strlen( pop_int->pop->GetOutput()   )  );
  }
}

// OnOutputFile():
//  Called to handle a change in the Output File's name.  Currently,
//   this does nothing, and all output filename checks are done
//   at Process time.
void OnOutputFile(LWControl *ctl, PopulateInterface *panel) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  char new_output[ MAX_PATH_LENGTH ];
  strcpy( new_output, pop_int->content_dir );
  DirStrings::AddPathPart( new_output, buffer );
  SET_STR( pop_int->output_scene, new_output, strlen( new_output ) );
}
