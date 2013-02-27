//
// PSMFreezer
//
// Scans a Lightwave scene file and replaces all motions of Particle Storm
//  with a LW motion derived from the referenced PSM file.  This effectively
//  eliminates the need for the Particle Storm Motion Plug-In.
//
// Other functions
//  - Drift:  Adds a culmulative drift per frame on each axis.  Useful
//     to ensure that the particle will align to path.
//  - Copy Frame 1 to Frame 0:  Frame 0 is usually position 0, 0, 0.
//     This will make frame 0 contain the data of frame 1.
//  - Shift Motion:  Shifts the motion of all children of the particle
//     to when the particle first moves.  Useful to make bats flap once
//     the particle starts moving.  Both distance and frame offset can
//     be provided.
//  - Tree:  Outputs a scene tree, for debugging.
//

#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>
#include <math.h>

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>
#include <portable/pfstream/pfstream.h>

#include "../Batarang/BatarangHierarchy.h"
#include "FreezerHierarchy.h"

//#define debug_psm_loader
//#define debug_psm_frames
//#define debug_psm_birthtest1
//#define debug_psm_birthtest2
//#define debug_psm_birthtest_frame

// Prototypes
       void Help(void);
       int  FindNumPSMFrames( ifstream &psm );
       void SkipFLAG( ifstream &psm );
       void ShiftMotion( pifstream &in, ofstream &out, int shift );

inline void MtoI( float  &value );
inline void MtoI( double &value );
inline void MtoI( long   &value );
inline void MtoI( int    &value );
inline void MtoI( short  &value );
inline void MtoI( unsigned long  &value );
inline void MtoI( unsigned int   &value );
inline void MtoI( unsigned short &value );

// Argument Template:
//  PSMFreezer <source> [-o <output>] [-c <path> [-d <x> <y> <z>] [-1] [-align] [-s <dist> <offset> [-t]
//   <source>:                      source scene filename and path
//  [-o <output>]:                  optional output name
//  [-c[ontent] <path>]:            contect directory; needed to find relative-path PSMs
//  [-d[rift] <x> <y> <z>]:         amount of drift per frame on X, Y and Z.  Useful for Align To Path when the particles haven't actually moved yet.
//  [-1[to0]]:                      copies frame 1 in the PSM (usually position 0,0,0) to frame 0
//  [-a[lign]]:                     turns on Align To Path on the particle object
//  [-s[hift] <distance> <offset>]: distance the particle has to move before the hierarchy should start animating.  Offset is how many frames after the motion to start animating
//  [-t[ree]]:                      display hieracrchy tree.  Only works with -s.


const char *PSMFreezer_Version  = "1";
const char *PSMFreezer_Revision = "0";

