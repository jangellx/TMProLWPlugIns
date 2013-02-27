//
// GridZoom.h
//

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>

extern "C" {
  XCALL_ (int) GridZoomIn_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData );
  XCALL_ (int) GridZoomInMore_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) GridZoomInDouble_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  XCALL_ (int) GridZoomOut_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData );
  XCALL_ (int) GridZoomOutMore_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) GridZoomOutDouble_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );

  XCALL_ (int) Grid10_0m_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );
  XCALL_ (int) Grid1_0m_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData );
  XCALL_ (int) Grid0_1m_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData );
}

