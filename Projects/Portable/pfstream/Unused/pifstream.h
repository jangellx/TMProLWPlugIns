
// pifsteram.h:  Portable ifstream class
//
// A simple class that inherits from ifstream.  The main feature
//  is handling of 1 or 2-character EOLs.  The matching output
//  class, pofstream, can also output 1 or 2-character EOLs.
//
// Note that in order for the EOL I/O routines to work correctly,
//  the file MUST be opened in BINARY mode, even thought these
//  functions handle ASCII files.  If opened in ASCII mode,
//  Windows would automatically skip the second EOL characters
//  (ie: the MS-DOS CR+LF EOL would only read in the CR).
//  Because of this, pifstream can ONLY be opened in Binary
//  mode, no matter what mode is passed to the constructor or
//  setmode().

#ifndef PIFSTREAM_INCLUDE
#define PIFSTREAM_INCLUDE

#include "fstream.h"
#include "eol_manager/eol_manager.h"

// The pifstream class itself
class pifstream : public ifstream, public EOLManager {
public:
  // -- Constructors/Destructor
	pifstream();
	pifstream(const char *_name, int _ios = ios::in,
            int _filebuf = filebuf::openprot);
	pifstream(filedesc _filedesc);
	pifstream(filedesc _filedesc, char * _name, int _value);
	~pifstream();

  // -- Member Functions
  int    GuessEOLType();                 // Attempts to determine the file's EOL type, and sets it as the current file's EOL type.  Returns the where the user was before this was called.
  char * getline( char *buffer, unsigned int bufsize,
                  int eoltype = -1 );    // getline() that handles all EOL types
	int    setmode(int = filebuf::binary); // Specail overloaded setmode that can ONLY set files to BINARY mode

protected:
  void P_Init();                         // Shared Constructor Initilization Routine
};


#endif // PIFSTREAM_INCLUDE