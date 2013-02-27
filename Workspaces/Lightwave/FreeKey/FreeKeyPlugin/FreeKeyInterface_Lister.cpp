//
// FreeKeyInterface_Lister.cpp
//
// This contains all the Item List-specific functions and controls
//  for the FreeKey interface
//

#include <stdio.h>

#include "FreeKey-Interface.h"

const int min_divider_x = 100;

// OnFoldList():
//  When clicked, this will fold all of the items in the list
void OnFoldList( LWControl *ctl, FreeKeyInterface *panel ) {
  SetFoldState( &fk->GetScene().GetBaseItem(), true, true );
  fk->GetScene().GetBaseItem().SetFolded( false );       // The base shouldn't be affected

  OnDisplayMode( fk_int->display_item_mode, fk_int );
}

// OnUnFoldList():
//  When clicked, this will fold all of the items in the list
void OnUnFoldList( LWControl *ctl, FreeKeyInterface *panel ) {
  SetFoldState( &fk->GetScene().GetBaseItem(), false, true );
  fk->GetScene().GetBaseItem().SetFolded( false );       // The base shouldn't be affected

  OnDisplayMode( fk_int->display_item_mode, fk_int );
}

// SetFoldState():
//  Changes the fold state of an item and it's children
void SetFoldState( LWItem *item, bool state, bool children ) {
  item->SetFolded( state );

  // Add the children, if applicable
  if( children ) {
    for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
      SetFoldState( item->GetChildren()[i], state, true );

    if( item->HasSkeleton() )
      SetFoldState( &item->GetSkeleton(), state, true );
  }
}

// OnFirstFrame():
//  Handles changes to the first frame number.
void OnFirstFrame( LWControl *ctl, FreeKeyInterface *panel ) {
  double value;
  GET_FLOAT( fk_int->item_first_frame, value );
  value /= default_fps;

  if( value > fk->GetDisplayMax() - (5.0/default_fps)) {
    value = fk->GetDisplayMax() - (5.0/default_fps);
    SET_FLOAT( fk_int->item_first_frame, fk->GetDisplayMin() * default_fps );
  }

  fk->SetDisplayMin( value );
  fk_int->RedrawItemList();
}

// OnLastFrame():
//  Handles changes to the last frame number.
void OnLastFrame( LWControl *ctl, FreeKeyInterface *panel ) {
  double value;
  GET_FLOAT( fk_int->item_last_frame, value );
  value /= default_fps;

  if( value < fk->GetDisplayMin() + (5.0/default_fps) ) {
    value = fk->GetDisplayMin() + (5.0/default_fps);
    SET_FLOAT( fk_int->item_last_frame, fk->GetDisplayMax() * default_fps );
  }

  fk->SetDisplayMax( value );
  fk_int->RedrawItemList();
}

// OnItemRangePresets():
//  Handles changing the First and Last display values based on
//   options in the presets pop-up.
void OnItemRangePresets( LWControl *ctl, FreeKeyInterface *panel ) {
  int preset;
  GET_INT( ctl, preset );

  double low = 0, high = 0, temp_low, temp_high;

  switch( preset ) {
  case 0:                // Render First/Last Keyframes
    SET_FLOAT( fk_int->item_first_frame, fk->GetScene().GetFirstFrame() );
    SET_FLOAT( fk_int->item_last_frame,  fk->GetScene().GetLastFrame()  );
    break;
  case 1:                // Preview First/Last Keyframes
    SET_FLOAT( fk_int->item_first_frame, fk->GetScene().GetPreviewFirstFrame() );
    SET_FLOAT( fk_int->item_last_frame,  fk->GetScene().GetPreviewLastFrame()  );
    break;
  case 2:                // First and Last Keyframes in Scene
    if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 ) {
      DynArray< LWItem * > store;
      fk->GetScene().GetBaseItem().BuildItemList( store, true, false );

      low  = store[0]->GetMotion().FindEarliestKey();
      high = store[0]->GetMotion().FindLatestKey();

      for( unsigned int i=1; i < store.NumElements(); i++ ) {
        temp_low = store[i]->GetMotion().FindEarliestKey();
        if( low > temp_low )    low = temp_low;

        temp_high = store[i]->GetMotion().FindLatestKey();
        if( high < temp_high )  high = temp_high;
      }

      SET_FLOAT( fk_int->item_first_frame, LWScene::TimeToFrame( low  ) );
      SET_FLOAT( fk_int->item_last_frame,  LWScene::TimeToFrame( high ) );
    }
    break;
  case 3:                // First and Last Keyframes in Selection
    if( fk->GetSelectedItems().NumElements() > 0 ) {
      low  = fk->GetSelectedItems()[0]->GetMotion().FindEarliestKey();
      high = fk->GetSelectedItems()[0]->GetMotion().FindLatestKey();

      for( unsigned int i=1; i < fk->GetSelectedItems().NumElements(); i++ ) {
        temp_low = fk->GetSelectedItems()[i]->GetMotion().FindEarliestKey();
        if( low > temp_low )    low = temp_low;

        temp_high = fk->GetSelectedItems()[i]->GetMotion().FindLatestKey();
        if( high < temp_high )  high = temp_high;
      }

      SET_FLOAT( fk_int->item_first_frame, LWScene::TimeToFrame( low  ) );
      SET_FLOAT( fk_int->item_last_frame,  LWScene::TimeToFrame( high ) );
    }
    break;
  case 4:                // First and Last Keyframes in Current Item
    if( fk->IsCurrentItemValid() ) {
      SET_FLOAT( fk_int->item_first_frame, LWScene::TimeToFrame( fk_int->sorted_items[ fk->GetCurrentItem() ]->GetMotion().FindEarliestKey() ) );
      SET_FLOAT( fk_int->item_last_frame,  LWScene::TimeToFrame( fk_int->sorted_items[ fk->GetCurrentItem() ]->GetMotion().FindLatestKey()   ) );
    }
    break;
  case 5:                // First and Last Keyframes in Current Group
    if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 ) {
      DynArray< LWItem * > store;
      fk->GetScene().GetBaseItem().BuildGroupItemList( store, fk->GetCurrentGroup(), true, false );

      low  = store[0]->GetMotion().FindEarliestKey();
      high = store[0]->GetMotion().FindLatestKey();

      for( unsigned int i=1; i < store.NumElements(); i++ ) {
        temp_low = store[i]->GetMotion().FindEarliestKey();
        if( low > temp_low )    low = temp_low;

        temp_high = store[i]->GetMotion().FindLatestKey();
        if( high < temp_high )  high = temp_high;
      }

      SET_FLOAT( fk_int->item_first_frame, LWScene::TimeToFrame( low  ) );
      SET_FLOAT( fk_int->item_last_frame,  LWScene::TimeToFrame( high ) );
    }
    break;
  case 6:                // First and Last Keyframes in Listed Items
    if( fk_int->sorted_items.NumElements() > 0 ) {
      low  = fk_int->sorted_items[0]->GetMotion().FindEarliestKey();
      high = fk_int->sorted_items[0]->GetMotion().FindLatestKey();

      for( unsigned int i=1; i < fk_int->sorted_items.NumElements(); i++ ) {
        temp_low = fk_int->sorted_items[i]->GetMotion().FindEarliestKey();
        if( low > temp_low )    low = temp_low;

        temp_high = fk_int->sorted_items[i]->GetMotion().FindLatestKey();
        if( high < temp_high )  high = temp_high;
      }

      SET_FLOAT( fk_int->item_first_frame, LWScene::TimeToFrame( low  ) );
      SET_FLOAT( fk_int->item_last_frame,  LWScene::TimeToFrame( high ) );
    }
    break;
  }
}

// OnKeyFrameMode():
//  Handles changing the keyframe display mode of the item list
void OnKeyFrameMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int value;
  GET_INT( ctl, value );
  fk->SetKeyDisplayMode( value );

  fk_int->RedrawItemList();
}

// OnDisplayMode():
//  Handles changing the display mode of the item list
void OnDisplayMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int new_display_mode;
  GET_INT( ctl, new_display_mode );

  unsigned int i;

  if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 ) {
    switch( new_display_mode ) {
      case 0:    // Hierarchical
        fk_int->sorted_items.Reset();
        if( fk->GetScene().GetBaseItem().GetChildren().NumElements() > 0 )
          fk->GetScene().GetBaseItem().BuildFoldedItemList( fk_int->sorted_items, true, false );

        //out.open( "e:\\Tree.txt" );
        //if( out ) {
        //  fk->GetScene().GetBaseItem().OutputAsText( 0, out );
        //  out.close();
        //}

        break;

      case 1:    // Natural
        fk_int->sorted_items.Reset();
        fk->GetScene().GetBaseItem().BuildNaturalItemList( fk_int->sorted_items, true );
        break;

      case 2:    // Alphabetical
        fk_int->sorted_items.Reset();
        fk->GetScene().GetBaseItem().BuildNaturalItemList( fk_int->sorted_items, true );
        fk_int->sorted_items.QSort( SortLWItems_Alpha );
        break;

      case 3:    // Selected Only
        fk_int->sorted_items = fk->GetSelectedItems();
        break;

      case 4:    // Unselected Only
        fk_int->sorted_items.Reset();
        fk->GetScene().GetBaseItem().BuildSelectedItemList( fk_int->sorted_items, false, true, false );
        break;

      case 5:    // Grouped Only
        fk_int->sorted_items.Reset();
        for( i=0; i < fk->GetGroupList().NumElements(); i++ )
          fk->GetScene().GetBaseItem().BuildGroupItemList( fk_int->sorted_items, i, true, false );
        break;

      case 6:   // Ungrouped Only
        fk_int->sorted_items.Reset();
        fk->GetScene().GetBaseItem().BuildGroupItemList( fk_int->sorted_items, -1, true, false );
        break;

      case 7:   // Current Group Only
        fk_int->sorted_items.Reset();
        fk->GetScene().GetBaseItem().BuildGroupItemList( fk_int->sorted_items, fk->GetCurrentGroup(), true, false );
        break;
    }
  }

  int list_height = CON_H( fk_int->item_list ) - 20;
  list_height = (list_height/fk_int->dd.item_height);
  int max_top = fk_int->sorted_items.NumElements() - list_height;

  ival.intv.value = max_top;
  fk_int->item_scroll->set( fk_int->item_scroll, CTL_RANGEMAX, &ival );

  if( fk->GetItemDisplayMode() == new_display_mode ) {
    if( fk->GetTopItem() > max_top ) {
      if( max_top < 0 )
        max_top = 0;
      fk->SetTopItem( max_top );
    }

    SET_INT( fk_int->item_scroll, fk->GetTopItem() );
  } else {
    fk->SetTopItem( 0 );
    fk->SetCurrentItem( -1 );
    SET_INT( fk_int->item_scroll, fk->GetTopItem() );
    fk->SetItemDisplayMode( new_display_mode );
  }

  fk_int->RedrawItemList();
}

