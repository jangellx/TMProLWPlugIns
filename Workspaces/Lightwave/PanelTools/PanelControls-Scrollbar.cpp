//
// PanelControls-Scrollbar.cpp
//

#include "PanelControls-Scrollbar.h"
#include "lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h"

// LWPanels Macro Stuff
extern LWPanControlDesc desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecva;
extern LWValue sval;

// Constructor:
PanelControls_Scrollbar::PanelControls_Scrollbar( GlobalFunc *_global, LWPanelID _panel, int length, bool _is_horiz,
                                                  LWControl *_parent_control, int x, int y )
  : global(_global), panel(_panel), event(NULL), user_data(NULL), is_horiz( _is_horiz ),
    min(0), max(100), knob_position(0), knob_size(1), focus(SCROLLFOCUS_NONE),
    parent_control(_parent_control), ox(x), oy(y), control(NULL) {

  panel_funcs   = (LWPanelFuncs     *)global( LWPANELFUNCS_GLOBAL,  GFUSE_ACQUIRE );
  raster_funcs  = (LWRasterFuncs    *)global( LWRASTERFUNCS_GLOBAL, GFUSE_ACQUIRE );

  ow = is_horiz ? length : scrollbar_thickness;
  oh = is_horiz ? scrollbar_thickness : length;

  if( parent_control == NULL ) {
    if( is_horiz )
      control = CANVAS_CTL( panel_funcs, panel, "", ow, oh-2 );
    else
      control = CANVAS_CTL( panel_funcs, panel, "", ow-2, oh );

    ival.intv.value = (int)&Scrollbar_ControlDraw;
    (*control->set)( control, CTL_USERDRAW, &ival );

    CON_SETEVENT( control, NULL, this );
  }
}

// Destructor
PanelControls_Scrollbar::~PanelControls_Scrollbar() {
  global( LWPANELFUNCS_GLOBAL,  GFUSE_RELEASE );
  global( LWCONTEXTMENU_GLOBAL, GFUSE_RELEASE );
}

// Handle():
void * PanelControls_Scrollbar::Handle( void *prev_focus, int flags, int mx, int my ) {
  // If someone else has the focus, do nothing
  if( (prev_focus != NULL) && (prev_focus != this) )
    return prev_focus;

  // Handle clicks
  if( !(flags & MOUSE_DOWN) ) {
    focus = SCROLLFOCUS_NONE;
    Scrollbar_ControlDraw( control, this, DR_REFRESH );
    return NULL;
  }

  if( focus == SCROLLFOCUS_CLICK )
    return NULL;

  int x = ox;
  int y = oy;
  int w = ow;
  int h = oh;

  if( parent_control == NULL ) {
    x = CON_HOTX( control ) - 1;
    y = CON_HOTY( control ) - 1;
    control->get( control, CTL_FLAGS, &ival );
  } else {
    x += CON_HOTX( parent_control );
    y += CON_HOTY( parent_control );
    parent_control->get( parent_control, CTL_FLAGS, &ival );
  }

  static int orig_scroll;
  static int mouse_down_y, mouse_down_x;

  double true_knob_position, true_knob_size, test_knob_position, test_knob_size;
  int length = is_horiz ? w : h;
  ComputeScrollbarExtents( length, true_knob_position, true_knob_size, test_knob_position, test_knob_size );

  if( focus == SCROLLFOCUS_DRAG ) {       // Mouse already being dragged
    if( !(flags & MOUSE_DOWN) ) {
      focus = SCROLLFOCUS_NONE;
      if( event != NULL )
        event( user_data, knob_position );
    } else {
      int delta = (is_horiz) ? mx - mouse_down_x : my - mouse_down_y;
      int length = (is_horiz ? w : h);
      int temp_size = (int)(length * test_knob_size + 0.000000001);
      int unit_knob_size = (knob_size < temp_size) ? temp_size : knob_size;
      int divisor = (knob_size < temp_size) ? length - unit_knob_size : length;

      true_knob_position = (orig_scroll / (double)(max - min)) + ((double)delta / (double)(length/*divisor*/));
      true_knob_position = LimitSliderKnob( true_knob_position, true_knob_size );
      knob_position = TruePositionToScrollPosition( true_knob_position );

      if( event != NULL )
        event( user_data, knob_position );
    }

  } else {                                // No focus yet
    if( (mx < x) || (mx > x + w) || (my < y) || (my > y + h) )
      return NULL;

    if( is_horiz ) {     // Horizontal
      if( (mx - x) < (test_knob_position * w) ) {                            // Mouse click to the left of the slider
        true_knob_position -= true_knob_size;
        true_knob_position = LimitSliderKnob( true_knob_position, true_knob_size );
        knob_position = TruePositionToScrollPosition( true_knob_position );
        focus = SCROLLFOCUS_CLICK;
        if( event != NULL )
          event( user_data, knob_position );

      } else if( (mx - x) > (test_knob_position + test_knob_size) * w ) {    // Mouse click to the right of the slider
        true_knob_position += true_knob_size;
        true_knob_position = LimitSliderKnob( true_knob_position, true_knob_size );
        knob_position = TruePositionToScrollPosition( true_knob_position );
        focus = SCROLLFOCUS_CLICK;
        if( event != NULL )
          event( user_data, knob_position );

      } else {                                                               // Click and drag on scrollbar
        focus = SCROLLFOCUS_DRAG;
        mouse_down_x = mx;
        mouse_down_y = my;
        orig_scroll = knob_position;
      }

    } else {             // Vertical
      if( (my - y) < (test_knob_position * h) ) {                            // Mouse click above of the slider
        true_knob_position -= true_knob_size;
        true_knob_position = LimitSliderKnob( true_knob_position, true_knob_size );
        knob_position = TruePositionToScrollPosition( true_knob_position );
        focus = SCROLLFOCUS_CLICK;
        if( event != NULL )
          event( user_data, knob_position );

      } else if( (my - y) > (test_knob_position + test_knob_size) * h ) {    // Mouse click below of the slider
        true_knob_position += true_knob_size;
        true_knob_position = LimitSliderKnob( true_knob_position, true_knob_size );
        knob_position = TruePositionToScrollPosition( true_knob_position );
        focus = SCROLLFOCUS_CLICK;
        if( event != NULL )
          event( user_data, knob_position );

      } else {                                                               // Click and drag on scrollbar
        focus = SCROLLFOCUS_DRAG;
        mouse_down_x = mx;
        mouse_down_y = my;
        orig_scroll = knob_position;
      }
    }
  }

  Scrollbar_ControlDraw( control, this, DR_REFRESH );
  return this;
}

