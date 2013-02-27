//
// SaveObjects.h
//

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

#include  <lwgeneric.h>

#include "SaveObjects-Object.h"

extern "C" {
  XCALL_ (int) SaveObjects_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData );

  XCALL_ (int) BackupAndSaveObject_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );

  XCALL_ (int) ReplaceWithBackupObject_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData );

  XCALL_ (int) ReplaceTest_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData );

  // Dash-Based
  XCALL_ (int) SaveNewObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData );
  XCALL_ (int) ReplaceWithPreviousObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                                void *local, void *serverData );
  XCALL_ (int) ReplaceWithNextObjectRevision_Dash_Activate( long version, GlobalFunc *global,
                                                            void *local, void *serverData );


  // Underscore-Based
  XCALL_ (int) SaveNewObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                          void *local, void *serverData );
  XCALL_ (int) ReplaceWithNextObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                                  void *local, void *serverData );
  XCALL_ (int) ReplaceWithPreviousObjectRevision_Underscore_Activate( long version, GlobalFunc *global,
                                                                      void *local, void *serverData );


  // ReplaceWithObjectRevision_Activate():
  XCALL_ (int) ReplaceWithObjectRevision_Activate( long version, GlobalFunc *global, void *local,
                                                   SaveObjects_NewRevision mode, char seperator );

  // SaveNewObjectRevision_Activate():
  XCALL_ (int) SaveNewObjectRevision_Activate( long version, GlobalFunc *global,
                                               void *local, char seperator );
}

