//
// Configfstream.cpp
//
// Contains the definitions for the classes found in
//  Configfstream.h
//

#include "Configfstream.h"

// Constructor()
Configifstream::Configifstream() : pifstream() {
  C_Init();
}

// Contructor( char *, int, int )
//  Note that the file is ALWAYS opened in Binary mode
Configifstream::Configifstream( const char *_name, int _ios,
                      int _filebuf )
                      : pifstream( _name, _ios|binary, _filebuf ) {
  C_Init();
}

// Constructor( filedesc )
Configifstream::Configifstream( filedesc _filedesc )
                      : pifstream( _filedesc ) {
  C_Init();
}

// Constructor( filedesc, char *, int )
Configifstream::Configifstream( filedesc _filedesc, char *name,
                      int _value )
                      : pifstream( _filedesc, name, _value ) {
  C_Init();
}


// C_Init():  Shared Initilization Function
//  Handles initilization that must be done by all
//  constructors (initialize variables, etc).
void Configifstream::C_Init() {
  setmode( filebuf::text );
  GuessEOLType();
  SetCommentStrings( configifstream_comment_strings );   // Default comment strings

  // Default Line Values
  current_line_number = -1;
  next_line_number    =  0;
  end_of_buffer       = true;
  line_buffer[0]      = '\0';
  carrot = line_buffer;
}


// Destructor
Configifstream::~Configifstream() {
  ;
}


// setmode():  An overloaded version of the normal setmode()
//  function.  This setmode() will only change the state to
//  binary mode.
int Configifstream::setmode(int mode) {
  return( ifstream::setmode( binary ) );
}

// ReadNextLine():
//  Reads the next line from the file.  Continues reading until
//   a line is found that is not blank and does not contain only
//   comments.  Returns the new line's number, or -1 if we're at
//   the end of file, and -2 if there was some error.
long Configifstream::ReadNextLine() {
  // Loop through multiple times (handles blank or comment-only lines)
  do {
    // Store the line's file position, if we haven't loaded this line before
    if( next_line_number == (long)line_positions.NumElements() )
      line_positions.Add( tellg() );

    // Set the current line number and increment the next line number
    current_line_number = next_line_number++;

    // Read in a line and remove the comments
    pifstream::getline( line_buffer, CONFIGFSTREAM_LINE_BUFFER_SIZE );
    if( KillComment( line_buffer ) ) {
      end_of_buffer = false;
      carrot = line_buffer;
      return current_line_number;
    }

    // EOF test
    if( eof() )
      return -1;

    // Error Test
    if( fail() || bad() )
      return -2;

  } while( true );
}

// KillComment():
//  Removes the comments from a string.  Returns false if the 
//   string contains only the comment
bool Configifstream::KillComment( char *var ) {
  // See if there are any comment strings to test against
  if( comment_strings == NULL )
    return true;

  // Strip the comments
  char *match;
  int i = 0;
  while( comment_strings[i] != NULL ) {             // Test against each comment string
    match = strstr( var, comment_strings[i++] );
    if( match != NULL )
      *match = '\0';                                // Match valid; set the EOL there
  }

  // Remove trailing white space and set the carrot
  StringTools::RemoveTailWhiteSpace( var );
  carrot = StringTools::SkipWhiteSpace( var );

  // Return false if the string is now empty
  if( carrot[0] == '\0' )
    return false;

  return true;
}

//
// Operator Overloads
//

// operator>>( char )
//  Reads a single character from the buffer, and does not skip any leading
//   white space at all.  Note that if the end of buffer is found, it the
//   variable will not be set.  Check eob() for this error
Configifstream & Configifstream::operator>>( char &var ) {
  if( !eob() ) {
    var = carrot[0];
    carrot++;
    TestForEOB();
  }

  return *this;
}

// operator>>( unsigned char )
//  Reads a single character from the buffer, and does not skip any leading
//   white space at all.  Note that if the end of buffer is found, it the
//   variable will not be set.  Check eob() for this error
Configifstream & Configifstream::operator>>( unsigned char &var ) {
  if( !eob() ) {
    var = carrot[0];
    carrot++;
    TestForEOB();
  }
  return *this;
}

// operator>>( signed char )
//  Reads a single character from the buffer, and does not skip any leading
//   white space at all.  Note that if the end of buffer is found, it the
//   variable will not be set.  Check eob() for this error
Configifstream & Configifstream::operator>>( signed char &var ) {
  if( !eob() ) {
    var = carrot[0];
    carrot++;
    TestForEOB();
  }
  return *this;
}

// operator>>( char * )
//  Reads one word from the buffer and stores it as a string.
Configifstream & Configifstream::operator>>( char * var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    char * new_pos = StringTools::FindWhiteSpace( carrot );

    if( new_pos[0] == '\0' )
      strcpy( var, carrot );                      // Rest of string is the word; copy it all 
    else {
      strncpy( var, carrot, new_pos - carrot );   // Part of string is the word; copy that part
      var[ new_pos - carrot + 1 ] = '\0';         //  Add the terminating null
    }

    carrot = new_pos;
    TestForEOB();
  }
  return *this;
}

// operator>>( short )
//  Reads one word from the buffer and stores it as a short
Configifstream & Configifstream::operator>>( short &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atoi( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( unsigned short )
//  Reads one word from the buffer and stores it as a unsigned short
Configifstream & Configifstream::operator>>( unsigned short &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atoi( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( long )
//  Reads one word from the buffer and stores it as a long
Configifstream & Configifstream::operator>>( long &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atol( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( unsigned long )
//  Reads one word from the buffer and stores it as a unsigned long
Configifstream & Configifstream::operator>>( unsigned long &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atol( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( float )
//  Reads one word from the buffer and stores it as a float
Configifstream & Configifstream::operator>>( float &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = (float)atof( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( double )
//  Reads one word from the buffer and stores it as a double
Configifstream & Configifstream::operator>>( double &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atof( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( int )
//  Reads one word from the buffer and stores it as a int
Configifstream & Configifstream::operator>>( int &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atoi( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// operator>>( unsigned int )
//  Reads one word from the buffer and stores it as a unsigned int
Configifstream & Configifstream::operator>>( unsigned int &var ) {
  if( !eob() ) {
    carrot = StringTools::SkipWhiteSpace( carrot );
    var = atoi( carrot );
    carrot = StringTools::FindWhiteSpace( carrot );

    TestForEOB();
  }
  return *this;
}

// getline():
//  Stores count characters from the buffer as a string.
//   The delimiter is ignored.
Configifstream & Configifstream::getline( char* string, int count, char delim ) {
  int len = strlen( carrot );
  if( len > count )
    len = count;

  strncpy( string, carrot, len );
  string[ len ] = '\0';

  carrot += len;
  TestForEOB();

  return *this;
}
