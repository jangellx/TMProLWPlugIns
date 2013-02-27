//
// FreeKeyLicense.h
//

#include <math.h>

// GenerateFreeKeyLicense():
//  This simply generates a FreeKey License Key based on the
//   dongle ID provided.
inline char * GenerateFreeKeyLicense( int dongle_id ) {
  static char buffer[64];
  double a = ( sin( (double)dongle_id ) * 1000000.0 ) + 2;
  double b = ((dongle_id % 137) + (dongle_id / 731) * 100000) - 3;

  a = a/2 * dongle_id;

  sprintf( buffer, "%o-%o", (long)a, (long)b );
  return buffer;
}

