//
// iBufferStream.cpp
//

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "iBufferStream.h"

// Constructor
iBufferStream::iBufferStream()
  : bfstream() {
  Close();
}

// Constructor
iBufferStream::iBufferStream( const char *_buffer, long _len ) {
  Open( _buffer, _len );
}

// Destructor
iBufferStream::~iBufferStream() {
  ;
}

// Open():
//  Sets the buffer that will be read from and it's length.
bool iBufferStream::Open( const char *_buffer, long _len ) {
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
void iBufferStream::Close() {
  buffer   = NULL;
  len      = 0;
  pos      = 0;
  is_error = false;
}

// Seek():
//  Seeks to a new location in the buffer relative
//   to the offset value provided (ie: beg, cur or end).
//   This is more or less the same as the normal istream's
//   seekg().
//  Returns false and sets the error state if this would pass
//   the begining or end of the buffer.
bool iBufferStream::Seek( long _pos, ibuf_offset offset ) {
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
// Read Functions
//

// Get( char *, int )
//  Reads _len characters from the buffer into _buf.
//   Returns NULL if there aren't that many characters
//   to read, and a pointer to _buf if everything's OK.
char * iBufferStream::Get( char * _buf, int _len ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return NULL;
  }

  if( pos + _len > len ) {
    is_error = true;
    return NULL;
  }

  memcpy( _buf, &(buffer[ pos ]), _len );
  pos += _len;

  return _buf;
}

// Get( char )
//  Reads a single char from the buffer.  Returns
//   false if an error occured (like hitting the eob).
bool iBufferStream::Get( char & byte ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return NULL;
  }

  byte = buffer[pos++];

  return true;
}


// GetString():
//  Reads a null-terminated string from a binary file (meaning it stops
//   reading when it finds a null character).  Returns false if no
//   null was found before _len was reached or if the end of the buffer
//   was read past.
char * iBufferStream::GetString( char * string, int _len ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return NULL;
  }

  for( int i=0; i < _len; i++ ) {
    if( (pos + i) >= len ) {          // Check for the end of the buffer
      is_error = true;
      return NULL;
    }

    string[i] = buffer[pos + i];      // Store the char (including '\0')
    if( buffer[pos + i] == '\0' )     // Test a char
      break;
  }

  // See if we passed _len before finding a null char
  if( i == _len ) {
    is_error = true;
    return NULL;
  }

  pos += i + 1;  // Set pos past the '\0' (hence the +1)

  return string;
}


//
// Operator Overloads
//

// operator>>( char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
iBufferStream & iBufferStream::operator>>( char &var ) {
  Get( var );
  return *this;
}

// operator>>( unsigned char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
iBufferStream & iBufferStream::operator>>( unsigned char &var ) {
  Get( (char &)var );
  return *this;
}

// operator>>( signed char )
//  Reads a single character from the stream, and does not skip any leading
//   white space at all.
iBufferStream & iBufferStream::operator>>( signed char &var ) {
  Get( (char &)var );
  return *this;
}


// operator>>( char * )
//  Reads from the input stream until a null character (ie: '\0') is found)
//   and stores it as a string.
iBufferStream & iBufferStream::operator>>( char * var ) {
  if( is_error || IsEOB() ) {
    is_error = true;
    return *this;
  }

  int i = -1;

  do {
    Get( var[++i] );                // read a char into the array

    if( pos >= len ) {              // Check for the end of the buffer
      is_error = true;
      break;
    }
  } while ( var[i] != '\0' );       // stop when we find a string terminator

  return *this;
}


// operator>>( short )
//  Reads in a short from the input stream, swapping endians if needed.
//   shorts are assumed to be 2 bytes.
iBufferStream & iBufferStream::operator>>( short &var ) {
  Read( (char *)&var, 2 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( unsigned short )
//  Reads in a unsigned short from the input stream, swapping endians if
//   needed.  unsigned shorts are assumed to be 2 bytes.
iBufferStream & iBufferStream::operator>>( unsigned short &var ) {
  Read( (char *)&var, 2 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( long )
//  Reads in a long from the input stream, swapping endians if needed.
//   longs are assumed to be 4 bytes.
iBufferStream & iBufferStream::operator>>( long &var ) {
  Read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( unsigned long )
//  Reads in a unsigned long from the input stream, swapping endians
//   if needed.  unsigned longs are assumed to be 4 bytes.
iBufferStream & iBufferStream::operator>>( unsigned long &var ) {
  Read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( float )
//  Reads in a float from the input stream, swapping endians if needed.
//   floats are assumed to be 4 bytes.
iBufferStream & iBufferStream::operator>>( float &var ) {
  Read( (char *)&var, 4 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( double )
//  Reads in a double from the input stream, swapping endians if needed.
//   doubles are assumed to be 8 bytes.
iBufferStream & iBufferStream::operator>>( double &var ) {
  Read( (char *)&var, 8 );             // Read in the value
  ConvertEndian( var );                // Change the endian, if needed
  return *this;
}

// operator>>( int )
//  Asserts, since the size of an int is platform specific.  Read in longs
//   or shorts, not ints.
iBufferStream & iBufferStream::operator>>( int &var ) {
  // As int sizes are platform specific, they are not supported.  Read in
  //  longs or shorts instead.
  assert( 0 ); // DON'T READ IN INTS!!!
  return *this;
}

// operator>>( unsigned int )
//  Asserts, since the size of an int is platform specific.  Read in longs
//   or shorts, not ints.
iBufferStream & iBufferStream::operator>>( unsigned int &var ) {
  // As int sizes are platform specific, they are not supported.  Read in
  //  longs or shorts instead.
  assert( 0 ); // DON'T READ IN UNSIGNED INTS!!!
  return *this;
}