// DrawItemJumpToFrame():
//  Draws two right-facing arrows on the button
void DrawItemJumpToFrame( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_X( ctl );
  int cy = CON_Y( ctl );
  int ch = CON_H( ctl );
  int cw = CON_W( ctl );

  PanelDraw::DrawArrow( fk_int->main_panel, df, cx+(ch/2)+2, cy+(cw/2)-2,
                        PanelDraw::arrow_double_right, PanelDraw::arrow_solid );
}

// OnItemList():
//  Handles adjusting the item selections.
void OnItemList( LWControl *ctl, FreeKeyInterface *panel ) {
  if( fk_int->sorted_items.NumElements() == 0 )
    return;

  // Figure out which item was clicked
  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int ch = CON_HOTH( ctl );
  int cw = CON_HOTW( ctl );
  int mx = fk_int->mouse_down_x + fk_int->mouse_delta_x - cx;
  int my = fk_int->mouse_down_y + fk_int->mouse_delta_y - 20 - cy;
  int ch_mod = ch-20;

  if( my < 0 )   // User clicked on title; nothing to do (yet...)
    return;

  int index = (my / fk_int->dd.item_height) + fk->GetTopItem();

  // Make sure the index clicked points to an item
  if( index >= (int)fk_int->sorted_items.NumElements() )
    return;

  // Select the item.  Note this does NOT add it to
  //  the selection; it just makes it the current item.
  //  Mostly for internal purposes
  int old_index = fk->GetCurrentItem();
  fk->SetCurrentItem( index );

  //
  // Handle input on the item name/switch side of the interface
  //
  if( (mx < fk->GetDivider() ) && (fk_int->mouse_state == MSTATE_DOWN) ) {
    // User Clicked Fold/Unfold in Hierarchy mode; do NOT change the selection
    if( (mx < fk_int->dd.item_text_column + fk_int->dd.item_fold_column_width) && (mx > fk_int->dd.item_text_column) &&
        (fk->GetItemDisplayMode() == 0) ) {

      int skel_count = fk_int->sorted_items[ index ]->HasSkeleton() ? fk_int->sorted_items[ index ]->GetSkeleton().GetChildren().NumElements() : 0;
      if( (fk_int->sorted_items[ index ]->GetChildren().NumElements() > 0) ||
          (skel_count > 0) ) {
        fk_int->sorted_items[ index ]->SetFolded( !fk_int->sorted_items[ index ]->GetFolded() );

        OnDisplayMode( fk_int->display_item_mode, fk_int );
      }
    } else {    // Not folding the list; handle clicks as normal
      // Remove previously selected items, if needed
      if( !fk_int->shift_down && !fk_int->ctrl_down )         // Remove all selected if no qualifiers are held down
        DeselectAll();

      // Handle Qualifiers
      if( fk_int->ctrl_down ) {          // CTRL is down; toggle item's selected state
        ToggleItem( fk_int->sorted_items[index], false );
      } else if( fk_int->shift_down ) {  // Shift is down; Set all items between old_index and index to the new state of index
        if( index >= (int)fk_int->sorted_items.NumElements() )              // Index is out-of-range
          ToggleRange( old_index, fk_int->sorted_items.NumElements() - 1 );
        else if ( old_index >= (int)fk_int->sorted_items.NumElements() )    // Old index is out-of-range
          ToggleRange( fk_int->sorted_items.NumElements() - 1, index );
        else
          ToggleRange( old_index, index );
      } else {                           // No qualifiers; add this one item
        SelectItem( fk_int->sorted_items[index], false );
      }

      // Test for any Column Clicks
      if( mx < fk_int->dd.item_type_column ) {                                  // Group Number
        // Do something cool with groups here
      } else if( mx < fk_int->dd.item_text_column ) {                           // Item Draw Mode
        // Do something with the item draw icons here
      } else if( mx > fk->GetDivider() ) {                              // Post-Item Name Columns
        if( mx < fk->GetDivider() + fk_int->dd.item_visibility_offset ) {       //   Item Visibility (Draw Mode)
          // Do something with the item type icons here
        } else if( mx < fk->GetDivider() + fk_int->dd.item_lock_offset ) {      //   Item Lock
          // Do something with item locking here
        }
      }

      // Update some controls
      if( fk->IsCurrentItemValid() ) {
        SET_STR( fk_int->select_item_name, (char *)fk_int->sorted_items[ fk->GetCurrentItem() ]->GetName(),
                                           strlen( fk_int->sorted_items[ fk->GetCurrentItem() ]->GetName() ) );
      }

      // Rebuild the list, if needed
      switch( fk->GetItemDisplayMode() ) {
        case 3:    // Selected Only
          fk_int->sorted_items = fk->GetSelectedItems();
          break;

        case 4:    // Unselected Only
          fk_int->sorted_items.Reset();
          fk->GetScene().GetBaseItem().BuildSelectedItemList( fk_int->sorted_items, false, true, false );
          break;
      }

      if( fk->GetCurrentItem() > (int)fk_int->sorted_items.NumElements() )
        fk->SetCurrentItem( -1 );
    }

    fk_int->UpdateSelectedCount();
    fk_int->RedrawItemList();
  } else {
    if( (fk_int->mouse_down_x + fk_int->mouse_delta_x ) - cx  > fk->GetDivider() ) {
      // Keyframe-side clicks

      // Set up some common variables
      //  These should be the same as those used in the drawing routine
      LWMotion *motion = &fk_int->sorted_items[ index ]->GetMotion();

      // Determine what time was clicked
      const int    click_x    = mx - fk->GetDivider();                      // Position of the mouse relative to the left edge of the timeline
      const double click_time = fk_int->dd.start_time + ( fk_int->dd.zoomed_total_time * ( (double)click_x/(double)fk_int->dd.timeline_size ) );

      // Compute the amount of time each pixel covers
      const double pixel_time_coverage = fk_int->dd.zoomed_total_time/(double)fk_int->dd.timeline_size;

      if( fk_int->mouse_button == MBUTTON_LEFT ) {
        //
        // Handle Keyframe Clicks
        //
        if( fk->GetKeyDisplayMode() != 0 ) {
          unsigned long prev_index;
          if( fk_int->mouse_state == MSTATE_DOWN ) {          // This keeps the display from flickering while the user drags the mouse over a keyframe in selection mode
            // We find the nearest key on each channel within +/- 2 pixels of where the
            //  user clicked.  If the shift key is held down, then all the keys within
            //  +/- 2 pixels are selected.
            unsigned long index;
            for( unsigned int i=0; i < motion->GetChannelList().NumElements(); i++ ) {
               // Store the last selected key for later use in Range Selecting, if applicable
               if( fk_int->shift_down ) {
                 if( motion->GetChannel(i).GetSelected().NumElements() == 0 )
                   prev_index = 0;
                 else
                   prev_index = motion->GetChannel(i).GetKeys().FindIndexOf( motion->GetChannel(i).GetSelected()[ motion->GetChannel(i).GetSelected().NumElements()-1 ] );
               }

              // Unselect all currently selected keys in the ,pyopm if no modifiers are held down
              if( !fk_int->shift_down && !fk_int->ctrl_down )
                  motion->GetChannel(i).GetSelected().Reset();

              // Find the nearest keyframe in the motion
              index = motion->GetChannel(i).FindKeyFrameIndex( click_time, LWCHANFIND_NEAREST );
              if( index != LWCHANFIND_NO_MATCH ) {
                if( (motion->GetChannel(i).GetKeys()[ index ]->GetTime() <= click_time + (pixel_time_coverage * 2.0)) &&
                    (motion->GetChannel(i).GetKeys()[ index ]->GetTime() >= click_time - (pixel_time_coverage * 3.0)) ) {
                  if( DA_NO_MATCH == motion->GetChannel(i).GetSelected().FindIndexOf( motion->GetChannel(i).GetKeys()[ index ] ) )     // Ensure the key isn't already selected
                    motion->GetChannel(i).GetSelected().Add( motion->GetChannel(i).GetKeys()[ index ] );
  
                  // Multi-select (get ALL the keys within the click range)
                  if( fk_int->ctrl_down || fk_int->shift_down ) {
                    // Test keys after this one to see if they are close enough to be selected
                    for( unsigned long k=index+1; k < motion->GetChannel(i).GetKeys().NumElements(); k++ ) {
                      if( motion->GetChannel(i).GetKeys()[ k ]->GetTime() <= click_time + (pixel_time_coverage * 2.0) ) {
                        if( DA_NO_MATCH == motion->GetChannel(i).GetSelected().FindIndexOf( motion->GetChannel(i).GetKeys()[ k ] ) )     // Ensure the key isn't already selected
                          motion->GetChannel(i).GetSelected().Add( motion->GetChannel(i).GetKeys()[ k ] );
                      } else
                        break;
                    }

                    // Test keys before this one to see if they are close enough to be selected
                    for( k=index-1; k != -1; k-- ) {   // k is unsigned so we have to test for -1 (testing for <= 0 wouldn't ever return)
                      if( motion->GetChannel(i).GetKeys()[ k ]->GetTime() >= click_time - (pixel_time_coverage * 3.0) ) {
                        if( DA_NO_MATCH == motion->GetChannel(i).GetSelected().FindIndexOf( motion->GetChannel(i).GetKeys()[ k ] ) )     // Ensure the key isn't already selected
                          motion->GetChannel(i).GetSelected().Add( motion->GetChannel(i).GetKeys()[ k ] );
                      } else
                        break;
                    }
                  }

                  // Range Select within single motions
                  if( fk_int->shift_down ) {
                    if( motion->GetChannel(i).GetSelected().NumElements() > 0 ) {
                      assert( prev_index != DA_NO_MATCH );     // The keyframe MUST be in the selection list
                      unsigned long low  = index < prev_index ? index : prev_index;
                      unsigned long high = index > prev_index ? index : prev_index;

                      // Add all keys within range to the selection
                      for( unsigned long k=low; k <= high; k++ ) {
                        if( DA_NO_MATCH == motion->GetChannel(i).GetSelected().FindIndexOf( motion->GetChannel(i).GetKeys()[ k ] ) )     // Ensure the key isn't already selected
                          motion->GetChannel(i).GetSelected().Add( motion->GetChannel(i).GetKeys()[ k ] );
                      }
                    }
                  }
                }
              }
            }
          }
          fk_int->RedrawItemList();
        } else if( fk->GetKeyDisplayMode() != 1 ) {
          //
          // Handle Extent Clicks
          //

        }
      } else if( fk_int->mouse_button == MBUTTON_RIGHT ) {
        //
        // Handle Bounding Box Selection of Keyframes
        //

        if( fk_int->mouse_state == MSTATE_DOWN ) {
          // Button Clicked; initialize the bounding box;
          fk_int->bbox_item_base = fk->GetCurrentItem();
          fk_int->bbox_t1 = click_time;
          fk_int->bbox_y1 = fk_int->dd.item_height % my;
          fk_int->bbox_t2 = fk_int->bbox_t1;
          fk_int->bbox_y2 = my;
          fk_int->draw_bbox = true;

/*
          char buffer[ 256 ];
          sprintf( buffer, "Mouse Clicked:   base: %d    t1/y1: %g/%d    t2/y2: %g/%d", fk_int->bbox_item_base, fk_int->bbox_t1, fk_int->bbox_y1, fk_int->bbox_t2, fk_int->bbox_y2 );
          SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );
*/
        } else if( fk_int->mouse_state == MSTATE_UP ) {
          // Button Released; clear the bounding box state
          fk_int->draw_bbox     = false;
          fk_int->now_scrolling = false;

          // Figure out what items to process (handles bbox being in an item, but not around it's keys
          unsigned int start_item = fk_int->bbox_item_base;
          unsigned int end_item   = (fk_int->bbox_y2 / fk_int->dd.item_height) + fk->GetTopItem();
          unsigned int final_start_item = start_item;
          unsigned int final_end_item   = end_item;

          if( (start_item <= fk_int->sorted_items.NumElements() ) || 
              (end_item <= fk_int->sorted_items.NumElements()) ) {

            if( start_item > fk_int->sorted_items.NumElements() )
              final_start_item = fk_int->sorted_items.NumElements();
            else if( (start_item < end_item) && (fk_int->bbox_y1 % fk_int->dd.item_height >= fk_int->dd.item_height/2 ) )
              final_start_item = start_item + 1;
            else if( (start_item > end_item) && (fk_int->bbox_y1 % fk_int->dd.item_height <= fk_int->dd.item_height/2 ) )
              final_start_item = start_item - 1;

            if( end_item > fk_int->sorted_items.NumElements() )
              final_end_item = fk_int->sorted_items.NumElements();
            else if( (start_item > end_item) && (fk_int->bbox_y2 % fk_int->dd.item_height >= fk_int->dd.item_height/2 ) )
              final_end_item = end_item + 1;
            else if( (start_item < end_item) && (fk_int->bbox_y2 % fk_int->dd.item_height <= fk_int->dd.item_height/2 ) )
              final_end_item = end_item - 1;

            // Ensure that start is after end
            if( start_item > end_item ) {             // We use these instead of final to ensure that we actually have items in the bbox below
              unsigned int temp = final_start_item;
              final_start_item  = final_end_item;
              final_end_item    = temp;
            };

            double start_time, end_time;
            if( fk_int->bbox_t1 < fk_int->bbox_t2 ) {
              start_time = fk_int->bbox_t1;
              end_time   = fk_int->bbox_t2;
            } else {
              start_time = fk_int->bbox_t2;
              end_time   = fk_int->bbox_t1;
            }

            // See if there are any items within the Bounding Box to select keys from
            if( final_end_item >= final_start_item ) {
              LWMotion *motion;
              for( unsigned int i=final_start_item; i <= final_end_item; i++ ) {
                motion = &fk_int->sorted_items[i]->GetMotion();

                // Make sure there are keys in the bbox for this item
                for( unsigned long j=0; j < motion->GetChannelList().NumElements(); j++ ) {
                  // Deselect all keys if shift or control aren't held down
                  if( !fk_int->shift_down && !fk_int->ctrl_down )
                    motion->GetChannel(j).GetSelected().Reset();

                  if( motion->GetChannel(j).GetKeys().NumElements() > 0 ) {
                    unsigned long low  = motion->GetChannel(j).FindKeyFrameIndex( start_time, LWCHANFIND_ROUND_UP );
                    unsigned long high = motion->GetChannel(j).FindKeyFrameIndex( end_time, LWCHANFIND_ROUND_DOWN );

                    // Select the keys within the bounding box
                    if( (low != LWCHANFIND_NO_MATCH) && (high != LWCHANFIND_NO_MATCH) ) {
                      // Add all keys within range to the selection
                      for( unsigned long k=low; k <= high; k++ ) {
                        if( DA_NO_MATCH == motion->GetChannel(j).GetSelected().FindIndexOf( motion->GetChannel(j).GetKeys()[ k ] ) )     // Ensure the key isn't already selected
                          motion->GetChannel(j).GetSelected().Add( motion->GetChannel(j).GetKeys()[ k ] );
                      }
                    }
                  }
                }
              }
            }
          }

          fk_int->RedrawItemList();

/*
          char buffer[ 256 ];
          sprintf( buffer, "Mouse Released:   base: %d    t1/y1: %g/%d    t2/y2: %g/%d", fk_int->bbox_item_base, fk_int->bbox_t1, fk_int->bbox_y1, fk_int->bbox_t2, fk_int->bbox_y2 );
          SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );
*/
        } else if( fk_int->mouse_state == MSTATE_MOVING ) {
          // Mouse Moved; update the bounding box area
          fk_int->bbox_t2 = click_time;
          fk_int->bbox_y2 = my;
          fk_int->RedrawItemList();

          // See if we should scroll the list
          if( (my < 5) || (my > ch_mod-5) ) {
            if( !fk_int->now_scrolling ) {
              fk_int->now_scrolling = true;
              #ifdef WIN32
                timeSetEvent( 200, 100, ScrollItemListVert, NULL, TIME_ONESHOT );
              #endif
            }

/*
            char buffer[ 256 ];
            sprintf( buffer, "Mouse Moved (Scrolling):   base: %d    t1/y1: %g/%d    t2/y2: %g/%d", fk_int->bbox_item_base, fk_int->bbox_t1, fk_int->bbox_y1, fk_int->bbox_t2, fk_int->bbox_y2 );
            SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );
*/
          } else {
            fk_int->now_scrolling = false;

/*
            char buffer[ 256 ];
            sprintf( buffer, "Mouse Moved:   base: %d    t1/y1: %g/%d    t2/y2: %g/%d", ch, my, fk_int->bbox_item_base, fk_int->bbox_t1, fk_int->bbox_y1, fk_int->bbox_t2, fk_int->bbox_y2 );
            SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );
*/
          }
        }
      }
    }
  }

  if( fk_int->mouse_state == MSTATE_DOWN )
    REDRAW_CON( fk_int->key_select_info_area );
}

