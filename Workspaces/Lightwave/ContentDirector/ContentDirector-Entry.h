//
// ContentDirector_Entry.h
//

#include <portable/StringTools/StringTools.h>
#include <portable/DirTools/DirTools.h>
#include <fstream.h>

enum CDEntryFromModes {
  CDFM_SPECIFIC_DIR, CDFM_ENV_VARIABLE };

class ContentDirector_Entry {
public:
  ContentDirector_Entry() : name(NULL), path(NULL), env(NULL), exists(false), mode(CDFM_SPECIFIC_DIR) { ; }
  ~ContentDirector_Entry() { StringTools::strdup2( name, NULL );
                             StringTools::strdup2( path, NULL ); }

  // File I/O
  bool OutputToStream( ostream &out );
  bool InputFromStream( istream &in );

  // Accessors
  inline const char * GetName() { return name; }
  inline void         SetName( const char *_name ) { StringTools::strdup2( name, _name ); }

  inline CDEntryFromModes GetMode() { return mode; }
  inline void             SetMode( CDEntryFromModes _mode ) { mode = _mode; }

  inline const char * GetPath() { return path; }
  inline void         SetPath( const char *_path ) { StringTools::strdup2( path, _path ); }

  inline const char * GetEnvVar() { return env; }
  inline void         SetEnvVar( const char *_env ) { StringTools::strdup2( env, _env ); }

  inline bool         GetExists() { return exists; }
         bool         CheckExists();

protected:
  char             * name;     // User Name/Comment
  CDEntryFromModes   mode;     // Mode
  char             * path;     // Content Dir Path
  char             * env;      // Environment Variable Name
  bool               exists;   // true if path exists
};


