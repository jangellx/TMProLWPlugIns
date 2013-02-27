//
// UberNull_CLI.cpp
//
// This file contains the command line implementation of UberNull.
//

#include "../UberNull/UberNull.h"

const char *source_name   = NULL;
const char *output_name   = NULL;
const char *ubernull_name = NULL;
int         location      = UBERNULL_FIRST;

// Function Prototypes
bool ParseCommands( int argc, char **argv );
void Help(void);

void main( int argc, char **argv ) {
	cout << " -- UberNull V" << UberNull_Version << " -- Copyright 1999 TM Productions" << endl;
	if( argc < 2 )          // Not Enough Arguments
		Help();

  if( argv[1][0] == '?' ) // 1st argument is a ?; display help
    Help();

  // Parse the command line arguments
  if( !ParseCommands( argc, argv ) )
    exit(20);

  // Create the UberNull instance
  UberNull ubernull( source_name, output_name, ubernull_name, location );

  // Open the Source and Output files and make sure the null name is OK
  int error = ubernull.OpenFiles();
  if( error != UBERNULL_OK ) {
    cerr << " Error:  " << ubernull_error_strings[ error ] << endl;
    exit(20);
  }

  // Process the source scene into the UberNull'ed output scene;
  error = ubernull.Process();
  if( error != UBERNULL_OK ) {
    cerr << " Error:  " << ubernull_error_strings[ error ] << endl;
    exit(20);
  }

  cerr << " Processing complete!  UberNull'ed scene created as "
       << ubernull.GetOutput() << endl;
}

// ParseCommands():
//  Parse the command line arguments for commands
bool ParseCommands( int argc, char **argv ) {
	if( argc < 2 ) {          // Not Enough Arguments
		Help();
    return false;
  }

  if( argv[1][0] == '?' ) { // 1st argument is a ?; display help
    Help();
    return false;
  }

  source_name = argv[1];
  
  for( int i=2; i < argc; i++ ) {
    // Test for unknown arguments
    if( argv[i][0] != '-' ) {
      cerr << " ERROR:  Unknown argument \"" << argv[i] << "\"!";
      return false;
    }

    // Make sure there are enough arguments left
    if( i+1 == argc ) {
      cerr << " ERROR:  Not enough arguments for arg " << i << "!" << endl;
      return false;
    }

    switch( argv[i][1] ) {
      case 'o':     // Output Name
        output_name = argv[++i];
        break;
      case 'n':     // Null Object Name
        ubernull_name = argv[++i];
        break;
      case 'l':     // Location Index (FIRST or LAST)
        if( stricmp( argv[++i], "first" ) == 0 ) {
          location = UBERNULL_FIRST;
        } else if( stricmp( argv[i], "last" ) == 0 ) {
          location = UBERNULL_LAST;
        } else {
          cerr << " ERROR: unknown location \"" << argv[i]
               << "\"; use FIRST or LAST" << endl;
          return false;
        }
        break;
      default:
        cerr << " ERROR: unknown argument \"" << argv[i] << "\"!" << endl;
        return false;
        break;
    }
  }

  return true;
}

// Help():
//  Provides a command template and instructions.
//  The program exits at the end of the function.
void Help(void) {
  cout << "                 http://www.tmproductions.com" << endl;
  cout << endl;
  cout << " UberNull's simple purpose is to parent all unparented objects," << endl;
  cout << "  lights, bones and cameras to a newly created UberNull.  A new" << endl;
  cout << "  scene file will be created, named <scene>-UberNull.lws by default." << endl;
  cout << endl;
	cout << " Usage:	UberNull <source.lws> <output.lws> -n(ull) <null name> -l(oc) <FIRST or LAST)" << endl;
	cout << "   <source.lws>		        the Lightwave scene file to parent to the UberNull" << endl;
	cout << "   -o(utput) <output.lws>	optional name of the Lightwave scene file created\n"
       << "                             with everything parented to the UberNull" << endl;
  cout << "   -n(ull) <null name>     optional name of the UberNull object " << endl;
  cout << "   -l(oc)  <location>      optional location of the object (FIRST or LAST) " << endl;

  exit(20);
}

