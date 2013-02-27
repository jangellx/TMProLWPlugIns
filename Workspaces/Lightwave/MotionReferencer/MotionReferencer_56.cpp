//
// MotionReferencer_56.cpp
//

#include <stdlib.h>
#include <stdio.h>

#include <moni.h>
#include <lwran.h>

#include "MotionReferencerInstance.h"
#include "MotionReferencer.h"

extern "C" {
  XCALL_ (int)  MotionReferencer_Activate_56( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (void) MotionReferencerEvaluate_56( LWInstance _inst,
                                             const ItemMotionAccess *motion_access );

  XCALL_ (LWInstance) MotionReferencerCreate_56( LWError *err, LWItemID item );

  XCALL_ (LWError)    MotionReferencerCopy_56( LWInstance from, LWInstance to, LWItemID item );
}

// MotionReferencer_Activate_56():
XCALL_ (int) MotionReferencer_Activate_56( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  // Version Check
  if( version != 3 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;

  // Fill in the local data
  ItemMotionHandler *im = (ItemMotionHandler *)local;
	im->create   = MotionReferencerCreate_56;
	im->destroy  = MotionReferencerDestroy;
	im->load     = MotionReferencerLoad;
	im->save     = MotionReferencerSave;
	im->copy     = MotionReferencerCopy_56;

	im->evaluate = MotionReferencerEvaluate_56;
	im->descln   = MotionReferencerDescribeLine;

  im->useItems = MotionReferencerUseItems;
  im->changeID = MotionReferencerChangeID;

  // Set the Global Functions
  if( MotionReferencerInstance::GetGlobal() == NULL )
    MotionReferencerInstance::SetGlobal( global );

  return AFUNC_OK;
}

// MotionReferencerCreate_56()
XCALL_ (LWInstance) MotionReferencerCreate_56( LWError	*err, LWItemID item ) {
  return MotionReferencerCreate( err, item );
}

// MotionReferencerCopy_56()
XCALL_ (LWError) MotionReferencerCopy_56( LWInstance from, LWInstance to, LWItemID item ){
  ((MotionReferencerInstance *)to)->CopyFrom(*((MotionReferencerInstance *)from));
  ((MotionReferencerInstance *)to)->SetItemID( item );

  return NULL;
}

// MotionReferencerEvaluate_56()
//  This is what actually does the motion modifications.
XCALL_ (void) MotionReferencerEvaluate_56( LWInstance _inst, const ItemMotionAccess *motion_access ) {
  MotionReferencerInstance &inst = *((MotionReferencerInstance *)(_inst));
  if( !inst.GetEnabled() )
    return;
}

