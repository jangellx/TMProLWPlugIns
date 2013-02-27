
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
#include "../eol_manager/eol_manager.h"

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
  const char * endl() {
    return eol_types[ eol ]; }
  const char * endline() {
    return eol_types[ eol ]; }
//                                 _outs << eol_types[eol];    // Use this instead of endl to output the EOL characters
//                                 _outs.flush();
//                                 return _outs; }

	int           setmode(int = filebuf::binary);                       // Special overloaded setmode that can ONLY set files to BINARY mode
  inline void   open( const char* szName, int nMode = ios::in | ios::binary, int nProt = filebuf::openprot ) {
                  ofstream::open( szName, nMode | ios::binary, nProt ); }

protected:
  void P_Init();                                               // Shared Constructor Initilization Routine
};

#endif // POFSTREAM_INCLUDE
