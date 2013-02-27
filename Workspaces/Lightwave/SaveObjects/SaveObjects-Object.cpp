//
// SaveObjects-Objects.cpp
//

#include "SaveObjects-Interface.h"
#include <stdio.h>

extern const char *seperator_strings[];

// Constructor
SaveObjects_Object::SaveObjects_Object( LWItemID _id, LWObjectInfo *objectinfo, int min_revision_digits )
  : item_id(_id), full_path(NULL), filename(NULL), read_only(true), save_object(false),
    exists(false), index(-1), revision(-1), revision_string(NULL), latest_revision(-1),
    latest_revision_string(NULL), backup_exists(false), duplicate_exists(false) {

  RefreshPath( objectinfo );

  if( min_revision_digits > 0 )
    FindRevision( min_revision_digits );
}

// RefreshPath():
//  Re-reads the path of the object from Layout
void SaveObjects_Object::RefreshPath( LWObjectInfo *objectinfo ) {
  StringTools::strdup2( full_path, (*objectinfo->filename)( item_id ) );
  filename = DirStrings::HasFile( full_path );

  assert( filename != NULL );
}

// RefreshReadOnly():
//  Updates the Read-Only and Exists state of the item.
bool SaveObjects_Object::RefreshReadOnly() {
  exists = DirInfo::Exists( full_path );
  if( !exists )
    read_only = false;
  else
    read_only = (GetFileAttributes( full_path ) & FILE_ATTRIBUTE_READONLY);

  return read_only;
}

// FindRevision():
//  Finds the revision in the filename.  Returns a pointer to the start
//   of the revision, or NULL if no revision was found.
char * SaveObjects_Object::FindRevision( int min_digits ) {
  char rev_string[ 10 ];
  revision = FindRevisionInString( rev_string, filename, min_digits );
  StringTools::strdup2( revision_string, rev_string );

  return revision_string;
}

// FindRevisionInString():
//  Fills rev_string with the revision part of string, if string contains at least
//   min_digits for the revision number.  Returns the revision as an integer.
//   Returns -1 and sets rev_string to "(none)" and revsion to -1 if no revision
//   was found.
int SaveObjects_Object::FindRevisionInString( char *rev_string, const char *string, int min_digits ) {
  revision = -1;

  if( (string == NULL) || (rev_string == NULL) || (min_digits < 1) )
    return -1;

  int rev = -1;
  char buffer[ MAX_PATH_LENGTH ];

  // Determine the revision, if any
  strcpy( buffer, string );

  // Strip the extension
  char *ext = DirStrings::HasExtension( buffer );
  if( ext != NULL )
    *ext = '\0';

  // Find the revision
  int len = strlen( buffer ) - 1;
  for( int i=len; i > -1; i-- ) {
    if( !isdigit( buffer[i] ) )
      break;
  }

  // Fetch the revision, if found
  if( (len - i) >= min_digits ) {
    strcpy( rev_string, &(buffer[ i + 1 ]) );
    rev = atoi( rev_string );
  } else {
    strcpy( rev_string, "(none)" );
  }

  revision = rev;
  return revision;
}

// FindLatestRevision():
//  Returns the latest revision number found, or -1 if no revisions exist.
int SaveObjects_Object::FindLatestRevision( int min_digits, char *path ) {
  // Build the test path
  char buffer[ MAX_PATH_LENGTH ];

  // - Copy the filename and add the new path to it
  DirStrings::GetPathPart( buffer, GetFullPath() );

  if( path != NULL )
    DirStrings::AddPathPart( buffer, path );

  // - Add the file part to the buffer
  char * file_part = DirStrings::HasFile( GetFullPath() );
  assert( file_part != NULL );
  DirStrings::AddPathPart( buffer, file_part );
  DirStrings::ChangeExtension( buffer, "" );

  // Add an * where the digits should start (ie: just before the extension)
  strcpy( &(buffer[ strlen( buffer ) ]), "*" );

  // Put back on the extension
  char *ext = DirStrings::HasExtension( GetFullPath() );
  if( ext )
    strcat( buffer, ext );

  // Cycle through the path looking for all matches
  DirInfo matches( buffer );
  const char * prev_match = matches.FindFirstWildcardMatch();

  int this_rev = -1;
  latest_revision = -1;
  StringTools::strdup2( latest_revision_string, "(none)" );

  char rev_string[ 10 ];
  while( prev_match != NULL ) {
    this_rev = FindRevisionInString( rev_string, prev_match, min_digits );
    if( this_rev > latest_revision ) {
      latest_revision = this_rev;
      StringTools::strdup2( latest_revision_string, rev_string );
    }

    prev_match = matches.FindNextWildcardMatch();
  }

  return latest_revision;
}

