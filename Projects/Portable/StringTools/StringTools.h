//
// StringTools.h
//
// The StringTools class provides a number of useful string functions
// without requiring the string to be contained within the class.
// Because of this, all the member functions are static.
//
// In this class, white space refers to any spaces, tabs, line feeds
// or carriage returns, except in the case of functions that test for
// EOL characters.
//

#ifndef INCLUDE_STRING_TOOLS_HEADER
#define INCLUDE_STRING_TOOLS_HEADER

#include <string.h>

class StringTools;

class StringTools {
public:
  static int    CountWords(        const char *string, bool quoted_word = false );  // Returns the number of space-delimited words in the string

  static char * RemoveAllWhiteSpace(     char *string, bool quoted_word = false );  // Removes all white space found anywhere within a string
  static char * RemoveHeadWhiteSpace(    char *string );      // Removes the white space at the begining of the string
  static char * RemoveTailWhiteSpace(    char *string );      // Removes the white space from the end of the string
  static char * RemoveCappingWhiteSpace( char *string );      // Removes the white space at the begining and end of the string

  static char * RemoveWord( char *string, int index, bool quoted_word = false ); // Removes the count'th word from the string, as though it had never been there

  static char * FindWord(       const char *string, int index, bool quoted_word = false );    // Returns a pointer to the index'th word in the string.  Returns NULL if there are no more words
  static int    FindWordLength( const char *string, bool quoted_word = false );               // Returns the length of the word pointed to in the string (ie: number of chars from current location to first white space)
  static char * FindWhiteSpace( const char *string, bool quoted_word = false );               // Returns a pointer to the first occurance of white space in the string.
  static char * SkipWhiteSpace( const char *string );                                         // Returns a pointer to the first character in the string that isn't a white space

  static char * IsolateWord(    const char *string, char *buffer, int index = 0, bool quoted_word = false, bool keep_quotes = true );  // Isolates the index'th word in the string and copies it into the buffer

  static char * stristr( const char *searched, const char *key );                   // Performs a case-insensitve sub-string search.  See strstr for more information.
  static char * strdup( const char *source );                                       // Performs a standard strdup(), but uses new instead of malloc() to allocate the string.
  static char *&strdup2( char * &output, const char *source );                      // "Smart" strdup that handles NULL pointers and frees the string's memory

  static char * Reverse( char *string, int len = -1 );                              // Reverses the string (in place)
  static char * Reverse( char *output, const char *string, int len = -1 );          // Reverses the string into the specified output buffer
  static char * Reverse( unsigned char *output, const unsigned char *string, int len = -1 ) {
    return Reverse( (char *)output, (const char *) string, len ); }                 // Reverses the string into the specified output buffer
};

#endif