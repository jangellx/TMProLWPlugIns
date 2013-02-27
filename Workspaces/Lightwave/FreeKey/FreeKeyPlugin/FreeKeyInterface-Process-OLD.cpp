//
// This contains source that attempting to run the FreeKey
//  process as a seperate thread.  Unfortunately, this killed
//  LW, since it would call panels and lock the UI,
//  and then the main thread would call the some redraw function
//  through panels itself, and then it would crash.
//

#include <stdio.h>

#include <windows.h>
#include <process.h>       // For Windows multithreading

#include "FreeKey-Interface.h"

// Globals
bool fk_continue_processing;
bool processing_open = false;

// OnFreeKey():
//  This actually processes the scene file.  To make the process
//   interuptable, the process task is spawned as another thread.
void OnFreeKey(LWControl *ctl, FreeKeyInterface *panel) {
  fk_int->OpenProcessPanel();
}

// MakeProcessPanel():
bool FreeKeyInterface::MakeProcessPanel() {
  // Create the Panel
  process_panel = panel_funcs->Create( main_title );

  if( !process_panel ) {                    // Make sure the panel was created
    message->Error("Error creating Processing Panel, aborting" );
    return false;
  }

try {
  // Progress Bar Area
  if( !( progress_bar = CANVAS_CTL( panel_funcs->orig_struct, process_panel,
                                  "", 320, 20 ) ) )
    throw false;

  int x = CON_X(progress_bar);
  int y = CON_Y(progress_bar);
  MOVE_CON( progress_bar, x+5, y + 20 );

  // Set the Panel Size
  //PAN_SETW( panel_funcs->orig_struct, progress_panel, 300 );
  PAN_SETH( panel_funcs->orig_struct, process_panel, 110 );
  
  // Set the Draw Functions
  panel_funcs->Set( process_panel, PAN_USERDRAW, DrawProcessPanel );

  ival.intv.value = (int)&DrawProgressBar;
  (*progress_bar->set)( progress_bar, CTL_USERDRAW, &ival );

  return true;

} catch( bool a ) {
  if( !a ) {
    message->Error("Error creating About Box interface controls; aborting" );
    return false;
  }
}

return true;
}

// OpenProcessPanel()
//  Opens the progress panel in blocking mode
bool FreeKeyInterface::OpenProcessPanel() {
  // Initialize the progress variables
  processing_open = false;
  fk_continue_processing = true;
  fk_int->progress_total = 1;
  fk_int->progress_cur   = 0;

  // Spawn the Process Thread
  _beginthread( ProcessScene, 0, NULL );

  // Open the panel
  if( panel_funcs->Open( process_panel, PANF_BLOCKING ) )
    fk_continue_processing = false;

  // Clear the progress name
  if( fk_int->progress_name != NULL ) {
    free( fk_int->progress_name );
    fk_int->progress_name = NULL;
  }

  return true;
}

// CloseProcessPanel()
//  Closes the progress panel
void FreeKeyInterface::CloseProcessPanel() {
  if( processing_open )
    panel_funcs->Close( fk_int->process_panel );
}

// DrawProgressBar():
//  Called to draw the Progress Bar.
void DrawProgressBar(LWControl *ctl, FreeKeyInterface *ui, DrMode mode) {
  if( !processing_open )
    return;

  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_X( ctl );
  int cy = CON_Y( ctl );
  int cw = CON_W( ctl );
  int ch = CON_H( ctl );

  // Clear the control
  df->DrawBox( fk_int->process_panel, COLOR_BG, cx+1, cy+1, cw-2, ch-2 );

  // Draw the bar
  df->DrawRGBBox( fk_int->process_panel, 0, 200, 200, cx+1, cy+1,
                  (cw-2) * (fk_int->progress_cur / fk_int->progress_total), ch-2 );

  // Write the name of the current object on the bar
  df->WriteText( fk_int->process_panel, fk_int->progress_name, COLOR_WHITE, cx, cy+2 );
}

// DrawProcessPanel():
//  Called to draw the Processing Panel
void DrawProcessPanel( LWPanelID panel, void *data, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int y  = CON_Y( fk_int->progress_bar );
  int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->process_panel );
  
  df->WriteText( fk_int->process_panel, "Processing...", COLOR_BLACK,
                 (pw/2)-(df->TextWidth(fk_int->process_panel, "Processing..."))/2,
                  y-15 );

  df->WriteText( fk_int->process_panel, "(Press OK to abort)", COLOR_BLACK,
                 (pw/2)-(df->TextWidth(fk_int->process_panel, "(Press OK to abort)"))/2,
                  y+30 );

  processing_open = true;
}

// ProcessScene():
//  Called to actually process the scene file.  This function
//   is designed to be run as a seperate thread.
void __cdecl ProcessScene( void *dummy ) {
  // Wait until the panel is open
  while( !processing_open )
    Sleep( 100L );

  int error;
  try {
    error = fk->OpenFiles();
    if( error != FREEKEY_OK )
      throw false;
    
    error = fk->Process( UpdateProgress );
    if( error != FREEKEY_OK ) {
      if( error == FREEKEY_USER_ABORT ) {  /// Check for an abort
        fk_int->message->Info( "FreeKey Info:  User Abort",
                               "Any partially written output file has been removed" );
      } else
        throw false;
    }

    fk->CloseFiles();
    fk_int->CloseProcessPanel();

    char buffer[256];
    sprintf( buffer, "Processed %d items into \"%s\"",
             fk->GetNumObjectsProcessed() + fk->GetNumLightsProcessed() + fk->GetNumBonesProcessed() );
    fk_int->message->Info( "FreeKey Info:  Processing comlete!",
                           buffer );

  } catch( bool ) {
    fk->CloseFiles();
    fk_int->CloseProcessPanel();
    fk_int->message->Error( "FreeKey Error (FATAL):", FreeKey::GetErrorString( error ) );
  }

  // Clean-up
  _endthread();
}

// UpdateProgress():
//  Updates the progress bar.  If the user aborted the process (closed
//   the Processing panel), then this returns false so FreeKey::Process()
//   knows to stop working.
bool __cdecl UpdateProgress( char *name, int current, int total ) {
  if( !processing_open )
    return true;

  // Free any existing progress bar item name
  if( fk_int->progress_name != NULL ) {
    free( fk_int->progress_name );
    fk_int->progress_name = NULL;
  }

  // Fill in some progress info
  fk_int->progress_name  = strdup( name );
  fk_int->progress_cur   = current;
  fk_int->progress_total = total;

  // Update the progress bar
  REDRAW_CON( fk_int->progress_bar );

  // Return a continue/abort value
  return fk_continue_processing;
}
