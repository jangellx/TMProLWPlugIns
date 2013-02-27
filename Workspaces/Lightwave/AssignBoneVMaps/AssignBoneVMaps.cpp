//
// QuickParent.cpp
//

#include <stdio.h>

#include "AssignBoneVMaps-Interface.h"

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName AssignBoneVMaps_Usernames[]   = { {"TM-P  Assign Bone VMaps (Debug)",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Assign Bone VMaps (Debug)",      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ActivateBones_Usernames[]     = { {"TM-P  Activate Bones (Debug)",        LANGID_USENGLISH | SRVTAG_USERNAME }, { "Activate Bones (Debug)",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName DeactivateBones_Usernames[]   = { {"TM-P  Dectivate Bones (Debug)",       LANGID_USENGLISH | SRVTAG_USERNAME }, { "Deactivate Bones (Debug)",       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ToggleActiveBones_Usernames[] = { {"TM-P  Toggle Activate Bones (Debug)", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Toggle Activate Bones (Debug)",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RecordBonePivots_Usernames[]  = { {"TM-P  Record Bone Pivots (Debug)",    LANGID_USENGLISH | SRVTAG_USERNAME }, { "Record Bone Pivots (Debug)",     LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RestBones_Usernames[]         = { {"TM-P  Rest Bones Pivots (Debug)",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Rest Bones (Debug)",             LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName AssignBoneVMaps_Usernames[]   = { {"TM-P  Assign Bone VMaps",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Assign Bone VMaps",      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ActivateBones_Usernames[]     = { {"TM-P  Activate Bones",        LANGID_USENGLISH | SRVTAG_USERNAME }, { "Activate Bones",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName DeactivateBones_Usernames[]   = { {"TM-P  Dectivate Bones",       LANGID_USENGLISH | SRVTAG_USERNAME }, { "Deactivate Bones",       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName ToggleActiveBones_Usernames[] = { {"TM-P  Toggle Activate Bones", LANGID_USENGLISH | SRVTAG_USERNAME }, { "Toggle Activate Bones",  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RecordBonePivots_Usernames[]  = { {"TM-P  Record Bone Pivots",    LANGID_USENGLISH | SRVTAG_USERNAME }, { "Record Bone Pivots",     LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
    ServerUserName RestBones_Usernames[]         = { {"TM-P  Rest Bones Pivots",     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Rest Bones",             LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_AssignBoneVMaps--Debug",         AssignBoneVMaps_Activate,      AssignBoneVMaps_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ActivateBones--Debug",           ActivateBones_Activate,        ActivateBones_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_DeactivateBones--Debug",         DeactivateBones_Activate,      DeactivateBones_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ToggleActivateBones--Debug",     ToggleActiveBones_Activate,    ToggleActiveBones_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RecordBonePivots--Debug",        RecordBonePivots_Activate,     RecordBonePivots_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RestBones--Debug",               RestBones_Activate,            RestBones_Usernames           },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_AssignBoneVMaps",                AssignBoneVMaps_Activate,      AssignBoneVMaps_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ActivateBones",                  ActivateBones_Activate,        ActivateBones_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_DeactivateBones",                DeactivateBones_Activate,      DeactivateBones_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RecordBonePivots",               RecordBonePivots_Activate,     RecordBonePivots_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_RestBones",                      RestBones_Activate,            RestBones_Usernames           },
  #endif
    { (const char *)NULL }
  };
}

// AssignBoneVMaps_Activate():
//  Opens a quick interface allowing to to assign bone VMaps to all
//   currently selected bones or bones within all selected objects.
XCALL_ (int) AssignBoneVMaps_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Create the interface class
  AssignBoneVMaps_Interface abvui( global, local );

  // Test Inputs
  if( !abvui.TestInputs( ABV_PROCESS_VMAPS ) )
    return AFUNC_OK;

  // Build a selected item list
  abvui.BuildItemList();

  // Create and open the UI
  if( !abvui.OpenUI() )
    return abvui.GetRetval();

  // Process the Objects/Bones
  abvui.ObtainUIStates();
  abvui.Process( ABV_PROCESS_VMAPS );

  return AFUNC_OK;
}

