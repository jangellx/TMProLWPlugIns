//
// AssignBoneVMaps.h
//

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>

extern "C" {
  XCALL_ (int) AssignBoneVMaps_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  XCALL_ (int) ActivateBones_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) DeactivateBones_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  XCALL_ (int) ToggleActiveBones_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );

  XCALL_ (int) RecordBonePivots_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  XCALL_ (int) RestBones_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );
}
