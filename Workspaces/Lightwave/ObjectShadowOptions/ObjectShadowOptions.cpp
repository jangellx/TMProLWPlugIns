//
// ObjectShadowOptions.cpp
//

#include "ObjectShadowOptions-Interface.h"

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  // User Names
  ServerUserName ObjectShadowOptions_Usernames[]   = { {"TM-P  Object Shadow Options"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Object Shadow Options"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_ObjectShadowOptions"DEBUG_PLUGIN_PLUGNAME, ObjectShadowOptions_Activate, ObjectShadowOptions_Usernames },
    { (const char *)NULL }
  };
}

// ObjectShadowOptions_Activate():
XCALL_ (int) ObjectShadowOptions_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Create the interface class
  ObjectShadowOptions_Interface oui( global, local );

  // Test Inputs
  if( !oui.TestInputs() )
    return AFUNC_OK;

  // Create and open the UI
  if( !oui.OpenUI() )
    return oui.GetRetval();

  // Process the Objects
  oui.ObtainUIStates();
  oui.Process();

  return AFUNC_OK;
}

