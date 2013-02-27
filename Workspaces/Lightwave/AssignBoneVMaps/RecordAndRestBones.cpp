//
// RecordAndRestBones.cpp
//

#include "AssignBoneVMaps-Interface.h"
#include <Lightwave/LWItemHierarchy/LWItemHierarchy.h>

#include <stdio.h>

int  AdjustBones_Process( void *local, GlobalFunc *global, ProcessMode mode );
void AdjustBoneCallback( LWLayoutGeneric *generic, LWItemHierarchy *item, ProcessMode mode );

// RecordBonePivots_Activate():
XCALL_ (int) RecordBonePivots_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;
    
  return AdjustBones_Process( local, global, ABV_PROCESS_RECORD_PIVOTS );
}

// RestBones_Activate():
XCALL_ (int) RestBones_Activate( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return AdjustBones_Process( local, global, ABV_PROCESS_REST );
}

// AdjustBonesProcess():
int AdjustBones_Process( void *local, GlobalFunc *global, ProcessMode mode ) {
  // Create the interface class
  AssignBoneVMaps_Interface abvui( global, local );

  // Test Inputs
  if( !abvui.TestInputs( mode ) )
    return AFUNC_OK;

  // Warn the user of consequences
  if( mode == ABV_PROCESS_RECORD_PIVOTS ) {
    if( (*abvui.message->yesNo)( "Record Bone Pivots",
                                 (abvui.sel_type == LWI_BONE) ? "This will record pivot points for all selected bones.  Are you sure?" :
                                                                "This will record pivot points for bones in the selected objects.  Are you sure?", "" ) == 0 ) {
      return AFUNC_OK;
    }
  }

  if( mode == ABV_PROCESS_REST ) {
    if( (*abvui.message->yesNo)( "Rest Bones",
                                 (abvui.sel_type == LWI_BONE) ? "This will rest all selected bones.  Are you sure?" :
                                                                "This will rest all bones in the selected objects.  Are you sure?", "" ) == 0 ) {
      return AFUNC_OK;
    }
  }

  // Build Item List
  abvui.BuildItemList();

  // Build Hierarchy
  LWItemHierarchy base( LWITEM_NULL );
  base.BuildChildList( abvui.iteminfo );
  base.ReadLayoutSelection( abvui.iteminfo, abvui.ui );

  #ifdef _DEBUG
    ofstream out( "i:\\AssignBoneVMaps_Hierarchy.txt" );
    base.OutputToStream( out, abvui.iteminfo );
    out.close();
  #endif

  // Walk the list and process the bones in hierarchical order
  AdjustBoneCallback( abvui.generic, &base, mode );

  // Reselect Items
  abvui.ReselectItems();

  return AFUNC_OK;
}

// AdjustBoneCallback():
void AdjustBoneCallback( LWLayoutGeneric *generic, LWItemHierarchy *item, ProcessMode mode ) {
  if( item->GetType() == LWI_BONE ) {
    assert( item->GetOwner() != NULL );
    if( item->GetIsSelected() || item->GetOwner()->GetIsSelected() ) {
      char buffer[100];
      sprintf( buffer, "SelectItem %x", item->GetID() );
      (*generic->evaluate)( generic->data, buffer );

      // Record Pivot rotation
      if( mode == ABV_PROCESS_RECORD_PIVOTS )
        (*generic->evaluate)( generic->data, "RecordPivotRotation" );

      // Rest Bone
      if( mode == ABV_PROCESS_REST )
        (*generic->evaluate)( generic->data, "RecordRestPosition" );
    }
  }

  // Process Children
  for( unsigned long i=0; i < item->GetChildren().NumElements(); i++ )
    AdjustBoneCallback( generic, item->GetChildren()[i], mode );

  // Process Skeleton
/*
  if( (item->GetSkeleton().NumElements() > 0) && (item->GetType() == LWI_OBJECT) ) {
    (*generic->evaluate)( generic->data, "EditBones" );
    for( i=0; i < item->GetSkeleton().NumElements(); i++ )
      AdjustBoneCallback( generic, item->GetSkeleton()[i], mode );

    (*generic->evaluate)( generic->data, "EditObjects" );
  }
*/
}
