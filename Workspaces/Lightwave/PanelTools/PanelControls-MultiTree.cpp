//
// PanelControls-MultiTree.cpp
//

#include "PanelControls-MultiTree.h"
#include "lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h"

// LWPanels Macro Stuff
extern LWPanControlDesc desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecva;
extern LWValue sval;

// Constructor:
PanelControls_MultiTree::PanelControls_MultiTree( GlobalFunc *_global, LWPanelID _panel, int w, int h, int *default_column_widths, int _tree_column, bool hscroll, bool vscroll )
  : global(_global), panel(_panel), user_data(NULL), tree_column(_tree_column),
    hscroll_control(NULL), vscroll_control(NULL), focus(MULTITREEFOCUS_NONE),
    query(NULL), click_event(NULL), item_event(NULL), drag_drop_event(NULL), custom_draw(NULL),
    autosize_column(NULL), resize_column(NULL), next_child(NULL), nth_child(NULL), prev_selected(0) {

  // Fetch some globals
  panel_funcs   = (LWPanelFuncs     *)global( LWPANELFUNCS_GLOBAL,  GFUSE_ACQUIRE );
  raster_funcs  = (LWRasterFuncs    *)global( LWRASTERFUNCS_GLOBAL, GFUSE_ACQUIRE );

  // Set up the default column widths and count
  int max_w = 0;
  for( int i=0; default_column_widths[i] > 0; i++ ) {
    column_widths.Add( default_column_widths[i] );
    max_w += default_column_widths[i];
  }

  assert( max_w > 0 );

  // Create the controls
  int list_w = vscroll ? w - scrollbar_thickness : w;
  int list_h = hscroll ? h - scrollbar_thickness : h;

  list_control = CANVAS_CTL( panel_funcs, panel, "", w-1, h-1 );

  if( hscroll ) {
    hscroll_control = new PanelControls_Scrollbar( global, panel, list_w, true, list_control, 0, list_h );
    hscroll_control->SetMax( max_w );
    hscroll_control->SetKnobSize( list_w );
    hscroll_control->SetEvent( MultiTree_HScrollEvent, this );
  }

  if( vscroll ) {
    vscroll_control = new PanelControls_Scrollbar( global, panel, list_h + 1, false, list_control, list_w, -1 );
    vscroll_control->SetMax( 1 );
    vscroll_control->SetKnobSize( 1 );
    vscroll_control->SetEvent( MultiTree_VScrollEvent, this );
  }

  ival.intv.value = (int)&MultiTree_ControlDraw;
  (*list_control->set)( list_control, CTL_USERDRAW, &ival );

  CON_SETEVENT( list_control, NULL, this );
}

// Destructor
PanelControls_MultiTree::~PanelControls_MultiTree() {
  if( hscroll_control != NULL ) {
    delete hscroll_control;
    hscroll_control = NULL;
  }

  if( vscroll_control != NULL ) {
    delete vscroll_control;
    vscroll_control = NULL;
  }

  ival.intv.value = (int)NULL;
  (*list_control->set)( list_control, CTL_USERDRAW, &ival );

  global( LWPANELFUNCS_GLOBAL,  GFUSE_RELEASE );
  global( LWCONTEXTMENU_GLOBAL, GFUSE_RELEASE );
}

// Refresh():
void PanelControls_MultiTree::Refresh() {
  drag_drop_target = NULL;
  drag_drop_source = NULL;
  drop_as          = MULTITREEDRAGDROP_NONE;

  items.Flush();
  BuildUnfoldedItemList( NULL, 0 );

  if( vscroll_control != NULL ) {
    vscroll_control->SetMax( (items.NumElements() == 0) ? 1 : items.NumElements() - 1 );

    // Update the knob size
    int h = CON_HOTH( list_control );
    if( hscroll_control != NULL )
      h -= scrollbar_thickness;

    vscroll_control->SetKnobSize( ((h - title_height) / GetRowHeight()) - 1 );
  }

  list_control->get( list_control, CTL_FLAGS, &ival );
  if( !(ival.intv.value & CTLF_INVISIBLE) )
    MultiTree_ControlDraw( list_control, this, DR_REFRESH );
};

