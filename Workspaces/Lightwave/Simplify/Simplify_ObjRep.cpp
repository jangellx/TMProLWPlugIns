//
// Simplify_ObjRep.cpp
//
// Simplify is an Object Replacement Plug-In that replaces
//  objects based on the distance to camera (LOD), as well as
//  allowing for low polygon interactive models for animation
//  purposes.
//


#include <stdio.h>

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "Simplify_ObjRep.h"
#include "Simplify_ObjRep_Instance.h"
#include "Simplify_ObjRep_Interface.h"
#include "Simplify_Config.h"
#include "Simplify_Generic.h"
#include "SimplifyLogo.h"

// Function Declarations
extern "C" {
  XCALL_ (int) Simplify_Activate(  long version, GlobalFunc *global,
                                   ObjReplacementHandler *local, void *serverData );
  XCALL_ (int) Simplify_Interface( long version, GlobalFunc *global,
                                   void *local, void *serverData );
  XCALL_ (LWInstance)          SimplifyCreate(       LWError	 *err, LWItemID item );
  XCALL_ (void)                SimplifyDestroy(      LWInstance inst);
  XCALL_ (LWError)             SimplifyCopy(         LWInstance from, LWInstance to, LWItemID item );
  XCALL_ (LWError)             SimplifyLoad(         LWInstance inst, const LWLoadState  *lState);
  XCALL_ (LWError)             SimplifySave(         LWInstance inst, const LWSaveState  *sState );
  XCALL_ (static void)         SimplifyEvaluate(     LWInstance inst, ObjReplacementAccess *objrep_access );
  XCALL_ (const char *)        SimplifyDescribeLine( LWInstance inst );
  XCALL_ (const LWItemID *)    SimplifyUseItems(     LWInstance inst );
  XCALL_ (void)                SimplifyChangeID(     LWInstance inst, const LWItemID *items );
}

// Globals
DynArray< SimplifyInstance * >   instances;
SimplifyConfig                   simp_config;
SimplifyGlobal                   simp_global;
SimplifyLogo                     logo;
char                             error_buffer[1024];

extern int Simplify_Version;
extern int Simplify_Revision;

