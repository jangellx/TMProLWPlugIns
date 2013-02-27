//
// IMP-FromStringTools.cpp
//

#include "Motify-FromStringTools.h"

// stristr():
//  Searches for a string in another string, case insensitive.  This should be
//   a standard function, but for some reason isn't.  <sigh>  See strstr() for
//   more information.
char * stristr( const char *searched, const char *key ) {
  int s_len   = strlen( searched );
  int k_len   = strlen( key );
  int stop_at = s_len - k_len + 1;

  // The key must be smaller than the searched
  if( s_len < k_len )
    return NULL;

  // Loop through, first finding a match for the first char, then
  //  for the entire string if the char matches.  If the char match
  //  worked but the sting match didn't try again until the char
  //  match fails or the string match succeeds, or stop_at is reached.
  char        k_test = key[0];
  char        s_test;
  const char *char_match;

  if( (k_test >= 'A') && (k_test <= 'Z') )
    k_test += 'a' - 'A';

  for( int i=0; i < stop_at; i++ ) {
    s_test = searched[i];
    if( (s_test >= 'A') && (s_test <= 'Z') )
      s_test += 'a' - 'A';

    // First find a character
    if( s_test == k_test ) {
      char_match = &(searched[i]);

      // The first char matches; see if the rest of the string does
      if( strnicmp1( char_match, key, k_len ) == 0 )
        return (char *)char_match;
    }
  }

  return NULL;
}

// SkipWhiteSpace():
// Returns a pointer to the first character in the string
//  that isn't a white space.  This can be an empty string
//  if the original string contains only white space.
char * SkipWhiteSpace( const char * string ) {
  int length = strlen( string );
  char *start = (char *)string;

  for( int i=0; i < length; i++ ) {
    if( (string[i] == ' '  ) ||
        (string[i] ==  9   ) ||
        (string[i] == '\n' ) ||
        (string[i] == '\r' ) ) {
    } else {
      start = (char *)&(string[i]);
      break;
    }
  }

  return start;
}
