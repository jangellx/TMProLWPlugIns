//
// bofstream.cpp
//
// This file contains the implementation of the bofstream class.
//

#include "bofstream.h"

// Constructor()
bofstream::bofstream() : ofstream(),
                         bfstream() {
  B_Init();
}

// Contructor( char *, int, int )
//  Note that the file is ALWAYS opened in Binary mode
bofstream::bofstream( const char *_name, int _ios,
                      int _filebuf )
                      : ofstream( _name, _ios|binary, _filebuf ),
                      bfstream() {
  ;
}

// Constructor( filedesc )
bofstream::bofstream( filedesc _filedesc )
                      : ofstream( _filedesc ),
                      bfstream() {
  B_Init();
}

// Constructor( filedesc, char *, int )
bofstream::bofstream( filedesc _filedesc, char *name,
                      int _value )
                      : ofstream( _filedesc, name, _value ),
                      bfstream() {
  B_Init();
}


// B_Init():  Shared Initilization Function
//  Handles initilization that must be done by all
//  constructors (initialize variables, etc).
void bofstream::B_Init() {
  setmode( binary );
}


// Destructor
bofstream::~bofstream() {
  ;
}


// setmode():  An overloaded version of the normal setmode()
//  function.  This setmode() will only change the state to
//  binary mode.
int bofstream::setmode(int mode) {
  return( ofstream::setmode( binary ) );
}

//
// Operator Overloads
//

// operator<<( char )
//  Writes a single character to the stream
bofstream & bofstream::operator<<( char var ) {
  (*this).put( var );
  return *this;
}

// operator<<( unsigned char )
//  Writes a single character to the stream
bofstream & bofstream::operator<<( unsigned char var ) {
  (*this).put( var );
  return *this;
}

// operator<<( signed char )
//  Writes a single character to the stream
bofstream & bofstream::operator<<( signed char var ) {
  (*this).put( var );
  return *this;
}

// operator<<( const char * )
//  writes out a string from the output stream.
bofstream & bofstream::operator<<( const char * var ) {
  int len = strlen( var );
  write( var, len + 1 );               // write out the string, including NUL terminator
  return *this;
}

// operator<<( const unsigned char * )
//  writes out a string from the output stream.
bofstream & bofstream::operator<<( const unsigned char * var ) {
  int len = strlen( (const char *)var );
  write( var, len + 1 );               // write out the string, including NUL terminator
  return *this;
}

// operator<<( short )
//  writes out a short from the output stream, swapping endians if needed.
//   shorts are assumed to be 2 bytes.
bofstream & bofstream::operator<<( short var ) {
  ConvertEndian( var );                // Change the endian, if needed
  write( (char *)&var, 2 );            // write out the value
  return *this;
}

// operator<<( unsigned short )
//  writes out a unsigned short from the output stream, swapping endians if
//   needed.  unsigned shorts are assumed to be 2 bytes.
bofstream & bofstream::operator<<( unsigned short var ) {
  ConvertEndian( var );                // Change the endian, if needed
  write( (char *)&var, 2 );            // write out the value
  return *this;
}

// operator<<( long )
//  writes out a long from the output stream, swapping endians if needed.
//   longs are assumed to be 4 bytes.
bofstream & bofstream::operator<<( long var ) {
  ConvertEndian( var );                // Change the endian, if needed
  write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( unsigned long )
//  writes out a unsigned long from the output stream, swapping endians
//   if needed.  unsigned longs are assumed to be 4 bytes.
bofstream & bofstream::operator<<( unsigned long var ) {
  ConvertEndian( var );                // Change the endian, if needed
  write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( float )
//  writes out a float from the output stream, swapping endians if needed.
//   floats are assumed to be 4 bytes.
bofstream & bofstream::operator<<( float var ) {
  ConvertEndian( var );                // Change the endian, if needed
  write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( double )
//  writes out a double from the output stream, swapping endians if needed.
//   doubles are assumed to be 8 bytes.
bofstream & bofstream::operator<<( double var ) {
  ConvertEndian( var );               // Change the endian, if needed
  write( (char *)&var, 8 );           // write out the value
  return *this;
}

// operator<<( int )
//  Asserts, since the size of an int is platform specific.  Cast ints to longs
//   or shorts instead.
bofstream & bofstream::operator<<( int var ) {
  // As int sizes are platform specific, they are not supported.  Cast ints to
  //  longs or shorts instead.
  assert( 0 ); // DON'T WRITE OUT INTS!!!
  return *this;
}

// operator<<( unsigned int )
//  Asserts, since the size of an int is platform specific.  Cast ints to longs
//   or shorts instead.
bofstream & bofstream::operator<<( unsigned int var ) {
  // As int sizes are platform specific, they are not supported.  Cast ints to
  //  longs or shorts instead.
  assert( 0 ); // DON'T WRITE OUT UNSIGNED INTS!!!
  return *this;
}
