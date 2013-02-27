//
// ParticleCloud.cpp
//

#include "particlecloud.h"
#include "ParticleCloud-Interface.h"

#include <portable/dynarray/dynarray.h>

#include <stdio.h>

#include <lightwave/vecmath/vecmath.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#define CUSTOBJ_PLUGIN_NAME "TM-P_ParticleCloud" DEBUG_PLUGIN_PLUGNAME

extern "C" {
  // User Names
  ServerUserName ParticleCloud_Usernames[]             = { {"TM-P  Particle Cloud"                   DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Particle Cloud"                  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ParticleCloud_Apply_Usernames[]       = { {"TM-P  Particle Cloud Apply/Properties"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Particle Cloud Apply/Properties" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ParticleCloud_Properties_Usernames[]  = { {"TM-P  Particle Cloud Properties"        DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Particle Cloud Properties"       DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ParticleCloud_SaveAll_Usernames[]     = { {"TM-P  Particle Cloud Save All"          DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Particle Cloud Save All"         DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ParticleCloud_Capture_Usernames[]     = { {"TM-P  Particle Cloud Start Capture"     DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Particle Cloud Start Capture"    DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWCUSTOMOBJ_HCLASS,    CUSTOBJ_PLUGIN_NAME,                                 ParticleCloud_Activate,            ParticleCloud_Usernames            },
    { LWCUSTOMOBJ_ICLASS,    CUSTOBJ_PLUGIN_NAME,                                 ParticleCloud_Interface_Activate,  ParticleCloud_Usernames            },
/*
    { LWLAYOUTGENERIC_CLASS, "TM-P_ParticleCloudApply"     DEBUG_PLUGIN_PLUGNAME, ParticleCloud_Apply_Activate,      ParticleCloud_Apply_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ParticleCloudPropeties" DEBUG_PLUGIN_PLUGNAME, ParticleCloud_Properties_Activate, ParticleCloud_Properties_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ParticleCloudSaveAll"   DEBUG_PLUGIN_PLUGNAME, ParticleCloud_SaveAll_Activate,    ParticleCloud_SaveAll_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ParticleCloudCapture"   DEBUG_PLUGIN_PLUGNAME, ParticleCloud_Capture_Activate,    ParticleCloud_Capture_Usernames    },
*/
    { (const char *)NULL }
  };
}

DynArray< ParticleCloud_Instance * > instances;

// ParticleCloud_Activate():
XCALL_ (int) ParticleCloud_Activate( long version, GlobalFunc *global,
                                     void *_local, void *serverData ) {

  if( version != LWCUSTOMOBJ_VERSION )
    return(AFUNC_BADVERSION);

  LWCustomObjHandler *local = (LWCustomObjHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = ParticleCloud_Create;
    local->inst->destroy    = ParticleCloud_Destroy;
    local->inst->load       = ParticleCloud_Load;
    local->inst->save       = ParticleCloud_Save;
    local->inst->copy       = ParticleCloud_Copy;
    local->inst->descln     = ParticleCloud_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = ParticleCloud_UseItems;
    local->item->changeID   = ParticleCloud_ChangeID;
  }

  if( local->rend != NULL ) {
    local->rend->init       = ParticleCloud_Init;
    local->rend->cleanup    = ParticleCloud_CleanUp;
    local->rend->newTime    = ParticleCloud_NewTime;
  }

  local->evaluate           = ParticleCloud_Evaluate;
  local->flags              = ParticleCloud_Flags;

  ParticleCloud_Instance::global = global;

  return AFUNC_OK;
}

// ParticleCloud_Create():
LWInstance ParticleCloud_Create( void *data, void *context, LWError *error ) {
  ParticleCloud_Instance *inst = new ParticleCloud_Instance( context );
  instances.Add( inst );
  return inst;
}

// ParticleCloud_Copy():
LWError ParticleCloud_Copy( LWInstance _to, LWInstance _from ) {
  ParticleCloud_Instance *to   = (ParticleCloud_Instance *)_to;
  ParticleCloud_Instance *from = (ParticleCloud_Instance *)_from;

  *to = *from;
  return NULL;
}

// ParticleCloud_Destroy():
void ParticleCloud_Destroy( LWInstance _inst ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;
  instances.Remove( instances.FindIndexOf( inst ) );
  delete inst;
}

// ParticleCloud_Load():
LWError ParticleCloud_Load( LWInstance _inst, const LWLoadState *loader ) {
  return NULL;
}

// ParticleCloud_Save():
LWError ParticleCloud_Save( LWInstance _inst, const LWSaveState *saver ) {
  return NULL;
}

// ParticleCloud_Describe():
const char * ParticleCloud_Describe( LWInstance _inst ) {
  return "Particle Cloud";
}

// ParticleCloud_Evaluate():
void ParticleCloud_Evaluate( LWInstance _inst, const LWCustomObjAccess *access ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;

  LWObjectInfo *object_info = (LWObjectInfo *)inst->global( LWOBJECTINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo   *item_info   = (LWItemInfo   *)inst->global( LWITEMINFO_GLOBAL,   GFUSE_TRANSIENT );

  LWMeshInfoID mesh = object_info->meshInfo( inst->GetItems()[0], (inst->GetUseFrozen() ? 1 : 0) );
  ParticleCloud_Point::mesh = mesh;

  // Get the point positions
  DynArray< ParticleCloud_Point * > points;
  mesh->scanPoints( mesh, PointScan_GetPositions, &points );

  // Get the item's transformation matrix
  LWDVector item_position, item_right, item_up, item_forward;
  LWFMatrix4 M;

  item_info->param( inst->GetItems()[0], LWIP_POSITION, inst->GetTime(), item_position );
  item_info->param( inst->GetItems()[0], LWIP_RIGHT,    inst->GetTime(), item_right );
  item_info->param( inst->GetItems()[0], LWIP_UP,       inst->GetTime(), item_up );
  item_info->param( inst->GetItems()[0], LWIP_FORWARD,  inst->GetTime(), item_forward );

  unsigned long i;
  for ( i = 0; i < 3; i++ ) {
    M[ 0 ][ i ] = ( float ) item_right[ i ];
    M[ 1 ][ i ] = ( float ) item_up[ i ];
    M[ 2 ][ i ] = ( float ) item_forward[ i ];
    M[ 3 ][ i ] = ( float ) item_position[ i ];
    M[ i ][ 3 ] = 0.0f;
  }
  M[ 3 ][ 3 ] = 1.0f;

  // Display each point
  LWFVector local, world;
  double position[3];
  char   buffer[10];

  float color[4] = { 0.75, 1.0, 0.75, 1.0 };
  access->setColor( access->dispData, color );

  for( i=0; i < points.NumElements(); i++ ) {
    // Convert from local to world space
    local[0] = points[i]->position[0];
    local[1] = points[i]->position[1];
    local[2] = points[i]->position[2];
    LWVecMath::transformp( local, M, world );

    // Convert to doubles
    position[0] = world[0];
    position[1] = world[1];
    position[2] = world[2];

    // Draw the point
    access->point( access->dispData, position, LWCSYS_WORLD );

    // Draw the point index
    if( inst->GetShowIndices() ) {
      sprintf( buffer, "%d", i );
      access->text( access->dispData, position, buffer, 0, LWCSYS_WORLD );
    }
  }
}

LWMeshInfoID ParticleCloud_Point::mesh = NULL;

// PointScan_GetPositions():
int PointScan_GetPositions( void *data, LWPntID id ) {
  DynArray< ParticleCloud_Point * > &points = *(DynArray< ParticleCloud_Point * > *) data;
  ParticleCloud_Point *point = new ParticleCloud_Point;

  point->mesh->pntOtherPos( point->mesh, id, point->position );
  points.Add( point );

  return 0;
}

// ParticleCloud_Flags():
unsigned int ParticleCloud_Flags( LWInstance _inst ) {
  return 0;
}

// ParticleCloud_UseItems():
const LWItemID * ParticleCloud_UseItems( LWInstance _inst ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;
  return inst->GetItems();
}

// ParticleCloud_ChangeID():
void ParticleCloud_ChangeID( LWInstance _inst, const LWItemID *ids ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;

  int i, j;
  for( j=0; j != (int)LWITEM_NULL; j++ ) {
    for( i=0; i != (int)LWITEM_NULL; i+=2 ) {
      if( ids[i] == inst->GetItems()[i] ) {
        inst->GetItems()[i] = ids[i];
        break;
      }
    }
  }
}

// ParticleCloud_Init():
LWError ParticleCloud_Init( LWInstance, int) {
  return NULL;
}

// ParticleCloud_CleanUp():
void ParticleCloud_CleanUp( LWInstance) {
  ;
}

// ParticleCloud_NewTime():
LWError ParticleCloud_NewTime( LWInstance _inst, LWFrame frame, LWTime time ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;
  inst->SetNow( frame, time );
  
  return NULL;
}

// ParticleCloud_Interface_Activate():
XCALL_ (int) ParticleCloud_Interface_Activate( long version, GlobalFunc *global,
                                               void *_local, void *serverData ) {
  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  // Get XPanels
  LWXPanelFuncs *xpanels = (LWXPanelFuncs *)global( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  if( xpanels == NULL )
    return AFUNC_BADGLOBAL;

  // Create the Panel
  LWXPanelID panel = (*xpanels->create)( LWXP_VIEW, pc_xpanel_controls );
  if( panel != NULL ) {
    (*xpanels->hint)(     panel, 0, pc_xpanel_hint );
    (*xpanels->describe)( panel, pc_xpanel_data, ParticleCloud_Get, ParticleCloud_Set );
    (*xpanels->viewInst)( panel, local->inst );
    (*xpanels->setData)(  panel, 0, local->inst );
  }

  // Fill in the local data
  local->panel   = panel;
  local->options = NULL;
  local->command = NULL;

  return AFUNC_OK;
}

