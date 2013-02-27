
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
//
// Due to some very poor EOL handling in the Motion Analysis Lightwave
//  Scene exporter, the new AnyEOL functions have been added.  These
//  force FreeKey to detect any EOL type at all.  If this state is
//  false, the normal EOL routines will be used.  Be sure to check
//  the state of this flag if it's not doing what you want it to.
//  By default, this option is disabled.
//

#ifndef PIFSTREAM_INCLUDE
#define PIFSTREAM_INCLUDE

#include "fstream.h"
#include "../eol_manager/eol_manager.h"

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
  int           GuessEOLType();                 // Attempts to determine the file's EOL type, and sets it as the current file's EOL type.  Returns the prev  EOL type

  inline bool   GetAnyEOL() { return any_eol; }              // Returns the Any EOL state
  inline void   SetAnyEOL( bool state ) { any_eol = state; } // Sets the Any EOL state
  char *        getline( char *buffer, unsigned int bufsize,
                         int eoltype = -1 );    // getline() that handles all EOL types
	int           setmode(int = filebuf::binary); // Specail overloaded setmode that can ONLY set files to BINARY mode

  inline void   open( const char* szName, int nMode = ios::out | ios::binary, int nProt = filebuf::openprot ) {
                  ifstream::open( szName, nMode | ios::binary, nProt ); }

protected:
  void P_Init();                         // Shared Constructor Initilization Routine
  bool any_eol;
};


#endif // PIFSTREAM_INCLUDE
