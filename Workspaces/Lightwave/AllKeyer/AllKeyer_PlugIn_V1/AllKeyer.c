
// Lightwave 3D All-Keyer Plug-In

// Simple version:  Takes the currently loaded scene and saves
// all of it's objects/lights/etc. out to a new scene file with
// keys at every frame limited by the FirstFrame/LastFrame values.
// Note that all other data from the scene is junked.

#include <stdlib.h>
#include <math.h>

#include "AllKeyer.h"



// Plug-In Information for Lightwave's Use
ServerRecord ServerDesc[] = {
    { "LayoutGeneric", "TM-P_AllKeyer", AllKeyer },
    { NULL }
};

// Activation Function
//  This plug-in is called from the Generic Plug-ins  pop-up.  Right
//   now, it has no user interface to speak of...
XCALL_ (static int) AllKeyer( long version, GlobalFunc *global,
                              void *local, void *serverData ) {

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  return( DoProcess( global, local, serverData ) );
}







