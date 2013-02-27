//
// BatarangHierarchy.h
//
// To generate a hierarchy:
//  - Use BuildHierarchyFromScene()
//
// To get a Natural Order List
//  - Use the item list filled in by BuildHierarchyFromScene()
//
// To get a Hierarchical List
//  - Use BuildItemList()
//  - Use FillInItemDepths() to get the amount of indentation
//     for displaying the list
//  - Use IndexItems() to determine how many times each item is in the scene
//

#ifndef BATARANGHIERARCHY_HEADER
#define BATARANGHIERARCHY_HEADER

#include <string.h>

#include <portable/dynarray/dynarray.h>
#include <portable/dirtools/dirtools.h>

#include "../FreeKey/FreeKey/FreeKeyMotion.h"

// Prototypes
class BatarangHierarchy;

// BatarantHierarchy Item Types (OR these together as needed)
const int BH_ITEM_SCENE       = 0x01;  // Base level "item"
const int BH_ITEM_OBJECT      = 0x02;
const int BH_ITEM_NULL_OBJECT = 0x04;
const int BH_ITEM_BONE        = 0x08;
const int BH_ITEM_LIGHT       = 0x10;
const int BH_ITEM_CAMERA      = 0x20;
const int BH_ITEM_SKELETON    = 0x30;  // Base of the skeleton

const int BH_ITEM_ANY         = BH_ITEM_OBJECT | BH_ITEM_NULL_OBJECT | BH_ITEM_LIGHT | BH_ITEM_CAMERA | BH_ITEM_BONE;

const int BH_DEPTH_ALL        = -1;

// Misc. Constants
const int BH_NO_PARENT        = 0;   // Index of No Parent

// Class BatarangHierarchy
class BatarangHierarchy {
public:
  BatarangHierarchy( const char *name = NULL,
                     int new_type = BH_ITEM_OBJECT,
                     int new_index = 0,
                     int new_parent_index = 0,
                     BatarangHierarchy *new_parent = NULL );
  ~BatarangHierarchy();

  // Static Setup Functions
  static bool         BuildHierarchyFromScene( ifstream &in, BatarangHierarchy &base,
                                               DynArray<BatarangHierarchy *> &item_list,
                                               bool load_motion = false );   // Reads in and builds a hierarchy from a LW Scene.  Returns a pointer to the base of the tree.
  static void         IndexItems( DynArray<BatarangHierarchy *> &item_list );                // Indexes each item with it's instance in the scene (ie: the third Test.lwo in the scene (Test.lwo (3) in Layout) would have an instance index of 3)

