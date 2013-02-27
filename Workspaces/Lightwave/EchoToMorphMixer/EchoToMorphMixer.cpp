//
// EchoToMorphMixer.cpp
//

#include "EchoToMorphMixer-Interface.h"

DynArray< EchoMapping * > mappings;

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName EchoToMM_Usernames[]   = { {"TM-P  Echo To Morph Mixer (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Echo To Morph Mixer (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName EchoToMM_Usernames[]   = { {"TM-P  Echo To Morph Mixer",         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Echo To Morph Mixer",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_EchoToMorphMixer--Debug", EchoToMM_Activate, EchoToMM_Usernames },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_EchoToMorphMixer",        EchoToMM_Activate, EchoToMM_Usernames },
  #endif
    { (const char *)NULL }
  };
}

// EchoToMM_Activate():
XCALL_ (int) EchoToMM_Activate( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  EchoToMM_Interface eui( global, local );

  // Create and open the UI
  eui.OpenUI();

  return AFUNC_OK;
}

// Startup():
void *Startup( void ) {
  return &mappings;
}

// Shutdown():
void Shutdown( void *serverData ) {
  mappings.Flush();
}

