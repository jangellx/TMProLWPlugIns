//
// UberNull.h
//
// This file contains the class definition for the UberNull
//

#include <string.h>
#include <fstream.h>

#include <portable/dynarray/dynarray.h>
#include <portable/dirtools/dirtools.h>
#include <portable/pfstream/pfstream.h>

#include "UberNullNames.h"

// UberNull Version and Revision, which becomes Version.Revision
//  when presented to the user (ie: V1.6)
static const int UberNull_Version          = 1;
static const int UberNull_Revision         = 9;
static const int UberNull_SubRevision      = 3;
static const int UberNull_Settings_Version = 1;

enum ubernull_locaiton_ids {
  UBERNULL_FIRST = 0,
  UBERNULL_LAST,
  UBERNULL_NUM_LOCATIONS,
  UBERNULL_INVALID_LOCATION };

static const char *ubernull_location_strings[] = {
  "First object in scene",
  "Last object in scene",
  NULL };

enum ubernull_error_ids {
  UBERNULL_OK,
  UBERNULL_SOURCE_DOESNT_EXIST,
  UBERNULL_CANT_OPEN_SOURCE,
  UBERNULL_INVALID_OUTPUT_PATH,
  UBERNULL_NAME_INVALID,
  UBERNULL_FILES_NOT_OPEN,
  UBERNULL_ERROR_PROCESSING,
  UBERNULL_SAME_SOURCE_AND_OUTPUT,
  UBERNULL_NO_NULLS_ENABLED,
  UBERNULL_NO_NULLS_MATCHED,
  UBERNULL_NUM_ERRORS };

extern const char *ubernull_error_strings[];

// UberNull Class
class UberNull {
public:
  UberNull(const char  *_source_name   = NULL,
           const char  *_output_name   = NULL,
           const char  *_ubernull_name = NULL,
           unsigned int _location = UBERNULL_FIRST );
  ~UberNull();

  int  OpenFiles();                    // Make sure all the paths are valid, and open the source and output files
  void CloseFiles();                   // Close the source and output files
  bool GenerateNames();                // Generates names for the output file and the null object based on the source filename
  int  Process();                      // Add an UberNull to the Source and create the Output file

  inline void        SetSource( const char *new_source );  // Set the Source Path
  inline const char *GetSource();                          // Get the Source Path

  inline void        SetOutput( const char *new_output );  // Get the Output Path
  inline const char *GetOutput();                          // Set the Output Path


  inline bool        GetUseMasterNull() { return use_master; }                // Gets the use of the names array
  inline void        SetUseMasterNull( bool state ) { use_master = state; }   // Toggles the use of the names array

  inline void        SetMasterNullName( const char *new_null );  // Get the UberNull's Name
  inline const char *GetMasterNullName();                        // Set the UberNull's Name

  inline unsigned int SetMasterLocation( unsigned int new_loc ); // Set the UberNull Location in the Output File
  inline unsigned int GetMasterLocation();                       // Get the UberNull Location in the Output File

  inline bool        AreFilesOpen();                             // Returns true if the source or output files are open

  inline bool        GetUseNames() { return use_names; }                // Gets the use of the names array
  inline void        SetUseNames( bool state ) { use_names = state; }   // Toggles the use of the names array
  DynArray< UberNullNames * > names;                                    // The UberNullName Names Array

protected:
  int FindUberNullIndex();                                              // Returns the index of the ubernull object.

  char source_name[ MAX_PATH_LENGTH ];
  char output_name[ MAX_PATH_LENGTH ];

  char master_name[ MAX_PATH_LENGTH ];

  unsigned int master_location;

  bool use_master;
  bool use_names;

  int  ubernull_index;

  pifstream *in;
  pofstream *out;
};

// AreFilesOpen():
//  Returns true if the source or output files are open
inline bool UberNull::AreFilesOpen() {
  if( (in == NULL) || (out == NULL) )
    return false;

  return true;
}

// SetSource():
//  Set the source filename.  This can't be done if the source
//   or output files are open.
inline void UberNull::SetSource( const char *new_source ) {
  if( !AreFilesOpen() ) {
    if( new_source == NULL )
      source_name[0] = '\0';
    else
      strcpy( source_name, new_source );
  }
}

// GetSource():
//  Returns a read-only pointer to the source filename.
inline const char * UberNull::GetSource() {
  return source_name;
}

// SetOutput():
//  Set the output filename.  This can't be done if the source
//   or output files are open.
inline void UberNull::SetOutput( const char *new_output ) {
  if( !AreFilesOpen() ) {
    if( new_output == NULL )
      output_name[0] = '\0';
    else
      strcpy( output_name, new_output );
  }
}

// GetOutput():
//  Returns a read-only pointer to the output filename.
inline const char * UberNull::GetOutput() {
  return output_name;
}

// SetMasterNullName():
//  Set the UberNull name.  This can't be done if the source
//   or output files are open.
inline void UberNull::SetMasterNullName( const char *new_null ) {
  if( !AreFilesOpen() ) {
    if( new_null == NULL )
      master_name[0] = '\0';
    else
      strcpy( master_name, new_null );
  }
}

// GetMasterNullName():
//  Returns a read-only pointer to the UberNull name.
inline const char * UberNull::GetMasterNullName() {
  return master_name;
}

// SetLocation():
//  Set the location of the Null Object.  Returns
//  the previous location.
inline unsigned int UberNull::GetMasterLocation() {
  return master_location;
}

// SetLocation():
//  Set the location of the Null Object.  Returns
//  the previous location.
inline unsigned int UberNull::SetMasterLocation( unsigned int new_loc ) {
  if( new_loc >= UBERNULL_NUM_LOCATIONS )
    return UBERNULL_INVALID_LOCATION;

  unsigned int old_loc = master_location;
  master_location = new_loc;
  return old_loc;
}
