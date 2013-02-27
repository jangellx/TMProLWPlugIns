//
// SelectOMatic-Lister.cpp
//

#include "SelectOMatic-Lister.h"

// Constructor:
LWLister::LWLister( GlobalFunc *_global,
                    LWPanelFuncs *_panel_funcs, LWPanelID _panel, int w, int h,
                    int          (__cdecl *_count)( void *data ),
                    const char * (__cdecl *_name)(  void *data, int index, int col ),
                    int          (__cdecl *_col_widths)( void *data, int col ),
                    void * _user_data )
  : global(_global), panel_funcs(_panel_funcs), panel(_panel),
    count(_count), name(_name), col_widths(_col_widths), select(NULL),
    user_data(_user_data),
    first_visible_item(0), leftmost_visible_pixel(0), max_hscroll(500) {

  // Fetch RasterFuncs
  raster_funcs = (LWRasterFuncs *)global( LWRASTERFUNCS_GLOBAL, GFUSE_ACQUIRE );

  // Get the column count
  int value;
  for( int i=0; (value = col_widths( user_data, i)) != 0; i++ )
    column_widths.Add( value );

  // Check the interface sizes
  if( h < 45 )
    h = 45;

  if( w < 60 )
    w = 60;

  // Create the slider raster images
  vslider_raster_knob   = raster_funcs->create( 15, 1, 0 );
  vslider_raster_trough = raster_funcs->create( 15, 1, 0 );

  hslider_raster_knob   = raster_funcs->create( 1, 15, 0 );
  hslider_raster_trough = raster_funcs->create( 1, 15, 0 );

  for( double d=0; d < 15.0; d++ ) {
    raster_funcs->drawRGBPixel( vslider_raster_knob,   (int)(184 - (44 * (d/14.0))), (int)(196 - (44 * (d/14.0))), (int)(184 - (44 * (d/14.0))), (int)d, 0 );
    raster_funcs->drawRGBPixel( vslider_raster_trough, (int)(104 + (60 * (d/14.0))), (int)(104 + (60 * (d/14.0))), (int)(104 + (60 * (d/14.0))), (int)d, 0 );

    raster_funcs->drawRGBPixel( hslider_raster_knob,   (int)(184 - (44 * (d/14.0))), (int)(196 - (44 * (d/14.0))), (int)(184 - (44 * (d/14.0))), 0, (int)d );
    raster_funcs->drawRGBPixel( hslider_raster_trough, (int)(104 + (60 * (d/14.0))), (int)(104 + (60 * (d/14.0))), (int)(104 + (60 * (d/14.0))), 0, (int)d );
  }

  // Create the controls
  lister  = CANVAS_CTL( panel_funcs, panel, "", w-18, h-18 );
  vslider = CANVAS_CTL( panel_funcs, panel, "", 15,   h-18 );
  hslider = CANVAS_CTL( panel_funcs, panel, "", w-18, 15   );

  PanelTools::PutOnLineWith( lister, vslider );
  PanelTools::AlignLabels( lister, hslider );

  CON_SETEVENT( lister,  NULL, this );
  CON_SETEVENT( vslider, NULL, this );
  CON_SETEVENT( hslider, NULL, this );

  ival.intv.value = (int)&DrawLWLister;
  (*lister->set)( lister, CTL_USERDRAW, &ival );

  ival.intv.value = (int)&DrawLWListerVSlider;
  (*vslider->set)( vslider, CTL_USERDRAW, &ival );

  ival.intv.value = (int)&DrawLWListerHSlider;
  (*hslider->set)( hslider, CTL_USERDRAW, &ival );
}

// Destructor:
LWLister::~LWLister() {
  raster_funcs->destroy( vslider_raster_knob );
  raster_funcs->destroy( vslider_raster_trough );

  raster_funcs->destroy( hslider_raster_knob );
  raster_funcs->destroy( hslider_raster_trough );

  global( LWRASTERFUNCS_GLOBAL, GFUSE_RELEASE );
}

