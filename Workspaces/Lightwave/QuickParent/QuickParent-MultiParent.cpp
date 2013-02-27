//
// QuickParent-MultiParent.cpp
//

#include <stdio.h>

#include <portable/dynarray/dynarray.h>

#include "QuickParent.h"

// Prototypes
int MultiParent( bool first, void *local, GlobalFunc *global );

// MultiParent_First_Activate():
//  Generic plug-in that simply parents items to the first item
//   selected.  The parent will be the only object selected upon
//   completion.
XCALL_ (int) MultiParent_First_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return MultiParent( true, local, global );
}

// MultiParent_Last_Activate():
//  Generic plug-in that simply parents items to the last item
//   selected.  The parent will be the only object selected upon
//   completion.
XCALL_ (int) MultiParent_Last_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return MultiParent( false, local, global );
}


// MultiParent():
//  Actually performs a multi-parenting operation
int MultiParent( bool first, void *local, GlobalFunc *global ) {
  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Get the Interface Global
  LWInterfaceInfo *ui = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  // Make sure at least 2 items are selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "MultiParent Error:  You must have at least two items selected", NULL );
    return AFUNC_OK;
  }

  if( ui->selItems[1] == NULL ) {
    (*message->error)( "MultiParent Error:  You must have at least two items selected", NULL );
    return AFUNC_OK;
  }

  // Copy the ID array
  DynArray< LWItemID > items;
  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  // If last is first, swap the first and last items
  if( first )   // I don't know why, but !first winds up being backward.  Go fig.
    items.Swap( 0, items.NumElements()-1 );

  // Cycle through each selected item.  If only 1 item is selected, nothing is done
  char parent_command[100];
  sprintf( parent_command, "ParentItem %x", items[0] );

  char buffer[100];
  for( i=1; i < items.NumElements(); i++ ) {
    // Step 1:  Select only the item being parented
    sprintf( buffer, "SelectItem %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );

    // Step 2:  Parent the item to master parent
    (*generic->evaluate)( generic->data, parent_command );
  }

  // Select the item at the top of the chain
  sprintf( buffer, "SelectItem %x", items[0] );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}
