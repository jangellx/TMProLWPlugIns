//
// DirInfo.h
//
// This file contains the DirInfo class,
//  which handles platform-independant pattern
//  matching using either regular expressions or
//  the platform's own wildcards.
//
// Note that any directory passed in MUST end in a /,
//  \ or :.  The part of the path after the last slash
//  will be used as the pattern.  The only exception to
//  this is the ChangePath/ChangeDir commands, which
//  always assume the path is a directory only, and
//  ChangePattern(), which assumes the path is a pattern
//  only.
//
// The pattern passed in can be either platform-specific
//  wildcards (ie: QRau*.lwo), regular expressions (Amiga-
//  like wildcards), or a file (test.txt).  If a file is
//  passed in, you can use this to test for it's existance,
//  although it's probably better to use the Exists()
//  static member for this.
//
// Note that to get a listing of all the files in a
//  directory, you cannot simply specify the dir name.
//  must also specifiy a wildcard pattern (ie: e:\test\*)
//  or else only the directory itself will be matched.
//
// Files can be matched in the following manner:
//  - Type:
//      Either Drive, File, Dir.  Use this mask to limit
//      the search to just files or directories.  This
//      is used by all find functions.
//  - All (FindFirst()/FindNext()):
//      Finds any file in the dir that matches the Type.
//  - Wildcard (Find____WildcardMatch()):
//      Pattern match using the platform's wildcards.
//      The file must also match the Type.
//  - Regular Expressions(Find____RegExMatch()):
//      Pattern match using regular expressions.  This
//      is similar to the Amiga/UNIX wildcard system.
//      See the regular expression functions for more
//      information.
//  - Extension (Find____ExtensionMatch()):
//      Pattern match by extension.  An extension list
//      containing all extensions to test against must
//      be passed to SetExtensionList() before this can
//      be used.  For matching just one extension, the
//      platform-specific wildcards is probably just
//      as good.  The Regular Expression functions should
//      do a good job, too, if you don't want to use this
//  - Header (Find____HeaderMatch()):
//      Pattern match by header.  A header lsit containing
//      all headers to look for must be passed to
//      SetHeaderList() before using this function.  ?'s
//      can be used to mark unknown characters in the
//      header (ie: FORM????ILBM).  Header searchs are
//      the slowest search that can be performed, as
//      each file must be opened and tested. individually.
//  - Attributes (Find____AttributeMatch()):
//      Match by file attributes.  Have to flesh this out
//      so you can have "must have" and "can't have"
//      attributes.
//
// The functions described above check only one of the
//  match modes aech.  To manually check more than one
//  mode at once, use Find____(), Find___WildcardMatch()
//  or Find____RegExMatch(), and then test the
//  HasMatchingHeader(), HasMatchingExtension(),
//  HasMatchingType() and HasMatchingAttributes()
//  functions.
//
// You can also use the function SetMatchFlags() to
//  force the above functions to match only files that
//  match all the criteria (ie: use Regular Expressions
//  to find all files with the extension .lwo and the
//  header FORM????LWLO).  This allows you to match
//  an extension, header and/or Type with any of
//  the Find functions.
//
// Note that if the path, pattern or full path is changed,
//  any currently running search (ie:  FindFirst() or
//  FindNext() have been called), the search is closed and
//  reset.
//  

#pragma once

#include <stdlib.h>

#ifdef WIN32                // Windows-specific includes
#include <wtypes.h>
#include <winbase.h>
#endif

#include "DirStrings.h"

class DirInfo;

// Match/No Match return values
#define DI_NO_MATCH           -1
#define DI_MATCHED             0

// "File" Type Flags
#define DI_TYPE_DRIVE        (1 << 0)
#define DI_TYPE_DIR          (1 << 1)
#define DI_TYPE_FILE         (1 << 2)
#define DI_TYPE_ANY          (DI_TYPE_DRIVE | DI_TYPE_DIR | DI_TYPE_FILE)

// Match Mode Flag Bits
#define DI_MODE_TYPE         (1 << 0)
#define DI_MODE_EXT          (1 << 1)
#define DI_MODE_HEADER       (1 << 2)
#define DI_MODE_ATTRIB       (1 << 3)
#define DI_MODE_ALL          (DI_MODE_TYPE | DI_MODE_EXT | DI_MODE_HEADER )

// Existance Test Modes
#define DI_EXIST_ONLY        00
#define DI_EXIST_WRITE       02
#define DI_EXIST_READ        04
#define DI_EXIST_READ_WRITE  06

// Header Settings
#define MAX_HEADER_LENGTH    20

// Delete Return Codes
#define DI_DELETE_OK                  0
#define DI_DELETE_READ_ONLY           1
#define DI_DELETE_WRITE_PROTECTED     1
#define DI_DELETE_FILE_NOT_FOUND      2
#define DI_DELETE_UNKNOWN_ERROR       3

