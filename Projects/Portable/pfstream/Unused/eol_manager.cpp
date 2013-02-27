
// eolmanager.cpp

#include "eol_manager/eol_manager.h"

//
// --------------------------- EOLManager Function Definitions
//

// Initialize Static Data Members
char *EOLManager::eol_types[3] = {
  "\n",
  "\r",
  "\r\n"
};

// Constructor
EOLManager::EOLManager() {
  // Set default EOL characters
  eol = LF; 
}

// Destructor
EOLManager::~EOLManager() {
  ;
}

// EOLString2Type():  Returns an EOL Type for the given
//  string, or -1 if the string isn't an EOL.  Note that
//  input string should contain ONLY the EOLs to compare
int EOLManager::EOLString2Type( const char *type ) {
  for( unsigned int i = 0; i < Num_EOLs; i++ ) {
    if( stricmp(  type, eol_types[i] ) == 0 )
      return( i );
  }
  return(-1);
}

// EOLType2String():  Converts the given EOL type into a
//  string.
const char * EOLManager::EOLType2String( int type ) {
  if( (type < Num_EOLs) && (type > -1) )
    return eol_types[ type ];
  else
    return NULL;
}


// SetEOLType(char *):  Changes this file's EOL type to the
//  given string, as long as that string is a valid EOL type
bool EOLManager::SetEOLType( const char *type ) {
  int neweol = EOLString2Type( type );
  if( neweol == -1 ) {
    return false;
  } else {
    eol = neweol;
    return true;
  }
}

// SetEOLType(int):  Changes this file's EOL type to the 
//  given enum
bool EOLManager::SetEOLType( int type ) {
  if( IsEOL( type ) ) {
    eol = type;
    return true;
  } else {
    return false;
  }
}

// IsEOL():  Checks to see if the given EOL type is valid.
bool EOLManager::IsEOL( int type ) {
  if( (type > -1) && (type < Num_EOLs) ) {
    return(true);
  } else {
    return(false);
  }
}
