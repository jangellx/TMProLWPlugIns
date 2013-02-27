//
// HVProperties.cpp
//

#define NULL 0

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwvolume.h>

#include <string.h>
#include <stdio.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  XCALL_ (int) HVProperties_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  ServerUserName HVProperties_Usernames[] = { {"TM-P  Hypervoxel Properties"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Hypervoxel Properties"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_HypervoxelProperties"DEBUG_PLUGIN_PLUGNAME, HVProperties_Activate, HVProperties_Usernames },
    { (const char *)NULL }
  };
}

// HVProperties_Activate():
XCALL_ (int) HVProperties_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );

  const char *server_name;
  char buffer[ 256 ];

  // Look for HyperVoxels
  server_name = (const char *)-1;
  for( unsigned long i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( NULL, LWVOLUMETRIC_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "HyperVoxelsFilter" ) == 0 ) {
      break;
    }
  }

  // Apply HyperBoxels if it isn't already there
  if( server_name == NULL ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWVOLUMETRIC_HCLASS, "HyperVoxelsFilter" );
    (*generic->evaluate)( generic->data, buffer );
  }


  // Open it's UI
  sprintf( buffer, "EditServer %s %d", LWVOLUMETRIC_HCLASS, i );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

