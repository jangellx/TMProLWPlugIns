//
//  UberNull
//
//  A simple program that parents every unparented object, light and camera to
//  a newly created NULL object, UberNull.
//
//
//  Argument Template:
//    UberNull <scene.lws>
//
//  That's it.  Just pass in the name of the scene to be UberNulled.  A new
//  scene will be created in the same directory, named <scene>-UberNull.lws
//
//  Since UberNull inserts a new NULL object at the begining of the scene,
//  all other objects must have their ParentObject incremented by 1. 
//

#include "UberNull.h"

const char *ubernull_error_strings[] = {
  "Everything's OK!",
  "The source scene file is does not exist!",
  "The source scene file is not accesibale.  It may be locked by another program",
  "The output scene file is not accessable.  It may be write protected or locked",
  "The UberNull's name is invalid",
  "The input and output files failed to open; aborting",
  "Error processing the source scene!",
  "Source and Output files cannot have the same names!",
  "No nulls enabled, nothing to process; aborting",
  "No enabled nulls match any items in the scene, nothing to do; aborting",
  "Don't use me -- I'm just the error enum count"
};

// Variables and Constants
const char *UberNullObject[] = {
  "AddNullObject ",
  "ShowObject 5 7",
  "ObjectMotion (unnamed)",
  "  9",
  "  1",
  "  0 0 0 0 0 0 1 1 1",
  "  0 0 0 0 0",
  "EndBehavior 1",
  "ShadowOptions 7",
  NULL
};


// Constructor
UberNull::UberNull( const char  *_source_name,
                    const char  *_output_name,
                    const char  *_ubernull_name,
                    unsigned int _location )
  : master_location(_location), in(NULL), out(NULL),
    use_master(true), use_names(false) {

  // Default Source Name
  if( _source_name != NULL )
    strcpy( source_name, _source_name );
  else
    source_name[0] = '\0';

  // Default Output Name
  if( _output_name != NULL )
    strcpy( output_name, _output_name );
  else
    output_name[0] = '\0';

  // Default UberNull Name
  if( _ubernull_name != NULL )
    strcpy( master_name, _ubernull_name );
  else
    master_name[0] = '\0';
}

// Destructor
UberNull::~UberNull() {
  delete in;
  delete out;

  names.Flush();
}


// OpenFiles():
//  Tests to make sure the paths are valid and opens the files.
//  This also generates an output and ubernull name if none are
//  specified.  Returns an error if something went wrong.
int UberNull::OpenFiles() {
  // See if the source and output names are the same
  if( stricmp( source_name, output_name ) == 0 )
    return UBERNULL_SAME_SOURCE_AND_OUTPUT;

  // Generate names if they are empty
  if( !GenerateNames() )
    return UBERNULL_SOURCE_DOESNT_EXIST;

  // Open the source scene file
  if( !DirInfo::Exists( source_name ) ) {
    char buffer[256];                        // Path doesn't exists; test with a .lws extension
    strcpy( buffer, source_name );
    DirStrings::ChangeExtension( buffer, ".lws" );

    if( DirInfo::Exists( buffer ) )
      strcpy( source_name, buffer );         // Extension version found; use it as the scene name from now on
    else
      return UBERNULL_SOURCE_DOESNT_EXIST;   // Extension version not found either; fail
  }

  in = new pifstream( source_name, ios::nocreate );
  if( !(*in) ) {
    CloseFiles();
    return UBERNULL_CANT_OPEN_SOURCE;
  }
  in->SetAnyEOL( true );

  // Open the Output File
  out = new pofstream( output_name );
  if( !(*out) ) {
    CloseFiles();
    return UBERNULL_INVALID_OUTPUT_PATH;
  }

  return UBERNULL_OK;
}

// CloseFiles():
//  Close the source and output files
void UberNull::CloseFiles() {
  delete in;
  delete out;

  in  = NULL;
  out = NULL;
}

