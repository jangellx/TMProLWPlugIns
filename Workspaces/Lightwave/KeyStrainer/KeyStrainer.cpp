//
// KeyStrainer.cpp
//

#include "KeyStrainer-Interface.h"

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  // User Names
  ServerUserName KeyStrainer_Usernames[]   = { {"TM-P  Key Strainer"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Key Strainer"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_KeyStrainer"DEBUG_PLUGIN_PLUGNAME, KeyStrainer_Activate, KeyStrainer_Usernames },
    { (const char *)NULL }
  };
}

// KeyStrainer_Activate():
XCALL_ (int) KeyStrainer_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  KeyStrainer_Interface kui( global, local );

  // Build a selected item list
  kui.BuildItemList();

  if( kui.items.NumElements() == 0 ) {
    (*kui.message->error)( "Key Strainer Error: An object, bone, light or camera must be selected.", NULL );
    return AFUNC_OK;
  }

  // Create and open the UI
  kui.OpenUI();

  if( !kui.do_process )
    return AFUNC_OK;

  int all_other_channels, for_state;
  GET_INT( kui.all_other_channels_button, all_other_channels );
  GET_INT( kui.for_popup, for_state );

  if( all_other_channels == 1 ) {
   if( (for_state != 0) && (for_state != 2) ) {
     if( (*kui.message->okCancel)( "REALLY Process All Other Channels?",
                                   "Warning:  You have multiple items selected and All Other Channels enabled.",
                                   "Are you sure you want to do this?  All non-motion channels will also be affected!" ) == 0 ) {
        return AFUNC_OK;
      }
    }
  }

//  kui.for_state = for_state;
//  GET_INT( kui.protect_popup, kui.protect_state );
  kui.Process();

  return AFUNC_OK;
}

