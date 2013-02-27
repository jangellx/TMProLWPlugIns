//
// SaveObjects-Object.h
//

#ifndef SAVE_OBJECTS_HEADER
#define SAVE_OBJECTS_HEADER

#include <portable/stringtools/stringtools.h>
#include <portable/dirtools/dirtools.h>

enum SaveObjects_PostSaveFileMode {
  PSFM_READ_ONLY = 0, PSFM_READ_WRITE, PSFM_PREVIOUS_STATE };

enum SaveObjects_VersioningMode {
  SOVM_DISABLED = 0, SOVM_BACKUP, SOVM_NEW_REVISION, SOVM_REVISION_BACKUP,
  SOVM_DUPLICATE, SOVM_DUPLICATE_AND_REPLACE };

enum SaveObjects_NewRevision {
  SONR_UNIQUE = 0, SONR_PREVIOUS, SONR_NEXT };

class SaveObjects_ProcessData;

// class SaveObjects_Object
class SaveObjects_Object {
public:
  SaveObjects_Object( LWItemID _id, LWObjectInfo *objectinfo, int min_revision_digits );
  ~SaveObjects_Object() { StringTools::strdup2( full_path, NULL );
                          StringTools::strdup2( revision_string, NULL );
                          StringTools::strdup2( latest_revision_string, NULL ); }

  // Initializers
  void   RefreshPath( LWObjectInfo *objectinfo );
  bool   RefreshReadOnly();

  char * FindRevision( int min_digits );
  int    FindLatestRevision( int min_digits, char *path );
  int    FindRevisionInString( char *rev_string, const char *string, int min_digits );
  char * GenerateSpecificRevisionPath( char *buffer, int rev, int min_digits, char *path, char seperator = '-' );

  bool   FindBackup();

  bool   FindDuplicate( char *path, char *prefix, char *suffix );

  // Management
  char * GenerateBackupPath( char *buffer );
  bool   BackupExisting();

  char * GenerateNewRevisionPath( char * buffer, int min_digits,
                                  SaveObjects_NewRevision mode = SONR_UNIQUE, char seperator = '-' );

  char * GenerateDuplicatePath( char *buffer, char *path, char *prefix, char *suffix );

  bool   SaveObject( SaveObjects_ProcessData *data );

  // Accessors
  inline void         SetItemID( LWItemID _id ) { item_id = _id; }
  inline LWItemID     GetItemID() { return item_id; }

  inline const char * GetFullPath() { return full_path; }
  inline const char * GetFileName() { return filename;  }

  inline void         SetIndex( int _index ) { index = _index; }
  inline int          GetIndex()             { return index;   }

  inline bool         GetExists()   { return exists;    }
  inline bool         GetReadOnly() { return read_only; }

  inline bool         DoesBackupExist()    { return backup_exists;    }
  inline bool         DoesDuplicateExist() { return duplicate_exists; }

  inline int          GetRevision()       { return revision;        }
  inline const char * GetRevisionString() { return revision_string; }

  inline int          GetLatestRevision()       { return latest_revision;        }
  inline const char * GetLatestRevisionString() { return latest_revision_string; }

  inline bool         GetSaveObject() { return save_object; }
  inline void         SetSaveObject( bool state ) { save_object = state; }

protected:
  LWItemID     item_id;         // Item ID

  char       * full_path;       // Full filename of the object
  char       * filename;        // Pointer within full_path

  bool         exists;          // True if the object exists on disk
  bool         read_only;       // True if the object on disk is read-only

  int          revision;        // Revision of the object from it's name, or -1 if no revision was found.
  char        *revision_string; // Revision as a string

  int          latest_revision;        // Lastest revision of the object from it's name, or -1 if no revision was found.  Used by Revision Backup mode
  char *       latest_revision_string; // Lastest revision as a string

  bool         backup_exists;   // True if a backup exists on disk
  bool         duplicate_exists;// True if a duplicate exists on disk

  bool         save_object;     // True if the object should be saved

  int          index;           // Natural order index
};

#endif