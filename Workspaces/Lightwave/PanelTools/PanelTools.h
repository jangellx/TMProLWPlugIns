//
// PanelTools.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

#include <lwglobsrv.h>

#include "PanelToolsGlobal.h"

extern "C" {
  XCALL_ (int) PanelTools_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData );

  XCALL_ (int) PanelControls_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) PanelProgress_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) PanelDraw_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );

  XCALL_ (int) RasterDraw_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData );
}