// Scrollbar_ControlDraw():
void Scrollbar_ControlDraw(  LWControl *con, void *data, DrMode mode ) {
  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)data;

  int x = scrollbar->ox;
  int y = scrollbar->oy;
  int w = scrollbar->ow;
  int h = scrollbar->oh;

  if( scrollbar->parent_control == NULL ) {
    x = CON_HOTX( scrollbar->control ) - 1;
    y = CON_HOTY( scrollbar->control ) - 1;
    scrollbar->control->get( scrollbar->control, CTL_FLAGS, &ival );
  } else {
    x += CON_HOTX( scrollbar->parent_control );
    y += CON_HOTY( scrollbar->parent_control );
    scrollbar->parent_control->get( scrollbar->parent_control, CTL_FLAGS, &ival );
  }

  if( mode == DR_ERASE ) {
    if( ival.intv.value != CTLF_INVISIBLE )
      scrollbar->panel_funcs->drawFuncs->drawBox( scrollbar->panel, LWP_BG, x, y, w, h );

  } else { // Ghosted/Normal (same thing)
    if( scrollbar->is_horiz )
      scrollbar->DrawHorizontalScrollbar( x, y, w );
    else
      scrollbar->DrawVerticalScrollbar( x, y, h );
  }
}

// SheetRowScrollbarExtents():
//  Returns the knob position and size for drawing/testing, which may be 
//   slightly different than the actual slider position.
void PanelControls_Scrollbar::ComputeScrollbarExtents( int length, double &true_knob_position, double &true_knob_size,
                                                                   double &test_knob_position, double &test_knob_size ) {

  // Compute the knob size
  true_knob_size =  (double)knob_size / (double)(max-min);

  // Max size test
  if( true_knob_size > 1.0 )
    true_knob_size = 1.0;

  // Min size test
  double test_size = (double)scrollbar_min_knob_size / (double)length;
  if( true_knob_size < test_size )
    test_knob_size = test_size;
  else
    test_knob_size = true_knob_size;

  // Compute the true knob position
  true_knob_position = (double)knob_position / (double)(max - min);

  // See if the knob beyond end of trough test
  if( true_knob_size + true_knob_position > 1.0 )
    true_knob_position = 1.0 - true_knob_size;

  // See if the knob beyond start of trough test
  if( true_knob_position < 0.0 )
    true_knob_position = 0.0;

  // Test adjustments
  test_knob_position = true_knob_position * (1.0 - (test_knob_size - true_knob_size));
  if( test_knob_position + test_knob_size > 1.0 )
    test_knob_position = 1.0 - test_knob_size;
}

