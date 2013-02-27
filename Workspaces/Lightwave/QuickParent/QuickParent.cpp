//
// QuickParent.cpp
//

#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>
#include "QuickParent.h"

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName MultiParent_First_Usernames[]   = { {"TM-P  Multi-Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_USERNAME }, { "Multi-Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName MultiParent_Last_Usernames[]    = { {"TM-P  Multi-Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_USERNAME }, { "Multi-Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ChainParent_First_Usernames[]   = { {"TM-P  Chain Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_USERNAME }, { "Chain Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ChainParent_Last_Usernames[]    = { {"TM-P  Chain Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_USERNAME }, { "Chain Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName Unparent_Usernames[]            = { {"TM-P  Unparent (Debug)",              LANGID_USENGLISH | SRVTAG_USERNAME }, { "Unparent (Debug)",              LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName About_QuickParent_Usernames[]   = { {"TM-P  About QuickParent (Debug)",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "About QuickParent",             LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName InsertIntoHierarchy_Usernames[] = { {"TM-P  Insert Into Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Insert Into Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RemoveFromHierarchy_Usernames[] = { {"TM-P  Remove From Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Remove From Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName MultiParent_First_Usernames[]   = { {"TM-P  Multi-Parent (First)",  LANGID_USENGLISH | SRVTAG_USERNAME }, { "Multi-Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName MultiParent_Last_Usernames[]    = { {"TM-P  Multi-Parent (Last)",   LANGID_USENGLISH | SRVTAG_USERNAME }, { "Multi-Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ChainParent_First_Usernames[]   = { {"TM-P  Chain Parent (First)",  LANGID_USENGLISH | SRVTAG_USERNAME }, { "Chain Parent (First) (Debug)",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ChainParent_Last_Usernames[]    = { {"TM-P  Chain Parent (Last)",   LANGID_USENGLISH | SRVTAG_USERNAME }, { "Chain Parent (Last) (Debug)",   LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName Unparent_Usernames[]            = { {"TM-P  Unparent",              LANGID_USENGLISH | SRVTAG_USERNAME }, { "Unparent (Debug)",              LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName About_QuickParent_Usernames[]   = { {"TM-P  About QuickParent",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "About QuickParent",             LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName InsertIntoHierarchy_Usernames[] = { {"TM-P  Insert Into Hierarchy", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Insert Into Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RemoveFromHierarchy_Usernames[] = { {"TM-P  Remove From Hierarchy", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Remove From Hierarchy (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P-MultiParent_First--Debug",         MultiParent_First_Activate,   MultiParent_First_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P-MultiParent_Last--Debug",          MultiParent_Last_Activate,    MultiParent_Last_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ChainParent_First--Debug",         ChainParent_First_Activate,   ChainParent_First_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ChainParent_Last--Debug",          ChainParent_Last_Activate,    ChainParent_Last_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_Unparent--Debug",                  Unparent_Activate,            Unparent_Usernames            },
    { LWLAYOUTGENERIC_CLASS, "TM-P_About_QuickParent--Debug",         About_QuickParent_Activate,   About_QuickParent_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_InsertIntoHierarchy--Debug",       InsertIntoHierarchy_Activate, InsertIntoHierarchy_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RemoveFromHierarchy--Debug",       RemoveFromHierarchy_Activate, RemoveFromHierarchy_Usernames },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_MultiParent_First",                MultiParent_First_Activate,   MultiParent_First_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MultiParent_Last",                 MultiParent_Last_Activate,    MultiParent_Last_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ChainParent_First",                ChainParent_First_Activate,   ChainParent_First_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ChainParent_Last",                 ChainParent_Last_Activate,    ChainParent_Last_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_Unparent",                         Unparent_Activate,            Unparent_Usernames            },
    { LWLAYOUTGENERIC_CLASS, "TM-P_About_QuickParent",                About_QuickParent_Activate,   About_QuickParent_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_InsertIntoHierarchy",              InsertIntoHierarchy_Activate, InsertIntoHierarchy_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RemoveFromHierarchy",              RemoveFromHierarchy_Activate, RemoveFromHierarchy_Usernames },
  #endif
    { (const char *)NULL }
  };
}


const char *about_text[] = {
  LWABOUT_YELLOW,
  "QuickParent",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Parenting Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2000-2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "ChainParent (First), ChainParent (Last),",
  "MultiParent (First), MultiParent (Last)",
  "Unparent, About QuickParent,"
  "Insert Into Hierarchy,",
  "Remove From Hierarchy",
  NULL };

// About_QuickParent_Activate():
//  Just a quick about box to ensure I get some credit. ;)
XCALL_ (int) About_QuickParent_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWMessageFuncs *message     = (LWMessageFuncs *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  LWPanelFuncs   *panel_funcs = (LWPanelFuncs   *)global( LWPANELFUNCS_GLOBAL,   GFUSE_TRANSIENT );

  LWAboutPanel_TMPro( panel_funcs, "QuickParent", about_text, NULL, true );
  return AFUNC_OK;
}

