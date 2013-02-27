//
// MotionReferencer.cpp
//

#include <stdio.h>

#include <portable/dynarray/dynarray.h>
#include "MotionReferencerInterface.h"

int MotionReferencer_Version      = 0;
int MotionReferencer_Revision     = 9;
int MotionReferencer_Subrevision  = 0;

extern "C" {
  XCALL_ (int) MotionReferencer_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );

  XCALL_ (int) MotionReferencer_Interface( long version, GlobalFunc *global,
                                           void *local, void *serverData );

  XCALL_ (LWInstance)       MotionReferencerCreate(       LWError	 *err, LWItemID item );
  XCALL_ (void)             MotionReferencerDestroy(      LWInstance inst);
  XCALL_ (LWError)          MotionReferencerCopy(         LWInstance from, LWInstance to, LWItemID item );
  XCALL_ (LWError)          MotionReferencerLoad(         LWInstance inst, const LWLoadState  *lState);
  XCALL_ (LWError)          MotionReferencerSave(         LWInstance inst, const LWSaveState  *sState );
  XCALL_ (const char *)     MotionReferencerDescribeLine( LWInstance inst );
  XCALL_ (const LWItemID *) MotionReferencerUseItems(     LWInstance inst );
  XCALL_ (void)             MotionReferencerChangeID(     LWInstance inst, const LWItemID *items );

  XCALL_ (int)  MotionReferencer_Activate_56( long version, GlobalFunc *global,
                                              void *local, void *serverData );
  XCALL_ (int)  MotionReferencer_Activate_L6( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  // Plug-In Information for Lightwave's Use
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
      { "ItemMotionHandler",   "TM-P_MotionReferencer--Debug",  MotionReferencer_Activate  },
      { "ItemMotionInterface", "TM-P_MotionReferencer--Debug",  MotionReferencer_Interface },
  #else
      { "ItemMotionHandler",   "TM-P_MotionReferencer",         MotionReferencer_Activate  },
      { "ItemMotionInterface", "TM-P_MotionReferencer",         MotionReferencer_Interface },
  #endif
      { NULL }
  };
}

DynArray< MotionReferencerInstance * > instances;
char                                   error_buffer[1024];

// MotionReferencer_Activate():
XCALL_ (int) MotionReferencer_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  unsigned long prodinfo, major;
  prodinfo = ( unsigned long ) global( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
  major = LWINF_GETMAJOR( prodinfo );

  if ( major < 6 )
    return MotionReferencer_Activate_56( version, global, local, serverData );
  else
    return MotionReferencer_Activate_L6( version, global, local, serverData );
}

// MotionReferencerCreate()
XCALL_ (LWInstance) MotionReferencerCreate( LWError	*err, LWItemID item ) {
  MotionReferencerInstance *inst = new MotionReferencerInstance;

  // Make sure the instance was allocated OK
  if( inst == NULL ) {
    const char *string = "MotionReferencer Error:  Unable to create instance!";
    err = &string;
    return NULL;
  }

  // Set the item used by the instance
  inst->SetItemID( item );

  // Instance OK; return it
  instances.Add( inst );
  err = NULL;
  return inst;
}

// MotionReferencerDestroy()
XCALL_ (void) MotionReferencerDestroy( LWInstance inst ) {
  instances.Remove( instances.FindIndexOf( (MotionReferencerInstance *)inst ) );
  delete inst;
}

// MotionReferencerCopy()
XCALL_ (LWError) MotionReferencerCopy( LWInstance from, LWInstance to,
                                       LWItemID item ) {
  ((MotionReferencerInstance *)to)->CopyFrom(*((MotionReferencerInstance *)from));
  ((MotionReferencerInstance *)to)->SetItemID( item );

  return NULL;
}

// MotionReferencerLoad()
XCALL_ (LWError) MotionReferencerLoad( LWInstance inst,
                                       const LWLoadState *lState) {
  // Check the save mode
  if( lState->ioMode == LWIO_OBJECT )
    return "MotionReferencer Error: MotionReferencer Item Motion Plug-In can only load data from scene files!";

  // Load the data
//  LWLoadState * loader = CLWLoadState::New( lState );
  bool retval = ((MotionReferencerInstance *)inst)->LoadFromScene( error_buffer, lState );
//  delete loader;

  // Return an error, if applicable
  if( retval )
    return NULL;
  else
    return error_buffer;
}

// MotionReferencerSave()
XCALL_ (LWError) MotionReferencerSave( LWInstance inst,
                                       const LWSaveState *sState ) {
  // Check the save mode
  if( sState->ioMode == LWIO_OBJECT )
    return "MotionReferencer Error: MotionReferencer Item Motion Plug-In can only save data into scene files!";

  // Save the data
//  CLWSaveState * saver = CLWSaveState::New( sState );
  bool retval = ((MotionReferencerInstance *)inst)->SaveToScene( error_buffer, sState );
//  delete saver;

  // Return an error, if applicable
  if( retval )
    return NULL;
  else
    return error_buffer;
}

// MotionReferencerDescribeLine()
XCALL_ (const char *) MotionReferencerDescribeLine( LWInstance inst ) {
  static char descline[ 256 ];
  if( ((MotionReferencerInstance *)inst)->GetEnabled() ) {
    sprintf( descline, "TM Productions' MotionReferencer V%d.%d.%d",
             MotionReferencer_Version, MotionReferencer_Revision, MotionReferencer_Subrevision );
  } else {
    sprintf( descline, "TM Productions' MotionReferencer V%d.%d.%d (-disabled-)", MotionReferencer_Version, MotionReferencer_Revision, MotionReferencer_Subrevision );
  }

  return descline;
}

// MotionReferencerUseItems()
XCALL_ (const LWItemID *) MotionReferencerUseItems( LWInstance inst ) {
  return ((MotionReferencerInstance *)inst)->GetItemList();
}

// MotionReferencerChangeID()
XCALL_ (void) MotionReferencerChangeID( LWInstance inst, const LWItemID *items ) {
  ((MotionReferencerInstance *)inst)->ChangeItemIDs( items );
}


// MotionReferencer_Interface():
//  Simplify Object Replacement Plug-In's Interface Function
XCALL_ (int) MotionReferencer_Interface( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  MotionReferencerInstance *mi = (MotionReferencerInstance *) local;

  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  MotionReferencerInterface Panel( global, mi, retval );

  return retval;
}
