//
// StringTools.cpp
//
// This file contains the implementation of the StringTools class
//

#include "StringTools.h"

// CountWords():
//  Returns the number of words in a string, white space delimited.
//   If quoted_word is true, then anything within quotes is considered
//   a single word.
int StringTools::CountWords( const char *string, bool quoted_word ) {
  int  count = 0;
  int  length = strlen( string );
  bool last_was_space = false;
  bool state_change = false;

  bool in_quotes = false;
  for( int i=0; i < length; i++ ) {
    if( quoted_word ) {
      if( in_quotes )
        in_quotes = !in_quotes;
    }

    if( !in_quotes ) {
      if( (string[i] == ' '  ) ||
          (string[i] ==  9   ) ||
          (string[i] == '\n' ) ||
          (string[i] == '\r' ) ) {
        state_change = (last_was_space == false) ? true : false;
        last_was_space = true;
      } else {
        if( i == 0 )
          count++;
        else
          state_change = (last_was_space == true)  ? true : false;

        last_was_space = false;
      }
    }

    if( state_change )
      count++;
  }

  return count;
}

// FindWord():
//  Returns a pointer to the x'th word in the string, with 0
//   being the first word, skipping all white space encountered.
//   Returns NULL if that word isn't in the string.  If quoted_word
//   is true, than anything surrounded by quotes is considered a
//   single word.
char * StringTools::FindWord( const char *string, int index, bool quoted_word ) {
  char * match = StringTools::SkipWhiteSpace( string );         // Skip any initial white space

  for( int i=0; i < index; i++ ) {
    match = StringTools::FindWhiteSpace( match, quoted_word );  // Skip this word
    if( match[0] == '\0' )                                      //   Error: no more words after this one
      return NULL;

    match = StringTools::SkipWhiteSpace( match );               // Skip the space between these two words
    if( match[0] == '\0' )                                      //   Error: end of string reached before next word was found
      return NULL;
  }

  return match;                                                 // OK; return this word
}

// FindWordLength():
//  Finds the length of the word pointed at by string (ie:
//   the number of chars from the current char to the first
//   white space).  Returns 0 if the first character is a
//   white space.
int StringTools::FindWordLength( const char *string, bool quoted_word ) {
  char * white = StringTools::FindWhiteSpace( string, quoted_word );
  return white - string;
}

// RemoveHeadWhiteSpace():
//  Removes the leading white space at the begining of the string.
//   Returns a pointer to the buffer.
char * StringTools::RemoveHeadWhiteSpace( char *string ) {
  strcpy( string, StringTools::SkipWhiteSpace( string ) );
  return string;
}

// RemoveTailWhiteSpace():
//  Removes the trailing white space at the end of the string.
//   Returns a pointer to the buffer.
char * StringTools::RemoveTailWhiteSpace( char *string ) {
  int length = strlen( string );
  for( int i=length-1; i >= 0; i-- ) {
    if( (string[i] == ' '  ) ||
        (string[i] ==  9   ) ||
        (string[i] == '\n' ) ||
        (string[i] == '\r' ) ) {
      string[i] = '\0';
    } else {
      break;
    }
  }

  return string;
}

// RemoveCappingWhiteSpace():
//  Removes the white space at the begining and end of the string.
//   Returns a pointer to the buffer
char * StringTools::RemoveCappingWhiteSpace( char *string ) {
  RemoveHeadWhiteSpace( string );
  RemoveTailWhiteSpace( string );
  return string;
}

// FindWhiteSpace():
//  Returns a pointer to the first white space in the string.
//   May return an empty string if there is no more white space
//   in the string.  If quoted_word is true, then anything within
//   quotes will be considered a single word.  Returns an empty
//   string if the quotes aren't closed.
char * StringTools::FindWhiteSpace( const char *string, bool quoted_word ) {
  int length = strlen( string );

  bool in_quotes = false;
  for( int i=0; i < length; i++ ) {
    if( quoted_word ) {
      if( string[i] == '\"' )
        in_quotes = !in_quotes;
    }

    if( !in_quotes ) {
      if( (string[i] == ' '  ) ||
          (string[i] ==  9   ) ||
          (string[i] == '\n' ) ||
          (string[i] == '\r' ) ) {
        break;
      }
    }
  }

  return (char *)&(string[i]);
}

