
// EOL_Manager.h:  EOL Management Class
//
// This class is used by pifstream and pofstream for EOL management

#ifndef EOLMANAGER_INCLUDE
#define EOLMANAGER_INCLUDE

#include "string.h"

// EOLManger Class Definition
class EOLManager {
public:
  // -- Constructor/Destructor
  EOLManager();
  ~EOLManager();

  // -- Member Functions
  bool SetEOLType( const char *type );     // set the EOL type for this file (string)
  bool SetEOLType( int type );             // set the EOL type for this file (enum)

  inline int  GetEOLType( void )           // get the EOL type for this file
    { return eol; }

  static int EOLString2Type( const char *type ); // returns an EOL type for the given string, or -1 if the string is an invalid EOL type
  static const char * EOLType2String( int type); // returns a string containing the EOL characters for the given EOL Type enum

  static bool IsEOL( int type );           // Tests to see if the value is a valid EOL type

  // -- Public Variables
  static const char *eol_types[3];         // All possible EOL types

  // EOL Enumerations
  //  Pass one of these to GetEOLString() and SetEOLType().
  //  GetEOLType() and DetermineEOLType() return from this
  //  enum.
  enum eols {
    LF = 0,                                // Line Feed Only (Amiga/Unix)
    CR,                                    // Carrige Return Only (Mac)
    CRLF,                                  // Line Feed followed by a Carrige Return (Windows)
    Num_EOLs
  };

protected:
  int eol;                                 // The EOL enum used by this file
};


#endif // EOLMANAGER_INCLUDE