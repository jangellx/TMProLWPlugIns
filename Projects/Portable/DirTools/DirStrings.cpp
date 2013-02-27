//
// DirStrings.cpp
//
// This file contains the implentation of the DirStrings class.
//

#include <string.h>
#include "DirStrings.h"

// StripPath():
//  Strips the path off of a file.  This can do one of two things:
//  - if levels == DT_STRIP_ALL, then only the file part will be
//    returned.  This is effectively the same as GetFilePart
//  - if levels != DT_STRIP_ALL, then that number of directoires
//    is removed from the path, going from right to left,
//    effectively going backward that many directories.  The last
//    item on the line is considered to be a directory, even if
//    it really is a file.  Stripping can result in an empty output
//    buffer, since it will also remove the root directory (:) if
//    found.
char * DirStrings::StripPath( char *output, int levels ) {
  if( levels == DT_STRIP_ALL )
    return GetPathPart( output );
  
  for( int j = strlen( output )-1; (j >= 0) && (levels > 0); j-- ) {
    if( (output[j] == '\\') || (output[j] == '/' ) ) {  // Look for a / or \ and remove it
      output[j] = '\0';
      levels--;
    }

    if( output[j] == ':' ) {                            // Found a :; clear out the whole path
      output[0] = '\0';
      levels = 0;
    }
  }

  if( levels > 0 )
    output[0] = '\0';

  return output;
}

// ChangePath():
//  Changes the path of the input path.  If a path
//  already exists, it is removed.  In either event, the
//  new path will be at the begining of the buffer.
//  Note that this assumes that the new path is indeed a
//  valid path, and will not add a :, /, \  or :\ to it's
//  end.  Note also that the drive part is considered to
//  be part of the path, and will be replaced along with
//  the rest of the path.
char * DirStrings::ChangePath( char *output, const char *new_path ) {
  if( !new_path )
    return output;

  char buffer[ MAX_PATH_LENGTH ];

  GetFilePart( buffer, output );
  strcpy( output, new_path );

  if( !IsEmpty( buffer ) )
    strcat( output, buffer );

  return output;
}

// ChangeDir():
//  Synonym for ChangePath()
char * DirStrings::ChangeDir( char *output, const char *new_path ) {
  return ChangePath( output, new_path );
}

// ChangeDirectory():
//  Synonym for ChangePath()
char * DirStrings::ChangeDirectory( char *output, const char *new_path ) {
  return ChangePath( output, new_path );
}


// ChangeDrive():
//  Changes the drive of the input path.  If a drive
//   already exists, it is removed.  In either event, the
//   new drive will be at the begining of the buffer.
//  Note that this assumes that the new drive is indeed a
//   valid drive, and will not add a : or :\ to the end.
//  Note that the Windows UNC \\ is also considered a drive.
char * DirStrings::ChangeDrive( char *output, const char *new_drive ) {
  if( !new_drive )
    return output;

  char buffer[ MAX_PATH_LENGTH ];
  char *after_drive;

  if( (output[0] == '\\') && (output[1] == '\\') ) {
    // Check for a leading \\ (UNC style)
    after_drive = &(output[1]);
  } else {
    // Check for a : or :\ drive
    after_drive = strrchr( output, ':' );
    if( after_drive ) {
      if( after_drive[1] == '\\' )
        after_drive = &( after_drive[1] );
    } else {
      after_drive = output;
    }
  }

  strcpy( buffer, new_drive );
  strcat( buffer, after_drive );
  strcpy( buffer, output );

  return output;
}

// ChangeFile():
//  Changes the file of the input path.  If a file part
//  already exists, it is removed.  In either event, the new
//  file part will be at the end of the buffer.  Note that
//  the file part is assumed to include an extension, and any
//  old extension and/or file part will be removed.
char * DirStrings::ChangeFile( char *output, const char *new_file ) {
  if( !new_file )
    return output;

  char * old_file = HasFile( output );   // Eliminate any existing extensions
  if( old_file )
    old_file[0] = '\0';

  strcat( output, new_file );
  return output;
}