// Redraw():
void LWLister::Redraw( DrMode mode ) {
  (lister->draw)(  lister,  mode );
  (vslider->draw)( vslider, mode );
  (hslider->draw)( hslider, mode );
}

// Handle():
bool LWLister::Handle( int flags, int x, int y ) {
  static bool   was_down = false;
  static int    down_x, down_y;
  static int    active_control = -1;
  static bool   do_more;
  static bool   was_dragging;
  static int    item_count;
  static int    previous_first_visible_item;
  static int    most_recent_first_visible_item;
  static int    previous_leftmost_visible_pixel;
  static int    most_recent_leftmost_visible_pixel;

  #ifdef _DEBUG
    static bool clear_log = true;
    ofstream out( "i:\\mousetest.txt", (clear_log ? 0 : ios::app) );
    clear_log = false;

    out << "X/Y: " << x << "/" << y << "    " << ((flags & MOUSE_DOWN) ? "Mouse Down" : "Mouse Up" ) << endl;
  #endif

  // See if the mouse has just been clicked and find out which control we're in
  if( flags & MOUSE_DOWN ) {
    if( was_down == false ) {
      down_x = x;
      down_y = y;
      was_down = true;
      do_more  = true;
      was_dragging = false;
      item_count = count( user_data );
      previous_first_visible_item           = first_visible_item;
      most_recent_first_visible_item        = previous_first_visible_item;
      previous_leftmost_visible_pixel       = leftmost_visible_pixel;
      most_recent_leftmost_visible_pixel    = previous_leftmost_visible_pixel;

      // Test the Lister
      int cx = CON_HOTX( lister );
      int cy = CON_HOTY( lister );
      int cw = CON_HOTW( lister );
      int ch = CON_HOTH( lister );

      if( (x >= cx) && (y >=cy) && (x <= cx+cw) && (y <= cy+ch) ) {
        active_control = 0;

        #ifdef _DEBUG
          out << "- Mouse Down:  Lister" << endl;
        #endif
      } else {
        // Test the Vertical Scrollbar
        cx = CON_HOTX( vslider );
        cy = CON_HOTY( vslider );
        cw = CON_HOTW( vslider );
        ch = CON_HOTH( vslider );

        #ifdef _DEBUG
          out << "- Mouse Down:  Vertical Scrollbar" << endl;
        #endif

        if( (x >= cx) && (y >=cy) && (x <= cx+cw) && (y <= cy+ch) ) {
          active_control = 1;
        } else {
          // Test the Horizontal Scrollbar
          cx = CON_HOTX( hslider );
          cy = CON_HOTY( hslider );
          cw = CON_HOTW( hslider );
          ch = CON_HOTH( hslider );

          #ifdef _DEBUG
            out << "- Mouse Down:  Horizontal Scrollbar" << endl;
          #endif

          if( (x >= cx) && (y >=cy) && (x <= cx+cw) && (y <= cy+ch) ) {
            active_control = 2;
          } else {
            active_control = -1;
            return false;
          }
        }
      }
    }
  } else {
    #ifdef _DEBUG
      out << "- Mouse Up" << endl;
    #endif
    was_down = false;
  }

  // Mouse was clicked; decide how to handle it
  if( was_down && do_more ) {
    if( active_control == 0 ) {
      // Lister
      #ifdef _DEBUG
        out << "- Handle Lister" << endl;
      #endif
      return true;
    } else if( active_control == 1 ) {
      // Vertical Scrollbar
      double slider_start, slider_end;
      GetVSliderKnobExtents( slider_start, slider_end );

      // See if the slider fills the trough
      if( (slider_start <= 0.0) && (slider_end >= 1.0) ) {
        do_more = false;
        #ifdef _DEBUG
          out << "- Handle Vertical Scrollbar:  Slider Fills Trough" << endl;
        #endif
        return true;
      }

      // See if the user clicked in an empty part of the trough
      int ch = CON_HOTH( vslider );
      int cy = CON_HOTY( vslider );
      double click = (double)(y-cy)/(double)ch;

      int slider_size = (int)((slider_end - slider_start) * item_count);
      if( !was_dragging ) {
        int jump = 0;
        if( click < slider_start )
          jump = -1;
        else if( click > slider_end )
          jump = 1;

        if( jump != 0 ) {
          #ifdef _DEBUG
            out << "- Handle Vertical Scrollbar:  Jump " << jump << endl;
          #endif

          slider_size *= jump;
          first_visible_item += slider_size;

          if( first_visible_item < 0 )
            first_visible_item = 0;

          if( first_visible_item > (item_count - slider_size) )
            first_visible_item = item_count - slider_size;

          do_more = false;
          Redraw();
          return true;
        }
      }

      // Handle the user dragging the scrollbar
      was_dragging = true;
      double delta = (double)(y - down_y) / (double)ch;

      #ifdef _DEBUG
        out << "- Handle Vertical Scrollbar:  Drag -- Delta " << (y - down_y) << endl;
      #endif

      first_visible_item = previous_first_visible_item + (int)(delta * (double)item_count);

      if( first_visible_item < 0 )
        first_visible_item = 0;

      if( first_visible_item > (item_count - slider_size) )
        first_visible_item = item_count - slider_size;

      if( most_recent_first_visible_item != first_visible_item )  // This just keeps the UI from flickering when nothing has changed
        Redraw();

      most_recent_first_visible_item = first_visible_item;
      return true;
    } else if( active_control == 2 ) {
      // Horizontal Scrollbar
      double slider_start, slider_end;
      GetHSliderKnobExtents( slider_start, slider_end );

      // See if the slider fills the trough
      if( (slider_start <= 0.0) && (slider_end >= 1.0) ) {
        do_more = false;
        #ifdef _DEBUG
          out << "- Handle Horizontal Scrollbar: Slider Fills Trough" << endl;
        #endif
        return true;
      }

      // See if the user clicked in an empty part of the trough
      int cw = CON_HOTW( hslider );
      int cx = CON_HOTX( hslider );
      double click = (double)(x-cx)/(double)cw;

      int slider_size = (int)((slider_end - slider_start) * max_hscroll);
      if( !was_dragging ) {
        int jump = 0;
        if( click < slider_start )
          jump = -1;
        else if( click > slider_end )
          jump = 1;

        if( jump != 0 ) {
          #ifdef _DEBUG
            out << "- Handle Horizontal Scrollbar:  Jump " << jump << endl;
          #endif

          slider_size *= jump;
          leftmost_visible_pixel += slider_size;

          if( leftmost_visible_pixel < 0 )
            leftmost_visible_pixel = 0;

          if( leftmost_visible_pixel > (max_hscroll - slider_size) )
            leftmost_visible_pixel = max_hscroll - slider_size;

          do_more = false;
          Redraw();
          return true;
        }
      }

      // Handle the user dragging the scrollbar
      was_dragging = true;
      double delta = (double)(x - down_x) / (double)cw;

      leftmost_visible_pixel = previous_leftmost_visible_pixel + (int)(delta * (double)max_hscroll);

      #ifdef _DEBUG
        out << "- Handle Horizontal Scrollbar:  Drag -- Delta " << (x - down_x) << "; new LVP:  " << leftmost_visible_pixel << endl;
      #endif

      if( leftmost_visible_pixel < 0 )
        leftmost_visible_pixel = 0;

      if( leftmost_visible_pixel > (max_hscroll - slider_size) )
        leftmost_visible_pixel = max_hscroll - slider_size;

      if( most_recent_leftmost_visible_pixel != leftmost_visible_pixel )  // This just keeps the UI from flickering when nothing has changed
        Redraw();

      most_recent_leftmost_visible_pixel = leftmost_visible_pixel;
      return true;
    }
  }

  return false;
}

