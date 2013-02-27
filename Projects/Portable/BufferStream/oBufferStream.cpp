//
// oBufferStream.cpp
//

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "oBufferStream.h"

// Constructor
oBufferStream::oBufferStream()
  : bfstream() {
  Close();
}

// Constructor
oBufferStream::oBufferStream( char *_buffer, long _len ) {
  Open( _buffer, _len );
}

// Destructor
oBufferStream::~oBufferStream() {
  ;
}

// Open():
//  Sets the buffer that will be read from and it's length.
bool oBufferStream::Open( char *_buffer, long _len ) {
  Close();

  // Make sure the buffer and length are valid
  if( (_buffer == NULL) || (_len < 1) ) {
    is_error = true;
    return false;
  }

  buffer = _buffer;
  len    = _len;

  return true;
}

// Close():
//  Closes the buffer.  Pretty much just resets the instance.
void oBufferStream::Close() {
  buffer   = NULL;
  len      = 0;
  pos      = 0;
  is_error = false;
}

// Seek():
//  Seeks to a new location in the buffer relative
//   to the offset value provided (ie: beg, cur or end).
//   This is more or less the same as the normal pstream's
//   seekp().
//  Returns false and sets the error state if this would pass
//   the begining or end of the buffer.
bool oBufferStream::Seek( long _pos, obuf_offset offset ) {
  long new_pos;

  if( offset == beg )
    new_pos = _pos;

  if( offset == cur )
    new_pos = pos + _pos;

  if( offset == end )
    new_pos = len + _pos;

  if( (new_pos < 0) || (new_pos > len) ) {
    is_error = true;
    return false;
  }

  pos = new_pos;

  return true;
}

//
// Write Functions
//

// Put( char *, int )
//  Writes _len characters to the buffer from _buf.
//   Returns false if there aren't that many characters
//   to read, and true if everything's OK.
bool oBufferStream::Put( const char * _buf, int _len ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return false;
  }

  if( (pos + _len) > len ) {
    is_error = true;
    return false;
  }

  memcpy( &(buffer[ pos ]), _buf, _len );
  pos += _len;

  return true;
}

// Put( char )
//  Writes a single char to the buffer.  Returns
//   false if an error occured (like hitting the eob).
bool oBufferStream::Put( char byte ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return NULL;
  }

  buffer[pos++] = byte;

  return true;
}


// PutString():
//  Writes a null-terminated string to the buffer, including the '\0'.
//   Returns false if the end of the buffer was read past.
bool oBufferStream::PutString( const char * string ) {
  return Put( string, strlen( string ) + 1 );
}


//
// Operator Overloads
//

// operator<<( char )
//  Writes a single character to the stream
oBufferStream & oBufferStream::operator<<( char var ) {
  Put( var );
  return *this;
}

// operator<<( unsigned char )
//  Writes a single character to the stream
oBufferStream & oBufferStream::operator<<( unsigned char var ) {
  Put( (char)var );
  return *this;
}

// operator<<( signed char )
//  Writes a single character to the stream
oBufferStream & oBufferStream::operator<<( signed char var ) {
  Put( (char)var );
  return *this;
}

// operator<<( const char * )
//  writes out a string from the output stream.
oBufferStream & oBufferStream::operator<<( const char * var ) {
  PutString( var );
  return *this;
}

// operator<<( const unsigned char * )
//  writes out a string from the output stream.
oBufferStream & oBufferStream::operator<<( const unsigned char * var ) {
  PutString( (const char *)var );
  return *this;
}

// operator<<( short )
//  writes out a short from the output stream, swapping endians if needed.
//   shorts are assumed to be 2 bytes.
oBufferStream & oBufferStream::operator<<( short var ) {
  ConvertEndian( var );                // Change the endian, if needed
  Write( (char *)&var, 2 );            // write out the value
  return *this;
}

// operator<<( unsigned short )
//  writes out a unsigned short from the output stream, swapping endians if
//   needed.  unsigned shorts are assumed to be 2 bytes.
oBufferStream & oBufferStream::operator<<( unsigned short var ) {
  ConvertEndian( var );                // Change the endian, if needed
  Write( (char *)&var, 2 );            // write out the value
  return *this;
}

// operator<<( long )
//  writes out a long from the output stream, swapping endians if needed.
//   longs are assumed to be 4 bytes.
oBufferStream & oBufferStream::operator<<( long var ) {
  ConvertEndian( var );                // Change the endian, if needed
  Write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( unsigned long )
//  writes out a unsigned long from the output stream, swapping endians
//   if needed.  unsigned longs are assumed to be 4 bytes.
oBufferStream & oBufferStream::operator<<( unsigned long var ) {
  ConvertEndian( var );                // Change the endian, if needed
  Write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( float )
//  writes out a float from the output stream, swapping endians if needed.
//   floats are assumed to be 4 bytes.
oBufferStream & oBufferStream::operator<<( float var ) {
  ConvertEndian( var );                // Change the endian, if needed
  Write( (char *)&var, 4 );            // write out the value
  return *this;
}

// operator<<( double )
//  writes out a double from the output stream, swapping endians if needed.
//   doubles are assumed to be 8 bytes.
oBufferStream & oBufferStream::operator<<( double var ) {
  ConvertEndian( var );               // Change the endian, if needed
  Write( (char *)&var, 8 );           // write out the value
  return *this;
}

// operator<<( int )
//  Asserts, since the size of an int is platform specific.  Cast ints to longs
//   or shorts instead.
oBufferStream & oBufferStream::operator<<( int var ) {
  // As int sizes are platform specific, they are not supported.  Cast ints to
  //  longs or shorts instead.
  assert( 0 ); // DON'T WRITE OUT INTS!!!
  return *this;
}

// operator<<( unsigned int )
//  Asserts, since the size of an int is platform specific.  Cast ints to longs
//   or shorts instead.
oBufferStream & oBufferStream::operator<<( unsigned int var ) {
  // As int sizes are platform specific, they are not supported.  Cast ints to
  //  longs or shorts instead.
  assert( 0 ); // DON'T WRITE OUT UNSIGNED INTS!!!
  return *this;
}
