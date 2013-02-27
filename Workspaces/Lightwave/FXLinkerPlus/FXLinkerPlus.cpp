//
// FXLinkerPlus.cpp
//

#include "FXLinkerPlus-Interface.h"

extern "C" {
  // User Names
  ServerUserName FXLinkerPlus_Generic_Usernames[]   = { {"TM-P  FX Linker Plus"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_USERNAME }, { "FX Linker Plus"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName FXLinkerPlus_Motion_Usernames[]    = { {"TM-P  FX Linker Plus (Motion)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "FX Linker Plus (Motion)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_FXLinkerPlus"DEBUG_PLUGIN_PLUGNAME, FXLinkerPlus_Generic_Activate,  FXLinkerPlus_Generic_Usernames },
    { LWITEMMOTION_HCLASS,   MOTION_PLUGIN_NAME,                       FXLinkerPlus_Motion_Activate,   FXLinkerPlus_Motion_Usernames  },
    { LWITEMMOTION_ICLASS,   MOTION_PLUGIN_NAME,                       FXLinkerPlus_MotionUI_Activate, FXLinkerPlus_Motion_Usernames  },
    { (const char *)NULL }
  };
}

// FXLinkerPlus_Generic_Activate():
XCALL_ (int) FXLinkerPlus_Generic_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  FXLinkerPlus_Interface::global = global;
  FXLinkerPlus_Interface fxui( local );

  // Create and open the UI
  fxui.CreateUI( NULL );
  fxui.OpenUI();

  return AFUNC_OK;
}

