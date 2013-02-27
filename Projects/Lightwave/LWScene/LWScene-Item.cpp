//
// LWItem.cpp
//

#include <iomanip.h>

#include <portable/stringtools/stringtools.h>

#include "LWScene.h"

// Constructor
LWItem::LWItem( const char *name,
                int new_type,
                int new_index,
                int new_parent_index,
                LWItem * new_parent ) {
  // Setup the item's name
  if( name == NULL )
    full_item_name[0] = '\0';
  else
    SetFullName( name );

  // Item types
  item_type           = new_type;
  index_in_scene      = new_index;
  parent_item_index   = new_parent_index;
  parent_item_type    = LWI_ITEM_OBJECT;        // Make this smarter (or obsolete) later
  parent_item         = new_parent;

  draw_mode           = LWIDRAW_TEXTURED;
  draw_color          = LWIDRAW_GREY;

  clone_number        = 0;
  depth               = 0;

  folded              = false;
  selected            = false;
  group               = -1;

  // Skeleton
  skeleton_base       = NULL;
}

// Destructor
LWItem::~LWItem() {
  // Do NOT flush the child lists here!

//  if( motion != NULL )
//    delete motion;

  if( skeleton_base != NULL )
    delete skeleton_base;
}

// FreeHierarchy()
//  Frees an entire hierarchy, including the current item if free_self is
//   true.  This should be used only on hierarchies that are dynamically
//   allocated.
void LWItem::FreeHierarchy() {
  // Free the children
  for( unsigned int i=0; i < children.NumElements(); i++ ) {
    children[i]->FreeHierarchy();
    delete children[i];
    children[i] = NULL;
  }

  // Free the Skeleton
  ClearSkeleton();
}

// ClearSkeleton()
//  Frees the skeleton of an item, deletes the Skeleton Base and
//   sets it to Null.  Note that this will free the bones in the
//   skeleton.
void LWItem::ClearSkeleton() {
  if( HasSkeleton() ) {
    skeleton_base->FreeHierarchy();
    delete skeleton_base;
    skeleton_base = NULL;
  }
}

// InitSkeleton()
//  Initializes the skeleton of the item.
void LWItem::InitSkeleton() {
  if( !HasSkeleton() )
    skeleton_base = new LWItem( "Skeleton Base", LWI_ITEM_SKELETON );
}

// LoadItemFromScene():
//  Loads an item from the scene file stream provided.  What exactly
//   is loaded is determined by the the mode bits and the version of
//   the scene file.  type is the string at the start of the item
//   that was read in by the calling function.  index is the index of
//   that item in the scene, and should match up with the type.
//   Indices count for 1, as in Lightwave scenes. version is the LWS
//   version, which the loader uses to figure out what to expect. 
//  item will contain a pointer to the item, or NULL if the item
//   shouldn't be loaded or there was an error.  If an error does
//   occur, it will be returned as a LWSERROR_ or LWOERROR_ if the
//   error occured loading an object file.
int LWItem::LoadItemFromScene( pifstream &in, const char *type, int index,
                               int mode, int version, LWItem *&item ) {
  item = NULL;

  if( !(mode & LWS_LOAD_HIERARCHY) )          // See if we're supposed to do anything
    return LWSERROR_OK;

  if( type == NULL )                          // Make sure the type/name is valid
    return LWSERROR_INVALID_ITEM_TYPE;

  if( strncmp( type, "LoadObject", 10 ) == 0 ) {
    item = new LWItem( &(type[11]), LWI_ITEM_OBJECT, index );
    return item->LoadObject( in, mode, version );
  } else if( strncmp( type, "AddNullObject", 13 ) == 0 ) {
    item = new LWItem( &(type[14]), LWI_ITEM_NULL_OBJECT, index );
    return item->LoadObject( in, mode, version );
  } else if( strncmp( type, "AddBone",     7 ) == 0 ) {
    item = new LWItem( NULL, LWI_ITEM_BONE, index );
    return item->LoadBone( in, mode, version );
  } else if( strncmp( type, "AddLight",    8 ) == 0 ) {
    item = new LWItem( NULL, LWI_ITEM_LIGHT, index );
    return item->LoadLight( in, mode, version );
  } else if( strncmp( type, "ShowCamera", 10 ) == 0 ) {
//    return item->LoadCamera( in, mode, version );
  } else
    return LWSERROR_UNKNOWN_ITEM_TYPE;

  return LWSERROR_OK;
}

