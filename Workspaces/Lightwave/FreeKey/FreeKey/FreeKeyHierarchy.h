//
// FreeKeyHierarchy.h
//
//
// This contains the class definition for FreeKeyHierarchy, which adds some
//  FreeKey-specific stuff to BatarangHierarchy.
//

#include <fstream.h>

#include "../../Batarang/BatarangHierarchy.h"

class FreeKeyHierarchy;

// Class FreeKeyHierarchy
class FreeKeyHierarchy : public BatarangHierarchy {
public:
  FreeKeyHierarchy( const char *name = NULL,
                    int new_type = BH_ITEM_OBJECT,
                    int new_index = 0,
                    int new_parent_index = 0,
                    BatarangHierarchy *new_parent = NULL )
    : BatarangHierarchy( name, new_type, new_index, new_parent_index, new_parent ),
      selected(false), folded(false), group(-1) { ; }

  // Static Setup Functions
  static bool BuildHierarchyFromScene( ifstream &in, FreeKeyHierarchy &base,
                                       DynArray<FreeKeyHierarchy *> &item_list );   // Reads in and builds a hierarchy from a LW Scene.  Returns a pointer to the base of the tree.
  static void IndexItems( DynArray<FreeKeyHierarchy *> &item_list );               // Indexes each item with it's instance in the scene (ie: the third Test.lwo in the scene (Test.lwo (3) in Layout) would have an instance index of 3)

  // Matianence Functions
  void        BuildItemList( DynArray<FreeKeyHierarchy *> &item_list, bool add_bones = false, bool add_self = true );  // Build a list of all items in the item's hierarchy

  // Accessors
  bool  GetSelected() { return selected; }
  bool  SetSelected( bool state ) { selected = state; }

  bool  GetFolded() { return folded; }
  bool  SetFolded( bool state ) { folded = state; }

  int   GetGroup() { return group; }
  int   SetGroup( int index ) { group = index; }

protected:
  bool selected;                  // True if this item has been selected
  bool folded;                    // True if this item has been folded (ie: it's children are hidden)
  int  group;                     // Group this item belongs to (-1 if it's not in a group)
};

