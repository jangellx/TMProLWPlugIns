//
// QuickParent-Unparent.cpp
//

#include <stdio.h>

#include "QuickParent.h"

#include <portable/dynarray/dynarray.h>

// Unparent_Activate():
//  Generic plug-in that simply unparents  all selected items.
XCALL_ (int) Unparent_Activate( long version, GlobalFunc *global,
                                void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Get the Interface Global
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  // Make sure at least 1 items are selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Unparent Error:  You must have at least one item selected", NULL );
    return AFUNC_OK;
  }

  // Cycle through each selected item.  If only 1 item is selected, nothing is done
  char unparent_command[100];
  sprintf( unparent_command, "ParentItem %x", LWITEM_NULL );

  char buffer[100];
  for( int i = 0; ui->selItems[i] != NULL; i++ ) {
    // Step 1:  Select only the next item to unparent
    sprintf( buffer, "SelectItem %x", ui->selItems[i] );
    (*generic->evaluate)( generic->data, buffer );

    // Step 2:  Unparent the item
    if( (*iteminfo->type)( ui->selItems[i] ) == LWI_BONE ) {  // Special Bone Handling: Unparented bones are parented to their owner object
      LWItemID parent = (*iteminfo->parent)( ui->selItems[i] );
      while( (*iteminfo->type)( parent ) == LWI_BONE )
        parent = (*iteminfo->parent)( parent );

      sprintf( buffer, "ParentItem %x", parent );
      (*generic->evaluate)( generic->data, buffer );

    } else {
      (*generic->evaluate)( generic->data, unparent_command );
    }
  }

  // Re-select the items
  for( i--; i != -1; i-- ) {
    sprintf( buffer, "AddToSelection %x", ui->selItems[i] );
    (*generic->evaluate)( generic->data, buffer );
  }

  return AFUNC_OK;
}

