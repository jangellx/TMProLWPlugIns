//
// SimplifyGeneric.cpp
//
// This file includes all the Simplify Generic plug-ins.
//

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "Simplify_Generic.h"
#include "Simplify_GlobalControl_Interface.h"

extern SimplifyGlobal simp_global;

// Function Declarations
extern "C" {
  XCALL_ (int) Simplify_TogglePreview( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) Simplify_ToggleRender( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) Simplify_ToggleLOD( long version, GlobalFunc *global,
                                   void *local, void *serverData );

  XCALL_ (int) Simplify_ToggleAutoLoad( long version, GlobalFunc *global,
                                        void *local, void *serverData );

  XCALL_ (int) Simplify_GlobalControl( long version, GlobalFunc *global,
                                       void *local, void *serverData );
}

// Simplify_TogglePreview()
XCALL_ (int) Simplify_TogglePreview( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  CMessage * message = CMessage::New( (MessageFuncs *) (*global)("Info Messages", GFUSE_TRANSIENT) );

  simp_global.preview = !simp_global.preview;
  if( simp_global.preview ) {
    message->Info( "Simpliy:  Preview objects are now active.  You must make a wireframe or",
                   "OpenGL preview of at least 1 frame before the objects will be replaced" );
  } else {
    message->Info( "Simpliy:  Preview objects are now disabled.  You must start",
                   "rendering a frame before the objects will be replaced" );
  }

  delete message;

  return AFUNC_OK;
}

// Simplify_ToggleRender()
XCALL_ (int) Simplify_ToggleRender( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  CMessage * message = CMessage::New( (MessageFuncs *) (*global)("Info Messages", GFUSE_TRANSIENT) );

  simp_global.render = !simp_global.render;
  if( simp_global.render ) {
    message->Info( "Simpliy:  Render objects will now be",
                   "rendered instead of Preview objects" );
  } else {
    message->Info( "Simpliy:  Preview objects will now be",
                   "rendered instead of Render objects" );
  }

  delete message;

  return AFUNC_OK;
}

// Simplify_ToggleLOD()
XCALL_ (int) Simplify_ToggleLOD( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {
  CMessage * message = CMessage::New( (MessageFuncs *) global("Info Messages", GFUSE_TRANSIENT) );

  simp_global.lod = !simp_global.lod;
  if( simp_global.lod ) {
    message->Info( "Simpliy:  LOD objects are now active" );
  } else {
    message->Info( "Simpliy:  LOD objects are now disabled" );
  }

  delete message;

  return AFUNC_OK;
}

// Simplify_ToggleAutoLoad()
XCALL_ (int) Simplify_ToggleAutoLoad( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  CMessage * message = CMessage::New( (MessageFuncs *) global("Info Messages", GFUSE_TRANSIENT) );

  simp_global.autoload = !simp_global.autoload;
  if( simp_global.autoload )
    message->Info( "Simpliy:  Auto-Loading of Default SDFs is now active" );
  else
    message->Info( "Simpliy:  Auto-Loading of Default SDFs is now disabled" );

  delete message;

  return AFUNC_OK;
}

// Simplify_GlobalControl()
XCALL_ (int) Simplify_GlobalControl( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {

  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  SimplifyGlobalInterface sg( global, local, retval );

  return retval;
}
