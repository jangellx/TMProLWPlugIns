//
// L6io.h
//

#include <stdio.h>
#include <lwio.h>

#include <lwmotion.h>

// CLWSaveState
class CLWSaveState {
public:
  CLWSaveState( const LWSaveState *load_state );

  // Use New() to create a new instance of this class
  static CLWSaveState *New( const LWSaveState *save_state );

  void             (&Write)(void *writeData, char *buf, int len);  // Function to write out the data, as defined by Lightwave
  inline void  SimpleWrite( char *buf, int len) {                  // Function to write out the data; doesn't require writeData
    Write( writeData, buf, len );
  }

public:
  int           &ioMode;       // I/O Mode?
  void        * &writeData;    // Data to write out?
  LWSaveState *  orig_struct;  // The original structure used to create this class
};
