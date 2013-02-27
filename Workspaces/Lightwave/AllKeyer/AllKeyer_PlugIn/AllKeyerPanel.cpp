//
// AllKeyerPanel.cpp
//

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK
#include "AllKeyerPanel.h"
#include "AllKeyer.h"

AllKeyerPanel   *akp;
extern AllKeyer *ak;

// ----
// --            AllKeyerPanel Member Functions
// ----

// Constructor
AllKeyerPanel::AllKeyerPanel( GlobalFunc *_global, 
                              CMessage   *_message, int &retval )
  : Global( *_global ), message(_message), proc_panel(NULL),
    main_panel(NULL), about_panel(NULL),
    main_panel_state(MPS_EXIT) {

  akp = this;

  // Get the LW Panels Pointer
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global(PANEL_SERVICES_NAME, GFUSE_TRANSIENT ),
                                    &Global );
  if( !panel_funcs ) {
    message->Error( "ERROR: Unable to find lwpanels.p to create the GUI",
                    "Add it and try again." );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  // Create the Panels
  MakeMainPanel();
  MakeProcPanel();
  MakeAboutPanel();

  if( !main_panel || !proc_panel || !about_panel ) {
    retval = AFUNC_OK;
    return;
  }

  retval = AFUNC_OK;
}

// Destructor
AllKeyerPanel::~AllKeyerPanel() {
  if( main_panel )
    panel_funcs->Destroy( main_panel  );
  if( proc_panel )
    panel_funcs->Destroy( proc_panel  );
  if( about_panel )
    panel_funcs->Destroy( about_panel );

  delete panel_funcs;
}

