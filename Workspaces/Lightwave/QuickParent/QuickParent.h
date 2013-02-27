//
// QuickParent.h
//

#ifndef NULL
  #define NULL 0
#endif

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwmaster.h>
#include  <lwglobsrv.h>

extern "C" {
  XCALL_ (int) MultiParent_First_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
  XCALL_ (int) MultiParent_Last_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  XCALL_ (int) ChainParent_First_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
  XCALL_ (int) ChainParent_Last_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  XCALL_ (int) Unparent_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData );

  XCALL_ (int) About_QuickParent_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );

  XCALL_ (int) InsertIntoHierarchy_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );

  XCALL_ (int) RemoveFromHierarchy_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );
}

