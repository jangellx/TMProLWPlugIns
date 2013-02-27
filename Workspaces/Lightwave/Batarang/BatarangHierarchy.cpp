//
// BatarangHierarchy.cpp
//

#include <fstream.h>
#include <iomanip.h>

#include "BatarangHierarchy.h"

// Constructor
BatarangHierarchy::BatarangHierarchy( const char *name,
                                      int new_type,
                                      int new_index,
                                      int new_parent_index,
                                      BatarangHierarchy * new_parent ) {
  // Setup the item's name
  if( name == NULL )
    full_item_name[0] = '\0';
  else
    SetFullName( name );

  // Item types
  item_type           = new_type;
  index_in_scene      = new_index;
  parent_object_index = new_parent_index;
  parent_object       = new_parent;
  times_found         = 0;
  copy_count          = 0;
  depth               = 0;

  folded              = false;
  selected            = false;
  group               = -1;

  // Motion
  motion              = NULL;

  // Skeleton
  skeleton_base       = NULL;
}

// Destructor
BatarangHierarchy::~BatarangHierarchy() {
  // Do NOT flush the child lists here!

  if( motion != NULL )
    delete motion;

  if( skeleton_base != NULL )
    delete skeleton_base;
}

// FreeHierarchy()
//  Frees an entire hierarchy, including the current item if free_self is
//   true.  This should be used only on hierarchies that are dynamically
//   allocated.

//
// NOTE:  THIS DOESN"T WORK:  It just crashes after a bit.  Free an Item
//  List (via BuildItemList()) instead
//

void BatarangHierarchy::FreeHierarchy( bool free_self ) {
  // Free the children
  for( unsigned int i=0; i < children.NumElements(); i++ ) {
    children[i]->FreeHierarchy( true );
    children[i] = NULL;
  }

  // Free the Skeleton
  for( i = 0; i < skeleton.NumElements(); i++ ) {
    skeleton[i]->FreeHierarchy( true );
    skeleton[i] = NULL;
  }

  // Free Self
  if( free_self )
    delete this;
}

// BuildTreeFromScene():
//  Builds a hierarchy tree from a scene file.  Retruns true if it
//   worked.  The new hierarchy will belong to the base argument
//   passed in, and the scene will be read from the in fstream
//   provided.  The source file will be reset to the beginning on
//   completion.
//  The item_list provided contains a simple list of all the items
//   in the hierarchy in the order they were found in the scene file.
//  If load_motion is true, then the motion of the item is loaded
//   via a FreeKeyMotion.
bool BatarangHierarchy::BuildHierarchyFromScene( ifstream &in, BatarangHierarchy &base,
                                                 DynArray<BatarangHierarchy *> &item_list,
                                                 bool load_motion ) {
  char line[8192];

  BatarangHierarchy * current = NULL;
  BatarangHierarchy * current_bone = NULL;

  DynArray<BatarangHierarchy *> base_item_list;

  int  object_index = 0;
  int  light_index  = 0;
  int  bone_index   = 0;

  bool   in_bone = false;
  char * temp    = NULL;

  // Hierarchy Scan
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() )
      return false;

    if( strnicmp( line, "LoadObject", 10 ) == 0 ) {
      in_bone = false;
      bone_index = 0;
      temp = DirStrings::HasFile( &(line[11]) );
      current = new BatarangHierarchy( temp, BH_ITEM_OBJECT, ++object_index );
      item_list.Add( current );
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
      in_bone = false;
      bone_index = 0;
      temp = DirStrings::HasFile( &(line[14]) );
      current = new BatarangHierarchy( temp, BH_ITEM_NULL_OBJECT, ++object_index );
      item_list.Add( current );
    }

    if( strnicmp( line, "LightName", 9 ) == 0 ) {
      temp = &(line[10]);
      current = new BatarangHierarchy( temp, BH_ITEM_LIGHT, -(++light_index) );  // Note that lights are indexed into the negatives!
      item_list.Add( current );
    }

    if( strnicmp( line, "BoneName", 8 ) == 0 ) {
      current_bone = new BatarangHierarchy( &(line[9]), BH_ITEM_BONE, ++bone_index );
      in_bone = true;
      current->skeleton.Add( current_bone );
    }

    if( strnicmp( line, "ParentObject", 12 ) == 0 ) {
      if( in_bone ) {                  // We only store the parent object for items that aren't bones
        int parent = atoi( &(line[13]) );
        if( current_bone != NULL ) {
          current_bone->SetParentIndex( parent );
        }
      } else {
        int parent = atoi( &(line[13]) );
        if( current != NULL )
          current->SetParentIndex( parent );
      }
    }

    // Load the motion, if applicable
    if( load_motion ) {
      if( ( strncmp( line, "ObjectMotion", 12 ) == 0 ) ||
          //( strncmp( line, "CameraMotion", 12 ) == 0 ) ||
          ( strncmp( line, "LightMotion", 11 ) == 0 ) ) {
        current->motion = FreeKeyMotion::NewMotionFromFile( in );
        if( current->motion == NULL )
          return false;
      }

      if( in_bone ) {
        if( strncmp( line, "BoneMotion", 10 ) == 0 ) {
          current_bone->motion = FreeKeyMotion::NewMotionFromFile( in );
          if( current_bone->motion == NULL )
            return false;
        }
      }
    }
  } while( !in.eof() );

  // Build Hierarchy
  for( int i=0; i < (int)item_list.NumElements(); i++ ) {
    if( item_list[i]->GetParentIndex() == 0 ) {  // No parent; add to the base list
      base.children.Add( item_list[i] );
    } else {                                     // Valid parent; add to the parent's list
      int parent_index = item_list[i]->GetParentIndex() - 1;
      item_list[ parent_index ]->children.Add( item_list[i] );
      item_list[i]->SetParent( item_list[ parent_index ] );
    }

    // Build child bone hierarchy tree
    if( item_list[i]->skeleton.NumElements() > 0 ) {
      item_list[i]->skeleton_base = new BatarangHierarchy( "Skeleton Base", BH_ITEM_SKELETON );
      for( unsigned int j=0; j < item_list[i]->skeleton.NumElements(); j++ ) {
        if( item_list[i]->skeleton[j]->GetParentIndex() == 0 ) {
          item_list[i]->skeleton_base->children.Add( item_list[i]->skeleton[j] );
        } else {
          int parent_index = item_list[i]->skeleton[j]->GetParentIndex()-1;
          item_list[i]->skeleton[ parent_index ]->children.Add( item_list[i]->skeleton[j] );
          item_list[i]->skeleton[j]->SetParent( item_list[i]->skeleton[ parent_index ] );
        }
      }
    }
  }

  // Go back to the start of the source file
  in.clear();
  in.seekg( 0, ios::beg );

  return true;
}

