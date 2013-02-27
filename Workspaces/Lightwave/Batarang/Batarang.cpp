//
// Batarang.cpp
//
// Batarang is was designed for a VERY specific purpose.  When you create an
// expression in Relativity for a bone in a Lightwave object, and have it reference
// an object in a scene (in this case, the object the bones belong to), it works. 
// But as soon as you start cloning the object, the name changes to end in (1), (2),
// etc, causing the expressions to fail to find the object.
//
// Batarang will scan a Lightwave scene file that contains, say, 100 bats, all
// of which have 8 bones each with Relativity applied, and each expecting to find,
// say, bat_base.lwo, so they can use bat_base.lwo's position as a random seed to
// offset their wing-flap motion, and Batarang will look for a LoadObject or
// AddNullObject keyword.  It will then store the name of the object, and scan
// through all it's bones for a Relativity Motion Plug-in.  It will then replace
// all occurances of the object name with the object name and the numbered extension
// representing the occurance of the object in the scene file (ie: bat_base (1) for
// the first bat, (2) for the second, and so on).
//
// Now when you load the newly created scene into Relativity, everything is
// hunky-dorey and all bones find the right bat base objects.
//
// What a pain in the...
//
// History:
// - 1.1:  Added support for things besides bones.  The hew -h(ierarchy) flag can be
//          used when Relativity is calling on a particlular object in a hierarchy.
//          This is done in 2 passes.  During the first pass, the entire scene is
//          scanned and a hierarchial tree of all the objects in the scene is built.
//          This data includes the object name, objecty index and parent object index.
//          On the second pass, this list is used to determine which objects belong
//          to group (1), group (2), etc.  Note that no change will be performed if
//          only 1 instance of the base object exists.  This pass will actually output
//          the new scene file.
//         Note that when the -h option is used, the object name passed in is the
//          object at the base of the hierarchy.  This means that defualt bone-matching
//          mode is overridden by the -h option (ie: only one can be used at a time).
//

#include <portable/dirtools/dirtools.h>
#include <portable/stringtools/stringtools.h>
#include <string.h>
#include <stdio.h>

#include <fstream.h>
#include <iomanip.h>

#include "portable/dirtools/dirtools.h"
#include "BatarangHierarchy.h"

// Prototypes
void Help(void);

// Argument Template:
//  Batarang <source> <object> [<output>]
//   <source>:  source scene filename and path
//   <object>:  object name to look for
//  [<output>]: optional output name

// Un-comment and enter the Special Edition ID here
// #define SPECIAL_EDITION_CZAPARA
#define SPECIAL_EDITION_NETTER

