//
// MotionReferencer_L6.cpp
//

#include <stdio.h>

#include <lwserver.h>
#include <lwmotion.h>

#include <portable/dynarray/dynarray.h>

#include "MotionReferencer.h"
#include "MotionReferencerInstance.h"

extern "C" {
  XCALL_ (int)  MotionReferencer_Activate_L6( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (void) MotionReferencerEvaluate_L6(  LWInstance _inst,
                                              const LWItemMotionAccess *motion_access );

  XCALL_ (LWInstance)   MotionReferencerCreate_L6( void *priv, void * item, LWError *err );
  XCALL_ (LWError)      MotionReferencerCopy_L6(   LWInstance from, LWInstance to );
  XCALL_ (unsigned int) MotionReferencerFlags_L6(  LWInstance inst );
}

// MotionReferencer_Activate_L6():
XCALL_ (int) MotionReferencer_Activate_L6( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  // Version Check
  if( version != 4 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;

  // Fill in the local data
  LWItemMotionHandler *im = (LWItemMotionHandler *)local;
  im->inst->priv     = NULL;
  im->inst->create   = MotionReferencerCreate_L6;
  im->inst->destroy  = MotionReferencerDestroy;
  im->inst->load     = MotionReferencerLoad;
  im->inst->save     = MotionReferencerSave;
  im->inst->copy     = MotionReferencerCopy_L6;
  im->inst->descln   = MotionReferencerDescribeLine;

  im->evaluate       = MotionReferencerEvaluate_L6;
  im->flags          = MotionReferencerFlags_L6;

  im->item->useItems = MotionReferencerUseItems;
  im->item->changeID = MotionReferencerChangeID;

  // Set the Global Functions
  if( MotionReferencerInstance::GetGlobal() == NULL )
    MotionReferencerInstance::SetGlobal( global );

  return AFUNC_OK;
}

// MotionReferencerCreate_L6()
XCALL_ (LWInstance) MotionReferencerCreate_L6( void *priv, void * item, LWError *err ) {
  return MotionReferencerCreate( err, (LWItemID)item );
}

// MotionReferencerCopy_L6()
XCALL_ (LWError) MotionReferencerCopy_L6( LWInstance from, LWInstance to ){
  ((MotionReferencerInstance *)to)->CopyFrom(*((MotionReferencerInstance *)from));

  return NULL;
}

// MotionReferencerEvaluate_L6()
//  This is what actually does the motion modifications.
XCALL_ (void) MotionReferencerEvaluate_L6( LWInstance _inst, const LWItemMotionAccess *motion_access ) {
  MotionReferencerInstance &inst = *((MotionReferencerInstance *)(_inst));
  if( !inst.GetEnabled() )
    return;
}

// MotionReferencerFlags_L6()
//  Get L6-specific item motion plug-in flags
XCALL_ (unsigned int) MotionReferencerFlags_L6( LWInstance inst ) {
  // Let the MotionReferencerInstance deal with it.
  return ((MotionReferencerInstance *)(inst))->GetFlags();
}