void main( int argc, char **argv ) {
  cout << " PSMFreezer V" << PSMFreezer_Version << "." << PSMFreezer_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few args
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * &source      = argv[1];             // Source Path
  char   output[ MAX_PATH_LENGTH ] = "";     // Output Path
  char * content = NULL;                     // Content Dir Path
  bool   do_drift      = false;              // Drift toggle
  double drift[3]      = { 0.0, 0.0, 0.0 };  // Amount of drift per frame to add to the PSM'ed data
  bool   copy1to0      = false;              // Copy Frame 1 from the PSM to Frame 0.
  bool   display_tree  = false;              // Display hierarchy tree
  bool   align_to_path = false;              // If we should add the PathAlignment keyword to the paritcle

  bool   do_shift      = false;              // If we should do the shift
  double shift_dist    = 0.0;                // Shift Distance
  int    shift_offset  = 0;                  // Number of frames to offset the shift

  for( int i=2; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
        // Align To Path Test
      case 'a':
        align_to_path = true;
        break;

      case 'c':
        // Content Dir Test
        if( ++i >= argc ) {
          cerr << " ERROR: No content dir following argument -c!" << endl;
          cerr << " Type PSEnumerate ? for help" << endl;
          exit(20);
        }
        content = argv[i];
        break;

      case 'd':
        // Drift Test
        if( i + 3 >= argc ) {
          cerr << " ERROR: X, Y and Z drift must follow -d!" << endl;
          cerr << " Type PSEnumerate ? for help" << endl;
          exit(20);
        }

        do_drift = true;
        drift[0] = atof( argv[++i] );
        drift[1] = atof( argv[++i] );
        drift[2] = atof( argv[++i] );
        break;

        // Copy 1 to 0 Test
      case '1':  // That's a 'one', not an 'el'
        copy1to0 = true;
        break;

      case 'o':
        // Output Scene Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          cerr << " Type PSEnumerate ? for help" << endl;
          exit(20);
        }
        strcpy( output, argv[i] );
        break;

      case 's':
        // Shift Distance
        if( i+2 >= argc ) {
          cerr << " ERROR: No shift distance following argument -s!" << endl;
          cerr << " Type PSEnumerate ? for help" << endl;
          exit(20);
        }
        shift_dist   = atof( argv[++i] );
        shift_offset = atoi( argv[++i] );
        do_shift = true;
        break;

      case 't':
        display_tree = true;
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type PSEnumerate ? for help" << endl;
        exit(20);
      }
    } else {
      cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
      cerr << " Type PSEnumerate ? for help" << endl;
      exit(20);
    }
  }

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-PSMFrozen" );                  // Append -PSMFrozen
    ext = DirStrings::HasExtension( source );        // Find the original extension
    if( ext )                                        // Append the origianl extension to the output_name
      strcat( output, ext );
  } 

  // Test the paths
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type PSEnumerate ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type PSEnumerate ? for help" << endl;
    exit(20);
  }

  // Open the Input and Output files
  pifstream in( source, ios::nocreate );
  if( !in ) {
    cerr << " ERROR: Unable to open source scene \"" << source << "\"!" << endl;
    cerr << " File may be exclusively locked by another program; aborting" << endl;
    exit(20);
  }
  in.GuessEOLType();

  ofstream out( output );
  if( !out ) {
    cerr << " ERROR: Unable to open output scene \"" << output << "\"!" << endl;
    cerr << " File may be in use by another program or write-protected; aborting" << endl;
    exit(20);
  }

  // Set up some variables
  char line[8192];
  int  psm_match   = -1;
  bool in_psm      = false;
  bool output_line = true;

  DynArray< char * > psm_names;
  DynArray< int >    psm_counts;

  streampos source_motion_pos   = -1;      // Where the motion is in the source file
  streampos source_motion_end   = -1;      // Where the end of the motion is in the source file
  streampos source_pstorm_start = -1;      // Start of the Particle Storm Plug-in Data

  streampos output_object_pos   = -1;      // The AddNullObject/LoadObject line output to the file
  streampos output_motion_pos   = -1;      // Where the motion is in the output file
  streampos end_of_motion       = -1;      // End of the output motion

  int  psm_index;                          // Index of this item in the PSM
  char psm_file[512];                      // PSM's filename
  char object_name[512];                   // Filename of the object which contains this PSM
  int  psm_scan_count = 0;                 // Number of times a particular PSM has been scanned

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------
  FreezerHierarchy * current = NULL;
  FreezerHierarchy   base( source, BH_ITEM_SCENE );

  DynArray<FreezerHierarchy *> item_list;
  DynArray<FreezerHierarchy *> base_item_list;

  int  object_index = 0;
  int  light_index  = 0;

  bool   in_bone = false;
  char * temp    = NULL;

  // Hierarchy Scan
  if( do_shift ) {
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
        current = new FreezerHierarchy( temp, BH_ITEM_OBJECT, ++object_index );
        item_list.Add( current );
      }

      if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
        in_bone = false;
        temp = DirStrings::HasFile( &(line[14]) );
        current = new FreezerHierarchy( temp, BH_ITEM_NULL_OBJECT, ++object_index );
        item_list.Add( current );
      }

      if( strnicmp( line, "LightName", 9 ) == 0 ) {
        temp = &(line[10]);
        current = new FreezerHierarchy( temp, BH_ITEM_LIGHT, -(++light_index) );
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

      // Particle Storm Checks
      if( strncmp( line, "Plugin", 6 ) == 0 ) {
        if( strstr( line, "ParticleStorm" ) != 0 ) {
          in_psm = true;
          current->HasPSM( true );
        }
      }
      if( in_psm ) {
        // Check for end of PS data
        if( strncmp( line, "EndPlugin", 9 ) == 0 )
          in_psm = false;

        // Check for a new PSM, or increment from an old one.
        if( strncmp( line, "MotionDataFile", 14 ) == 0 ) {
          // Tack on the Content Dir
          if( content == NULL )
            psm_file[0] = '\0';
          else
            strcpy( psm_file, content );

          DirStrings::AddPathPart( psm_file, &(line[16]) ); // Skip both
          psm_file[ strlen( psm_file ) - 1 ] = '\0';        //  quotes
        }

        // Check for MotionIndex
        if( strncmp( line, "MotionIndex", 11 ) == 0 ) {
          psm_index = atoi( &(line[12]) );
          cerr << "    Particle Index:  " << psm_index << endl;

          // We now have enough data to freeze the PSM
          try {
            // Test for PSM existance
            if( !DirInfo::Exists( psm_file ) ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  The PSM file \"" << psm_file << "\" doesn't exist!" << endl;
              throw( false );
            }

            // Open the PSM file
            ifstream psm( psm_file, ios::binary );
            if( !psm ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  Unable to open file \"" << psm_file << "\"!" << endl;
              throw( false );
            }

            // Make sure it really is a PSM file
            psm.ignore( 8 );
            psm.read( line, 4 );
            if( strnicmp( line, "PSM2", 4 ) != 0 ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  \"" << psm_file << "\" isn't a Particle Storm 2 PSM file!" 
                   << "  (header:  " << line << ")" << endl;
              throw( false );
            }

            // Find out the size of the first chunk
            unsigned long chunk_size;

            // Header Data
            psm.ignore( 14 );
            streampos first_frame_pos = psm.tellg();
            streampos chunk_start = -1;

            // Figure out how many frames are in the PSM
            int frames_per_particle = FindNumPSMFrames( psm );
            if( frames_per_particle < 0 ) {
              cerr << " ERROR:  File error reading from PSM \""
                   << psm_file << "\" while counting frames; skipping" << endl;
              throw( false );
            }

            if( frames_per_particle == 0 ) {
              cerr << " ERROR:  PSM \""
                   << psm_file << "\" contains no frames; skipping" << endl;
              throw( false );
            }

            float birthxyz[3] = {0.0f, 0.0f, 0.0f};
            float xyz[3];
            for( int frames = 0; /*NULL*/; frames++ ) {
              // Read in the chunk data
              chunk_start = psm.tellg();
              psm.get( line, 5 );
              psm.read( (char *)(&chunk_size), 4 );
              MtoI(chunk_size);

              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from PSM \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;


              // Read in the POSN data
              psm.ignore( 16 );   // Skip FRFL and FRAM
              psm.get( line, 5 );
              if( strncmp( line, "POSN", 4 ) != 0 ) {
                cerr << " ERROR:  Expected POSN; found \""
                     << line << "\" while scanning PSM; aborting" << endl;
                exit(20);
              }

              int posn_size;
              psm.read( (char *)(&posn_size), 4 );
              MtoI(posn_size);

              psm.seekg( 12 * psm_index, ios::cur );        // Read in just the one particle's data
              psm.read( (char *)(&xyz), 12 );
              MtoI(xyz[0]);
              MtoI(xyz[1]);
              MtoI(xyz[2]);

              if( current->GetBirthFrame() < 0 ) {      // <0 means we haven't found the birthframe yet
                // See if the particle is born yet.  This occurs when the
                //  position is non-zero on all axes
                if( ( !((xyz[0] < 0.00005) && (xyz[0] > -0.00005)) ) ||
                    ( !((xyz[0] < 0.00005) && (xyz[0] > -0.00005)) ) ||
                    ( !((xyz[0] < 0.00005) && (xyz[0] > -0.00005)) ) ) {
                  current->SetBirthFrame( frames );
                  birthxyz[0] = xyz[0];
                  birthxyz[1] = xyz[1];
                  birthxyz[2] = xyz[2];
#ifdef debug_psm_birthtest1
                  cerr << "      Birth XYZ:  " << birthxyz[0] << " "
                                               << birthxyz[1] << " "
                                               << birthxyz[2] << endl;
#endif
                  cerr << "      Birth Frame:  " << current->GetBirthFrame() << endl;
                }
              } else {
                // If the particles is born and has moved more than 
                //  shift_dist, we have found the movement frame.

#ifdef debug_psm_birthtest_frame
                cerr << "      -- Frame:  "  << frames << endl;
#endif

#ifdef debug_psm_birthtest2
                cerr << "      Birth XYZ:  " << birthxyz[0] << " "
                                             << birthxyz[1] << " "
                                             << birthxyz[2] << endl;
                cerr << "      This XYZ:   " << xyz[0] << " "
                                             << xyz[1] << " "
                                             << xyz[2] << endl;
#endif
                double dist = sqrt( ((xyz[0] - birthxyz[0]) * (xyz[0] - birthxyz[0])) +
                                    ((xyz[1] - birthxyz[1]) * (xyz[1] - birthxyz[1])) +
                                    ((xyz[2] - birthxyz[2]) * (xyz[2] - birthxyz[2])) );
                if( dist > shift_dist ) {
                  current->SetMovementFrame( frames + shift_offset );  // Subtract the offset off here


                  cerr << "      Movement Frame:    " << setw(4) <<  current->GetMovementFrame() << ";  Offset By " << shift_offset
                       << ";    Shift/Distance:  " << shift_dist << "/" << dist << endl;
                  break;
                }
              }
            

              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from psm \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;

              // Scan to the next chunk
              int dummy = psm.tellg();
              psm.seekg( abs( chunk_size - (psm.tellg() - chunk_start ) + 8 ), ios::cur );
            }
          
            psm.close();

          } catch( bool a ) {
            ; // Catch errors here, if needed
          }
        }
      }
    } while( !in.eof() );

    // Build Hierarchy
    for( i=0; i < (int)item_list.NumElements(); i++ ) {
      if( item_list[i]->GetParentIndex() == 0 ) {  // No parent; add to the base list
        base.children.Add( item_list[i] );
      } else {                                     // Valid parent; add to the parent's list
        int parent_index = item_list[i]->GetParentIndex() - 1;
        item_list[ parent_index ]->children.Add( item_list[i] );
        item_list[i]->SetParent( item_list[ parent_index ] );
      }
    }

    // Display hierarchy, if applicable
    if( display_tree ) {
      cerr << "  -------------------- Scene Hierarchy --------------------- " << endl;
      base.OutputAsText( 4 );
    }

    // Go back to the start of the source file
    in.clear();
    in.seekg( 0, ios::beg );
  }
  