// LoadObject():
//  Loads an object from the scene file stream provided.  What exactly
//   is loaded is determined by the mode bits and the version of the
//   scene file.
int LWItem::LoadObject( pifstream &in, int mode, int version ) {
  // Load the .lwo, if applicable
  if( mode & LWS_LOAD_LWOS ) {
    ; // Insert LWO Loader Here
  }

  // Loop Variables
  DynArray< LWItem * > skeleton;
  char                 line[8192];
  char               * temp = NULL;

  LWItem             * bone = NULL;
  int                  bone_index = 0;
  int                  error;

  // Loop
  do {
    in.getline( line, 8192 );

    if( (in.bad() || in.fail()) && !in.eof() )
      return LWSERROR_ERROR_READING_FROM_SCENE;

    if( (mode & LWS_LOAD_HIERARCHY) ) {
      if( mode & LWS_LOAD_OBJECTS ) {
        // Object-Level Data
      }

      // Item-Independant Data
      if( strncmp( line, "ShowObject", 10 ) == 0 ) {
        // ShowObject <mode> <color>
        draw_mode  = atoi( &(line[11]) );
        temp = StringTools::SkipWhiteSpace( &(line[11]) );     // Find the next word
        temp = StringTools::FindWhiteSpace( temp );
        draw_color = atoi( temp );
      } else if( strncmp( line, "ObjectMotion", 12 ) == 0 ) {
        // ObjectMotion
        if( mode & LWS_LOAD_MOTIONS ) {
          int error = motion.LoadMotion( in, mode, version, &(line[13]), 30 );    // Assume an FPS of 30; change this if problems arise with L6 scenes
          if( error != LWSERROR_OK )
            return error;
        }
      } else if( strncmp( line, "ParentObject", 12 ) == 0 ) {
        // ParentObject
        parent_item_index = atoi( &(line[13]) );
        parent_item_type = LWI_ITEM_OBJECT;
      } else if( strncmp( line, "ParentItem", 10 ) == 0 ) {
        // ParentItem
        ; // Insert L6-specific ParentItem code here

      } else if( strncmp( line, "AddBone", 7 ) == 0 ) {
        // AddBone
        error = LWItem::LoadItemFromScene( in, line, ++bone_index, mode, version, bone );

        if( bone )                          // Add the bone to the skeleton if it was loaded
          skeleton.Add( bone );
        else if( error != LWSERROR_OK )     // Note that bone can be NULL if the mode bits say that the item shouldn't be loaded
          return error;
      }
    }

    // Test for the end of the object
    if( strncmp( line, "ShadowOptions", 13 ) == 0 ) {
      // Shadow Options
      if( mode & LWS_LOAD_OBJECTS ) {
        ; // Insert ShadowOptions bit-field decoder here
      }

      break;                                               // End of object, stop looking
    }
  } while( !in.eof() );

  // Build Bone Hierarchy, if any bones exists
  int parent_index;
  if( skeleton.NumElements() > 0 ) {
    ClearSkeleton();
    InitSkeleton();
    for( unsigned int j=0; j < skeleton.NumElements(); j++ ) {
      if( skeleton[j]->GetParentIndex() == LWI_NO_PARENT ) {            // Unparented Bones
        skeleton_base->children.Add( skeleton[j] );
      } else {                                                          // Parented Bones
        parent_index = skeleton[j]->GetParentIndex()-1;
        skeleton[ parent_index ]->children.Add( skeleton[j] );
        skeleton[j]->SetParent( skeleton[ parent_index ] );
      }
    }

    // Post Processing
    if( mode & LWS_LOAD_FULL_POST ) {
      FillInItemCloneNumber( skeleton );
      FillInItemDepths();
    }
  }

  return LWSERROR_OK;
}

