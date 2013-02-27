//
// SpaceBall.cpp
//

#include "spaceball-interface.h"
#include <portable/pfstream/pfstream.h>

#include <stdio.h>
#include <string.h>

extern "C" {
  // User Names
  ServerUserName SpaceBall_Usernames[]           = { {"TM-P  SpaceBall (Master)"    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall (Master)"    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallProperties_Usernames[] = { {"TM-P  SpaceBall Properties"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Properties"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallApply_Usernames[]      = { {"TM-P  SpaceBall Apply"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Apply"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallEnable_Usernames[]     = { {"TM-P  SpaceBall Enable"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Enable"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallDisable_Usernames[]    = { {"TM-P  SpaceBall Disable"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Disable"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallToggle_Usernames[]     = { {"TM-P  SpaceBall Toggle"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Toggle"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallReset_Usernames[]      = { {"TM-P  SpaceBall Reset"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Reset"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallConfig_Usernames[]     = { {"TM-P  SpaceBall Config"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Config"      DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallMove_Usernames[]       = { {"TM-P  SpaceBall Move"        DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Move"        DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallScale_Usernames[]      = { {"TM-P  SpaceBall Scale"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Scale"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SpaceBallMoveScale_Usernames[]  = { {"TM-P  SpaceBall Move Toggle" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "SpaceBall Move Toggle" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWMASTER_HCLASS,  MASTER_PLUGIN_NAME,                                   SpaceBall_Activate,           SpaceBall_Usernames           },
    { LWMASTER_ICLASS,  MASTER_PLUGIN_NAME,                                   SpaceBall_Interface_Activate, SpaceBall_Usernames           },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallProperties"DEBUG_PLUGIN_PLUGNAME, SpaceBallProperties_Activate, SpaceBallProperties_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallApply"     DEBUG_PLUGIN_PLUGNAME, SpaceBallApply_Activate,      SpaceBallApply_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallEnable"    DEBUG_PLUGIN_PLUGNAME, SpaceBallEnable_Activate,     SpaceBallEnable_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallDisable"   DEBUG_PLUGIN_PLUGNAME, SpaceBallDisable_Activate,    SpaceBallDisable_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallToggle"    DEBUG_PLUGIN_PLUGNAME, SpaceBallToggle_Activate,     SpaceBallToggle_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallReset"     DEBUG_PLUGIN_PLUGNAME, SpaceBallReset_Activate,      SpaceBallReset_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallConfig"    DEBUG_PLUGIN_PLUGNAME, SpaceBallConfig_Activate,     SpaceBallConfig_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallMove"      DEBUG_PLUGIN_PLUGNAME, SpaceBallMove_Activate,       SpaceBallMove_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallScale"     DEBUG_PLUGIN_PLUGNAME, SpaceBallScale_Activate,      SpaceBallScale_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SpaceBallMoveScale" DEBUG_PLUGIN_PLUGNAME, SpaceBallMoveScale_Activate,  SpaceBallMoveScale_Usernames  },
    { (const char *)NULL }
  };
}

// Globals
GlobalFunc      *global           = NULL;
HostDisplayInfo *hdi              = NULL;
LWCommandFunc   *command          = NULL;
LWInstUpdate    *instance_update  = NULL;
LWMessageFuncs  *message          = NULL;

int              instance_count   = 0;
WNDPROC          orig_window_proc = NULL;

SiOpenData       oData;      // Platform-specific device data
SiGetEventData   eData;      // Platform-specific event data
SiHdl            hdl;        // Spaceball device handle
SiSpwEvent       siEvent;    // SpaceWare data event

char             command_buffer[512] = "";

bool             enable_spaceball       = true;
bool             is_safe                = true;
bool             move_rotate            = true;
bool             open_config_on_startup = true;
char           * function_map[ num_functions ];

SpaceBall_Interface *sb_int;

double           tscalar = 1000.0;
double           rscalar = 10.0;
double           sscalar = 1000.0;

// SpaceBall_Activate():
XCALL_ (int) SpaceBall_Activate( long version, GlobalFunc *_global,
                                 void *_local, void *serverData ) {

  if( version != LWMASTER_VERSION )
    return(AFUNC_BADVERSION);

  LWMasterHandler *local = (LWMasterHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = SpaceBall_Create;
    local->inst->destroy    = SpaceBall_Destroy;
    local->inst->load       = SpaceBall_Load;
    local->inst->save       = SpaceBall_Save;
    local->inst->copy       = SpaceBall_Copy;
    local->inst->descln     = SpaceBall_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = SpaceBall_UseItems;
    local->item->changeID   = SpaceBall_ChangeID;
  }

  local->event              = SpaceBall_Event;
  local->flags              = SpaceBall_Flags;

  global = _global;

  return AFUNC_OK;
}

// SpaceBall_End():
void SpaceBall_End() {
  // Release the globals
  global( "LW Command Interface",   GFUSE_RELEASE );
  global( LWINSTUPDATE_GLOBAL,      GFUSE_RELEASE );
  global( LWHOSTDISPLAYINFO_GLOBAL, GFUSE_RELEASE );
  global( LWMESSAGEFUNCS_GLOBAL,    GFUSE_RELEASE );

  // Shutdown the SpaceBall
  SiClose( hdl );
  SiTerminate();

  // Restore the Layout's window function
  SetWindowLong( hdi->window, GWL_WNDPROC, (long)orig_window_proc );
}

// SpaceBall_Create():
LWInstance SpaceBall_Create( void *data, void *context, LWError *error ) {
  // Check for LWSN; if so, exit now
  unsigned long sysid = ( unsigned long )global( LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT );
  if( (sysid & LWSYS_TYPEBITS) != LWSYS_LAYOUT )
    return &instance_count;

  if( instance_count > 0 ) {
    *error = "SpaceBall Error:  Only one instance of the driver can be activate at a time";
    return NULL;
  }

  instance_count++;

  // Get some globals
  command         = (LWCommandFunc   *)global( "LW Command Interface",   GFUSE_ACQUIRE );
  instance_update = (LWInstUpdate    *)global( LWINSTUPDATE_GLOBAL,      GFUSE_ACQUIRE );
  hdi             = (HostDisplayInfo *)global( LWHOSTDISPLAYINFO_GLOBAL, GFUSE_ACQUIRE );
  message         = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,    GFUSE_ACQUIRE );

  // Hijack Layout's window function
  orig_window_proc = (WNDPROC)SetWindowLong( hdi->window, GWL_WNDPROC, (long)SpaceBallWindowProc );

  // Load Settings
  LoadSettings();

  // Initialize the SpaceBall drivers
  SiInitialize();
  SiOpenWinInit (&oData, hdi->window);
  SiSetUiMode(hdl, (open_config_on_startup ? SI_UI_ALL_CONTROLS : SI_UI_NO_CONTROLS ) );
  hdl = SiOpen ("Lightwave 3D (Layout)", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData);
  if( hdl == SI_NO_HANDLE)  {
    SpaceBall_End();
    *error = "SpaceBall Error:  Couldn't open spaceball drivers";
    return NULL;
  }

  error = NULL;
  return &instance_count;
}

// SpaceBall_Destroy():
void SpaceBall_Destroy( LWInstance _inst ) {
  // Check for LWSN; if so, exit now
  unsigned long sysid = ( unsigned long )global( LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT );
  if( (sysid & LWSYS_TYPEBITS) != LWSYS_LAYOUT )
    return;

  instance_count--;
  if( instance_count < 1 ) {
    SpaceBall_End();
    SaveSettings();

    for( unsigned long i=0; i < num_functions; i++ ) {
      StringTools::strdup2( function_map[i], NULL );
      function_map[i] = NULL;
    }
  }
}

// SpaceBall_Copy():
LWError SpaceBall_Copy( LWInstance _to, LWInstance _from ) {
  return NULL;
}

// SpaceBall_Load():
LWError SpaceBall_Load( LWInstance _inst, const LWLoadState *loader ) {
  return NULL;
}

// SpaceBall_Save():
LWError SpaceBall_Save( LWInstance _inst, const LWSaveState *saver ) {
  return NULL;
}

// SpaceBall_Describe():
const char * SpaceBall_Describe( LWInstance _inst ) {
  return "SpaceBall Driver for Lightwave 3D";
}

// SpaceBall_Event():
double SpaceBall_Event( LWInstance _inst, const LWMasterAccess *access ) {
  if( access->eventCode == LWEVNT_COMMAND ) {
    if( strncmp( (const char *)access->eventData, "RenderFrame", 11 ) == 0 )
      is_safe = false;
    else if( strncmp( (const char *)access->eventData, "RenderScene", 11 ) == 0 )
      is_safe = false;
    else if( strncmp( (const char *)access->eventData, "RenderObject", 12 ) == 0 )
      is_safe = false;

    if( sb_int != NULL ) {
      if( sb_int->IsOpen() ) {
        int do_capture;
        GET_INT( sb_int->capture, do_capture );

        if( do_capture ) {
          if( (sb_int->current_function >= 0) && (sb_int->current_function < num_functions) ) {
            StringTools::strdup2( function_map[ sb_int->current_function ], (const char *)access->eventData );
            REDRAW_CON( sb_int->list );
            HandleList( sb_int->list, NULL );
          }
        }
      }
    }

  } else if ( access->eventCode == LWEVNT_RENDER_DONE ) {
    is_safe = true;
  }

  return 0.0;
}

// SpaceBall_Flags():
unsigned int SpaceBall_Flags( LWInstance _inst ) {
  return 0;
}

// SpaceBall_UseItems():
const LWItemID * SpaceBall_UseItems( LWInstance _inst ) {
  return NULL;
}

// SpaceBall_ChangeID():
void SpaceBall_ChangeID( LWInstance _inst, const LWItemID *ids ) {
  return;
}

// SpaceBall_Interface_Activate():
XCALL_ (int) SpaceBall_Interface_Activate( long version, GlobalFunc *_global,
                                         void *_local, void *serverData ) {
  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  local->panel   = NULL;
  local->options = SpaceBall_OpenUI;
  local->command = NULL;

  if( global == NULL )
    global = _global;

  return AFUNC_OK;
}

// SpaceBallProperties_Activate():
XCALL_ (int) SpaceBallProperties_Activate( long version, GlobalFunc *global,
                                           void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  // Apply the Master if it isn't already there
  if( instance_count == 0 ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer " LWMASTER_HCLASS " " MASTER_PLUGIN_NAME );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Open it's UI
  SpaceBall_OpenUI( NULL );

  return AFUNC_OK;
}

// SpaceBallApply_Activate():
XCALL_ (int) SpaceBallApply_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  // Apply the Master if it isn't already there
  if( instance_count == 0 ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer " LWMASTER_HCLASS " " MASTER_PLUGIN_NAME );
    (*generic->evaluate)( generic->data, buffer );
  }

  return AFUNC_OK;
}

// SpaceBall_OpenUI():
LWError SpaceBall_OpenUI( void *data ) {
  if( sb_int == NULL ) {
    new SpaceBall_Interface();

    if( !sb_int->CreationOK() ) {
      delete sb_int;
      sb_int = NULL;
      return NULL;
    }
  }

  if( !sb_int->IsOpen() )
    sb_int->OpenUI();

  return NULL;
}

// SpaceBallWindowProc():
LRESULT CALLBACK SpaceBallWindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
  SiGetEventWinInit (&eData, uMsg, wParam, lParam);
  if( SiGetEvent( hdl, 0, &eData, &siEvent ) == SI_IS_EVENT ) {
    if( enable_spaceball && is_safe ) {
      switch (siEvent.type) {
        case SI_MOTION_EVENT: {
          long *data = siEvent.u.spwData.mData;
          if( (data[SI_TX] != 0) || (data[SI_TY] != 0) || (data[SI_TZ] != 0) ) {
            if( move_rotate) {
              sprintf( command_buffer, "AddPosition %g %g %g", (double)data[ SI_TX ] / tscalar,
                                                               (double)data[ SI_TY ] / tscalar,
                                                               (double)data[ SI_TZ ] / tscalar );
            } else {
              sprintf( command_buffer, "AddScale %g %g %g", (double)data[ SI_TX ] / sscalar,
                                                            (double)data[ SI_TY ] / sscalar,
                                                            (double)data[ SI_TZ ] / sscalar );
            }

            command( command_buffer );
          }

          if( (data[SI_RX] != 0) || (data[SI_RY] != 0) || (data[SI_RZ] != 0) ) {
            sprintf( command_buffer, "AddRotation %g %g %g", (double)data[ SI_RY ] / -rscalar,
                                                             (double)data[ SI_RX ] / -rscalar,
                                                             (double)data[ SI_RZ ] / -rscalar );
            command( command_buffer );
          }
        } break;

        case SI_ZERO_EVENT:
        
          break;

        case SI_BUTTON_EVENT: {
          int index = SiButtonPressed( &siEvent );
          if( index == SI_NO_BUTTON )
            break;

//          if( index == SPW_ERROR )
//            break;

          if( (index < 1) && (index > 29) )
            break;

          index--;
          if( function_map[ index ] == NULL )
            break;

          command( function_map[ index ] );
//          command( "Generic_TM-P_SpaceBallToggle"DEBUG_PLUGIN_PLUGNAME );
        } break;

        default:
          break;
      }
    }
  }
    
  return CallWindowProc( orig_window_proc, hwnd, uMsg, wParam, lParam );
}

// SpaceBallEnable_Activate():
XCALL_ (int) SpaceBallEnable_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, true );
}

// SpaceBallDisable_Activate():
XCALL_ (int) SpaceBallDisable_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, false );
}

// SpaceBallToggle_Activate():
XCALL_ (int) SpaceBallToggle_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, !enable_spaceball );
}

// SpaceBallState():
XCALL_ (int) SpaceBallState( long version, GlobalFunc *global,
                             void *local, void *serverData, bool new_state ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( new_state == enable_spaceball )
    return AFUNC_OK;

  if( new_state == true ) {
    SiBeep( hdl, "aAaAc" );
  } else {
    SiBeep( hdl, "cAcAf" );
  }

  enable_spaceball = new_state;
  return AFUNC_OK;
}

// SpaceBallReset_Activate():
XCALL_ (int) SpaceBallReset_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  is_safe = true;

  return AFUNC_OK;
}