// BuildUnfoldedItemList():
void PanelControls_MultiTree::BuildUnfoldedItemList( void *item, int depth ) {
  bool is_folded = false;
  if( (query != NULL) && (item != NULL) )
    is_folded = (query( this, user_data, item, MULTITREEQUERY_IS_FOLDED ) != 0);

  PanelControls_MultiTreeItem *new_node;

  bool is_foldable;
  if( next_child != NULL ) {
    // Use next_child function
    for( void *next_item = next_child( this, user_data, item, NULL ); next_item != NULL; next_item = next_child( this, user_data, item, next_item ) ) {
      is_foldable = false;
      if( query != NULL )
        is_foldable = (query( this, user_data, next_item, MULTITREEQUERY_IS_FOLDABLE ) != 0);

      new_node = new PanelControls_MultiTreeItem( next_item, depth + 1, is_foldable );
      items.Add( new_node );
      if( is_foldable ) {
        if( query( this, user_data, next_item, MULTITREEQUERY_IS_FOLDED ) == 0 )
          BuildUnfoldedItemList( new_node->GetItem(), new_node->GetDepth() );
      }
    }

  } else if( nth_child != NULL ) {
    // Use nth_child function
    int i=0;
    for( void *next_item = nth_child( this, user_data, item, i ); next_item != NULL; next_item = nth_child( this, user_data, item, ++i ) ) {
      is_foldable = false;
      if( query != NULL )
        is_foldable = (query( this, user_data, next_item, MULTITREEQUERY_IS_FOLDABLE ) != 0);

      new_node = new PanelControls_MultiTreeItem( next_item, depth + 1, is_foldable );
      items.Add( new_node );
      if( is_foldable ) {
        if( query( this, user_data, next_item, MULTITREEQUERY_IS_FOLDED ) == 0 )
          BuildUnfoldedItemList( new_node->GetItem(), new_node->GetDepth() );
      }
    }
  }
}

// Redraw():
void PanelControls_MultiTree::Redraw() {
  MultiTree_ControlDraw( list_control, this, DR_REFRESH );
};

// ScrollToItem():
void PanelControls_MultiTree::ScrollToItem( void *item ) {
  for( unsigned long i=0; i < items.NumElements(); i++ ) {
    if( items[i]->GetItem() == item ) {
      ScrollToIndex( i );
      break;
    }
  }
}

// ScrollToIndex():
void PanelControls_MultiTree::ScrollToIndex( int index ) {
  if( vscroll_control != NULL )
    vscroll_control->SetKnobPosition( index );
}

const int fold_icon_width = 15;
const int item_text_pad   =  3;
const int indent_step     = 10;

