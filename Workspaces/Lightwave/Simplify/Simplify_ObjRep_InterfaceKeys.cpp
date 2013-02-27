//
// Simplifyinterface-Keys.cpp
//
// This contains all the Keyboard Panel functions and keyboard
//  listener functions

#include "Simplify_ObjRep_Interface.h"

extern SimplifyInterface * simp_int;
extern DynArray< SimplifyInstance * > instances;

#ifdef DEBUG_PANEL_SIZE
extern int  psize_w;
extern bool update_size;
#endif

static char *keys_text[] = {
  // Key(s)                             Description
  "Alt-Up",                           "Previous Instance",
  "Alt-Down",                         "Next Instance",
  "",                                 "",
//  "s",                                "Save .SDF",
//  "l",                                "Load .SDF",
//  "",                                 "",
  "Up",                               "Previous LOD",
  "Down",                             "Next LOD",
  "Shift-Up",                         "First LOD",
  "Shift-Down",                       "Last LOD",
  "Ctrl-Up",                          "Page Up LOD List",
  "Ctrl-Down",                        "Page Down LOD List",
  "",                                 "",
  "+, =",                             "Add New LOD",
  "-",                                "Remove Current LOD",
  "c",                                "Clone Current LOD",
  "",                                 "",
  "TAB",                              "Toggle Current LOD",
  NULL };


// MakeKeysPanel():
bool SimplifyInterface::MakeKeysPanel() {
  // Create the Panel
  keys_panel = panel_funcs->Create( "Simplify Keyboard Shortcuts" );

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
bool SimplifyInterface::OpenKeysPanel() {
  if( !panel_funcs->Open( keys_panel, PANF_BLOCKING ) )
    return false;
  return true;
}

// OnF1():
//  Called to open the Keyboard Help Panel.
void OnF1(LWControl *ctl, SimplifyInterface *panel) {
  panel->OpenKeysPanel();
}

// DrawKeysPanel():
//  Called to draw the Keyboard Help Panel
void DrawKeysPanel( LWPanelID panel, void *data, DrMode mode ) {
  CDrawFuncs *df = simp_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( simp_int->panel_funcs->orig_struct, simp_int->keys_panel );
  int pw = PAN_GETW( simp_int->panel_funcs->orig_struct, simp_int->keys_panel );

  const int column_center = 125;
  int y = 5;

  for( int i=0; keys_text[i] != NULL; i+=2 ) {
    df->WriteText( simp_int->keys_panel, keys_text[i], COLOR_WHITE,
             column_center - 5 - df->TextWidth(simp_int->about_panel, keys_text[i]), y );

    df->WriteText( simp_int->keys_panel, keys_text[i+1], COLOR_BLACK,
             column_center + 5, y );
    y += 15;
  }
}

// OnKeyDown():
//  Called on a key-down event.  This stores the state of
//   the modifiers and actually processes the keystrokes.
void OnKeyDown(LWPanelID panel, void *data, DualKey key) {
  if( key == DK_SHIFT )
    simp_int->shift_down = true;

  if( key == DK_CTRL )
    simp_int->ctrl_down  = true;

  if( key == DK_ALT )
    simp_int->alt_down  = true;

  simp_int->last_key_down = key;

  // Used to move list up/down
  int list_height = CON_H( simp_int->lod_listbox );
  list_height = (list_height/item_height) - 1;
  int max_top = simp_int->instance.lod.NumElements() - list_height;

  int value;

  switch( simp_int->last_key_down ) {
    case -1:                             // No key deperessed, do nothing
      break;

    case DK_SC_UP:
      if( simp_int->shift_down ) {         // Jump to the top of the Item List
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
          simp_int->lod_top = 0;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      } else if( simp_int->alt_down && simp_int->ctrl_down ) {
        ;
      } else if( simp_int->alt_down ) {    // Previous Instance
        GET_INT( simp_int->instance_popup, value );
        if( --value <= 0 )
          value = 0;
        SET_INT( simp_int->instance_popup, value );
        OnInstancePopup( simp_int->instance_popup, simp_int );
      } else if( simp_int->ctrl_down ) {
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
            simp_int->lod_top -= list_height;
          if( simp_int->lod_top < 0 )
            simp_int->lod_top = 0;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      } else {                           // Scroll Item List Up (1 element)
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
            simp_int->lod_top--;
          if( simp_int->lod_top < 0 )
            simp_int->lod_top = 0;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      }
      break;

    case DK_SC_DOWN:
      if( simp_int->shift_down ) {         // Jump to the bottom of the Item List
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
          simp_int->lod_top = max_top;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      } else if( simp_int->alt_down && simp_int->ctrl_down ) {
        ;
      } else if( simp_int->alt_down ) {    // Next instance
        GET_INT( simp_int->instance_popup, value );
        if( ++value >= (int)instances.NumElements() )
          value = instances.NumElements();
        SET_INT( simp_int->instance_popup, value );
        OnInstancePopup( simp_int->instance_popup, simp_int );
      } else if( simp_int->ctrl_down ) {   // Scroll LOD List Down (1 page)
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
          simp_int->lod_top += list_height;
          if( simp_int->lod_top > max_top )
            simp_int->lod_top = max_top;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      } else {                           // Scroll LOD List Down (1 element)
        if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
          simp_int->lod_top++;
          if( simp_int->lod_top > max_top )
            simp_int->lod_top = max_top;
          SET_INT( simp_int->lod_scroll, simp_int->lod_top );
          REDRAW_CON( simp_int->lod_listbox );
        }
      }
      break;

//    case DK_RETURN:
//    case DK_PAD_ENTER:
//    case DK_CHAR('\r'):
    case DK_CHAR( 9 ):
      if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
        GET_INT( simp_int->lod_enable, value );
        value = (value > 0) ? value = 0 : 1;
        SET_INT( simp_int->lod_enable, value );
        OnLODEnable( simp_int->lod_enable, simp_int );
      }
      break;

    case DK_F1:                         // Keyboard Help
    case DK_HELP:
      OnF1( simp_int->f1, simp_int );
      break;

    case DK_CHAR('='):                  // Add a New LOD
    case DK_CHAR('+'):
      if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() )
        OnLODAdd( simp_int->lod_add, simp_int );
      break;

    case DK_CHAR('-'):                  // Remove an Existing LOD
      if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() )
        OnLODRemove( simp_int->lod_remove, simp_int );
      break;

    case DK_CHAR('c'):                  // Clone LOD
      if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() )
        OnLODClone( simp_int->lod_clone, simp_int );
      break;

