//
// FreeKeyinterface-Keys.cpp
//
// This contains all the Keyboard Panel functions and keyboard
//  listener functions

#include "FreeKey-Interface.h"

static char *keys_text[] = {
  // Key(s)                             Description
  "Up",                               "Previous Item",
  "Down",                             "Next Item",
  "Shift-Up",                         "First Item",
  "Shift-Down",                       "Last Item",
  "Ctrl-Up",                          "Page Up Item List",
  "Ctrl-Down",                        "Page Down Item List",
  "",                                 "",
  "c",                                "Select Current And Its Children",
  "C",                                "Deselect Current And Its Children",
  "Alt-c",                            "Select Children",
  "g",                                "Select All in Group",
  "G",                                "Deselect All In Group",
  "Alt-g",                            "Add Selected to Current Group",
  "Alt-a",                            "Select All",
  "/",                                "Deselect All",
  "[",                                "Previous Selection Method",
  "]",                                "Next Selection Method",
  "Space",                            "Cycle Select/Deselect/Invert Mode",
  "Return, Enter",                    "Apply Selection",
  "",                                 "",
  "f",                                "Fold/Unfold Current",
  "F",                                "Fold/Unfold All",
  "Tab",                              "Cycle Display Mode Forward",
  "Shift-Tab",                        "Cycle Display Mode Backward",
  "",                                 "",
  "Alt-Up",                           "Previous Group",
  "Alt-Down",                         "Next Group",
  "",                                 "",
  "{",                                "Previous Group Edit Mode",
  "}",                                "Next Group Edit Mode",
  "Shift-Return, Shift-Enter",        "Add Selection To Group",
  "",                                 "",
  "+, =",                             "Add New Group",
  "-",                                "Remove Current Group",
  NULL };


