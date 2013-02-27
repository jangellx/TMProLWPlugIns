//
// Motify-FromLWItemHierarchy.h
//
// This class builds and maintains a tree of items in hierarchical
//  form from the data provided by Lightwave.  It is designed for
//  use from within plug-ins, specifically those written for L6 or
//  later.
//  
// Note that bones are just listed as children of an object they.
//  belong to.  They are not special cased.
//

#define NULL 0                      // Avoids LW SDK setting this to a void *
#define LWI_SCENE  (LWI_OBJECT - 1) // Special marker for the scene base item

#include <lwrender.h>
#include <lwgeneric.h>

#include <fstream.h>

#include "Motify-FromDynArray.h"

// LWItemHierarchy
class LWItemHierarchy {
public:
  LWItemHierarchy( LWItemID _id, LWItemHierarchy *_parent = NULL,
                                 LWItemHierarchy *_owner  = NULL );
  ~LWItemHierarchy();

  // Hierarchy Functions
  void                            BuildChildList( LWItemInfo *iteminfo );

  void                            OutputToStream( ofstream &out, LWItemInfo *iteminfo, int indent = 0 );

  LWItemHierarchy               * FindChildByID( LWItemID _id );

  // Accessors
  LWItemID                        GetID() { return id; }
  LWItemType                      GetType() { return type; }
  LWItemHierarchy               * GetParent() { return parent; }
  LWItemHierarchy               * GetOwner() { return owner; }
  DynArray< LWItemHierarchy *>  & GetChildren() { return children; }
//  DynArray< LWItemHierarchy *>  & GetSkeleton() { return skeleton; }

  // Selection Functions
  void                            ReadLayoutSelection( LWItemInfo *iteminfo, LWInterfaceInfo *ui, bool current_only = false );
  void                            WriteLayoutSelection();

  void                            SelectAll();                               // Internal only; does NOT affect Layout
  void                            SelectCurrentItem(       bool recursive ); // Internal only; does NOT affect Layout
  void                            SelectChildren(          bool recursive ); // Internal only; does NOT affect Layout
  void                            SelectBones(             bool recursive ); // Internal only; does NOT affect Layout
  void                            SelectSelectedsChildren( bool recursive ); // Internal only; does NOT affect Layout
  void                            SelectSelectedsBones(    bool recursive ); // Internal only; does NOT affect Layout

  bool                            GetIsSelected() { return is_selected; }
  void                            SetIsSelected( bool state ) { is_selected = state; }

//  bool                            GetIsCleared() { return is_cleared; }
//  void                            SetIsCleared( bool state ) { is_cleared = state; }

  unsigned long                   CountSelected();

  bool                            AreAnyChildrenBones();

protected:
  LWItemID                        id;              // LW Item ID
  LWItemType                      type;            // Item Type
  LWItemHierarchy               * parent;          // Parent Item
  LWItemHierarchy               * owner;           // Bone's Owner (bones only); object the bone belongs to

  DynArray< LWItemHierarchy * >   children;        // Child Items
//  DynArray< LWItemHierarchy * >   skeleton;        // Skeleton (objects only)

  bool                            is_selected;     // true if the item is selected in Layout
//  bool                            is_cleared;      // true if the item has been cleared from the scene; Internal only
};

