//
// ContentDirector-Entry.cpp
//

#include "ContentDirector-Entry.h"

// OutputToStream():
bool ContentDirector_Entry::OutputToStream( ostream &out ) {
  out << "Entry {" << endl;
  out << "  Name " << ((GetName() == NULL) ? "" : GetName())<< endl;
  out << "  Mode " << GetMode() << endl;

  if( GetPath() != NULL )
    out << "  Path " << GetPath() << endl;

  if( GetEnvVar() != NULL )
    out << "  EnvVar " << GetEnvVar() << endl;

  out << "}" << endl;

  return true;
}

// InputFromStream():
//  Note this does NOT read the Entry keyword; it assumes that
//   has been read, but that nothing after it has been.  Returns
//   false if there was an error reading the entry.
bool ContentDirector_Entry::InputFromStream( istream &in ) {
  char buffer[ MAX_PATH_LENGTH ];

  in.getline( buffer, MAX_PATH_LENGTH );    // Junk the rest of the line

  while( !in.eof() ) {
    in >> buffer;

    if( (in.fail() || in.bad()) && !(in.eof()) )
      return false;

    if( stricmp( buffer, "Name" ) == 0 ) {
      in.getline( buffer, MAX_PATH_LENGTH );
      SetName( &(buffer[1]) );
    } else if( stricmp( buffer, "Mode" ) == 0 ) {
      int temp_mode;
      in >> temp_mode;
      mode = (CDEntryFromModes) temp_mode;
    } else if( stricmp( buffer, "Path" ) == 0 ) {
      in.getline( buffer, MAX_PATH_LENGTH );
      SetPath( &(buffer[1]) );
    } else if( stricmp( buffer, "EnvVar" ) == 0 ) {
      in.getline( buffer, MAX_PATH_LENGTH );
      SetEnvVar( &(buffer[1]) );
    } else if( stricmp( buffer, "}" ) == 0 ) {
      break;
    }
  }

  CheckExists();

  return true;
}

// CheckExists():
bool ContentDirector_Entry::CheckExists() {
  if( mode == CDFM_SPECIFIC_DIR )
    return (exists = DirInfo::Exists( path ));

  if( env == NULL )
    return (exists = false);

  const char * env_path = getenv( env );
  return (exists = DirInfo::Exists( env_path ));
}