// ChangeFileOnly():
//  Changes the file of the input path.  If a file part
//  already exists, it is removed.  In either event, the new
//  file part will be at the end of the buffer.  Note that
//  the only the file part is changed.  If there is an extension
//  in the original file part, it will be retained.  The new
//  file part is assumed to be just a file part, and is not
//  modified or checked in any wau.
char * DirStrings::ChangeFileOnly( char *output, const char *new_file ) {
  if( !new_file )
    return output;

  char * old_file = HasFile( output );   // Eliminate any existing file part...
  char * old_ext  = 0;
  char buffer[MAX_FILENAME_LENGTH];
  if( old_file ) {
    old_ext = HasExtension( old_file );  // ...but keep the extension
    if( old_ext ) {
      strcpy( buffer, old_ext );
      old_file[0] = '\0';
    }
  }

  strcat( output, new_file );
  if( old_ext )
    strcpy( output, buffer );

  return output;
}

// ChangeExtension():
//  Changes the extension of the input path.  If an extentions
//  already exists, it is removed.  In either event, the new
//  extension will be at the end of the buffer.  Note that
//  this assumes that the new extensions starts with a '.',
//  and no '.' will be added by this function to the extension.
char * DirStrings::ChangeExtension( char *output, const char *new_ext ) {
  if( !new_ext )
    return output;

  char * old_ext = HasExtension( output );   // Eliminate any existing extensions
  if( old_ext )
    old_ext[0] = '\0';

  strcat( output, new_ext );
  return output;
}

// AddPathPart():
//  Adds the new path to the end of the output path.  Directory
//   slashes will be added as needed.  If the new path contains
//   leading /'s (forward slashes) or ..\'s (dot-dot-backward slash),
//   one dir level will be stripped off of the output path, if they
//   exist.  If the input put contains a drive designation, then it
//   will replace the ouput path.  Note that the Windows UNC \\ is
//   considered a drive designator.
char * DirStrings::AddPathPart( char *output, const char *new_part ) {
  if( strchr( new_part, ':' ) ) {                // Test for full path
    strcpy( output, new_part );
    return output;
  }

  int len = strlen( new_part );
  if( len == 0 )
    return output;

  // Check for a full path (replace existing)
  if( strchr( new_part, ':' ) ) {                // New Part has a :, so it must be a full path.
    strcpy( output, new_part );
    return output;
  }

#ifdef WIN32
  if( (new_part[0] == '\\') && (new_part[1] == '\\') ) {
    strcpy( output, new_part );
    return output;
  }
#endif

  // Handle partial paths (add to existing)
  int   first_char = 0;                          // Handle / and ..\ (go back X directories)
  int   strip_count = 0;
  bool  keep_searching = false;
  len = strlen( new_part );

  while( first_char < len ) {
    keep_searching = false;
    if( new_part[ first_char ] == '/' ) {                        // Find a /
      first_char++;
      keep_searching = true;
      strip_count++;
    }
    
    if( strncmp( &new_part[ first_char ], "..\\", 3 ) == 0 ) {   // Find a ..\ 
      first_char += 3;
      keep_searching = true;
      strip_count++;
    }

    if( !keep_searching )                                   // No more go-backs; stop
      break;
  }

  if( strip_count > 0 )
    StripPath( output, strip_count );

  int outlen = strlen(output);
  if( outlen > 0 ) {                                        // Add a trailing / or \ if needed
    if( (output[ outlen-1 ] != '/'  ) &&
        (output[ outlen-1 ] != '\\' ) )
#ifdef WIN32
      strcat( output, "\\");
#endif
#ifdef AMIGA
      strcat( output, "/");    
#endif
  }
  strcat( output, new_part );
  return output;
}

