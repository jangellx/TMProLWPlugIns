//
// SaveObjects.cpp
//

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#include "SaveObjects-Interface.h"
#include <stdio.h>

extern "C" {
  // User Names
  ServerUserName SaveObjects_Usernames[]                                  = { {"TM-P  Save Objects"                                       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Save Objects"                                       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName BackupAndSaveObject_Usernames[]                          = { {"TM-P  Backup And Save Object"                             DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Backup And Save Object"                             DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithBackupObject_Usernames[]                      = { {"TM-P  Replace With Backup Object"                         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Replace With Backup Object"                         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SaveNewObjectRevision_Dash_Usernames[]                   = { {"TM-P  Save New Object Revision (Dash)"                    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Save New Object Revision (Dash)"                    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithPreviousObjectRevision_Dash_Usernames[]       = { {"TM-P  Replace With Previous Object Revision (Dash)"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Replace With Previous Object Revision (Dash)"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithNextObjectRevision_Dash_Usernames[]           = { {"TM-P  Replace With Next Object Revision (Dash)"           DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Replace With Next Object Revision (Dash)"           DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SaveNewObjectRevision_Underscore_Usernames[]             = { {"TM-P  Save New Object Revision (Underscore)"              DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Save New Object Revision (Underscore)"              DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithPreviousObjectRevision_Underscore_Usernames[] = { {"TM-P  Replace With Previous Object Revision (Underscore)" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Replace With Previous Object Revision (Underscore)" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithNextObjectRevision_Underscore_Usernames[]     = { {"TM-P  Replace With Next Object Revision (Underscore)"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Replace With Next Object Revision (Underscore)"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_SaveObjects"                                  DEBUG_PLUGIN_PLUGNAME , SaveObjects_Activate,                                  SaveObjects_Usernames                                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_BackupAndSaveObject"                          DEBUG_PLUGIN_PLUGNAME , BackupAndSaveObject_Activate,                          BackupAndSaveObject_Usernames                          },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithBackupObject"                      DEBUG_PLUGIN_PLUGNAME , ReplaceWithBackupObject_Activate,                      ReplaceWithBackupObject_Usernames                      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceTest"                                  DEBUG_PLUGIN_PLUGNAME , ReplaceTest_Activate,                                  NULL                                                   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SaveNewObjectRevision_Dash"                   DEBUG_PLUGIN_PLUGNAME , SaveNewObjectRevision_Dash_Activate,                   SaveNewObjectRevision_Dash_Usernames                   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithPreviousObjectRevision_Dash"       DEBUG_PLUGIN_PLUGNAME , ReplaceWithPreviousObjectRevision_Dash_Activate,       ReplaceWithPreviousObjectRevision_Dash_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithNextObjectRevision_Dash"           DEBUG_PLUGIN_PLUGNAME , ReplaceWithNextObjectRevision_Dash_Activate,           ReplaceWithNextObjectRevision_Dash_Usernames           },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SaveNewObjectRevision_Underscore"             DEBUG_PLUGIN_PLUGNAME , SaveNewObjectRevision_Underscore_Activate,             SaveNewObjectRevision_Underscore_Usernames             },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithPreviousObjectRevision_Underscore" DEBUG_PLUGIN_PLUGNAME , ReplaceWithPreviousObjectRevision_Underscore_Activate, ReplaceWithPreviousObjectRevision_Underscore_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithNextObjectRevision_Underscore"     DEBUG_PLUGIN_PLUGNAME , ReplaceWithNextObjectRevision_Underscore_Activate,     ReplaceWithNextObjectRevision_Underscore_Usernames     },
    { (const char *)NULL }
  };
}

// SaveObjects_Activate():
XCALL_ (int) SaveObjects_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  SaveObjects_Interface sui( global, local );

  // Build a selected item list
  sui.BuildItemList();

  // Create and open the UI
  sui.OpenUI();

  sui.ReselectItems();

  return AFUNC_OK;
}

// BackupObjectAndSave_Activate():
XCALL_ (int) BackupAndSaveObject_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class (we needs some of the globals it gets)
  SaveObjects_Interface sui( global, local );
  if( sui.sel_type != LWI_OBJECT ) {
    (*sui.message->error)( "Backup And Save Object Error:  You must select an object to backup and save.", NULL );
    return AFUNC_OK;
  }

  // Build a selected item list
  sui.BuildItemList();
  
  // Save the object
  SaveObjects_Object object( sui.items[0], sui.objectinfo, 0 );
  object.BackupExisting();

  SaveObjects_ProcessData process_data( sui.generic, true, PSFM_PREVIOUS_STATE );
  process_data.message = sui.message;
  object.SaveObject( &process_data );

  // Reselect the previously selected items
  sui.ReselectItems();

  return AFUNC_OK;
}

// ReplaceWithBackupObject_Activate():
XCALL_ (int) ReplaceWithBackupObject_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class (we needs some of the globals it gets)
  SaveObjects_Interface sui( global, local );
  if( sui.sel_type != LWI_OBJECT ) {
    (*sui.message->error)( "Replace With Backup Object Error:  You must select an object to replace.", NULL );
    return AFUNC_OK;
  }

  // Build a selected item list
  sui.BuildItemList();

  // See if the backup exists
  SaveObjects_Object object( sui.items[0], sui.objectinfo, 0 );

  char backup_path[ MAX_PATH_LENGTH ];
  object.GenerateBackupPath( backup_path );

  if( !DirInfo::Exists( backup_path ) ) {
    (*sui.message->error)( "Replace With Backup Object Error:  Backup does not exist:",
                           backup_path );
    return AFUNC_OK;
  }

  // Generate the temp path
  char temp_path[ MAX_PATH_LENGTH ];
  strcpy( temp_path, backup_path );
  strcat( temp_path, "-temp" );

  // Delete the temp object if it already exists
  if( DirInfo::Exists( temp_path ) ) {
    SetFileAttributes( backup_path, FILE_ATTRIBUTE_NORMAL );
    DirInfo::Delete( temp_path );
  }

  // Rename the backup to the original object's name and visa-versa 
  SetFileAttributes( backup_path, FILE_ATTRIBUTE_NORMAL );
  DirInfo::Rename( backup_path, temp_path );                           // Rename Backup to Temp

  DWORD prev_attrib = GetFileAttributes( object.GetFullPath() );
  SetFileAttributes( object.GetFullPath(), FILE_ATTRIBUTE_NORMAL );
  DirInfo::Rename( object.GetFullPath(), backup_path );                // Rename Object to Backup
  SetFileAttributes( backup_path, FILE_ATTRIBUTE_READONLY );

  DirInfo::Rename( temp_path, object.GetFullPath() );                  // Rename Temp to Object
  SetFileAttributes( object.GetFullPath(), prev_attrib );

  // Replace the object in Layout
  char buffer[ MAX_PATH_LENGTH + 50 ];
  sprintf( buffer, "SelectItem %x", object.GetItemID() );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  sprintf( buffer, "ReplaceWithObject %s", object.GetFullPath() );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  // Reselect the previously selected items
  sui.ReselectItems();

  return AFUNC_OK;
}

#define REPLACE_PATH "I:\\ArachTest\\Content\\Objects\\RedBall"

// ReplaceTest_Activate():
XCALL_ (int) ReplaceTest_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  char *object = REPLACE_PATH".lwo";
  char *temp   = REPLACE_PATH"-temp.lwo";
  char *backup = REPLACE_PATH"-bu.lwo";

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;

  // Rename the backup to the original object's name and visa-versa 
  rename( backup, temp   );
  rename( object, backup );
  rename( temp,   object );

  (*generic->evaluate)( generic->data, "ReplaceWithObject "REPLACE_PATH".lwo" );

  return AFUNC_OK;
}

// SaveNewObjectRevision_Dash_Activate():
XCALL_ (int) SaveNewObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                  void *local, void *serverData ) {
  return SaveNewObjectRevision_Activate( version, global, local, '-' );
}

// SaveNewObjectRevision_Underscore_Activate():
XCALL_ (int) SaveNewObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                        void *local, void *serverData ) {
  return SaveNewObjectRevision_Activate( version, global, local, '_' );
}

// ReplaceWithPrevionsObjectRevision_Dash_Activate():
XCALL_ (int) ReplaceWithPreviousObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                              void *local, void *serverData ) {
  return ReplaceWithObjectRevision_Activate( version, global, local, SONR_PREVIOUS, '-' );
}

// ReplaceWithNextObjectRevision_Dash_Activate():
XCALL_ (int) ReplaceWithNextObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                          void *local, void *serverData ) {
  return ReplaceWithObjectRevision_Activate( version, global, local, SONR_NEXT, '-' );
}