// DrawLWLister():
void DrawLWLister( LWControl *ctl, LWLister *lister, DrMode mode ) {
  CDrawFuncs df( lister->GetPanelFuncs()->drawFuncs );
  LWRasterFuncs *rf = lister->GetRasterFuncs();

  // Get some sizes
  int cx = CON_HOTX( lister->GetLister() );
  int cy = CON_HOTY( lister->GetLister() );
  int cw = CON_HOTW( lister->GetLister() );
  int ch = CON_HOTH( lister->GetLister() );

  int line_height = df.DispMetrics()->textHeight + 2;

  int line, col;
  int col_x = 0;
  int count = (*lister->count)( lister->GetUserData() );
  int text_pad;

  for( line=0; ((line+1) * line_height) < ch; line++ ) {
    if( (lister->GetFirstVisibleItem() + (line - 1)) >= count )
      break;

    col_x = 0;
    for( col = 0; col < (int)lister->GetColumnWidths().NumElements(); col++ ) {
      text_pad = (col == 0) ? 5 : 2;

      // Compute the raster size and possible negative X offset
      int raster_col_x = col_x - lister->GetLeftmostVisiblePixel();
      int raster_col_w = lister->GetColumnWidths()[ col ];
      if( (raster_col_x + raster_col_w) < 0 ) {                                  // Ends before left edge
        // Set the next column's X position
        col_x += lister->GetColumnWidths()[ col ];
        continue;
      }

      if( raster_col_x > (lister->GetLeftmostVisiblePixel() + cw) )              // Starts after right edge
        break;

      if( raster_col_x < 0 )
        raster_col_w += raster_col_x;
      else
        raster_col_x = 0;

      // Compute the drawing position
      int draw_x = 0;
      if( col_x > lister->GetLeftmostVisiblePixel() )
        draw_x = col_x - lister->GetLeftmostVisiblePixel();

      // Adjust the raster width for the drawing position
      if( raster_col_w > (cw - draw_x) )
        raster_col_w = (cw - draw_x);

      // Create a raster area for this column of this line
      LWRasterID raster = rf->create( raster_col_w, line_height, 0 );

      if( line == 0 ) {
        // Header Line
        rf->drawRGBBox( raster, 112, 112, 112, 0, 0, raster_col_w, line_height );                            // Clear background

        const char * text = (*lister->name)( lister->GetUserData(), -1, col );
        if( text != NULL )
          rf->drawText( raster, (char *)text, COLOR_BLACK, raster_col_x + text_pad, 1 );                     // Text

        rf->drawLine( raster, COLOR_BLACK, 0, line_height-1, raster_col_w-1, line_height -1 );               // Draw row seperator line
        if( (col_x - lister->GetLeftmostVisiblePixel() + lister->GetColumnWidths()[ col ]) < cw )
          rf->drawLine( raster, COLOR_BLACK, raster_col_w-1, 0, raster_col_w-1, line_height - 1 );           // Draw column seperator Line
      } else {
        // Item Line
        rf->drawRGBBox( raster, 128, 128, 128, 0, 0, raster_col_w, line_height );                            // Clear background

        const char * text = (*lister->name)( lister->GetUserData(), lister->GetFirstVisibleItem() + (line - 1), col );
        if( text != NULL )
          rf->drawText( raster, (char *)text, COLOR_BLACK, raster_col_x + text_pad, 1 );                     // Text

        rf->drawLine( raster, RGB_(112, 112, 112), 0, line_height-1, raster_col_w-1, line_height -1 );       // Draw row seperator line
      }

      rf->blitPanel( raster, 0, 0, lister->GetPanel(), cx + draw_x, cy + (line * line_height), raster_col_w, line_height );
      rf->destroy( raster );

      // Set the next column's X position
      col_x += lister->GetColumnWidths()[ col ];

      // Exit the column loop if no more columns can fit
      if( col_x >= cw )
        break;
    }

    // Handle running out of columns
    int right_edge = col_x - lister->GetLeftmostVisiblePixel();
    if( right_edge < 0 )
      right_edge = 0;

    if( right_edge < cw ) {
      // Create a raster area for this column of this line
      LWRasterID raster = rf->create( cw - right_edge, line_height, 0 );

      if( line == 0 ) {
        // Header Line
        rf->drawRGBBox( raster, 112, 112, 112, 0, 0, cw - right_edge, line_height );                            // Clear background
        rf->drawLine( raster, COLOR_BLACK, 0, line_height-1, cw - right_edge -1, line_height -1 );              // Draw row seperator line
      } else {
        // Item Line
        rf->drawRGBBox( raster, 128, 128, 128, 0, 0, cw - right_edge, line_height );                            // Clear background
        rf->drawLine( raster, RGB_(112, 112, 112), 0, line_height-1, cw - right_edge -1, line_height -1 );      // Draw row seperator line
      }

      rf->blitPanel( raster, 0, 0, lister->GetPanel(), cx + right_edge, cy + (line * line_height), cw-right_edge, line_height );
      rf->destroy( raster );
    }
  }

  // Fill in the empty part of the list
  int remaining = (line * line_height);
  df.DrawRGBBox( lister->GetPanel(), 128, 128, 128, cx, cy+remaining, cw, ch-remaining );
}