// FindChild():
//  Finds the first child object that matches the name provided.
//   The occurace'th instance of the item will be returned, or 0
//   if no match is found.  The item must also match the item_type,
//   which can be any of the BH_ITEM_* types OR'ed together.  The
//   search will stop at depth levels into the hierarchy, or, if
//   depth is -1, until the end of the hierarchy.  If not match
//   is found, NULL is returned.
//
//  NOTE:  The occurance code doesn't currently work -- the local
//         occurance variable isn't passed back to the calling
//         function, so it'll probably freak out a bit.  Just use
//         occurances of 1 until this gets fixed.
//
BatarangHierarchy * BatarangHierarchy::FindChild( const char *name,
                                                  int  match_type,
                                                  int  occurance,
                                                  int  depth ) {
  if( item_type & match_type ) {            // Make sure it's the right type
    if( stricmp( item_name, name ) == 0 ) { // Check for a name match
      if( occurance == 1 )                  // Now see if we want this occurance or not
        return this;                        //  Right occurance; return this
      else
        occurance--;                        //  Wrong occurance, decrement it.
    }
  }

  if( depth != BH_DEPTH_ALL )
    depth--;

  BatarangHierarchy *match;
  if( (depth > 0) || (depth == BH_DEPTH_ALL) ) { // Search children, but only if depth > 0 or no depth is specified (ie: BH_DEPTH_ALL)
    for( unsigned int i=0; i < children.NumElements(); i++ ) {
      match = children[i]->FindChild( name, match_type, occurance, depth );
      if( match != NULL )                        // See if we have a match
        return match;                            //  Got one; stop looking
    }
  }

  // No match found, return NULL
  return NULL;
}

// FindIndexInScene():
//  Finds an item by it's index in the scene file. A pointer to
//   the item will be returned, or 0
//   if no match is found.  The item must match the item_type,
//   which can be any of the BH_ITEM_* types OR'ed together.  The
//   search will stop at depth levels into the hierarchy, or, if
//   depth is -1, until the end of the hierarchy.  If not match
//   is found, NULL is returned.
//  This does NOT search through bones, as bone indices are
//   at an object level and are independant of the indices of
//   objects.  To search through the skeleton, just call
//   this on the base of the skeleton.
BatarangHierarchy * BatarangHierarchy::FindIndexInScene( int match_index,
                                                         int match_type,
                                                         int depth ) {
  if( item_type & match_type ) {             // Make sure it's the right type
    if( index_in_scene == match_index ) {    // Check for a name match
      return this;
    }
  }

  if( depth != BH_DEPTH_ALL )
    depth--;

  BatarangHierarchy *match;
  if( (depth > 0) || (depth == BH_DEPTH_ALL) ) { // Search children, but only if depth > 0 or no depth is specified (ie: BH_DEPTH_ALL)
    for( unsigned int i=0; i < children.NumElements(); i++ ) {
      match = children[i]->FindIndexInScene( match_index, match_type, depth );
      if( match != NULL )                        // See if we have a match
        return match;                            //  Got one; stop looking
    }
  }

  // No match found, return NULL
  return NULL;
}


