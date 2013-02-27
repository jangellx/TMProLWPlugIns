//
// Motify.cpp
//

#include "Motify-Interface.h"

// LWPanels Macro Stuff
extern LWPanControlDesc desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName Motify_Usernames[]   = { {"TM-P  Mot-ify Delete Motion Keys (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Mot-ify Delete Motion Keys (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName Motify_Usernames[]   = { {"TM-P  Mot-ify Delete Motion Keys",         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Mot-ify Delete Motion Keys",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_Mot-ify--Debug", Motify_Activate, Motify_Usernames },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_Mot-ify",        Motify_Activate, Motify_Usernames },
  #endif
    { (const char *)NULL }
  };
}

// Motify_Activate():
XCALL_ (int) Motify_Activate( long version, GlobalFunc *global,
                              void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  Motify_Interface mui( global, local );

  if( mui.sel_type == -1 ) {
    (*mui.message->error)( "Mot-ify Error: An object, bone, light or camera must be selected.", NULL );
    return AFUNC_OK;
  }

  // Build a selected item list
  mui.BuildItemList();

  // Create and open the UI
  mui.OpenUI();
  if( !mui.do_process )
    return AFUNC_OK;

  int all_other_channels, for_state;
  GET_INT( mui.all_other_channels_button, all_other_channels );
  GET_INT( mui.for_popup, for_state );

  if( all_other_channels == 1 ) {
   if( (for_state != 0) && (for_state != 2) ) {
     if( (*mui.message->okCancel)( "REALLY Process All Other Channels?",
                                   "Warning:  You have multiple items selected and All Other Channels enabled.",
                                   "Are you sure you want to do this?  All non-motion channels will also be affected!" ) == 0 ) {
        return AFUNC_OK;
      }
    }
  }

  mui.for_state = for_state;
  GET_INT( mui.protect_popup, mui.protect_state );
  mui.Process();

//  mui.ReselectItems();

  return AFUNC_OK;
}

