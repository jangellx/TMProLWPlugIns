//
// bifstream.cpp
//
// This file contains the implementation of the bifstream class.
//

#include "bifstream.h"

// Constructors
//  Note that every bifstream file should always be opened in
//  binary mode, or converted to binary mode before using EOL-
//  specific IO routines like getline( char*, char*, int )

// Constructor()
bifstream::bifstream() : ifstream(),
                         bfstream() {
  B_Init();
}

// Contructor( char *, int, int )
//  Note that the file is ALWAYS opened in Binary mode
bifstream::bifstream( const char *_name, int _ios,
                      int _filebuf )
                      : ifstream( _name, _ios|binary, _filebuf ),
                      bfstream() {
  ;
}

// Constructor( filedesc )
bifstream::bifstream( filedesc _filedesc )
                      : ifstream( _filedesc ),
                      bfstream() {
  B_Init();
}

// Constructor( filedesc, char *, int )
bifstream::bifstream( filedesc _filedesc, char *name,
                      int _value )
                      : ifstream( _filedesc, name, _value ),
                      bfstream() {
  B_Init();
}


// B_Init():  Shared Initilization Function
//  Handles initilization that must be done by all
//  constructors (initialize variables, etc).
void bifstream::B_Init() {
  setmode( binary );
}


// Destructor
bifstream::~bifstream() {
  ;
}


// setmode():  An overloaded version of the normal setmode()
//  function.  This setmode() will only change the state to
//  binary mode.
int bifstream::setmode(int mode) {
  return( ifstream::setmode( binary ) );
}

//
// Operator Overloads
//

// operator>>( char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
bifstream & bifstream::operator>>( char &var ) {
  (*this).get( var );
  return *this;
}

// operator>>( unsigned char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
bifstream & bifstream::operator>>( unsigned char &var ) {
  (*this).get( var );
  return *this;
}

// operator>>( signed char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
bifstream & bifstream::operator>>( signed char &var ) {
  (*this).get( var );
  return *this;
}


// operator>>( char * )
//  Reads from the input stream until a null character (ie: '\0') is found)
//   and stores it as a string.
bifstream & bifstream::operator>>( char * var ) {
  int i = -1;

  do {
    this->get( var[++i] );          // read a char into the array
  } while ( var[i] != '\0' );       // stop when we find a string terminator

  return *this;
}


// operator>>( short )
//  Reads in a short from the input stream, swapping endians if needed.
//   shorts are assumed to be 2 bytes.
bifstream & bifstream::operator>>( short &var ) {
  read( (char *)&var, 2 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( unsigned short )
//  Reads in a unsigned short from the input stream, swapping endians if
//   needed.  unsigned shorts are assumed to be 2 bytes.
bifstream & bifstream::operator>>( unsigned short &var ) {
  read( (char *)&var, 2 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( long )
//  Reads in a long from the input stream, swapping endians if needed.
//   longs are assumed to be 4 bytes.
bifstream & bifstream::operator>>( long &var ) {
  read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( unsigned long )
//  Reads in a unsigned long from the input stream, swapping endians
//   if needed.  unsigned longs are assumed to be 4 bytes.
bifstream & bifstream::operator>>( unsigned long &var ) {
  read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( float )
//  Reads in a float from the input stream, swapping endians if needed.
//   floats are assumed to be 4 bytes.
bifstream & bifstream::operator>>( float &var ) {
  read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( double )
//  Reads in a double from the input stream, swapping endians if needed.
//   doubles are assumed to be 8 bytes.
bifstream & bifstream::operator>>( double &var ) {
  read( (char *)&var, 8 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( int )
//  Asserts, since the size of an int is platform specific.  Read in longs
//   or shorts, not ints.
bifstream & bifstream::operator>>( int &var ) {
  // As int sizes are platform specific, they are not supported.  Read in
  //  longs or shorts instead.
  assert( 0 ); // DON'T READ IN INTS!!!
  return *this;
}

// operator>>( unsigned int )
//  Asserts, since the size of an int is platform specific.  Read in longs
//   or shorts, not ints.
bifstream & bifstream::operator>>( unsigned int &var ) {
  // As int sizes are platform specific, they are not supported.  Read in
  //  longs or shorts instead.
  assert( 0 ); // DON'T READ IN UNSIGNED INTS!!!
  return *this;
}


// GetString()
//  Reads from the input stream until a null character (ie: '\0') is found)
//   and stores it as a string.  Len is the length of the buffer we are
//   reading into.
char * bifstream::GetString( char * string, int len ) {
  int i = -1;

  do {
    this->get( string[++i] );        // read a char into the array

    // See if the buffer is full
    if( i+2 == len ) {
      string[i+1] = '\0';
      break;
    }

  } while( string[i] != '\0' );      // stop when we find a string terminator

  return string;
}
