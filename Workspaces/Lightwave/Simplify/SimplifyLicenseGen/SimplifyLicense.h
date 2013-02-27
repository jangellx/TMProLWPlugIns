//
// SimplifyLicense.h
//

#include <math.h>

// GenerateSimplifyLicense():
//  This simply generates a Simplify License Key based on the
//   dongle ID provided.
inline char * GenerateSimplifyLicense( int dongle_id ) {
  static char buffer[64];
  double a = ( tan( (double)dongle_id ) * 1000000.0 ) * 2;
  double b = ((dongle_id % 546) + (dongle_id / 645) * 100000) / 3;

  a = a/2 * dongle_id;

  sprintf( buffer, "%o-%o", (long)a, (long)b );
  return buffer;
}

