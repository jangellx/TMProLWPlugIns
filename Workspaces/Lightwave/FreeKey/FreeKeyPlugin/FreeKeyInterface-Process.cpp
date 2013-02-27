//
// FreeKeyInterface-Process.cpp
//
// This contains all of the functions for FreeKey to process a
//  scene file, including all interface controls.
//
// Note that this uses platform-dependant multithreading routines.
//  Be sure to change this when porting to other platforms.
//

#include <stdio.h>

#include <windows.h>
#include <process.h>       // For Windows multithreading

#include "FreeKey-Interface.h"

// OnFreeKey():
//  This actually processes the scene file.  To make the process
//   interuptable, the process task is spawned as another thread.
void OnFreeKey(LWControl *ctl, FreeKeyInterface *panel) {  
  if( !fk_int->fkd->TestKey( fk_int->fkd->ReadKey(), fk_int->fkd->GetDongleID() ) ) {
    fk_int->message->Error( "FreeKey Error:  Licence key not found; Processing disabled",
                            "Registering FreeKey will make it fully functional." );
    return;
  }

  if( fk_int->processing ) {
    fk_int->stop_processing = true;
    return;
  }

  fk_int->stop_processing = false;
  fk_int->processing      = true;
  fk_int->progress_total  = 1;
  fk_int->progress_cur    = 0;

  // See if there's anything to Freekey
  if( !AnyAssignedToGroups( &fk->GetScene().GetBaseItem() ) ) {
    fk_int->SetStatusText( "ERROR:  No items assigned to any groups", STATUSMODE_ERROR, true );
    fk_int->message->Error( "Freekey Error:  No items have",
                            "been assigned to any groups!  Aborting" );

    fk_int->processing = false;
    fk_int->stop_processing = true;
    return;
  }

  // Change the FreeKey-ify control's text to Abort
  SET_STR( fk_int->freekey, "Abort Process", strlen( "Abort Process" ) );
  REDRAW_CON( fk_int->freekey );

  // Count the number of item for total
  fk_int->progress_total = fk->GetScene().GetBaseItem().CountItems(false, true);

  int error;
  try {
    // Begin Processing
    error = fk->Process( UpdateProgress );
    if( error != FREEKEY_OK ) {
      if( error == FREEKEY_USER_ABORT ) {  /// Check for an abort
        fk_int->message->Info( "FreeKey Info:  User Abort",
                               "Any partially written output file has been removed" );
      } else
        throw false;
    }

    sprintf( fk_int->status_text, "Complete!  Processed %d items sucessfully.",
             fk->GetNumObjectsProcessed() + fk->GetNumLightsProcessed() + fk->GetNumBonesProcessed() );
    fk_int->SetStatusText( NULL, STATUSMODE_INFO, true );
    fk_int->message->Info( "FreeKey Info:  Processing comlete!" );
  } catch( bool ) {
    // Handle Errors
//    fk->CloseFiles();

    sprintf( fk_int->status_text, "ERROR:  %s", FreeKey::GetErrorString( error ) );
    fk_int->SetStatusText( NULL, STATUSMODE_ERROR, true );

    fk_int->message->Error( "FreeKey Error, aborting:", FreeKey::GetErrorString( error ) );
  }

  // Restore the FreeKey-ify control's text
  SET_STR( fk_int->freekey, "FreeKey-ify!", strlen( "FreeKey-ify!" ) );
  REDRAW_CON( fk_int->freekey );

  fk_int->processing = false;
  fk_int->stop_processing = false;
}

// SetStatusText():
//  Sets the status text and mode.  If the text is NULL, only the mode is changed.
//   the default mode is Info if none is provided.  This function can optionally
//   redraw the status line after settings it's contents.
void FreeKeyInterface::SetStatusText( const char *text, enum fk_status_mode mode, bool draw_now ) {
  if( text != NULL )
    strcpy( status_text, text );

  status_mode = mode;

  if( draw_now )
    REDRAW_CON( fk_int->status_line );
}

// DrawStatusLine():
//  Called to draw the Status Line.
void DrawStatusLine(LWControl *ctl, FreeKeyInterface *ui, DrMode mode) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int cw = CON_HOTW( ctl );
  int ch = CON_HOTH( ctl );

  if( fk_int->processing ) {
    // Progress Bar Drawing
    // Draw the bar
    df->DrawRGBBox( fk_int->main_panel, 0, 60, 150, cx+1, cy+1,
                    (int)( (cw-2) * ((double)fk_int->progress_cur / (double)fk_int->progress_total) ), ch-2 );

    // Clear the area outside the bar
    df->DrawRGBBox( fk_int->main_panel, 132, 130, 132, cx + (cw-2) * ((double)fk_int->progress_cur / (double)fk_int->progress_total) + 1, cy+1,
                    cw - (int)( ((double)(cw-2) * ((double)fk_int->progress_cur / (double)fk_int->progress_total)) - 2), ch-2 );

    // Write the name of the current object on the bar
    df->WriteText( fk_int->main_panel, fk_int->status_text, COLOR_WHITE, cx+4, cy );
  } else {
    // Standard Message Drawing
    if( fk_int->status_mode == STATUSMODE_ERROR ) {
      df->DrawRGBBox( fk_int->main_panel, 175, 0, 0, cx+1, cy+1, cw-2, ch-2 );
      df->WriteText( fk_int->main_panel, fk_int->status_text, COLOR_LT_YELLOW, cx+4, cy );
    } else if( fk_int->status_mode == STATUSMODE_NOTICE ) {
      df->DrawRGBBox( fk_int->main_panel, 200, 185, 0, cx+1, cy+1, cw-2, ch-2 );
      df->WriteText( fk_int->main_panel, fk_int->status_text, COLOR_BLACK, cx+4, cy );
    } else {  // Default to Info mode
      df->DrawRGBBox( fk_int->main_panel, 132, 130, 132, cx+1, cy+1, cw-2, ch-2 );
      df->WriteText( fk_int->main_panel, fk_int->status_text, COLOR_BLACK, cx+4, cy );
    }
  }
}

// UpdateProgress():
//  Updates the progress bar.  If the user aborted the process, then
//   this returns false so FreeKey::Process() knows to stop working.
bool __cdecl UpdateProgress( char *name, int current, int total ) {
  // Fill in some progress info
  fk_int->progress_cur   = current;
  fk_int->progress_total = total;

  // Update the progress bar
  sprintf( fk_int->status_text, "Processing... %s", name );
//  REDRAW_CON( fk_int->status_line );
  DrawStatusLine( fk_int->status_line, fk_int, DR_REFRESH );


  // Return a continue/abort value
  return !fk_int->stop_processing;
}

// AnyAssignedToGroups()
//  Returns true if any item is assigned to any group.
//   Called just before processing.
bool AnyAssignedToGroups( LWItem *item ) {
  if( item == NULL )
    return false;

  if( item->GetGroup() != -1 )
    return true;

  for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
    if( AnyAssignedToGroups( item->GetChildren()[i] ) )
      return true;

  if( AnyAssignedToGroups( &item->GetSkeleton() ) )
    return true;

  return false;
}
