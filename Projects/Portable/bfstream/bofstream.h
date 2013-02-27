//
// bofstream.h
//
// bofstream is a Binary File Input Stream class.  It's main feature is
//  the automatic handling of different endians based on platform, as well
//  as a series of helper functions to get different data types.
//
// - STREAM OVERLOADS -
// Using standard stream operations, you can write out to a binary file.
//  A key point is that the endians are automatically converted to the
//  mode you require for your application (ie: saving an IFF image on an
//  Intel machine would require the byte order to be reversed).  This allows
//  you to deal with writing your saver and not worry about the endian
//  conversions.
//
// The following data types are not overloaded by this class:
//  char      as chars are single bytes and do not need endian reversal,
//  char *    and unsigned/short.  Strings do not require endian conversion.
//             * actually, char * is now in here.  I don't understand why
//               the stream operators aren't being inherited...
//
// - DATATYPE SIZES -
// Datatypes are assumed to be of the following sizes.  This represents both
//  the signed and unsigned variants of each type:
//
//   char             1 byte
//   short            2 bytes
//   long             4 bytes
//   float            4 bytes
//   double           8 bytes
//
// Note that ints are NOT supported, as their size is platform-specific.  If
//  you attempt to write out an int, you will get an assert().  Cast it to a
//  long or short instead.
//

#ifndef BOFSTREAM_HEADER
#define BOFSTREAM_HEADER

#include <assert.h>
#include <fstream.h>

#include "bfstream_base.h"

class bofstream : public ofstream, public bfstream {
public:
  // Constructors/Destructor
  bofstream();
  bofstream( const char *_name, int _ios = ios::in,
             int _filebuf = filebuf::openprot );
  bofstream( filedesc _filedesc );
  bofstream( filedesc _fildesc, char *name, int _value );
  ~bofstream();

  inline void open( const char* szName, int nMode = ios::out, int nProt = filebuf::openprot );

  void B_Init(void);

  // Overloads
  int         setmode( int = filebuf::binary );  // Special overloaded setmode that can ONLY set files to BINARY mode

  bofstream & operator<<( char var );
  bofstream & operator<<( unsigned char var );
  bofstream & operator<<( signed char var );

  bofstream & operator<<( const char * var );
  bofstream & operator<<( const unsigned char * var );

  bofstream & operator<<( short var );
  bofstream & operator<<( unsigned short var );
  bofstream & operator<<( int var );
  bofstream & operator<<( unsigned int var );
  bofstream & operator<<( long var );
  bofstream & operator<<( unsigned long var );
  bofstream & operator<<( float var );
  bofstream & operator<<( double var );
  bofstream & operator<<( long double var );
};

inline void bofstream::open( const char* szName, int nMode, int nProt ) {
  ofstream::open( szName, nMode | ios::binary, nProt );
}

#endif