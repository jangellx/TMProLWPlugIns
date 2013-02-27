//
// ObjectShadowOptions.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

#include  <lwgeneric.h>

extern "C" {
  XCALL_ (int) ObjectShadowOptions_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );
}

