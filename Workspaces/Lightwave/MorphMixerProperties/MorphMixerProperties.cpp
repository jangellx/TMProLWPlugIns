//
// MorphMixerProperties.cpp
//

#define NULL 0

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwdisplce.h>

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
  XCALL_ (int) MorphMixerProperties_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) MorphMixerPropertiesApply_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData );

  ServerUserName MorphMixerProperties_Usernames[]      = { {"TM-P  MorphMixer Properties"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },          { "Morph Mixer Properties"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },         { (const char *)NULL } };
  ServerUserName MorphMixerPropertiesApply_Usernames[] = { {"TM-P  MorphMixer Properties (Apply)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Morph Mixer Properties (Apply)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_MorphMixerProperties"DEBUG_PLUGIN_PLUGNAME,      MorphMixerProperties_Activate,      MorphMixerProperties_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MorphMixerPropertiesApply"DEBUG_PLUGIN_PLUGNAME, MorphMixerPropertiesApply_Activate, MorphMixerPropertiesApply_Usernames },
    { (const char *)NULL }
  };
}

// MorphMixerProperties_Activate():
XCALL_ (int) MorphMixerProperties_Internal( long version, GlobalFunc *global,
                                            void *local, void *serverData,
                                            bool activate ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL )
    return AFUNC_OK;

  if( (*iteminfo->type)( ui->selItems[0] ) != LWI_OBJECT )
    return AFUNC_OK;

  const char *server_name;
  char buffer[ 256 ];

  // Look for MorphMixer on the current object
  server_name = (const char *)-1;
  for( unsigned long i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( ui->selItems[0], LWDISPLACEMENT_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "LW_MorphMixer" ) == 0 ) {
      break;
    }
  }

  // Apply MorphMixer if it isn't already there
  if( server_name == NULL ) {
    if( activate ) {
      LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
      char buffer[ 256 ];
      sprintf( buffer, "ApplyServer %s %s", LWDISPLACEMENT_HCLASS, "LW_MorphMixer" );
      (*generic->evaluate)( generic->data, buffer );
    } else {
      return AFUNC_OK;
    }
  }

  // Open it's UI
  sprintf( buffer, "EditServer %s %d", LWDISPLACEMENT_HCLASS, i );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// MorphMixerProperties_Activate():
XCALL_ (int) MorphMixerProperties_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData ) {
  return MorphMixerProperties_Internal( version, global, local, serverData, false );
}

// MorphMixerPropertiesApply_Activate():
XCALL_ (int) MorphMixerPropertiesApply_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData ) {
  return MorphMixerProperties_Internal( version, global, local, serverData, true );
}
