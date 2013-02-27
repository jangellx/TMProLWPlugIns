//
// SchemFlow-Item.h
//

#define NULL 0   // Avoids LW SDK setting this to a void *

#include <lwrender.h>
#include <lwgeneric.h>

#include <portable/dynarray/dynarray.h>

// SchemFlow_Item
class SchemFlow_Item {
public:
  SchemFlow_Item( LWItemID _id, SchemFlow_Item *_parent = NULL );
  ~SchemFlow_Item();

  void                           BuildChildList( LWItemInfo *iteminfo );
  double                         CountLeafs();

  LWItemID                       GetID() { return id; }
  SchemFlow_Item               * GetParent() { return parent; }
  DynArray< SchemFlow_Item *>  & GetChildren() { return children; }
  double                         GetLeafCount() { return leaf_count; }

  void                           ArrangeChildren( LWInterfaceInfo * ui, LWLayoutGeneric * generic,
                                                  double parent_x = 0.0, double parent_y = 0.0 );

protected:
  LWItemID                       id;
  SchemFlow_Item               * parent;
  DynArray< SchemFlow_Item * >   children;
  double                         leaf_count;      // Number of decendants without children.  1 means purly vertical stack, with each extra item adding another .5
};

