//
// LWbifstream.h
//
// This is the same as the normal bifstream class, except it
//  supports reading VX datatypes.  From the LWO2 File Format
//  Specification:
//
//   This is an index into an array of items (points or polygons),
//   or a collection of items each uniquely identified by an integer
//   (clips or envelopes). A VX is written as a variable length 2-
//   or 4-byte element. If the index value is less than 65,280 (0xFF00),
//   then the index is written as an unsigned two-byte integer.
//   Otherwise the index is written as an unsigned four byte integer
//   with bits 24-31 set. When reading an index, if the first byte
//   encountered is 255 (0xFF), then the four-byte form is being used
//   and the first byte should be discarded or masked out. 
//

#ifndef HEADER_LWBIFSTREAM
#define HEADER_LWBIFSTREAM

#include <portable/bfstream/bfstream.h>

// class LWbifstream
class LWbifstream : public bifstream {
public:
  // Constructors/Destructor
  LWbifstream() : bifstream() {;}
  LWbifstream( const char *_name, int _ios = ios::in,
               int _filebuf = filebuf::openprot ) : bifstream( _name, _ios, _filebuf) {;};
  LWbifstream( filedesc _filedesc ) : bifstream( _filedesc ) {;};
  LWbifstream( filedesc _filedesc, char *name, int _value ) : bifstream( _filedesc, name, _value ) {;};

  // Read Functions
  unsigned long   ReadVX();
  char          * GetLWString( char *string, int len );
};

#endif