// Rename Return Codes
#define DI_RENAME_OK                  0
#define DI_RENAME_READ_ONLY           1
#define DI_RENAME_WRITE_PROTECTED     1
#define DI_RENAME_FILE_NOT_FOUND      2
#define DI_RENAME_FILE_ALREADY_EXISTS 3
#define DI_RENAME_INVALID_CHARS       4
#define DI_RENAME_UNKNOWN_ERROR       5

class DirInfo {
public:
  DirInfo( const char *_fullpath = NULL );
  ~DirInfo();

  // Directory Control
  bool Refresh();                        // Refresh the directory.
  bool GoBack();                         // Go back one directory.  Returns false if we're already at the root.
  bool MakePathAbsolute();               // Convert the path for this DirInfo instance into an absolute path, including drive name.  Returns false if path doesn't exist

  bool ChangeDir(       const char *new_dir);         // Change the directory we're in
  bool ChangePath(      const char *new_dir);         // Change the directory we're in (synonym for ChangeDir())
  bool ChangeDirectory( const char *new_dir);         // Change the directory we're in (synonym for ChangeDir())
  bool ChangeFullPath(  const char *new_path);        // Change the directory we're in and the pattern we're looking for
  bool ChangePattern(   const char *new_pattern );    // Change the pattern to match against

  // Search Settings
  int          SetMatchType( int state );             // Set the match type.  Returns the last match type
  int          GetMatchType();                        // Get the match type
  bool         SetHeaderList( const char **list );    // Set the header list
  const char * GetHeaderList();                       // Get the header list
  bool         SetExtensionList( const char **list ); // Set the extension list
  const char * GetExtensionList();                    // Get the extension list

  // Search Routines
  const char *FindFirstWildcardMatch();  // Find the first file that matches the pattern (platform wildcards)
  const char *FindNextWildcardMatch();   // Find the next file that matches the pattern (platform wildcards)

  const char *FindFirstRegExMatch();     // Find the first file that matches the pattern (regular expressions)
  const char *FindNextRegExMatch();      // Find the next file that matches the pattern (regular expressions)

  const char *FindFirstExtensionMatch(); // Find the first file with the matching an entry in the extension list
  const char *FindNextExtensionMatch();  // Find the next file with the matching an entry in the extension list

  const char *FindFirstHeaderMatch();    // Find the first file with the matching an entry in the header list
  const char *FindNextHeaderMatch();     // Find the next file with the matching an entry in the header list

  const char *FindFirstMatch();          // Find the first file that matches all the criteria
  const char *FindNextMatch();           // Find the next file that matches all the criteria

  const char *FindFirst();               // Find the first file in the dir (no pattern matching)
  const char *FindNext();                // Find the next file in the dir (no pattern matching)

  // String Accessors
  const char *GetLastMatch();            // Return a pointer to the last successful file matched.
  const char *GetPath();                 // Return a pointer to the current path
  const char *GetPattern();              // Returns a pointer to the current pattern
  const char *GetFullPath();             // Returns a pointer to the full path (path with pattern)

  // Test on the Last Matched File
  inline bool IsDrive();                 // Returns true if the last matched file is a drive
  inline bool IsDir();                   // Returns true if the last matched file is a dir or drive
  inline bool IsFile();                  // Returns true if the last matched file is a file

  int         GetType();                 // Returns the type of file (DI_DRIVE, DI_DIR or DI_FILE)
  inline const char *GetHeader() {       // Returns a pointer to the file's header
                     return header; }
  int  HasMatchingExtension();           // Returns the index into the given the ext array that matches the last matched file, or DI_NO_MATCH if not matched
  int  HasMatchingHeader();              // Returns the index into the given the header array that matches the last matched file, or DI_NO_MATCH if not matched
  int  HasMatchingType();                // Returns a flag representing the matching "file" type (drive, dir or file).

  bool HasHeader( const char *test );    // Returns true if the header matches the given test

  // File Attributes
  int  GetAttributes();                  // Get the attributes of the last matched file
  int  GetProtectionBits();              // Synonym for GetAttributes()
  int  SetAttributes( int attrib );      // Set the attributes of the last matched file.  Returns the original protection bits.
  int  SetProtectionBits( int attrib );  // Synonym for SetAttributes()

  // File Attribute Tests
  bool IsCompressed();                   //  Win32
  bool IsTemporary();                    //  Win32
  bool IsNormal();                       //  Win32;  no other attributes set
  bool IsOffline();                      //  Win32
  bool IsReadOnly();                     //  Win32
  bool IsSystem();                       //  Win32
  bool IsArchived();                     //  Win32/Amiga
  bool IsHidden();                       //  Win32/Amiga
  bool IsRead();                         //  Amiga
  bool IsWrite();                        //  Amiga
  bool IsExecutable();                   //  Amiga
  bool IsDeletable();                    //  Amiga
  bool IsScript();                       //  Amiga
  bool IsPure();                         //  Amiga

