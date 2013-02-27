
// postream.cpp
//

#include "string.h"
#include "assert.h"
#include "pofstream/pofstream.h"

//
// --------------------------- pofstream Function Definitions
//

// Constructor()
pofstream::pofstream()
 : ofstream(), EOLManager() {
  P_Init();
}

// Constructor( char*, int, int )
pofstream::pofstream( const char *_name, int _ios, int _filebuf)
  : ofstream(_name, _ios|binary, _filebuf), EOLManager() {
  ;
}

// Constructor( filedesc )
pofstream::pofstream( filedesc _filedesc )
  : ofstream( _filedesc ), EOLManager() {
  P_Init();
}

// Constructor( filedesc, char *, int )
pofstream::pofstream( filedesc _filedesc, char *name, int _value )
  : ofstream( _filedesc, name, _value ), EOLManager() {
  P_Init();
}

// Destructor()
pofstream::~pofstream() {
  ;
}

// P_Init():  Shared Initilization Function
//  Handles initilization that must be done by all
//  constructors (initialize variables, etc).
void pofstream::P_Init() {
  setmode( binary );
}

// setmode():  An overloaded version of the normal setmode()
//  function.  In order for the EOL I/O routines to work, this
//  should be set to binary at all times.  Thus, this setmode()
//  will only change the state to binary mode.
int pofstream::setmode(int mode) {
  return( ofstream::setmode( binary ) );
}