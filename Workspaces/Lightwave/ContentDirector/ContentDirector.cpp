//
// ContentDirector.cpp
//

#include "ContentDirector-Interface.h"

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  ServerUserName ContentDirector_Usernames[] = { {"TM-P  Content Director"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Content Director"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName QuickCD_Usernames[]         = { {"TM-P  QuickCD"         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "QuickCD"         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_ContentDirector"DEBUG_PLUGIN_PLUGNAME, ContentDirector_Activate, ContentDirector_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_QuickCD"        DEBUG_PLUGIN_PLUGNAME, QuickCD_Activate,         QuickCD_Usernames         },
    { (const char *)NULL }
  };
}

// ContentDirector_Activate():
XCALL_ (int) ContentDirector_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  ContentDirector_Interface cui( global, local );

  // Create and open the UI
  cui.CreateUI();
  cui.OpenUI();

  return AFUNC_OK;
}

// QuickCD_Activate():
XCALL_ (int) QuickCD_Activate( long version, GlobalFunc *global,
                               void *local, void *serverData ) {

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  ContentDirector_Interface cui( global, local );

  // Create the UI
  cui.CreateUI();

  // Switch to the QuickCD dir, if one exists
  SET_INT( cui.dir_list, cui.default_entry );
  HandleDirList( cui.dir_list, NULL );
  HandleChangeContentDirButton( cui.change_content_button, NULL );

  return AFUNC_OK;
}
