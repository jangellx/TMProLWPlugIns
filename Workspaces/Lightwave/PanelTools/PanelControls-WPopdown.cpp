//
// PanelControls-WPopdown.cpp
//

#include "PanelControls-WPopdown.h"
#include "lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h"

// LWPanels Macro Stuff
LWPanControlDesc desc;
LWValue ival    = {LWT_INTEGER};
LWValue ivecval = {LWT_VINT};
LWValue fval    = {LWT_FLOAT};
LWValue fvecval = {LWT_VFLOAT};
LWValue sval    = {LWT_STRING};

// Constructor
PanelControls_WPopdown::PanelControls_WPopdown( GlobalFunc *_global, LWPanelID _panel, int width,
                                                const char *_title, const char **_strings )
  : global(_global), panel(_panel), title(_title), event(NULL), user_data(NULL), 
    strings(_strings), string_count(0), count(NULL), name(NULL), prev_index(0) {

  strings = _strings;
  for( string_count=0; strings[ string_count ] != NULL; string_count++ ) { ; }

  panel_funcs   = (LWPanelFuncs     *)global( LWPANELFUNCS_GLOBAL,  GFUSE_ACQUIRE );
  raster_funcs  = (LWRasterFuncs    *)global( LWRASTERFUNCS_GLOBAL, GFUSE_ACQUIRE );
  context_funcs = (ContextMenuFuncs *)global( LWCONTEXTMENU_GLOBAL, GFUSE_ACQUIRE );

  control = AREA_CTL( panel_funcs, panel, "", width-2, panel_funcs->drawFuncs->dispMetrics()->textHeight + 4 );
  CON_SETEVENT( control, WPopdown_ControlEvent, this );

  ival.intv.value = (int)&WPopdown_ControlDraw;
  (*control->set)( control, CTL_USERDRAW, &ival );

  menu = CONTEXT_MENU( context_funcs, WPopdown_StringName, WPopdown_StringCount, this );
}

// Constructor
PanelControls_WPopdown::PanelControls_WPopdown( GlobalFunc *_global, LWPanelID _panel, int width, 
                                                const char *_title, WPopdown_Name *_name, WPopdown_Count *_count )
  : global(_global), panel(_panel), title(_title), event(NULL), user_data(NULL),
    strings(NULL), string_count(0), count(_count), name(_name) {

  panel_funcs   = (LWPanelFuncs     *)global( LWPANELFUNCS_GLOBAL,  GFUSE_ACQUIRE );
  context_funcs = (ContextMenuFuncs *)global( LWCONTEXTMENU_GLOBAL, GFUSE_ACQUIRE );

  control = AREA_CTL( panel_funcs, panel, "", width-2, panel_funcs->drawFuncs->dispMetrics()->textHeight + 4 );
  CON_SETEVENT( control, WPopdown_ControlEvent, this );

  ival.intv.value = (int)&WPopdown_ControlDraw;
  (*control->set)( control, CTL_USERDRAW, &ival );

  menu = CONTEXT_MENU( context_funcs, WPopdown_StringName, WPopdown_StringCount, this );
}

// Destructor
PanelControls_WPopdown::~PanelControls_WPopdown() {
  context_funcs->cmenuDestroy( menu );

  global( LWPANELFUNCS_GLOBAL,  GFUSE_RELEASE );
  global( LWCONTEXTMENU_GLOBAL, GFUSE_RELEASE );
}

// OpenMenu():
//  Opens he menu and calls the event callback, if found
int PanelControls_WPopdown::OpenMenu( bool force ) {
  bool do_open = force;
  if( !do_open ) {
    control->get( control, CTL_FLAGS, &ival );
    if( ival.intv.value == 0 )
      do_open = true;
  }

  if( do_open ) {
    int index = context_funcs->cmenuDeploy( menu, panel, prev_index );
    if( index != -1 ) {
      prev_index = index;
      if( event != NULL )
        event( this, user_data, index );
    }

    return index;
  }

  return -1;
}

// Popdown Callbacks
int WPopdown_StringCount( void * data ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)data;

  if( popdown->count != NULL )
    return popdown->count( popdown, popdown->user_data );

  return popdown->string_count;
}

char * WPopdown_StringName( void * data, int index ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)data;

  if( popdown->name != NULL )
    return popdown->name( popdown, popdown->user_data, index );

  if( (index < 0) || (index >= popdown->string_count) )
    return "";

  return (char *)(popdown->GetStrings()[ index ]);
}

// WPopdown_ControlEvent():
void WPopdown_ControlEvent( LWControlID con, void *data ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)data;
  popdown->OpenMenu( false );
}

// WPopdown_ControlDraw():
void WPopdown_ControlDraw(  LWControl *con, void *data, DrMode mode ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)data;

  int x = CON_HOTX( popdown->control ) - 1;
  int y = CON_HOTY( popdown->control ) - 1;
  int w = CON_HOTW( popdown->control ) + 2;
  int h = CON_HOTH( popdown->control ) + 2;

  LWRasterID raster = popdown->raster_funcs->create( w, h, 0 );

  popdown->control->get( popdown->control, CTL_FLAGS, &ival );

  const char *title = (popdown->title == NULL) ? "(unknown)" : popdown->title;

  if( mode == DR_ERASE ) {
    if( ival.intv.value != CTLF_INVISIBLE )
      popdown->raster_funcs->drawBox( raster, LWP_BG, 0, 0, w, h );

  } else {
    RasterDraw::Button( raster, popdown->raster_funcs, 0, 0, w, h,  184, 196, 184,  0 );

    if( ival.intv.value == CTLF_DISABLE ) {    // Ghosted
      RasterDraw::DrawArrow( raster, popdown->raster_funcs, w - 9, h/2, RasterDraw::arrow_popup, RasterDraw::arrow_solid, RGB_( 204, 216, 204 ) );
      popdown->raster_funcs->drawText( raster, (char *)title, RGB_( 204, 216, 204 ), 6, 4 );

      RasterDraw::DrawArrow( raster, popdown->raster_funcs, w - 12, h/2 - 1, RasterDraw::arrow_popup, RasterDraw::arrow_solid, RGB_( 108, 120, 108 ) );
      popdown->raster_funcs->drawText( raster, (char *)title, RGB_( 108, 120, 108 ), 5, 3 );

    } else {                                   // Normal
      RasterDraw::DrawArrow( raster, popdown->raster_funcs, w - 12, h/2 - 1, RasterDraw::arrow_popup, RasterDraw::arrow_solid, RGB_( 0, 0, 0 ) );
      popdown->raster_funcs->drawText( raster, (char *)title, RGB_( 0, 0, 0 ), 5, 3 );
    }
  }

  popdown->raster_funcs->blitPanel( raster, 0, 0, popdown->panel, x, y, w, h );
  popdown->raster_funcs->destroy( raster );
}

