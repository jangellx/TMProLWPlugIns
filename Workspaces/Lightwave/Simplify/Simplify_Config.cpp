//
// Simplify_Config.cpp
//
// This contains the functions related to the Simplify Config File.
//  The file is a user-generated ASCII text file.  It is loaded
//  automatically on when Simplify is activated.
//

#include <portable/dirtools/dirtools.h>
#include <portable/configfstream/configfstream.h>

#include "Simplify_Config.h"

// Load():
//  Loads the config file provided, returning one of the
//   SIMPERROR_ enums if something goes wrong.
int SimplifyConfig::Load( const char * cfg_path ) {
  was_loaded    = true;
  was_loaded_ok = true;

  // See if the config exists
  if( !DirInfo::Exists( cfg_path ) )
    return SIMPCONFIG_NOT_FOUND;

  // Open the config
  Configifstream cfg( cfg_path );
  if( !cfg )
    return SIMPCONFIG_ERROR_OPENING_FILE;

  char buffer[ MAX_PATH_LENGTH ];

  // Read in the keywords
  do {
    if( cfg.ReadNextLine() > -1 ) {                         // Read the next line into the stream's buffer
      cfg.SkipWhiteSpace();
      cfg >> buffer;
      cfg.SkipWhiteSpace();

      if( strnicmp( buffer, "DefaultSDF", 10 ) == 0 ) {                // DefaultSDF
        cfg.getline( buffer, MAX_PATH_LENGTH );
        default_sdf = strdup( StringTools::RemoveTailWhiteSpace( buffer ) );
      } else if( strnicmp( buffer, "SaveInScene", 11 ) == 0 ) {         // SaveInScene
        save_in_scene = true;
      }
    } else
      break;
  } while( !cfg.eof() );

  was_loaded_ok = true;
  return SIMPCONFIG_OK;
}
