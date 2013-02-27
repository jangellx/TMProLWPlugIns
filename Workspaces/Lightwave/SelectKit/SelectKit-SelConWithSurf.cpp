//
// SelectKit-SelConWithSurf.cpp
//

#include "SelectKit.h"

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
    // Select polygons belonging to the selected surfaces
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

  if( !(poly->flags & EDDF_SELECT) ) {
    for( unsigned long i=0; i < surfaces.NumElements(); i++ ) {
      const char *surface = op->polyTag( op->state, poly->pol, LWPTAG_SURF );
      if( surface != NULL ) {
        if( strcmp( surfaces[i], surface ) == 0 )
          return (*op->polSelect)( op->state, poly->pol, 1 );
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