// --------------------------------------------------------------

// --------------------------------------------------------------

// --------------------------------------------------------------

  // The Loop
  FreezerHierarchy *parent_particle = NULL;  // The ancestor of the current item that has particle data.  NULL if no parent has such data
  object_index = 0;

  int  last_frame = 0;
  bool found_last_frame = false;
  
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }

    if( strnicmp( line, "LastFrame", 9 ) == 0 ) {
      if( !found_last_frame ) {
        found_last_frame = true;
        last_frame = atoi( &(line[10]) );
#ifdef debug_psm_loader
        cerr << "    Last Frame:  " << last_frame << endl;
#endif
      }
    }

    // Look for an object, grab it's name and increment the object_index
    if( strnicmp( line, "LoadObject", 10 ) == 0 ) {
      strcpy( object_name, &(line[11]) );
      output_object_pos = out.tellp();
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
      strcpy( object_name, &(line[14]) );
      output_object_pos = out.tellp();
    }

    // Find an ObjectMotion and mark it.  Shift the motion of the item
    //  if any of it's ancestors are controlled by Particle Storm
    if( strncmp( line, "ObjectMotion", 12 ) == 0 ) {
      output_motion_pos = out.tellp();

      // Get a pointer to this item in the scene
      FreezerHierarchy *parent = (FreezerHierarchy *)(base.FindIndexInScene( ++object_index ));  // Also increments object_index!
      if( parent != NULL ) {
        // Here we find out if this item has an ancestor that is a particle.
        //  If it does, we store that item as the parent_particle
        parent_particle = NULL;
        while( (parent = (FreezerHierarchy *)(parent->GetParent())) != NULL ) {
          if( parent->HasPSM() ) {        // Match found; store it and shift the motion
            parent_particle = parent;

            out << line << endl;
            output_line = false;
            if( parent_particle->HasMovement() )
              ShiftMotion( in, out, parent_particle->GetMovementFrame() );
            else
              ShiftMotion( in, out, last_frame );

            break;
          }
        }
      }
    }

    // Find a BoneMotion and see if it should be shifted
    if( strncmp( line, "BoneMotion", 10 ) == 0 ) {
      if( parent_particle != NULL ) {
        out << line << endl;
        output_line = false;
        if( parent_particle->HasMovement() )
          ShiftMotion( in, out, parent_particle->GetMovementFrame() );
        else
          ShiftMotion( in, out, last_frame );
        }
    }

    // Find an EndBehavior and mark it
    if( strncmp( line, "EndBehavior", 11 ) == 0 )
      source_motion_end = in.tellg();

    // Check for Particle Storm Data
    if( !in_psm )
      streampos source_pstorm_start = in.tellg();

    if( strncmp( line, "Plugin", 6 ) == 0 ) {
      if( strstr( line, "ParticleStorm" ) != 0 )
        in_psm = true;
    }

    if( in_psm ) {
      // Don't output lines if we're in a PSM.
      output_line = false;

      // Check for end of PS data
      if( strncmp( line, "EndPlugin", 9 ) == 0 )
        in_psm = false;

      // Check for a new PSM, or increment from an old one.
      if( strncmp( line, "MotionDataFile", 14 ) == 0 ) {
        psm_match = -1;
        for( int i=0; i < (int)psm_names.NumElements(); i++ ) {
          if( stricmp( psm_names[i], &(line[15]) ) == 0 ) {
            psm_match = i;
            break;
          }
        }

        // Not loaded yet; add it as a new one.
        if( psm_match == -1 ) {
          psm_names.Add( strdup( &(line[15]) ) );
          psm_match = psm_names.NumElements()-1;
          psm_counts.Add( -1 );
        }

        // Store the filename for later, and tack on the Content Dir
        if( content == NULL )
          psm_file[0] = '\0';
        else
          strcpy( psm_file, content );

        DirStrings::AddPathPart( psm_file, &(line[16]) ); // Skip both
        psm_file[ strlen( psm_file ) - 1 ] = '\0';        //  quotes
      }

      // Check for MotionIndex and increment it.
      if( strncmp( line, "MotionIndex", 11 ) == 0 ) {
        psm_index = atoi( &(line[12]) );
        if( psm_counts[ psm_match ] == -1 )   // No index yet assigned; get it from the file
            psm_counts[ psm_match ] = psm_index;

        // Only scan a PSM once.  Needed so that when we look through
        //  a second time to find the data between the motion and the
        //  Particle Storm data, we don't process the PSM again.
        if( ++psm_scan_count > 1 )
          psm_scan_count = 0;
        else {

          // We now have enough data to freeze the PSM
          try {
            // Test for PSM existance
            if( !DirInfo::Exists( psm_file ) ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  The PSM file \"" << psm_file << "\" doesn't exist!" << endl;
              throw( false );
            }

            // Open the PSM file
            ifstream psm( psm_file, ios::binary );
            if( !psm ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  Unable to open file \"" << psm_file << "\"!" << endl;
              throw( false );
            }

            // Make sure it really is a PSM file
            psm.ignore( 8 );
            psm.read( line, 4 );
            if( strnicmp( line, "PSM2", 4 ) != 0 ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  \"" << psm_file << "\" isn't a Particle Storm 2 PSM file!" 
                   << "  (header:  " << line << ")" << endl;
              throw( false );
            }

            // Find out the size of the first chunk
            unsigned long chunk_size;
            int num_particles;

            // Header Data
//            psm.ignore( 14 );
            int temp;
            short eh;
            psm.get( line, 5);
            psm.read( (char *)(&temp), 4);
            psm.read( (char *)(&num_particles), 4);
            psm.read( (char *)(&eh),   2);
            MtoI(temp);                     // 6???
            MtoI(num_particles);            // Number of particles
            MtoI(eh);                       // 0???

            if( psm_index > num_particles-1 ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  index " << psm_index << " doesn't exist in \"" << psm_file << "\"!" << endl;
              throw( false );
            }

#ifdef debug_psm_loader
            cerr << "    Header:  " << line << " " << temp << " " << num_particles << " " << eh << endl;
#endif

            streampos first_frame_pos = psm.tellg();
            streampos chunk_start = -1;

            // Figure out how many frames are in the PSM
            int frames_per_particle = FindNumPSMFrames( psm );
            if( frames_per_particle < 0 ) {
              cerr << " ERROR:  File error reading from PSM \""
                   << psm_file << "\" while counting frames; skipping" << endl;
              throw( false );
            }

            if( frames_per_particle == 0 ) {
              cerr << " ERROR:  PSM \""
                   << psm_file << "\" contains no frames; skipping" << endl;
              throw( false );
            }

#ifdef debug_psm_loader
            cerr << endl << " ---------------------------------------------------------------------------------- " << endl << endl;
#endif
            cerr << " " << object_name << ", Index " << setw(3) << psm_index << ";  PSM " << DirStrings::HasFile( psm_file )
                 << ";  " << frames_per_particle << " frames total"
                 << endl;
#ifdef debug_psm_frames
            cerr << "    Index " << psm_index << ":" << endl;
#endif

            // Output initial motion data
            out.seekp( output_motion_pos, ios::beg );
            out << "ObjectMotion (unnamed)" << endl;
            out << "  9" << endl;                              // Num Channels
            if( (frames_per_particle <= last_frame) && do_drift ) {
              out << "  "  << frames_per_particle + 1 << endl; // Frame Count is equal to the last frame, if needed
            } else {
              out << "  "  << frames_per_particle << endl;     // Frame Count
            }

            float xyz[3];
            for( int frames = 0; /*NULL*/; frames++ ) {
              // Read in the chunk data
              chunk_start = psm.tellg();
              psm.get( line, 5 );
              psm.read( (char *)(&chunk_size), 4 );
              MtoI(chunk_size);

#ifdef debug_psm_loader
              cerr << "    Chunk Size (frame " << frames << "):  "
                   << line << " " << chunk_size <<  " Loc: " << psm.tellg() << endl;
#endif
              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from PSM \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;

              int   dummyi;
              float dummyf;
              psm.get( line, 5);
#ifdef debug_psm_loader
              cerr << "     " << line << endl;
#endif
              psm.get( line, 5);                  // FRFL?
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif
              psm.read( (char *)(&dummyi), 4 );
              MtoI( dummyi );
#ifdef debug_psm_loader
              cerr << " " << dummyi;
#endif
              psm.read( (char *)(&dummyi), 4 );
              MtoI( dummyi );
#ifdef debug_psm_loader
              cerr << " " << dummyi << endl;
#endif

              // Read in the POSN data
//              psm.ignore( 16 );
              psm.get( line, 5 );
              if( strncmp( line, "POSN", 4 ) != 0 ) {
                cerr << " ERROR:  Expected POSN; found \""
                     << line << "\" while scanning PSM; aborting" << endl;
                exit(20);
              }

              int posn_size;
              psm.read( (char *)(&posn_size), 4 );
              MtoI(posn_size);

#ifdef debug_psm_loader
              cerr << "      NumParticles/POSN size:  " << num_particles << "/" << posn_size << endl;
#endif
              psm.seekg( 12 * psm_index, ios::cur );        // Read in just the one particle's data
              psm.read( (char *)(&xyz), 12 );
              MtoI(xyz[0]);
              MtoI(xyz[1]);
              MtoI(xyz[2]);

              // Output the motion to the scene file
              //  drift[] is an optional drift amount, which is added
              //  to each frame of the motion so that Align To Path
              //  will point in the correct direction.
              if( (frames == 0) && copy1to0 ) {
#ifdef debug_psm_frames
                cerr << "        Frame   0:  (frame 0 copied from frame 1)" << endl;
#endif
              } else {
                // Copy frame 1 to 0, if needed
                int inc = 0;
                if( (frames == 1) && copy1to0 )
                  inc = -1;

                for( /*NULL*/; inc < 1; inc++ ) {
#ifdef debug_psm_frames
                  cerr << "        Frame " << setw(3) << frames << ":  X/Y/Z   "
                       << setw(12) << xyz[0] << "  "
                       << setw(12) << xyz[1] << "  " 
                       << setw(12) << xyz[2] << endl;
#endif
                  out << "  " << xyz[0] + (drift[0] * frames)
                      << " "  << xyz[1] + (drift[1] * frames)
                      << " "  << xyz[2] + (drift[2] * frames)
                      << " 0 0 0 1 1 1" << endl;
                  out << "  " << frames - inc << " 0 0 0 0" << endl;
                }
              }

              // Skip the rest of the POSN data
              psm.seekg( posn_size - (12 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Flags
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              SkipFLAG( psm );
#ifdef debug_psm_loader
              cerr << "  <skipped>" << endl;
#endif

              psm.get( line, 5);           // Color
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int colr_size;
              psm.read( (char *)(&colr_size), 4 );
              MtoI( colr_size );
#ifdef debug_psm_loader
              cerr << " " << colr_size;
#endif

              unsigned char rgba[4];
              psm.seekg( psm_index * 4, ios::cur );
              psm.read( rgba, 4 );

#ifdef debug_psm_loader
              cerr << "  RGBA: " << setw(4) << (int)rgba[0] << " "
                                 << setw(4) << (int)rgba[1] << " "
                                 << setw(4) << (int)rgba[2] << "  "
                                 << setw(4) << (int)rgba[3] << endl;
#endif

              // Skip the rest of the COLR data
              psm.seekg( colr_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Size
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif
              int size_size;
              psm.read( (char *)(&size_size), 4 );
              MtoI( size_size );
#ifdef debug_psm_loader
              cerr << " " << size_size;
#endif

              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyf), 4 );
#ifdef debug_psm_loader
              cerr << "  Size: " << setw(4) << dummyf << endl;
#endif

              // Skip the rest of the SIZE data
              psm.seekg( size_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // PLNK?
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int plnk_size;
              psm.read( (char *)(&plnk_size), 4 );
              MtoI( plnk_size );
#ifdef debug_psm_loader
              cerr << " " << plnk_size;
#endif
              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyi), 4 );
#ifdef debug_psm_loader
              cerr << "  PLNK: " << setw(4) << dummyi << endl;
#endif

              // Skip the rest of the PLNK data
              psm.seekg( plnk_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Particle Age
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int page_size;
              psm.read( (char *)(&page_size), 4 );
              MtoI( page_size );
#ifdef debug_psm_loader
              cerr << " " << page_size;
#endif

              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyi), 4 );
#ifdef debug_psm_loader
              cerr << "  PAge: " << setw(4) << dummyi << endl;
#endif

              // Skip the rest of the PAGE data
              psm.seekg( page_size - (4 * psm_index), ios::cur );


              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from psm \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;

              // Scan to the next chunk
              int dummy = psm.tellg();
              psm.seekg( abs( chunk_size - (psm.tellg() - chunk_start ) + 8 ), ios::cur );

              int test = psm.tellg();
#ifdef debug_psm_loader
              cerr << "   PSM Location:  " << test << endl;
#endif
            }
          
            // Test if the PSM is too short.  If it is, have it add extra keys until last frame.
            if( (frames <= last_frame) && (do_drift) ) {
              out << "  " << xyz[0] + (drift[0] * (last_frame - frames) )
                  << " "  << xyz[1] + (drift[1] * (last_frame - frames) )
                  << " "  << xyz[2] + (drift[2] * (last_frame - frames) )
                  << " 0 0 0 1 1 1" << endl;
              out << "  " << last_frame << " 0 0 0 0" << endl;
            }

            // Output the end of the motion data
            out << "EndBehavior 1" << endl;
            in_psm = false;                             // false because we won't be in a PSM when we jump back to the end of the motion
            in.seekg( source_motion_end, ios::beg );

            psm.close();

            // Add the Align To Path keyword.
            // NOTE:  This IS NOT in the correct place in the scene, but it should work
            //        (LW isn't too picky).
            if( align_to_path )
              out << "PathAlignment 1" << endl;

          } catch( bool a ) {
            ; // Catch errors here, if needed
          }
        }
      }
    }

    // See if we should output the line or not
    if( output_line == false )
      output_line = true;
    else
      out << line << endl;

  } while( !in.eof() );

  // Free the strdup()ed PSM Names
  if( psm_names.NumElements() > 0 )
    cout << " PSMs found:" << endl;
  else
    cout << " No PSMs found in load-from scene!" << endl;

  for( i=0; i < (int)psm_names.NumElements(); i++ ) {
    cout << "   Ending Enums: " << setw(10) << psm_counts[i] << "  Name: " << psm_names[i] << endl;
    free( psm_names[i] );
  }

  cout << " Enumeration Complete; successfully generated \""
       << output << "\"!" << endl;
}

// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Freezes all Particle Storm data referenced in a scene\n"
       << "   directly into the scene.  Can also offset the motion\n"
       << "   of items parented to the particle so they don't animate\n"
       << "   until the particle has emitted, and add drift to keep the\n"
       << "   particle aligned to path." << endl;
  cout << endl;
  cout << "  Usage:   PSMFreezer <source> [-o <output>] [-c <path> [-d <x> <y> <z>] [-1] [-align] [-s <dist> <offset> [-t]" << endl;
  cout << "    <source>:               source scene filename and path" << endl; 
  cout << "   [-o[utput] <output>]:    optional output name" << endl;
  cout << "   [-c[ontent] <path>]:     contect directory; needed to find relative-path PSMs" << endl;
  cout << "   [-d[rift] <x> <y> <z>]:  amount of drift per frame on X, Y and Z.  Useful for Align To Path" << endl;
  cout << "   [-1[to0]]:               copy frame 1 in the PSM (usually position 0,0,0) to frame 0" << endl;
  cout << "   [-a[lign]]:              turns on Align To Path on the particle object" << endl;
  cout << "   [-s[hift] <distance>     distance the particle has to move before the hierarchy should start animating" << endl;
  cout << "             <offset>]:     how many frames after the motion to start animating" << endl;
  cout << "   [-t[ree]]:               display hieracrchy tree.  Only works with -s" << endl;

  exit(0);
}

