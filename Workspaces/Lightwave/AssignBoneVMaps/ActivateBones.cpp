//
// ActivateBones.cpp
//

#include "AssignBoneVMaps-Interface.h"

int ActivateBones_Process( void *local, GlobalFunc *global, ProcessMode mode );

// ActivateBones_Activate():
XCALL_ (int) ActivateBones_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return ActivateBones_Process( local, global, ABV_PROCESS_ACTIVATE );
}

// DeactivateBones_Activate():
XCALL_ (int) DeactivateBones_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return ActivateBones_Process( local, global, ABV_PROCESS_DEACTIVATE );
}

// ToggleActivateBones_Active():
XCALL_ (int) ToggleActiveBones_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return ActivateBones_Process( local, global, ABV_PROCESS_TOGGLE_ACTIVE );
}

// ActivateBones_Process():
int ActivateBones_Process( void *local, GlobalFunc *global, ProcessMode mode ) {
  // Create the interface class
  AssignBoneVMaps_Interface abvui( global, local );

  // Test Inputs
  if( !abvui.TestInputs( mode ) )
    return AFUNC_OK;

  // Build a selected item list
  abvui.BuildItemList();

  // Process the Objects/Bones
  abvui.Process( mode );

  return AFUNC_OK;
}