// MakeKeysPanel():
bool FreeKeyInterface::MakeKeysPanel() {
  // Create the Panel
  keys_panel = panel_funcs->Create( "FreeKey Keyboard Shortcuts" );

  if( !keys_panel ) {                    // Make sure the panel was created
    message->Error("Error creating Keys panel, aborting" );
    return false;
  }

try {
  for( int num_keys = 0; keys_text[num_keys] != NULL; num_keys++ );

  PAN_SETW( panel_funcs->orig_struct, keys_panel, 380 );
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
bool FreeKeyInterface::OpenKeysPanel() {
  if( !panel_funcs->Open( keys_panel, PANF_BLOCKING ) )
    return false;
  return true;
}

// OnF1():
//  Called to open the Keyboard Help Panel.
void OnF1(LWControl *ctl, FreeKeyInterface *panel) {
  panel->OpenKeysPanel();
}

// DrawKeysPanel():
//  Called to draw the Keyboard Help Panel
void DrawKeysPanel( LWPanelID panel, void *data, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int ph = PAN_GETH( fk_int->panel_funcs->orig_struct, fk_int->keys_panel );
  int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->keys_panel );

  const int column_center = 170;
  int y = 5;

  for( int i=0; keys_text[i] != NULL; i+=2 ) {
    df->WriteText( fk_int->keys_panel, keys_text[i], COLOR_WHITE,
             column_center - 5 - df->TextWidth(fk_int->about_panel, keys_text[i]), y );

    df->WriteText( fk_int->keys_panel, keys_text[i+1], COLOR_BLACK,
             column_center + 5, y );
    y += 15;
  }
}

// OnKeyDown():
//  Called on a key-down event.  This stores the state of
//   the modifiers and actually processes the keystrokes.
void OnKeyDown(LWPanelID panel, void *data, DualKey key) {
  if( key == DK_SHIFT )
    fk_int->shift_down = true;

  if( key == DK_CTRL )
    fk_int->ctrl_down  = true;

  if( key == DK_ALT ) {
    if( fk_int->draw_bbox && !fk_int->alt_down ) {
      fk_int->quickdraw_item_list = true;
      fk_int->RedrawItemList();
    }
    fk_int->alt_down  = true;
  }

  fk_int->last_key_down = key;

  // Used to move list up/down
  int list_height = CON_H( fk_int->item_list ) - 20;
  list_height = (list_height/ fk_int->dd.item_height);
  int max_top = fk_int->sorted_items.NumElements() - list_height;

  int value;
  static bool last_was_folded = false;  // Used to determine what to do on a Fold/Unfold All

  switch( fk_int->last_key_down ) {
    case -1:                            // No key deperessed, do nothing
      break;

    case DK_SC_UP:
      if( fk_int->shift_down ) {        // Jump to the top of the Item List
        fk->SetTopItem( 0 );
        SET_INT( fk_int->item_scroll, fk->GetTopItem() );
        fk_int->RedrawItemList();
      } else if( fk_int->alt_down && fk_int->ctrl_down ) {
        ;
      } else if( fk_int->alt_down ) {   // Scroll Group List Up (1 element)
        GET_INT( fk_int->group_list, value );
        if( value > 0 ) {
          SET_INT( fk_int->group_list, --value );
          OnGroupList( fk_int->group_list, fk_int );
        }
      } else if( fk_int->ctrl_down ) {
        fk->SetTopItem( fk->GetTopItem() - list_height );
        if( fk->GetTopItem() < 0 )
          fk->SetTopItem( 0 );
        SET_INT( fk_int->item_scroll, fk->GetTopItem() );
        fk_int->RedrawItemList();
      } else {                          // Scroll Item List Up (1 element)
        fk->SetTopItem( fk->GetTopItem() - 1 );
        if( fk->GetTopItem() < 0 )
          fk->SetTopItem( 0 );
        else {
          SET_INT( fk_int->item_scroll, fk->GetTopItem() );
          fk_int->RedrawItemList();
        }
      }
      break;

    case DK_SC_DOWN:
      if( fk_int->shift_down ) {        // Jump to the bottom of the Item List
        fk->SetTopItem( max_top );
        SET_INT( fk_int->item_scroll, fk->GetTopItem() );
        fk_int->RedrawItemList();
      } else if( fk_int->alt_down && fk_int->ctrl_down ) {
        ;
      } else if( fk_int->alt_down ) {   // Scroll Group List Down (1 element)
        GET_INT( fk_int->group_list, value );
        if( value < (int)fk->GetGroupList().NumElements() - 2 ) {   // Don't go to (new group)!
          SET_INT( fk_int->group_list, ++value );
          OnGroupList( fk_int->group_list, fk_int );
        }
      } else if( fk_int->ctrl_down ) {  // Scroll Item List Down (1 page)
        fk->SetTopItem( fk->GetTopItem() + list_height );
        if( fk->GetTopItem() > max_top )
          fk->SetTopItem( max_top );
        SET_INT( fk_int->item_scroll, fk->GetTopItem() );
        fk_int->RedrawItemList();
      } else {                          // Scroll Item List Down (1 element)
        fk->SetTopItem( fk->GetTopItem() + 1 );
        if( fk->GetTopItem() > max_top )
          fk->SetTopItem( max_top );
        else {
          SET_INT( fk_int->item_scroll, fk->GetTopItem() );
          fk_int->RedrawItemList();
        }
      }
      break;

    case DK_RETURN:
    case DK_PAD_ENTER:
    case '\r':
      if( fk_int->shift_down )          // Add selected to group (Apply)
        OnGroupEditApply( fk_int->group_edit_apply, fk_int );
      else                              // Add to selection (Apply)
        OnApplySelection( fk_int->apply_select_item_name, fk_int );
      break;

    case DK_F1:                         // Keyboard Help
    case DK_HELP:
      OnF1( fk_int->f1, fk_int );
      break;

    case DK_CHAR('='):                  // Add a New Group
    case DK_CHAR('+'):
      fk->SetCurrentGroup( fk->GetGroupList().NumElements() - 1 );
      OnGroupList( fk_int->group_list, fk_int );
      break;

    case DK_CHAR('-'):                  // Remove an Existing Group
      OnGroupDel( fk_int->group_delete, fk_int );
      break;

    case DK_CHAR('['):                  // Scroll Selection Mode Up (1 element)
      GET_INT( fk_int->select_item_mode, value );
      if( value > 0 ) {
        SET_INT( fk_int->select_item_mode, --value );
        OnSelectionMode( fk_int->select_item_mode, fk_int );
      }
      break;

    case DK_CHAR(']'):                  // Scroll Selection Mode Down (1 element)
      GET_INT( fk_int->select_item_mode, value );
      if( value < num_selection_modes - 1) {
        SET_INT( fk_int->select_item_mode, ++value );
        OnSelectionMode( fk_int->select_item_mode, fk_int );
      }
      break;

    case DK_CHAR('{'):                  // Scroll the Group Edit Mode Up (1 element)
      GET_INT( fk_int->group_edit_mode, value );
      if( value > 0 )
        SET_INT( fk_int->group_edit_mode, --value );
      break;

    case DK_CHAR('}'):                  // Scroll the Group Edit Mode Up (1 element)
      GET_INT( fk_int->group_edit_mode, value );
      if( value < num_group_edit_modes - 1 )
        SET_INT( fk_int->group_edit_mode, ++value );
      break;

    case DK_CHAR(' '):                  // Cycle Select/Deselect/Invert
      GET_INT( fk_int->select_toggle, value );
      if( value < num_selection_toggles - 1)
        SET_INT( fk_int->select_toggle, ++value );
      else
        SET_INT( fk_int->select_toggle, 0 );

      OnSelectionMode( fk_int->select_item_mode, fk_int );
      break;

    case DK_CHAR('f'):                  // Fold/Unfold Current Item
      if( fk->IsCurrentItemValid() ) {
        fk_int->sorted_items[ fk->GetCurrentItem() ]->SetFolded( !fk_int->sorted_items[ fk->GetCurrentItem() ]->GetFolded() );
        fk_int->sorted_items.Reset();                       // Rebuild item list to represent fold states
        if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 )
          fk->GetScene().GetBaseItem().BuildFoldedItemList( fk_int->sorted_items, true, false );

        if( fk->GetCurrentItem() > fk_int->sorted_items.NumElements() - 1 )
          fk->SetCurrentItem( fk_int->sorted_items.NumElements() - 1 );

        OnDisplayMode( fk_int->display_item_mode, fk_int );
      }
      break;

    case DK_CHAR('F'):                  // Fold/Unfold All Items
      last_was_folded = !last_was_folded;
      if( last_was_folded )
        OnFoldList( fk_int->list_fold, fk_int );
      else 
        OnUnFoldList( fk_int->list_unfold, fk_int );

      OnDisplayMode( fk_int->display_item_mode, fk_int );
      break;

    case DK_CHAR('a'):
      if( fk_int->alt_down ||           // Select All
          fk_int->ctrl_down ) {
        SelectAll();
        fk_int->RedrawItemList();
        fk_int->UpdateSelectedCount();
      }
      break;

    case DK_CHAR('/'):                  // Deselect All
      DeselectAll();
      fk_int->UpdateSelectedCount();
      fk_int->RedrawItemList();
      break;

    case DK_CHAR('c'):
      if( fk_int->alt_down ) {          // Select Children
        SelectChildren();
        fk_int->UpdateSelectedCount();
      } else {                          // Select Current And It's Children
        fk_int->UpdateSelectedCount();
        SelectItem( fk_int->sorted_items[ fk->GetCurrentItem() ], true );
      }

      fk_int->RedrawItemList();
      break;

    case DK_CHAR('C'):                   // Deselect Current And It's Children
      DeselectItem( fk_int->sorted_items[ fk->GetCurrentItem() ], true );
      fk_int->UpdateSelectedCount();
      fk_int->RedrawItemList();
      break;

    case DK_CHAR('g'):
      if( fk_int->alt_down ||            // Add Selected To Group
          fk_int->ctrl_down ) {
        SetSelectionToGroup( fk->GetCurrentGroup() );
        fk_int->RedrawItemList();
      } else {                           // Select all in group
        SelectAllInGroup( fk->GetCurrentGroup(), false );
        fk_int->RedrawItemList();
      }
      break;

    case DK_CHAR('G'):                   // Deselect all in group
      DeselectAllInGroup( fk->GetCurrentGroup(), false );
      fk_int->UpdateSelectedCount();
      fk_int->RedrawItemList();
      break;

    case DK_CHAR( 9 ):                   // Cyle Display Mode -
      GET_INT( fk_int->display_item_mode, value );
      if( fk_int->shift_down ) {         // - Backward
        if( --value < 0 ) { value = 0; }
      } else {                           // - Forward
        if( ++value > 7 ) { value = 7; }
      }

      SET_INT( fk_int->display_item_mode, value );
      OnDisplayMode( fk_int->display_item_mode, fk_int );
      break;

    case DK_CHAR( '`' ):                 // Draw Style (that's the tilde key)
      if( fk->GetDrawStyle() == DRAWSTYLE_56 ) {
        fk->SetDrawStyle( DRAWSTYLE_L6 );
        fk_int->SetStatusText( "Now using L6 Draw Style", STATUSMODE_INFO, true );
      } else {
        fk->SetDrawStyle( DRAWSTYLE_56 );
        fk_int->SetStatusText( "Now using 5.6 Draw Style", STATUSMODE_INFO, true );
      }
      SET_INT( fk_int->opt_draw_style, fk->GetDrawStyle() );
      fk_int->RedrawItemList();
      break;
  }
}

// OnKeyUp():
//  Called on a key-up event.  This just clears out the
//   modifiers and the last key down.
void OnKeyUp(LWPanelID panel, void *data, DualKey key) {
  if( key == DK_SHIFT )
    fk_int->shift_down = false;

  if( key == DK_CTRL )
    fk_int->ctrl_down  = false;

  if( key == DK_ALT ) {
    fk_int->alt_down  = false;
    if( fk_int->quickdraw_item_list ) {
      fk_int->quickdraw_item_list = false;
      fk_int->RedrawItemList();
    }
  }

  // Clear out the last key pressed
  fk_int->last_key_down = -1;
}