// OutputAsText()
//  Outputs the entire hierarchy as a tree to the stream provided.
//   Indent is the default indentation, which gets 2 added to it
//   at each level of the hierarchy.
void BatarangHierarchy::OutputAsText( int indent, ostream &out ) {
  out << setw(indent) << ""                                 // Indentation
      << item_name                                          // Item Name
      << setw( 40 - strlen( item_name ) - indent ) << ""    // padding spaces
      << "  Index "  << setw(3) << index_in_scene           // Index in scene
      << ";    Parent " << setw(3) << parent_object_index   // Parent Index
      << ";    Copy " << setw(3) << copy_count              // Copy Count
      << endl;

  // Output skeleton data
  unsigned int i;
  if( skeleton_base != NULL ) {
    //cerr << setw(indent) << "" << "  - Object Skeleton:" << endl;
    skeleton_base->OutputAsText( indent + 4, out );
  }

  // Output children data
  for( i=0; i < children.NumElements(); i++ )
    children[i]->OutputAsText( indent + 2, out );
}

// BuildItemList()
//  Adds each item in the hierarchy to the DynArray provided.  The list
//   created is in hierarchical order -- simply indent it and you'll
//   have a complete hierarchy tree.  For a natural order list, see
//   the item list filled in by BuildHierarchyFromScene().  If add_self
//   is false, this item won't be added to the list (useful for getting
//   rid of scene/skeleton bases).
void BatarangHierarchy::BuildItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones, bool add_self ) {
  if( add_self )
    item_list.Add( this );

  // Add the bones
  if( add_bones && (skeleton_base != NULL) )
    skeleton_base->BuildItemList( item_list, false, false );       // No point in adding the bones of a bone

  // Build childrens' trees
  for( unsigned int i=0; i < children.NumElements(); i++ )
    children[i]->BuildItemList( item_list, add_bones, true );
}

// BuildFoldedItemList()
//  Builds a list of items in the same way as BuildItemList(), excluding
//   all the children/bones of folded items.
void BatarangHierarchy::BuildFoldedItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones, bool add_self ) {
  if( add_self )
    item_list.Add( this );

  if( !GetFolded() ) {
    // Add the bones
    if( add_bones && (skeleton_base != NULL) )
      skeleton_base->BuildFoldedItemList( item_list, false, false );       // No point in adding the bones of a bone

    // Build childrens' trees
    for( unsigned int i=0; i < children.NumElements(); i++ )
      children[i]->BuildFoldedItemList( item_list, add_bones, true );
  }
}

// BuildSelectedItemList():
//  Builds a list of all selected items.  If sel is false, then the list
//   will contain all unselected items.
void BatarangHierarchy::BuildSelectedItemList( DynArray<BatarangHierarchy *> &item_list, bool sel, bool add_bones, bool add_self ) {
  if( add_self ) {
    if( GetSelected() == sel )
      item_list.Add( this );
  }

  // Add the bones
  if( add_bones && (skeleton_base != NULL) )
    skeleton_base->BuildSelectedItemList( item_list, sel, false, false );       // No point in adding the bones of a bone

  // Build childrens' trees
  for( unsigned int i=0; i < children.NumElements(); i++ )
    children[i]->BuildSelectedItemList( item_list, sel, add_bones, true );
}

// BuildGroupItemList():
//  Builds a list of all items mathing the given group index.
void BatarangHierarchy::BuildGroupItemList( DynArray<BatarangHierarchy *> &item_list, int match_group, bool add_bones, bool add_self ) {
  if( add_self ) {
    if( GetGroup() == match_group )
      item_list.Add( this );
  }

  // Add the bones
  if( add_bones && (skeleton_base != NULL) )
    skeleton_base->BuildGroupItemList( item_list, match_group, false, false );       // No point in adding the bones of a bone

  // Build childrens' trees
  for( unsigned int i=0; i < children.NumElements(); i++ )
    children[i]->BuildGroupItemList( item_list, match_group, add_bones, true );
}

