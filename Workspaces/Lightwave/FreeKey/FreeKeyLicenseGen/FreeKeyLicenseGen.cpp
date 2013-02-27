//
// FreeKeyLicenseGen
//

#include <stdio.h>
#include <stdlib.h>

#include <fstream.h>
#include <iomanip.h>

#include <Portable/DirTools/DirTools.h>

#include "FreeKeyLicense.h"

void main( int argc, char **argv ) {
  cout << "FreeKey License Key Generator" << endl;
  cout << " --- FOR INTERNAL USE BY TM PRODUCTONS ONLY ---" << endl;
  cout << "        Copyright 2000 TM Productions" << endl;

  if( strcmp( argv[1], "-f" ) == 0 ) {
    // Read IDs from a file
    if( !DirInfo::Exists( argv[2] ) ) {
      cout << " ERROR:  Dongle ID file \"" << argv[2] << "\" doesn't exist; aborting" << endl;
      return;
    }

    char buffer[ MAX_PATH_LENGTH ];
    strcpy( buffer, argv[2] );
    (*DirStrings::HasExtension( buffer )) = '\0';
    strcat( buffer, "-Keys" );
    strcat( buffer, DirStrings::HasExtension( argv[2] ) );

    ifstream in( argv[2] );
    if( !in ) {
      cout << " ERROR:  Couldn't open Dongle ID file \"" << argv[2] << "\"; aborting" << endl;
      return;
    }

    ofstream out( buffer );
    if( !out ) {
      cout << " ERROR:  Couldn't open output key file \"" << buffer << "\"; aborting" << endl;
      return;
    }

    char line[1024];
    do {
      in.getline( line, 1024 );
      if( (in.fail() || in.bad()) && !in.eof()) {
        cout << " ERROR:  something bad happened reading from the dongle id file; aborting" << endl;
        return;
      }

      if( atoi( line ) != 0 ) {
        out << GenerateFreeKeyLicense( atoi( line ) ) << endl;
        cout << setiosflags(ios::dec) << setw( 7 ) << atoi( line )
             << "  ==  "  << GenerateFreeKeyLicense( atoi( line ) ) << endl;
      }
    } while( !in.eof() );

    in.close();
    out.close();
  } else {
    // Read IDs from the command line
    for( int i=1; i < argc; i++ ) {
      cout << setiosflags(ios::dec) << setw( 7 ) << atoi( argv[i] )
           << "  ==  "  << GenerateFreeKeyLicense( atoi(argv[i]) ) << endl;
    }
  }
}
