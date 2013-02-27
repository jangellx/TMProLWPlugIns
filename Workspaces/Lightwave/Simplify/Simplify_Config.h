//
// Simplify_Config.h
//
// This contains the functions related to the Simplify Config Class.
//  The file is a user-generated ASCII text file.  It is loaded
//  automatically on when Simplify is activated.  It is based on
//  Configfstream, and as such supports comments.
//
// The config file currently supports the following data:
//  
//  DefaultSDF <path.sdf>
//   This is the SDF file that will be loaded if a default
//   SDF matching the object name can't be found.
//
//  SaveInScene
//   If present, this switch causes the Save Data As pop-up
//   to default to Save In Scene instead of Save As SDF.
//

#include "Simplify_ObjRep_Interface.h"

enum simplify_config_errors {
  SIMPCONFIG_OK                       = 0,
  SIMPCONFIG_NOT_FOUND,
  SIMPCONFIG_ERROR_OPENING_FILE,
  SIMPCONFIG_ERROR_READING_FROM_FILE,
  SIMPCONFIG_ERROR_NOT_SIMP_CONFIG,
  NUM_SIMP_CONFIG_ERRORS };


class SimplifyConfig;

class SimplifyConfig {
public:
  SimplifyConfig() : save_in_scene(false), default_sdf(NULL), was_loaded(false), was_loaded_ok(false) { ; }
  ~SimplifyConfig() { if( default_sdf != NULL ) free( default_sdf ); }

  int    Load( const char * cfg_path );

  bool   GetSaveInScene() { return save_in_scene; }
  char * GetDefaultSDF()  { return default_sdf;   }

  bool   WasLoaded() { return was_loaded; }
  bool   WasLoadedOK() { return was_loaded_ok; }

protected:
  bool   save_in_scene;
  char * default_sdf;

  bool   was_loaded;            // True if any attempt to load the config file has been made
  bool   was_loaded_ok;         // True if the config was found and loaded without error
};

