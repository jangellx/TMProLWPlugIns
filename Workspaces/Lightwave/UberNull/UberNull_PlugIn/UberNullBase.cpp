// 
// UberNullBase.cpp
//
// This file contains the C++ implementation of UberNull,
// which consists of the user interface and the activation
// function, as well as the processing function.


#include <stdlib.h>
#include <splug.h>
#include <moni.h>
#include <lwran.h>
#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "UberNullInterface.h"

extern "C" {
  XCALL_ (int) UberNull_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData );
}

// UberNull_Activate():
//  Creates the UberNullInterface Class, which in turn does
//   all the work of creating the UI and handling events,
//   including the act of UberNull-ing
XCALL_ (int) UberNull_Activate( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  UberNullInterface uber_null( global, serverData,  // Activation-Function-Provided data
                               retval );            // Return Value

  return retval;
}

// ShutDown()
//  Frees the UberNull instance
void Shutdown( void * serverData ) {
  if( un != NULL ) {
    delete un;
    un = NULL;
  }

}
