//
// CLIFileManipBase.cpp
//
// This contains the implimentation of the functions defined
//  in CLIFileManipBase.h
//

#include "CLIFileManipBase.h"

const char *clifilemanipbase_error_strings[] = {
  "Everything's OK!",
  "The source file does not exist!",
  "The source file is not accesibale.  It may be locked by another program",
  "The output file is not accessable.  It may be write protected or locked",
  "The input and output files failed to open; aborting",
  "Source and Output files cannot have the same names!",
  "Don't use me -- I'm just the error enum count"
};

// Constructor
CLIFileManipBase::CLIFileManipBase( const char  *_source_name,
                            const char  *_output_name )
  : _in(NULL), _out(NULL) {

  // Default Source Name
  if( _source_name != NULL )
    strcpy( source_name, _source_name );
  else
    source_name[0] = '\0';

  // Default Output Name
  if( _output_name != NULL )
    strcpy( output_name, _output_name );
  else
    output_name[0] = '\0';
}

// Destructor
CLIFileManipBase::~CLIFileManipBase() {
  delete _in;
  delete _out;
}


// OpenFiles():
//  Tests to make sure the paths are valid and opens the files.
//   This also generates an output and ubernull name if none are
//   specified.  Returns an error if something went wrong.
//  You should call GenerateNames() before calling this function.
//   Overload me if you need to open more files.
int CLIFileManipBase::OpenFiles() {
  // See if the source and output names are the same
  if( stricmp( source_name, output_name ) == 0 )
    return CLIFILEMANIPBASE_SAME_SOURCE_AND_OUTPUT;

  // Open the Source File
  int error = OpenSource();
  if( error != CLIFILEMANIPBASE_OK )
    return error;

  // Open the Output File
  error = OpenOutput();
  if( error != CLIFILEMANIPBASE_OK ) {
    CloseFiles();
    return error;
  }

  return CLIFILEMANIPBASE_OK;
}

// OpenSource():
//  Tests and opens the source file
int CLIFileManipBase::OpenSource() {
  if( source_name[0] == '\0' )
    return CLIFILEMANIPBASE_SOURCE_DOESNT_EXIST;

  if( !DirInfo::Exists( source_name ) ) {
    return CLIFILEMANIPBASE_SOURCE_DOESNT_EXIST;
  }

  _in = new ifstream( source_name, ios::nocreate );
  if( !(*_in) ) {
    _in->close();
    return CLIFILEMANIPBASE_CANT_OPEN_SOURCE;
  }

  return CLIFILEMANIPBASE_OK;
}

// OpenOutput():
//  Tests and opens the output file
int CLIFileManipBase::OpenOutput() {
  _out = new ofstream( output_name );
  if( !(*_out) ) {
    _out->close();
    return CLIFILEMANIPBASE_INVALID_OUTPUT_PATH;
  }

  return CLIFILEMANIPBASE_OK;
}

// CloseFiles():
//  Close the source and output files.  Overload me if you
//  need to close more files
void CLIFileManipBase::CloseFiles() {
  delete _in;
  delete _out;

  _in  = NULL;
  _out = NULL;
}

// GenerateNames():
//  Generates new names for the output file and null object.
//   New names are generated only if the original names are
//   blank (ie: first char is \0).  Returns false if the
//   source is also empty.
bool CLIFileManipBase::GenerateNames( const char *suffix ) {
  // Generate an output name, if needed
  if( output_name[0] == '\0' ) {
    if( suffix == NULL ) {                                 // No suffix and invalid name; abort
      return false;
    } else {
      strcpy( output_name, source_name );                   // Copy the in name
      char * ext = DirStrings::HasExtension( output_name ); // Find any extension
      if( ext )                                             // Strip the extension
        *ext = '\0';
      strcat( output_name, suffix );                        // Append -<suffix>
      ext = DirStrings::HasExtension( source_name );        // Find the original extension
      if( ext )                                             // Append the origianl extension to the output_name
        strcat( output_name, ext );
    }
  }

  return true;
}
