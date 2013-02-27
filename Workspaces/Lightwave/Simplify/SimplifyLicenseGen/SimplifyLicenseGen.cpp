//
// SimplifyLicenseGen
//

#include <stdio.h>
#include <stdlib.h>

#include <iostream.h>
#include <iomanip.h>

#include "SimplifyLicense.h"

void main( int argc, char **argv ) {
  cout << "Simplify License Key Generator" << endl;
  cout << " --- FOR INTERNAL USE BY TM PRODUCTONS ONLY ---" << endl;
  cout << "        Copyright 2000 TM Productions" << endl;

  for( int i=1; i < argc; i++ ) {
    cout << setiosflags(ios::dec) << setw( 7 ) << atoi( argv[i] )
         << "  ==  "  << GenerateSimplifyLicense( atoi(argv[i]) ) << endl;
  }
}