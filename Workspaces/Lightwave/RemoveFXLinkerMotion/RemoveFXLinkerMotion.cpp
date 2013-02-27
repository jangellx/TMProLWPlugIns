//
// RemoveFXLinkerMotion.cpp
//

#define NULL 0

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwmotion.h>

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
  XCALL_ (int) RemoveFXLinkerMotion_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  ServerUserName RemoveFXLinkerMotion_Usernames[] = { {"TM-P  Remove FX Linker Motion"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Remove FX Linker Motion"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_RemoveFXLinkerMotion"DEBUG_PLUGIN_PLUGNAME, RemoveFXLinkerMotion_Activate, RemoveFXLinkerMotion_Usernames },
    { (const char *)NULL }
  };
}

// Prototypes
void FindAndRemoveFXLink( LWLayoutGeneric *generic, LWItemInfo *iteminfo, LWItemID item );

// RemoveFXLinkerMotion_Activate():
XCALL_ (int) RemoveFXLinkerMotion_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  int mode = (*message->yesNoCan)( "Remove FX Linker Motion",
                                   "Remove FX_Link Motion Plug-in from all items (Yes)",
                                   "or from only the selected items (No)?" );
  if( mode == 0 )
    return AFUNC_OK;

  // Look for and remove FX_Link
  int j;
  if( mode == 1 ) {
    // Selected Items
    if( ui->selItems[0] == NULL )
      return AFUNC_OK;

    for( j=0; ui->selItems[j] != NULL; j++ )
      FindAndRemoveFXLink( generic, iteminfo, ui->selItems[j] );
  } else {
    // All Items
    LWItemType types[] = { LWI_OBJECT, LWI_LIGHT, LWI_CAMERA };
    LWItemID item, bone;
    for( j=0; j < 3; j++ ) {
      for( item = (*iteminfo->first)( types[j], NULL ); item != LWITEM_NULL; item = (*iteminfo->next)( item ) ) {
        FindAndRemoveFXLink( generic, iteminfo, item );

        for( bone = (*iteminfo->first)( LWI_BONE, item ); bone != LWITEM_NULL; bone = (*iteminfo->next)( bone ) )
          FindAndRemoveFXLink( generic, iteminfo, item );
      }
    }
  }

  // Reselect the items
  char buffer[ 256 ];
  sprintf( buffer, "SelectItem %x", ui->selItems[0] );
  (*generic->evaluate)( generic->data, buffer );

  for( j=1; ui->selItems[j] != NULL; j++ ) {
    sprintf( buffer, "AddToSelection %x", ui->selItems[j] );
    (*generic->evaluate)( generic->data, buffer );
  }

  return AFUNC_OK;
}

// FindAndRemoveFXLink():
void FindAndRemoveFXLink( LWLayoutGeneric *generic, LWItemInfo *iteminfo, LWItemID item) {
  // Look for FX_Link
  const char *server_name = (const char *)-1;
  for( int i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( item, LWITEMMOTION_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "FX_Link" ) == 0 ) {
      break;
    }
  }

  // Remove FX_Link if found
  if( server_name != NULL ) {
    char buffer[ 256 ];
    sprintf( buffer, "SelectItem %x", item );
    (*generic->evaluate)( generic->data, buffer );

    sprintf( buffer, "RemoveServer %s %d", LWITEMMOTION_HCLASS, i );
    (*generic->evaluate)( generic->data, buffer );
  }
}

