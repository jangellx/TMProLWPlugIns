//
// SelConWithSurf.cpp
//

#define NULL 0

#include <lwserver.h>
#include <lwcmdseq.h>
#include <lwmeshedt.h>

#include <stdlib.h>

#include <portable/dynarray/dynarray.h>
#include <portable/stringtools/stringtools.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  ServerUserName SCWS_Usernames[]            = { {"TM-P  Select Connected With Surface"DEBUG_PLUGIN_USERNAME,  LANGID_USENGLISH | SRVTAG_USERNAME },  { "Sel Con/Surf"DEBUG_PLUGIN_USERNAME,  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { "pol",  SRVTAG_ENABLE }, { (const char *)NULL } };

  XCALL_ (int) SCWS_Activate( long version, GlobalFunc *global,
                              void *local, void *serverData );

  EDError SCWS_PolyScan_Select(       void *_op, const EDPolygonInfo *poly );
  EDError SCWS_PolyScan_FindSurfaces( void *_op, const EDPolygonInfo *poly );

  ServerRecord ServerDesc[] = {
    { LWMODCOMMAND_CLASS, "TM-P_SelectConnectedWithSurface"DEBUG_PLUGIN_PLUGNAME, SCWS_Activate, SCWS_Usernames },
    { NULL }
  };
}

DynArray< char * > surfaces;

// SCWS_Activate
XCALL_ (int) SCWS_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Build a list of surfaces for the selected polygons
  MeshEditOp *op = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  surfaces.Flush();

  EDError err = op->polyScan( op->state, SCWS_PolyScan_FindSurfaces, (void *)op, OPLYR_FG );
  if( err != EDERR_NONE ) {
    op->done( op->state, err, 0 );
  } else {
    // Select points belonging to the selected polygons
    err = op->polyScan( op->state, SCWS_PolyScan_Select, (void *)op, OPLYR_FG );
    op->done( op->state, err, 0 );
  }

  // Clean up
  surfaces.Flush();

  return CSERR_NONE;
}

// SCWS_PolyScan_Select():
EDError SCWS_PolyScan_Select( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  for( unsigned long i=0; i < surfaces.NumElements(); i++ ) {
    if( !(poly->flags & EDDF_SELECT) ) {
      const char *surface = op->polyTag( op->state, poly->pol, LWPTAG_SURF );
      if( surface != NULL ) {
        for( unsigned long ii=0; i < surfaces.NumElements(); i++ ) {
          if( strcmp( surfaces[i], surface ) == 0 )
            return (*op->polSelect)( op->state, poly->pol, 1 );
        }
      }
    }
  }

  return EDERR_NONE;
}

// SCWS_PolyScan_FindSurfaces():
EDError SCWS_PolyScan_FindSurfaces( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( poly->flags & EDDF_SELECT ) {
    const char *surface = op->polyTag( op->state, poly->pol, LWPTAG_SURF );
    if( surface != NULL ) {
      unsigned long i;
      for( i=0; i < surfaces.NumElements(); i++ ) {
        if( strcmp( surfaces[i], surface ) == 0 )
          break;
      }

      if( i == surfaces.NumElements() )
        surfaces.Add( StringTools::strdup( surface ) );
    }
  }

  return EDERR_NONE;
}

