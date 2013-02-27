//
// PanelTools-Scrollbar.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwpanel.h>

typedef void * ScrollbarID;
typedef void   Scrollbar_Event ( void *user_data, int position );

enum Scrollbar_FocusStates {
  SCROLLFOCUS_NONE, SCROLLFOCUS_CLICK, SCROLLFOCUS_DRAG };

const int scrollbar_thickness     = 17;
const int scrollbar_min_knob_size = 30;

// class PanelControls_Scrollbar
class PanelControls_Scrollbar {
public:
  PanelControls_Scrollbar( GlobalFunc *_global, LWPanelID _panel, int length, bool _is_horiz, LWControl *_parent_control = (LWControl *)NULL, int x = 0, int y = 0 );
  ~PanelControls_Scrollbar();

  // Management
         void *        Handle( void * prev_focus, int flags, int mx, int my );
         void          UpdateScrollbarSizes();

         void          ComputeScrollbarExtents( int length, double &true_knob_position, double &true_knob_size,
                                                            double &test_knob_position, double &test_knob_size );

         void          DrawVerticalScrollbar(   int x, int y, int h );
         void          DrawHorizontalScrollbar( int x, int y, int w );

         double        LimitSliderKnob(         double knob_position, double knob_size );

  inline int           TruePositionToScrollPosition( double knob_position ) { return (int)((double)(max - min) * (knob_position + 0.000000001)); } // Fraction fixes possible round-off error

  // Accessors
  inline LWControl   * GetControl() { return control; }

  inline void          SetEvent( Scrollbar_Event *_event, void *_user_data ) { event = _event; user_data = _user_data; }

  inline bool          GetIsHoriz() { return is_horiz; }

  inline int           GetMin()                        { return min; }
  inline void          SetMin( int _min )              { min = (_min < max) ? _min : max; }

  inline int           GetMax()                        { return max; }
  inline void          SetMax( int _max )              { max = (min < _max) ? _max : min; }

  inline void          SetMinMax( int _min, int _max ) { min = _min; SetMax( _max ); }

  inline int           GetKnobPosition()               { return knob_position; }
  inline void          SetKnobPosition( int _pos )     { knob_position = _pos; }

  inline int           GetKnobSize()                   { return knob_size; }
  inline void          SetKnobSize( int _size )        { knob_size = _size; }

public:
  LWPanelFuncs      * panel_funcs;
  LWRasterFuncs     * raster_funcs;

  LWPanelID           panel;
  LWControl         * control;

  Scrollbar_Event   * event;
  void              * user_data;

  bool                is_horiz;

  int                 min;
  int                 max;
  int                 knob_position;
  int                 knob_size;

  LWControl         * parent_control;
  int                 ox, oy, ow, oh;  // Offset position/size ot draw/handle at

  Scrollbar_FocusStates focus;

  GlobalFunc        * global;
};

void Scrollbar_ControlDraw(  LWControl *con, void *data, DrMode mode );

