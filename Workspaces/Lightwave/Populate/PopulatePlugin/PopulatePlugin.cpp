//
//  PopulatePlugin.cpp
//

#include <stdlib.h>
#include <splug.h>
#include <moni.h>
#include <lwran.h>

#include "populateinterface.h"

extern "C" {
  // Plug-In Information for Lightwave's Use
  // Function Prototypes
  extern XCALL_ (int) Populate_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  // Plug-In Information for Lightwave's Use
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
      { "LayoutGeneric", "TM-P_Populate--Debug", Populate_Activate },
  #else
      { "LayoutGeneric", "TM-P_Populate",        Populate_Activate },
  #endif
      { NULL }
  };

}

// UberNull_Activate():
//  Creates the UberNullInterface Class, which in turn does
//   all the work of creating the UI and handling events,
//   including the act of UberNull-ing
XCALL_ (int) Populate_Activate( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  PopulateInterface _populate( global, serverData,  // Activation-Function-Provided data
                               retval );            // Return Value

  return retval;
}
