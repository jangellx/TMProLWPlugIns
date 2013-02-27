//
// DirStrings.h
//
// This file contains the DirStrings class, which contains
//  a number of useful functions for handling filenames
//  and paths.  This includes changing the paths, extensions
//  and other filename matinance.
//
// All functions of DirStrings are static, and thus can be called
//  without instancing the class.
//
// Note that all the functions that require an output buffer
//  expect the buffer to be large enough to hold the full path
//  (MAX_PATH_LENGTH chars or larger).
//

#pragma once

class DirStrings;

#define MAX_DIRECTORY_LENGTH 256
#define MAX_FILENAME_LENGTH  256
#define MAX_PATH_LENGTH      (MAX_DIRECTORY_LENGTH + MAX_FILENAME_LENGTH)

#define DT_STRIP_ALL          -1


// Platform-Specific Filename Strings
#ifdef AMIGA
  static const char   DRIVE_SEPERATOR_CHAR   = ':';
  static const char   DIR_SEPERATOR_CHAR     = '/';
  static const char * DIR_SEPERATOR_STRING   = "/";
#endif

#ifdef WIN32
  static const char * DRIVE_SEPERATOR_STRING = ":\\";
  static const char   DIR_SEPERATOR_CHAR     = '\\';
  static const char * DIR_SEPERATOR_STRING   = "\\";
#endif

#ifdef MAC
  // I don't know what Macs use...
#endif


class DirStrings {
public:
  // String Manipulation Tools
  static char * StripPath(       char *output, int levels = DT_STRIP_ALL );  // Remove the directory X levels back from the path
  static char * ChangePath(      char *output, const char *new_dir       );  // Change the directory and drive
  static char * ChangeDir(       char *output, const char *new_dir       );  // Change the directory and drive.  Synonym for ChangePath().
  static char * ChangeDirectory( char *output, const char *new_dir       );  // Change the directory and drive.  Synonym for ChangePath().
  static char * ChangeDrive(     char *output, const char *new_drive     );  // Change the drive (ie: DH0:, C:\, etc.
  static char * ChangeFile(      char *output, const char *new_file      );  // Change the file part of the path, including any existing extension
  static char * ChangeFileOnly(  char *output, const char *new_file      );  // Change the file part of the path, but keep any extension that may already exist
  static char * ChangeExtension( char *output, const char *new_ext       );  // Change the extension

  static char * AddPathPart(     char *output, const char *new_part );    // Add a new part to the path (dir or file)
  static char * GetPathPart(     char *output, const char *path = 0 );    // Returns a pointer to the path part of the input string (ie: everything but the filename)
  static char * GetFilePart(     char *output, const char *path = 0 );    // Returns a pointer to the file part of the input string (ie: the filename with extension)
  static char * GetFilePartOnly( char *output, const char *path = 0 );    // Returns a pointer to the file part of the input string, but without the extension (ie: the filename without the extension)
  static char * GetExtension(    char *output, const char *path = 0 );    // Returns a pointer to the extension part of the input string (ie: after the last . in the file part)
  static char * GetDrivePart(    char *output, const char *path = 0 );    // Returns a pointer to the drive part of the input string

  static char * AddDirSeperator( char *path );                               // Adds a directory seperator (ie: a '/' or '\') to the end of path if one isn't already there
  static char * RemoveTrailingDirSeperator( char *path, bool any = false );  // Removes a trailing directory seperator (ie: a '/' or '\' if not preceded by ':') from the end of path if present

  static char * MakeWinPath(   char *output, const char *path = 0 );      // Convert the path into a Windows-style path (drives end in :\ and are 1 character; dirs marked with \)
  static char * MakeAmigaPath( char *output, const char *path = 0 );      // Convert the path into an Amiga/Unix-style path ( drives end in :; dirs end in /)
  static char * MakeUnixPath(  char *output, const char *path = 0 );      // Convert the path into an Amiga/Unix-style path ( drives end in :; dirs end in /)
  static char * MakeMacPath(   char *output, const char *path = 0 );      // Convert the path into a Macintosh-style path   ( ??? )

  static bool IsDriveOnly(     const char *path );                        // Returns true if the input path is only a drive (ie: ends in : or \:)
  static bool IsPathOnly(      const char *path );                        // Returns true if the input path has no file part (ie:  ends in /, \ or :)
  static bool IsFileOnly(      const char *path );                        // Returns true if the input path contains only a filename (ie:  no /, \ or :)
  static bool IsExtensionOnly( const char *path );                        // Returns true if the input path contains only an extension (ie:  starts with a ., but isn't a ../)
  static bool IsFullPath(      const char *path );                        // Returns true if the input path is a full, complete path (has a :)
  static bool IsNone(          const char *path );                        // Returns true if the input path is "(none)"
  static bool IsEmpty(         const char *path );                        // Returns true if the input path is ""

  static bool   HasDrive(      const char *path );                        // Returns true if the input path has a drive part( ie:  contains a :)
  static bool   HasPath(       const char *path );                        // Returns true if the input path has a path part (ie: contains a /, \ or :)
  static char * HasFile(       const char *path );                        // Returns a pointer to the file part if the input path has a file part (ie: doesn't end in /, \ or :)
  static char * HasExtension(  const char *path );                        // Returns a pointer to the extension part if the input path has an extension (ie: the file part has a . in it)
  static char * HasWildcards(  const char *path );                        // Returns a pointer to the first wildcard in the path, or NULL if there aren't any
  static char * HasWhiteSpace( const char *path );                        // Returns a pointer to the first space in the path, or NULL if there aren't any

  static char * NextPathPart(     const char *path );                      // Returns the next part of the path

  static const char * HasIllegalChars( const char *path, bool is_file = false );                 // Returns a pointer to the first illegal characters in the path, or NULL if there aren't any.  If is_file is true, dir characters are also considered illegal
  static       char * RemoveIllegalChars( char *path, char replacement, bool is_file = false );  // Finds all illegal characters in the filename provided and replaces them the replacement char
};