// GetVSliderExtents():
bool LWLister::GetVSliderKnobExtents( double &slider_start, double &slider_end ) {
  CDrawFuncs df( panel_funcs->drawFuncs );

  slider_start = 0.0;
  slider_end   = 1.0;

  int item_count = count( user_data );
  int ch = CON_HOTH( vslider );

  if( item_count > 0 ) {
    int line_height = df.DispMetrics()->textHeight + 2;
    int list_height = CON_HOTH( lister );
    double slider_size = ((double)(list_height - line_height)/(double)line_height) / (double)(item_count);        // "- line_height" is to get rid of the heading stuff
    if( slider_size < 1.0 ) {
      slider_start = (double)first_visible_item / (double)item_count;
      slider_end   = slider_start + slider_size;
    } else {
      first_visible_item = 0;
    }
  }

  // See if the knob is too small
  double slider_center = slider_start + (slider_end - slider_start)/2;
  if( ((slider_end - slider_start) * (double)ch) < 30 ) {
    double shift = (15.0/(double)ch);
    slider_start = slider_center - shift;
    slider_end   = slider_center + shift;
  }

  // Make sure the knob exists entirely in the trough
  if( slider_start < 0.0 ) {
    slider_end   += 0.0 - slider_start;
    slider_start  = 0.0;
  }

  if( slider_end > 1.0 ) {
    slider_start -= slider_end - 1.0;
    slider_end    = 1.0;
  }

  return true;
}

