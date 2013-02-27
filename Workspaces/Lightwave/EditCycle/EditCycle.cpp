//
// EditCycle.cpp
//

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>

extern "C" {
  // Prototypes
  XCALL_ (int) EditCycle_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );

  XCALL_ (int) MoveRotateCycle_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  // User Names
  #ifdef _DEBUG
    ServerUserName EditCycle_Usernames[]       = { {"TM-P  Change Edit Mode (Debug)",  LANGID_USENGLISH | SRVTAG_USERNAME }, { "Change Edit Mode (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName MoveRotateCycle_Usernames[] = { {"TM-P  Move/Rotate Cycle (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Move/Rotate (Debug)",      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName EditCycle_Usernames[    ]   = { {"TM-P  Change Edit Mode",          LANGID_USENGLISH | SRVTAG_USERNAME }, { "Change Edit Mode",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName MoveRotateCycle_Usernames[] = { {"TM-P  Move/Rotate Cycle",         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Move/Rotate",              LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_EditCycle--Debug",         EditCycle_Activate,       EditCycle_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MoveRotateCycle--Debug",   MoveRotateCycle_Activate, MoveRotateCycle_Usernames },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_EditCycle",                EditCycle_Activate,       EditCycle_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MoveRotateCycle",          MoveRotateCycle_Activate, MoveRotateCycle_Usernames },
  #endif
    { (const char *)NULL }
  };
}


// EditCycle_Activate():
XCALL_ (int) EditCycle_Activate( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic    = (LWLayoutGeneric *)local;
  LWInterfaceInfo *ui         = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo      *iteminfo   = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );

  LWItemType item_type = LWI_BONE;  // Test for no items selected (assume bone edit mode and skip to light mode)
  if( ui->selItems[0] != NULL )
    item_type = (iteminfo->type)( ui->selItems[0] );

  switch( item_type ) {
    case LWI_LIGHT:
      (*generic->evaluate)( generic->data, "EditCameras" );
      break;

    case LWI_OBJECT:
      (*generic->evaluate)( generic->data, "EditBones" );
      break;

    case LWI_CAMERA:
      (*generic->evaluate)( generic->data, "EditObjects" );
      break;

    case LWI_BONE:
      (*generic->evaluate)( generic->data, "EditLights" );
      break;
  }

  return AFUNC_OK;
}

// MoveRotateCycle_Activate():
XCALL_ (int) MoveRotateCycle_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  static bool was_moving = true;
  was_moving = !was_moving;

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, (was_moving ? "MoveTool" : "RotateTool") );

  return AFUNC_OK;
}