// ReplaceWithPrevionsObjectRevision_Underscore_Activate():
XCALL_ (int) ReplaceWithPreviousObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                                    void *local, void *serverData ) {
  return ReplaceWithObjectRevision_Activate( version, global, local, SONR_PREVIOUS, '_' );
}

// ReplaceWithNextObjectRevision_Underscore_Activate():
XCALL_ (int) ReplaceWithNextObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                                void *local, void *serverData ) {
  return ReplaceWithObjectRevision_Activate( version, global, local, SONR_NEXT, '_' );
}

// ReplaceWithObjectRevision_Activate():
XCALL_ (int) ReplaceWithObjectRevision_Activate( long version, GlobalFunc *global, void *local,
                                                 SaveObjects_NewRevision mode, char seperator ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class (we needs some of the globals it gets)
  SaveObjects_Interface sui( global, local );
  if( sui.sel_type != LWI_OBJECT ) {
    (*sui.message->error)( "Replace With Next Object Revision Error:  You must select an object to replace.", NULL );
    return AFUNC_OK;
  }

  // Build a selected item list
  sui.BuildItemList();

  // Generate a new output path
  SaveObjects_Object object( sui.items[0], sui.objectinfo, 2 );

  char path[ MAX_PATH_LENGTH ];
  object.GenerateNewRevisionPath( path, 2, mode, seperator );

  if( !DirInfo::Exists( path ) ) {
    (*sui.message->error)( "Replace With Next Object Revision Error:  No immediatly following revision found:", path );
    return AFUNC_OK;
  }

  // Replace the object
  char buffer[ MAX_PATH_LENGTH + 50 ];
  sprintf( buffer, "SelectItem %x", object.GetItemID() );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  sprintf( buffer, "ReplaceWithObject %s", path );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  // Reselect the previously selected items
  sui.ReselectItems();

  return AFUNC_OK;
}

// SaveNewObjectRevision_Activate():
XCALL_ (int) SaveNewObjectRevision_Activate( long version, GlobalFunc *global, void *local, char seperator ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class (we needs some of the globals it gets)
  SaveObjects_Interface sui( global, local );
  if( sui.sel_type != LWI_OBJECT ) {
    (*sui.message->error)( "Save New Object Revision Error:  You must select an object to save.", NULL );
    return AFUNC_OK;
  }

  // Build a selected item list
  sui.BuildItemList();

  // Generate a new output path
  SaveObjects_Object object( sui.items[0], sui.objectinfo, 2 );

  char path[ MAX_PATH_LENGTH ];
  object.FindLatestRevision( 2, NULL );
  object.GenerateSpecificRevisionPath( path, object.GetLatestRevision() + 1, 2, NULL, seperator );

  // Save the new object revision in Layout
  char buffer[ MAX_PATH_LENGTH + 50 ];
  sprintf( buffer, "SelectItem %x", object.GetItemID() );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  sprintf( buffer, "SaveObject %s", path );
  (*sui.generic->evaluate)( sui.generic->data, buffer );

  // Reselect the previously selected items
  sui.ReselectItems();

  return AFUNC_OK;
}