// GenerateNewRevisionPath():
//  Generates a new revision based on the filename of the object.
//   The previously read revision is used as a starting point,
//   after which the revision is incremented until a newer version
//   is not found on disk.  That revision number becomes the last
//   revision.
//  mode determines if the this function returns a unique revision,
//   the next revision, or the previous revision.  Unique always
//   retruns a valid path.
char * SaveObjects_Object::GenerateNewRevisionPath( char * buffer, int min_digits,
                                                    SaveObjects_NewRevision mode,
                                                    char seperator ) {
  // Generate the name sans revision and extension
  strcpy( buffer, full_path );
  char *ext = DirStrings::HasExtension( full_path );

  if( ext == NULL )
    ext = &(full_path[ strlen( full_path ) - 1]);

  // - If it ends in a digit, add a '-'
  if( revision == -1 ) {
    int len = strlen( buffer );
    if( isdigit( buffer[len - 1] ) )
      strncat( buffer, &seperator, 1 );
  }

  // Figure out how many digits the path currently has
  int digit_len = (revision == -1) ? 0 : strlen( revision_string );

  int test_rev = (revision == -1) ? 0 : revision;
  while( true ) {
    buffer[ ext - full_path - digit_len ] = '\0';
    (mode == SONR_PREVIOUS) ? test_rev-- : test_rev++;

    sprintf( buffer, "%s%0*d%s", buffer, min_digits, test_rev, ext );
    if( mode != SONR_UNIQUE )
      return buffer;

    if( !DirInfo::Exists( buffer ) )
      return buffer;
  }

  // We should never get here
  assert( false );
}

// GenerateSpecificRevisionPath():
//  Generates a specific revision path from full_path stored in the instance.
//   Note that this does not recognize any extensions existing in full_path,
//   and simply adds a new revision just before the extension.  Used by the
//   "Revision Backup" mode.
char * SaveObjects_Object::GenerateSpecificRevisionPath( char *buffer, int rev,
                                                         int min_digits, char *path,
                                                         char seperator ) {
  assert( buffer != NULL );

  // Generate the new path
  strcpy( buffer, GetFullPath() );

  // - Copy the filename and add the new path to it
  DirStrings::GetPathPart( buffer, GetFullPath() );

  if( path != NULL )
    DirStrings::AddPathPart( buffer, path );

  // - Add the file part to the buffer
  char * file_part = DirStrings::HasFile( GetFullPath() );
  assert( file_part != NULL );
  DirStrings::AddPathPart( buffer, file_part );
  DirStrings::ChangeExtension( buffer, "" );

  // - Get the extension, if any
  char *ext = DirStrings::HasExtension( full_path );

  // - Remove the existing revision, if any
  if( revision != -1 )
    buffer[ strlen( buffer ) - strlen( revision_string ) ] = '\0';

  // - If it ends in a digit, add a '-'
  if( revision == -1 ) {
    int len = strlen( buffer );
    if( isdigit( buffer[len - 1] ) )
      strncat( buffer, &seperator, 1 );
  }

  // Add the revisioning data and restore the extension
  sprintf( buffer, "%s%0*d%s", buffer, min_digits, rev, ((ext == NULL) ? "" : ext) );

  return buffer;
}
                                                         

// FindBackup():
//  Returns true if a backup exists on disk, and false if not.
bool SaveObjects_Object::FindBackup() { 
  char buffer[ MAX_PATH_LENGTH ];
  GenerateBackupPath( buffer );

  backup_exists = DirInfo::Exists( buffer );
  return backup_exists;
}

// GenerateBackupPath();
char * SaveObjects_Object::GenerateBackupPath( char * buffer ) {
/*  Alternate Version, placing -bu before the extension
  strcpy( buffer, GetFullPath() );

  char *ext = DirStrings::HasExtension( GetFullPath() );
  if( ext != NULL )
    DirStrings::ChangeExtension( buffer, "" );

  strcat( buffer, "-bu" );

  if( ext != NULL )
    strcat( buffer, ext );

  return buffer;
*/

  strcpy( buffer, GetFullPath() );
  strcat( buffer, "-bu" );
  return buffer;
}

