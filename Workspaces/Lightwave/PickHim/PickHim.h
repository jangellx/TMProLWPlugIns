//
// PickHim.h
//

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwcustobj.h>

extern "C" {
  // Custom Object Functions
  XCALL_ (int) PickHim_Custom_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );

  XCALL_ (void) PickHim_Custom_Evaluate( LWInstance inst, const LWCustomObjAccess *access );
  XCALL_ (int)  PickHim_Custom_Flags(    LWInstance inst );


  // Enable/Disable Functions
  XCALL_ (int) DisablePickers_Generic_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );

  XCALL_ (int) EnablePickers_Generic_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );

  XCALL_ (int) TogglePickers_Generic_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData );

  // Create/Apply/Property functions
  XCALL_ (int) ApplyPickHim_Generic_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) CreatePicker_Generic_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) PickHimProperties_Generic_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData );
}

