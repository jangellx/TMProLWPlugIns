//
// ReplaceObjects.h
//


#ifndef NULL
  #define NULL 0
#endif

#include <lwgeneric.h>

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

extern "C" {
  XCALL_ (int) ReplaceObjects_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) ReplaceWithOriginalObject_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData );
}

void *Startup( void );
void  Shutdown( void *serverData );