// MakeMainPanel( void ):
//  Craete the Main Panel
bool AllKeyerPanel::MakeMainPanel(void) {
  main_panel = panel_funcs->Create("All-Keyer from TM Productions" );

  if( !main_panel ) {   // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }


  // Options Section
try {
  if( !( compare_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,                     // Compare Mode pop-up
                                    "Compare Mode", compare_mode_names, 150 ) ) )
    throw false;

	if(!(use_frame_0 = BOOL_CTL(panel_funcs->orig_struct, main_panel,"Use Frame 0?")))          // Use Frame 0?
    throw false;

	if(!(last_key = BOOL_CTL(panel_funcs->orig_struct, main_panel,"Last Key is Last Frame?")))  // Last Key is Last Frame?
    throw false;

	if(!(pause_on_errors = BOOL_CTL(panel_funcs->orig_struct, main_panel,"Pause On Errors?")))  // Pause On Errors?
    throw false;

  LWControl *opt_array[5] = {
    use_frame_0,
    last_key,
    pause_on_errors,
    NULL
  };
  
  MOVE_CON( compare_mode, ( CON_X( compare_mode ) + 8 ),
                          ( CON_Y( compare_mode ) + 6 ) );
  PanelTools::CenterUnder( compare_mode, use_frame_0 );
  PanelTools::AlignWith( opt_array );
  PanelTools::PutUnder(  opt_array );

  SET_INT( last_key, true );
  SET_INT( pause_on_errors, true );

	if(!( option_border = BORDER_CTL(panel_funcs->orig_struct, main_panel,
                          "Options", (CON_W(compare_mode)) + 7,
                          (CON_Y(pause_on_errors)) + (CON_H(pause_on_errors)) + 30 )))       // Options Border
    throw false;

  MOVE_CON( option_border, (CON_X(compare_mode))+1,
                           (CON_Y(compare_mode)) ); 


  // Files Section
  if( !( compare_file = FILE_CTL( panel_funcs->orig_struct, main_panel,                       // Key Mode pop-up
                                  "Compare File", 50 ) ) )
    throw false;

  if( !( source_pattern = FILE_CTL( panel_funcs->orig_struct, main_panel,                     // Key Mode pop-up
                                    "Source Pattern", 50 ) ) )
    throw false;

  if( !( destination_path = FILE_CTL( panel_funcs->orig_struct, main_panel,                   // Key Mode pop-up
                                      "Destination Path", 50 ) ) )
    throw false;

  SET_STR( compare_file,     "(none)", 6 );  // Set the default file names
  SET_STR( source_pattern,    (char *)ak->SceneInfo()->name, 6 );
  SET_STR( destination_path, "(none)", 6 );

  // Key Mode
  if( !( key_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,                         // Key Mode pop-up
                                "Key Mode", key_mode_names, 150 ) ) )
    throw false;

  PanelTools::PutOnLineWith( compare_mode, compare_file, 10 );
  LWControl *file_array[5] = {
    compare_file,
    source_pattern,
    destination_path,
    key_mode,
    NULL
  };
  PanelTools::AlignLabels( file_array, 5 );

  if( !( key_frame_step = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,                         // Key Mode pop-up
                                          "Frame Step", 100, 1, 1000) ) )
    throw false;
  PanelTools::PutOnLineWith( key_mode, key_frame_step, 12 );

  if( !( key_threshold = PERCENT_CTL( panel_funcs->orig_struct, main_panel,                         // Key Mode pop-up
                                      "Threshold" ) ) )
    throw false;

  PanelTools::AlignLabels( key_frame_step, key_threshold );
  int x = CON_X( key_threshold );
  int y = CON_Y( key_frame_step );
  MOVE_CON( key_threshold, x, y );

  ERASE_CON(  key_threshold  );           // Show only the Frame Step entry
  RENDER_CON( key_frame_step );
  SET_INT(    key_frame_step, 1 );        // Set the default Frame Step value
  SET_FLOAT(  key_threshold, 1.0f );      // Set the default Threshold value

  // Log Controls
  if( !( log_file = FILE_CTL( panel_funcs->orig_struct, main_panel,                         // Key Mode pop-up
                              "Log File", 47 ) ) )
    throw false;

  SET_STR( log_file, "(none)", 6 );  // Set the default log file name

  if( !( log_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,            // Log Detail Mode pop-up
                                "Log Mode", log_mode_names, 150 ) ) )
    throw false;
  
	if(!( log_append = BOOL_CTL(panel_funcs->orig_struct, main_panel,"Append?")))  // Append? mode
    throw false;

  MOVE_CON( log_file, (CON_X(log_file)) + 8, CON_Y(log_file) );
  PanelTools::PutUnder( pause_on_errors, log_file, 20 );
  LWControl *log_array[5] = {
    log_file,
    log_mode,
    log_append,
    NULL
  };
  PanelTools::PutOnLineWith( log_array, 10 );

  x = CON_X(log_append);
  int w = CON_W(log_append);
	if(!( log_border = BORDER_CTL(panel_funcs->orig_struct, main_panel,
                          "Log", x + w,
                          (CON_H(log_file)) + 2 )))                        // Options Border
    throw false;

  MOVE_CON( log_border, (CON_X(log_file))+1,
                        (CON_Y(log_file)) ); 

  // Other Buttons and the Status Bar
  if( !( process_scenes = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,    // Process Scenes button
                                      "Process Scenes", 150 ) ) )
    throw false;

  w = PAN_GETW( panel_funcs->orig_struct, main_panel );
  int cw = CON_W( process_scenes );
  y = CON_Y( log_border );
  int h = CON_H( log_border );
  MOVE_CON( process_scenes, (w-cw)/2, y+h+8 );

  if( !( status = STRRO_CTL( panel_funcs->orig_struct, main_panel,              // Process Scenes button
                            "Status", 90 ) ) )
    throw false;

  SET_STR( status, "Lightwave 3D All-Keyer, Copyright 1999 TM Productions", 6 );

  y = CON_Y( process_scenes );
  h = CON_H( process_scenes );
  MOVE_CON( status, CON_X( status ), y+h );

  if( !( about = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,    // Process Scenes button
                              "About", 80 ) ) )
    throw false;
  PanelTools::PutOnLineWith( status, about );

  y = CON_Y( status );
  h = CON_H( status );
  PAN_SETH( panel_funcs->orig_struct, main_panel, y + h + 30 );

  // Set up all the Event Handlers
  CON_SETEVENT( key_mode,         OnKeyMode, this );
  CON_SETEVENT( about,            OnAbout,   this );
  CON_SETEVENT( process_scenes,   OnProcess, this );
  CON_SETEVENT( source_pattern,   OnSource,  this );
  CON_SETEVENT( destination_path, OnDest,    this );
  CON_SETEVENT( compare_file,     OnCompare, this );
  CON_SETEVENT( log_file,         OnLog,     this );

  return true;

 } // Error Control
catch( bool a ) {
  if( a == false ) {
    message->Error("Error creating interface controls; aborting" );
    return false;
  }
 }
}

// MakeProcPanel():
//  Creates and fills in the Processing Panel
bool AllKeyerPanel::MakeProcPanel(void) {
  return true;
}

