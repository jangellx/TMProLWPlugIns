//
// SelectKit.h
//

#define NULL 0

#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>

#include <lwserver.h>
#include <lwcmdseq.h>
#include <lwmeshedt.h>

#include <stdlib.h>

XCALL_ (int) AboutSelectKit_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

XCALL_ (int) SEOP_Activate_Odd( long version, GlobalFunc *global,
                                void *local, void *serverData );
XCALL_ (int) SEOP_Activate_Even( long version, GlobalFunc *global,
                                 void *local, void *serverData );
XCALL_ (int) SEOP_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData,
                            bool def_state );

XCALL_ (int) SelectPointsOfPolys_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
XCALL_ (int) SelectPolysOfPoints_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
XCALL_ (int) SCWS_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData );
XCALL_ (int) SCWP_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData );
XCALL_ (int) SCWSS_Activate( long version, GlobalFunc *global,
                             void *local, void *serverData );

EDError SEOP_PolyScan( void *_op, const EDPolygonInfo *poly );

EDError SelectPointsOfPolys_PolyScan( void *_op, const EDPolygonInfo *poly );
EDError SelectPolysOfPoints_PolyScan( void *_op, const EDPolygonInfo *poly );

EDError SCWS_PolyScan_Select(         void *_op, const EDPolygonInfo *poly );
EDError SCWS_PolyScan_FindSurfaces(   void *_op, const EDPolygonInfo *poly );

EDError SCWP_PolyScan_Select(         void *_op, const EDPolygonInfo *poly );
EDError SCWP_PolyScan_FindParts(      void *_op, const EDPolygonInfo *poly );

EDError SCWSS_PointScan_Select(       void *_op, const EDPointInfo *point );
EDError SCWSS_PointScan_FindSets(     void *_op, const EDPointInfo *point );

