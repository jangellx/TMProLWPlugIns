//
// bfstream.h
//
// This file contains the implemenation of the bfstream_base class.
//

#include "bfstream_base.h"

// Constructor
bfstream::bfstream() {
  SetEndian( ENDIAN_INTEL );
}

// Destructor
bfstream::~bfstream() {
  SetEndian( ENDIAN_INTEL );
}

// ConvertEndian()
void bfstream::ConvertEndian( float &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}

// ConvertEndian()
void bfstream::ConvertEndian( double &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}

// ConvertEndian()
void bfstream::ConvertEndian( short &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}

// ConvertEndian()
void bfstream::ConvertEndian( long &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}

// ConvertEndian()
void bfstream::ConvertEndian( unsigned short &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}

// ConvertEndian()
void bfstream::ConvertEndian( unsigned long &value ) {
  switch( endian_mode ) {
  case ENDIAN_INTEL:
#ifndef WIN32
    ReverseEndian( value );
#endif
    break;

  case ENDIAN_MOTOROLA:
#ifdef WIN32
    ReverseEndian( value );
#endif
  }
}
