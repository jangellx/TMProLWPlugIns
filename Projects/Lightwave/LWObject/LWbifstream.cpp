//
// LWbifstream.cpp
//

#include "lwbifstream.h"

// LWVertex
//  Variable-sized union used when reading VX's
union LWVetrex {
  unsigned short short_vert[2];
  unsigned long  long_vert; };

// ReadVX():
unsigned long LWbifstream::ReadVX() {
  LWVetrex vertex;
  (*this) >> vertex.short_vert[1];

  if( vertex.short_vert[2] == 0xFF ) {
    (*this) >> vertex.short_vert[0];
    return vertex.long_vert & 0x00FFFFFF;
  } else {
    return vertex.short_vert[1];
  }
}

// GetLWString():
char * LWbifstream::GetLWString( char *string, int len ) {
  GetString( string, len );
  if( (strlen( string ) % 2) == 0 )         // NOTE:  length doesn't include the trailing /0; if it did, we'd test against 1 instead of 0
    seekg( 1, ios::cur );
  return string;
}