  // File Attribute Set Functions
  bool SetCompressed();                  //  Win32
  bool SetTemporary();                   //  Win32
  bool SetNormal();                      //  Win32;  no other attributes set
  bool SetOffline();                     //  Win32
  bool SetReadOnly();                    //  Win32
  bool SetSystem();                      //  Win32
  bool SetArchived();                    //  Win32/Amiga
  bool SetHidden();                      //  Win32/Amiga
  bool SetRead();                        //  Amiga
  bool SetWrite();                       //  Amiga
  bool SetExecutable();                  //  Amiga
  bool SetDeletable();                   //  Amiga
  bool SetScript();                      //  Amiga
  bool SetPure();                        //  Amiga

  // File Information
  long GetFileSize();                    // Returns the file size in bytes (up to 4 gigs; equal to FileSizeLow() )
  long GetFileSizeHigh();                // Win32:  Returns the High File Size (second DWORD; only non-zero if Low File Size is > MAXDWORD)
  long GetFileSizeLow();                 // Win32:  Returns the Low File Size (first DWORD).  

  long GetCreationTime();                // Get the time the file was created
  long GetAccessTime();                  // Win32: Get the last access time
  long GetWriteTime();                   // Get last modified time
  long GetModifiedTime();                // Synonym for GetWriteTime()

  const char *GetDOSFilename();          // Win32; returns a pointer to the 8.3 filename

  // Settings
  bool SetHeaderState( bool state );     // Set if the file haeder should be stored.  If true, this results in improved performance when you just want a dir listing

  // Static Functions
  static bool Exists(   const char *path, int mode = DI_EXIST_ONLY );  // Returns true if the given path exists
  static bool IsDir(    const char *path );                            // Returns true if the path points to a directory
  static bool Rename(   const char *path );                            // Renames the specified file
  static bool MakeDir(  const char *path );                            // Make a new directory.  Returns true on success
  static bool MakeTree( const char *path, bool has_file = false);      // Makes an entire tree of directories if any of those directories dosn't exist yet.
  static bool MakePathAbsolute( char *path );                          // Convert the given path into an absolute path, including drive name.  Returns false if the path doesn't exist.

#ifdef WIN32
  static bool HasBeenWrittenAfter( const char *path, FILETIME *comp_time );  // Returns true if the the file has been written after the time provided
#endif

  static bool ChangeWorkingDir( char *path, bool change_drive = false );     // Change the Current Working Directory (Win32)
  static bool ChangeWorkingDrive( char *path );                              // Change the Current Working Drive (Win32)
  
  static int  Delete(   const char *file );    // Deletes a file from disk
  static int  DelTree(  const char *path );    // Deletes an entire directory tree from disk
  static int  Rename(   const char *file, const char *name ); // Renames a file

  // Free Resource Functions
  bool CloseSearch();                               // System-specific post-search clean-up

protected:
  bool GetFileData();                               // Retrive data from the OS-specific structures into our own structures
  bool TestRegularExpression( const char * test );  // Test the input string against the patter as a expression agains

  char path[MAX_DIRECTORY_LENGTH];       // Path of the directory we're looking in
  char pattern[MAX_FILENAME_LENGTH];     // Pattern we're trying to match against.
  char full_path[MAX_PATH_LENGTH];       // The path with the pattern appeneded to it.  May include drive, too.

  const char **extension_list;           // List of extensions to match against
  const char **header_list;              // List of headers to match against
  long         match_attributes;         // Attributes that must be in the file for it to be matched
  long         skip_attributes;          // Attributes that must not be in the file for it to be matched

  int   mode_flags;                      // Pattern match mode flags (extension, header, type)
  int   type_flags;                      // Pattern match type flags (file, dir, drive)

  bool  get_header;                      // Do we get a header each time we load the file?

  char  last_match[MAX_PATH_LENGTH];     // The last file that was matched by any of the Find functions
  long  attributes;                      // Protection bits of the last matched file
  int   file_type;                       // Type of "file" (drive, file or directory)
  char  header[MAX_HEADER_LENGTH];       // Header of the last file matched.

#ifdef WIN32                           // -- Windows-Specific Data
  HANDLE             find_handle;        // Search handle used by Windows
  LPWIN32_FIND_DATA  find_data;          // Data returned by the Window's FindFirstFile() and FindNextFile()

  FILETIME           creation_time;      // Creation Time
  FILETIME           last_access_time;   // Last Access Time
  FILETIME           last_write_time;    // Last Write Time
#endif

#ifdef AMIGA                           // -- Amiga-Specific Data

#endif

};


// ---
// ------------------------------------------- Inline Accessors ----------
// ---

// IsDrive():
//  Returns true if the last mached file is a drive
inline bool DirInfo::IsDrive() {
  return( file_type == DI_TYPE_DRIVE ? true : false );
}

// IsDir():
//  Returns true if the last mached file is a dir
//   or a drive
inline bool DirInfo::IsDir() {
  return( file_type == DI_TYPE_DIR ? true : false );
}

// IsFile():
//  Returns true if the last mached file is a file
inline bool DirInfo::IsFile() {
  return( file_type == DI_TYPE_FILE ? true : false );
}
