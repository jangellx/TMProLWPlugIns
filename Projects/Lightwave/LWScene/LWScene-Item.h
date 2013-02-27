//
// LWScene-Item.h
//
// This contains what used to be BatarangHierarchy, and handles the
//  loading and saving of items to and from a scene file, as well
//  as a number of item-level and hierarchy-level management functions.
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

#ifndef LWITEM_HEADER
#define LWITEM_HEADER

#include <string.h>

#include <portable/pfstream/pfstream.h>
#include <portable/dynarray/dynarray.h>
#include <portable/dirtools/dirtools.h>

#include "LWScene-Motion.h"

// Prototypes
class LWItem;
// BatarantHierarchy Item Types (OR these together as needed)
const int LWI_ITEM_SCENE       = 0x01;  // Base level "item"
const int LWI_ITEM_SKELETON    = 0x02;  // Base of the skeleton
const int LWI_ITEM_OBJECT      = 0x04;
const int LWI_ITEM_NULL_OBJECT = 0x08;
const int LWI_ITEM_BONE        = 0x10;
const int LWI_ITEM_LIGHT       = 0x20;
const int LWI_ITEM_CAMERA      = 0x40;

const int LWI_NON_ITEMS        = LWI_ITEM_SCENE | LWI_ITEM_SKELETON;
const int LWI_ITEM_ANY         = LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT | LWI_ITEM_LIGHT | LWI_ITEM_CAMERA | LWI_ITEM_BONE;

const int LWI_DEPTH_ALL        = -1;

enum LWIDrawMode {
  LWIDRAW_NONE                 = 0,
  LWIDRAW_BOUNDING_BOX,
  LWIDRAW_POINTS_ONLY,
  LWIDRAW_PARTIAL_WIREFRAME,
  LWIDRAW_WIREFRAME,
  LWIDRAW_FRONTFACE_WIREFRAME,
  LWIDRAW_PARTIAL_SOLID,
  LWIDRAW_SOLID,
  LWIDRAW_TEXTURED,
  NUM_LWI_DRAW_MODES};

enum LWIDrawColor {
  LWIDRAW_GREY                 = 0,
  LWIDRAW_GRAY                 = 0,
  LWIDRAW_BLUE,
  LWIDRAW_GREEN,
  LWIDRAW_CYAN,
  LWIDRAW_RED,
  LWIDRAW_PURPLE,
  LWIDRAW_ORANGE,
  NUM_LWI_DRAW_COLORS};

// Misc. Constants
static const int LWI_NO_PARENT        = 0;   // Index of Parent if there is no parent

// Class LWItem
class LWItem {
public:
  LWItem( const char *name = NULL,
          int new_type = LWI_ITEM_OBJECT,
          int new_index = 0,
          int new_parent_index = 0,
          LWItem *new_parent = NULL );
  ~LWItem();

  // Static Setup Functions
  static int          LoadItemFromScene( pifstream &in, const char *type, int index, int mode,
                                         int version, LWItem *&item );            // Loads an item from the input stream provided.
  static void         IndexItems( DynArray<LWItem *> &item_list );                // Indexes each item with it's instance in the scene (ie: the third Test.lwo in the scene (Test.lwo (3) in Layout) would have an instance index of 3)
  static bool         BuildHierarchyFromScene( ifstream &in, LWItem &base,
                                               DynArray<LWItem *> &item_list,
                                               bool load_motion );

  static void FillInItemCloneNumber( DynArray< LWItem * > &item_list );           // Sets the item's copy count (-1 for only 1 instance of the item)

  // Matianence Functions
  void                BuildItemList( DynArray<LWItem *> &item_list, bool add_bones = false, bool add_self = true );                                    // Build a list of all items in the item's hierarchy (hierarchical order)
  void                BuildFoldedItemList( DynArray<LWItem *> &item_list, bool add_bones = false, bool add_self = true  );                             // Build a list of all items in the item's hierarchy (hierarchical order), save for the folded items
  void                BuildNaturalItemList( DynArray<LWItem *> &item_list, bool add_bones = false );                                                   // Build a list of all items in the item's hierarchy (natural order by item ID)
  void                BuildSelectedItemList( DynArray<LWItem *> &item_list, bool sel = true, bool add_bones = false, bool add_self = true );           // Build a list of all selected or unselected items
  void                BuildGroupItemList( DynArray<LWItem *> &item_list, unsigned long  match_group, bool add_bones = false, bool add_self = true  );  // Build a list of all items in the item's hierarchy that match the group index provided

  void                OutputAsText( int indent = 0, ostream &out = cerr, bool output_motion = false );                                          // Output the hierarchy tree as text to the stream provided (defaults to cerr)
  void                FillInItemDepths( int this_depth = 0 );                                                                                   // Fills in the depth of the each item in the hierarchy

  void                FreeHierarchy();                                                                                                          // Frees the entire hierarchy of items, except the calling item
  void                ClearSkeleton();
  void                InitSkeleton();

  // Search Functions
  LWItem *            FindChild( const char *name, int match_type = LWI_ITEM_ANY,         // Find a child item by name.  depth of -1 means the whole tree; 0 means only this instance; etc.
                                 int occurance = 1, int depth = -1 );
  LWItem *            FindIndexInScene( int match_index, int match_type,                  // Find a child item by it's type and index in the scene.  depth of -1 means the whole tree; 0 means only this instance; etc.
                                        int depth = -1 );
  int                 CountOccurances( const char *name, int depth = -1 );                // Find the number of occurances of the item in the hierarchy.
  int                 CountItems( bool add_self = true, bool add_bones = false );         // Counts how many items are in a hierarchy

