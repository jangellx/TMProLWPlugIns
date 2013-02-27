//
// SelectOMatic-Lister.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

class LWLister {
public:
  LWLister( GlobalFunc *_global,
            LWPanelFuncs *_panel_funcs, LWPanelID _panel, int w, int h,
            int          (__cdecl *_count)(      void *data ),
            const char * (__cdecl *_name)(       void *data, int index, int col ),
            int          (__cdecl *_col_widths)( void *data, int col ),
            void * _user_data );
  ~LWLister();

  // Management
  void Redraw( DrMode mode = DR_REFRESH );
  bool Handle( int flags, int x, int y );

  bool GetVSliderKnobExtents( double &slider_start, double &slider_end );
  bool GetHSliderKnobExtents( double &slider_start, double &slider_end );

  void SetSelectCallback( bool (__cdecl *_select)( void *data, int index, int col ) ) { select = _select; }

  // Accessors
  inline int    GetX() { return CON_X( lister ); }
  inline int    GetY() { return CON_Y( lister ); }

  inline int    GetW() { int w = CON_W( lister ); int sw = CON_W( vslider ); return w+sw+3; }
  inline int    GetH() { int h = CON_H( lister ); int sh = CON_H( hslider ); return h+sh+3; }

  inline void   MoveCon() { int x, y; int w = CON_W(lister); int h = CON_H( lister); MOVE_CON( vslider, x+w+3, y ); MOVE_CON( hslider, x, y+h+3 ); }

  inline LWPanelFuncs * GetPanelFuncs() { return panel_funcs; }
  inline LWPanelID      GetPanel()      { return panel; }

  inline LWControl    * GetLister()  { return lister;  }
  inline LWControl    * GetVSlider() { return vslider; }
  inline LWControl    * GetHSlider() { return hslider; }

  inline DynArray< int > & GetColumnWidths() { return column_widths; }

  inline void   SetFirstVisibleItem( int _item ) { first_visible_item = _item; }
  inline int    GetFirstVisibleItem()            { return first_visible_item;  }

  inline void   SetLeftmostVisiblePixel( int _left ) { leftmost_visible_pixel = _left; Redraw( DR_REFRESH ); }
  inline int    GetLeftmostVisiblePixel()            { return leftmost_visible_pixel; }

  inline void   SetMaxHScroll( int _max ) { max_hscroll = (_max < 1) ? 1 : _max; }
  inline int    GetMaxHScroll()           { return max_hscroll; }

  inline void   SetUserData( void *_data ) { user_data = _data; }
  inline void * GetUserData() { return user_data; }

  inline GlobalFunc    & GetGlobal()      { return *global; }
  inline LWRasterFuncs * GetRasterFuncs() { return raster_funcs; }

  inline LWRasterID GetVSliderRasterKnob()   { return vslider_raster_knob;   }
  inline LWRasterID GetVSliderRasterTrough() { return vslider_raster_trough; }

  inline LWRasterID GetHSliderRasterKnob()   { return hslider_raster_knob;   }
  inline LWRasterID GetHSliderRasterTrough() { return hslider_raster_trough; }

protected:
  // Panels/Controls
  LWPanelID       panel;
  LWPanelFuncs  * panel_funcs;

  LWControl     * lister;
  LWControl     * vslider;
  LWControl     * hslider;

  // List States
  DynArray< int > column_widths;

  int             first_visible_item;
  int             leftmost_visible_pixel;
  int             max_hscroll;

  void          * user_data;

  // Globals
  GlobalFunc    * global;
  LWRasterFuncs * raster_funcs; 

  // Imagery
  LWRasterID vslider_raster_knob;
  LWRasterID vslider_raster_trough;

  LWRasterID hslider_raster_knob;
  LWRasterID hslider_raster_trough;

public:
  // Callbacks
  int          (__cdecl *count)(      void *data );
  const char * (__cdecl *name)(       void *data, int index, int col );
  int          (__cdecl *col_widths)( void *data, int col );
  bool         (__cdecl *select)(     void *data, int index, int col );
};

// Draw Callbacks
void DrawLWLister(        LWControl *ctl, LWLister *lister, DrMode mode );
void DrawLWListerVSlider( LWControl *ctl, LWLister *lister, DrMode mode );
void DrawLWListerHSlider( LWControl *ctl, LWLister *lister, DrMode mode );

