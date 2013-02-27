//
// MotionReferencer.h
//

extern "C" {
  XCALL_ (LWInstance)       MotionReferencerCreate(       LWError *err, LWItemID item );
  XCALL_ (void)             MotionReferencerDestroy(      LWInstance inst);
  XCALL_ (LWError)          MotionReferencerLoad(         LWInstance inst, const LWLoadState  *lState);
  XCALL_ (LWError)          MotionReferencerSave(         LWInstance inst, const LWSaveState  *sState );
  XCALL_ (const char *)     MotionReferencerDescribeLine( LWInstance inst );
  XCALL_ (const LWItemID *) MotionReferencerUseItems(     LWInstance inst );
  XCALL_ (void)             MotionReferencerChangeID(     LWInstance inst, const LWItemID *items );
}