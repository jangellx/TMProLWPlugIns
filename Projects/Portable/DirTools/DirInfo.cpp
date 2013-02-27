//
// DirInfo.cpp
//
// This file contains the DirInfo class implementation.
//

#include "string.h"
#include "io.h"
#include "stdio.h"
#include "fstream.h"

#include "DirInfo.h"
#include "DirStrings.h"

#include "errno.h"

#ifdef WIN32
#include "direct.h"
#endif

// Constructor
DirInfo::DirInfo( const char *_fullpath )
  : extension_list(NULL), header_list(NULL),
    type_flags( DI_TYPE_ANY ), mode_flags( DI_MODE_ALL ),
    attributes(0), file_type(DI_TYPE_FILE), get_header(true)
#ifdef WIN32
    , find_handle(NULL)
#endif
     {

  if( _fullpath ) {                    // Path assigned; make a local copy
    strcpy( full_path, _fullpath );
    DirStrings::GetPathPart( path,    full_path );
    DirStrings::GetFilePart( pattern, full_path );
  } else {                             // No path assigned; clear to empty
    path[0]      = '\0';
    pattern[0]   = '\0';
    full_path[0] = '\0';
  }

  header[0] ='\0';

#ifdef WIN32
  find_data = new WIN32_FIND_DATA;
#endif
}

// Destructor
DirInfo::~DirInfo() {
  CloseSearch();
#ifdef WIN32
  delete find_data;
#endif
}

// ---
// ------------------------------------------ Directory Control ----------
// ---

// ChangeDir()
//  Change the directory name
bool DirInfo::ChangeDir( const char *new_dir ) {
  if( !new_dir )
    return false;

  strcpy( path, new_dir );
  DirStrings::ChangeDir( full_path, path );

  CloseSearch();
  return true;
}

// ChangePath()
//  Change the directory name.  Synonym for ChangeDir()
bool DirInfo::ChangePath( const char *new_dir ) {
  return ChangeDir( new_dir );
}

// ChangeDirectory()
//  Change the directory name.  Synonym for ChangeDir()
bool DirInfo::ChangeDirectory( const char *new_dir ) {
  return ChangeDir( new_dir );
}


// ChangeFullPath()
//  Change the file path and pattern.
bool DirInfo::ChangeFullPath( const char *new_path ) {
  if( !new_path )
    return false;

  strcpy( full_path, new_path );
  DirStrings::GetPathPart( path, full_path ) ;
  DirStrings::GetFilePart( pattern, full_path );

  CloseSearch();
  return true;
}

// ChangePattern()
//  Change the file pattern.
bool DirInfo::ChangePattern( const char *new_pattern ) {
  if( !new_pattern )
    return false;

  strcpy( pattern, new_pattern );
  strcpy( full_path, path );
  DirStrings::ChangeDir( pattern, path );

  CloseSearch();
  return true;
}

// ---
// -------------------------------------------- Search Routines ----------
// ---

// FindFirstWildcardMatch()
//  Uses the system wildcard functions to search for a file
//  matching the pattern.  If this function returns NULL,
//  there was either an error or no file was found.
const char *DirInfo::FindFirstWildcardMatch() {
  CloseSearch();                              // Initialization

#ifdef WIN32                                  // Windows Standard Pattern Matching
  find_handle = FindFirstFile( full_path, find_data );
  if( find_handle == INVALID_HANDLE_VALUE ) { // Error attempting to get file information
    find_handle = NULL;
    return NULL;
  }

  GetFileData();

  return last_match;
#endif


#ifdef AMIGA                                  // Amiga Standard Pattern Matching

#endif

  return NULL;  
}

// FindNextWildcardMatch()
//  Uses the system wildcard functions to search for a file
//  matching the pattern.  If this function returns NULL,
//  there was either an error or no file was found.
const char *DirInfo::FindNextWildcardMatch() {
#ifdef WIN32                                  // Windows Standard Pattern Matching
  if( !find_handle )                          // FindFirst hasn't been called yet, so do that instead
    return FindFirstWildcardMatch();

  if( FindNextFile( find_handle, find_data ) == 0 )
    return NULL;                              // Error or no more matches; call GetLastError() for more info

  GetFileData();

  return last_match;
#endif


#ifdef AMIGA                                  // Amiga Standard Pattern Matching

#endif

  return NULL;  
}

