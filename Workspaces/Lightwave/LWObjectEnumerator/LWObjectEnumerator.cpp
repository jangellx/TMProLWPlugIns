//
// LWObjectEnumerator.cpp
//

#include <string.h>
#include <stdio.h>

#include <portable/pfstream/pfstream.h>

void main( int argc, char *argv[] ) {
  if( argc < 2 ) {
    cerr << " You must specifiy a scene file to enumerate; aborting" << endl;
    return;
  }

  bool split = false;
  if( argc > 2 ) {
    if( strnicmp( argv[2], "-s", 2 ) == 0 )
      split = true;
  }

  // Open the input file
  pifstream in( argv[1], ios::nocreate );
  if( !in ) {
    cerr << " Error opening source scene \"" << argv[1] << "\"; aborting" << endl;
    return;
  }
  in.SetAnyEOL( true );

  char ext_buffer[ 100 ];
  char output[ 1024 ];

  strcpy( output, argv[1] );
  char * dot = strrchr( output, '.' );
  if( dot != NULL )
    strcpy( ext_buffer, dot );

  strcat( output, "-Enumed" );
  strcat( output, ext_buffer );

  // Open the output file
  pofstream out( output );
  if( !out ) {
    cerr << " Error opening output scene \"" << output << "\"; aborting" << endl;
    return;
  }

  cerr << " Enumerating \"" << argv[1] << "\" into \"" << output << "\"..." << endl;

  // Enumerate
  char buffer[8192];
  int obj_index = 0, null_index = 0;
  int split_index = 0;
  long prev_split = -1;
  long split_size = 1024 * 1024;

  char split_buffer[ 1024 ];
  pofstream *split_out = NULL;

  do {
    if( split ) {
      // Create a new Split File, if needed
      if( (prev_split == -1) || ((in.tellg() - prev_split) > split_size ) ) {
        if( split_out != NULL ) {
          split_out->close();
          delete split_out;
        }

        strcpy( split_buffer, argv[1] );
        sprintf( split_buffer, "%s-Split_%d.txt", argv[1], split_index++ );
        cerr << " - New Split File: \"" << split_buffer << "\"" << endl;
        split_out = new pofstream( split_buffer );

        prev_split = in.tellg();
      }
    }

    // Read new data into the buffer
    in.getline( buffer, 8192 );

    // Input Error Test
    if( (in.fail() || in.bad()) && !in.eof() ) {
      cerr << " Error reading source scene \"" << argv[1] << "\"; aborting" << endl;
      return;
    }

    if( strncmp( buffer, "AddNullObject", 13 ) == 0 ) {
      // AddNullObject
      cerr << "  N" << null_index << "..." << endl;
      out << "AddNullObject N" << null_index++ << " - " << &(buffer[14]) << out.endl();

      if( split_out != NULL )
        (*split_out) << "AddNullObject N" << null_index << " - " << &(buffer[14]) << split_out->endl();
    } else if( strncmp( buffer, "LoadObject", 10 ) == 0 ) {
      // LoadObject
      cerr << "  O" << obj_index << "..." << endl;

      char * slash = strrchr( buffer, '\\' );
      if( slash != NULL ) {
        slash[0] = '\0';
        out << "LoadObject " << &(buffer[11]) << "\\O" << obj_index++ << " - " << &(slash[1]) << out.endl();

        if( split_out != NULL )
          (*split_out) << "LoadObject " << &(buffer[11]) << "\\O" << obj_index << " - " << &(slash[1]) << split_out->endl();
      } else {
        out << "LoadObject 0" << obj_index++ << " - " << &(buffer[11]) << out.endl();

        if( split_out != NULL )
          (*split_out) << "LoadObject 0" << obj_index << " - " << &(buffer[11]) << split_out->endl();
      }
    } else {
      // All Other Data
      out << buffer << out.endl();

      if( split_out != NULL )
        (*split_out) << buffer << split_out->endl();
    }

   // Output Error Test
    if( out.fail() || out.bad() ) {
      cerr << " Error writing output scene \"" << output << "\"; aborting" << endl;
      return;
    }
  } while( !in.eof() );

  if( split_out != NULL ) {
    split_out->close();
    delete split_out;
  }

  cerr << " Enumeratation complete!" << endl;
}
