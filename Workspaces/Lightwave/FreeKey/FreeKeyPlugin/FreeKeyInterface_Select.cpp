//
// FreeKeyInterface_Select.cpp
//
// Contains all the selection functions for the FreeKey Interface.
//  Note that many of these routines are not exactly optimal, and
//  often scan through a single object multiple times (ie:
//  SelectChildren()) while checking an item and it's children.
//

#include "FreeKey-Interface.h"

extern FreeKeyInterface * fk_int;
extern FreeKey          * fk;

// ToggleItem():
//  Toggles the selection state of an item.  If children == true,
//   children will be processed based on match_parent if true, 
//   then the childrens' states will be that of the parent; if not,
//   their states will be toggled individually.
//   Bones are considered GetChildren().
void ToggleItem( LWItem *item, bool children, bool match_parent) {
  // See if the item is in the selected list
  int ix = fk->GetSelectedItems().FindIndexOf( item );
  if( ix == DA_NO_MATCH ) {
    fk->GetSelectedItems().Add( item );
    item->SetSelected( true );
  } else {
    fk->GetSelectedItems().Remove( ix );
    item->SetSelected( false );
  }
 
  // Add the children, if applicable
  if( children ) {
    if( match_parent ) {
      for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ ) {
        if( ix == DA_NO_MATCH ) {                               // See if we should be selecting or removing
          SelectItem( item->GetChildren()[i], children );
          item->GetChildren()[i]->SetSelected( true );
        } else {
          DeselectItem( item->GetChildren()[i], children );
          item->GetChildren()[i]->SetSelected( false );
        }
      }
      if( item->HasSkeleton() ) {
        if( ix == DA_NO_MATCH )                                 // See if we should be selecting or removing
          SelectItem( &item->GetSkeleton(), children );
        else
          DeselectItem( &item->GetSkeleton(), children );
        item->GetSkeleton().SetSelected( false );
      }

    } else {
      for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
        ToggleItem( item->GetChildren()[i], children, match_parent );

      if( item->HasSkeleton() )
        ToggleItem( &item->GetSkeleton(), children, match_parent );
    }
  }
}

// SelectItem():
//  Selects the given item and optionally it's children if they aren't
//   already selected.  Bones are considered GetChildren().
void SelectItem( LWItem *item, bool children ) {
  if( item == NULL )
    return;

  // See if the item is in the selected list
  if( !(item->GetItemType() & LWI_NON_ITEMS) ) {             // Don't select things that aren't items (scene base, skeleton base), but do select their children
    int ix = fk->GetSelectedItems().FindIndexOf( item );
    if( ix == DA_NO_MATCH ) {
      fk->GetSelectedItems().Add( item );
      item->SetSelected( true );
    }
  }
 
  // Add the children, if applicable
  if( children ) {
    for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
      SelectItem( item->GetChildren()[i], children );

    if( item->HasSkeleton() )
      SelectItem( &item->GetSkeleton(), children );
  }
}

// DeselectItem():
//  Deselects the given item and optionally it's children if they aren't
//   already selected.  Bones are considered GetChildren().
void DeselectItem( LWItem *item, bool children ) {
  if( item == NULL )
    return;

  // See if the item is in the selected list
  if( !(item->GetItemType() & LWI_NON_ITEMS) ) {             // Don't deselect things that aren't items (scene base, skeleton base), but do deselect their children
    int ix = fk->GetSelectedItems().FindIndexOf( item );
    if( ix != DA_NO_MATCH ) {
      fk->GetSelectedItems().Remove( ix );
      item->SetSelected( false );
    }
  }

  // Add the children, if applicable
  if( children ) {
    for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
      DeselectItem( item->GetChildren()[i], children );

    if( item->HasSkeleton() )
      DeselectItem( &item->GetSkeleton(), children );
  }
}

// SelectItemByNameExact():
//  Selects the items and optionally the children that exactly
//   match the item name.
void SelectItemByNameExact( char *name, int select, bool children ) {
  unsigned int i;
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildItemList( store, true, false );

  for( i = 0; i < store.NumElements(); i++ ) {
    if( stricmp( store[i]->GetName(), name ) == 0 ) {
      if( select == 0 )
        SelectItem( store[i], children );
      else if( select == 1 )
        DeselectItem( store[i], children );
      else
        ToggleItem( store[i], false, false );
    }
  }
}

