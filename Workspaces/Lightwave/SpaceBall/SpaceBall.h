//
// SpaceBall.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwserver.h>
#include <lwhost.h>
#include <lwmonitor.h>
#include <lwrender.h>
#include <lwio.h>
#include <lwdyna.h>

#include <lwgeneric.h>
#include <lwmaster.h>
#include <lwdisplay.h>

#define OS_WIN32
#include <si.h>

// Plug-in Name
#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#define MASTER_PLUGIN_NAME "TM-P_SpaceBallMaster"DEBUG_PLUGIN_PLUGNAME

extern "C" {
  // SpaceBall Master
  XCALL_ (int) SpaceBall_Activate( long version, GlobalFunc *global,
                                   void *_local, void *serverData );

  // SpaceBall Master Interface
  XCALL_ (int) SpaceBall_Interface_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );

  // SQUID Generics
  XCALL_ (int) SpaceBallProperties_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData );
  XCALL_ (int) SpaceBallApply_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );

  XCALL_ (int) SpaceBallEnable_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );
  XCALL_ (int) SpaceBallDisable_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );
  XCALL_ (int) SpaceBallToggle_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  XCALL_ (int) SpaceBallState( long version, GlobalFunc *global,
                               void *local, void *serverData, bool new_state );

  XCALL_ (int) SpaceBallReset_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) SpaceBallConfig_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData );

  XCALL_ (int) SpaceBallMove_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );
  XCALL_ (int) SpaceBallScale_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData );
  XCALL_ (int) SpaceBallMoveScale_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData );

  XCALL_ (int) SpaceBallMoveState( long version, GlobalFunc *global,
                                   void *local, void *serverData, bool new_state );

//  void * Startup( void );
//  void   Shutdown( void *serverData );

  // Master functions
  LWInstance       SpaceBall_Create(   void *data, void *context, LWError *error );
  void             SpaceBall_Destroy(  LWInstance _inst );
  LWError          SpaceBall_Copy(     LWInstance _to, LWInstance _from );
  LWError          SpaceBall_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError          SpaceBall_Save(     LWInstance _inst, const LWSaveState *saver );
  const char     * SpaceBall_Describe( LWInstance _inst );

  double           SpaceBall_Event   ( LWInstance _inst, const LWMasterAccess *access );
  unsigned int     SpaceBall_Flags   ( LWInstance _inst );

  const LWItemID * SpaceBall_UseItems( LWInstance _inst );
  void             SpaceBall_ChangeID( LWInstance _inst, const LWItemID *ids );

  LWError          SpaceBall_OpenUI( void *data );

  void             LoadSettings();
  void             SaveSettings();
}

// Externs
typedef int LWCommandFunc( const char *cmd );

extern GlobalFunc      *global;
extern HostDisplayInfo *hdi;
extern LWCommandFunc   *command;
extern LWMessageFuncs  *message;

extern SiHdl            hdl;                    // Spaceball device handle
extern bool             open_config_on_startup;

LRESULT CALLBACK SpaceBallWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

