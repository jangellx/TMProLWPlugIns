//
// PanelTools.cpp
//

#include "paneltools.h"
#include "paneltools_global.h"
#include "panelcontrols_global.h"
#include "panelprogress_global.h"
#include "paneldraw_global.h"
#include "rasterdraw_global.h"

extern "C" {
  ServerRecord ServerDesc[] = {
    { LWGLOBALSERVICE_CLASS, TMP_PANELTOOLS_GLOBAL,     PanelTools_Activate    },
    { LWGLOBALSERVICE_CLASS, TMP_PANELCONTROLS_GLOBAL,  PanelControls_Activate },
    { LWGLOBALSERVICE_CLASS, TMP_PANELPROGRESS_GLOBAL,  PanelProgress_Activate },
    { LWGLOBALSERVICE_CLASS, TMP_PANELDRAW_GLOBAL,      PanelDraw_Activate     },
    { LWGLOBALSERVICE_CLASS, TMP_RASTERDRAW_GLOBAL,     RasterDraw_Activate    },
    NULL
  };
}

// PanelTools_Activate():
XCALL_ (int) PanelTools_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData ) {

  if( version != LWGLOBALSERVICE_VERSION )
    return AFUNC_BADVERSION;
    
  LWGlobalService *service = (LWGlobalService *)local;
  service->data = &panel_tools_functions;

  return AFUNC_OK;
}

// PanelControls_Activate():
XCALL_ (int) PanelControls_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {

  if( version != LWGLOBALSERVICE_VERSION )
    return AFUNC_BADVERSION;

  LWGlobalService *service = (LWGlobalService *)local;
  service->data = &panel_control_functions;

  return AFUNC_OK;
}

// PanelProgress_Activate():
XCALL_ (int) PanelProgress_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {

  if( version != LWGLOBALSERVICE_VERSION )
    return AFUNC_BADVERSION;

  LWGlobalService *service = (LWGlobalService *)local;
  service->data = &panel_progress_functions;

  return AFUNC_OK;
}

// PanelDraw_Activate():
XCALL_ (int) PanelDraw_Activate( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {

  if( version != LWGLOBALSERVICE_VERSION )
    return AFUNC_BADVERSION;

  LWGlobalService *service = (LWGlobalService *)local;
  service->data = &panel_draw_functions;

  return AFUNC_OK;
}

// RasterDraw_Activate():
XCALL_ (int) RasterDraw_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData ) {

  if( version != LWGLOBALSERVICE_VERSION )
    return AFUNC_BADVERSION;

  LWGlobalService *service = (LWGlobalService *)local;
  service->data = &raster_draw_functions;

  return AFUNC_OK;
}
