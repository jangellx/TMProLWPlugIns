//
// SchemFlow.h
//

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>

extern "C" {
  XCALL_ (int) SchemFlow_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );
}