// SelectItemByNameStart():
//  Selects the items and optionally the children that start
//   with the item name.
void SelectItemByNameStart( char *name, int select, bool children ) {
  unsigned int i;
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildItemList( store, true, false );

  int len = strlen( name );
  for( i = 0; i < store.NumElements(); i++ ) {
    if( strnicmp( store[i]->GetName(), name, len ) == 0 ) {
      if( select == 0 )
        SelectItem( store[i], children );
      else if( select == 1 )
        DeselectItem( store[i], children );
      else
        ToggleItem( store[i], false, false );
    }
  }
}

// SelectItemByNameEnd():
//  Selects the items and optionally the children that end
//   with the item name.
void SelectItemByNameEnd( char *name, int select, bool children ) {
  unsigned int i;
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildItemList( store, true, false );

  int len = strlen( name );
  int comp_len;
  for( i = 0; i < store.NumElements(); i++ ) {
    comp_len = strlen( store[i]->GetName() );
    if( comp_len >= len ) {
      if( strnicmp( &(store[i]->GetName()[ comp_len - len ]), name, len ) == 0 ) {
        if( select == 0 )
          SelectItem( store[i], children );
        else if( select == 1 )
          DeselectItem( store[i], children );
        else
          ToggleItem( store[i], false, false );
      }
    }
  }
}

// SelectItemByNameContain():
//  Selects the items and optionally the children that contain
//   the item name.
void SelectItemByNameContain( char *name, int select, bool children ) {
  unsigned int i;
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildItemList( store, true, false );

  int len = strlen( name );
  for( i = 0; i < store.NumElements(); i++ ) {
    if( StringTools::stristr( store[i]->GetName(), name ) != NULL ) {
      if( select == 0 )
        SelectItem( store[i], children );
      else if( select == 1 )
        DeselectItem( store[i], children );
      else
        ToggleItem( store[i], false, false );
    }
  }
}

// InvertSelection():
//  Toggles the selection state of all items.
void InvertSelection() {
  ToggleItem( &fk->GetScene().GetBaseItem(), true, false );
  fk->GetScene().GetBaseItem().SetSelected( false );

/*
  unsigned int i, j;
  for( i = 0; i < fk->GetBaseItem().GetChildren().NumElements(); i++ ) {
    ToggleItem( fk->GetItemList()[i], true, false );

    for( j = 0; j < fk->GetItemList()[i]->GetChildren().NumElements(); j++ )
      ToggleItem( fk->GetItemList()[i]->GetChildren()[j], false );

    for( j = 0; j < fk->GetItemList()[i]->skeleton.NumElements(); j++ )
      ToggleItem( fk->GetItemList()[i]->skeleton[j], false );
  }
*/
}

// SelectAll():
//  Selects all items
void SelectAll() {
  for( unsigned int i=0; i < fk->GetScene().GetBaseItem().GetChildren().NumElements(); i++ )
    SelectItem( fk->GetScene().GetBaseItem().GetChildren()[i], true );
}

// DeselectAll():
//  Removes all items fromt he selection
void DeselectAll() {
  for( unsigned int i=0; i < fk->GetSelectedItems().NumElements(); i++ )
    fk->GetSelectedItems()[i]->SetSelected( false );
  fk->GetSelectedItems().Reset();
}

// ToggleRange():
//  Toggles the selection state of the items between the first
//   and last indices provided.  All indices within the range
//   inclusive will be changed to the state of the last item
//   after toggling.  This does not affect folded GetChildren().
void ToggleRange( int first, int last ) {
  // Handle one end of the range being -1 (ie: no item selected)
  if( first == -1 ) {
    SelectItem( fk_int->sorted_items[last], false );
    return;
  }

  if( last == -1 ) {
    SelectItem( fk_int->sorted_items[first], false );
    return;
  }

  // Toggle the state of the last item
  ToggleItem( fk_int->sorted_items[ last ], false );
  bool state = fk_int->sorted_items[ last ]->GetSelected();

  int low  = first;
  int high = last;
  if( high < low ) {     // Sort low-to-high
    int temp = high;
    high = low;
    low = temp;
    low++;               // Make sure we don't select last again
  } else
    high--;              // Make sure we don't select last again

  for( int i = low; i <= high; i++ ) {
    if( state )
      SelectItem( fk_int->sorted_items[ i ], false );
    else
      DeselectItem( fk_int->sorted_items[ i ], false );
  }
}

