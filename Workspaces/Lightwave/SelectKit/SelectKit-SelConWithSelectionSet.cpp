//
// SelectKit-SelConWithSelectionSet.cpp
//

#include "SelectKit.h"
#include <lwpanel.h>

DynArray< void * > selection_set_ids;
DynArray< char * > selection_set_names;

LWObjectFuncs *object_funcs = NULL;
int            vmap_count   = 0;

// Context Menu Callbacks
int SelSetCount( void *userdata ) {
  return selection_set_names.NumElements() + 1;
}

char * SelSetName( void *userdata, int index ) {
  if( index == (int)selection_set_names.NumElements() )
    return "< All Sets >";
  else
    return selection_set_names[ index ];
}

// SCWSS_Activate
XCALL_ (int) SCWSS_Activate( long version, GlobalFunc *global,
                             void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Setup
  object_funcs   = (LWObjectFuncs *)global( LWOBJECTFUNCS_GLOBAL, GFUSE_TRANSIENT );
  vmap_count = object_funcs->numVMaps( LWVMAP_PICK );

  // Build a list of selection_set_ids for the selected polygons
  MeshEditOp *op = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  selection_set_ids.Flush();

  EDError err = op->pointScan( op->state, SCWSS_PointScan_FindSets, (void *)op, OPLYR_FG );
  if( err != EDERR_NONE ) {
    op->done( op->state, err, 0 );
  } else {
    // If there are multiple selection sets, ask the user which one(s) they want
    bool do_select = true;
    if( selection_set_ids.NumElements() > 1 ) {
      // Get some globals
      ContextMenuFuncs *context_funcs = (ContextMenuFuncs *)global( LWCONTEXTMENU_GLOBAL, GFUSE_TRANSIENT );
      LWPanelFuncs     *panel_funcs   = (LWPanelFuncs     *)global( LWPANELFUNCS_GLOBAL,  GFUSE_TRANSIENT );

      // Set up te context menu
      LWPanPopupDesc menu_desc;
      menu_desc.type    = LWT_POPUP;
      menu_desc.width   = 200;
      menu_desc.countFn = SelSetCount;
      menu_desc.nameFn  = SelSetName;

      // Set up the panel, open the menu and clean up
      LWPanelID panel = panel_funcs->create( "Selection Sets", panel_funcs );

      LWContextMenuID menu = context_funcs->cmenuCreate( &menu_desc, NULL );
      int index = context_funcs->cmenuDeploy( menu, panel, 0 );
      context_funcs->cmenuDestroy( menu );

      panel_funcs->destroy( panel );

      // Limit to a single selection set or abort, if applicable
      if( index == -1 ) {
        do_select = false;
      } else if( index != (int)selection_set_names.NumElements() ) {
        void *id = selection_set_ids[ index ];
        selection_set_ids.Reset();
        selection_set_ids.Add( id );
      }
    }

    if( do_select ) {
      // Select points belonging to the selected selection sets
      err = op->pointScan( op->state, SCWSS_PointScan_Select, (void *)op, OPLYR_FG );
      op->done( op->state, err, 0 );
    }
  }

  // Clean up
  selection_set_ids.Reset();
  selection_set_names.Flush();

  return CSERR_NONE;
}

// SCWSS_PointScan_Select():
EDError SCWSS_PointScan_Select( void *_op, const EDPointInfo *point ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  float val;
  for( unsigned long i=0; i < selection_set_ids.NumElements(); i++ ) {
    if( !(point->flags & EDDF_SELECT) ) {
      op->pointVSet( op->state, selection_set_ids[i], NULL, NULL );

      if( op->pointVGet( op->state, point->pnt, &val) != 0 ) {
        return (*op->pntSelect)( op->state, point->pnt, 1 );
      }
    }
  }

  return EDERR_NONE;
}

// SCWSS_PointScan_FindSets():
EDError SCWSS_PointScan_FindSets( void *_op, const EDPointInfo *point ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  float val;
  if( point->flags & EDDF_SELECT ) {
    for( int v=0; v < vmap_count; v++ ) {
      const char *set_name = object_funcs->vmapName( LWVMAP_PICK, v );
      void       *set_id   = op->pointVSet( op->state, NULL, LWVMAP_PICK, set_name );

      if( op->pointVGet( op->state, point->pnt, &val) != 0 ) {
        unsigned long i;
        for( i=0; i < selection_set_ids.NumElements(); i++ ) {
          if( selection_set_ids[i] == set_id )
            break;
        }

        if( i == selection_set_ids.NumElements() ) {
          selection_set_ids.Add( set_id );
          selection_set_names.Add( StringTools::strdup( set_name ) );
        }
      }
    }
  }

  return EDERR_NONE;
}