// BackupExisting():
//  Creates a backup of the original object by renaming the object
//   that currently exists on disk to end in -bu.  Returns false
//   if no object already exists on disk to back it up.
bool SaveObjects_Object::BackupExisting() {
  // See if the object already exists on disk so it can be backed up
  if( !DirInfo::Exists( GetFullPath() ) )
    return false;

  // Generate the new filename
  char buffer[ MAX_PATH_LENGTH ];
  GenerateBackupPath( buffer );

  // If the backup already exists, delete it se we can make a new one
  if( DirInfo::Exists( buffer ) ) {
    SetFileAttributes( buffer, FILE_ATTRIBUTE_NORMAL );
    DirInfo::Delete( buffer );
  }

  // Rename the existing object as the backup
  DirInfo::Rename( GetFullPath(), buffer );
  SetFileAttributes( buffer, FILE_ATTRIBUTE_READONLY );

  return true;
};

// FindDuplicate():
bool SaveObjects_Object::FindDuplicate( char *path, char *prefix, char *suffix ) {
  char buffer[ MAX_PATH_LENGTH ];

  // GenerateDuplicatePath() returns false if both paths are the same
  if( GenerateDuplicatePath( buffer, path, prefix, suffix ) == NULL )
    duplicate_exists = false;
  else
    duplicate_exists = DirInfo::Exists( buffer );

  return duplicate_exists;
}

// GenerateDuplicatePath():
//  Returns false if the generated path and the object's path are equal
char * SaveObjects_Object::GenerateDuplicatePath( char *buffer, char *path, char *prefix, char *suffix ) {
  assert( GetFullPath() != NULL );
  assert( path != NULL );
  assert( buffer != NULL );

  DirStrings::GetPathPart( buffer, GetFullPath() );
  DirStrings::AddPathPart( buffer, path );
  if( prefix != NULL ) {
    if( prefix[0] != '\0' )
      DirStrings::AddPathPart( buffer, prefix );
  }

  char * file_part = DirStrings::HasFile( GetFullPath() );
  assert( file_part != NULL );

  if( prefix == NULL ) {
    DirStrings::AddPathPart( buffer, file_part );
  } else {
    if( prefix[0] == '\0' )
      DirStrings::AddPathPart( buffer, file_part );
    else 
      strcat( buffer, file_part );
  }

  DirStrings::ChangeExtension( buffer, "" );
  if( suffix != NULL )
    strcat( buffer, suffix );

  char *ext = DirStrings::HasExtension( GetFullPath() );
  if( ext )
    strcat( buffer, ext );

  // Error if the paths the same equal
  if( stricmp( buffer, GetFullPath() ) == 0 )
    return NULL;

  return buffer;
}


