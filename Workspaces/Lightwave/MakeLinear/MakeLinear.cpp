//
// MakeLinear.cpp
//
// Makes all keyframes in the provided motion/envelope linear.
//  By default this is all frames, but it can optionally be just two
//  adjoining frames in order to eliminate motion blur.
//

#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>

#include <portable/dirtools/dirtools.h>

// Prototypes
void Help(void);

// Argument Template:
//  MakeLinear <source> <-a[djacentonly]>
//   <source>:          source scene filename and path
//   [-o <output>]:     optional output name
//   [-a[djacentonly]]: only makes adjacent keys linear.  Use to eliminate motion blur between two keys

const char *MakeLinear_Version  = "1";
const char *MakeLinear_Revision = "0";

void main( int argc, char **argv ) {
  cout << " MakeLinear V" << MakeLinear_Version << "." << MakeLinear_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few args
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * &source        = argv[1];
  int    adjacent_only  = false;
  char   output[ MAX_PATH_LENGTH ] = "";

  for( int i=2; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
      case 'o':
        // Output Scene Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          cerr << " Type MakeLinear ? for help" << endl;
          exit(20);
        }
        strcpy( output, argv[i] );
        break;

      case 'a':
        // Load Lights Test
        cout << "  Adjacent Mode Active" << endl;
        adjacent_only = true;
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type MakeLinear ? for help" << endl;
        exit(20);
      }
    } else {
      cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
      cerr << " Type MakeLinear ? for help" << endl;
      exit(20);
    }
  }

  cout << " Processing Scene \"" << source << "\" into \"" << output << "\"..." << endl;

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-MadeLinear" );                 // Append -MadeLinear
    ext = DirStrings::HasExtension( source );        // Find the original extension
    if( ext )                                        // Append the origianl extension to the output_name
      strcat( output, ext );
  } 

  // Test the paths
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type MakeLinear ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type MakeLinear ? for help" << endl;
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

  // Make sure it's a motion or envelope file
  in.getline( line, 8192 );
  if( ( strncmp( line, "LWEN", 4) != 0 ) && 
      ( strncmp( line, "LWMO", 4) != 0 ) ) {
    cerr << " ERROR: Input scene \"" << source << "\" isn't a Lightwave motion or envelope file!" << endl;
    exit(20);
  }
  out << line << endl;

  // Test the version number
  int version;
  in >> version;
  out << version << endl;
  
  if( version != 1 ) {
    cerr << " ERROR: Unsupported file version \"" << version << "\"!" << endl;
    exit(20);
  }

  // Read in the number of keys
  int num_channels;
  in >> num_channels;
  out << num_channels << endl;

  // Read in the number of keys
  int num_keys;
  in >> num_keys;
  out << num_keys << endl;

  if( num_keys < 1 ) {
    cerr << " ERROR: No keyframes found in \"" << version << "\"!" << endl;
    exit(20);
  }

  // Loop through the keys and make them linear
  int prev_keyframe = 0;
  int key, linear;
  float t, c, b;
  for( i=0; i < num_keys; i++ ) {
    // Read the motion channels
    in.getline( line, 8192 );  // Junk the EOLs
    in.getline( line, 8192 );
    out << line << endl;

    // Read in the spline data
    in >> key >> linear >> t >> c >> b;
    if( adjacent_only ) {                // Only create linear keys for adjacent frames
      if( prev_keyframe + 1 == key )
        out << key << " 1 " << t << " " << c << " " << b << endl;
      else 
        out << key << " " << linear << " " << t << " " << c << " " << b << endl;
    } else
      out << key << " 1 " << t << " " << c << " " << b << endl;

    prev_keyframe = key;

    // Check for an EOF
    if( in.eof() )
      break;

    // Error test
    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }
  }

  cerr << "- Operation Sucessful!" << endl;
  cerr << "  " << num_keys << " keyframes in a the file " << source << " have been made linear" << endl;
}

// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Makes the keys of a motion/envelope linear." << endl;
  cout << endl;
  cout << "  Usage:   MakeLinear <source> [-o <output>] [-a]" << endl;
  cout << "    <source>:         source scene filename and path" << endl; 
  cout << "   [-o <output>]:     optional output name" << endl;
  cout << "   [-a[djacentonly]]: only makes adjacent keys linear" << endl;

  exit(0);
}