// FindFirst():
//  Find the first file in the dir (no pattern matching).
//   Returns NULL if there was an error or no match was
//   found.
const char *DirInfo::FindFirstMatch() {
  CloseSearch();                              // Initialization

#ifdef WIN32                                  // Windows Standard Matching
  find_handle = FindFirstFile( full_path, find_data );
  if( find_handle == INVALID_HANDLE_VALUE ) { // Error attempting to get file information
    find_handle = NULL;
    return NULL;
  }

  GetFileData();

  return last_match;
#endif


#ifdef AMIGA                                  // Amiga Standard Matching

#endif

  return NULL;  
}

// FindNext()
//  Find the next file in the dir (no pattern matching).
//   Returns NULL if there was an error or no match was
//   found.
const char *DirInfo::FindNext() {
#ifdef WIN32                                  // Windows Standard Matching
  if( !find_handle )                          // FindFirst hasn't been called yet, so do that instead
    return FindFirstMatch();

  if( FindNextFile( find_handle, find_data ) )
    return NULL;                              // Error or no more matches

  GetFileData();

  return last_match;
#endif


#ifdef AMIGA                                  // Amiga Standard Pattern Matching

#endif

  return NULL;  
}

// GetFileData()
//  Stores platform-specific data in our own data structures for
//  convenience
bool DirInfo::GetFileData() {
  bool ok = false;
#ifdef WIN32
  attributes = find_data->dwFileAttributes;       // Store the File Attributes
  strcpy( last_match, find_data->cFileName );     // Store the filename as the Last Match

  if( (attributes & FILE_ATTRIBUTE_DIRECTORY) )   // Get the File Type
    file_type = DI_TYPE_DIR;
  else
    file_type = DI_TYPE_FILE;

  memcpy( &creation_time,    &find_data->ftCreationTime,   sizeof( FILETIME ) );
  memcpy( &last_access_time, &find_data->ftLastAccessTime, sizeof( FILETIME ) );
  memcpy( &last_write_time,  &find_data->ftLastWriteTime,  sizeof( FILETIME ) );

  ok = true;

#endif

#ifdef AMIGA
  return false;
#endif

  if( ok && get_header ) {                        // Read in the header
    ifstream in( last_match, ios::in | ios::binary | ios::nocreate );
    if( in ) {
      in.read( header, MAX_HEADER_LENGTH );
      in.close();
    }
    return true;
  } else {
    header[0] = '\0';
  }

  return false;
}

// CloseSearch()
//  Platform-specific clean-up of the operating system directory
//  access routines.
bool DirInfo::CloseSearch() {
#ifdef WIN32                                 // Windows-Specific Cleanup
  if( find_handle ) {
    FindClose( find_handle );
    find_handle = NULL;
  }

  return true;
#endif

#ifdef AMIGA                                 // Amiga-Specific Cleanup

#endif
}

// ---
// ------------------------------------------- Static Functions ----------
// ---

// Exists():
//  Test to see if the specified file exists with the
//  given access mode
bool DirInfo::Exists( const char *path, int mode ) {
  if( access( path, mode ) == 0 )
    return true;

  return false;
}

// SetHeaderState()
//  Decide if the header should be read in every time a new
//  file is matched.  Set this to false if you just need a dir
//  listing, wince reading the header can be slow.  Returns
//  the previous state.
bool DirInfo::SetHeaderState( bool state ) {
  bool old = get_header;
  get_header = state;

  if( !get_header )
     header[0] = '\0';

  return get_header;
}

// HasHeader()
//  Tests to see if the header matches the input test.
//  Question marks can be used for unknown characters.
bool DirInfo::HasHeader( const char *test ) {
  if( !test )
    return false;

  if( strlen( test )> MAX_HEADER_LENGTH )  // The header can't be bigger than the header buffer
    return false;

  for( int i=0; test[i] != '\0'; i++ ) {
    if( test[0] != '?' ) {                // Wildcard; skip character
      if( test[0] != header[0] )
        return false;                     // Doesn't match; fail
    }
  }

  return true;
}

// MakeDir():
//  Creates a new directory if it doesn't already exist.
bool DirInfo::MakeDir( const char *path ) {
  if( !path )
    return false;

  return (CreateDirectory( path, NULL ) == TRUE) ? true : false;
}