// GenerateNames():
//  Generates new names for the output file and null object.
//   New names are generated only if the original names are
//   blank (ie: first char is \0).  Returns false if the
//   source is also empty.
bool UberNull::GenerateNames() {
  // Make sure the source filename is valid
  if( source_name[0] == '\0' )
    return false;

  // Generate a null object name, if needed
  if( master_name[0] == '\0' ) {
    char scene_name[256];
    DirStrings::GetFilePartOnly( scene_name, source_name );
    strcpy( master_name, "--UberNull--" );
    strcat( master_name, scene_name );
  }

  // Generate an output name, if needed
  if( output_name[0] == '\0' ) {
    strcpy( output_name, source_name );                   // Copy the in name
    char * ext = DirStrings::HasExtension( output_name ); // Find any extension
    if( ext )                                             // Strip the extension
      *ext = '\0';
    strcat( output_name, "-UberNull" );                   // Append -UberNull
    ext = DirStrings::HasExtension( source_name );        // Find the original extension
    if( ext )                                             // Append the origianl extension to the output_name
      strcat( output_name, ext );
  }

  return true;
}

// FindUberNullIndex():
//  Sets the index of each UberNull to be added to the scene, returning the
//  index of the last object before the Last In Scene UberNulls are added.
//  Note that index counts from 1!
// This will set the index for both the Master Null and all the Name Nulls
//  that are enabled.
int UberNull::FindUberNullIndex() {
  enum UnItem {
    UNITEM_NONE,
    UNITEM_OBJECT,
    UNITEM_BONE,
    UNITEM_LIGHT,
    UNITEM_CAMERA };

       ubernull_index = 1;
  int  index          = 0;
  int  cur_name       = -1;
  bool found_parent   = false;
  int  in_item        = UNITEM_NONE;

  unsigned int i;

  char buffer[1024];

  // Reset all the name indices to -1
  for( i=0; i < names.NumElements(); i++ ) {
    names[i]->index_in_scene    = -1;
    names[i]->children_in_scene = 0;
  }

  // First Scan:  Figure out which nulls actually have children
  do {
    *in >> buffer;
    if( (stricmp( buffer, "AddNullObject") == 0) ||
        (stricmp( buffer, "LoadObject")    == 0) ) {

      if( !found_parent && (cur_name != -1) )
        names[ cur_name ]->children_in_scene++;

      in_item = UNITEM_OBJECT;

      in->getline( buffer, 1024 );
      cur_name = -1;
      for( i=0; i < names.NumElements(); i++ ) {
        if( names[i]->GetEnable() ) {
          if( names[i]->TestForMatch( DirStrings::HasFile( &buffer[1] ) ) ) {
            cur_name = i;
            break;
          }
        }
      }
    } else if( stricmp( buffer, "BoneName") == 0 ) {
      if( found_parent && (cur_name != -1) )
        names[ cur_name ]->children_in_scene++;

      in_item = UNITEM_BONE;
      cur_name = -1;
      found_parent = false;
    } else if( stricmp( buffer, "LightName") == 0 ) {
      if( found_parent && (cur_name != -1) )
        names[ cur_name ]->children_in_scene++;

      in_item = UNITEM_LIGHT;
      in->getline( buffer, 1024 );
      cur_name = -1;
      for( i=0; i < names.NumElements(); i++ ) {
        if( names[i]->GetEnable() ) {
          if( names[i]->TestForMatch( &buffer[1] ) ) {
            cur_name = i;
            break;
          }
        }
      }

      found_parent = false;
    } else if( stricmp( buffer, "AddCamera") == 0 ) {
      if( found_parent && (cur_name != -1) )
        names[ cur_name ]->children_in_scene++;

      in_item = UNITEM_CAMERA;
      cur_name = -1;
      found_parent = false;
    } else if( stricmp( buffer, "ParentObject" ) == 0 ) {
      // Check for a parent object
      if( (in_item == UNITEM_OBJECT) || (in_item == UNITEM_LIGHT) )
        bool found_parent = true;
    }
  } while( !in->eof() && !in->fail() );

  //
  // --------------------------------------------------------------
  //


  // Second Scan:  Assign indices to nulls with children
  in->seekg( 0 );
  in->clear();

  // Set the initial index (dependant on if the Master is the first in the scene)
  if( GetUseMasterNull() ) {
    if( master_location == UBERNULL_FIRST )
      index++;
  }

  // First-In-Scene Scan for name indices
  for( i=0; i < names.NumElements(); i++ ) {
    if( names[i]->GetEnable() ) {
      if( names[i]->children_in_scene > 0 ) {
        if( names[i]->GetLocation() == UNNLOC_SCENE_FIRST )
          names[i]->index_in_scene = ++index;
      }
    }
  }

  // Scan the input file
  do {
    *in >> buffer;
    if( (stricmp( buffer, "AddNullObject") == 0) ||
        (stricmp( buffer, "LoadObject")    == 0) ) {

      char * name;
      if( buffer[0] == 'A' )
        name = &( buffer[ 14 ] );
      else
        name = &( buffer[ 11 ] );

      index++;
      in_item = UNITEM_OBJECT;

      // Handle Start-Of-Hierarchy for name indices
      in->getline( buffer, 1024 );
      cur_name = -1;
      for( i=0; i < names.NumElements(); i++ ) {
        if( names[i]->GetEnable() ) {
          if( names[i]->children_in_scene > 0 ) {
            if( names[i]->TestForMatch( DirStrings::HasFile( name ) ) ) {
              if( names[i]->GetLocation() == UNNLOC_HIERARCHY_FIRST ) {
                if( names[i]->index_in_scene == -1 ) {
                  names[i]->index_in_scene = index++;
                  cur_name = i;
                  break;
                }
              }
            }
          }
        }
      }
    }
  } while( !in->eof() && !in->fail() );

  int last_index = index;

  // Master Null Last-In-Scene
  if( master_location == UBERNULL_LAST )
    ubernull_index = ++index;

  // Set up the Last In Scene options
  for( i=0; i < names.NumElements(); i++ ) {
    if( names[i]->GetEnable() ) {
      if( names[i]->children_in_scene > 0 ) {
        if( names[i]->GetLocation() == UNNLOC_SCENE_LAST ) {
          names[i]->index_in_scene = ++index;
        } else if( names[i]->GetLocation() == UNNLOC_HIERARCHY_FIRST ) {
          // Handle First-In-Hierarchy nulls that don't have objects (ie: Lights/Camera on null)
          if( names[i]->index_in_scene == -1 )
            names[i]->index_in_scene = ++index;
        }
      }
    }
  }

  // Go back to the start of the file and clear out any errors
  in->seekg( 0 );
  in->clear();

  return last_index;
}

