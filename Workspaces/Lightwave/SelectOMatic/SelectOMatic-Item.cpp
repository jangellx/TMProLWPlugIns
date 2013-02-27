//
// SelectOMatic-Item.cpp
//

#include "SelectOMatic-Item.h"
#include <stdio.h>

void * SOM_GROUP        = (void *)-1;
int    next_group_index = 0;

// Constructor
SelectOMatic_Item::SelectOMatic_Item( const char *_name, SelectOMatic_Item *_parent )
  : id( SOM_GROUP ), original_index(next_group_index++), name(NULL), is_selected(false), is_folded(false), parent(_parent) {
  SetName( _name );
}

// Constructor
SelectOMatic_Item::SelectOMatic_Item( LWItemID _id, int _index, SelectOMatic_Item *_parent )
  : id(_id),  original_index(_index), name(NULL), is_selected(false), is_folded(false), parent(_parent) {
  ;
}

// Destructor
SelectOMatic_Item::~SelectOMatic_Item() {
  SetName( NULL );
  children.Flush();
}

// operator=( pointer )
SelectOMatic_Item & SelectOMatic_Item::operator=( SelectOMatic_Item *other_item ) {
  SetID(   other_item->GetID() );
  SetName( other_item->GetName() );

  is_selected    = false;
  is_folded      = false;
  original_index = next_group_index++;

  original_index = other_item->GetOriginalIndex();
  parent         = other_item->GetParent();

  children.Flush();
  SelectOMatic_Item *item;
  for( unsigned long i=0; i < other_item->GetChildren().NumElements(); i++ )
    item = new SelectOMatic_Item( other_item->GetChildren()[i] );

  return *this;
}

// FindSelectedItems():
//  Items only, not groups
void SelectOMatic_Item::FindSelectedItems( DynArray< SelectOMatic_Item * > &array ) {
  if( id != SOM_GROUP ) {
    if( is_selected )
      array.Add( this );

  } else {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->FindSelectedItems( array );
  }
}

// FindSelectedGroups():
//  Adds groups id they are selected, and adds parent groups of selected items.
void SelectOMatic_Item::FindSelectedGroups( DynArray< SelectOMatic_Item * > &array ) {
  if( id == SOM_GROUP ) {
    if( is_selected ) {
      if( array.FindIndexOf( this ) == DA_NO_MATCH )
        array.Add( this );
    }

    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->FindSelectedGroups( array );

  } else {
    if( is_selected ) {
      if( array.FindIndexOf( parent ) == DA_NO_MATCH )
        array.Add( parent );
    }
  }
}

// FindSelected():
//  Find all selected items and groups
void SelectOMatic_Item::FindSelected( DynArray< SelectOMatic_Item * > &array ) {
  if( is_selected )
    array.Add( this );

  if( id == SOM_GROUP ) {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->FindSelected( array );
  }
}

// FindAllItems():
//  Find all items, but not groups
void SelectOMatic_Item::FindAllItems( DynArray< SelectOMatic_Item * > &array ) {
  if( id == SOM_GROUP ) {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->FindAllItems( array );
  } else {
    array.Add( this );
  }
}

// IsDragDropOK():
bool SelectOMatic_Item::IsDragDropOK( SelectOMatic_Item *target ) {
  if( is_selected ) {
    for( SelectOMatic_Item *target_parent = target; target_parent != NULL; target_parent = target_parent->GetParent() ) {
      if( target_parent == this )
        return false;
    }
  }

  if( id == SOM_GROUP ) {
    for( unsigned long i=0; i < children.NumElements(); i++ ) {
      if( !children[i]->IsDragDropOK( target ) )
        return false;
    }
  }

  return true;
}

// BuildDragDropList():
//  Only adds items who's ancestors aren't selected.  Removes items from parent's list.
//   Returns true if the item was added to the list.
bool SelectOMatic_Item::BuildDragDropList( DynArray< SelectOMatic_Item * > &array, bool root ) {
  if( is_selected ) {
    if( id == SOM_GROUP ) {
      array.Add( this );
      return true;

    } else if( !root ) {
      array.Add( this );
      return true;
    }
  }

  if( id == SOM_GROUP ) {
    for( long i=0; i < (long)children.NumElements(); i++ ) {  // long handles i-- later on.  unsigned long would probably work, but hey
      if( children[i]->BuildDragDropList( array, root ) )
        children.Remove( i-- );
    }
  }

  return false;
}

