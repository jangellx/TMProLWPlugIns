//
// Motify-FromLWItemHierarchy-Item.cpp
//

#include <stdio.h>
#include <iomanip.h>

#include "Motify-FromLWItemHierarchy.h"

// Constructor
LWItemHierarchy::LWItemHierarchy( LWItemID _id, LWItemHierarchy *_parent, LWItemHierarchy *_owner )
  : id(_id), parent(_parent), owner(_owner), is_selected(false), type(LWI_SCENE) {
  ;
}

// Destructor
LWItemHierarchy::~LWItemHierarchy() {
  children.Flush();
//  skeleton.Flush();
}

// BuildChildList():
//  Scans the entire Lightwave item list and builds a hierarchy
void LWItemHierarchy::BuildChildList( LWItemInfo *iteminfo ) {
  assert( iteminfo != NULL );

  if( owner == NULL ) {
    // Base Item: Build the unparented item list
    type = LWI_SCENE;
    LWItemType item_types[3] = { LWI_OBJECT, LWI_LIGHT, LWI_CAMERA };

    // Build a list of all unparented items
    LWItemID current_item;
    unsigned long i;
    for( i=0; i < 3; i++ ) {
      current_item = (*iteminfo->first)( item_types[i], LWITEM_NULL );
      while( current_item != LWITEM_NULL ) {
        if( (*iteminfo->parent)( current_item ) == LWITEM_NULL )
          children.Add( new LWItemHierarchy( current_item, NULL, this ) );

        current_item = (*iteminfo->next)( current_item );
      }
    }

    // Build a hierarchy list
    for( i=0; i < children.NumElements(); i++ )
      children[i]->BuildChildList( iteminfo );

  } else {
    // Get the item type
    type = (iteminfo->type)( id );

    // Child Item: Build the child list
    LWItemID current_child = (*iteminfo->firstChild)( id );
    LWItemHierarchy *new_child;
    LWItemHierarchy *new_owner;
    while( current_child != LWITEM_NULL ) {
      if( (type == LWI_OBJECT) && ((iteminfo->type)( current_child ) == LWI_BONE) )
        new_owner = this;
      else
        new_owner = owner;
        
      new_child =  new LWItemHierarchy( current_child, this, new_owner );
      children.Add( new_child );

      new_child->BuildChildList( iteminfo );

      current_child = (*iteminfo->nextChild)( id, current_child );
    }
  }
}

// FindChildByID():
//  Walks the hierarchy and returns a pointer to the item with the ID specified,
//   or NULL if the item cannot be found
LWItemHierarchy * LWItemHierarchy::FindChildByID( LWItemID _id ) {
  if( _id == GetID() )
    return this;

  LWItemHierarchy *temp;
  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    temp = children[i]->FindChildByID( _id );
    if( temp != NULL )
      return temp;
  }

  return NULL;
}


// ReadLayoutSelection():
//  Marks an object as being selected if it is selected in Layout
void LWItemHierarchy::ReadLayoutSelection( LWItemInfo *iteminfo, LWInterfaceInfo *ui, bool current_only ) {
  is_selected = false;

  // See if this item is selected
  if( ui->selItems[0] != NULL ) {
    if( (iteminfo->type)( ui->selItems[0] ) == type ) {      // Consider item unselected if the item types don't match
      for( unsigned long i=0; ui->selItems[i] != NULL; i++ ) {
        if( ui->selItems[i] == id ) {
          is_selected = true;
          break;
        }

        if( current_only )
          break;
      }
    }
  }

  // Test Children
  for( unsigned long i=0; i < children.NumElements(); i++ )
    children[i]->ReadLayoutSelection( iteminfo, ui );
}

// OutputToStream():
void LWItemHierarchy::OutputToStream( ofstream &out, LWItemInfo *iteminfo, int indent ) {
  out << setw( indent ) << "" << setiosflags( ios::left );

  out << setw( 50 - indent ) << ((*iteminfo->name)( id ));

  switch( type ) {
    case LWI_SCENE:    out << setw( 20 ) << "Scene Root"; break;
    case LWI_OBJECT:   out << setw( 20 ) << "Object"    ; break;
    case LWI_LIGHT:    out << setw( 20 ) << "Light"     ; break;
    case LWI_CAMERA:   out << setw( 20 ) << "Camera"    ; break;
    case LWI_BONE:     out << setw( 20 ) << "Bone"      ; break;
    default:           out << setw( 20 ) << "(unknown)" ; break;
  }

  out << setw( 10 ) << (is_selected ? "Selected" : "");
  out << setw( 15 ) << id;
  out << setw( 15 ) << ((owner == NULL) ? 0 : owner->GetID() );
  out << endl;

  for( unsigned long i=0; i < children.NumElements(); i++ )
    children[i]->OutputToStream( out, iteminfo, indent + 2 );
}

// SelectAll():
// Selects all items and their children
void LWItemHierarchy::SelectAll() {
  if( type != LWI_SCENE )
    SetIsSelected( true );

  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    children[i]->SetIsSelected( true );
    children[i]->SelectChildren( true );
  }
}

// SelectSelectedsChildren():
//  Selects the children of the currently selected items.
//   If recursive is true, the children's children will
//   also be selected. Internal only; does NOT affect Layout
void LWItemHierarchy::SelectSelectedsChildren( bool recursive ) {
  if( parent != NULL ) {
    if( parent->GetType() != LWI_SCENE ) {
      if( parent->GetIsSelected() )
        SetIsSelected( true );
    }
  }

  if( recursive ) {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->SelectSelectedsChildren( recursive );
  }
}

// SelectChildren():
//  Selects the children of this item.  If recursive is
//   true, the children's children will also be selected.
//   Internal only; does NOT affect Layout
void LWItemHierarchy::SelectChildren( bool recursive ) {
  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    children[i]->SetIsSelected( true );

    if( recursive )
      children[i]->SelectChildren( recursive);
  }
}

// SelectSelectedsBones():
//  Selects the bones of the currently selected items.
//   If recursive is true, the bones's children will
//   also be selected. Internal only; does NOT affect Layout
void LWItemHierarchy::SelectSelectedsBones( bool recursive ) {
  if( type == LWI_BONE ) {
    if( parent != NULL ) {
      if( (parent->GetType() != LWI_BONE) || recursive ) {  // Only select child bones if recursive is on
        if( parent->GetIsSelected() )
          SetIsSelected( true );
      }
    }
  }

  if( recursive ) {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      children[i]->SelectSelectedsBones( recursive );
  }
}

// SelectBones():
//  Selects the bones of this item.  If recursive is
//   true, the bone's children will also be selected.
//   Internal only; does NOT affect Layout
void LWItemHierarchy::SelectBones( bool recursive ) {
  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    if( children[i]->GetType() == LWI_BONE ) {
      children[i]->SetIsSelected( true );

      if( recursive )
        children[i]->SelectBones( recursive );
    }
  }
}

// CountSelected():
//  Returns the number of selected items
unsigned long LWItemHierarchy::CountSelected() {
  unsigned long count = is_selected ? 1 : 0;
  for( unsigned long i=0; i < children.NumElements(); i++ )
    count += children[i]->CountSelected();

  return count;
}

// AreAnyChildrenBones():
//  Returns true if any children are bones.
bool LWItemHierarchy::AreAnyChildrenBones() {
  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    if( children[i]->GetType() == LWI_BONE )
      return true;
  }

  return false;
}