#ifdef WIN32
// ScrollItemListVert():
//  Called by a timer event to scroll the item list vertically.
void CALLBACK ScrollItemListVert( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) {
  int cy = CON_HOTY( fk_int->item_list );
  int ch = CON_HOTH( fk_int->item_list );
  int my = fk_int->mouse_down_y + fk_int->mouse_delta_y - 20 - cy;
  int ch_mod = CON_HOTH( fk_int->item_list ) - 20;

  if( !fk_int->now_scrolling )
    return;
  
  if( my < 5 ) {
    // Scroll Up
    if( fk->GetTopItem() > 0 ) {
      fk->SetTopItem( fk->GetTopItem() - 1 );
      timeSetEvent( 100, 100, ScrollItemListVert, NULL, TIME_ONESHOT );
      fk_int->RedrawItemList();
    }
  } else if( my > ch_mod-5 ) {
    // Scroll Down
    if( fk->GetTopItem() < (int)fk_int->sorted_items.NumElements() - 1 ) {
      fk->SetTopItem( fk->GetTopItem() + 1 );
      timeSetEvent( 100, 100, ScrollItemListVert, NULL, TIME_ONESHOT );
      fk_int->RedrawItemList();
    }
  } else {
    // Stop Scrolling
    fk_int->now_scrolling = false;
  }
}
#endif

// OnItemScroll():
//  Handles adjusting the visible items via the scroll bar.
void OnItemScroll( LWControl *ctl, FreeKeyInterface *panel ) {
  int value;
  GET_INT( fk_int->item_scroll, value );
  fk->SetTopItem( value );

  fk_int->RedrawItemList();
}

// UpdateItemListType():
//  Called to change which item list should be visible, and thus drawn.
void FreeKeyInterface::UpdateItemListType( bool use_opengl_item_list ) {
  if( use_opengl_item_list ) {
    UNGHOST_CON( opengl_item_list );
    ERASE_CON( item_list );
  } else {
    UNGHOST_CON( item_list );
    ERASE_CON( opengl_item_list );

    fk_int->dd.opengl_font.Flush();
  }
}

// RedrawItemList():
//  Redraws the item list (both OpenGL and Panels).
void FreeKeyInterface::RedrawItemList() {
  REDRAW_CON( item_list );    
  REDRAW_CON( opengl_item_list );
}


