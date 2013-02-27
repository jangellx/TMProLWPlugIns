//
// CLIFileManipBase.h
//
// This contains the CLIFileManipBase class.  It contains
//  the most common functions used to create a command line
//  program that reads in a source file and generates an
//  output file.  Use this as the base class for other
//  command-line file manipulation programs.  This is also
//  a useful base class for GUI-base file manipulation
//  programs, as there is nothing really command-line
//  specific about it.
//
//  Note that the files are ASCII by default.  You might
//   want to add a function to switch the modes to binary,
//   or make a new subclass.  I just didn't need it when
//   I was writing this.
//
//  Things you'll want to add to the child class:
//   - GenerateNames() function that generates souce and output filenames
//   - Process() function that process the file(s)
//

#ifndef HEADER_CLIFILEMANIPBASE
#define HEADER_CLIFILEMANIPBASE

#include <string.h>
#include <fstream.h>
#include <portable/dirtools/dirtools.h>

extern const char *clifilemanipbase_error_strings[];

// Error IDs
// Values 0 through 100 are reserved; all other values
//  can be used for whatever you like
enum clifilemanipbase_error_ids {
  CLIFILEMANIPBASE_OK,
  CLIFILEMANIPBASE_SOURCE_DOESNT_EXIST,
  CLIFILEMANIPBASE_CANT_OPEN_SOURCE,
  CLIFILEMANIPBASE_INVALID_OUTPUT_PATH,
  CLIFILEMANIPBASE_FILES_NOT_OPEN,
  CLIFILEMANIPBASE_SAME_SOURCE_AND_OUTPUT,
  CLIFILEMANIPBASE_NUM_ERRORS };

class CLIFileManipBase;

// Class CLIFileManipBase
class CLIFileManipBase {
public:
  CLIFileManipBase( const char *_source_name = NULL,
                    const char *_output_name = NULL );

  ~CLIFileManipBase();

  bool               GenerateNames( const char *suffix );  // Generates an output name.  Overload for any other path initializations
  
  int                OpenFiles();                          // Make sure all the paths are valid, and open the source and output files
  int                OpenSource();                         // Open the Source File only
  int                OpenOutput();                         // Open the Output File only
  void               CloseFiles();                         // Close the source and output files

  inline void        SetSource( const char *new_source );  // Set the Source Path
  inline const char *GetSource() { return source_name; }   // Get the Source Path

  inline void        SetOutput( const char *new_output );  // Get the Output Path
  inline const char *GetOutput() { return output_name; }   // Set the Output Path

  inline bool        AreFilesOpen();                       // Returns true if the source or output files are open.  Overload to add other file checks, if needed

protected:
  char source_name[ MAX_PATH_LENGTH ];
  char output_name[ MAX_PATH_LENGTH ];

  ifstream *_in;
  ofstream *_out;
};

// AreFilesOpen():
//  Returns true if the source or output files are open
inline bool CLIFileManipBase::AreFilesOpen() {
  if( (_in == NULL) || (_out == NULL) )
    return false;

  return true;
}

// SetSource():
//  Set the source filename.  This can't be done if the source
//   or output files are open.
inline void CLIFileManipBase::SetSource( const char *new_source ) {
  if( !AreFilesOpen() ) {
    if( new_source == NULL )
      source_name[0] = '\0';
    else
      strcpy( source_name, new_source );
  }
}

// SetOutput():
//  Set the output filename.  This can't be done if the source
//   or output files are open.
inline void CLIFileManipBase::SetOutput( const char *new_output ) {
  if( !AreFilesOpen() ) {
    if( new_output == NULL )
      output_name[0] = '\0';
    else
      strcpy( output_name, new_output );
  }
}

#endif