// GetPathPart():
//  Fills in the output pointer with path part of the input
//  string.  Both forward and backward slashes are
//  considered directory slashes.  For example:
//
//   Input                Output (Win32)    Output (Amiga)
//  c:                   c:\               c:
//  c:\file              c:\               c:
//  c:/file              c:\               c:
//  c:\dir\file          c:\dir            c:dir
//  c:\dir/dir\file      c:\dir\dir        c:dir/dir
//  c:\dir\              c:\dir            c:dir
char * DirStrings::GetPathPart( char *output, const char *path ) {
#ifdef WIN32
  // Windows-specific directory routine
  if( path )
    strcpy( output, path );

  char *slash = strrchr( output, '/' );          // Look for a slash
  if( !slash )                                   // Didn't find a /, so try a \ 
    slash = strrchr( output, '\\' );

  if( slash ) {                                  // Found it.  This must be the end of that path, so return it.
    if( slash != output ) {
      if( slash[-1] == ':' )                     // Handle a dir :, which should be followed by a \ 
        slash[1] = '\0';
      else
        slash[0] = '\0';                         // Remove everything past the slash, including the slash
      }
    return output;
  }

  char * colon = strchr( output, ':' );          // No slashes; look for a colon
  if( colon ) {                                  // Found it; return everything after the :
    colon[1] = '\0';
    return output;
  }

  output[0] = '\0';                              // No dir data found, so this must only be a file.  Return an empty string
#endif

#ifdef AMIGA
  // Insert Amiga-specific routine here
#endif

  return output;
}

// GetFilePart():
//  Returns the file part of the path.  This is everything after
//  the last / or \, or after the last : if there isn't a directory
//  slash.  If there are no slashes or colons, the entire buffer is
//  the path.  If the buffer ends in  a slash or colon, there is no
//  file part, and an empty buffer is returned.
char * DirStrings::GetFilePart(  char *output, const char *path ) {
  if( !path )
    path = output;

  char *filepart = HasFile( path );
  if( !filepart ) {                             // No file part; return an empty string
    output[0] = '\0';
    return output;
  }
  
  strcpy( output, filepart );
  return output;
}

// GetFilePartOnly():
//  Returns the file part of the path without the extension.  This is
//  everything after the last / or \, or after the last : if there
//  isn't a directory slash, and everything before the last '.'.  If
//  there are no slashes, colons or dots, the entire buffer is
//  the file part.  If the buffer ends in  a slash or colon, there is no
//  file part, and an empty buffer is returned.
char * DirStrings::GetFilePartOnly(  char *output, const char *path ) {
  if( !path )
    path = output;

  char *filepart = HasFile( path );
  if( !filepart ) {                             // No file part; return an empty string
    output[0] = '\0';
    return output;
  }

  char *ext = HasExtension( output );           // Strip the extension, if any
  if( ext )
    ext[0] = '\0';

  strcpy( output, filepart );
  return output;
}

// GetExtension():
//  Returns the extension part of the path, which is the part of
//  the file part after the last '.'.  If there is no extension,
//  or no file part, the output will be an empty string.
char * DirStrings::GetExtension( char *output, const char *path ) {
  if( !path )
    path = output;

  char *file = HasFile( path );
  if( file == NULL ) {                            // No file part; return an empty string
    output[0] = '\0';
    return output;
  }

  char * dot = strrchr(path, '.');
  if( !dot ) {                                    // No extension; return an empty string
    output[0] = '\0';
    return output;
  }

  if( dot == path )                               // Entire output is extension
    if( path != output ) {
      strcpy( output, path );

    return output;
  }

  strcpy( output, dot );                          // Extension is part of file part; copy the extension into the output buffer
  return output;
}