// SelectAllInGroup():
//  Selects all items in the given group
void SelectAllInGroup( int group, bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildGroupItemList( store, fk->GetCurrentGroup(), true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    SelectItem( store[ i ], children );
}

// DeselectAllInGroup():
//  Deselects all items in the given group
void DeselectAllInGroup( int group, bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildGroupItemList( store, fk->GetCurrentGroup(), true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    DeselectItem( store[ i ], children );
}

// InvertAllInGroup():
//  Inverts all items in the given group
void InvertAllInGroup( int group, bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildGroupItemList( store, fk->GetCurrentGroup(), true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    ToggleItem( store[ i ], false, false );
}

// SelectAllSelected():
//  Selects all currently selected items.  Most useful for
//   selecting those item's children.
void SelectAllSelected( bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildSelectedItemList( store, true, true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    SelectItem( store[ i ], children );
}

// DeselectAllSelected():
//  Deselects all currently selected items.  Most useful for
//   deselecting those item's children.
void DeselectAllSelected( bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildSelectedItemList( store, true, true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    DeselectItem( store[ i ], false );                        // No need to do children; if they're selected, they'll be unselected here
}

// SelectAllUnselected():
//  Selects all currently unselected items. 
void SelectAllUnselected( bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildSelectedItemList( store, false, true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    SelectItem( store[ i ], children );
}

// DeselectAllUnSelected():
//  Deselects all currently selected items.
void DeselectAllUnselected( bool children ) {
  DynArray< LWItem * > store;
  fk->GetScene().GetBaseItem().BuildSelectedItemList( store, false, true, false );

  for( unsigned int i=0; i < store.NumElements(); i++ )
    DeselectItem( store[ i ], children );
}


// SelectChildren():
//  Selects the children of the currently selected items.
//   Bones are considered children.
void SelectChildren() {
  // Copy the list so it won't be corrupted with the new children we're going to add later
  DynArray< LWItem * > *orig_sel = new DynArray< LWItem * >;
  (*orig_sel) = fk->GetSelectedItems();

  // Go through each item in the selection list and add it's children
  for( unsigned int i=0; i < orig_sel->NumElements(); i++ )
    SelectItem( (*orig_sel)[i], true );

  // Free the temp list.
  delete orig_sel;
}

// DeselectChildren():
//  Deselects the children of the currently selected items.
//   Bones are considered children.
void DeselectChildren() {
  // Copy the list so it won't be corrupted with the new children we're going to add later
  DynArray< LWItem * > *orig_sel = new DynArray< LWItem * >;
  (*orig_sel) = fk->GetSelectedItems();

  // Go through each item in the selection list and add it's children
  for( unsigned int i=0; i < orig_sel->NumElements(); i++ )
    DeselectItem( (*orig_sel)[i], true );

  // Free the temp list.
  delete orig_sel;
}

// DrawSelectInfo():
//  Displays some information about selections
void DrawSelectInfo( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int cw = CON_HOTW( ctl );
  int ch = CON_HOTH( ctl );

  // Erase the control
  df->DrawRGBBox( fk_int->main_panel, 128, 128, 128, cx, cy, cw, ch );

  char buffer[128];

  // Write some useful text
  sprintf( buffer, "%d", fk->GetSelectedItems().NumElements() );
  df->WriteText( fk_int->main_panel, "Selected:", COLOR_BLACK, cx+10, cy+2 );
  df->WriteText( fk_int->main_panel, buffer,      COLOR_BLACK, cx+70, cy+2 );

  sprintf( buffer, "%d", fk->GetScene().GetBaseItem().CountItems( false, true ) );
  df->WriteText( fk_int->main_panel, "Total:",    COLOR_BLACK, cx+10, cy + 19 );
  df->WriteText( fk_int->main_panel, buffer,      COLOR_BLACK, cx+70, cy + 19 );
}

// UpdateSelectCount():
void FreeKeyInterface::UpdateSelectedCount() {
  SET_INT( fk_int->select_count, fk->GetSelectedItems().NumElements() );
  REDRAW_CON( fk_int->select_info_area );
}

// OnSelectionMode():
//  Called when the Selection Mode or Selection Toggle changes
void OnSelectionMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int sel_opt, sel_mode;
  GET_INT( fk_int->select_item_mode, sel_mode );
  GET_INT( fk_int->select_toggle, sel_opt );

  if( fk->GetToolOptionsTab() == TOOLTAB_ITEM_SELECTION ) {
    if( (sel_opt == 2) && (sel_mode > 0) )
      GHOST_CON( fk_int->select_children_toggle );
    else
      UNGHOST_CON( fk_int->select_children_toggle );
  }
}

// OnApplySelection():
//  Changes the selection based on the state of select_item_mode
void OnApplySelection( LWControl *ctl, FreeKeyInterface *panel ) {
  int value, children, compare, select;
  GET_INT( fk_int->select_item_mode,       value );
  GET_INT( fk_int->select_children_toggle, children );
  GET_INT( fk_int->select_name_mode,       compare );
  GET_INT( fk_int->select_toggle,          select );

  char buffer[1024];
  GET_STR( fk_int->select_item_name, buffer, 1024 );

  switch( value ) {
  case 0:             // Last Clicked
    if( fk->GetCurrentItem() != -1 ) {
      if( select == 0 )
        SelectItem( fk_int->sorted_items[ fk->GetCurrentItem() ], (bool)children );
      else if( select == 1 )
        DeselectItem( fk_int->sorted_items[ fk->GetCurrentItem() ], (bool)children );
      else
        ToggleItem( fk_int->sorted_items[ fk->GetCurrentItem() ], (bool)children, true );
    }
    break;

  case 1:             // All Selected
    if( select == 0 )
      SelectAllSelected( (bool)children );
    else if( select == 1 )
      DeselectAllSelected( (bool)children );
    else
      DeselectAllSelected( false );
    break;

  case 2:             // All Unselected
    if( select == 0 )
      SelectAllUnselected( (bool)children );
    else if( select == 1 )
      DeselectAllUnselected( (bool)children );
    else
      SelectAllUnselected( false );
    break;

  case 3:             // Select All Matching Name
    switch( compare ) {
    case 0:           //   Exact Match
      if( select == 0 )
        SelectItemByNameExact( buffer, select, (bool)children );
      else if( select == 1 )
        SelectItemByNameExact( buffer, select, (bool)children );
      else
        SelectItemByNameExact( buffer, select, false );

      break;

    case 1:           //   Starting With
      if( select == 0 )
        SelectItemByNameStart( buffer, select, (bool)children );
      else if( select == 1 )
        SelectItemByNameStart( buffer, select, (bool)children );
      else
        SelectItemByNameStart( buffer, select, false );
      break;

    case 2:           //   Ending With
      if( select == 0 )
        SelectItemByNameEnd( buffer, select, (bool)children );
      else if( select == 1 )
        SelectItemByNameEnd( buffer, select, (bool)children );
      else
        SelectItemByNameEnd( buffer, select, false );
      break;

    case 3:           //   Containing
      if( select == 0 )
        SelectItemByNameContain( buffer, select, (bool)children );
      else if( select == 1 )
        SelectItemByNameContain( buffer, select, (bool)children );
      else
        SelectItemByNameContain( buffer, select, false );
      break;
    }
    break;

  case 4:             // All In Group
    if( select == 0 )
      SelectAllInGroup( fk->GetCurrentGroup(), (bool)children );
    else if( select == 1 )
      DeselectAllInGroup( fk->GetCurrentGroup(), (bool)children );
    else
      InvertAllInGroup( fk->GetCurrentGroup(), false  );
    break;

  case 5:             // All
    if( select == 0 )
      SelectAll();
    else if( select == 1 )
      DeselectAll();
    else
      InvertSelection();
    break;
  }

  fk_int->UpdateSelectedCount();
  fk_int->RedrawItemList();
}