// Handle():
void * PanelControls_MultiTree::Handle( void * prev_focus, int flags, int mx, int my ) {
  // See if the control is disabled
  list_control->get( list_control, CTL_FLAGS, &ival );
  if( (ival.intv.value & CTLF_DISABLE) || (ival.intv.value & CTLF_INVISIBLE) )
    return NULL;

  // Test the scrollbars
  if( hscroll_control != NULL )
    prev_focus = hscroll_control->Handle( prev_focus, flags, mx, my );

  if( vscroll_control != NULL )
    prev_focus = vscroll_control->Handle( prev_focus, flags, mx, my );

  // If someone else has the focus, do nothing
  if( (prev_focus != NULL) && (prev_focus != this) )
    return prev_focus;

  // Handle clicks
  if( !(flags & MOUSE_DOWN) ) {
    bool do_refresh = false;
    if( focus == MULTITREEFOCUS_DRAG_DROP ) {
      if( (drag_drop_source != NULL) && (drag_drop_target != NULL) && (drop_as != MULTITREEDRAGDROP_NONE ) ) {
        if( drag_drop_event( this, user_data, drag_drop_source, drag_drop_target, &drop_as, 1 ) != NULL ) {
          if( drop_as != MULTITREEDRAGDROP_NONE )
            do_refresh = true;
        }
      }

    } else if( defer_selection ) {  // Keeps single item selection from interfering with drag/drop events
      if( item_event != NULL ) {
        item_event( this, user_data, items[ prev_selected ]->GetItem(), prev_column, MULTITREEITEM_SELECT );
        item_event( this, user_data, NULL, prev_column, MULTITREEITEM_DONE_SELECTING );
      }
    }

    focus = MULTITREEFOCUS_NONE;

    if( do_refresh )
      Refresh();
    else
      MultiTree_ControlDraw( list_control, this, DR_REFRESH );

    return NULL;
  }

  // See if we should transition from a click to a drag
  if( focus == MULTITREEFOCUS_CLICK ) {
    // Drag/Drop Items
    if( (drag_drop_event != NULL) && (drag_drop_source != NULL) ) {
      int dx = abs( mouse_down_x - mx);
      int dy = abs( mouse_down_y - my);
      int delta = (dx > dy)? dx : dy;
      if( delta > 3 )
        focus = MULTITREEFOCUS_DRAG_DROP;
    }

  } else if( focus == MULTITREEFOCUS_RESIZE_COLUMN ) {
    // Resizing Columns
    column_widths[ prev_column ] = prev_column_width + (mx - mouse_down_x);
    if( resize_column != NULL ) {
      column_widths[ prev_column ] = resize_column( this, user_data, prev_column, column_widths[ prev_column ] );
      if( column_widths[ prev_column ] < 1 )
        column_widths[ prev_column ] = 1;
    } else {
      if( column_widths[ prev_column ] < 5 )
        column_widths[ prev_column ] = 5;
    }

    MultiTree_ControlDraw( list_control, this, DR_REFRESH );
    return this;
  }

  // Still a click; exit out
  if( focus == MULTITREEFOCUS_CLICK )
    return this;

  int x = CON_HOTX( list_control );
  int y = CON_HOTY( list_control );
  int w = CON_HOTW( list_control ) + 1;
  int h = CON_HOTH( list_control ) + 1;

  int list_w = (vscroll_control == NULL) ? w : w - scrollbar_thickness;
  int list_h = (hscroll_control == NULL) ? h : h - scrollbar_thickness;

  // See if this is our click.  If not, return NULL
  if( focus == MULTITREEFOCUS_NONE ) {
    defer_selection = false;
    if( (mx < x) || (mx > x + list_w) || (my < y) ||( my > y + list_h) )
      return NULL;
  }

  int  col_x          = 0;
  int  leftmost_pixel = (hscroll_control == NULL) ? 0 : hscroll_control->GetKnobPosition();
  int  row_height = GetRowHeight();
  int  top_row    = vscroll_control->GetKnobPosition();
  bool double_click = false;

  bool do_drag_drop = true;
  if( focus == MULTITREEFOCUS_NONE ) {
    mouse_down_x = mx;
    mouse_down_y = my;
    drag_drop_source = NULL;
    drag_drop_target = NULL;
  }

  // Figure out which column this is
  unsigned long i;
  int click_x, click_y;
  for( i=0; i < column_widths.NumElements(); i++ ) {
    if( mx < x + col_x + column_widths[i] - leftmost_pixel ) {
      click_x = mx - (x + col_x - leftmost_pixel);
      break;
    }

    col_x += column_widths[i];
  }

  if( i >= column_widths.NumElements() )
    return this;

  if( focus == MULTITREEFOCUS_NONE ) {
    // Test the title area
    if( my < y + title_height ) {
      focus = MULTITREEFOCUS_CLICK;

      // Check for column border clicks
      if( (click_x < 3) && (i > 0) ) {
        if( double_click && (autosize_column != NULL) ) {
          column_widths[ i-1 ] = autosize_column( this, user_data, i-1 );
          focus = MULTITREEFOCUS_FINISHED;
          MultiTree_ControlDraw( list_control, this, DR_REFRESH );
        } else {
          focus = MULTITREEFOCUS_RESIZE_COLUMN;
          prev_column = i-1;
          prev_column_width = column_widths[ prev_column ];
        }
        return this;
      }

      click_y = (y + title_height) - my;
      if( click_event != NULL ) {
        int click_retval = click_event( this, user_data, NULL, i, flags, click_x, click_y, (double_click ? 1 : 0) );
        if( click_retval == MULTITREEEVENT_FINISHED ) {
          focus = MULTITREEFOCUS_FINISHED;
        } else if( click_retval == MULTITREEEVENT_LOSE_FOCUS ) {
          focus = MULTITREEFOCUS_NONE;
          MultiTree_ControlDraw( list_control, this, DR_REFRESH );
          return NULL;
        }
      }

      return this;
    }
  }

  click_y = (my - (y + title_height)) % row_height;

  // Figure out which row this is
  int row_index = (my - (y + title_height)) / row_height + top_row;
  if( row_index >= (int)items.NumElements() ) {
    // Empty Space Click Events
    if( click_event != NULL ) {
      int click_retval = click_event( this, user_data, (void *)-1, i, flags, click_x, click_y, (double_click ? 1 : 0) );
      if( click_retval == MULTITREEEVENT_FINISHED ) {
        focus = MULTITREEFOCUS_FINISHED;
      } else if( click_retval == MULTITREEEVENT_LOSE_FOCUS ) {
        focus = MULTITREEFOCUS_NONE;
        MultiTree_ControlDraw( list_control, this, DR_REFRESH );
        return NULL;
      }
    }
    return this;
  }

  if( focus == MULTITREEFOCUS_NONE ) {
    focus = MULTITREEFOCUS_CLICK;

    drag_drop_source = items[ row_index ]->GetItem();
    drag_drop_target = NULL;

    // Check for the fold icon
    if( (int)i == tree_column ) {
      if( query != NULL ) {
        if( query( this, user_data, items[ row_index ]->GetItem(), MULTITREEQUERY_IS_FOLDABLE ) != 0 ) {
          int indent = item_text_pad + indent_step * (items[ row_index ]->GetDepth() - 1);
          if( (click_x > indent)  && (click_x < indent + fold_icon_width) ) {
            focus = MULTITREEFOCUS_FINISHED;
            MultiTree_ItemEvents new_fold = (query( this, user_data, items[ row_index ]->GetItem(), MULTITREEQUERY_IS_FOLDED ) == 0) ? MULTITREEITEM_FOLD : MULTITREEITEM_UNFOLD;

            if( item_event != NULL )
              item_event( this, user_data, items[ row_index ]->GetItem(), i, new_fold );

            Refresh();
            return this;
          }
        }
      }
    }

    // Handle item events for shift-based multi-select
    if( flags & IQ_SHIFT ) {
      if( items.NumElements() == 0 )
        return this;

      int low  = (prev_selected <  row_index) ? prev_selected : row_index;
      int high = (prev_selected >= row_index) ? prev_selected : row_index;

      if( low  >= (int)items.NumElements() )   low  = (int)items.NumElements() - 1;
      if( high >= (int)items.NumElements() )   high = (int)items.NumElements() - 1;

      bool is_selected = false;
      if( query != NULL )
        is_selected = (query( this, user_data, items[ row_index ]->GetItem(), MULTITREEQUERY_IS_SELECTED ) == 0) ? false : true;

      MultiTree_ItemEvents select_as = is_selected ? MULTITREEITEM_DESELECT : MULTITREEITEM_ADD_TO_SELECTION;
      for( int i = low; i <= high; i++ ) {
        if( item_event( this, user_data, items[ i ]->GetItem(), i, select_as ) != MULTITREEEVENT_CONTINUE )
          break;
      }

      item_event( this, user_data, NULL, i, MULTITREEITEM_DONE_SELECTING );

      focus = MULTITREEFOCUS_FINISHED;
      prev_selected = row_index;
      return NULL;
    }

    // Handle click events
    bool do_item_event = true;
    if( click_event != NULL ) {
      int click_retval = click_event( this, user_data, items[ row_index ]->GetItem(), i, flags, click_x, click_y, (double_click ? 1 : 0) );
      if( click_retval == MULTITREEEVENT_HANDLED ) {
        do_item_event = false;
      } else if( click_retval == MULTITREEEVENT_FINISHED ) {
        focus = MULTITREEFOCUS_FINISHED;
        do_item_event = false;
      } else if( click_retval == MULTITREEEVENT_LOSE_FOCUS ) {
        focus = MULTITREEFOCUS_NONE;
        MultiTree_ControlDraw( list_control, this, DR_REFRESH );
        return NULL;
      }
    }

    if( !do_item_event || (item_event == NULL) ) {
      prev_selected = row_index;
      return this;
    }

    // Ctrl-select or normal select (no modifiers)
    MultiTree_ItemEvents select_as = MULTITREEITEM_SELECT;
    bool is_selected = false;
    if( query != NULL )
      is_selected = (query( this, user_data, items[ row_index ]->GetItem(), MULTITREEQUERY_IS_SELECTED ) == 0) ? false : true;

    if( flags & IQ_CTRL ) {
      select_as = is_selected ? MULTITREEITEM_DESELECT : MULTITREEITEM_ADD_TO_SELECTION;
      defer_selection = false;
    } else {
      defer_selection = is_selected;
      prev_selected = row_index;
      prev_column = i;
    }

    if( do_item_event && (item_event != NULL) && !defer_selection ) {
      item_event( this, user_data, items[ row_index ]->GetItem(), i, select_as );

      int item_retval = item_event( this, user_data, NULL, i, MULTITREEITEM_DONE_SELECTING );
      if( item_retval == MULTITREEEVENT_CONTINUE ) {
        do_drag_drop = true;

      } else {
        do_drag_drop = false;
        if( item_retval == MULTITREEEVENT_FINISHED ) {
          focus = MULTITREEFOCUS_FINISHED;

        } else if( item_retval == MULTITREEEVENT_LOSE_FOCUS ) {
          focus = MULTITREEFOCUS_NONE;
          return NULL;
        }
      }
    }

    prev_selected = row_index;
  }

  // Handle Drag-Drop Events
  bool do_redraw = do_drag_drop;
  if( do_drag_drop && (drag_drop_event != NULL) && (drag_drop_source != NULL) ) {
    if( focus == MULTITREEFOCUS_DRAG_DROP ) {
      if( my < y + title_height ) {                                                        // scroll up
        if( vscroll_control != NULL ) {
          if( vscroll_control->GetKnobPosition() > 0 )
            vscroll_control->SetKnobPosition( vscroll_control->GetKnobPosition() - 1 );
        }
        drag_drop_target = NULL;

      } else if( my > y + h - ((hscroll_control != NULL) ? scrollbar_thickness : 0) ) {    // scroll down
        if( vscroll_control != NULL ) {
          if( vscroll_control->GetKnobPosition() < vscroll_control->GetMax() )
            vscroll_control->SetKnobPosition( vscroll_control->GetKnobPosition() + 1 );
        }
        drag_drop_target = NULL;

      } else {                                                                             // target
        // Figure out which is the target
        int target_index = (my - (y + title_height)) / row_height + top_row;

        if( items[ target_index ] != drag_drop_source )
          int a = 1;

        if( target_index >= (int)items.NumElements() )
          return this;

        click_y = (my - (y + title_height)) % row_height;

        drop_as = MULTITREEDRAGDROP_AS_CHILD;
        if( click_y < row_height * 0.3333 )
          drop_as = MULTITREEDRAGDROP_AS_INSERTION_BEFORE;
        else if( click_y > row_height * 0.6666 )
          drop_as = MULTITREEDRAGDROP_AS_INSERTION_AFTER;

        drag_drop_target = drag_drop_event( this, user_data, drag_drop_source, items[ target_index ]->GetItem(), &drop_as, 0 );
        do_redraw = true;
      }
    }
  }

  if( do_redraw )
    MultiTree_ControlDraw( list_control, this, DR_REFRESH );

  return this;
}