// SkipWhiteSpace():
// Returns a pointer to the first character in the string
//  that isn't a white space.  This can be an empty string
//  if the original string contains only white space.
char * StringTools::SkipWhiteSpace( const char * string ) {
  int length = strlen( string );
  char *start = (char *)string;

  for( int i=0; i < length; i++ ) {
    if( (string[i] == ' '  ) ||
        (string[i] == '\t' ) ||
        (string[i] == '\n' ) ||
        (string[i] == '\r' ) ) {
      start++;
    } else {
      break;
    }
  }

  return start;
}

// IsolateWord():
//  Isolates the index'th word in the string and copies it into the buffer.
//   If quoted_word is true, then anything in quotes is considered a single
//   word.  If keep_quotes is true, then the quotes will exist in the output
//   string; otherwise, they will be wiped.
char * StringTools::IsolateWord( const char *string, char *buffer, int index,
                                 bool quoted_word, bool keep_quotes ) {
  char *start = (char *)string;
  start = SkipWhiteSpace( start );

  for( int i = 0; i < index; i++ ) {       // Find the start of the index'th word
    start = FindWhiteSpace( start, quoted_word );
    start = SkipWhiteSpace( start );
  }

  char * end = FindWhiteSpace( start, quoted_word );    // Find the end of the word
  if( end - start > 1 ) {
    if( (start[0] == '\"') && !keep_quotes )            // Strip off lead quotes, if requested
      start++;
  }

  if( end - start > 1 ) {
    if( (end[-1] == '\"') && !keep_quotes )             // Strip off tail quotes, if requested
      end--;
  }
  
  strncpy( buffer, start, end - start );                // Copy it into the buffer
  buffer[ end - start ] = '\0';                         // End the string

  return buffer;
}


// stristr():
//  Searches for a string in another string, case insensitive.  This should be
//   a standard function, but for some reason isn't.  <sigh>  See strstr() for
//   more information.
char * StringTools::stristr( const char *searched, const char *key ) {
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
      if( strnicmp( char_match, key, k_len ) == 0 )
        return (char *)char_match;
    }
  }

  return NULL;
}

// strdup():
//  Works just like the standard strdup(), but uses new instead of
//   malloc() to allocate memeory for the new string.  This is just
//   so you can delete instead of free()'ing your strings.
char * StringTools::strdup( const char * source ) {
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
char * &StringTools::strdup2( char * &output, const char *source ) {
  if( output != NULL ) {
    delete output;
    output = NULL;
  }

  if( source != NULL ) {
    if( source[0] != '\0' )
      output = StringTools::strdup( source );
  }

  return output;
}

// Reverse():
//  Reverses a string.  Returns a pointer to the reversed string.
//   Len is the optional length of the string.  If it is -1, the
//   length will be computed with strlen().  This is an in-place
//   operation.
char * StringTools::Reverse( char *string, int len ) {
  // Figure out the string length, if not provided
  if( len == -1 )
    len = strlen( string );
  len--;

  char c;

  for( int i=0; i < len; i++, len-- ) {
    c = string[i];
    string[i] = string[len];
    string[len] = c;
  }

  return string;
}

// Reverse():
//  Reverses a string.  Returns a pointer to the reversed string.
//   Len is the optional length of the string.  If it is -1, the
//   length will be computed with strlen().  This variant is does
//   not change the original string, but rather fills in the output
//   string specified with the the reversed source.
char * StringTools::Reverse( char *output, const char *string, int len ) {
  // Figure out the string length, if not provided
  if( len == -1 )
    len = strlen( string );

  for( int i=0; i < len; i++)
    output[len - i-1] = string[i];

  return output;
}
