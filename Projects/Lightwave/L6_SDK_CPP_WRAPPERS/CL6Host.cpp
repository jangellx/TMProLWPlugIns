//
// Cl6Host.cpp
// 
// This file contains the class definitions for the CLWRAN classes.
//  Basically, it just uses the contructor to convert the function
//  pointers from the structure provided into function references,
//  converts and data member pointers into references to those
//  pointers (functionally, they work the same as the did in the
//  struct), and adds some accessor functions along the way.
//

#include "Cl6Host.h"

// CMessage
CMessage::CMessage( LWMessageFuncs *message )
  : orig_struct( message ) {
 ;
}

CMessage * CMessage::New( LWMessageFuncs *message ) {
  if( message == NULL )
    return NULL;
  else
    return new CMessage( message );
}