// GetHSliderExtents():
bool LWLister::GetHSliderKnobExtents( double &slider_start, double &slider_end ) {
  CDrawFuncs df( panel_funcs->drawFuncs );

  slider_start = 0.0;
  slider_end   = 1.0;

  int cw = CON_HOTW( hslider );

  if( max_hscroll > cw ) {
    double slider_size = (double)cw / (double)max_hscroll;
    if( slider_size < 1.0 ) {
      slider_start = (double)leftmost_visible_pixel / (double)max_hscroll;
      slider_end   = slider_start + slider_size;
    } else {
      leftmost_visible_pixel = 0;
    }
  }

  // See if the knob is too small
  double slider_center = slider_start + (slider_end - slider_start)/2;
  if( ((slider_end - slider_start) * (double)cw) < 30 ) {
    double shift = (15.0/(double)cw);
    slider_start = slider_center - shift;
    slider_end   = slider_center + shift;
  }

  // Make sure the knob exists entirely in the trough
  if( slider_start < 0.0 ) {
    slider_end   += 0.0 - slider_start;
    slider_start  = 0.0;
  }

  if( slider_end > 1.0 ) {
    slider_start -= slider_end - 1.0;
    slider_end    = 1.0;
  }

  return true;
}

// DrawLWListerVSlider():
void DrawLWListerVSlider( LWControl *ctl, LWLister *lister, DrMode mode ) {
  CDrawFuncs df( lister->GetPanelFuncs()->drawFuncs );
  LWRasterFuncs *rf = lister->GetRasterFuncs();

  // Get some sizes
  int cx = CON_HOTX( lister->GetVSlider() );
  int cy = CON_HOTY( lister->GetVSlider() );
  int cw = CON_HOTW( lister->GetVSlider() );
  int ch = CON_HOTH( lister->GetVSlider() );

  // Figure out where the slider should be drawn
  double slider_start  = 0.0;
  double slider_end    = 1.0;

  lister->GetVSliderKnobExtents( slider_start, slider_end );
  double slider_center = slider_start + (slider_end - slider_start)/2;

  // Draw the trough and knob
  double here;
  for( int i=0; i < ch; i++ ) {
    here = ((double)i/(double)ch);
    if( (here < slider_start) || (here > slider_end) )
      rf->blitPanel( lister->GetVSliderRasterTrough(), 0, 0, lister->GetPanel(), cx, cy+i, 15, 1 );
    else
      rf->blitPanel( lister->GetVSliderRasterKnob(),   0, 0, lister->GetPanel(), cx, cy+i, 15, 1 );
  }

  // Draw ticks on the knob
  int tick_start = (int)(slider_center * (double)ch) - 8;

  for( i=0; i < 4; i++ ) {
    df.DrawLine( lister->GetPanel(), RGB_(108, 120, 108), cx+3, cy + tick_start,   cx+12, cy+tick_start   );
    df.DrawLine( lister->GetPanel(), RGB_(204, 216, 204), cx+3, cy + tick_start+1, cx+12, cy+tick_start+1 );
    tick_start += 5;
  }

  // Draw the border
  PanelDraw::EmptyBox( lister->GetPanel(), &df, cx-1, cy-1, cw+2, ch+2, 1, 1, 80, 80, 80,  80, 80, 80 );
}

