
// Lightwave 3D All-Keyer Plug-In

// Simple version:  Takes the currently loaded scene and saves
// all of it's objects/lights/etc. out to a new scene file with
// keys at every frame limited by the FirstFrame/LastFrame values.
// Note that all other data from the scene is junked.

#include <stdlib.h>
#include <math.h>

#include "Main_LW.h"

extern XCALL_(int)UITest ( long version, GlobalFunc *global, void *inst, void *serverData);

// Function Prototypes
extern int DoAllKeyer( GlobalFunc *global, void *local, void *serverData );

// Plug-In Information for Lightwave's Use
ServerRecord ServerDesc[] = {
#ifdef _DEBUG
    { "LayoutGeneric", "TM-P_AllKeyer--DEBUG", AllKeyer_Activate },
#else
    { "LayoutGeneric", "TM-P_AllKeyer", AllKeyer_Activate },
#endif
  	{ "LayoutGeneric", "TestUI", UITest },
    { NULL }
};


// Activation Function
//  This plug-in is called from the Generic Plug-ins  pop-up.  Right
//   now, it has no user interface to speak of...
XCALL_ (static int) AllKeyer_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  return( DoAllKeyer( global, local, serverData ) );
}