// MakeAboutPanel():
//  Creates and fills in the About Panel
bool AllKeyerPanel::MakeAboutPanel(void) {
  about_panel = panel_funcs->Create( "All-Keyer, by TM Productions" );

  if( !about_panel ) {   // Make sure the panel was created
    message->Error("Error creating about panel, aborting" );
    return false;
  }

  // About Box Area
  if( !( about_area = CANVAS_CTL( panel_funcs->orig_struct, about_panel,                     // Compare Mode pop-up
                                  "", 320, 20 ) ) ) {
    message->Error("Error creating interface controls, aborting" );
    return false;
  }

  PAN_SETW( panel_funcs->orig_struct, about_panel, 300 );
  PAN_SETH( panel_funcs->orig_struct, about_panel, 200 );

  ival.intv.value = (int)&DrawAbout;
  (*about_area->set)( about_area, CTL_USERDRAW, &ival );

  MOVE_CON( about_area, (CON_X( about_area )) + 10,
                        (CON_Y( about_area )) );

  return true;
}

// OpenMain():
//  Open the Main Panel and close the Processing Panel
int AllKeyerPanel::OpenMain(void) {
  main_panel_state = MPS_EXIT;   // Default state.  Only changes to MPS_PROCESS if the Process Scenes button was clicked

  if( proc_panel )               // Close the process panel if it exists
    panel_funcs->Close( proc_panel );

  if( main_panel )               // Open the main panel if it exists
    panel_funcs->Open( main_panel, PANF_BLOCKING );

  return main_panel_state;
}

// OpenAbout():
//  Open the About Panel
bool AllKeyerPanel::OpenAbout(void) {
  if( about_panel )
    panel_funcs->Open( about_panel, PANF_BLOCKING );

  return true;
}

// DrawAbout():
//  Called to draw the About box
void DrawAbout(LWControl *ctl, AllKeyerPanel * akp2, DrMode mode) {
  CDrawFuncs *df = akp->panel_funcs->DrawFuncs;

  int x = CON_X( ctl );
  int y = CON_Y( ctl );
  int w = CON_W( ctl );
  int h = CON_H( ctl );

  int ph = PAN_GETH( akp->panel_funcs->orig_struct, akp->about_panel );
  int pw = PAN_GETW( akp->panel_funcs->orig_struct, akp->about_panel );

  df->WriteText( akp->about_panel, "All-Keyer", COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(akp->about_panel, "All-Keyer"))/2,
                  y+10 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( akp->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(akp->about_panel, about_text[i] ))/2,
             y );
    y += 15;
  }
}

// OnKeyMode():
//  Called whenever the Key Mode changes, toggling
//  key_frame_step and key_threshold gadgets on and off.
void OnKeyMode(LWControl *ctl, AllKeyerPanel *panel) {
  int selected;
  GET_INT( ctl, selected );

  if( selected == 1 ) {
    ERASE_CON( panel->key_frame_step );
    RENDER_CON( panel->key_threshold );
  } else {
    ERASE_CON( panel->key_threshold );
    RENDER_CON( panel->key_frame_step );
  }
}

// OnAbout():
//  Called to open the About Box.
void OnAbout(LWControl *ctl, AllKeyerPanel *panel) {
  panel->OpenAbout();
}

// OnCompare():
//  Called when the compare file changes
void OnCompare(LWControl *ctl, AllKeyerPanel *panel) {
  char buffer[MAX_PATH_LENGTH];
  GET_STR( panel->compare_file, buffer, MAX_PATH_LENGTH );
  if( buffer[0] == '\0' );
    SET_STR( panel->compare_file, "(none)", 6 );
}

// OnSource():
//  Called when the source pattern changes
void OnSource(LWControl *ctl, AllKeyerPanel *panel) {
  char buffer[MAX_PATH_LENGTH];
  GET_STR( panel->source_pattern, buffer, MAX_PATH_LENGTH );
  if( buffer[0] == '\0' );
    SET_STR( panel->source_pattern, "(none)", 6 );
}

// OnDest():
//  Called when the destination path changes
void OnDest(LWControl *ctl, AllKeyerPanel *panel) {
  char buffer[MAX_PATH_LENGTH];
  GET_STR( panel->destination_path, buffer, MAX_PATH_LENGTH );
  if( buffer[0] == '\0' );
    SET_STR( panel->destination_path, "(none)", 6 );
}

// OnLog():
//  Called when the log file changes
void OnLog(LWControl *ctl, AllKeyerPanel *panel) {
  char buffer[MAX_PATH_LENGTH];
  GET_STR( panel->log_file, buffer, MAX_PATH_LENGTH );
  if( buffer[0] == '\0' );
    SET_STR( panel->log_file, "(none)", 6 );
}

// OnProcess():
//  Called to begin processing scenes.  First it checks to
//  make sure that the settings are valid (output path is OK,
//  source pattern is valid, etc.)
void OnProcess(LWControl *ctl, AllKeyerPanel *panel) {
  panel->main_panel_state = MPS_PROCESS;
  panel->panel_funcs->Close( panel->main_panel );
}