void main( int argc, char **argv ) {
  cout << " Batarang --- Copyright 1999 TM Productions" << endl;
#ifdef SPECIAL_EDITION_CZAPARA
  cout << " -Special Chris Zapara (Flat Earth) Edition-" << endl;
#endif

#ifdef SPECIAL_EDITION_NETTER
  cout << " -Special Netter Digital Edition-" << endl;
#endif

  cout << " Version 1.1 Beta" << endl;

  // Check for too few args
  if( argc < 3 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Arguments 1 and 2 are always the Source and Match objects
  char * &source = argv[1];
  char * &object = argv[2];

  // Default Output Name
  char output[ MAX_PATH_LENGTH ];
  strcpy( output, source );
  char * ext = DirStrings::HasExtension( output ); // Find any extension
  if( ext )                                        // Strip the extension
    *ext = '\0';
  strcat( output, "-Bataranged" );                 // Append -UberNull
  ext = DirStrings::HasExtension( source );        // Find the original extension
  if( ext )                                        // Append the origianl extension to the output_name
    strcat( output, ext );

  // Optional Argument Parsing
  bool hierarchy_mode = false;
  bool display_tree   = false;
  for( int cur_arg = 3; cur_arg < argc; cur_arg++ ) {
    if( argv[cur_arg][0] != '-' ) {          // Make sure arguments start with '-'
      cerr << " ERROR:  Unknown Argument \"" << argv[cur_arg]
           << "\"; aborting" << endl;
    } else if( argv[cur_arg][1] == 'h' ) {   // Hierarchy keyword
      hierarchy_mode = true;
    } else if( argv[cur_arg][1] == 't' ) {   // Display Tree
      display_tree = true;
    } else if( argv[cur_arg][1] == 'd' ) {   // Output Scene
      strcpy( output, argv[cur_arg] );
    } else {
      cerr << " ERROR:  Unknown Argument \"" << argv[cur_arg]
           << "\"; aborting" << endl;
    }
  }

  // Turn off Display Tree if Hierarchy Mode is off
  if( hierarchy_mode == false )
    display_tree = false;

  // Test filename arguments
  if( DirStrings::HasPath( object ) ) {
    cerr << " Object should be a filename only -- no path!" << endl;
    cerr << " Type Batarang ? for help" << endl;
    exit(20);
  }

  strlwr( object );

  // Test the args
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type Batarang ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type Batarang ? for help" << endl;
    exit(20);
  }

  // Open the Input and Output files
  ifstream in( source, ios::nocreate );
  if( !in ) {
    cerr << " ERROR: Unable to open source scene \"" << source << "\"!" << endl;
    cerr << " File may be exclusively locked by another program; aborting" << endl;
    exit(20);
  }

  ofstream out( output );
  if( !out ) {
    cerr << " ERROR: Unable to open output scene \"" << output << "\"!" << endl;
    cerr << " File may be in use by another program or write-protected; aborting" << endl;
    exit(20);
  }

  char line[8192];
  char *temp;

  BatarangHierarchy * current = NULL;
  BatarangHierarchy   base( source, BH_ITEM_SCENE );

  DynArray<BatarangHierarchy *> item_list;
  DynArray<BatarangHierarchy *> base_item_list;

  int  object_index = 0;
  int  light_index  = 0;
  int  base_count   = 0;

  bool in_bone = false;

  unsigned int i, j;

  BatarangHierarchy *h_base = NULL;
  // Hierarchy Scan
  if( hierarchy_mode ) {
    cerr << " Building Hierarchy Tree... " << endl;

    do {
      in.getline( line, 8192 );
      if( (in.bad() || in.fail()) && !in.eof() ) {
        cerr << " ERROR:  File error reading from source \""
             << source << "\"; aborting" << endl;
        exit(20);
      }

      if( strnicmp( line, "LoadObject", 10 ) == 0 ) {
        in_bone = false;
        temp = DirStrings::HasFile( &(line[11]) );
        current = new BatarangHierarchy( temp, BH_ITEM_OBJECT, ++object_index );
        item_list.Add( current );

        if( stricmp( temp, object ) == 0 ) {
          base_count++;

          if( base_count == 1 )
            h_base = current;
        }
      }

      if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
        in_bone = false;
        temp = DirStrings::HasFile( &(line[14]) );
        current = new BatarangHierarchy( temp, BH_ITEM_NULL_OBJECT, ++object_index );
        item_list.Add( current );

        if( stricmp( temp, object ) == 0 ) {
          base_count++;

          if( base_count == 1 )
            h_base = current;
        }
      }

      if( strnicmp( line, "LightName", 9 ) == 0 ) {
        temp = DirStrings::HasFile( &(line[10]) );
        current = new BatarangHierarchy( temp, BH_ITEM_LIGHT, ++light_index );
        item_list.Add( current );
      }

      if( strnicmp( line, "AddBone", 7 ) == 0 )
        in_bone = true;

      if( !in_bone ) {                  // We only store the parent object for items that aren't bones
        if( strnicmp( line, "ParentObject", 12 ) == 0 ) {
          int parent = atoi( &(line[13]) );
          if( current != NULL )
            current->SetParentIndex( parent );
        }
      }
    } while( !in.eof() );

    // Build Hierarchy
    for( i=0; i < item_list.NumElements(); i++ ) {
      if( item_list[i]->GetParentIndex() == 0 )  // No parent; add to the base list
        base.children.Add( item_list[i] );
      else {                                     // Valid parent; add to the parent's list
        int parent_index = item_list[i]->GetParentIndex() - 1;
        item_list[i]->SetParent( item_list[ parent_index ] );
        item_list[ parent_index ]->children.Add( item_list[i] );
      }
    }

    // Build a list of all children of the base item
    h_base->BuildItemList( base_item_list );
    BatarangHierarchy::IndexItems( item_list );

    // Display hierarchy, if applicable
    if( display_tree ) {
      base.OutputAsText( 4 );
      cerr << "  Base Object Count:  " << base_count << endl;
    }

    // Make sure the hierarchy base was found
    if( h_base == NULL ) {
      cerr << " ERROR:  Hierarchy base \"" << object << "\" not found in scene \""
           << source << "\"; Nothing to do!" << endl;
      exit(20);
    }

    // Make sure more than one instance of the base was found;
    if( base_count == 1 ) {
      cerr << " ERROR:  Only one occurance of the hierarchy base object \"" << object << "\" found in scene \""
           << source << "\"; Nothing to do!" << endl;
      exit(20);
    }
  }

  // Variable Setup
  int  increment = 0;                                           // LW scenes count from 1; we increment immediately when we find a match
  int  cur_index = 0;                                           // Current item index
  int  item_name_start = 0;                                     // Start of an item name after the AddNullObject or LoadObject keyword.
  bool object_matched = false;                                  // True if the object matches the base or and object in the base's hierarchy

  bool found_any_matches = false;                               // Becomes true as soon as 1 object matched

  char * start_here = NULL;
  char * name_here  = NULL;

  // Jump back to the start of the file
  in.clear();
  in.seekg( 0, ios::beg );

  // Search Loop
  cerr << " Processing..." << endl;
  do {
    in.getline( line, 8192 );                                  // Get a line from the file
    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }

    // Look for a new object
    item_name_start = 0;
    if( strnicmp( line, "LoadObject",    10 ) == 0 ) {
      item_name_start = 11;
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 )
      item_name_start = 14;

    object_matched = false;
    if( item_name_start > 0 ) {
      cur_index++;
      BatarangHierarchy * ancestor;
      // See if this object is the one we're looking for (both bone and -h mode)
      temp = DirStrings::HasFile( &(line[ item_name_start ]) );
      if( stricmp( temp, object ) == 0 ) {
        increment++;                                            // Matched; Increment the incrementer
        object_matched = true;
      } else {
        if( hierarchy_mode ) {
          // Hierarchy Mode:  Look for a matching item in the entire tree.
          ancestor = base.FindIndexInScene( cur_index );
          if( ancestor != NULL ) {
            ancestor = ancestor->HasAncestorNamed( object );
            if( ancestor != NULL ) {
              increment = ancestor->GetCopyNumber();
              object_matched = true;
            }
          }
        }
      }

      // output the AddNullObject/LoadObject line
      out << line << endl;

      if( object_matched ) {
        do {
          in.getline( line, 8192 );                              // Get a line from the file
          if( (in.bad() || in.fail()) && !in.eof() ) {
            cerr << " ERROR:  File error reading from source \""
                 << source << "\"; aborting" << endl;
            exit(20);
          }

          // The following is a hack to allow Arty's Parent-It Lite script to work with Batarang
          if( strnicmp( line, "Script 56.P\\LAYOUT\\LSCRIPT-IA\\PARENT_LITE.LS", strlen( "Script 56.P\\LAYOUT\\LSCRIPT-IA\\PARENT_LITE.LS") ) == 0 ) {
            out << line << endl;
            in.getline( line, 8192 );                            // Get the object name from the file
            out << line << " (" << increment << ")" << endl;
            in.getline( line, 8192 );                            // Get another line so the rest of the program is happy
          }

          // Follower Support
          if( strnicmp( line, "Plugin ItemMotionHandler 1 LW_Follower", strlen( "Plugin ItemMotionHandler 1 LW_Follower") ) == 0 ) {
            // This is a bit hacky.  We're just assuming that this line
            //  reads Follower <item name> and fixing it.  That's all.
            out << line << endl;
            in.getline( line, 8192 );                            // Get the Follower keyword and the object name from the file
            out << line << " (" << increment << ")" << endl;
            in.getline( line, 8192 );                            // Get another line so the rest of the program is happy
          }

          if( strnicmp( line, "ShadowOptions", 12 ) == 0 ) {     // End of object; stop looking
            out << line << endl;
            break;
          }

          char *start_here = line;
          bool  break_loop = false;
          do {  // Handle >1 occurance of name in expression
            name_here = NULL;
            if( hierarchy_mode ) {
              // Hierarchy Mode
              unsigned int sh_loc = strlen( start_here );

              // Find a , or (
              for( i=0; i < sh_loc; i++ ) {
                if( (start_here[i] == '(') || (start_here[i] == ',') ) {
                  // See if an object name is here
                  for( j=0; j < item_list.NumElements(); j++ ) {
                    if( strnicmp( &(start_here[i+1]), item_list[j]->GetName(), strlen( item_list[j]->GetName() ) ) == 0 ) {
                      name_here = &(start_here[i+1]);
                      break_loop = true;
                      break;
                    }
                  }
                }

                if( break_loop ) {
                  break_loop = false;
                  break;
                }
              }
            } else {
              // Default (Bone) Mode
              name_here = StringTools::stristr( start_here, object );
            }

            // Name not found; skip it.
            if( name_here == NULL )
              break;

            found_any_matches = true;

            // Look for a ',', which always follows an item name
            char * name_end  = strchr( name_here, ',');

            if( name_end == NULL ) {
              cerr << " ERROR:  , missing from expression \""
                   << line << "\" after item name; skipping..." << endl;
            } else {
              char end_buffer[8192]; 
              strcpy( end_buffer, name_end );
              name_end[0] = '\0';

              char ext_buffer[20];
              sprintf( ext_buffer, " (%d)", increment );
              sprintf( start_here, "%s%s%s", start_here, ext_buffer, end_buffer );

              start_here = name_end;
            }
          } while( true );

          // Output the line (changed or original)
          out << line << endl;
      
        } while( !in.eof() );
      }
    } else {
      // Nothing we want; output the original line
      out << line << endl;
    }
  } while( !in.eof() );

  if( found_any_matches == true ) {
    cerr << " Bateranged output scene \"" << output
         << "\" created sucessfully! " << endl;
  } else {
    cerr << " No matches for object \"" << object << "\" found in source \""
         << source << "\"!" << endl;
  }
}


// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Searches for all occurances of <object> in the input scene,\n"
       << "   and edit's all the bones that use Relativity and reference\n"
       << "   this object, and changes the expression to represent the\n"
       << "   current index of the object in the scene." << endl;
  cout << endl;
  cout << "  Usage:   Batarang <source> <object> [-h] [-tree] [-o <output>] " << endl;
  cout << "    <source>:       source scene filename and path" << endl; 
  cout << "    <object>:       object name to look for (without path)" << endl;
  cout << "   [-h]:            hierarchy mode.  If used, the object name is the base\n"
       << "                     of the hierarchy to search through; otherwise, it's\n"
       << "                     the object to change the bones of." << endl;
  cout << "   [-tree]:         Outputs a hierarchy tree of the entire scene to the\n"
       << "                     screen. Only valid if -h is specified" << endl;
  cout << "   [-o <output>]:   optional output scene" << endl;

  exit(0);
}