// GetDrivePart():
//  Returns the drive part of the path, including the trailing
//   : and \.  An empty buffer is returned if there is no drive
//   in part.  Windows UNC \\Name is considered a drive designator,
//   sans the trailing \.
char * DirStrings::GetDrivePart( char *output, const char *path ) {
  if( !path )
    path = output;

  // Test for a drive designator (:)
  char *colon = strchr( path, ':' );
  if( colon ) {
    if( path != output )
      strncpy( output, path, colon - path + 1 );

    output[colon - path + 1] = '\0';
#ifdef WIN32
      strcat( output, "\\" );                               // Add a slash after the colon for Windows
  } else if( (path[0] == '\\') && (path[1] == '\\') ) {
    // Test for a UNC path
    char * slash1 = strchr( &(path[2]), '\\' );
    char * slash2 = strchr( &(path[2]), '/' );
    char * slash;

    if( (slash1 == NULL) && (slash2 != NULL) )
      slash = slash2;                                       // Backward Slash Found
    else if( (slash2 == NULL) && (slash1 != NULL) )
      slash = slash1;                                       // Forward Slash Found
    else if( (slash1 == NULL) && (slash2 == NULL) )
      slash = NULL;                                         // No slashes found; must be only a UNC path base
    else
      slash  = (slash1 < slash2) ? slash1 : slash2;         // Both slashes found; use the first

    if( slash != NULL ) {
      // Find the next part of the path (ie: include the "drive" part)
      char *next = NextPathPart( slash );
      if( next != NULL )
        slash = next + 1;

      if( path != output )
        strncpy( output, path, slash - path + 1 );

      output[slash - path + 1] = '\0';
    }
#endif
  } else {
    // No drive in path; return an empty buffer
    output[0] = '\0';
  }

  return output;
}

// AddDirSeperator():
//  Adds a directory seperator (ie: a '/' or '\') to the end of path if
//   one isn't already there.  Returns a pointer to the same path.
char * DirStrings::AddDirSeperator( char *path ) {
  int len = strlen( path );

#ifdef WIN32
  if( path[len-1] != '\\' )
    strcat( path, "\\" );
#else
  if( path[len-1] != '/' )
    strcat( path, "/" );
#endif

  return path;    
}

// RemoveTrailingDirSeperator():
//  Removes a trailing directory seperator (ie: a '/' or '\' if not
//   preceded by a ':') from the end of path if present.  Returns a
//   pointer to the same path.  If any is true, then the slash will
//   be removed even if preceded by a :.
char * DirStrings::RemoveTrailingDirSeperator( char *path, bool any ) {
  int len = strlen( path );

#ifdef WIN32
  if( path[len-1] == '\\' ) {
    if( any || (path[len-2] != ':') )
      path[len-1] = '\0';
  }
#else
  if( path[len-1] == '/' )
    path[len-1] = '\0' );
#endif

  return path;    
}

// IsDriveOnly():
//  Returns true if the last character is a :,  the last two characters are :\ 
//   or the first two chars are a \\ with nothing after the first (if any) slash
//   afterward (UNC path).
bool DirStrings::IsDriveOnly( const char *path ) {
  int end = strlen( path )-1;

  if( end > 0 ) {
    if( path[end] == ':' )         // Ends in a :, return true
      return true;
    else {
      if( end > 1 ) {              // Ends in a :\, return true
        if( strcmp( &(path[end-1]), ":\\" ) )
          return true;
      }
    }
  }

#ifdef WIN32
  // UNC test
  if( (path[0] == '\\') || (path[1] == '\\') ) {
    char * slash1 = strchr( &(path[2]), '\\' );
    char * slash2 = strchr( &(path[2]), '/' );
    char * slash;

    if( (slash1 == NULL) && (slash2 != NULL) )
      slash = slash1;                                       // Backward Slash Found
    else if( (slash2 == NULL) && (slash1 != NULL) )
      slash = slash2;                                       // Forward Slash Found
    else if( (slash1 == NULL) && (slash2 == NULL) )
      slash = NULL;                                         // No slashes found; must be only a UNC path base
    else
      slash  = (slash1 < slash2) ? slash1 : slash2;         // Both slashes found; use the first

    // See if any other slashes were found
    if( slash == NULL )
      return true;

    // See if there is anything beyond the slash
    if( slash[1] = '\0' )
      return true;
  }
#endif;

  return false;                    // No match; return false
}