// MultiTree_ControlDraw():
void MultiTree_ControlDraw( LWControl *con, void *data, DrMode mode ) {
  const int title_bg_color                 = RGB_( 112, 112, 112 ); // Sheet Title background color
  const int title_divider_color            = RGB_(   0,   0,   0 ); // Color of the horizontal dividing line between title area and the first row
  const int row_bg_color                   = RGB_( 128, 128, 128 ); // Row background color
  const int row_bg_selected_color          = RGB_( 160, 160, 160 ); // Selected row background color
  const int row_divider_color              = RGB_( 112, 112, 112 ); // Color of the horizontal dividing line between rows
  const int selected_line_color            = RGB_( 255, 255, 191 ); // Color of selected lines (ie: dividers)
  const int title_text_pad                 = 3;

  PanelControls_MultiTree *mt = (PanelControls_MultiTree *)data;

  int x = CON_HOTX( mt->list_control );
  int y = CON_HOTY( mt->list_control );
  int w = CON_HOTW( mt->list_control ) + 1;
  int h = CON_HOTH( mt->list_control ) + 1;

  mt->list_control->get( mt->list_control, CTL_FLAGS, &ival );

  // Erase
  if( mode & DR_ERASE ) {
    if( ival.intv.value != CTLF_INVISIBLE )
      mt->panel_funcs->drawFuncs->drawBox( mt->panel, LWP_BG, x, y, w, h );

    return;
  }

  // Ghosted/Normal
  PanelControls_MultiTreeItem *node;
  unsigned long i, j;
  int  col_x          = 0;
  int  leftmost_pixel = (mt->hscroll_control == NULL) ? 0 : mt->hscroll_control->GetKnobPosition();

  int  raster_x, raster_w;
  int  raster_h   = (mt->hscroll_control == NULL) ? h - 1 : h - scrollbar_thickness;
  int  list_w     = (mt->vscroll_control == NULL) ? w     : w - scrollbar_thickness;
  int  row_height = mt->GetRowHeight();

  int  top_row    = mt->vscroll_control->GetKnobPosition();
  int  icon_w;
  int  panel_x    = x;
  int  indent;
  bool is_selected;

  // See if the control is disabled
  mt->list_control->get( mt->list_control, CTL_FLAGS, &ival );
  bool is_disabled = (ival.intv.value & CTLF_DISABLE) ? true : false;

  // Walk through each column in the list
  for( i=0; i < mt->column_widths.NumElements(); i++ ) {
    if( col_x + mt->column_widths[i] < leftmost_pixel ) {
      col_x += mt->column_widths[i];
      continue;
    }

    if( col_x - leftmost_pixel > list_w )
      break;

    // Create the raster
    raster_x = (leftmost_pixel > col_x) ? (leftmost_pixel - col_x) : 0;
    raster_w = mt->column_widths[i] - raster_x;
    raster_w -= ((col_x + mt->column_widths[i] > list_w + leftmost_pixel) ? ((col_x + mt->column_widths[i]) - (list_w + leftmost_pixel)) : 0 );

    LWRasterID raster = mt->raster_funcs->create( raster_w, raster_h, 0 );

    // Draw the title area
    mt->raster_funcs->drawBox( raster, title_bg_color,      0,                0, raster_w, title_height - 1 );
    mt->raster_funcs->drawBox( raster, title_divider_color, 0, title_height - 1, raster_w, 1 );

    icon_w = 0;
    if( mt->custom_draw != NULL ) {
      if( mt->custom_draw( mt, mt->user_data, NULL, i, raster, MULTITREEDRAW_FIRST, -raster_x, 0, mt->column_widths[i], title_height ) == NULL ) {
        icon_w = (int)mt->custom_draw( mt, mt->user_data, NULL, i, raster, MULTITREEDRAW_ICON, -raster_x + title_text_pad, 0, mt->column_widths[i], title_height );

        // Draw the title text
        const char * text = mt->custom_draw( mt, mt->user_data, NULL, i, raster, MULTITREEDRAW_TEXT, 0, 0, icon_w, row_height );
        if( text != NULL ) {
          if( text == MULTITREE_CHECKMARK )
            RasterDraw::DrawLWIcon( raster, mt->raster_funcs, -raster_x + title_text_pad, title_height - row_height, RasterDraw::check, RGB_(0, 0, 0), RGB_(0, 0, 0) );
          else
            mt->raster_funcs->drawText( raster, (char *)text, RGB_( 0, 0, 0 ), -raster_x + title_text_pad + icon_w, title_height - row_height );
        }

        mt->custom_draw( mt, mt->user_data, NULL, i, raster, MULTITREEDRAW_LAST, 0, 0, mt->column_widths[i], title_height );
      }
    }

    // Draw the title column seperator
    mt->raster_funcs->drawBox( raster, RGB_( 64, 64, 64 ), -raster_x + mt->column_widths[i]-1, 0, 1, title_height );

    // Walk through each item in the column
    j = top_row - 1;
    for( int row_y = title_height; row_y < raster_h; row_y += row_height ) {
      // See if we're beyond the last item
      if( ++j >= mt->items.NumElements() ) {
        mt->raster_funcs->drawBox( raster, row_bg_color, 0, row_y, raster_w, row_height );
        continue;
      }

      // Draw the row background
      is_selected = false;
      node = mt->items[j];
      if( !is_disabled ) {
        if( mt->query != NULL )
          is_selected = (mt->query( mt, mt->user_data, node->GetItem(), MULTITREEQUERY_IS_SELECTED ) != 0);
      }

      mt->raster_funcs->drawBox( raster, (is_selected ? row_bg_selected_color : row_bg_color ), 0, row_y, raster_w, row_height );

      // Draw the item-specific stuff
      icon_w = 0;
      if( mt->custom_draw != NULL ) {
        if( mt->custom_draw( mt, mt->user_data, node->GetItem(), i, raster, MULTITREEDRAW_FIRST, -raster_x, 0, mt->column_widths[i], title_height ) == NULL ) {
          // Compute the indent
          indent = item_text_pad;
          if( (int)i == mt->GetTreeColumn() ) {
            indent += indent_step * (node->GetDepth() - 1);

            // Draw the fold icon, if available
            if( node->GetIsFoldable() ) {
              if( mt->query != NULL ) {
                bool is_folded = (mt->query( mt, mt->user_data, node->GetItem(), MULTITREEQUERY_IS_FOLDED ) != 0);
                RasterDraw::DrawFoldIcon( raster, mt->raster_funcs, -raster_x + indent + fold_icon_width/2, row_y + row_height/2, 1, 1, RGB_( 200, 200, 200 ), is_folded, RasterDraw::triangle );
              }
            }

            indent += fold_icon_width;
          }

          // Draw the icon, if enabled
          icon_w = (int)mt->custom_draw( mt, mt->user_data, node->GetItem(), i, raster, MULTITREEDRAW_ICON, -raster_x + indent, row_y, mt->column_widths[i], title_height );
          indent += icon_w;

          // Draw the text
          const char * text = mt->custom_draw( mt, mt->user_data, node->GetItem(), i, raster, MULTITREEDRAW_TEXT, -raster_x + indent, row_y, mt->column_widths[i], row_height );
          if( text != NULL ) {
            if( text == MULTITREE_CHECKMARK )
              RasterDraw::DrawLWIcon( raster, mt->raster_funcs, -raster_x + indent, row_y + 1, RasterDraw::check, RGB_(0, 0, 0), RGB_(0, 0, 0) );
            else
              mt->raster_funcs->drawText( raster, (char *)text, RGB_( 0, 0, 0 ), -raster_x + indent, row_y + 1 );
          }

          mt->custom_draw( mt, mt->user_data, node->GetItem(), i, raster, MULTITREEDRAW_LAST, -raster_x + indent, row_y, mt->column_widths[i], title_height );
        }
      }

      // Draw the row seperator
      mt->raster_funcs->drawBox( raster, row_divider_color, 0, row_y + row_height - 1, raster_w, 1 );

      // Draw the drag-and-drop marker
      if( mt->focus == MULTITREEFOCUS_DRAG_DROP ) {
        if( mt->drag_drop_target != NULL ) {
          if( ((int)i == mt->GetTreeColumn()) || (mt->GetTreeColumn() == -1) ) {
            if( mt->drag_drop_target == node->GetItem() ) {
              if( mt->drop_as != MULTITREEDRAGDROP_NONE ) {
                if( mt->drop_as == MULTITREEDRAGDROP_AS_CHILD )
                  indent += indent_step;

                indent -= icon_w; 

                int dd_y = (mt->drop_as == MULTITREEDRAGDROP_AS_INSERTION_BEFORE) ? row_y : row_y + row_height - 2;
                mt->raster_funcs->drawBox( raster, selected_line_color, indent, dd_y, raster_w - indent, 2 );
              }
            }
          }
        }
      }
    }

    // Draw the column seperator
//    RasterDraw::VerticalDivider( raster, mt->raster_funcs, -raster_x + mt->column_widths[i] - 2, 0, raster_h, 1 );

    // Blit it to the panel
    mt->raster_funcs->blitPanel( raster, 0, 0, mt->panel, panel_x, y, raster_w, raster_h );
    mt->raster_funcs->destroy( raster );

    panel_x += raster_w;
    col_x += mt->column_widths[i];
  }

  // Erase the right edge, if needed
  int right_edge = -leftmost_pixel;
  if( mt->hscroll_control == NULL ) {
    for( i=0; i < mt->column_widths.NumElements(); i++ )
      right_edge += mt->column_widths[i];
  } else {
    right_edge += mt->hscroll_control->GetMax();
  }

  if( right_edge < list_w ) {
    mt->panel_funcs->drawFuncs->drawBox( mt->panel, title_bg_color,      x + right_edge, y,                    list_w - right_edge, title_height - 1 );
    mt->panel_funcs->drawFuncs->drawBox( mt->panel, title_divider_color, x + right_edge, y + title_height - 1, list_w - right_edge, 1 );
    mt->panel_funcs->drawFuncs->drawBox( mt->panel, row_bg_color,        x + right_edge, y + title_height,     list_w - right_edge, raster_h - title_height );
  }

  // Draw the scrollbars
  if( mt->vscroll_control == NULL )
    mt->panel_funcs->drawFuncs->drawBox( mt->panel, RGB_( 200, 200, 200 ), x, y + h - scrollbar_thickness - 1, w, 1 );
  else
    Scrollbar_ControlDraw( mt->vscroll_control->GetControl(), mt->vscroll_control, mode );

  if( mt->hscroll_control == NULL )
    mt->panel_funcs->drawFuncs->drawBox( mt->panel, RGB_( 200, 200, 200 ), x + w - scrollbar_thickness - 1, y, 1, h );
  else
    Scrollbar_ControlDraw( mt->hscroll_control->GetControl(), mt->hscroll_control, mode );
}

// GetNumVisibleItems():
int PanelControls_MultiTree::GetNumVisibleItems() {
  int h = CON_HOTH( list_control );
  if( hscroll_control != NULL )
    h -= scrollbar_thickness;

  int count = (h - title_height) / GetRowHeight();
  return( count < (int)items.NumElements() ? count : (int)items.NumElements());
}

// MultiTree_HScrollEvent():
void MultiTree_HScrollEvent( void *user_data, int position ) {
  PanelControls_MultiTree * mt = (PanelControls_MultiTree *)user_data;
  MultiTree_ControlDraw( mt->list_control, mt, DR_REFRESH );
}

// MultiTree_VScrollEvent():
void MultiTree_VScrollEvent( void *user_data, int position ) {
  PanelControls_MultiTree * mt = (PanelControls_MultiTree *)user_data;
  MultiTree_ControlDraw( mt->list_control, mt, DR_REFRESH );
}