// LoadBone():
//  Loads a bone from the scene file stream provided.  What exactly
//   is loaded is determined by the mode bits and the version of the
//   scene file.
int LWItem::LoadBone( pifstream &in, int mode, int version ) {
  // Loop Variables
  char       line[8192];
  char     * temp = NULL;
  streampos  old;

  // Loop
  do {
    old = in.tellg();
    in.getline( line, 8192 );

    if( (in.bad() || in.fail()) && !in.eof() )
      return LWSERROR_ERROR_READING_FROM_SCENE;

    if( mode & LWS_LOAD_HIERARCHY ) {
      if( mode & LWS_LOAD_BONES ) {
        // Bone-Level Data
      }

      // Item-Independant Data
      if( strncmp( line, "ShowBone", 8 ) == 0 ) {
        // ShowBone <mode> <color>
        draw_mode  = atoi( &(line[9]) );
        temp = StringTools::SkipWhiteSpace( &(line[9]) );     // Find the next word
        temp = StringTools::FindWhiteSpace( temp );
        draw_color = atoi( temp );
      } else if( strncmp( line, "BoneName", 8 ) == 0 ) {
        // BoneName
        SetFullName( &(line[9]) );
      } else if( strncmp( line, "BoneMotion", 10 ) == 0 ) {
        // BoneMotion
        if( mode & LWS_LOAD_MOTIONS ) {
          int error = motion.LoadMotion( in, mode, version, &(line[13]), 30 );    // Assume an FPS of 30; change this if problems arise with L6 scenes
          if( error != LWSERROR_OK )
            return error;
        }
      } else if( strncmp( line, "ParentObject", 12 ) == 0 ) {
        // ParentObject (really the parent bone)
        parent_item_index = atoi( &(line[13]) );
        parent_item_type = LWI_ITEM_BONE;
      } else if( strncmp( line, "ParentItem", 10 ) == 0 ) {
        // ParentItem
        ; // Insert L6-specific ParentItem code here
      }
    }

    // Test for the end of the bone
    if( (strncmp( line, "ShadowOptions", 13 ) == 0) ||     // End of object; no more bones
        (strncmp( line, "AddBone",        7 ) == 0) ||     // Begining of another bone
        (strncmp( line, "AmbientColor",  12 ) == 0) ) {    // End of all objects and bones
      in.seekg( old, ios::beg );                           // Seek back to the start of the line
      break;                                               // Break out
    }
  } while( !in.eof() );

  return LWSERROR_OK;
}