// FindNumPSMFrames()
//  Scans through the PSM provided and returns the number of frames it
//   contains.  The ifstream passed in should be at the start of the
//   first chunk.
int FindNumPSMFrames( ifstream &psm ) {
  char line[5];
  int chunk_size = 0;

  streampos orig_pos = psm.tellg();
  streampos chunk_start;
  
  for( int frames =0; /*NULL*/; frames++ ) {
    // Read in the chunk data
    chunk_start = psm.tellg();
    chunk_size  = 0;

    psm.get( line, 5 );
    psm.read( (char *)(&chunk_size), 4 );
    MtoI(chunk_size);

    // Scan to the next chunk
    int dummy = psm.tellg();
    psm.seekg( abs( chunk_size - (psm.tellg() - chunk_start ) + 8 ), ios::cur );

    // Error test
    if( (psm.bad() || psm.fail()) && !psm.eof() )
      return -1;

    // EOF test
    if( psm.eof() )
      break;
  }

  psm.seekg( orig_pos, ios::beg );
  psm.clear();

  return frames;
}

// SkipFLAG()
//  Skips the FLAG chunk, so I have no idea what it's for, and it
//   doesn't seem to have a sub-chunk length.
void SkipFLAG( ifstream &psm ) {
  char *skip = "COLR";               // Chunk to skip
  int match_count = 0;               // When 4, we found the chunk
  char c;

  do {
    psm.read( &c, 1 );               // Read in a char
    if( skip[ match_count ] == c )   // See if the char matches the one after the last matched char
      match_count++;
    else
      match_count = 0;

  } while( match_count < 4 );
  
  // Seek back to the start of the chunk
  psm.seekg( -4, ios::cur );
}