// DrawItemListbox():
//  Called to draw the LWPanels version of the item list
//  This verion of the routine supports scrolling the item name list.
//   This means it's a hack.  The list is drawn in 2 passes:  First,
//   the item names are displayed, positioned to match the scroll bar.
//   Since this tends to make parts of the name overwrite the interface,
//   the overflow is erased and the corrupted parts of the UI are redrawn.
//   The second pass draws all the controls, plus the second set of guide
//   lines.
void DrawItemListbox( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;
  fk_int->dd.Generate( ctl, false, fk->GetDrawStyle() );

  // Clear the control
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.bgr, fk_int->dd.bgb, fk_int->dd.bgg, fk_int->dd.cx, fk_int->dd.cy, fk_int->dd.cw, fk_int->dd.ch );

  //
  // Column Title Drawing
  //
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.r, fk_int->dd.g, fk_int->dd.b, fk_int->dd.cx, fk_int->dd.truey, fk_int->dd.cw, 19 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, fk_int->dd.cx, fk_int->dd.truey+19, fk_int->dd.cw,  1 );

  df->WriteText( fk_int->main_panel, "Item", COLOR_BLACK, fk_int->dd.cx+fk_int->dd.item_text_column + 14, fk_int->dd.truey + 5 );

  PanelDraw::DrawLWIcon( fk_int->main_panel, df, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, fk_int->dd.truey+6,PanelDraw::visibility_eye, PanelDraw::black );
  PanelDraw::DrawLWIcon( fk_int->main_panel, df, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_lock_offset + 2, fk_int->dd.truey+6, PanelDraw::lock, PanelDraw::black );

  // Draw the ticks
  int x = 0;
  for( double d = (float)((int)fk_int->dd.start_frame); x < fk_int->dd.timeline_size; d += fk_int->dd.frame_step ) {
    x = (int)( ((d/fk_int->dd.frame_rate - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    if( x < fk_int->dd.timeline_size )
      df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, fk_int->dd.start_edge + x, fk_int->dd.truey+16, 1, 3 );
  }

  char buffer[10];
  int last_label_x = 0;
  d = 0.0;

  // Write some frame numbers
  for( int i = (int)fk_int->dd.start_frame - fk_int->dd.frame_step_offset; d < fk_int->dd.start_edge + fk_int->dd.timeline_size; i+=fk_int->dd.frame_num_step ) {
    d = fk_int->dd.start_edge + ((double)i - fk_int->dd.start_frame) * fk_int->dd.frame_size;
    if( d > fk_int->dd.start_edge + fk_int->dd.timeline_size - 1)
      break;

    if( d > fk_int->dd.start_edge ) {
      // Full Mark (at frame number labels)
      df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, (int)d, fk_int->dd.truey+12, 1, 7 );

      // Half Mark (between frame number labels)
      if( fk_int->dd.frame_num_step >= 10 ) {               // Only draw if >=10 frames are displayed (we don't want half-frame ticks; it's confusing to the user)
        int half_d = (int)( d - (((double)fk_int->dd.frame_num_step * fk_int->dd.frame_size)/2.0) );
        if( half_d > fk_int->dd.start_edge )
          df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, half_d, fk_int->dd.truey+14, 1, 5 );
      }

      // Draw text, but only if there's room
      if( (d - last_label_x > fk_int->dd.label_width)      &&         // Keep enough space between each label
          (d < fk_int->dd.start_edge + fk_int->dd.timeline_size - 25) &&         // Don't draw too close to the right edge...
          (d > fk_int->dd.start_edge + 10 ) ) {                          // ...or left edge
        last_label_x = (int)d;
        sprintf( buffer, "%d", i );
        df->WriteText( fk_int->main_panel, buffer, COLOR_BLACK, (int)(d - (df->TextWidth( fk_int->main_panel, buffer )/2 )), fk_int->dd.truey-1 );
      }
    }
  }

  //
  // Item Name Drawing
  //

  // Rows
  for( int j=1; j < fk_int->dd.ch/fk_int->dd.item_height; j++ ) {
    df->DrawRGBBox( fk_int->main_panel, 112, 112, 112,
                    fk_int->dd.cx, fk_int->dd.cy+(j*fk_int->dd.item_height), fk->GetDivider(), 1 );
  }

  // Items in the list
  int item_text_color;
  if( fk_int->sorted_items.NumElements() == 0 ) {  // Check for no items in list
    df->WriteText( fk_int->main_panel, "(none)", fk_int->dd.item_color_name, fk_int->dd.cx+fk_int->dd.item_text_column + 4, fk_int->dd.cy );
  } else {
    int depth_offset;                              // Amount to shift current item inward in Hierarchical mode
    int indent;                                    // Total amount ot indent the name
    int baseline;                                  // Base of the current line

    // Draw Group BG
    //  We do this first so that we don't chop of the bottom of the letters from the prev. line
    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      if( (fk->GetDrawStyle() == DRAWSTYLE_L6) && fk_int->sorted_items[i]->GetSelected() ) {
        // Item is selected
        df->DrawRGBBox( fk_int->main_panel, 160, 160, 160,
                        fk_int->dd.cx+fk_int->dd.item_text_column+1, baseline+1, fk->GetDivider() - fk_int->dd.item_text_column + fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
      } else {
        if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
          if( fk_int->sorted_items[i]->GetGroup() == fk->GetCurrentGroup() ) {
            // Item belongs to current group
            df->DrawRGBBox( fk_int->main_panel, fk_int->dd.group_cur_r, fk_int->dd.group_cur_g, fk_int->dd.group_cur_b,
                        fk_int->dd.cx+fk_int->dd.item_text_column+1, baseline+1, fk->GetDivider() - fk_int->dd.item_text_column + fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
          } else {
            // Item belongs to another group
            df->DrawRGBBox( fk_int->main_panel, fk_int->dd.group_not_r, fk_int->dd.group_not_g, fk_int->dd.group_not_b,
                        fk_int->dd.cx+fk_int->dd.item_text_column+1, baseline+1, fk->GetDivider() - fk_int->dd.item_text_column + fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
          }
        }
      }
      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }

    // Draw Item Names
    for( i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      depth_offset = 0;
      indent       = 0;
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );

      if( fk->GetItemDisplayMode() == 0 ) {        // Hierarchical Mode Only (indent based on hierarchy depth)
        depth_offset = fk_int->dd.item_depth_offset * fk_int->sorted_items[i]->GetDepth();
        indent = fk_int->dd.item_fold_column_width;
      }

      if( fk->GetItemDisplayMode() == 1 ) {        // Natural Mode Only (indent the bones)
        if( fk_int->sorted_items[i]->GetItemType() == LWI_ITEM_BONE )
          depth_offset = fk_int->dd.item_depth_offset;
      }

      // Figure out what color the text should be drawn in
      if( fk->GetDrawStyle() == DRAWSTYLE_L6 )
        item_text_color = fk_int->dd.item_color_name;
      else
        item_text_color = fk_int->sorted_items[i]->GetSelected() ? COLOR_LT_YELLOW : fk_int->dd.item_color_name;

      static char name_buffer[1024];
      strcpy( name_buffer, (char *)fk_int->sorted_items[i]->GetName() );
      indent += fk_int->dd.cx + fk_int->dd.item_text_column + depth_offset + 4;

      // Add the item's clone index, if applicable
      if( fk_int->sorted_items[i]->GetCloneNumber() > 0 )
        sprintf( name_buffer, "%s (%d)", name_buffer, fk_int->sorted_items[i]->GetCloneNumber() );

      // Display the item name
      df->WriteText( fk_int->main_panel, name_buffer,
                     item_text_color, indent - fk->GetNameScroll(), baseline );

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  //
  // Erasing (First Pass)
  //

  // Erase the parts of the names that are out of range
  if( (fk->GetItemDisplayMode() == 0)  &&
      (fk_int->sorted_items.NumElements() > 0) )
    df->DrawRGBBox( fk_int->main_panel, fk_int->dd.bgr, fk_int->dd.bgb, fk_int->dd.bgg, fk_int->dd.cx, fk_int->dd.cy, fk_int->dd.item_fold_column_end, fk_int->dd.ch );
  else
    df->DrawRGBBox( fk_int->main_panel, fk_int->dd.bgr, fk_int->dd.bgb, fk_int->dd.bgg, fk_int->dd.cx, fk_int->dd.cy, fk_int->dd.item_text_column, fk_int->dd.ch );

  df->DrawRGBBox( fk_int->main_panel, 144, 144, 144, 0, fk_int->dd.cy, fk_int->dd.cx, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.bgr, fk_int->dd.bgb, fk_int->dd.bgg, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_midcol_left_offset, fk_int->dd.cy, fk_int->dd.cw-fk->GetDivider() - fk_int->dd.item_midcol_left_offset, fk_int->dd.ch );

  // Draw the keyframe columns
  d = 0;
  for( i = (int)fk_int->dd.start_frame - fk_int->dd.frame_step_offset; d < fk_int->dd.start_edge + fk_int->dd.timeline_size; i+=fk_int->dd.frame_num_step ) {
    d = fk_int->dd.start_edge + ((double)i - fk_int->dd.start_frame) * fk_int->dd.frame_size;
    if( d > fk_int->dd.start_edge + fk_int->dd.timeline_size - 1)
      break;

    if( d > fk_int->dd.start_edge ) {
      // Full Mark (at frame number labels)
      df->DrawRGBBox( fk_int->main_panel, 112, 112, 112, (int)d, fk_int->dd.cy, 1, fk_int->dd.ch );

      // Half Mark (between frame number labels)
      if( fk_int->dd.frame_num_step >= 10 ) {               // Only draw if >=10 frames are displayed (we don't want half-frame ticks; it's confusing to the user)
        int half_d = (int)( d - (((double)fk_int->dd.frame_num_step * fk_int->dd.frame_size)/2.0) );
        if( half_d > fk_int->dd.start_edge )
          df->DrawRGBBox( fk_int->main_panel, 124, 124, 124, half_d, fk_int->dd.cy, 1, fk_int->dd.ch );
      }
    }
  }


  //
  // Keyframe Drawing
  //
  double extent_start_time;
  double extent_end_time;
  int    extent_start_edge;
  int    extent_width;

  // Draw Group BG
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line

    int rx = fk_int->dd.item_text_column - 1;
    if( fk->GetItemDisplayMode() == 0 ) 
      rx += fk_int->dd.item_fold_column_width;

    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      if( (fk->GetDrawStyle() == DRAWSTYLE_L6) && fk_int->sorted_items[i]->GetSelected() ) {
        // Item is selected
        df->DrawRGBBox( fk_int->main_panel, 160, 160, 160,
                            fk_int->dd.cx + fk->GetDivider(), baseline+1, fk_int->dd.cw-fk->GetDivider(), fk_int->dd.item_height-1 );
      } else {
        if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
          if( fk_int->sorted_items[i]->GetGroup() == fk->GetCurrentGroup() ) {
            // Item belongs to current group
            df->DrawRGBBox( fk_int->main_panel, 132, 132, 116,
                            fk_int->dd.cx + fk->GetDivider(), baseline+1, fk_int->dd.cw-fk->GetDivider(), fk_int->dd.item_height-1 );
          } else {
            // Item belongs to another group
            df->DrawRGBBox( fk_int->main_panel, 116, 116, 132,
                            fk_int->dd.cx + fk->GetDivider(), baseline+1, fk_int->dd.cw-fk->GetDivider(), fk_int->dd.item_height-1 );
          }
        }
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  // Draw the Keyframe Rows
  for( j=1; j < fk_int->dd.ch/fk_int->dd.item_height; j++ ) {
    df->DrawRGBBox( fk_int->main_panel, 112, 112, 112,
                    fk_int->dd.cx+fk->GetDivider()+1 + fk_int->dd.item_midcol_left_offset, fk_int->dd.cy+(j*fk_int->dd.item_height), fk_int->dd.cw-fk->GetDivider() - fk_int->dd.item_midcol_left_offset - 1, 1 );
  }

  // Draw the First Frame/Last Frame/Now markers
  if( (fk->GetScene().GetStartTime() > fk_int->dd.start_time) && (fk->GetScene().GetStartTime() < fk_int->dd.end_time) ) {
    x = (int)( ((fk->GetScene().GetStartTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    df->DrawRGBBox( fk_int->main_panel, 0, 172, 0, x + fk_int->dd.start_edge, fk_int->dd.cy-6, 1, fk_int->dd.ch+6 );
  }

  if( (fk->GetScene().GetEndTime() > fk_int->dd.start_time) && (fk->GetScene().GetEndTime() < fk_int->dd.end_time) ) {
    x = (int)( ((fk->GetScene().GetEndTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    df->DrawRGBBox( fk_int->main_panel, 172, 0, 0, x + fk_int->dd.start_edge, fk_int->dd.cy-6, 1, fk_int->dd.ch+6 );
  }

  // Draw the Bounding Box Selection, if applicable
  if( fk_int->draw_bbox ) {
    if( (fk_int->bbox_t1 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Left Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider() + fk_int->dd.bbox_x,
                      fk_int->dd.cy + fk_int->dd.bbox_y, 1, fk_int->dd.bbox_h );
    }

    if( (fk_int->bbox_t2 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Right Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider() + fk_int->dd.bbox_x + fk_int->dd.bbox_w,
                      fk_int->dd.cy + fk_int->dd.bbox_y, 1, fk_int->dd.bbox_h );
    }

    if( fk_int->dd.draw_horiz_top ) {                                                    // Top
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.bbox_x,
                      fk_int->dd.cy + fk_int->dd.bbox_y, fk_int->dd.bbox_w, 1 );
    }

    if( fk_int->dd.draw_horiz_bottom ) {                                                 // Bottom
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+fk_int->dd.bbox_x,
                      fk_int->dd.cy+ fk_int->dd.bbox_y+ fk_int->dd.bbox_h, fk_int->dd.bbox_w, 1 );
    }
  }

  // Draw the Keyframe Data
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line
    int timeline_width = fk_int->dd.cw - fk->GetDivider();
    LWMotion *motion;

    for( i=fk->GetTopItem(); i < (long)fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      motion = &fk_int->sorted_items[i]->GetMotion();

      extent_start_time = motion->FindEarliestKey();
      extent_end_time   = motion->FindLatestKey();
      int total_keys    = motion->FindMaxKeys();

      if( total_keys > 0 ) {
        // Only try drawing if the keys are in the visible part of the list
        if( (extent_start_time <= fk_int->dd.end_time) && (extent_end_time >= fk_int->dd.start_time) ) {
          if( extent_start_time <= fk_int->dd.start_time )
            extent_start_edge = -2;
          else
            extent_start_edge = (int)( ((extent_start_time - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size - 1.0 );

          if( total_keys == 1 ) 
            extent_width =  (int)( fk_int->dd.frame_size / 8 );
          else if( extent_end_time > fk_int->dd.end_time )
            extent_width = fk_int->dd.timeline_size - extent_start_edge + 2;
          else {
            extent_width = (int)( ((extent_end_time - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size ) - extent_start_edge + 2;
            if( extent_start_edge < 0 )
              extent_width -= extent_start_edge;
          }

          if( extent_width < 3 )
            extent_width = 3;

          // Make pastely versions of the item color
          IMG_BYTE item_color[3], hsv[3];
          PanelDraw::GetLWColor( item_color, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor() );
          ImageClass::RGB2HSV( item_color, hsv );
          hsv[1] /= 2;
          ImageClass::HSV2RGB( hsv, item_color );

          // Keyframe Extents
          if( fk->GetKeyDisplayMode() != 1 ) {
              DrawExtent( fk_int->main_panel, df,
                          fk_int->dd.start_edge + extent_start_edge, baseline+1, extent_width, fk_int->dd.item_height - 2,
                          fk_int->dd.start_edge, fk_int->dd.timeline_size, 1, 1,
                          (int)(item_color[0] * 0.90), (int)(item_color[1] * 0.90), (int)(item_color[2] * 0.90),
                          (int)(item_color[0] * 0.70), (int)(item_color[1] * 0.70), (int)(item_color[2] * 0.70),
                          (int)(item_color[0] * 0.80), (int)(item_color[1] * 0.80), (int)(item_color[2] * 0.80) );
          }

          // Keyframes
          if( fk->GetKeyDisplayMode() != 0 ) {
            LWKeyFrame * key = NULL;
            double       old_time = fk_int->dd.start_time;
            int          key_x;

            // Draw a line from start to end
            df->DrawRGBBox( fk_int->main_panel,
                            item_color[0], item_color[1], item_color[2],
                            fk_int->dd.start_edge + extent_start_edge, baseline+7, extent_width, 1 );

            // Draw the keyframe marks
            if( !fk_int->quickdraw_item_list ) {
              do {
                key = motion->FindNextKey( old_time + .001 );  // that's 1/1000th of a second, or .03 frames.  Should be small enough...
                if( key == NULL )
                  break;
                else if( key->GetTime() > fk_int->dd.end_time )
                  break;
                else {
                  old_time = key->GetTime();
                  key_x = (int)( ((key->GetTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );

                  // See if a keyframe on any channel has been selected
                  bool is_sel = false;
                  for( unsigned int m = 0; m < motion->GetChannelList().NumElements(); m++ ) {
                    for( unsigned int n=0; n < motion->GetChannel(m).GetSelected().NumElements(); n++ ) {
                      if( motion->GetChannel(m).GetSelected()[n]->GetTime() == key->GetTime() ) {
                        is_sel = true;
                        break;
                      }
                    }

                    if( is_sel )
                      break;
                  }
                  DrawKeyframeMark( fk_int->main_panel, df, fk_int->dd.start_edge + key_x - 2, baseline + 5, is_sel );
                }
              } while( key != NULL );
            }
          }
        }
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  // Draw the Bounding Box Selection again (this time over the keys), if applicable
  if( fk_int->draw_bbox ) {
    if( (fk_int->bbox_t1 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Left Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider() + fk_int->dd.bbox_x,
                      fk_int->dd.cy + fk_int->dd.bbox_y, 1, fk_int->dd.bbox_h );
    }

    if( (fk_int->bbox_t2 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Right Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider() + fk_int->dd.bbox_x + fk_int->dd.bbox_w,
                      fk_int->dd.cy + fk_int->dd.bbox_y, 1, fk_int->dd.bbox_h );
    }

    if( fk_int->dd.draw_horiz_top ) {                                                    // Top
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.bbox_x,
                      fk_int->dd.cy + fk_int->dd.bbox_y, fk_int->dd.bbox_w, 1 );
    }

    if( fk_int->dd.draw_horiz_bottom ) {                                                 // Bottom
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+fk_int->dd.bbox_x,
                      fk_int->dd.cy+ fk_int->dd.bbox_y+ fk_int->dd.bbox_h, fk_int->dd.bbox_w, 1 );
    }
  }

  //
  // Erasing/Column Drawing (Second Pass)
  //
  df->DrawRGBBox( fk_int->main_panel, 144, 144, 144, fk_int->dd.cx + fk_int->dd.cw, fk_int->dd.cy,
                  PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->main_panel ) - (fk_int->dd.cx+fk_int->dd.cw), fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.bgr, fk_int->dd.bgb, fk_int->dd.bgg, fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_midcol_left_offset, fk_int->dd.cy, -fk_int->dd.item_midcol_left_offset, fk_int->dd.ch );

  // Fill back in damaged areas
  df->DrawRGBBox( fk_int->main_panel,  80,  80,  80, fk_int->dd.cx-1,     fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, 200, 200, 200, fk_int->dd.cx+fk_int->dd.cw, fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, 200, 200, 200, 0,           fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel,   0,   0,   0, PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->main_panel ),     fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel,  80,  80,  80, PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->main_panel ) - 1, fk_int->dd.cy, 1, fk_int->dd.ch );

  REDRAW_CON( fk_int->item_scroll );
  DrawToolsPane( true );

  for( j=1; j < fk_int->dd.ch/fk_int->dd.item_height; j++ ) {
    // Icon Rows
    if( (fk->GetItemDisplayMode() == 0)  &&
        (fk_int->sorted_items.NumElements() > 0) )
      df->DrawRGBBox( fk_int->main_panel, 112, 112, 112, fk_int->dd.cx+1, fk_int->dd.cy+(j*fk_int->dd.item_height), fk_int->dd.item_fold_column_end, 1 );
    else
      df->DrawRGBBox( fk_int->main_panel, 112, 112, 112, fk_int->dd.cx+1, fk_int->dd.cy+(j*fk_int->dd.item_height), fk_int->dd.item_text_column, 1 );
  }

  // Draw Group BG
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line

    int rx = fk_int->dd.item_text_column - 1;
    if( fk->GetItemDisplayMode() == 0 ) 
      rx += fk_int->dd.item_fold_column_width;

    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      if( (fk->GetDrawStyle() == DRAWSTYLE_L6) && fk_int->sorted_items[i]->GetSelected() ) {
        // Item is selected
        df->DrawRGBBox( fk_int->main_panel, 160, 160, 160,
                            fk_int->dd.cx+1, baseline+1, rx, fk_int->dd.item_height-1 );
        df->DrawRGBBox( fk_int->main_panel, 160, 160, 160,
                            fk_int->dd.cx + fk->GetDivider()+fk_int->dd.item_visibility_offset, baseline+1, -fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
      } else {
        if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
          if( fk_int->sorted_items[i]->GetGroup() == fk->GetCurrentGroup() ) {
            // Item belongs to current group
            df->DrawRGBBox( fk_int->main_panel, 132, 132, 116,
                            fk_int->dd.cx+1, baseline+1, rx, fk_int->dd.item_height-1 );
            df->DrawRGBBox( fk_int->main_panel, 132, 132, 116,
                            fk_int->dd.cx + fk->GetDivider()+fk_int->dd.item_visibility_offset, baseline+1, -fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
          } else {
            // Item belongs to another group
            df->DrawRGBBox( fk_int->main_panel, 116, 116, 132,
                            fk_int->dd.cx+1, baseline+1, rx, fk_int->dd.item_height-1 );
            df->DrawRGBBox( fk_int->main_panel, 116, 116, 132,
                            fk_int->dd.cx + fk->GetDivider()+fk_int->dd.item_visibility_offset, baseline+1, -fk_int->dd.item_visibility_offset, fk_int->dd.item_height-1 );
          }
        }
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  // Main Columns
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c1r, fk_int->dd.c1g, fk_int->dd.c1b,
                  fk_int->dd.cx+fk_int->dd.item_type_column, fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2b,
                  fk_int->dd.cx+fk_int->dd.item_type_column, fk_int->dd.truey, 1, 19 );

  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c1r, fk_int->dd.c1g, fk_int->dd.c1b,
                  fk_int->dd.cx+fk_int->dd.item_text_column, fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2b,
                  fk_int->dd.cx+fk_int->dd.item_text_column, fk_int->dd.truey, 1, 19 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  fk_int->dd.cx+fk->GetDivider(), fk_int->dd.truey, 1, fk_int->dd.trueh );

  // Secondary Columns
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c1r, fk_int->dd.c1r, fk_int->dd.c1g,
                  fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_lock_offset, fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2g,
                  fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_lock_offset, fk_int->dd.truey, 1, 19 );

  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c1r, fk_int->dd.c1r, fk_int->dd.c1g,
                  fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset, fk_int->dd.cy, 1, fk_int->dd.ch );
  df->DrawRGBBox( fk_int->main_panel, fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2g,
                  fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset, fk_int->dd.truey, 1, 19 );

  //
  // Icon Drawing
  //

  // Icons for items in the list
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line
    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );

      // Figure out what color the text should be drawn in
      item_text_color = fk_int->sorted_items[i]->GetSelected() ? COLOR_LT_YELLOW : COLOR_WHITE;
      static char name_buffer[1024];

      // Display the group index
      if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
        sprintf( name_buffer, "%d", fk_int->sorted_items[i]->GetGroup() + 1 );
        df->WriteText( fk_int->main_panel, name_buffer,
                       item_text_color, fk_int->dd.cx+fk_int->dd.item_group_column+4,
                       baseline );
      }

      // Fold Icons
      if( fk->GetItemDisplayMode() == 0 ) {        // Hierarchical Mode Only
        if( (fk_int->sorted_items[i]->GetChildren().NumElements() > 0) ||
            (fk_int->sorted_items[i]->HasSkeleton()) ) {
          PanelDraw::DrawFoldIcon( fk_int->main_panel, df, 
                                   fk_int->dd.cx + fk_int->dd.item_text_column + 3, baseline,
                                   fk_int->dd.item_height, fk_int->dd.item_height,
                                   fk_int->sorted_items[i]->GetFolded(), fk_int->dd.fold_type );
        }
      }

      // Item Type Icons
      switch( fk_int->sorted_items[i]->GetItemType() ) {
      case LWI_ITEM_OBJECT:
      case LWI_ITEM_NULL_OBJECT:
        PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                               fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                               PanelDraw::object, (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_BONE:
        PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                               fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                               PanelDraw::bone, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_LIGHT:
        PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                               fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                               PanelDraw::light, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_CAMERA:
        PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                               fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                               PanelDraw::camera, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
        break;
      }


      // Item Draw Mode Icons (Visibility)
      switch( fk_int->sorted_items[i]->GetItemType() ) {
      case LWI_ITEM_OBJECT:
      case LWI_ITEM_NULL_OBJECT:
        if( fk_int->sorted_items[i]->GetDrawMode() != LWIDRAW_NONE ) {
          if( fk->GetDrawStyle() == DRAWSTYLE_56 ) {
            PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                                   fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                   (PanelDraw::IconType)(PanelDraw::draw_bounding_box + fk_int->sorted_items[i]->GetDrawMode() - 1),
                                   (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor() );
          } else {
            PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                                   fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                   (PanelDraw::IconType)(PanelDraw::draw_bounding_box + fk_int->sorted_items[i]->GetDrawMode() - 1),
                                   80, 80, 80, PanelDraw::draw_L6 );
          }
        }
        break;

      case LWI_ITEM_BONE:
      case LWI_ITEM_CAMERA:
      case LWI_ITEM_LIGHT:
        if( fk_int->sorted_items[i]->GetDrawMode() != LWIDRAW_NONE ) {
          if( fk->GetDrawStyle() == DRAWSTYLE_56 ) {
            PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                                   fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                   PanelDraw::check,
                                   (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
          } else {
            PanelDraw::DrawLWIcon( fk_int->main_panel, df,
                                   fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                   PanelDraw::check,
                                   80, 80, 80, PanelDraw::draw_L6 );
          }
        }
        break;
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

/* Testing:  Draw a box around the Control Perimiter and the Hot Parimiter
  PanelDraw::EmptyBox( fk_int->main_panel, df, CON_X(ctl), CON_Y(ctl), CON_W(ctl), CON_H(ctl),
                       1, 1,   0, 255, 0,   0, 255, 0 );

  PanelDraw::EmptyBox( fk_int->main_panel, df, CON_HOTX(ctl), CON_HOTY(ctl), CON_HOTW(ctl), CON_HOTH(ctl),
                       1, 1,   255, 0, 0,   255, 0, 0 );
*/
}

// DrawExtent():
//  Draws a keyframe extent bar
bool DrawExtent( LWPanelID panel, CDrawFuncs *df,
                 int x, int y, int width, int height,
                 int min_x, int min_w,
                 int x_thickness, int y_thickness,
                 int shine_r,  int shine_g,  int shine_b,
                 int shadow_r, int shadow_g, int shadow_b,
                 int fill_r,   int fill_g,   int fill_b ) {

  if( (x + width < min_x) || (x > min_x + min_w ) )
    return false;

  if( fk->GetDrawStyle() == 0 ) {
    PanelDraw::Button( panel, df,
                       x, y, width, height, 1, 1,
                       fill_r,   fill_g,   fill_b,
                       fill_r,   fill_g,   fill_b,
                       fill_r,   fill_g,   fill_b );
  } else {
    PanelDraw::Button( panel, df,
                       x, y, width, height, 1, 1,
                       shine_r,  shine_g,  shine_b,
                       shadow_r, shadow_g, shadow_b,
                       fill_r,   fill_g,   fill_b );
  }

  // Draw the Extent Handles
  //  Left
  if( min_x < x ) {
    df->DrawRGBPixel( panel, 0, 0, 0, x-1, y+1 );
    df->DrawRGBPixel( panel, 0, 0, 0, x-2, y+2 );
    df->DrawRGBPixel( panel, 0, 0, 0, x-3, y+3 );
    df->DrawRGBPixel( panel, 0, 0, 0, x-4, y+4 );
    df->DrawRGBBox(   panel, 0, 0, 0, x-5, y+5,  1, height-6 );
    df->DrawRGBBox(   panel, 0, 0, 0, x-5, y+height-2, 6, 1     );

    df->DrawRGBBox(   panel, shine_r,  shine_g,  shine_b,  x-1, y+2,    1, height-4  );
    df->DrawRGBPixel( panel, shadow_r, shadow_g, shadow_b, x-2, y+3 );
    df->DrawRGBPixel( panel, shadow_r, shadow_g, shadow_b, x-3, y+4 );
    df->DrawRGBBox(   panel, shadow_r, shadow_g, shadow_b, x-4, y+5,    1, height-8 );
    df->DrawRGBBox(   panel, shine_r,  shine_g,  shine_b,  x-4, y+height-3, 4, 1     );

    df->DrawRGBBox(   panel, fill_r,   fill_g,   fill_b,   x-2, y+4,    1, height-7 );
    df->DrawRGBBox(   panel, fill_r,   fill_g,   fill_b,   x-3, y+5,    1, height-8 );
  }

  //  Right
  if( min_x+min_w > x+width ) {
    df->DrawRGBPixel( panel, 0, 0, 0, x+width,   y+1 );
    df->DrawRGBPixel( panel, 0, 0, 0, x+width+1, y+2 );
    df->DrawRGBPixel( panel, 0, 0, 0, x+width+2, y+3 );
    df->DrawRGBPixel( panel, 0, 0, 0, x+width+3, y+4 );
    df->DrawRGBBox(   panel, 0, 0, 0, x+width+4, y+5,  1, height-6 );
    df->DrawRGBBox(   panel, 0, 0, 0, x+width,   y+height-2, 5, 1     );

    df->DrawRGBBox(   panel, shine_r,  shine_g,  shine_b,  x+width,   y+2,    1, height-4 );
    df->DrawRGBPixel( panel, shadow_r, shadow_g, shadow_b, x+width+1, y+3 );
    df->DrawRGBPixel( panel, shadow_r, shadow_g, shadow_b, x+width+2, y+4 );
    df->DrawRGBBox(   panel, shadow_r, shadow_g, shadow_b, x+width+3, y+5,    1, height-8 );
    df->DrawRGBBox(   panel, shine_r,  shine_g,  shine_b,  x+width+1, y+height-3, 3, 1    );

    df->DrawRGBBox(   panel, fill_r,   fill_g,   fill_b,   x+width+1, y+4,    1, height-7 );
    df->DrawRGBBox(   panel, fill_r,   fill_g,   fill_b,   x+width+2, y+5,    1, height-8 );
  }

  return true;
}

// DrawKeyframeMark():
//  Draws a keyframe mark in the item list.  If selected
//   is true, then a selected key will be drawn.
bool DrawKeyframeMark( LWPanelID panel, CDrawFuncs *df,
                       int x, int y, bool selected ) {
  if( selected ) {
    df->DrawRGBBox( panel, 255, 255, 191, x, y, 5, 5 );
  } else {
    df->DrawRGBBox( panel, 255, 255, 255, x+2, y,   1, 5 );
    df->DrawRGBBox( panel, 255, 255, 255, x,   y+2, 5, 1 );
  }

  return true;
}

// OnItemZoom():
//  Handles changes in the keyframe zoom factor control
void OnItemZoom( LWControl *ctl, FreeKeyInterface *panel ) {
  double value;
  GET_FLOAT( fk_int->item_zoom, value );

  value /= 100.0;
  if( value < .01 )
    value = .01;

  fk->SetZoom( value );
  SET_FLOAT( fk_int->item_zoom, fk->GetZoom() * 100.0 );

  REDRAW_CON( fk_int->item_multiscroll );
  fk_int->RedrawItemList();
}

// OnItemMultiScroll2():
//  Handles input on the multi-scroll
void OnItemMultiScroll2( LWControl *ctl, FreeKeyInterface *panel ) {
  static orig_pos          = 0;
  static last_clicked      = CLICKED_NONE;
  static double orig_pos2  = 0.0;
  static double orig_fact  = 0.0;

  int mx = CON_MOUSEX( ctl );
  int cx = CON_HOTX( ctl );
  int cw = CON_HOTW( ctl );
  int ix = CON_HOTX( fk_int->item_list );

  int values[3];
  GET_IVEC( fk_int->item_multiscroll, values[0], values[1], values[2] );

  // See if the user just clicked
  if( values[2] < 2 ) {
    // Figure out which item was clicked

    // Test the Divider
    int div_near = abs( mx - ix - fk->GetDivider() );
    if( div_near < 4 ) {
      orig_pos = fk->GetDivider();
      last_clicked = CLICKED_DIVIDER;

    // Test the Name Scrollbar
    } else if( mx < ix+fk->GetDivider() - 4 ) {
      // Compute the scrollbar's position
      int scroll_space = ix + fk->GetDivider() - cx - 7;
      int bw = scroll_space/5;
      if( bw < 15 )
        bw = 15;
      int bx = (int) (cx + ( scroll_space * ((double)fk->GetNameScroll() ) / (double)fk_int->dd.item_name_max_scroll ));
      if( bx+bw > cx+scroll_space )
        bx = cx+scroll_space - bw;
      
      // See if it was actually clicked
      if( (mx > bx) && (mx < bx+bw) ) {
        last_clicked = CLICKED_NAME_SCROLL;
        orig_pos = fk->GetNameScroll();
      } else {
        last_clicked = CLICKED_NONE;
      }

    // Test the Key Scrollbar
    } else if( mx > ix+fk->GetDivider() + 4 ) {
      int scroll_space = (cx + cw) - (ix + fk->GetDivider()) - 7;
      int bw = (int)((double)(scroll_space) * fk->GetZoom());
      if( bw < 35 )
        bw = 35;
      int bx = (int) (7 + ix + fk->GetDivider() + scroll_space * fk->GetTimeScroll());
      if( bx+bw > ix + fk->GetDivider() + scroll_space + 7 )
        bx = ix + fk->GetDivider() + scroll_space - bw + 7;

      // See what was actually clicked
      if( (mx > bx) && (mx < bx+bw) ) {
        orig_pos2 = fk->GetTimeScroll();
        orig_fact = fk->GetZoom();

        if( (mx > bx+10) && (mx < bx+bw-10) ) {
          last_clicked = CLICKED_KEY_SCROLL;
        } else if ( (mx > bx) && (mx < bx+10) ) {        // Left Edge
          last_clicked = CLICKED_KEY_EDGE_LEFT;
        } else if ( (mx > bx+bw-10) && (mx < bx+bw) ) {  // Right Edge
          last_clicked = CLICKED_KEY_EDGE_RIGHT;
        }
      } else {
        last_clicked = CLICKED_NONE;
      }
    
    // Default
    } else {
      last_clicked = CLICKED_NONE;
    }
  }

  //
  // Update the control/value
  //
  if( last_clicked == CLICKED_DIVIDER ) {
    // Divider
    int old_value = fk->GetDivider();
    fk->SetDivider( values[0] + orig_pos );
    if( fk->GetDivider() < cx-ix+min_divider_x )
      fk->SetDivider( cx-ix+min_divider_x );
    if( fk->GetDivider() > cx - ix + CON_HOTW( ctl ) - 50 )
      fk->SetDivider( cx - ix + CON_HOTW( ctl ) - 50 );

    if( old_value != fk->GetDivider() ) {
      REDRAW_CON( ctl );
      fk_int->RedrawItemList();
    }
  } else if( last_clicked == CLICKED_NAME_SCROLL ) {
    // Name Scroll Bar
    // Compute the scrollbar's position
    int scroll_space = ix + fk->GetDivider() - cx - 7;
    int bw = scroll_space/5;
    if( bw < 15 )
      bw = 15;
    int bx = (int) (fk_int->dd.cx + ( scroll_space * ((double)fk->GetNameScroll() ) / (double)fk_int->dd.item_name_max_scroll ));
    if( bx+bw > fk_int->dd.cx+scroll_space )
      bx = cx+scroll_space - bw;

    // Update the scroll amount
    double multiplier = (double)fk_int->dd.item_name_max_scroll / (double)(scroll_space - bw);
    int old_pos = fk->GetNameScroll();

    fk->SetNameScroll( orig_pos + (int)(multiplier * (double)values[0]) );
    if( fk->GetNameScroll() < 0 )
      fk->SetNameScroll( 0 );
    if( fk->GetNameScroll() > fk_int->dd.item_name_max_scroll )
      fk->SetNameScroll( fk_int->dd.item_name_max_scroll );

    // Redraw the controls
    if( old_pos != fk->GetNameScroll() ) {
      REDRAW_CON( ctl );
      fk_int->RedrawItemList();
    }
  } else if( last_clicked == CLICKED_KEY_SCROLL ) {
    // Key Scroll Bar
    // Figure out the scrollbar's position
    int    scroll_space  = (cx + cw) - (ix + fk->GetDivider()) - 7;
    int    scroll_loc    = (int)( (double)scroll_space * orig_pos2 );
    int    new_loc       = scroll_loc + values[0];

    double new_time      = (double)new_loc/(double)scroll_space;
    if( new_time < 0.0 )
      new_time = 0.0;
    if( new_time > 1.0 - fk->GetZoom() )
      new_time = 1.0 - fk->GetZoom();

    // Redraw the controls
    if( fk->GetTimeScroll() != new_time ) {
      fk->SetTimeScroll( new_time );
      REDRAW_CON( ctl );
      fk_int->RedrawItemList();
    }
  } else if( last_clicked == CLICKED_KEY_EDGE_RIGHT ) {
    // Key Right Edge
    // Figure out the scrollbar's position
    int    scroll_space  = (cx + cw) - (ix + fk->GetDivider()) - 7;
    int    scroll_loc    = (int)( (double)scroll_space * orig_pos2 );
    int    scroll_width  = (int)((double)(scroll_space) * orig_fact) + values[0];
    int    scroll_max    = scroll_space - scroll_loc;

    if( scroll_width < 35 )
      scroll_width = 35;
    if( scroll_width > scroll_max )
      scroll_width = scroll_max;

    double new_zoom = (double)scroll_width/(double)scroll_space;

    // Redraw the controls
    if( fk->GetZoom() != new_zoom ) {
      fk->SetZoom( new_zoom );
      REDRAW_CON( ctl );
      fk_int->RedrawItemList();
      SET_FLOAT( fk_int->item_zoom, fk->GetZoom() * 100.0 );
    }
  } else if( last_clicked == CLICKED_KEY_EDGE_LEFT ) {
    // Key Left Edge
    // Figure out the scrollbar's position
    int    scroll_space  = (cx + cw) - (ix + fk->GetDivider()) - 7;
    int    scroll_loc    = (int)((double)scroll_space * orig_pos2);
    int    scroll_width  = (int)((double)(scroll_space) * orig_fact) - values[0];
    int    scroll_max    = scroll_space;

    if( scroll_width < 35 )
      scroll_width = 35;
    if( scroll_width > scroll_space )
      scroll_width = scroll_space;

    // Calculate the new zoom factor
    double new_zoom = (double)scroll_width/(double)scroll_space;

    // Calculate the new position of the control
    int new_loc  = scroll_loc + values[0];
    if( new_loc > (scroll_max - 35) )
      new_loc = scroll_max - 35;
    if( new_loc < 0 )
      new_loc = 0;

    double final_pos = (double)new_loc / (double)scroll_space;

    // Redraw the controls
    if( fk->GetZoom() != new_zoom ) {
      fk->SetZoom( new_zoom );
      fk->SetTimeScroll( final_pos );

      REDRAW_CON( ctl );
      fk_int->RedrawItemList();
      SET_FLOAT( fk_int->item_zoom, fk->GetZoom() * 100.0 );
    }
  }
}

// DrawItemMultiScroll():
//  Called to draw the column the lister
void DrawItemMultiScroll( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int ch = CON_HOTH( ctl );
  int cw = CON_HOTW( ctl );

  // Clear the control
  df->DrawRGBBox( fk_int->main_panel, 132, 130, 132, cx, cy, cw, ch );

  // Draw the Min. Divider Position  Mark
  df->DrawRGBBox( fk_int->main_panel, 110, 110, 110, cx+min_divider_x, cy, 1, ch );

  // Draw the controls
  DrawMSDivider();
  DrawMSScrollLow();
}

// DrawMSDivider():
//  Draws the divider representing the division between the
//   item names and item keyframes.
void DrawMSDivider() {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( fk_int->item_list );
  int cy = CON_HOTY( fk_int->item_multiscroll );
  int ch = CON_HOTH( fk_int->item_multiscroll );
  int cw = CON_HOTW( fk_int->item_multiscroll );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider(),   cy,   1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-1, cy+2, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+1, cy+2, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-2, cy+4, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+2, cy+4, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-3, cy+6, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+3, cy+6, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-4, cy+8, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+4, cy+8, 1, 2 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-3, cy+10, 7, 1 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-1, cy+11, 1, ch-11 );
  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+1, cy+11, 1, ch-11 );

  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider(),   cy+2, 1, 2 );

  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-1, cy+4, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider()-1, cy+6, 1, 4 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, cx+fk->GetDivider()+1, cy+4, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider()+1, cy+6, 1, 4 );

  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-2, cy+6, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider()-2, cy+8, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, cx+fk->GetDivider()+2, cy+6, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider()+2, cy+8, 1, 2 );

  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-3, cy+8, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, cx+fk->GetDivider()+3, cy+8, 1, 2 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, cx+fk->GetDivider()-2, cy+9, 2, 1 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, cx+fk->GetDivider()+1, cy+9, 2, 1 );

  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider(), cy+4, 1, ch-4 );

  df->DrawRGBBox( fk_int->main_panel,   0,   0,   0, cx+fk->GetDivider()-4, cy+ch-3, 3, 1 );
  df->DrawRGBBox( fk_int->main_panel,   0,   0,   0, cx+fk->GetDivider()+2, cy+ch-3, 3, 1 );
  df->DrawRGBPixel( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-3, cy+ch-2 );
  df->DrawRGBBox( fk_int->main_panel, 156, 170, 156, cx+fk->GetDivider()-2, cy+ch-2, 5, 1 );
  df->DrawRGBPixel( fk_int->main_panel, 90, 105, 90, cx+fk->GetDivider()+3, cy+ch-2 );
  df->DrawRGBBox( fk_int->main_panel,   0,   0,   0, cx+fk->GetDivider()-4, cy+ch-1, 9, 1 );

  df->DrawRGBPixel( fk_int->main_panel, 0,   0,   0, cx+fk->GetDivider()-4, cy+ch-2 );
  df->DrawRGBPixel( fk_int->main_panel, 0,   0,   0, cx+fk->GetDivider()+4, cy+ch-2 );
}