//  These are disabled until I get around to making a file requester pop-up.
//   Right now these just try to load/save the default SDF names.
//    case DK_CHAR('l'):                  // Load .SDF
//      OnLoadSDF( simp_int->load_sdf, simp_int );
//      break;
//
//    case DK_CHAR('s'):                  // Save .SDF
//      OnSaveSDF( simp_int->save_sdf, simp_int );
//      break;

#ifdef DEBUG_PANEL_SIZE
    case DK_SC_LEFT:
      if( simp_int->ctrl_down ) {
        if( simp_int->shift_down )
          psize_w-=10;
        else
          psize_w-=1;
        update_size = true;
        simp_int->CloseMainPanel();
      }
      break;

    case DK_SC_RIGHT:
      if( simp_int->ctrl_down ) {
        if( simp_int->shift_down )
          psize_w+=10;
        else
          psize_w+=1;
        update_size = true;
        simp_int->CloseMainPanel();
      }
      break;
#endif
  }
}

// OnKeyUp():
//  Called on a key-up event.  This just clears out the
//   modifiers and the last key down.
void OnKeyUp(LWPanelID panel, void *data, DualKey key) {
  if( key == DK_SHIFT )
    simp_int->shift_down = false;

  if( key == DK_CTRL )
    simp_int->ctrl_down  = false;

  if( key == DK_ALT )
    simp_int->alt_down  = false;

  // Clear out the last key pressed
  simp_int->last_key_down = -1;
}
