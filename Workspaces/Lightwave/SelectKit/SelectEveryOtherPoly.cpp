//
// SelectEveryOtherPoly.cpp
//

#include "SelectKit.h"

#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>

#include <lwserver.h>
#include <lwcmdseq.h>
#include <lwmeshedt.h>

#include <stdlib.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  ServerUserName AboutSelectKit_Usernames[]      = { {"TM-P  Abourt SelectKit"                  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "About SelectKit"   DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },                            { (const char *)NULL } };
  ServerUserName SEOP_Usernames_Odd[]            = { {"TM-P  Select Every Other Polygon (Odd)"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Select/Poly (Odd)" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "pol",  SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SEOP_Usernames_Even[]           = { {"TM-P  Select Every Other Polygon (Even)" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Select/Poly (Even)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "pol",  SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SelectPointsOfPolys_Usernames[] = { {"TM-P  Select Points of Polygon"          DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Select Points/Poly"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "spol", SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SelectPolysOfPoints_Usernames[] = { {"TM-P  Select Polygons of Points"         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Select Poly/Points"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "spnt", SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SCWS_Usernames[]                = { {"TM-P  Select With Same Surface"          DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Sel With Surf"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "spol", SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SCWP_Usernames[]                = { {"TM-P  Select With Same Part"             DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Sel With Part"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "spol", SRVTAG_ENABLE }, { (const char *)NULL } };
  ServerUserName SCWSS_Usernames[]               = { {"TM-P  Select With Same Selection Set"    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Sel With Sel Set"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "spnt", SRVTAG_ENABLE }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWMODCOMMAND_CLASS, "TM-P_AboutSelectKit"            DEBUG_PLUGIN_PLUGNAME, AboutSelectKit_Activate,      AboutSelectKit_Usernames      },
    { LWMESHEDIT_CLASS,   "TM-P_SelectEveryOtherPoly_Odd"  DEBUG_PLUGIN_PLUGNAME, SEOP_Activate_Odd,            SEOP_Usernames_Odd            },
    { LWMESHEDIT_CLASS,   "TM-P_SelectEveryOtherPoly_Even" DEBUG_PLUGIN_PLUGNAME, SEOP_Activate_Even,           SEOP_Usernames_Even           },
    { LWMODCOMMAND_CLASS, "TM-P_SelectPolygonsOfPoints"    DEBUG_PLUGIN_PLUGNAME, SelectPointsOfPolys_Activate, SelectPointsOfPolys_Usernames },
    { LWMODCOMMAND_CLASS, "TM-P_SelectPointsOfPolygons"    DEBUG_PLUGIN_PLUGNAME, SelectPolysOfPoints_Activate, SelectPolysOfPoints_Usernames },
    { LWMODCOMMAND_CLASS, "TM-P_SelectWithSurface"         DEBUG_PLUGIN_PLUGNAME, SCWS_Activate,                SCWS_Usernames                },
    { LWMODCOMMAND_CLASS, "TM-P_SelectWithPart"            DEBUG_PLUGIN_PLUGNAME, SCWP_Activate,                SCWP_Usernames                },
    { LWMODCOMMAND_CLASS, "TM-P_SelectWithSelectionSet"    DEBUG_PLUGIN_PLUGNAME, SCWSS_Activate,               SCWSS_Usernames               },
    { NULL }
  };
}

//
// -----------------------------------------------------------
//

const char *about_text[] = {
  LWABOUT_YELLOW,
  "SekectKit",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Modeling Selection Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "AboutSelectKit,",
  "SelectEveryOtherPoly_Odd, SelectEveryOtherPoly_Even,",
  "SelectPolygonsOfPoints, SelectPointsOfPolygons,",
  "SelectConectedWithSurface, SelectConnectedWithPart,",
  "SelectedConnectedWithSelectionSet",
  NULL };

// AboutSelectKit_Activate():
XCALL_ (int) AboutSelectKit_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWPanelFuncs   *panel_funcs = (LWPanelFuncs   *)global( LWPANELFUNCS_GLOBAL,   GFUSE_TRANSIENT );

  LWAboutPanel_TMPro( panel_funcs, "SelectKit", about_text, NULL, true );
  return AFUNC_OK;
}

//
// -----------------------------------------------------------
//

int selection_state = 1;
int selected_polys  = 0;

// Activate Function (Odd)
XCALL_ (int) SEOP_Activate_Odd( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  return SEOP_Activate( version, global, local, serverData, true );
}

// Activate Function (Even)
XCALL_ (int) SEOP_Activate_Even( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  return SEOP_Activate( version, global, local, serverData, false );
}

// Activate Function (Actual)
XCALL_ (int) SEOP_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData,
                            bool def_state ) {
  
  selection_state = def_state;

  if ( version != LWMESHEDIT_VERSION )
    return AFUNC_BADVERSION;

  MeshEditOp * op = ((MeshEditBegin *)local)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  if( op == NULL )
    return AFUNC_BADLOCAL;

  selected_polys = op->polyCount( op->state, OPLYR_FG, EDCOUNT_SELECT );
  EDError err = op->polyScan( op->state, SEOP_PolyScan, (void *)op, OPLYR_FG );
  op->done( op->state, err, 0 );

  return AFUNC_OK;
}

// SEOP_PolyScan Function():
EDError SEOP_PolyScan( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( (poly->flags & EDDF_SELECT) || (selected_polys == 0) ) {
    selection_state = !selection_state;
    return (*op->polSelect)( op->state, poly->pol, selection_state );
  }

  return EDERR_NONE;
}

//
// -----------------------------------------------------------
//

// SelectPointsOfPolys_Activate():
XCALL_ (int) SelectPointsOfPolys_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Clear the selection
  (*command->evaluate)( command->data, "SEL_POINT CLEAR" );

  // Select points belonging to the selected polygons
  MeshEditOp * op = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  EDError err = op->polyScan( op->state, SelectPointsOfPolys_PolyScan, (void *)op, OPLYR_FG );
  op->done( op->state, err, 0 );

  return AFUNC_OK;
}

// SelectPointsOfPolys_PolyScan():
EDError SelectPointsOfPolys_PolyScan( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( poly->flags & EDDF_SELECT ) {
    for( int i=0; i < poly->numPnts; i++ )
      op->pntSelect( op->state, poly->points[i], 1 );
  }

  return EDERR_NONE;
}

// SelectPolyOsfPoints_Activate():
XCALL_ (int) SelectPolysOfPoints_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Clear the selection
  (*command->evaluate)( command->data, "SEL_POLYGON CLEAR" );

  // Select polygons belonging to the selected points
  MeshEditOp * op = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  EDError err = op->polyScan( op->state, SelectPolysOfPoints_PolyScan, (void *)op, OPLYR_FG );
  op->done( op->state, err, 0 );

  return EDERR_NONE;
}

// SelectPolysOfPoints_PolyScan():
EDError SelectPolysOfPoints_PolyScan( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  EDPointInfo * point;
  for( int i=0; i < poly->numPnts; i++ ) {
    point = op->pointInfo( op->state, poly->points[i] );
    if( point->flags & EDDF_SELECT )
      return (*op->polSelect)( op->state, poly->pol, selection_state );
  }

  return EDERR_NONE;
}