  // Item Name Functions
  inline const char * GetName() { return item_name; }
  inline const char * GetFullName() { return full_item_name; }
  inline void         SetFullName( const char *new_name){ strcpy( full_item_name, new_name );
                                                          if( (GetItemType() & (LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT) ) ) item_name = DirStrings::HasFile( full_item_name );
                                                          else item_name = full_item_name; }

  // Item Type Functions
  inline int  GetItemType() { return item_type; }                              // Get the Item's Type (see LWI_ITEM_*)
  inline void SetItemType( int new_type ) { item_type = new_type; }            // Set the Item's Type

  // Index In Scene Functions
  inline int  GetIndexInScene() { return index_in_scene; }                     // Get the index of the object in the scene
  inline void SetIndexInScene( int new_index ) { index_in_scene = new_index; } // Set the index of the object in the scene

  // Parent Functions
  inline int                 GetParentIndex() { return parent_item_index; }                      // Get the index of the parent object in the scene
  inline void                SetParentIndex( int new_index ) { parent_item_index = new_index; }  // Set the index of the parent object in the scene

  inline int                 GetParentType() { return parent_item_type; }                        // Get the index of the parent object in the scene
  inline void                SetParentType( int new_type ) { parent_item_type = new_type; }      // Set the index of the parent object in the scene

  inline LWItem *            GetParent() { return parent_item; }                                 // Get the the parent object in the scene
  inline void                SetParent( LWItem * new_parent ) { parent_item = new_parent; }      // Set the the parent object in the scene

  LWItem *                   HasAncestorNamed( const char * match );           // Return a pointer to the ancestor that matches the name passed in
  LWItem *                   HasAncestorNamed( const char * match, int len );  // Return a pointer to the ancestor that who's first len characters match the name passed in

  // Draw Mode Functions
  inline int           GetDrawMode() { return draw_mode; }
  inline void          SetDrawMode( int new_mode ) { draw_mode = new_mode; }
  inline bool          IsDrawn() { return (draw_mode > 0) ? true : false; }    // Returns TRUE if the object is beind drawn at all.  Useful for lights/cameras/bones

  inline int           GetDrawColor() { return draw_color; }
  inline void          SetDrawColor( int new_color ) { draw_color = new_color; }

  // Motion Accessors
  inline LWMotion    & GetMotion() { return motion; }

  // Children Accessors
  inline DynArray<LWItem *> & GetChildren() { return children; }
  inline bool                 HasSkeleton() { return (skeleton_base == NULL) ? false : true; }
  inline LWItem             & GetSkeleton() { return *skeleton_base; }

  // Counter functions
  inline int  GetCloneNumber() { return clone_number; }
  inline void SetCloneNumber( int index ) { clone_number = index; }

  // Depth Functions
  inline int  GetDepth() { return depth; }
  inline void SetDepth( int _depth ) { depth = _depth; }

  // UI-Level Accessors
  bool           GetSelected() { return selected; }
  void           SetSelected( bool state ) { selected = (GetItemType() & LWI_NON_ITEMS) ? false : state; }  // Sets the item state, unless the item is a scene or skeleton base (you're not allowed to select those)

  bool           GetFolded() { return folded; }
  void           SetFolded( bool state ) { folded = state; }

  unsigned long  GetGroup() { return group; }
  void           SetGroup( unsigned long index ) { group = index; }

  // Debug Functions
  unsigned long RAMUsageToStream( ofstream &out );

protected:
  // Protected Load Functions
  int LoadObject( pifstream &in, int mode, int version );
  int LoadBone(   pifstream &in, int mode, int version );
  int LoadLight(  pifstream &in, int mode, int version );
  int LoadCamera( pifstream &in, int mode, int version );

  // Protected Data Members
  char   full_item_name[ MAX_PATH_LENGTH ];       // Item Name, with path
  char * item_name;                               // Item Name only (no path)
  int    item_type;                               // Item Type (see LWI_ITEM_*)
  int    index_in_scene;                          // Index of item in the scene.  Starts counting from 1.

  int    parent_item_index;                       // Parent item's index in scene.  Starts counting from 1.
  int    parent_item_type;                        // Parent item's type (LWI_ITEM_OBJECT, _LIGHT, etc.)

  int    draw_mode;                               // Drawing method (wire, solid, etc.)
  int    draw_color;                              // Drawing Color

  int    clone_number;                            // Index representing which clone of the item this is in the scene (ie: Test.lwo (3) would be 3).
  int    depth;                                   // How many levels deep in the hierarchy this item is (ie: how many ancestors it has)

  LWMotion             motion;                    // Item Motion

  LWItem             * parent_item;               // Parent Item as a LWItem
  DynArray<LWItem *>   children;                  // Array of children of this object

  LWItem             * skeleton_base;             // Root of the skeleton

  // UI-Level Data Members
  bool          selected;                         // True if this item has been selected
  bool          folded;                           // True if this item has been folded (ie: it's children are hidden)
  unsigned long group;                            // Group this item belongs to (-1 if it's not in a group)
};

// QSort Functions
int __cdecl SortLWItems_Alpha( const void *elem1, const void *elem2 );

#endif