// DrawMSScrollLow():
//  Draws the lower scroll bar in the MultiScroll
void DrawMSScrollLow() {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( fk_int->item_multiscroll );
  int cy = CON_HOTY( fk_int->item_multiscroll );
  int ch = CON_HOTH( fk_int->item_multiscroll );
  int cw = CON_HOTW( fk_int->item_multiscroll );
  int ix = CON_HOTX( fk_int->item_list );

  // Draw the Item Name Scroll Bar
  int scroll_space = ix + fk->GetDivider() - cx - 7;
  int bw = scroll_space/5;
  if( bw < 15 )
    bw = 15;
  int bx = (int) (cx + scroll_space * ((double)fk->GetNameScroll() ) / (double)fk_int->dd.item_name_max_scroll);
  if( bx+bw > cx+scroll_space )
    bx = cx+scroll_space - bw;

  PanelDraw::Button( fk_int->main_panel, df, bx, cy+1, bw, ch-2, 1, 1, LW_BUTTON_COLOR_GREEN );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)-1, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2),   cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)-4, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2)-3, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)+2, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2)+3, cy+4, 1, ch-8 );

  // Draw the Key Scroll Bar
  scroll_space = (cx + cw) - (ix + fk->GetDivider()) - 7;
  bw = (int)((double)(scroll_space) * fk->GetZoom());
  if( bw < 35 )
    bw = 35;
  bx = (int) (7 + ix + fk->GetDivider() + scroll_space * fk->GetTimeScroll());
  if( bx+bw > ix + fk->GetDivider() + scroll_space + 7 )
    bx = ix + fk->GetDivider() + scroll_space - bw + 7;

  PanelDraw::Button( fk_int->main_panel, df, bx+9, cy+1, bw-18, ch-2, 1, 1, LW_BUTTON_COLOR_GREEN );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)-1, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2),   cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)-4, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2)-3, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx + (bw/2)+2, cy+4, 1, ch-8 );
  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx + (bw/2)+3, cy+4, 1, ch-8 );

  // Draw the Key Scroll Bar Handles
  //  Left
  PanelDraw::Button( fk_int->main_panel, df, bx, cy+1, 10, ch-2, 1, 1, LW_BUTTON_COLOR_GREEN );

  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+10-3, cy+3,    1, ch-6  );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+10-4, cy+4 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+10-5, cy+5 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+10-6, cy+6 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+10-7, cy+7 );
  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+10-8, cy+8,    1, ch-11 );
  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+10-8, cy+ch-4, 6, 1     );

  df->DrawRGBBox(   fk_int->main_panel, 222, 235, 222, bx+10-4, cy+5,    1, ch-9  );
  df->DrawRGBPixel( fk_int->main_panel,  90, 105,  90, bx+10-5, cy+6 );
  df->DrawRGBPixel( fk_int->main_panel,  90, 105,  90, bx+10-6, cy+7 );
  df->DrawRGBBox(   fk_int->main_panel,  90, 105,  90, bx+10-7, cy+8,    1, ch-13 );
  df->DrawRGBBox(   fk_int->main_panel, 222, 235, 222, bx+10-7, cy+ch-5, 4, 1     );


  //  Right
  PanelDraw::Button( fk_int->main_panel, df, bx+bw-10, cy+1, 10, ch-2, 1, 1, LW_BUTTON_COLOR_GREEN );

  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+bw-10+2, cy+3,     1, ch-6  );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+bw-10+3, cy+4 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+bw-10+4, cy+5 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+bw-10+5, cy+6 );
  df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, bx+bw-10+6, cy+7 );
  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+bw-10+7, cy+8,     1, ch-11 );
  df->DrawRGBBox(   fk_int->main_panel, 0, 0, 0, bx+bw-10+7, cy+ch-4, -6, 1     );

  df->DrawRGBBox(   fk_int->main_panel,  90, 105,  90, bx-10+bw+3, cy+5,     1, ch-9  );
  df->DrawRGBPixel( fk_int->main_panel,  90, 105,  90, bx-10+bw+4, cy+6 );
  df->DrawRGBPixel( fk_int->main_panel,  90, 105,  90, bx-10+bw+5, cy+7 );
  df->DrawRGBBox(   fk_int->main_panel, 222, 235, 222, bx-10+bw+6, cy+8,     1, ch-13 );
  df->DrawRGBBox(   fk_int->main_panel, 222, 235, 222, bx-10+bw+6, cy+ch-5, -4, 1     );
}