// DrawVerticalScrollbar():
//  Draw a vertical scrollbar
void PanelControls_Scrollbar::DrawVerticalScrollbar( int x, int y, int h ) {
  assert( h > 0 );
  LWRasterID raster = raster_funcs->create( scrollbar_thickness, h, 0 );

  // Compute the sizes
  double true_knob_position, true_knob_size, test_knob_position, test_knob_size;
  ComputeScrollbarExtents( h, true_knob_position, true_knob_size, test_knob_position, test_knob_size );

  // Draw the border
  RasterDraw::EmptyBox( raster, raster_funcs, 0, 0, scrollbar_thickness, h, 1, 1, RGB_( 80, 80, 80 ), RGB_( 80, 80, 80 ) );

  // Draw the trough
  RasterDraw::DrawHorizontalGradient( raster, raster_funcs, 1, 1, scrollbar_thickness-2, h-2,
                                      104, 104, 104,  164, 164, 164 );

  // Draw the knob
  if( test_knob_size + test_knob_position > 1.0 )
    test_knob_position -= (test_knob_size + test_knob_position) - 1.0;

  int mod = (focus == SCROLLFOCUS_DRAG) ? 12 : 0;

  RasterDraw::DrawHorizontalGradient( raster, raster_funcs, 1, 1 + (int)((h-2) * test_knob_position), scrollbar_thickness-2, (int)((h-2) * test_knob_size),
                                      184+mod, 196+mod, 184+mod,  140+mod, 152+mod, 140+mod );

  // Draw the notches on the knob
  RasterDraw::DrawLWIcon( raster, raster_funcs, scrollbar_thickness/2, (int)((h-2) * (test_knob_position + (test_knob_size/2))),
                          RasterDraw::vertical_slider_knob_ticks, RGB_( 108+mod, 120+mod, 108+mod ), RGB_( 204+mod, 216+mod, 204+mod ) );

  // Blit it to the panel
  raster_funcs->blitPanel( raster, 0, 0, panel, x, y, scrollbar_thickness, h );
  raster_funcs->destroy( raster );
}

// DrawHorizontalScrollbar():
//  Draw a horizontal scrollbar
void PanelControls_Scrollbar::DrawHorizontalScrollbar( int x, int y, int w ) {
  assert( w > 0 );
  LWRasterID raster = raster_funcs->create( w, scrollbar_thickness, 0 );

  // Compute the sizes
  double true_knob_position, true_knob_size, test_knob_position, test_knob_size;
  ComputeScrollbarExtents( w, true_knob_position, true_knob_size, test_knob_position, test_knob_size );

  // Draw the border
  RasterDraw::EmptyBox( raster, raster_funcs, 0, 0, w, scrollbar_thickness, 1, 1, RGB_( 80, 80, 80 ), RGB_( 80, 80, 80 ) );

  // Draw the trough
  RasterDraw::DrawVerticalGradient( raster, raster_funcs, 1, 1, w-2, scrollbar_thickness-2,
                                    104, 104, 104,  164, 164, 164 );

  // Draw the knob
  if( test_knob_size + test_knob_position > 1.0 )
    test_knob_position -= (test_knob_size + test_knob_position) - 1.0;

  int mod = (focus == SCROLLFOCUS_DRAG) ? 12 : 0;

  RasterDraw::DrawVerticalGradient( raster, raster_funcs, 1 + (int)((w-2) * test_knob_position), 1, (int)((w-2) * test_knob_size), scrollbar_thickness-2,
                                 184+mod, 196+mod, 184+mod,  140+mod, 152+mod, 140+mod );

  // Draw the notches on the knob
  RasterDraw::DrawLWIcon( raster, raster_funcs, (int)((w-2) * (test_knob_position + (test_knob_size/2))), scrollbar_thickness/2, 
                          RasterDraw::horizontal_slider_knob_ticks, RGB_( 108+mod, 120+mod, 108+mod ), RGB_( 204+mod, 216+mod, 204+mod ) );

  // Blit it to the panel
  raster_funcs->blitPanel( raster, 0, 0, panel, x, y, w, scrollbar_thickness );
  raster_funcs->destroy( raster );
}

// LimitSliderKnob():
//  Caps the knob position to fit in a 0.0-1.0 range modified by the knob size
double PanelControls_Scrollbar::LimitSliderKnob( double knob_position, double knob_size ) {
  if( knob_position < 0.0 )
    knob_position = 0.0;

  if( knob_position + knob_size > 1.0 )
    knob_position = 1.0 - knob_size;

  return knob_position;
}