  // Matianence Functions
  void                BuildItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones = false, bool add_self = true );                             // Build a list of all items in the item's hierarchy (hierarchical order)
  void                BuildFoldedItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones = false, bool add_self = true  );                      // Build a list of all items in the item's hierarchy (hierarchical order), save for the folded items
  void                BuildGroupItemList( DynArray<BatarangHierarchy *> &item_list, int match_group, bool add_bones = false, bool add_self = true  );      // Build a list of all items in the item's hierarchy that match the group index provided
  void                BuildNaturalItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones = false );                                            // Build a list of all items in the item's hierarchy (natural order by item ID)
  void                BuildSelectedItemList( DynArray<BatarangHierarchy *> &item_list, bool sel = true, bool add_bones = false, bool add_self = true );    // Build a list of all selected or unselected items
  void                OutputAsText( int indent = 0, ostream &out = cerr );                                                                                 // Output the hierarchy tree as text to the stream provided (defaults to cerr)
  void                FillInItemDepths( int this_depth = 0 );                                                                                              // Fills in the depth of the each item in the hierarchy
  void                FreeHierarchy( bool free_self = false );                                                                                             // Frees the entire hierarchy of items

  // Search Functions
  BatarangHierarchy * FindChild( const char *name, int match_type = BH_ITEM_ANY,         // Find a child item by name.  depth of -1 means the whole tree; 0 means only this instance; etc.
                                 int occurance = 1, int depth = -1 );
  BatarangHierarchy * FindIndexInScene( int match_index, int match_type = BH_ITEM_ANY,   // Find a child item by it's index in the scene.  depth of -1 means the whole tree; 0 means only this instance; etc.
                                        int depth = -1 );
  int                 CountOccurances( const char *name, int depth = -1 );               // Find the number of occurances of the item in the hierarchy.

  // Item Name Functions
  inline const char * GetName() { return item_name; }
  inline const char * GetFullName() { return full_item_name; }
  inline void         SetFullName( const char *new_name){ strcpy( full_item_name, new_name );
                                                          item_name = DirStrings::HasFile( full_item_name ); }

  // Item Type Functions
  inline int  GetItemType() { return item_type; }                              // Get the Item's Type (see BH_ITEM_*)
  inline void SetItemType( int new_type ) { item_type = new_type; }            // Set the Item's Type

  // Index In Scene Functions
  inline int  GetIndexInScene() { return index_in_scene; }                     // Get the index of the object in the scene
  inline void SetIndexInScene( int new_index ) { index_in_scene = new_index; } // Set the index of the object in the scene

  // Parent Functions
  inline int                 GetParentIndex() { return parent_object_index; }                             // Get the index of the parent object in the scene
  inline void                SetParentIndex( int new_index ) { parent_object_index = new_index; }         // Set the index of the parent object in the scene
  inline BatarangHierarchy * GetParent() { return parent_object; }                                        // Get the the parent object in the scene
  inline void                SetParent( BatarangHierarchy * new_parent ) { parent_object = new_parent; }  // Set the the parent object in the scene

  BatarangHierarchy *        HasAncestorNamed( const char * match );           // Return a pointer to the ancestor that matches the name passed in
  BatarangHierarchy *        HasAncestorNamed( const char * match, int len );  // Return a pointer to the ancestor that who's first len characters match the name passed in

  // Motion Accessors
  inline FreeKeyMotion &     GetMotion() { return *motion; }

  // Counter functions
  inline int  IncrementTimesFound() { return ++times_found; }
  inline int  GetTimesFound() { return times_found; }

  inline int  GetCopyNumber() { return copy_count; }
  inline void SetCopyNumber( int index ) { copy_count = index; }

         int  CountItems( bool add_self = true, bool add_bones = false );

  // Depth Functions
  inline int  GetDepth() { return depth; }
  inline void SetDepth( int _depth ) { depth = _depth; }

  // UI-Level Accessors
  bool  GetSelected() { return selected; }
  bool  SetSelected( bool state ) { selected = state; }

  bool  GetFolded() { return folded; }
  void  SetFolded( bool state ) { folded = state; }

  int   GetGroup() { return group; }
  int   SetGroup( int index ) { group = index; }

protected:
  // Protected Data Members
  char   full_item_name[256];                     // Item Name, with path
  char * item_name;                               // Item Name only (no path)
  int    item_type;                               // Item Type (see BH_ITEM_*)
  int    index_in_scene;                          // Index of item in the scene.  Starts counting from 1.
  int    parent_object_index;                     // Parent object's index in scene.  Starts counting from 1.

  int    times_found;                             // Number of times the item has been found in the scene (used during increment stage)
  int    copy_count;                              // Index representing which copy of the item this is in the scene (ie: Test.lwo (3) would be 3).

  int    depth;                                   // How many levels deep in the hierarchy this item is (ie: how many ancestors it has)

  BatarangHierarchy * parent_object;              // Parent Object as a BatarangHierarchy

  FreeKeyMotion * motion;                         // Item Motion; may be NULL if motion isn't loaded

  // UI-Level Data Members
  bool selected;                                  // True if this item has been selected
  bool folded;                                    // True if this item has been folded (ie: it's children are hidden)
  int  group;                                     // Group this item belongs to (-1 if it's not in a group)

public:
  // Public Data Memebers
  DynArray<BatarangHierarchy *> children;         // Array of children of this object
  DynArray<BatarangHierarchy *> skeleton;         // Object Skeleton (ie: bones)
  BatarangHierarchy *           skeleton_base;    // Root of the skeleton as a Batarang Tree
};

// QSort Functions
int SortBH_Alpha( const void *elem1, const void *elem2 );

#endif