// IsPathOnly():
//  Returns true if the last character is a :, / or \ 
bool DirStrings::IsPathOnly( const char *path ) {
  int end = strlen( path )-1;
  if( (path[end] == ':' ) ||  
      (path[end] == '/' ) ||
      (path[end] == '\\') )
    return true;

  return false;
}

// IsFileOnly():
//  Returns true if there is no :, / or \ 
bool DirStrings::IsFileOnly( const char *path ) {
  if( strchr( path, ':'  ) ||
      strchr( path, '\\' ) ||
      strchr( path, '//' ) )
    return false;

  return true;
}

// IsExtensionOnly():
//  Returns true if there is only one . at the begining of
//  of the buffer
bool DirStrings::IsExtensionOnly( const char *path ) {
  if( path[0] == '.' )
    if( !strchr( &(path[1]), '.'  ) )
      return true;

  return false;
}

// IsFullPath():
//  Returns true if there is a : and in the path
bool DirStrings::IsFullPath( const char *path ) {
  if( strchr( path, ':' ) )
    return true;

  return false;
}

// IsNone():
//  Returns true if the path is "(none)"
bool DirStrings::IsNone( const char *path ) {
  if( stricmp( path, "(none)" ) )
    return true;

  return false;
}

// IsEmpty():
//  Returns true if the path is ""
bool DirStrings::IsEmpty( const char *path ) {
  if( path[0] == '\0' )
    return true;

  return false;
}

// HasDrive():
//  Returns true if the path has a :, or if it starts
//   with a UNC-style //
bool DirStrings::HasDrive( const char *path ) {
  if( strchr( path, ':' ) )
    return true;

#ifdef WIN32
  // UNC test
  if( (path[0] == '\\') && (path[1] == '\\') )
    return true;
#endif

  return false;
}

// HasFile():
//  Returns true if the path doesn't end in /, \ or :
char * DirStrings::HasFile( const char *path ) {
  if( (IsPathOnly( path ) ) )
    return (char *)false;

  const char *filepart;
  for( int end = strlen(path) - 1; end >= 0; end-- ) {  // Look for a slash or :
    if( (path[end] == '\\' ) ||
        (path[end] == '/'  ) ||
        (path[end] == ':'  ) ) {
      filepart = &( path[end+1] );
      break;
    }
  }

  if( end < 0 )                                         // Entire path is file part; return pointer to everything
    return (char *)path;  

  return (char *)filepart;
}

// HasPath():
//  Returns true if the path has a /, \ or :
bool DirStrings::HasPath( const char *path ) {
  if( strchr( path, '\\' ) || 
      strchr( path, '/'  ) ||
      strchr( path, ':'  ) ) {
    return true;
  } else {
    return false;
  }
}

// NextPathPart():
//  Returns a pointer to the next part of the path.  Returns NULL if there
//   are no more matches.
char * DirStrings::NextPathPart( const char *path ) {
  char *next = strchr( path, ':' );
  if( next != NULL ) {
    #ifdef WIN32
      if( next[1] == '\\' )     // Test for a trailing backslash after the colon
        return next + 2;
    #endif

    return next + 1;
  }

  next = strchr( path, '/' );
  char * next_backslash = strchr( path, '\\' );

  // If next is NULL, use backslash.  If backslash is NULL, use next.  Otherwise, see if backslash or next is lower
  next = (next == NULL) ? next_backslash :
         ((next_backslash == NULL) ? next :
         ((next < next_backslash) ? next : next_backslash) );

  if( next == NULL )
    return NULL;

  #ifdef WIN32
    // UNC test
    if( (next[0] == '\\') && (path[1] == '\\') )
      return next + 1;
  #endif

  return next + 1;
}

// HasExtension():
//  Returns a pointer to the extension if the path has
//  a '.' after the last /, \ or :.  The pointer points
//  to the '.' itself.
char * DirStrings::HasExtension( const char *path ) {
  int found_slash = -1;

  char *dot = strrchr( path, '.' );
  if( !dot )
    return false;

  for( int end = strlen( path )-1; end >= 0; end-- ) {
    if( (path[end] == '\\') ||
        (path[end] == '/' ) ||
        (path[end] == ':' ) ) {
      break; 
    }
  }

  if( &(path[end]) < dot )
    return dot;

  return false;
}