// Process():
//  UberNull the scene.
int UberNull::Process() {
  // Make sure the files are open
  if( !AreFilesOpen() )
    return UBERNULL_FILES_NOT_OPEN;

  // Begin processing
  char line[2048];

  unsigned int i;
  int  increment  = 0;
  int  index = 0;
  bool is_object = false;
  bool get_new_line = true;
  int  endbehavior_count = 0;  // the first EndBehavior is for the motion; everything else is envelopes (morph/dissolve/light/etc.)

  char name[1024] = "\255";     // Item Name
  int  output_index;

  int  last_index = FindUberNullIndex();

  // Make sure something will get UberNulled; otherwise, abort
  if( !GetUseMasterNull() ) {
    if( !GetUseNames() ) {
      return UBERNULL_NO_NULLS_ENABLED;
    } else {
      // Check for enabled children
      for( unsigned int i=0; i < names.NumElements(); i++ ) {
        if( names[ i ]->GetEnable() == true )
          break;
      }

      if( i == names.NumElements() ) {
        return UBERNULL_NO_NULLS_ENABLED;
      }

      // Some children are enabled; check at least one match
      for( i=0; i < names.NumElements(); i++ ) {
        if( names[ i ]->children_in_scene > 0 )
          break;
      }

      if( i == names.NumElements() ) {
        return UBERNULL_NO_NULLS_MATCHED;
      }
    }
  }

  // Main processing loop
  do {
    if( get_new_line )
      in->getline( line, 2048 );
    else 
      get_new_line = true;

    // Add the First-In-Scene Nulls
    if( strncmp( line, "FramesPerSecond", 15 ) == 0 ) {
      *out << line << endl;

      // Master Null
      if( use_master && (master_location == UBERNULL_FIRST ) ) {
        *out << endl;
        *out << UberNullObject[0] << master_name << endl;
        for( i=1; UberNullObject[i] != NULL; i++ )
          *out << UberNullObject[i] << endl;

        index++;
        increment++;
      }

      // Name Nulls
      for( i=0; i < names.NumElements(); i++ ) {
        if( names[i]->GetEnable() ) {
          if( names[i]->children_in_scene > 0 ) {
            if( names[i]->GetLocation() == UNNLOC_SCENE_FIRST ) {
              *out << endl;
              *out << UberNullObject[0] << names[i]->GetName() << endl;
              for( int j=1; UberNullObject[j+1] != NULL; j++ )
                *out << UberNullObject[j] << endl;
            
              if( use_master ) 
                *out << "ParentObject " << ubernull_index << endl;

              *out << UberNullObject[j] << endl;

              index++;
              increment++;
              // BIG NOTE:  The indices MUST be in sync with those in the instance, or else parenting loops will occur
              assert( index == names[i]->index_in_scene );
            }
          }
        }
      }
    } else {
      // Add the Last-In-Scene Name Nulls
      if( index == last_index ) {
        if( use_master && ( master_location == UBERNULL_LAST ) ) {
          *out << endl;
          *out << UberNullObject[0] << master_name << endl;
          for( int i=1; UberNullObject[i] != NULL; i++ )
            *out << UberNullObject[i] << endl;

          if( master_location == UBERNULL_LAST )
            *out << endl;

          index++;
          increment++;
          assert( index == ubernull_index );
        }

        // Name Nulls
        for( i=0; i < names.NumElements(); i++ ) {
          if( names[i]->GetEnable() ) {
            if( names[i]->children_in_scene > 0 ) {
              if( names[i]->GetLocation() == UNNLOC_SCENE_LAST ) {
                *out << endl;
                *out << UberNullObject[0] << names[i]->GetName() << endl;
                for( int j=1; UberNullObject[j+1] != NULL; j++ )
                  *out << UberNullObject[j] << endl;
            
                if( use_master ) 
                  *out << "ParentObject " << ubernull_index << endl;

                *out << UberNullObject[j] << endl;

                index++;
                increment++;
                // BIG NOTE:  The indices MUST be in sync with those in the instance, or else parenting loops will occur
                assert( index == names[i]->index_in_scene );
              }
            }
          }
        }

        // Increment Index again so we don't accidentially get stuck in this loop
        index++;
      }

      // Test for an item name
      if( strncmp( line, "LoadObject",    10 ) == 0 ) {
        strcpy( name, DirStrings::HasFile( &line[11] ) );
        is_object = true;
      } else if( strncmp( line, "AddNullObject", 13 ) == 0 ) {
        strcpy( name, DirStrings::HasFile( &line[14] ) );
        is_object = true;
      } else if( strncmp( line, "AddBone",        7 ) == 0 ) {
        strcpy( name, &line[8] );
        is_object = false;
      } else if( strncmp( line, "AddLight",       8 ) == 0 ) {
        strcpy( name, &line[9] );
        is_object = false;
      } else if( strncmp( line, "ShowCamera",    10 ) == 0 ) {
        name[0] = '\0';
        is_object = false;
      } else {
        name[0] = '\255';
        is_object = false;
      }

      if( (name[0] != '\255') && is_object ) {
        // Increment the Parent Object (or add a ParentObject line)
        index++;
        endbehavior_count = 0;

        // See if we should insert another null object here
        for( i=0; i < names.NumElements(); i++ ) {
          if( names[i]->GetEnable() ) {
            if( names[i]->children_in_scene > 0 ) {
              if( names[i]->GetLocation() == UNNLOC_HIERARCHY_FIRST ) {
                if( names[i]->index_in_scene == index ) {
                  *out << endl;
                  *out << UberNullObject[0] << names[i]->GetName() << endl;
                  for( int j=1; UberNullObject[j+1] != NULL; j++ )
                    *out << UberNullObject[j] << endl;
              
                  if( use_master ) 
                    *out << "ParentObject " << ubernull_index << endl;

                  *out << UberNullObject[j] << endl;
                  *out << endl;

                  // BIG NOTE:  The indices MUST be in sync with those in the instance, or else parenting loops will occur
                  assert( index == names[i]->index_in_scene );

                  index++;
                  increment++;
                }
              }
            }
          }
        }

        *out << line << endl;
        do {
          if( get_new_line )
            in->getline( line, 2048 );
          else 
            get_new_line = true;

          // Fix the MorphTarget index, if needed
          if( increment != 0 ) { // only bother with this if we have to increment by a non-zero amount
            if( strncmp( line, "MorphTarget", 11 ) == 0 ) {
              int target = atoi( &line[ 12 ] ) + increment;
              *out << "MorphTarget " << target << endl;
            } else if( strncmp( line, "TargetObject", 12 ) == 0 ) {
              int target = atoi( &line[ 12 ] ) + increment;
              *out << "TargetObject " << target << endl;
            } else {             // These are not the droids we're looking for; output original line
              *out << line << endl;
            }
          } else {
          // Default Output:  Just put the line out again.
            *out << line << endl;
          }

          if( strncmp( line, "EndBehavior", 11 ) == 0 ) {
            endbehavior_count++;
            if( endbehavior_count == 1 ) {
              do {
                if( get_new_line )
                  in->getline( line, 2048 );
                else 
                  get_new_line = true;

                if( strncmp( line, "ParentObject", 12 ) == 0 ) {
                  if( increment != 0 ) { // only bother with this if we have to increment by a non-zero amount
                    int parent = atoi( &line[13] ) + increment;
                    *out << "ParentObject " << parent << endl;
                  } else {               // 0 increment; just output the original line and stop looking
                    *out << line << endl;
                  }

                  break;               // Stop looking for a parent object from this point on
                }

                // Add a new ParentObject line, if needed
                if( ! ( (strncmp( line, "LockedChannels",    14 ) == 0 ) ||
                        (strncmp( line, "HLimits",            7 ) == 0 ) ||
                        (strncmp( line, "PLimits",            7 ) == 0 ) ||
                        (strncmp( line, "BLimits",            7 ) == 0 ) ||
                        (strncmp( line, "PivotPoint",        10 ) == 0 ) ) ) {  // We've passed where Parent Object would be (I think...); write it out

                  // Figure out what index to parent this item to, if any
                  if( use_master )
                    output_index = ubernull_index;
                  else
                    output_index = -1;

                  if( name[0] != '\0' ) {
                    for( i=0; i < names.NumElements(); i++ ) {
                      if( names[i]->GetEnable() ) {
                        if( names[i]->children_in_scene > 0 ) {
                          if( names[i]->TestForMatch( name ) ) {
                            output_index = names[i]->index_in_scene;
                            break;
                          }
                        }
                      }
                    }
                  }

                  // Output the item name
                  if( output_index != -1 )
                    *out << "ParentObject " << output_index << endl;

                  get_new_line = false;
                  break;
                } else {
                  *out << line << endl;
                }
              } while( !in->eof() && !in->fail() );
            }
          }

          // Check for the end of an item
          if( strncmp( line, "ShadowOptions", 13 ) == 0 )    // End of object
            break;

          if( strncmp( line, "ShadowCasting", 13 ) == 0 )    // End of light (more or less)
            break;

          if( strncmp( line, "ShadowType", 10 ) == 0 )
            break;

          if( strncmp( line, "DepthOfField", 12 ) == 0 )     // End of camera (more or less)
            break;

        } while( !in->eof() && !in->fail() );
        name[0] = '\255';
      } else {
        // Nothing we need here; just copy the line to the output file
        *out << line << endl;
      }
    }

  } while( !in->eof() && !in->fail() );

  return UBERNULL_OK;
}
