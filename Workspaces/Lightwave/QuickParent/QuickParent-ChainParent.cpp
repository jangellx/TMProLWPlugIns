//
// QuickParent-ChainParent.cpp
//

#include <stdio.h>

#include <portable/dynarray/dynarray.h>

#include "QuickParent.h"

// Prototypes
int ChainParent( bool first, void *local, GlobalFunc *global );

// ChainParent_First_Activate():
//  Generic plug-in that simply parents items to each other in the
//   order they were selected.  The first item becomes the root parent,
//   and will be the only object selected upon completion.
XCALL_ (int) ChainParent_First_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return ChainParent( true, local, global );
}

// ChainParent_Last_Activate():
//  Generic plug-in that simply parents items to each other in the
//   order they were selected.  The last item becomes the root parent,
//   and will be the only object selected upon completion.
XCALL_ (int) ChainParent_Last_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  return ChainParent( false, local, global );
}


// ChainParent():
//  Actually performs a chaining operation
int ChainParent( bool first, void *local, GlobalFunc *global ) {
  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Get the Interface Global
  LWInterfaceInfo *ui = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  // Make sure at least 2 items are selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "ChainParent Error:  You must have at least two items selected", NULL );
    return AFUNC_OK;
  }

  if( ui->selItems[1] == NULL ) {
    (*message->error)( "Chain Error:  You must have at least two items selected", NULL );
    return AFUNC_OK;
  }

  // Copy the ID array
  DynArray< LWItemID > items;
  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  // If last is first, reverse the array
  if( !first )
    items.Reverse();

  // Cycle through each selected item.  If only 1 item is selected, nothing is done
  char buffer[100];
  for( i=0; i < (items.NumElements() - 1); i++ ) {
    // Step 1:  Select only the item being parented
    sprintf( buffer, "SelectItem %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );

    // Step 2:  Parent the item to the next item
    sprintf( buffer, "ParentItem %x", items[i+1] );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Select the item at the top of the chain
  sprintf( buffer, "SelectItem %x", items[i] );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

