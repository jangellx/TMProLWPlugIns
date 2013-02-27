//
// MultiLFS.cpp
//
// MultiLFS stands for Multiple Load From Scene.  Basically, you pass
// in a source scene, a scene to load into it, and the number of times
// to load it in.  This makes it easy to load 100 bats into a scene.
//

#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>

// Prototypes
void Help(void);

// Argument Template:
//  MultiLFS <source> <lfs> <count> [-o <output>] [-lights] [-psm]
//   <source>:     source scene filename and path
//   <lfs>:        load-from scene filename and path
//   <count>:      number of times to load from scene
//  [-o <output>]: optional output name
//  [-lights]:     optionally load lights
//  [-psm]:        optionally modify any particle storm data found

const char *MultiLFS_Version  = "1";
const char *MultiLFS_Revision = "0";

void main( int argc, char **argv ) {
  cout << " MultiLFS V" << MultiLFS_Version << "." << MultiLFS_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few args
  if( argc < 4 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * &source = argv[1];
  char * &lfs    = argv[2];
  int    count   = atoi( argv[3] );
  int    lights  = false;
  bool   psm     = false;
  char   output[ MAX_PATH_LENGTH ] = "";

  for( int i=4; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
      case 'o':
        // Output Scene Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          cerr << " Type MultiLFS ? for help" << endl;
          exit(20);
        }
        strcpy( output, argv[i] );
        break;

      case 'l':
        // Load Lights Test
        lights = true;
        break;

      case 'p':
        // Enumerate Particle Storm information
        psm = true;
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type MultiLFS ? for help" << endl;
        exit(20);
      }
    } else {
      cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
      cerr << " Type MultiLFS ? for help" << endl;
      exit(20);
    }
  }

  // Test the args
  if( count < 1 ) {
    cerr << " ERROR: The count must be non-zero!" << endl;
    cerr << " Type MultiLFS ? for help" << endl;
    exit(20);
  }

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-MultiLFSed" );                 // Append -MultiLFS
    ext = DirStrings::HasExtension( source );        // Find the original extension
    if( ext )                                        // Append the origianl extension to the output_name
      strcat( output, ext );
  } 

  // Test the paths
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type MultiLFS ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type MultiLFS ? for help" << endl;
    exit(20);
  }

  // Open the Input and Output files
  ifstream in( source, ios::nocreate );
  if( !in ) {
    cerr << " ERROR: Unable to open source scene \"" << source << "\"!" << endl;
    cerr << " File may be exclusively locked by another program; aborting" << endl;
    exit(20);
  }

  ifstream lfs_in( lfs, ios::nocreate );
  if( !lfs_in ) {
    cerr << " ERROR: Unable to open load-from scene \"" << lfs << "\"!" << endl;
    cerr << " File may be exclusively locked by another program; aborting" << endl;
    exit(20);
  }

  ofstream out( output );
  if( !out ) {
    cerr << " ERROR: Unable to open output scene \"" << output << "\"!" << endl;
    cerr << " File may be in use by another program or write-protected; aborting" << endl;
    exit(20);
  }

  // Set up some variables
  char line[8192], lfs_line[8192];
  int  orig_object_count = 0;
  int  object_count = 0, lfs_object_count = 0;
  int  light_count  = 0, lfs_light_count  = 0;

  streampos lfs_end_of_objects = 0;

  DynArray< char * > psm_names;
  DynArray< int >    psm_counts;

  // The Loop
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }

    // Look for a new object
    if( (strnicmp( line, "LoadObject",    10 ) == 0) ||
        (strnicmp( line, "AddNullObject", 13 ) == 0) )
      object_count++;

    // Look for the end of the object data (ie: AmbientColor)
    if( strnicmp( line, "AmbientColor", 12 ) == 0 ) {
      cout << "  Adding Objects..." << endl;
      orig_object_count = object_count;
      // Load the object data from the scene <count> times
      for( int j=0; j < count; j++ ) {
        lfs_in.seekg( 0 );       // Seek to the start of the file
        lfs_object_count = 0;    // Reset the LFS object counter

        bool in_object = false;
        bool in_bone   = false;
        bool in_psm    = false;
        int  psm_match = -1;
        bool output_line = true;

        do {
          lfs_in.getline( lfs_line, 8192 );
          if( (in.bad() || in.fail()) && !in.eof() ) {
            cerr << " ERROR:  File error reading from load-from scene \""
                 << lfs << "\"; aborting" << endl;
            exit(20);
          }

          // Look for the end of the object data (AmbientColor)
          if( strnicmp( lfs_line, "AmbientColor", 12 ) == 0 )
            break;

          // Look for an object
          if( (strnicmp( lfs_line, "LoadObject",    10 ) == 0) ||
              (strnicmp( lfs_line, "AddNullObject", 13 ) == 0) ) {
            lfs_object_count++;
            in_object = true;
            in_bone   = false;
          }

          // See if we're found any bones
          if( strnicmp( lfs_line, "AddBone", 7 ) == 0 )
            in_bone = true;

          // Increment the Parent Object, but only for objects (not bones)
          if( (strnicmp( lfs_line, "ParentObject", 12 ) == 0) && !in_bone ) {
            int parent = atoi( &lfs_line[12] );
            sprintf( lfs_line, "ParentObject %d", parent + object_count );
          }

          // Increment the Morph Target
          if( strncmp( lfs_line, "MorphTarget", 11 ) == 0 ) {
            int target = atoi( &lfs_line[ 12 ] );
            sprintf( lfs_line, "MorphTarget %d", target + object_count );
          }

          // Check for Particle Storm Data
          if( psm ) {
            if( strncmp( lfs_line, "Plugin", 6 ) == 0 ) {
              if( strstr( lfs_line, "ParticleStorm" ) != 0 ){
                in_psm = true;
              }
            }

            if( in_psm ) {
              // Check for end of PS data
              if( strncmp( lfs_line, "EndPlugin", 9 ) == 0 )
                in_psm = false;

              // Check for a new PSM, or increment from an old one.
              if( strncmp( lfs_line, "MotionDataFile", 14 ) == 0 ) {
                psm_match = -1;
                for( int i=0; i < (int)psm_names.NumElements(); i++ ) {
                  if( stricmp( psm_names[i], &(lfs_line[15]) ) == 0 ) {
                    psm_match = i;
                    break;
                  }
                }

                // Not loaded yet; add it as a new one.
                if( psm_match == -1 ) {
                  psm_names.Add( strdup( &(lfs_line[15]) ) );
                  psm_match = psm_names.NumElements()-1;
                  psm_counts.Add( -1 );
                }
              }

              // Check for MotionIndex and increment it.
              if( strncmp( lfs_line, "MotionIndex", 11 ) == 0 ) {
                if( psm_counts[ psm_match ] == -1 ) // No index yet assigned; get it from the file
                  psm_counts[ psm_match ] = atoi( &(lfs_line[12]) );

                out << "MotionIndex " << psm_counts[ psm_match ]++ << endl;
                output_line = false;
              }
            }
          }

          // Output the line if we're in an object
          if( output_line ) {
            if( in_object ) {
              out << lfs_line << endl;
            }
          } else {
            output_line = true;
          }

          if( strnicmp( lfs_line, "ShadowOptions", 13 ) == 0 ) {
            in_object = false;
            in_bone   = false;
            out << endl;
          }
        
        } while( !lfs_in.eof() );

        // Add the lfs object count to the base scene object count for the next pass
        object_count += lfs_object_count;
      }
      lfs_end_of_objects = lfs_in.tellg();
    }

    // Look for a new light
    if( strnicmp( line, "AddLight", 8 ) == 0 )
      light_count++;

    if( lights ) { // Only load lights if the user asked us to
      // Look for the end of the light data (ie: ShowCamera)
      if( strnicmp( line, "ShowCamera", 10 ) == 0 ) {
        cout << "  Adding Lights..." << endl;
        object_count = orig_object_count;
        // Load the light data from the scene <count> times
        for( int j=0; j < count; j++ ) {
          lfs_in.seekg( lfs_end_of_objects, ios::beg ); // Seek to the start of the file
          lfs_light_count = 0;                          // Reset the LFS object counter

          bool in_light = false;

          do {
            lfs_in.getline( lfs_line, 8192 );
            if( (lfs_in.bad() || lfs_in.fail()) && !lfs_in.eof() ) {
              cerr << " ERROR:  File error reading from load-from scene \""
                   << lfs << "\"; aborting" << endl;
              exit(20);
            }

            // Look for the end of the light data (ShowCamera)
            if( strnicmp( lfs_line, "ShowCamera", 10 ) == 0 ) {
              break;
            }

            // Look for a light
            if( strnicmp( lfs_line, "AddLight", 8 ) == 0 ) {
              lfs_light_count++;
              in_light = true;
            }

            // Increment the Parent Object
            if( strnicmp( lfs_line, "ParentObject", 12 ) == 0 ) {
              int parent = atoi( &lfs_line[12] );
              sprintf( lfs_line, "ParentObject %d", parent + object_count );
            }

            // Increment the Target Object
            if( strnicmp( lfs_line, "TargetObject", 12 ) == 0 ) {
              int target = atoi( &lfs_line[12] );
              sprintf( lfs_line, "TargetObject %d", target + object_count );
            }

            out << lfs_line << endl;
          } while( !lfs_in.eof() );

          // Add the lfs object count to the base scene object count for the next pass
          light_count += lfs_light_count;
          object_count += lfs_object_count;
        }
      }
    }

    out << line << endl;

  } while( !in.eof() );

  // Free the strdup()ed PSM Names
  if( psm ) {
    if( psm_names.NumElements() > 0 )
      cout << " PSMs found:" << endl;
    else
      cout << " No PSMs found in load-from scene!" << endl;

    for( i=0; i < (int)psm_names.NumElements(); i++ ) {
      cout << "   Enums: " << setw(10) << psm_counts[i] << "  Name: " << psm_names[i] << endl;
      free( psm_names[i] );
    }
  }

  cout << "  Successfully added " << count << " instances of \"" << lfs
       << "\" to " << output << "\"!" << endl;

}


// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Loads a scene into another scene the number of times specified." << endl;
  cout << endl;
  cout << "  Usage:   MultiLFS <source> <lfs> <count> [-o <output>] [-lights] [-psm]" << endl;
  cout << "    <source>:     source scene filename and path" << endl; 
  cout << "    <lfs>:        load-from scene filename and path" << endl;
  cout << "    <count>:      number of times to load from scene" << endl;
  cout << "   [-o <output>]: optional output name" << endl;
  cout << "   [-lights]:     optionally load lights as well as objects" << endl;
  cout << "   [-psm]:        optionally modify any particle storm data found" << endl;

  exit(0);
}
