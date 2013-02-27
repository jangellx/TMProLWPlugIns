//
// UberNullnames.h
//

#include <fstream.h>

enum ubernullnames_location {
  UNNLOC_SCENE_FIRST,
  UNNLOC_SCENE_LAST,
  UNNLOC_HIERARCHY_FIRST };

enum ubernullnames_match_modes {
  UNNMATCH_STARTING_WITH,
  UNNMATCH_ENDING_WITH,
  UNNMATCH_CONTAINING,
  UNNMATCH_EXACT };

class UberNullNames;

// Class UberNullNames
class UberNullNames {
public:
  UberNullNames();
  ~UberNullNames();

  // Interface Accessors
  inline const char * GetName() { return name; }
               void   SetName( const char *_name );

  inline ubernullnames_location GetLocation() { return loc; }
  inline void                   SetLocation( ubernullnames_location _loc ) { loc = _loc; }

  inline       bool   GetEnable() { return enable; }
  inline       void   SetEnable( bool state ) { enable = state; }

  inline const char * GetMatchString() { return match_string; }
               void   SetMatchString( const char *_match_string );

               bool   TestForMatch( const char *string );

  inline ubernullnames_match_modes GetMatchMode() { return match_mode; }
  inline void                      SetMatchMode( ubernullnames_match_modes _match_mode ) { match_mode = _match_mode; }

  bool                SaveSettings( ofstream &out );
  bool                LoadSettings( ifstream &in );

public:
  // Processing Variables
  int                         index_in_scene;
  int                         children_in_scene;

protected:
  char                      * name;

  ubernullnames_location      loc;

  char                      * match_string;
  ubernullnames_match_modes   match_mode;

  bool                        enable;
};