// ShiftMotion()
//  Shifts the motion pointed to at in by the amount specified, storing
//   the shifted motion into out.  The in streampos should be at the
//   line immediately after the one with the ObjectMotion/BoneMotion/
//   LightMotion keyword.
void ShiftMotion( pifstream &in, ofstream &out, int shift ) {
  // Make sure the shift is >0; otherwise, we dont' do anything.
  if( shift < 1 )
    return;

  // Read in and output the number of characters. 
  char line[8192];
  in.getline( line, 8192 );
  out << line << endl;

  int num_keys;
  in >> num_keys;

  // Check for >1 key.  If there is only one, there's nothing to shift,
  //  so we just output the original num_keys and let the main function
  //  deal with the rest.
  if( num_keys < 2 ) {
    in.getline( line, 8192 );           // Junk the EOLs
    out << "  " << num_keys << endl;
    return;
  }

  out << "  " << num_keys + 1 << endl;  // +1 is for the new shift key

  // Read in Frame 0
  double key[9];
  int    frame;
  int    linear;
  double tcb[3];

  in >> key[0] >> key[1] >> key[2]
     >> key[3] >> key[4] >> key[5]
     >> key[6] >> key[7] >> key[8];
  in >> frame >> linear >> tcb[0] >> tcb[1] >> tcb[2];

  // Output Frame 0 
  out << "  " << key[0] << " " << key[1] << " " << key[2] 
      << " "  << key[3] << " " << key[4] << " " << key[5] 
      << " "  << key[6] << " " << key[7] << " " << key[8] << endl;
  out << "  " << frame  << " 1 "
      << tcb[0] << " " << tcb[1] << " " << tcb[2] << endl;
  // Output Frame 0 + Shift
  out << "  " << key[0] << " " << key[1] << " " << key[2] 
      << " "  << key[3] << " " << key[4] << " " << key[5] 
      << " "  << key[6] << " " << key[7] << " " << key[8] << endl;
  out << "  " << frame + shift << " 1 "
      << tcb[0] << " " << tcb[1] << " " << tcb[2] << endl;

  // Itterate through the rest of the keys
  for( int i=1; i < num_keys; i++ ) {   // Start at 1, since we've already done frame 0
    in >> key[0] >> key[1] >> key[2]
       >> key[3] >> key[4] >> key[5]
       >> key[6] >> key[7] >> key[8];
    in >> frame >> linear >> tcb[0] >> tcb[1] >> tcb[2];

    out << "  " << key[0] << " " << key[1] << " " << key[2] 
        << " "  << key[3] << " " << key[4] << " " << key[5] 
        << " "  << key[6] << " " << key[7] << " " << key[8] << endl;
    out << "  " << frame + shift << " " << linear << " "
        << tcb[0] << " " << tcb[1] << " " << tcb[2] << endl;
  }

  // Junk the final EOL
  in.getline( line, 8192 );
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( float &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( double &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( long &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( int &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( short &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);

  char temp = s[0];
  s[0] = s[1];
  s[1] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( unsigned long &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( unsigned int &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);
  
  // Outer bytes
  char temp = s[0];
  s[0] = s[3];
  s[3] = temp;

  // Inner bytes
  temp = s[1];
  s[1] = s[2];
  s[2] = temp;

#endif
}

// MtoI()
//  Swaps endians from Motorola byte order to Intel byte order.
inline void MtoI( unsigned short &value ) {
#ifdef WIN32
  char *s   = (char *)(&value);

  char temp = s[0];
  s[0] = s[1];
  s[1] = temp;

#endif
}