// DrawLWListerHSlider():
void DrawLWListerHSlider( LWControl *ctl, LWLister *lister, DrMode mode ) {
  CDrawFuncs df( lister->GetPanelFuncs()->drawFuncs );
  LWRasterFuncs *rf = lister->GetRasterFuncs();

  // Get some sizes
  int cx = CON_HOTX( lister->GetHSlider() );
  int cy = CON_HOTY( lister->GetHSlider() );
  int cw = CON_HOTW( lister->GetHSlider() );
  int ch = CON_HOTH( lister->GetHSlider() );

  // Figure out where the slider should be drawn
  double slider_start  = 0.0;
  double slider_end    = 1.0;

  lister->GetHSliderKnobExtents( slider_start, slider_end );
  double slider_center = slider_start + (slider_end - slider_start)/2;

  // Draw the trough and knob
  double here;
  for( int i=0; i < cw; i++ ) {
    here = ((double)i/(double)cw);
    if( (here < slider_start) || (here > slider_end) )
      rf->blitPanel( lister->GetHSliderRasterTrough(), 0, 0, lister->GetPanel(), cx+i, cy, 1, 15 );
    else
      rf->blitPanel( lister->GetHSliderRasterKnob(),   0, 0, lister->GetPanel(), cx+i, cy, 1, 15 );
  }

  // Draw ticks on the knob
  int tick_start = (int)(slider_center * (double)cw) - 8;

  for( i=0; i < 4; i++ ) {
    df.DrawLine( lister->GetPanel(), RGB_(108, 120, 108), cx + tick_start,   cy+3, cx+tick_start,   cy+12 );
    df.DrawLine( lister->GetPanel(), RGB_(204, 216, 204), cx + tick_start+1, cy+3, cx+tick_start+1, cy+12 );
    tick_start += 5;
  }

  // Draw the border
  PanelDraw::EmptyBox( lister->GetPanel(), &df, cx-1, cy-1, cw+2, ch+2, 1, 1, 80, 80, 80,  80, 80, 80 );
}

