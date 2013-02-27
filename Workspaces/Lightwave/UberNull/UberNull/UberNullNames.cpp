//
// UberNullNames.cpp
//

#include <stdlib.h>
#include <string.h>

#include <portable/stringtools/stringtools.h>

#include "UberNullNames.h"


// Constructor:
UberNullNames::UberNullNames()
  : match_mode(UNNMATCH_STARTING_WITH), match_string(NULL),
    enable(false), name(NULL), loc(UNNLOC_SCENE_FIRST),
    index_in_scene(-1), children_in_scene(0) {
  ;
}

// Destructor:
UberNullNames::~UberNullNames() {
  if( name != NULL )
    free( name );

  if( match_string != NULL )
    free( match_string );
}

// SetName():
void UberNullNames::SetName( const char *_name ) {
  if( name != NULL ) {
    free(name);
    name = NULL;
  }

  if( _name != NULL )
    name = strdup( _name );
}

// SetMatchString():
void UberNullNames::SetMatchString( const char *_match_string ) {
  if( match_string != NULL ) {
    free(match_string);
    match_string = NULL;
  }

  if( _match_string != NULL )
    match_string = strdup( _match_string );
}

// TestForMatch():
//  Returns true if the string passed in matches the match string
//   and match mode.
bool UberNullNames::TestForMatch( const char *string ) {
  if( match_mode == UNNMATCH_STARTING_WITH ) {
    if( strnicmp( string, GetMatchString(), strlen( GetMatchString() ) ) == 0 )
      return true;
  } else if( match_mode == UNNMATCH_ENDING_WITH ) {
    if( strnicmp( &(string[ strlen( GetMatchString() ) - strlen(string) ]), GetMatchString(), strlen( GetMatchString() ) ) == 0 )
      return true;
  } else if( match_mode == UNNMATCH_CONTAINING ) {
    if( StringTools::stristr( string, (char *)GetMatchString() ) != NULL )
      return true;
  } else if( match_mode == UNNMATCH_EXACT ) {
    if( stricmp( string, GetMatchString() ) == 0 )
      return true;
  }

  return false;
}

// SaveSettings():
//  Saves the settings for the named null to disk
bool UberNullNames::SaveSettings( ofstream &out ) {
  out << "NamedNull {" << endl;
  if( GetEnable() )
    out << "  Enabled" << endl;
  out << "  Name " << GetName() << endl;
  out << "  Loc " << GetLocation() << endl;
  out << "  MatchString " << GetMatchString() << endl;
  out << "  MatchMode " << GetMatchMode() << endl;
  out << "}" << endl;

  if( !out )
    return false;

  return true;
}

// LoadSettings():
//  Loads the settings for the named null from disk
bool UberNullNames::LoadSettings( ifstream &in ) {
  char line[ 8192 ];
  char *start;
  do {
    in.getline( line, 8192 );

    start = StringTools::SkipWhiteSpace( line );
    if( start != NULL ) {
      if( strnicmp( start, "Enabled", 7 ) == 0 )
        SetEnable( true );
      else if( strnicmp( start, "Name", 4 ) == 0 )
        SetName( &start[5] );
      else if( strnicmp( start, "Loc", 3 ) == 0 )
        SetLocation( (ubernullnames_location)atoi( &start[4] ) );
      else if( strnicmp( start, "MatchString", 11 ) == 0 )
        SetMatchString( &start[12] );
      else if( strnicmp( start, "MatchMode", 9 ) == 0 )
        SetMatchMode( (ubernullnames_match_modes)atoi( &start[10] ) );
      else if( start[0], "}" )
        break;
    }

    if( in.eof() )
      return false;

    if( in.bad() || in.fail() )
      return false;
  } while( true );

  return true;
}
