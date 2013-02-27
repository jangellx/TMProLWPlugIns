//
// StripPlugins.cpp
//
// StripPlugins strips all the plug-ins from a .LWS file.  It
// does this by copying the source .LWS into the optional
// output .LWS, skipping anything between the keywords Plugin
// and EndPlugin.  Note that this does NOT affect Lightwave
// objects in any way.
//

#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>

#include <portable/dirtools/dirtools.h>

// Prototypes
void Help(void);

const char *StripPlugins_Version  = "1";
const char *StripPlugins_Revision = "0";

void main( int argc, char **argv ) {
  cout << " StripPlugins V" << StripPlugins_Version << "." << StripPlugins_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few args
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * &source = argv[1];
  char   output[ MAX_PATH_LENGTH ] = "";

  for( int i=2; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
      case 'o':
        // Output Scene Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          cerr << " Type StripPlugins ? for help" << endl;
          exit(20);
        }
        strcpy( output, argv[i] );
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type StripPlugins ? for help" << endl;
        exit(20);
      }
    }
  }

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-Stripped" );                   // Append -MultiLFS
    ext = DirStrings::HasExtension( source );        // Find the original extension
    if( ext )                                        // Append the origianl extension to the output_name
      strcat( output, ext );
  } 

  // Test the paths
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type StripPlugins ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type StripPlugins ? for help" << endl;
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

  // Variable Setup
  char line[8192];
  bool in_plugin;
  int  plugin_count = 0;

  // Processing Loop
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }

    // Look for a plug-in
    if( strnicmp( line, "Plugin", 6 ) == 0 ) {
      in_plugin = true;
      plugin_count++;
    }

    // Output the line if we're not in a plug-in
    if( !in_plugin )
      out << line;

    // Look for the end of a plug-in
    if( strnicmp( line, "EndPlugin", 9 ) == 0 )
      in_plugin = false;

  } while( !in.eof() );

  cout << " " << plugin_count << " plug-ins successfully stripped; new scene \""
       << output << "\" created" << endl;

}

// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Creates a new scene that contains no plug-ins (ie: strips the plug-ins)" << endl;
  cout << endl;
  cout << "  Usage:   StripPlugins <source> [-o <output>]" << endl;
  cout << "    <source>:     source scene filename and path" << endl; 
  cout << "   [-o <output>]: optional output name" << endl;

  exit(0);
}
