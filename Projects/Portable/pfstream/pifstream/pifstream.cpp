
// pifstream.cpp

#include "string.h"
#include "assert.h"
#include "../pifstream/pifstream.h"

//
// --------------------------- pifstream Function Definitions
//

// Constructors
//  Note that every pifstream file should always be opened in
//  binary mode, or converted to binary mode before using EOL-
//  specific IO routines like getline( char*, char*, int )

// Constructor()
pifstream::pifstream() : ifstream(),
                         EOLManager() {
  P_Init();
}

// Contructor( char *, int, int )
//  Note that the file is ALWAYS opened in Binary mode
pifstream::pifstream( const char *_name, int _ios,
                      int _filebuf )
                      : ifstream( _name, _ios|binary, _filebuf ),
                      EOLManager() {
  P_Init();
}

// Constructor( filedesc )
pifstream::pifstream( filedesc _filedesc )
                      : ifstream( _filedesc ),
                      EOLManager() {
  P_Init();
}

// Constructor( filedesc, char *, int )
pifstream::pifstream( filedesc _filedesc, char *name,
                      int _value )
                      : ifstream( _filedesc, name, _value ),
                      EOLManager() {
  P_Init();
}


// P_Init():  Shared Initilization Function
//  Handles initilization that must be done by all
//  constructors (initialize variables, etc).
void pifstream::P_Init() {
  setmode( binary );
  any_eol = false;
}


// Destructor
pifstream::~pifstream() {
  ;
}


// getline( char *, int, int ):  This is a variation on the
//  standard getline() functions (which, incidentially, can
//  still be used instead of this function).  The file's
//  current EOL type is used if none is supplied
// Note that the file MUST be in binary mode for this to
//  work correctly; if it isn't not, it will assert.
char * pifstream::getline( char *buffer, unsigned int bufsize,
                           int eoltype ) {
  // Do some legality tests
  if( eoltype == -1 )                  // Handle -1 (current EOL type)
    eoltype = eol;
  else
    assert( IsEOL( eoltype ) );        // Bounds-check EOL  

  assert( buffer != NULL );            // Make sure the buffer isn't NULL

  char         c;
  const char * EOL       = EOLType2String(eoltype);
  int          EOLLength = strlen(EOL);
  int          sofar = 0;

  for( unsigned int i = 0; i < bufsize-1; i++ ) {
    get(c);

    // Test for errors/EOF
    if( !good() )
      break;
    if( eof() )
      break;

    // Add the char to the buffer
    buffer[i] = c; // Store the current char

    // Check for EOL(s)
    if( any_eol ) {
      // Any EOL type
      //  This is a bit hacky.  It just finds a LF or CR, and breaks.  If a CR is found,
      //  the next char will be tested to see if it is a LF (for CRLF checks).
      if( c == '\n' )
        break;
      else if( c == '\r' ) {
        get(c);

        // Test for errors/EOF
        if( !good() )
          break;
        if( eof() )
          break;
        
        // Test for the second EOL char in a CR/LF line
        if( c == '\n' )
          break;
        else
          putback( c );

        break;
      }
    } else {
      // Specified EOL type
      if( c == EOL[sofar] )
        sofar++;
      else
        sofar = 0;

      // Exit if we've found our EOL(s)
      if( sofar == EOLLength ) {
        sofar--;
        break;
      }
    }
  }

  // Finish Up
  buffer[ i - sofar ] = '\0';
  return buffer;
}


// setmode():  An overloaded version of the normal setmode()
//  function.  In order for the EOL I/O routines to work, this
//  should be set to binary at all times.  Thus, this setmode()
//  will only change the state to binary mode.
int pifstream::setmode(int mode) {
  return( ifstream::setmode( binary ) );
}

// GuessEOLType():  Attampts to determine the EOL type by
//  scanning the source file.  Returns the old eoltype, or
//  or -1 if the search failed.
// The search starts from the begining of the file.  This will
//  always work unless, for some reason, there is an LF or CR
//  in the middle of a line.  The file pointer is returned to
//  it's previous location once the scan is over.
// Note that this code is not general, and must be modified to
//  find new EOL types (although I doubt there will be any...)
int pifstream::GuessEOLType() {
  int oldeol = eol;
  int length = 0;  
  char c;

  // Store the file position and jump to the start
  streampos oldpos = tellg();
  seekg( 0, ios::beg );

  // Begin Search
  while( good() ) {
    read(&c, 1);

    // LF Test
    if( c == '\n' ) {
      eol = LF;
      break;
    }

    // CR/CR+LF Test
    if( c == '\r' ) {
      read(&c, 1);
      if( c == '\n' )
        eol = CRLF;
      else {
        eol = CR;
      }
      break;
    }
  };

  // Finish up
  seekg( oldpos, ios::beg );

  if( !good() )
    return( -1 );
  else
    return( oldeol );
}