// DrawMSNowMarker():
//  Draws a marker representing the current point in time in
//   the key list
void DrawMSNowMarker() {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( fk_int->item_multiscroll );
  int cy = CON_HOTY( fk_int->item_multiscroll );
  int ch = CON_HOTH( fk_int->item_multiscroll );
  int cw = CON_HOTW( fk_int->item_multiscroll );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  cx+fk->GetDivider(), cy-1, 1, 2 );

  for( int i=2; i < 12; i++ ) {
    df->DrawRGBBox( fk_int->main_panel, 156, 170, 156,
                    cx+fk->GetDivider()-(5-(i/2)), cy+ch-i-3, (5-(i/2))*2, 1 );
    df->DrawRGBPixel( fk_int->main_panel, 90, 105, 90, cx+fk->GetDivider()+(5-(i/2)), cy+ch-i-3 );
    df->DrawRGBPixel( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-(5-(i/2)), cy+ch-i-3 );
    df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-(5-(i/2))-1, cy+ch-i-3 );
    df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+(5-(i/2))+1, cy+ch-i-3 );
  }

  for( i=-1; i < 2; i++ ) {
    df->DrawRGBBox( fk_int->main_panel, 156, 170, 156,
                    cx+fk->GetDivider()-4, cy+ch-i-3, 8, 1 );
    df->DrawRGBPixel( fk_int->main_panel, 222, 235, 222, cx+fk->GetDivider()-4, cy+ch-i-3);
    df->DrawRGBPixel( fk_int->main_panel, 90, 105, 90, cx+fk->GetDivider()+4, cy+ch-i-3 );
    df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()-4-1, cy+ch-i-3 );
    df->DrawRGBPixel( fk_int->main_panel, 0, 0, 0, cx+fk->GetDivider()+4+1, cy+ch-i-3 );
  }

  df->DrawRGBBox( fk_int->main_panel, 90, 105, 90, cx+fk->GetDivider()-4, cy+ch-2, 9, 1 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  cx+fk->GetDivider()-4, cy+ch-1, 9, 1 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  cx+fk->GetDivider()-2, cy+ch-6, 1, 3 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  cx+fk->GetDivider(), cy+ch-8, 1, 5 );

  df->DrawRGBBox( fk_int->main_panel, 0, 0, 0,
                  cx+fk->GetDivider()+2, cy+ch-6, 1, 3 );
}
