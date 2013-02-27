//
// SchemFlow-Item.cpp
//

#include <stdio.h>

#include "schemflow-item.h"

// Constructor
SchemFlow_Item::SchemFlow_Item( LWItemID _id, SchemFlow_Item *_parent )
  : id(_id), parent(_parent), leaf_count(0) {
  ;
}

// Destructor
SchemFlow_Item::~SchemFlow_Item() {
  children.Flush();
}

// BuildChildList():
void SchemFlow_Item::BuildChildList( LWItemInfo *iteminfo ) {
  LWItemID current_child = (*iteminfo->firstChild)( id );
  SchemFlow_Item *new_child;
  while( current_child != LWITEM_NULL ) {
    new_child =  new SchemFlow_Item( current_child, this );
    children.Add( new_child );

    new_child->BuildChildList( iteminfo );

    current_child = (*iteminfo->nextChild)( id, current_child );
  }
}

// CountLeafs():
double SchemFlow_Item::CountLeafs() {
  leaf_count = 0.0;

  if( children.NumElements() == 0 ) {
    leaf_count += 1.0;
  } else {
    for( unsigned long i=0; i < children.NumElements(); i++ )
      leaf_count += children[i]->CountLeafs();
  }

  return leaf_count;
}

// ArrangeChildren():
//  Actually moves around items in the schematic view.
void SchemFlow_Item::ArrangeChildren( LWInterfaceInfo * ui, LWLayoutGeneric * generic,
                                      double parent_x, double parent_y ) {
  char buffer[ 256 ];
  const double item_width  = 2.0;
  const double item_height = 1.0;

//  double parent_x = 0.0;
//  double parent_y = 0.0;
//  if( parent != NULL )
//    (*ui->schemaPos)( parent->GetID(), &parent_x, &parent_y );

  // Move each child
  double prev_weight = 0.0;
  double weight;
  double x, y;
  for( unsigned long i=0; i < children.NumElements(); i++ ) {
    // Select the child
    sprintf( buffer, "SelectItem %x", children[i]->GetID() );
    (*generic->evaluate)( generic->data, buffer );

    weight = children[i]->GetLeafCount() / leaf_count;
    x = (prev_weight * item_width) + ((weight/2) * item_width) * 1.5;

    // Compute the child's position
    y = parent_y - item_height;

    // Move the item
    sprintf( buffer, "SchematicPosition %g %g", x, y );
    (*generic->evaluate)( generic->data, buffer );

    prev_weight += weight;

    children[i]->ArrangeChildren( ui, generic, x, y );
  }
}

