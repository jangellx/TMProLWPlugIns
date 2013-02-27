
// pofstream.h:  Portable ofstream class
//
// A simple extension of ofstream.  The main feature is
//  the ability to output whatever EOL type the user
//  wants.
// Note that since the EOL functions handle all EOLs
//  themselves, the output file must be opened in
//  binary mode.  This happens automatically in the
//  constructors.  A special overloaded version of
//  setmode() forces binary-only mode.  


#ifndef POFSTREAM_INCLUDE
#define POFSTREAM_INCLUDE

#include "fstream.h"
#include "eol_manager/eol_manager.h"

// The pofstream class itself
class pofstream : public ofstream, public EOLManager {
public:
  // -- Constructors/Destructor
	pofstream();
	pofstream(const char *_name, int _ios=ios::out,
            int _filebuf=filebuf::openprot);
	pofstream(filedesc);
	pofstream(filedesc, char *, int);
	~pofstream();

  // -- Member Functions
  inline char * endl() { return eol_types[eol]; }   // returns the EOL for the file.  Use this in place of endl.

	int    setmode(int = filebuf::binary); // Specail overloaded setmode that can ONLY set files to BINARY mode

protected:
  void P_Init();                         // Shared Constructor Initilization Routine
};


#endif // POFSTREAM_INCLUDE