// LoadLight():
//  Loads a light from the scene file stream provided.  What exactly
//   is loaded is determined by the mode bits and the version of the
//   scene file.
int LWItem::LoadLight( pifstream &in, int mode, int version ) {
  // Loop Variables
  char       line[8192];
  char     * temp = NULL;
  streampos  old;

  // Loop
  do {
    old = in.tellg();
    in.getline( line, 8192 );

    if( (in.bad() || in.fail()) && !in.eof() )
      return LWSERROR_ERROR_READING_FROM_SCENE;

    if( mode & LWS_LOAD_HIERARCHY ) {
      if( mode & LWS_LOAD_LIGHTS ) {
        // Light-Level Data
      }

      // Item-Independant Data
      if( strncmp( line, "ShowLight", 9 ) == 0 ) {
        // ShowObject <mode> <color>
        draw_mode  = atoi( &(line[10]) );
        temp = StringTools::SkipWhiteSpace( &(line[10]) );     // Find the next word
        temp = StringTools::FindWhiteSpace( temp );
        draw_color = atoi( temp );
      } else if( strncmp( line, "LightName", 9 ) == 0 ) {
        // LightName
        SetFullName( &(line[10]) );
      } else if( strncmp( line, "LightMotion", 11 ) == 0 ) {
        // LightMotion
        if( mode & LWS_LOAD_MOTIONS ) {
          int error = motion.LoadMotion( in, mode, version, &(line[13]), 30 );    // Assume an FPS of 30; change this if problems arise with L6 scenes
          if( error != LWSERROR_OK )
            return error;
        }
      } else if( strncmp( line, "ParentObject", 12 ) == 0 ) {
        // ParentObject
        parent_item_index = atoi( &(line[13]) );
        parent_item_type = LWI_ITEM_OBJECT;
      } else if( strncmp( line, "ParentItem", 10 ) == 0 ) {
        // ParentItem
        ; // Insert L6-specific ParentItem code here

      }
    }

    // Test for the end of the bone
    if( (strncmp( line, "ShowCamera", 10 ) == 0) ||        // End of all lights
        (strncmp( line, "AddLight",    8 ) == 0) ) {       // Begining of another light
      in.seekg( old, ios::beg );                           // Seek back to the start of the line
      break;                                               // Break out
    }
  } while( !in.eof() );

  return LWSERROR_OK;
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
bool LWItem::BuildHierarchyFromScene( ifstream &in, LWItem &base,
                                                 DynArray<LWItem *> &item_list,
                                                 bool load_motion ) {
  char line[8192];

  LWItem * current = NULL;
  LWItem * current_bone = NULL;

  DynArray<LWItem *> base_item_list;

  int  object_index = 0;
  int  light_index  = 0;
  int  bone_index   = 0;

  bool   in_bone = false;
  char * temp    = NULL;

  // Hacky Temp Thing For Reference
  DynArray< LWItem * > skeleton;

  // Hierarchy Scan
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() )
      return false;

    if( strnicmp( line, "LoadObject", 10 ) == 0 ) {
      in_bone = false;
      bone_index = 0;
      temp = DirStrings::HasFile( &(line[11]) );
      current = new LWItem( temp, LWI_ITEM_OBJECT, ++object_index );
      item_list.Add( current );
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
      in_bone = false;
      bone_index = 0;
      temp = DirStrings::HasFile( &(line[14]) );
      current = new LWItem( temp, LWI_ITEM_NULL_OBJECT, ++object_index );
      item_list.Add( current );
    }

    if( strnicmp( line, "LightName", 9 ) == 0 ) {
      temp = &(line[10]);
      current = new LWItem( temp, LWI_ITEM_LIGHT, -(++light_index) );  // Note that lights are indexed into the negatives!
      item_list.Add( current );
    }

    if( strnicmp( line, "BoneName", 8 ) == 0 ) {
      current_bone = new LWItem( &(line[9]), LWI_ITEM_BONE, ++bone_index );
      in_bone = true;
      skeleton.Add( current_bone );
    }

    if( strnicmp( line, "ParentObject", 12 ) == 0 ) {
      if( in_bone ) {                                                  // We only store the parent object for items that aren't bones
        int parent = atoi( &(line[13]) );
        if( current_bone != NULL ) {
          current_bone->SetParentIndex( parent );                      // Here we store the parent bone for a bone
        }
      } else {
        int parent = atoi( &(line[13]) );
        if( current != NULL )
          current->SetParentIndex( parent );
      }
    }

    // Load the motion, if applicable
/*    if( load_motion ) {
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
    } */
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
    if( skeleton.NumElements() > 0 ) {
      item_list[i]->skeleton_base = new LWItem( "Skeleton Base", LWI_ITEM_SKELETON );
      for( unsigned int j=0; j < skeleton.NumElements(); j++ ) {
        if( skeleton[j]->GetParentIndex() == 0 ) {
          item_list[i]->skeleton_base->children.Add( skeleton[j] );
        } else {
          int parent_index = skeleton[j]->GetParentIndex()-1;
          skeleton[ parent_index ]->children.Add( skeleton[j] );
          skeleton[j]->SetParent( skeleton[ parent_index ] );
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
//   which can be any of the LWI_ITEM_* types OR'ed together.  The
//   search will stop at depth levels into the hierarchy, or, if
//   depth is -1, until the end of the hierarchy.  If not match
//   is found, NULL is returned.
//
//  NOTE:  The occurance code doesn't currently work -- the local
//         occurance variable isn't passed back to the calling
//         function, so it'll probably freak out a bit.  Just use
//         occurances of 1 until this gets fixed.
//
LWItem * LWItem::FindChild( const char *name,
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

  if( depth != LWI_DEPTH_ALL )
    depth--;

  LWItem *match;
  if( (depth > 0) || (depth == LWI_DEPTH_ALL) ) { // Search children, but only if depth > 0 or no depth is specified (ie: LWI_DEPTH_ALL)
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
//   which can be any of the LWI_ITEM_* types OR'ed together.  The
//   search will stop at depth levels into the hierarchy, or, if
//   depth is -1, until the end of the hierarchy.  If not match
//   is found, NULL is returned.
//  Note that you should really only use one item type (save for
//   null objects and normal objects) at a time, as they are all
//   indexed seperately (ie: there may be a light, object and 
//   camera all with an index of 1).
//  This does NOT search through bones, as bone indices are
//   at an object level and are independant of the indices of
//   objects.  To search through the skeleton, just call
//   this on the base of the skeleton.
LWItem * LWItem::FindIndexInScene( int match_index,
                                   int match_type,
                                   int depth ) {
  if( item_type & match_type ) {             // Make sure it's the right type
    if( index_in_scene == match_index ) {    // Check for a match
      return this;
    }
  }

  if( depth != LWI_DEPTH_ALL )
    depth--;

  LWItem *match;
  if( (depth > 0) || (depth == LWI_DEPTH_ALL) ) { // Search children, but only if depth > 0 or no depth is specified (ie: LWI_DEPTH_ALL)
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
void LWItem::OutputAsText( int indent, ostream &out, bool output_motion ) {
  out << setw(indent) << ""                                 // Indentation
      << item_name                                          // Item Name
      << setw( 40 - strlen( item_name ) - indent ) << ""    // padding spaces
      << "  Index "  << setw(3) << index_in_scene           // Index in scene
      << ";    Parent " << setw(3) << parent_item_index     // Parent Index
      << ";    Clone  " << setw(3) << clone_number          // Copy Count
      << endl;

  if( output_motion && (motion.GetChannelList().NumElements() > 0) ) {
    out << setw(indent + 4) << ""
        << "Keys:  " << motion.GetChannel(0).GetKeys().NumElements() << endl;
  }

  // Output skeleton data
  unsigned int i;
  if( skeleton_base != NULL ) {
    //cerr << setw(indent) << "" << "  - Object Skeleton:" << endl;
    skeleton_base->OutputAsText( indent + 4, out, output_motion );
  }

  // Output children data
  for( i=0; i < children.NumElements(); i++ )
    children[i]->OutputAsText( indent + 2, out, output_motion );
}

// BuildItemList()
//  Adds each item in the hierarchy to the DynArray provided.  The list
//   created is in hierarchical order -- simply indent it and you'll
//   have a complete hierarchy tree.  For a natural order list, see
//   the item list filled in by BuildHierarchyFromScene().  If add_self
//   is false, this item won't be added to the list (useful for getting
//   rid of scene/skeleton bases).
void LWItem::BuildItemList( DynArray<LWItem *> &item_list, bool add_bones, bool add_self ) {
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
void LWItem::BuildFoldedItemList( DynArray<LWItem *> &item_list, bool add_bones, bool add_self ) {
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
void LWItem::BuildSelectedItemList( DynArray<LWItem *> &item_list, bool sel, bool add_bones, bool add_self ) {
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
void LWItem::BuildGroupItemList( DynArray<LWItem *> &item_list, unsigned long match_group, bool add_bones, bool add_self ) {
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
void LWItem::BuildNaturalItemList( DynArray<LWItem *> &item_list, bool add_bones ) {
  int i=0, j;
  LWItem *item = NULL, *bone;

  // Add Objects
  do {
    item = FindIndexInScene( ++i, LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT );
    if( item == NULL )
      break;

    item_list.Add( item );

    // Add Bones
    if( add_bones ) {
      if( item->skeleton_base != NULL ) {
        j    = 0;
        bone = NULL;

        do {
          bone = item->skeleton_base->FindIndexInScene( ++j, LWI_ITEM_BONE );
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
    item = FindIndexInScene( --i, LWI_ITEM_LIGHT );
    if( item == NULL )
      break;

    item_list.Add( item );
  } while( true );
}

// IndexItems()
//  Indexes each instance of the same object in a scene.  For example,
//   the third Test.lwo (shown in Layout as Test.lwo (3)) would have
//   an instance index of 3.
//  This is an old function, and may not work correctly.  Use
//  FillInItemCloneNumber() instead
void LWItem::IndexItems( DynArray<LWItem *> &item_list ) {
  unsigned int i, j, count;
  for( i=0; i < item_list.NumElements(); i++ ) {                   // Do every object
    const char * item_name = item_list[i]->GetFullName();
    count = 1;

    for( j=0; j < i; j++ ) {                                       // Compare against every object before it
      if( strcmp( item_name, item_list[j]->GetFullName() ) == 0 )
        count++;
    }

    item_list[i]->SetCloneNumber( count );                         // Set the count
  }
}

// FillInItemDepths()
//  Sets the depth memeber of each instance.  This is very similar t
//   OutputAsText(), but instead updates the depths internally.
void LWItem::FillInItemDepths( int this_depth ) {
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
LWItem * LWItem::HasAncestorNamed( const char * match ) {
  // Get a pointer to this item in the scene
  LWItem *ancestor = this;
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
//   items starting with NO_ will match.)
//  Basically, this function usese a strnicmp() instead of the stricmp()
//   function found in it's sibling.
LWItem * LWItem::HasAncestorNamed( const char * match, int len ) {
  // Get a pointer to this item in the scene
  LWItem *ancestor = this;
  do {
    if( strnicmp( ancestor->GetName(), match, len ) == 0 )        // Match found; return it
      return ancestor;
  } while( (ancestor = ancestor->GetParent()) != NULL );

  return NULL;
}

// SortLWI_Alpha():
//  QSort function to sort alphabetically
int SortLWItems_Alpha(const void *elem1, const void *elem2 ) {
  LWItem * first  = *((LWItem **) elem1);
  LWItem * second = *((LWItem **) elem2);

  return stricmp( first->GetName(), second->GetName() );
}


// CountItems():
//  Returns the number of items in the hierarchy.  If add_self is true,
//   then this item will be counted, too.  If add_bones is true, then
//   bones will also be counted.
int LWItem::CountItems( bool add_self, bool add_bones ) {
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

// FillInItemCloneNumber():
//  Fills in the Item's Clone Number.  This will be -1 if the item is unique
//   in the scene; otherwise, it will be the instance of that item in the
//   scene, counting from 1.   A value of 0 means that this function probably
//   hasn't been called yet.
//  item_list is the list of items sorted in natural order to process.  This
//   list should include items and their children (this can be build with
//   LWItem::BuiltNaturalItemList()); if the list is not in natural order, the
//   clone indicies may not line up.  Skeletons, children, etc. are not
//   affected by this.  Bones should not be included in this list, and should
//   be processed seperately.
//  Items are "cloned" by type; Lights will be indexed seperately from objects,
//   etc.
void LWItem::FillInItemCloneNumber( DynArray< LWItem * > &item_list ) {
  // Set all items to unassigned clone index of 0
  for( unsigned int i=0; i < item_list.NumElements(); i++ )
    item_list[i]->SetCloneNumber( 0 );

  // Update each item's clone count
  unsigned int j;
  int index;

  for( i=0; i < item_list.NumElements(); i++ ) {
    if( item_list[i]->GetCloneNumber() == 0 ) {          // Only process this item if it hasn't been done yet
      index = 1;

      // Scan for clones
      for( j=i+1; j < item_list.NumElements(); j++ ) {
        if( ( (item_list[i]->GetItemType() & (LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT)) &&   // Special Case:  Nulls and Objects are
              (item_list[i]->GetItemType() & (LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT)) ) || //                cloned with the same indices
            ( item_list[i]->GetItemType() == item_list[j]->GetItemType() ) ) {              // All other item types here
          if( stricmp( item_list[i]->GetName(), item_list[j]->GetName() ) == 0 )
            item_list[j]->SetCloneNumber( ++index );
        }
      }

      // Set this item's clone number (will be -1 if it isn't cloned)
      item_list[i]->SetCloneNumber( (index > 1 ? 1 : -1) );
    }
  }
}

// RAMUsageToStream():
//  Outputs RAM usage information to the stream provided
unsigned long LWItem::RAMUsageToStream( ofstream &out ) {
  unsigned long size = 0;
  out << " ----------------------------------------------------------------- " << endl;
  out << endl;
  out << "LWItem:  " << item_name << endl;
  out << " Basic Item Information" << endl;
  out << "  Item Name:             " << full_item_name << endl;
  out << "  Item Type:             " << item_type << endl;
  out << "  Clone Number:          " << clone_number << endl;
  out << "  Draw Mode/Color:       " << draw_mode << "/" << draw_color << endl;
  out << endl;

  out << "  Index In Scene:        " << index_in_scene << endl;
  out << "  Parent Index (from 1): " << parent_item_index << endl;
  out << "  Parent Type:           " << parent_item_type << endl;
  out << "  Depth (# Ancestors):   " << depth << endl;
  out << "  Number of Children:    " << children.NumElements() << endl;
  out << "  Has Skeleton:          " << ( HasSkeleton() ? "Yes" : "No" ) << endl;

  out << endl;
  out << " UI Level Information" << endl;
  out << "  Group:                 " << group << endl;
  out << "  Selected:              " << (selected ? "Yes" : "No") << endl;
  out << "  Folded:                " << (folded   ? "Yes" : "No") << endl;

  out << " Motion Data" << endl;
  out << "  Number of Channels:    " << motion.GetChannelList().NumElements() << endl;

  long key_count = 0;
  for( unsigned long i=0; i < motion.GetChannelList().NumElements(); i++ ) {
    key_count += motion.GetChannelList()[i]->GetKeys().NumElements();
    out << "   Channel " << i << " keys:        " << motion.GetChannelList()[i]->GetKeys().NumElements() << endl;
  }

  out << "  Number of Channels:    " << motion.GetChannelList().NumElements() << endl;
  out << "  Total Number of Keys:  " << key_count << endl;

  out << " Basic RAM Usage" << endl;
  out << "  sizeof( this ):        " << sizeof( *this ) << endl;
  size += sizeof( *this );

  out << "  Size of Motion:        " << sizeof( motion ) << endl;
  out << "  Size of 1 Channel:     " << sizeof( LWChannel ) << endl;
  out << "  Size of All Channels:  " << sizeof( LWChannel ) * motion.GetChannelList().NumElements() << endl;
  size += sizeof( LWChannel ) * motion.GetChannelList().NumElements();

  out << "  Size of 1 Key (TCB):   " << sizeof( LWKeyFrame ) + sizeof( LWSplineTCB ) << endl;
  for( i=0; i < motion.GetChannelList().NumElements(); i++ )
    out << "   Size of All Keys, Channel " << i << ":  " << motion.GetChannelList()[i]->GetKeys().NumElements() * (sizeof( LWKeyFrame ) + sizeof( LWSplineTCB )) << endl;

  out << "  Total Key Size (All Channels): " << key_count * (sizeof( LWKeyFrame ) + sizeof( LWSplineTCB )) << endl;
  size += key_count * (sizeof( LWKeyFrame ) + sizeof( LWSplineTCB ));

  out << endl;
  if( !HasSkeleton() && (children.NumElements() == 0) )
    out << "  Total Size, sans children/skeleton: " << size << endl;
  else
    out << "  Total Size:          " << size << endl;

  // Process Children
  if( children.NumElements() > 0 ) {
    out << "LWItem Start Children:  " << item_name << " (" << clone_number << ")..." << endl; 
    for( i=0; i < children.NumElements(); i++ )
      size += children[i]->RAMUsageToStream( out );
  }

  if( HasSkeleton() ) {
    out << "LWItem Start Skeleton:  " << item_name << " (" << clone_number << ")..." << endl; 
    for( i=0; i < skeleton_base->GetChildren().NumElements(); i++ )
      size += skeleton_base->GetChildren()[i]->RAMUsageToStream( out );
  }

  if( HasSkeleton() || (children.NumElements() > 0) )
    out << "LWItem " << index_in_scene << " End:  " << item_name << " (" << clone_number << "), Total Size:  " << size << endl; 

  out << endl;

  return size;
}