// HasWildcards():
//  Checks the path for any platform-specific wildcards.  Returns
//  a pointer to the first wildcard found.
//
char * DirStrings::HasWildcards( const char *path ) {
#ifdef WIN32
  char *wildcard = strchr( path, '*' );     // Test for *
  if( wildcard )
    return wildcard;

  wildcard = strchr( path, '?' );           // Test for ?
  if( wildcard )
    return wildcard;

  return false;
#endif

#ifdef AMIGA
  // Insert Amiga-Specific Implementation Here
#endif
}

// HasIllegalChars():
//  Checks the path for any platform-specific illegal characters.  Returns
//   a pointer to the first illegal character found, or NULL if all characters
//   are legal.  If is_file == true, then directory slashes and colons are also
//   considered illegal.
const char * DirStrings::HasIllegalChars( const char *path, bool is_file ) {
#ifdef WIN32
  char file_illegal_chars[]   = { '/', '\\', ':',                 '\0' };
  char common_illegal_chars[] = { '*', '?', '\"', '<', '>', '|',  '\0' };

  char len = strlen( path );
  int h, i;
  for( h=0; h < len; h++ ) {
    // Common Checks
    for( i=0; common_illegal_chars[i] != '\0'; i++ ) {
      if( path[h] == common_illegal_chars[i] )
        return &(path[h]);
    }

    // File-specific checks
    if( is_file ) {
      for( i=0; file_illegal_chars[i] != '\0'; i++ ) {
        if( path[h] == file_illegal_chars[i] )
          return &(path[h]);
      }
    }
  }

  return NULL;
#endif

#ifdef AMIGA
  // Insert Amiga-Specific Implementation Here
#endif
}

// RemoveIllegalChars():
//  Finds all illegal characters in the filename provided and
//   replaces them the replacement char.  If is_file == true,
//   then directory slashes and colons are also considered illegal
//   and will be replaced.  Returns a pointer to the path.
char * DirStrings::RemoveIllegalChars( char *path, char replacement, bool is_file ) {
  char *match = path;
  while( true ) {
    match = (char *)HasIllegalChars( match, is_file );
    if( match == NULL )
      break;

    match[0] = replacement;
    match++;
  }

  return path;
}

// HasWhiteSpace():
//  Checks the path for any white space.  Returns
//  a pointer to the first space or tab found.
//
char * DirStrings::HasWhiteSpace( const char *path ) {
  char *space;

  if( (space = strchr( path, ' ' ) ) )  return space;  // Test for a space
  if( (space = strchr( path,  9  ) ) )  return space;  // Test for a tab
  if( (space = strchr( path, '\n') ) )  return space;  // Test for a lf
  if( (space = strchr( path, '\r') ) )  return space;  // Test for a cr

  return false;
}


// MakeWinPath():
//  Converts the provided path into a Windows-compatable path
//  (makes all /'s int \'s; adds \'s after :'s). Note that this
//  currently doesn't convert "//" into "\..\"), nor convert
//  a leading / into a ..\ 
char * DirStrings::MakeWinPath( char *output, const char *path ) {
  char buffer[MAX_PATH_LENGTH];

  if( path )
    strcpy( buffer, path );
  else
    strcpy( buffer, output );

  int len = strlen( buffer );
  int o_index = 0;
  for( int i=0; i < len; i++ ) {
    switch(buffer[i]) {
    case '/':                         // Change /'s into \'s
      output[o_index] = '\\';
      break;
    case ':':
      if( buffer[i+1] != '\\' ) {    // Add a \ after a :
        output[o_index]   = ':';
        output[++o_index] = '\\';
      }
      break;
    default:                          // Output every other char
      output[o_index] = buffer[i];
    }

    o_index++; 
  }

  return output;
}