// QSortChildren():
void SelectOMatic_Item::QSortChildren( int (__cdecl *compare )(const void *elem1, const void *elem2 ) ) {
  children.QSort( compare );

  for( unsigned long i=0; i < children.NumElements(); i++ )
    children[i]->QSortChildren( compare );
}

// File I/O Setup
#define SOMITEMIOPRE_ITEM  LWID_( 'I','T','E','M' )
#define SOMITEMIOPRE_ITID  LWID_( 'I','T','I','D' )
#define SOMITEMIOPRE_FOLD  LWID_( 'F','O','L','D' )
#define SOMITEMIOPRE_NAME  LWID_( 'N','A','M','E' )
#define SOMITEMIOPRE_CHLD  LWID_( 'C','H','L','D' )

static enum selectomatic_item_io_ids {
  SOMITEMIOID_ITEM = 0,
  SOMITEMIOID_ITID = 0,
  SOMITEMIOID_FOLD,
  SOMITEMIOID_NAME,
  SOMITEMIOID_CHLD,
};

static const LWBlockIdent selectomatic_item_io_item[] = {
  { SOMITEMIOPRE_ITEM, "Item" },
  0
};

static const LWBlockIdent selectomatic_item_io_root[] = {
  { SOMITEMIOPRE_ITID, "ItemID"    },
  { SOMITEMIOPRE_FOLD, "Folded"    },
  { SOMITEMIOPRE_NAME, "GroupName" },
  { SOMITEMIOPRE_CHLD, "Children"  },
  0
};


// Load():
void SelectOMatic_Item::Load( const LWLoadState *loader ) {
  int  keyword_id;
  char cvalue;
  char buffer[512];

  while( keyword_id = LWLOAD_FIND( loader, selectomatic_item_io_root ) ) {
    switch( keyword_id ) {
      // Item ID
      case SOMITEMIOPRE_ITID:
        LWLOAD_STR( loader, buffer, 512 );
        sscanf( buffer, "%x", &id );
        break;

      // Folded
      case SOMITEMIOPRE_FOLD:
        LWLOAD_I1( loader, &cvalue, 1 );
        is_folded = (cvalue != 0);
        break;

      // Group Name
      case SOMITEMIOPRE_NAME:
        LWLOAD_STR( loader, buffer, 512 );
        SetName( buffer );
        break;

      // Children
      case SOMITEMIOPRE_CHLD: {
        while( keyword_id = LWLOAD_FIND( loader, selectomatic_item_io_item ) ) {
          SelectOMatic_Item *item = new SelectOMatic_Item( NULL, this );
          item->Load( loader );
          children.Add( item );
        }
      }
    }

    LWLOAD_END( loader );            // End Keyword
  }

  LWLOAD_END( loader );              // End ITEM
}

// Save():
void SelectOMatic_Item::Save( const LWSaveState *saver ) {
  char cvalue;
  char buffer[20];

  // Item Base
  LWSAVE_BEGIN( saver, &selectomatic_item_io_item[ SOMITEMIOID_ITEM ], 0 );

  // Item ID
  LWSAVE_BEGIN( saver, &selectomatic_item_io_root[ SOMITEMIOID_ITID ], 1 );
  sprintf( buffer, "%x", id );
  LWSAVE_STR( saver, buffer );
  LWSAVE_END( saver );

  if( id == SOM_GROUP ) {
    // Fold State
    LWSAVE_BEGIN( saver, &selectomatic_item_io_root[ SOMITEMIOID_FOLD ], 1 );
    cvalue = is_folded ? 1 : 0;
    LWSAVE_I1( saver, &cvalue, 1 );
    LWSAVE_END( saver );

    // Group Name
    LWSAVE_BEGIN( saver, &selectomatic_item_io_root[ SOMITEMIOID_NAME ], 1 );
    LWSAVE_STR( saver, ((name == NULL) ? "" : name) );
    LWSAVE_END( saver );

    // Children
    if( children.NumElements() > 0 ) {
      LWSAVE_BEGIN( saver, &selectomatic_item_io_root[ SOMITEMIOID_CHLD ], 0 );
      for( unsigned long i=0; i < children.NumElements(); i++ )
        children[i]->Save( saver );
      LWSAVE_END( saver );
    }
  }

  LWSAVE_END( saver );  // End item
}
