//
// QuickParent-InsertIntoHierarchy.cpp
//

#include "QuickParent.h"
#include <stdio.h>

// InsertIntoHierachy_Activate():
//  Generic plug-in that inserts the selected item into an existing
//   hierarchy.
XCALL_ (int) InsertIntoHierarchy_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Get the Interface Global
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  // Make sure at least 2 items are selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Insert Into Hierarchy Error:  You must have two items selected", NULL );
    return AFUNC_OK;
  }

  if( ui->selItems[1] == NULL ) {
    (*message->error)( "Insert Into Hierarchy Error:  You must have two items selected", NULL );
    return AFUNC_OK;
  }

  // Confirm the insertion
  char buffer[ 1024 ], buffer2[ 1024 ];
  sprintf( buffer, "Do you want to insert \"%s\" in between", (*iteminfo->name)( ui->selItems[1] ) );
  sprintf( buffer2, "\"%s\" and it's children?", (*iteminfo->name)( ui->selItems[0] ) );
  if( (*message->yesNo)( "Insert Into Hierarchy?", buffer, buffer2 ) == 0 )
    return AFUNC_OK;

  // Reparent the children
  for( LWItemID child = (*iteminfo->firstChild)( ui->selItems[0] ); child != NULL; /*NULL*/ ) {
    sprintf( buffer, "SelectItem %x", child );
    (*generic->evaluate)( generic->data, buffer );

    // Get the next child now, since it won't be a child for long
    child = (*iteminfo->nextChild)( ui->selItems[0], child );

    sprintf( buffer, "ParentItem %x", ui->selItems[1] );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Parent the object to it's new parent
  sprintf( buffer, "SelectItem %x", ui->selItems[1] );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "ParentItem %x", ui->selItems[0] );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// RemoveFromHierachy_Activate():
//  Generic plug-in that removes the selected item from an existing
//   hierarchy.
XCALL_ (int) RemoveFromHierarchy_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Get the Interface Global
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  // Make sure at least 2 items are selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Remove From Hierarchy Error:  You must have an item selected", NULL );
    return AFUNC_OK;
  }

  // Check for a parent
  LWItemID parent = (*iteminfo->parent)( ui->selItems[0] );
  if( parent == LWITEM_NULL ) {
    (*message->error)( "Remove From Hierarchy Error:  Item has no parent; cannot remove from hierarchy without one", NULL );
    return AFUNC_OK;
  }

  // Confirm the removal
  char buffer[ 1024 ], buffer2[ 1024 ];
  sprintf( buffer, "Do you want to remove \"%s\" from in between", (*iteminfo->name)( ui->selItems[1] ) );
  sprintf( buffer2, "\"%s\" and it's children?", (*iteminfo->name)( parent ) );
  if( (*message->yesNo)( "Remove From Hierarchy?", buffer, buffer2 ) == 0 )
    return AFUNC_OK;

  // Unparent the object
  sprintf( buffer, "SelectItem %x", ui->selItems[0] );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "ParentItem %x", LWITEM_NULL );
  (*generic->evaluate)( generic->data, buffer );

  // Reparent the children
  for( LWItemID child = (*iteminfo->firstChild)( ui->selItems[0] ); child != NULL; /*NULL*/ ) {
    sprintf( buffer, "SelectItem %x", child );
    (*generic->evaluate)( generic->data, buffer );

    // Get the next child now, since it won't be a child for long
    child = (*iteminfo->nextChild)( ui->selItems[0], child );

    sprintf( buffer, "ParentItem %x", parent );
    (*generic->evaluate)( generic->data, buffer );
  }

  return AFUNC_OK;
}