// SaveObject():
//  Saves the object to disk, overwritting any existing one.
//  If overwrite_read_only is true, then read-only objects will
//   be set to read/write and be saved over; otherwise, read-only
//   objects will not be saved.  Returns false if this is the case.
//  mode determins the file mode of the file after it has been saved.
//  Note that this ignores the GetSaveObject() state of the object;
//   that must be tested by the calling function.
bool SaveObjects_Object::SaveObject( SaveObjects_ProcessData *data ) {
  DWORD prev_attrib;

  // Skip Read Only objects if the toggle is off
  bool file_exists;
  if( DirInfo::Exists( GetFullPath() ) ) {
    if( (data->versioning_mode != SOVM_NEW_REVISION) &&
        (data->versioning_mode != SOVM_DUPLICATE) &&
        (data->versioning_mode != SOVM_DUPLICATE_AND_REPLACE) ) {

      prev_attrib = GetFileAttributes( GetFullPath() );
      if( (prev_attrib & FILE_ATTRIBUTE_READONLY) && !data->overwrite_read_only )
        return false;

      SetFileAttributes( GetFullPath(), FILE_ATTRIBUTE_NORMAL );
    }
    file_exists = true;
  } else {
    prev_attrib = FILE_ATTRIBUTE_NORMAL;
    file_exists = false;
  }

  // Backup or Increment Revision, if enabled
  char output_path[ MAX_PATH_LENGTH ];
  if( data->versioning_mode == SOVM_NEW_REVISION ) {
    GenerateSpecificRevisionPath( output_path, latest_revision + 1, data->min_digits, NULL,
                                  seperator_strings[ data->seperator ][0] );
  } else if( (data->versioning_mode == SOVM_DUPLICATE) ||
             (data->versioning_mode == SOVM_DUPLICATE_AND_REPLACE) ) {
    if( GenerateDuplicatePath( output_path, data->path, data->prefix, data->suffix ) == NULL ) {
      (*data->message->error)( "SaveObjects Error:  Duplicate output and object source paths are equal:", output_path );
      return false;
    }

    if( !DirInfo::MakeTree( output_path, true ) ) {   // Create the duplicate path, if needed
      (*data->message->error)( "SaveObjects Error:  Unable to create the duplicate output path", output_path );
      return false;
    }

  } else if( data->versioning_mode == SOVM_REVISION_BACKUP ) {
    if( latest_revision == -1 ) {
      GenerateSpecificRevisionPath( output_path, 0, data->min_digits, data->path,
                                    seperator_strings[ data->seperator ][0] );

      if( !DirInfo::MakeTree( output_path, true ) ) { // Create the revision backup path, if needed
        (*data->message->error)( "SaveObjects Error:  Unable to create the duplicate output path", output_path );
        return false;
      }

      if( DirInfo::Exists( output_path ) ) {
        (*data->message->error)( "SaveObjects Error:  Backup revision already exists:", output_path );
        return false;
      }

      SetFileAttributes( GetFullPath(), FILE_ATTRIBUTE_NORMAL );
      DirInfo::Rename( GetFullPath(), output_path );

      // Set the renamed file's attributes
      switch( data->post_save_mode ) {
        case PSFM_READ_ONLY:       // Read Only
          SetFileAttributes( output_path, FILE_ATTRIBUTE_READONLY );
          break;

        case PSFM_PREVIOUS_STATE:  // Previous State
          SetFileAttributes( output_path, prev_attrib );
          break;

        case PSFM_READ_WRITE:      // Read/Write
        default:
          // Already set above
          break;
      }
    }

    if( latest_revision == -1 ) {
      latest_revision = 1;
    } else {
      DirInfo::MakeTree( output_path, true );    // Create the revision backup path, if needed
      latest_revision++;
    }

    GenerateSpecificRevisionPath( output_path, latest_revision, data->min_digits, data->path,
                                  seperator_strings[ data->seperator ][0] );
    if( DirInfo::Exists( output_path ) ) {
      (*data->message->error)( "SaveObjects Error:  Backup revision already exists:", output_path );
      return false;
    }
  } else {
    strcpy( output_path, GetFullPath() );

    if( data->versioning_mode == SOVM_BACKUP ) {
      if( file_exists )
        BackupExisting();
    }
  }

  // Save the object
  char buffer[ MAX_PATH_LENGTH + 50 ];
  sprintf( buffer, "SelectItem %x", GetItemID() );
  (*data->generic->evaluate)( data->generic->data, buffer );

  if( data->versioning_mode == SOVM_DUPLICATE )
    sprintf( buffer, "SaveObjectCopy %s", output_path );
  else
    sprintf( buffer, "SaveObject %s", output_path );

  (*data->generic->evaluate)( data->generic->data, buffer );

  // Post-Save File Attribute Adjustment
  if( (data->versioning_mode != SOVM_NEW_REVISION) &&
      (data->versioning_mode != SOVM_DUPLICATE) &&
      (data->versioning_mode != SOVM_DUPLICATE_AND_REPLACE) ) {
    switch( data->post_save_mode ) {
      case PSFM_READ_ONLY:       // Read Only
        SetFileAttributes( output_path, FILE_ATTRIBUTE_READONLY );
        break;

      case PSFM_PREVIOUS_STATE:  // Previous State
        SetFileAttributes( output_path, prev_attrib );
        break;

      case PSFM_READ_WRITE:      // Read/Write
      default:
        // Already set above
        break;
    }
  }

  // Revision Backup:  Call this function again in SOVM_DISABLED mode so the original object will also be saved over
  if( data->versioning_mode == SOVM_REVISION_BACKUP ) {
    int old_mode = data->versioning_mode;
    data->versioning_mode = SOVM_DISABLED;
    SaveObject( data );
    data->versioning_mode = old_mode;
  }

  return true;
}