// Simplify_Activate():
//  Simplify Object Replacement Plug-In's Activate Function
XCALL_ (int) Simplify_Activate(  long version, GlobalFunc *global,
                                 ObjReplacementHandler *local, void *serverData ) {

  // Version Check
  if( version != 3 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;

  // Fill in the local data
	local->create   = SimplifyCreate;
	local->destroy  = SimplifyDestroy;
	local->load     = SimplifyLoad;
	local->save     = SimplifySave;
	local->copy     = SimplifyCopy;

	local->evaluate = SimplifyEvaluate;
	local->descln   = SimplifyDescribeLine;

  local->useItems = SimplifyUseItems;
  local->changeID = SimplifyChangeID;

  // Get the Camera ID
  if( SimplifyInstance::GetCameraID() == NULL ) {
    LWItemInfo *info = (LWItemInfo *) ( (*global)("LW Item Info", GFUSE_TRANSIENT) );
    SimplifyInstance::SetCameraID( (*info->first)( LWI_CAMERA, NULL ) );
  }

  // Set the Global Functions
  if( SimplifyInstance::GetGlobal() == NULL )
    SimplifyInstance::SetGlobal( global );

  // Load the default config file
  if( !simp_config.WasLoaded() ) {
    DirInfoFunc * dir_func = (DirInfoFunc *) ( (*global)("Directory Info", GFUSE_TRANSIENT) );
    char def_config[ MAX_PATH_LENGTH ];

    // First try the Install Path
    strcpy( def_config, (*dir_func)( "Install" ) );
    DirStrings::AddPathPart( def_config, "Simplify.cfg" );

    if( !DirInfo::Exists( def_config ) ) {
      // Then the Programs Dir
      strcpy( def_config, (*dir_func)( "Install" ) );
      DirStrings::AddPathPart( def_config, "Programs/Simplify.cfg" );

      // Then the Alternate Settings Dir
      if( !DirInfo::Exists( def_config ) ) {
        strcpy( def_config, (*dir_func)( "Settings" ) );
        DirStrings::AddPathPart( def_config, "Simplify.cfg" );

        // Then the Plug-ins Dir
        if( !DirInfo::Exists( def_config ) ) {
          const char * temp = (*dir_func)( "Plugins" );
          if( temp != NULL ) {
            strcpy( def_config, temp );
            DirStrings::AddPathPart( def_config, "Simplify.cfg" );

            // And finally, the Layout Plug-ins Dir
            if( !DirInfo::Exists( def_config ) ) {
              strcpy( def_config, temp );
              DirStrings::AddPathPart( def_config, "Layout/Simplify.cfg" );
            }
          }
        }
      }
    }

    CMessage * message = CMessage::New( (MessageFuncs *) ( (*global)("Info Messages", GFUSE_TRANSIENT) ) );
#ifdef _DEBUG
    message->Info( "Config File Path:", def_config );
#endif
  
    switch( simp_config.Load( def_config ) ) {
    case SIMPCONFIG_OK:                          // Config loaded OK
    case SIMPCONFIG_NOT_FOUND:                   // This is OK; just means use the default settings
      break;

    case SIMPCONFIG_ERROR_OPENING_FILE:          // Error; say so
      message->Error( "Simplify Error:  Error occurred opening the Simplify Config File",
                      def_config );
      break;

    case SIMPCONFIG_ERROR_READING_FROM_FILE:     // Another error; say so
      message->Error( "Simplify Error:  Error occurred reading the Simplify Config File",
                      def_config );
      break;

    case SIMPCONFIG_ERROR_NOT_SIMP_CONFIG:       // Not a config; report an error
      message->Error( "Simplify Error:  This is not a Simplify Config File:",
                      def_config );
      break;
    };

    delete message;
  }

  return AFUNC_OK;
}

// SimplifyEvaluate()
//  This is what actually does the object replacement.
XCALL_ (void) SimplifyEvaluate( LWInstance inst,
                                ObjReplacementAccess *objrep_access ) {
  // Let the SimplifyInstance deal with it.
  ((SimplifyInstance *)(inst))->EvaluateReplacement( objrep_access );
}

// SimplifyCreate()
XCALL_ (LWInstance) SimplifyCreate( LWError	*err, LWItemID item ) {
  SimplifyInstance *inst = new SimplifyInstance;

  // Make sure the instance was allocated OK
  if( inst == NULL ) {
    const char *string = "Simplify Error:  Unable to create instance!";
    err = &string;
    return NULL;
  }

  // Get the Object Info Functions so we can get the object's filename
  CLWObjectInfo * obj_info = CLWObjectInfo::New( (LWObjectInfo *) (*SimplifyInstance::GetGlobal())("LW Object Info", GFUSE_TRANSIENT ) );
  CMessage * message = CMessage::New( (MessageFuncs *) (*SimplifyInstance::GetGlobal())("Info Messages", GFUSE_TRANSIENT) );

  if( !obj_info ) {
    message->Error( "Simplify Error:  Unable to get LW Object Info!" );
    return false;
  }

  // Set the Defauit Nominal Camera Zoom, in case the auto-load fails
  LWItemInfo *iteminfo = (LWItemInfo *)(*SimplifyInstance::GetGlobal())( "LW Item Info", GFUSE_TRANSIENT );
  LWItemID cam_id = (*iteminfo->first)( LWI_CAMERA, NULL );
  LWCameraInfo *camerainfo = (LWCameraInfo *)(*SimplifyInstance::GetGlobal())( "LW Camera Info", GFUSE_TRANSIENT );

  double default_zoom_factor = (*camerainfo->zoomFactor)( cam_id, 0 );
  inst->SetLODNominalCameraZoom( (float)default_zoom_factor );

  // See if an Auto-Load SDF exists
  char def_sdf[ MAX_PATH_LENGTH ];
  if( simp_global.autoload ) {
    // Get DirFuncs so we can get the Content Dir
    DirInfoFunc * dir_func = (DirInfoFunc *) (*SimplifyInstance::GetGlobal())("Directory Info", GFUSE_TRANSIENT );
    strcpy( def_sdf, obj_info->Filename( item ) );
    DirStrings::ChangeExtension( def_sdf, ".SDF" );
    inst->SetSDF( def_sdf );

    strcpy( def_sdf, (*dir_func)( "Content" ) );
    DirStrings::AddPathPart( def_sdf, inst->GetSDF() );

    if( DirInfo::Exists( def_sdf ) ) {
      char * error;
      if( !inst->LoadFromFile( def_sdf, error ) ) {
        // Error loading default instance; create a clean one
        delete inst;
        inst = new SimplifyInstance;
        message->Error( error_buffer );
        message->Error( "Simplify Error: Unable to auto-load default instance data for ", obj_info->Filename( inst->GetItemID() ) );
      }

      delete message;
    } else if( simp_config.GetDefaultSDF() != NULL ) {
      // Default SDF not found; load the default one from the user config, if it exists
      if( DirInfo::Exists( simp_config.GetDefaultSDF() ) ) {
        char * error;
        if( !inst->LoadFromFile( simp_config.GetDefaultSDF(), error ) ) {
          // Error loading default instance; create a clean one
          delete inst;
          inst = new SimplifyInstance;
          message->Error( error_buffer );
          message->Error( "Simplify Error: Unable to auto-load deafult instance data from",
                          simp_config.GetDefaultSDF() );
        }

        delete message;
      }
    }
  }

  // Set the item used by the instance
  inst->SetItemID( item );

  // Set the Render Filename to the object's filename, if needed
  if( inst->GetRender()[0] == '\0' )
    inst->SetRender( obj_info->Filename( inst->GetItemID() ) );

  // Free obj_info
  delete obj_info;

  // Instance OK; return it
  instances.Add( inst );
  err = NULL;
  return inst;
}

// SimplifyDestroy()
XCALL_ (void) SimplifyDestroy( LWInstance inst ) {
  instances.Remove( instances.FindIndexOf( (SimplifyInstance *)inst ) );
  delete inst;
}

// SimplifyCopy()
XCALL_ (LWError) SimplifyCopy( LWInstance from, LWInstance to,
                               LWItemID item ) {

  ((SimplifyInstance *)to)->CopyFrom(*((SimplifyInstance *)from));
  ((SimplifyInstance *)to)->SetItemID( item );

  return NULL;
}

// SimplifyLoad()
XCALL_ (LWError) SimplifyLoad( LWInstance inst,
                               const LWLoadState *lState) {
  // Check the save mode
  if( lState->ioMode == LWIO_OBJECT )
    return "Simplify Error: Simplify Object Replacement Plug-In can only load data from scene files!";

  // Load the data
  CLWLoadState * loader = CLWLoadState::New( lState );
  bool retval = ((SimplifyInstance *)inst)->LoadFromScene( error_buffer, loader );
  delete loader;

  // Return an error, if applicable
  if( retval )
    return NULL;
  else
    return error_buffer;
}

// SimplifySave()
XCALL_ (LWError) SimplifySave( LWInstance inst,
                               const LWSaveState *sState ) {
  // Check the save mode
  if( sState->ioMode == LWIO_OBJECT )
    return "Simplify Error: Simplify Object Replacement Plug-In can only save data into scene files!";

  // Save the data
  CLWSaveState * saver = CLWSaveState::New( sState );
  bool retval = ((SimplifyInstance *)inst)->SaveToScene( error_buffer, saver );
  delete saver;

  // Return an error, if applicable
  if( retval )
    return NULL;
  else
    return error_buffer;
}

// SimplifyDescribeLine()
XCALL_ (const char *) SimplifyDescribeLine( LWInstance inst ) {
  static char descline[ 256 ];
  if( ( !((SimplifyInstance *)inst)->GetSavedState() ) &&
      (  ((SimplifyInstance *)inst)->GetStoreAs() == 0 ) ) {
    sprintf( descline, "TM Productions' Simplify V%d.%d ( - UNSAVED - )", Simplify_Version, Simplify_Revision );
  } else {
    if( ((SimplifyInstance *)inst)->GetEnabled() ) {
      if( ((SimplifyInstance *)inst)->GetEnablePreview() ) {
        if( ((SimplifyInstance *)inst)->GetEnableLOD() )
          sprintf( descline, "TM Productions' Simplify V%d.%d (Preview/LOD Mode)", Simplify_Version, Simplify_Revision );
        else
          sprintf( descline, "TM Productions' Simplify V%d.%d (Preview/Render Mode)", Simplify_Version, Simplify_Revision );
      } else {
        if( ((SimplifyInstance *)inst)->GetEnableLOD() )
          sprintf( descline, "TM Productions' Simplify V%d.%d (LOD Mode Only)", Simplify_Version, Simplify_Revision );
        else
          sprintf( descline, "TM Productions' Simplify V%d.%d (Render Mode Only)", Simplify_Version, Simplify_Revision );
      }
    } else
      sprintf( descline, "TM Productions' Simplify V%d.%d (-disabled-)", Simplify_Version, Simplify_Revision );
  }

  return descline;
}

// SimplifyUseItems()
XCALL_ (const LWItemID *) SimplifyUseItems( LWInstance inst ) {
  return ((SimplifyInstance *)inst)->GetItemList();
}

// SimplifyChangeID()
XCALL_ (void) SimplifyChangeID( LWInstance inst, const LWItemID *items ) {
  ((SimplifyInstance *)inst)->ChangeItemIDs( items );
}


// Simplify_Interface():
//  Simplify Object Replacement Plug-In's Interface Function
XCALL_ (int) Simplify_Interface( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {

  SimplifyInstance *si = (SimplifyInstance *) local;

  // Version Check
  if( version != 1 )
    return( AFUNC_BADVERSION );

  XCALL_INIT;      // Required LW Plug-In Initialization Call

  int retval;
  SimplifyInterface Panel( global, si, retval );

  return retval;
}
