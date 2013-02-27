//
// IMP-FromStringTools.h
//
// Source borrowed from portable\stringtools\stringtools[.h|.cpp]
//

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

inline char * strdup1( const char * source );
inline char * &strdup2( char * &output, const char *source );
       char * stristr( const char *searched, const char *key );
       char * SkipWhiteSpace( const char * string );

// strdup1():
//  Works just like the standard strdup(), but uses new instead of
//   malloc() to allocate memeory for the new string.  This is just
//   so you can delete instead of free()'ing your strings.
inline char * strdup1( const char * source ) {
  int len = strlen( source );
  char * buffer = new char[len+1];
  memcpy( buffer, source, len+1 );
  return buffer;
}

// strdup2():
//  A "smart" version of strdup.  This takes 2 paramaters:  the output
//   pointer as a reference and the source string.  If the source is NULL
//   or is an empty string (first char is '\0') and the output is non-NULL,
//   the output's buffer will be freed and set to NULL.
inline char * &strdup2( char * &output, const char *source ) {
  if( output != NULL ) {
    delete output;
    output = NULL;
  }

  if( source != NULL ) {
    if( source[0] != '\0' )
      output = strdup1( source );
  }

  return output;
}

// stricmp1():
//  Exactly the same as stricmp(), but works on things besides Windows.
inline int stricmp1( const char *string1, const char *string2 ) {
  char c1, c2;
  for( int i=0; /*NULL*/; i++ ) {
    if( (string1[i] == '\0') && (string2[i] == '\0') )
      return 0;

    if( string1[i] == '\0' )
      return -1;

    if( string2[i] == '\0' )
      return 1;

    c1 = tolower( string1[i] );
    c2 = tolower( string2[i] );

    if( c1 == c2 )
      continue;

    if( c1 < c2 )
      return -1;

    return 1;
  }

  // We should never get here.
  return 0;
}

// strnicmp1():
//  Exactly the same as strnicmp(), but works on things besides Windows.
inline int strnicmp1( const char *string1, const char *string2, size_t count ) {
  char c1, c2;
  for( size_t i=0; i < count; i++ ) {
    if( (string1[i] == '\0') && (string2[i] == '\0') )
      return 0;

    if( string1[i] == '\0' )
      return -1;

    if( string2[i] == '\0' )
      return 1;

    c1 = tolower( string1[i] );
    c2 = tolower( string2[i] );

    if( c1 == c2 )
      continue;

    if( c1 < c2 )
      return -1;

    return 1;
  }

  // We'll get here if i == count.
  return 0;
}
