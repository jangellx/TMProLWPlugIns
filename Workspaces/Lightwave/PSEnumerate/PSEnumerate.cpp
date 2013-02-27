//
// PSEnumerate
//
// A simply hack of a program that enumerates the Particle Storm index within
//  a scene file.
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
//  PSEnumerate <source> [-start index] [-o <output>]
//   <source>:      source scene filename and path
//  [-start index]: enumerate starting from this index
//  [-o <output>]:  optional output name

const char *PSEnumerate_Version  = "1";
const char *PSEnumerate_Revision = "0";

void main( int argc, char **argv ) {
  cout << " PSEnumerate V" << PSEnumerate_Version << "." << PSEnumerate_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few args
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * &source          = argv[1];
  int    start_index      = 0;
  bool   use_start_index  = false;
  char   output[ MAX_PATH_LENGTH ] = "";

  for( int i=2; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
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
        // Start Index
        if( ++i >= argc ) {
          cerr << " ERROR: No start index following argument -s!" << endl;
          cerr << " Type PSEnumerate ? for help" << endl;
          exit(20);
        }
        start_index = atoi( argv[i] );
        use_start_index = true;
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

  // Test the args
  if( start_index < 0 ) {
    cerr << " ERROR: The start index must be greater than 0!" << endl;
    cerr << " Type PSEnumerate ? for help" << endl;
    exit(20);
  }

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-PSEnumed" );                   // Append -PSEnumed
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

  // Set up some variables
  char line[8192];
  int  psm_match   = -1;
  bool in_psm      = false;
  bool output_line = true;

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

    // Check for Particle Storm Data
    if( strncmp( line, "Plugin", 6 ) == 0 ) {
      if( strstr( line, "ParticleStorm" ) != 0 ){
        in_psm = true;
      }
    }

    if( in_psm ) {
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
      }

      // Check for MotionIndex and increment it.
      if( strncmp( line, "MotionIndex", 11 ) == 0 ) {
        if( psm_counts[ psm_match ] == -1 ) { // No index yet assigned; get it from the file
          if( use_start_index == false )      // User didn't supply a start count; use first in scene
            psm_counts[ psm_match ] = atoi( &(line[12]) );
          else
            psm_counts[ psm_match ] = start_index;
        }

        out << "MotionIndex " << psm_counts[ psm_match ]++ << endl;
        output_line = false;
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
  cout << "  Loads a scene into another scene the number of times specified." << endl;
  cout << endl;
  cout << "  Usage:   PSEnumerate <source> [-start index] [-o <output>]" << endl;
  cout << "    <source>:      source scene filename and path" << endl; 
  cout << "   [-start index]: enumerate starting from this index" << endl;
  cout << "   [-o <output>]:  optional output name" << endl;

  exit(0);
}
