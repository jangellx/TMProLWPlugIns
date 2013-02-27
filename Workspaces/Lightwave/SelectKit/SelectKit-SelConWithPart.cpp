//
// SelectKit-SelConWithSurf.cpp
//

#include "SelectKit.h"

DynArray< char * > parts;

// SCWP_Activate
XCALL_ (int) SCWP_Activate( long version, GlobalFunc *global,
                            void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Build a list of parts for the selected polygons
  MeshEditOp *op = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  parts.Flush();

  EDError err = op->polyScan( op->state, SCWP_PolyScan_FindParts, (void *)op, OPLYR_FG );
  if( err != EDERR_NONE ) {
    op->done( op->state, err, 0 );
  } else {
    // Select polygons belonging to the selected parts
    err = op->polyScan( op->state, SCWP_PolyScan_Select, (void *)op, OPLYR_FG );
    op->done( op->state, err, 0 );
  }

  // Clean up
  parts.Flush();

  return CSERR_NONE;
}

// SCWP_PolyScan_Select():
EDError SCWP_PolyScan_Select( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( !(poly->flags & EDDF_SELECT) ) {
    for( unsigned long i=0; i < parts.NumElements(); i++ ) {
      const char *part = op->polyTag( op->state, poly->pol, LWPTAG_PART );
      if( part != NULL ) {
        if( strcmp( parts[i], part ) == 0 )
          return (*op->polSelect)( op->state, poly->pol, 1 );
      }
    }
  }

  return EDERR_NONE;
}

// SCWP_PolyScan_FindParts():
EDError SCWP_PolyScan_FindParts( void *_op, const EDPolygonInfo *poly ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( poly->flags & EDDF_SELECT ) {
    const char *part = op->polyTag( op->state, poly->pol, LWPTAG_PART );
    if( part != NULL ) {
      unsigned long i;
      for( i=0; i < parts.NumElements(); i++ ) {
        if( strcmp( parts[i], part ) == 0 )
          break;
      }

      if( i == parts.NumElements() )
        parts.Add( StringTools::strdup( part ) );
    }
  }

  return EDERR_NONE;
}
