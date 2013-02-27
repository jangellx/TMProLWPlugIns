//
// SelectOMatic.h
//

#define NULL 0

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwmaster.h>
#include  <lwglobsrv.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern const char *selectomatic_server_name;

extern "C" {
  XCALL_ (int) SOM_Master_Activate( long version, GlobalFunc *global,
                                    void *_local, void *serverData );
  XCALL_ (int) SOM_Interface_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );
  XCALL_ (int) SOM_SelectGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  XCALL_ (int) SOM_InterfaceGeneric_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) SelectGoal_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData );
  XCALL_ (int) SelectTarget_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) SelectAllRoots_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) SelectRoot_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData );

  XCALL_ (int) SelectAllNulls_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );

  XCALL_ (int) SelectClones_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) SelectRootGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) SelectFirstGroup_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );
  XCALL_ (int) SelectLastGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) SelectNextGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) SelectPrevGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) SelectNextInGroup_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
  XCALL_ (int) SelectPrevInGroup_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
  XCALL_ (int) SelectNextInGroupHierarchy_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData );
  XCALL_ (int) SelectPrevInGroupHierarchy_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData );
  XCALL_ (int) SelectParentGroup_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
  XCALL_ (int) SelectFirstChildGroup_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData );
  XCALL_ (int) SelectFirstInGroup_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData );
  XCALL_ (int) SelectLastInGroup_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );


/*
  XCALL_ (int) SelectChildren_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) SelectDescendants_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData );
*/
}

// Global Class Name
extern const char * TMPSELECTGROUP_GLOBAL;
