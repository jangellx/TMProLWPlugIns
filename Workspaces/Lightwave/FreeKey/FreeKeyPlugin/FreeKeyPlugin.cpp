//
// FreeKeyPlugin.cpp
//
// This contains the basic part of the FreeKey Generic Plug-in, including
//  the Activate function.  Interface-specific code can be found in
//  FreeKey-Interface.h|cpp.
//

#include <stdlib.h>

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "../freekey/freekey.h"

#include "freekey-interface.h"

extern "C" {
  // Plug-In Information for Lightwave's Use
  // Function Prototypes
  extern XCALL_ (int) FreeKey_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  // Plug-In Information for Lightwave's Use
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
      { "LayoutGeneric", "TM-P_FreeKey_2--Debug", FreeKey_Activate },
  #else
      { "LayoutGeneric", "TM-P_FreeKey_2",        FreeKey_Activate },
  #endif
      { NULL }
  };

}

// ShutDown()
//  Frees the FreeKey instance
void Shutdown( void * serverData ) {
  if( fk != NULL ) {
    delete fk;
    fk = NULL;
  }

}

// FreeKey_Activate():
//  Creates the FreeKeyInterface Class, which in turn does
//   all the work of creating the UI and handling events,
//   including the act of FreeKey-ing
XCALL_ (int) FreeKey_Activate( long version, GlobalFunc *global,
                               void *local, void *serverData ) {
  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  FreeKeyInterface _freekey( global, (st_LayoutGeneric *)local,  // Activation-Function-Provided data
                             retval );                           // Return Value

  return retval;
}
