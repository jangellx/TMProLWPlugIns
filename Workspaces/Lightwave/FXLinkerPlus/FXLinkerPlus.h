//
// FXLinkerPlus.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

#include <lwmotion.h>
#include <lwgeneric.h>
#include <lwprtcl.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#define MOTION_PLUGIN_NAME "TM-P_FXLinkerPlusMotion"DEBUG_PLUGIN_PLUGNAME

extern "C" {
  XCALL_ (int) FXLinkerPlus_Generic_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) FXLinkerPlus_Motion_Activate( long version, GlobalFunc *global,
                                             void *_local, void *serverData );

  XCALL_ (int) FXLinkerPlus_MotionUI_Activate( long version, GlobalFunc *global,
                                               void *_local, void *serverData );

  void         FXLinkerPlus_Motion_Evaluate( LWInstance _inst, const LWItemMotionAccess *access );
  unsigned int FXLinkerPlus_Motion_Flags   ( LWInstance _inst );

  LWInstance   FXLinkerPlus_Motion_Create(   void *priv, void *context, LWError *error );
  void         FXLinkerPlus_Motion_Destroy(  LWInstance _inst );
  LWError      FXLinkerPlus_Motion_Copy(     LWInstance _to, LWInstance _from);
  LWError      FXLinkerPlus_Motion_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError      FXLinkerPlus_Motion_Save(     LWInstance _inst, const LWSaveState *saver );
  const char * FXLinkerPlus_Motion_Describe( LWInstance _inst );

  const LWItemID * FXLinkerPlus_Motion_UseItems( LWInstance _inst );
  void             FXLinkerPlus_Motion_ChangeID( LWInstance _inst, const LWItemID *id );

  LWError FXLinkerPlus_MotionUI_Open( LWInstance _inst );

/*
  void *Startup( void );
  void Shutdown( void *serverData );
*/
}

