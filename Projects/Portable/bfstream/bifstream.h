//
// bifstream.h
//
// bifstream is a Binary File Input Stream class.  It's main feature is
//  the automatic handling of different endians based on platform, as well
//  as a series of helper functions to get different data types.
//
// - STREAM OVERLOADS -
// Using standard stream operations, you can read in from a binary file.
//  A key point is that the endians are automatically converted to the
//  mode you require for your application (ie: loading an IFF image on an
//  Intel machine would require the byte order to be reversed).  This allows
//  you to deal with writing your loader and not worry about the endian
//  conversions.
//
// While chars do not require endian conversion, the operator>>(char) and
//  variants are overridden.  This is because, by default, white space is
//  skipped by the stream operator, and a bug in MSVC ignores the
//  ios::skipws flag. It took me 3 hours to figure this out.  The new
//  extractor NEVER skips white space.
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
//  you attempt to read into an int, you will get an assert().
//

#ifndef BIFSTREAM_HEADER
#define BIFSTREAM_HEADER

#include <assert.h>
#include <fstream.h>

#include "bfstream_base.h"

class bifstream : public ifstream, public bfstream {
public:
  // Constructors/Destructor
  bifstream();
  bifstream( const char *_name, int _ios = ios::in,
             int _filebuf = filebuf::openprot );
  bifstream( filedesc _filedesc );
  bifstream( filedesc _fildesc, char *name, int _value );
  ~bifstream();

  inline void open( const char* szName, int nMode = ios::out, int nProt = filebuf::openprot );


  void B_Init(void);

  // Overloads
          int         setmode( int = filebuf::binary );  // Special overloaded setmode that can ONLY set files to BINARY mode

          bifstream & operator>>( char &var );
          bifstream & operator>>( unsigned char &var );
          bifstream & operator>>( signed char &var );
          bifstream & operator>>( char * var );
  inline  bifstream & operator>>( unsigned char * var ) { return (*this)>>((char *)var); }
  inline  bifstream & operator>>( signed char * var )   { return (*this)>>((char *)var); }
          bifstream & operator>>( short & var );
          bifstream & operator>>( unsigned short & var );
          bifstream & operator>>( int & var );
          bifstream & operator>>( unsigned int & var );
          bifstream & operator>>( long & var );
          bifstream & operator>>( unsigned long & var );
          bifstream & operator>>( float & var );
          bifstream & operator>>( double & var );
          bifstream & operator>>( long double & var );

  char * GetString( char * string, int len );          // Retrieves a string from a binary file.  The string must be null-terminated
};

inline void bifstream::open( const char* szName, int nMode, int nProt ) {
  ifstream::open( szName, nMode | ios::binary, nProt );
}

#endif