// BuildNaturalItemList()
//  Builds a list of items in natural order (ie: the order they were loaded
//   from the scene file) into the DynArray provided.  Note that the base item
//   is NOT added to the list.  If add_bones is true, then bones will also be
//   added just after the object they belong to.  Lights are added to the end
//   of the list.
void BatarangHierarchy::BuildNaturalItemList( DynArray<BatarangHierarchy *> &item_list, bool add_bones ) {
  int i=0, j;
  BatarangHierarchy *item = NULL, *bone;

  // Add Objects
  do {
    item = FindIndexInScene( ++i, BH_ITEM_OBJECT | BH_ITEM_NULL_OBJECT );
    if( item == NULL )
      break;

    item_list.Add( item );

    // Add Bones
    if( add_bones ) {
      if( item->skeleton_base != NULL ) {
        j    = 0;
        bone = NULL;

        do {
          bone = item->skeleton_base->FindIndexInScene( ++j, BH_ITEM_BONE );
          if( bone == NULL )
            break;

          item_list.Add( bone );
        } while( true );
      }
    }
  } while( true );


  // Add Lights
  i=0;
  do {
    item = FindIndexInScene( --i, BH_ITEM_LIGHT );
    if( item == NULL )
      break;

    item_list.Add( item );
  } while( true );
}

// IndexItems()
//  Indexes each instance of the same object in a scene.  For example,
//   the third Test.lwo (shown in Layout as Test.lwo (3)) would have
//   an instance index of 3.
void BatarangHierarchy::IndexItems( DynArray<BatarangHierarchy *> &item_list ) {
  unsigned int i, j, count;
  for( i=0; i < item_list.NumElements(); i++ ) {                   // Do every object
    const char * item_name = item_list[i]->GetFullName();
    count = 1;

    for( j=0; j < i; j++ ) {                                       // Compare against every object before it
      if( strcmp( item_name, item_list[j]->GetFullName() ) == 0 )
        count++;
    }

    item_list[i]->SetCopyNumber( count );                          // Set the count
  }
}

// FillInItemDepths()
//  Sets the depth memeber of each instance.  This is very similar t
//   OutputAsText(), but instead updates the depths internally.
void BatarangHierarchy::FillInItemDepths( int this_depth ) {
  // Set the depth
  depth = this_depth;

  // Handle skeleton data
  unsigned int i;
  if( skeleton_base != NULL )
    skeleton_base->FillInItemDepths( this_depth + 1 );

  // Handle children data
  for( i=0; i < children.NumElements(); i++ )
    children[i]->FillInItemDepths( this_depth + 1 );
}

// HasAncestorNamed( const char * ):
//  Returns a pointer to the ancestor, or NULL if there
//   is no parent with the name provided.
BatarangHierarchy * BatarangHierarchy::HasAncestorNamed( const char * match ) {
  // Get a pointer to this item in the scene
  BatarangHierarchy *ancestor = this;
  do {
    if( stricmp( ancestor->GetName(), match ) == 0 )        // Match found; return it
      return ancestor;
  } while( (ancestor = ancestor->GetParent()) != NULL );

  return NULL;
}

// HasAncestorNamed( const char *, int ):
//  Returns a pointer to the ancestor who's name starts with len number
//   of characters in match, or NULL if there is no parent with the name
//   provided.  This allows you to check for objects starting the same name,
//   but ending in differnt ones (ie: if match is "NO_" and len is 3, all
//   items starting with NO_ will match.
//  Basically, this function usese a strnicmp() instead of the stricmp()
//   function found in it's sibling.
BatarangHierarchy * BatarangHierarchy::HasAncestorNamed( const char * match, int len ) {
  // Get a pointer to this item in the scene
  BatarangHierarchy *ancestor = this;
  do {
    if( strnicmp( ancestor->GetName(), match, len ) == 0 )        // Match found; return it
      return ancestor;
  } while( (ancestor = ancestor->GetParent()) != NULL );

  return NULL;
}

// SortBH_Alpha():
//  QSort function to sort alphabetically
int SortBH_Alpha(const void *elem1, const void *elem2 ) {
  BatarangHierarchy * first  = *((BatarangHierarchy **) elem1);
  BatarangHierarchy * second = *((BatarangHierarchy **) elem2);

  return stricmp( first->GetName(), second->GetName() );
}


// CountItems():
//  Returns the number of items in the hierarchy.  If add_self is true,
//   then this item will be counted, too.  If add_bones is true, then
//   bones will also be counted.
int BatarangHierarchy::CountItems( bool add_self, bool add_bones ) {
  int count = 0;
  if( add_self )
    count++;

  for( unsigned int i=0; i < children.NumElements(); i++ )
    count += children[i]->CountItems( true, add_bones );

  if( add_bones ) {
    if( skeleton_base != NULL )
      count += skeleton_base->CountItems( false, false );
  }

  return count;
}

