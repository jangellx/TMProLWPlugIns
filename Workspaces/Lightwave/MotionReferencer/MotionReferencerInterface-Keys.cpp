//
// MotionReferencerInterface-Keys.cpp
//
// This contains all the Keyboard Panel functions and keyboard
//  listener functions

#include <portable/dynarray/dynarray.h>

#include "MotionReferencerInterface.h"

extern MotionReferencerInterface * mr_int;
extern DynArray< MotionReferencerInstance * > instances;

static char *keys_text[] = {
  // Key(s)                             Description
  "Alt-Up",                           "Previous Instance",
  "Alt-Down",                         "Next Instance",
  "",                                 "",
//  "s",                                "Save .SDF",
//  "l",                                "Load .SDF",
//  "",                                 "",
  NULL };


// MakeKeysPanel():
bool MotionReferencerInterface::MakeKeysPanel() {
  // Create the Panel
  keys_panel = panel_funcs->Create( "MotionReferencer Keyboard Shortcuts" );

  if( !keys_panel ) {                    // Make sure the panel was created
    message->Error("Error creating Keys panel, aborting" );
    return false;
  }

  try {
    for( int num_keys = 0; keys_text[num_keys] != NULL; num_keys++ );

    PAN_SETW( panel_funcs->orig_struct, keys_panel, 290 );
    PAN_SETH( panel_funcs->orig_struct, keys_panel, 40 + 15 * num_keys/2 );

    // Set the Draw Function
    panel_funcs->Set( keys_panel, PAN_USERDRAW, DrawKeysPanel );

    return true;

  } catch( bool a ) {
    if( !a ) {
      message->Error("Error creating Keyboard Help Panel interface controls; aborting" );
      return false;
    }
  }

  return true;
}

// OpenKeysPanel()
//  Opens the Keys panel in blocking mode
bool MotionReferencerInterface::OpenKeysPanel() {
  if( !panel_funcs->Open( keys_panel, PANF_BLOCKING ) )
    return false;
  return true;
}

// OnF1():
//  Called to open the Keyboard Help Panel.
void OnF1(LWControl *ctl, MotionReferencerInterface *panel) {
  panel->OpenKeysPanel();
}

// DrawKeysPanel():
//  Called to draw the Keyboard Help Panel
void DrawKeysPanel( LWPanelID panel, void *data, DrMode mode ) {
  CDrawFuncs *df = mr_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( mr_int->panel_funcs->orig_struct, mr_int->keys_panel );
  int pw = PAN_GETW( mr_int->panel_funcs->orig_struct, mr_int->keys_panel );

  const int column_center = 125;
  int y = 5;

  for( int i=0; keys_text[i] != NULL; i+=2 ) {
    df->WriteText( mr_int->keys_panel, keys_text[i], COLOR_WHITE,
             column_center - 5 - df->TextWidth(mr_int->about_panel, keys_text[i]), y );

    df->WriteText( mr_int->keys_panel, keys_text[i+1], COLOR_BLACK,
             column_center + 5, y );
    y += 15;
  }
}

// OnKeyDown():
//  Called on a key-down event.  This stores the state of
//   the modifiers and actually processes the keystrokes.
void OnKeyDown(LWPanelID panel, void *data, LWDualKey key) {
  if( key == LWDK_SHIFT )
    mr_int->shift_down = true;

  if( key == LWDK_CTRL )
    mr_int->ctrl_down  = true;

  if( key == LWDK_ALT )
    mr_int->alt_down  = true;

  mr_int->last_key_down = key;

  // Used to move list up/down
  int value;

  switch( mr_int->last_key_down ) {
    case -1:                             // No key deperessed, do nothing
      break;

    case LWDK_SC_UP:
      if( mr_int->alt_down ) {    // Previous Instance
        GET_INT( mr_int->instance_popup, value );
        if( --value <= 0 )
          value = 0;
        SET_INT( mr_int->instance_popup, value );
        OnInstancePopup( mr_int->instance_popup, mr_int );
      }
      break;

    case LWDK_SC_DOWN:
      if( mr_int->alt_down ) {    // Next instance
        GET_INT( mr_int->instance_popup, value );
        if( ++value >= (int)instances.NumElements() )
          value = instances.NumElements();
        SET_INT( mr_int->instance_popup, value );
        OnInstancePopup( mr_int->instance_popup, mr_int );
      }
      break;

//    case LWDK_RETURN:
//    case LWDK_PAD_ENTER:
//    case LWDK_CHAR('\r'):
    case LWDK_CHAR( 9 ):
      break;

    case LWDK_F1:                         // Keyboard Help
    case LWDK_HELP:
      OnF1( mr_int->f1, mr_int );
      break;

  }
}

// OnKeyUp():
//  Called on a key-up event.  This just clears out the
//   modifiers and the last key down.
void OnKeyUp(LWPanelID panel, void *data, LWDualKey key) {
  if( key == LWDK_SHIFT )
    mr_int->shift_down = false;

  if( key == LWDK_CTRL )
    mr_int->ctrl_down  = false;

  if( key == LWDK_ALT )
    mr_int->alt_down  = false;

  // Clear out the last key pressed
  mr_int->last_key_down = -1;
}