// MakeTree():
//  Makes an entire tree of directories if any of the
//   directories in the path don't exist yet.  If has_file
//   is true, then the path will be tested to see if it
//   has a file part, and that part will not be made into
//   a directory.
//  Returns false if the path is to be made on a drive that
//   doesn't exist.
bool DirInfo::MakeTree( const char *path, bool has_file ) {
  char  buffer[ MAX_PATH_LENGTH ];
  char *working_path = (char *)path;

  // Find the end of the path
  if( has_file ) {  // Check for a file, if applicable
    char * file = DirStrings::HasFile( working_path );
    if( file != NULL )
      file[0] = '\0';
  }

  DirStrings::AddDirSeperator( working_path );

  // See if the drive exists, if applicable
  DirStrings::GetDrivePart( buffer, working_path );
  if( buffer[0] != '\0' ) {
    int bufLength = strlen( buffer ) - 1;
    buffer[ bufLength ] = '\0';

    if( !Exists( buffer ) )
      return false;                                 // Drive doesn't exist; abort

    working_path = working_path + bufLength + 1;
  }

  // Make dirs along the path.  This routine can probably be made a bit more efficient
  char * next_part = DirStrings::NextPathPart( working_path );
  while( next_part != NULL ) {
    int end = next_part - path - 1;
    strncpy( buffer, path, end );
    buffer[ end ] = '\0';                           // truncates the trailing slash, too

    if( !Exists( buffer ) ) {
      if( !MakeDir( buffer ) )
        return false;                               // Failed to make the directory; abort
    }

    next_part = DirStrings::NextPathPart( next_part );
  };
  
  return true;
}


// Delete():
//  Deletes a file from disk.  This is just your standard
//   delete operation, which destroys the file on the disk
//   using standard OS calls
int DirInfo::Delete( const char *file ) {
  int bad = remove( file );

  if( bad == -1 ) {
    switch( errno ) {
    case EACCES:
      return DI_DELETE_READ_ONLY;
    case ENOENT:
      return DI_DELETE_FILE_NOT_FOUND;
    default:
      return DI_DELETE_UNKNOWN_ERROR;
    }
  }

  return DI_DELETE_OK;
}

// Rename():
//  Renames a file on disk.
int DirInfo::Rename( const char *file, const char *name ) {
  if( Exists( name ) )
    return DI_RENAME_FILE_ALREADY_EXISTS;

  int bad = rename( file, name );
  if( bad == -1 ) {
    switch( errno ) {
    case EACCES:
      return DI_RENAME_READ_ONLY;
    case ENOENT:
      return DI_RENAME_FILE_NOT_FOUND;
    case EINVAL:
      return DI_RENAME_INVALID_CHARS;
    default:
      return DI_RENAME_UNKNOWN_ERROR;
    }
  }

  return DI_RENAME_OK;
}

// ChangeWorkingDir()
//  Change the Current Working Directory on OS's that support this.
//   This will return true and do nothing on other OS's.  Note that
//   this will also do a ChangeDrive() if change_drive is true.
bool DirInfo::ChangeWorkingDir( char *path, bool change_drive ) {
#ifdef WIN32
  if( change_drive ) {
    if( !ChangeWorkingDrive( path ) )
      return false;
  }

  if( _chdir( path ) == -1 )
    return false;
  else
    return true;
#else
  ;  // Insert meaningful code here
#endif
}

// ChangeWorkingDrive()
//  Change the Current Working Drive on OS's that support this.
//   This will return true and do nothing on other OS's
bool DirInfo::ChangeWorkingDrive( char *path ) {
#ifdef WIN32
  char drive[ 10 ];
  if( DirStrings::GetDrivePart( drive, path )[0] != '\0' ) {   // No Drive Part
    strlwr( drive );
    if( _chdrive( drive[0] - 'a' + 1 ) == -1 )
      return false;
    else
      return true;
  } else {
    return true;
  }
#else
  ;  // Insert meaningful code here
#endif
}

// IsDir( const char * ):
//  Returns true if the file pointed to is a directory.
bool DirInfo::IsDir( const char *path ) {
#ifdef WIN32
  DWORD attrib = GetFileAttributes( path );
  if( attrib & FILE_ATTRIBUTE_DIRECTORY )
    return true;
  
  return false;
#else
  ;  // Insert meaningful code here
#endif
}

#ifdef WIN32
// HasBeenWrittenSince():
//  Returns true if the file was last written to after comp_time, and
//   false if the file doesn't exist or hasn't been written since then.
//  This is very Win32 due to the FILETIME datatype.  Update this to be
//   more platform-independant sometime.
bool DirInfo::HasBeenWrittenAfter( const char *path, FILETIME *comp_time ) {
  if( !DirInfo::Exists( path ) )
    return false;

  // See if the file was actually updated since rendering started
  __int64 comp = *((__int64 *)comp_time);
  if( comp != (__int64)0 ) {
    HANDLE file = CreateFile( path, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                              NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    if( file == INVALID_HANDLE_VALUE )
      return false;

    __int64 end_time;
    GetFileTime( file, NULL, NULL, (FILETIME *)&end_time );
    CloseHandle( file );

    if( end_time <= comp )
      return false;
  }

  return true;
}
#endif