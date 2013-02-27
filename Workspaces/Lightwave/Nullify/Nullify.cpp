//
// Nullify.cpp
//

#include "nullify_interface.h"

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName Nullify_Usernames[]   = { {"TM-P  Nullify (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Nullify (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName Nullify_Usernames[]   = { {"TM-P  Nullify",         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Nullify",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_Nullify--Debug", Nullify_Activate, Nullify_Usernames     },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_Nullify",        Nullify_Activate, Nullify_Usernames     },
  #endif
    { (const char *)NULL }
  };
}

// Nullify_Activate():
XCALL_ (int) Nullify_Activate( long version, GlobalFunc *global,
                               void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Create the interface class
  Nullify_Interface nui( global, local );

  // Test Inputs
  if( !nui.TestInputs() )
    return AFUNC_OK;

  // Create and open the UI
  if( !nui.OpenUI() )
    return nui.GetRetval();

  // Process the Objects
  nui.ObtainUIStates();
  nui.Process();

  return AFUNC_OK;
}