// SpaceBallConfig_Activate():
XCALL_ (int) SpaceBallConfig_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  SiSetUiMode(hdl, SI_UI_ALL_CONTROLS);

  return AFUNC_OK;
}

// SpaceBallMove_Activate():
XCALL_ (int) SpaceBallMove_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, true );
}

// SpaceBallScale_Activate():
XCALL_ (int) SpaceBallScale_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, false );
}

// SpaceBallMoveScale_Activate():
XCALL_ (int) SpaceBallMoveScale_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  return SpaceBallState( version, global, local, serverData, !move_rotate );
}

// SpaceBallMoveState():
XCALL_ (int) SpaceBallMoveState( long version, GlobalFunc *global,
                                 void *local, void *serverData, bool new_state ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  move_rotate = new_state;
  return AFUNC_OK;
}


// LoadSettings():
void LoadSettings() {
  LWDirInfoFunc  *dirfunc = (LWDirInfoFunc  *)global( LWDIRINFOFUNC_GLOBAL,  GFUSE_TRANSIENT );

  memset( function_map, 0, sizeof(const char *) * num_functions );
  open_config_on_startup = true;

  // Open the file.  Something bad will probably happen if dirfunc() returns NULL...
  char buffer[ 8192 ] = "";
  if( dirfunc( LWFTYPE_SETTING ) != NULL )
    strcpy( buffer, dirfunc( LWFTYPE_SETTING ) );

  strcat( buffer, "\\LWSpaceball.cfg" );

  pifstream in( buffer );
  if( !in )
    return;

  in.GuessEOLType();

  // Check the header
  in >> buffer;
  if( strcmp( buffer, "SBAL" ) != 0 )
    return;

  int index;
  in >> index;

  if( index != 1 )
    return;

  
  // Scan for keywords
  while( true ) {
    in >> buffer;
    if( (in.bad() || in.fail()) && !in.eof() )
      break;

    if( strcmp( buffer, "OpenConfigOnStartup" ) == 0 ) {
      in >> index;
      open_config_on_startup = (index == 1);

    } else if( strcmp( buffer, "Function" ) == 0 ) {
      in >> index;
      in.getline( buffer, 8192 );
      StringTools::strdup2( function_map[ index ], buffer+1 );
    }
    if( in.eof() )
      break;
  }
}

// SaveSettings():
void SaveSettings() {
  LWDirInfoFunc  *dirfunc = (LWDirInfoFunc  *)global( LWDIRINFOFUNC_GLOBAL,  GFUSE_TRANSIENT );

  // Open the file.  Something bad will probably happen if dirfunc() returns NULL...
  char buffer[ 512 ] = "";
  if( dirfunc( LWFTYPE_SETTING ) != NULL )
    strcpy( buffer, dirfunc( LWFTYPE_SETTING ) );

  strcat( buffer, "\\LWSpaceball.cfg" );

  pofstream out( buffer );
  if( !out )
    return;
  
  out << "SBAL 1" << out.endl();
  out << "OpenConfigOnStartup " << open_config_on_startup << out.endl();

  for( int i=0; i < num_functions; i++ ) {
    if( function_map[i] != NULL )
      out << "Function " << i << " " << function_map[i] << out.endl();
  }
}

