//
// AssignBoneVMaps-Interface.cpp
//

#include <stdio.h>

#include "AssignBoneVMaps-Interface.h"

bool AssignBoneVMaps_Interface::adjust_bone_settings = false;
bool AssignBoneVMaps_Interface::vmap_only            = false;
bool AssignBoneVMaps_Interface::normalize            = false;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "AssignBoneVMaps",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Bone Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2000-2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "AssignBoneVMaps,",
  "ActivateBones, DeactivateBones, ToggleActiveBones,",
  "RecordBonePivots, RestBones",
  NULL };

AssignBoneVMaps_Interface *abvui = NULL;

// Constructor():
AssignBoneVMaps_Interface::AssignBoneVMaps_Interface( GlobalFunc *_global, void *local )
  : global(*_global), retval(AFUNC_OK), is_ok(true), user_abort(false), panel(NULL),
    sel_type(-1) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  iteminfo    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  boneinfo    = (LWBoneInfo      *)global( LWBONEINFO_GLOBAL,      GFUSE_TRANSIENT );
  objectinfo  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );

  if( ui->selItems[0] != NULL )
    sel_type = (iteminfo->type)( ui->selItems[0] );

  assert( abvui == NULL );
  abvui = this;
}

// Destructor():
AssignBoneVMaps_Interface::~AssignBoneVMaps_Interface() {
  if( panel != NULL )
    (*xpanels->destroy)( panel );

  abvui = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool AssignBoneVMaps_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*xpanels->destroy)( panel );
    panel = NULL;
  }

  // Create the control array
  static LWXPanelControl controls[] = {
    { ABVUI_ABOUT,                "About",                "vButton"  },
    { ABVUI_ADJUST_BONE_SETTINGS, "Adjust Bone Settings", "iBoolean" },
    { ABVUI_USE_WEIGHT_MAP_ONLY,  "Use Weight Map Only",  "iBoolean" },
    { ABVUI_WEIGHT_NORMALIZATION, "Weight Normalization", "iBoolean" },
    { 0 } };

  // Create the data array
  static LWXPanelDataDesc data[] = {
    { ABVUI_ABOUT,                "About",                 ""        },
    { ABVUI_ADJUST_BONE_SETTINGS, "Adjust Bone Settings",  "integer" },
    { ABVUI_USE_WEIGHT_MAP_ONLY,  "Use Weight Map Only",   "integer" },
    { ABVUI_WEIGHT_NORMALIZATION, "Weight Normalization",  "integer" },
    { 0 } };

  // Panel Hints Array
  char title_string[256];
  sprintf( title_string, "Assign Bone VMaps -- %c2000 Joe Angell, TM Productions", 169 );
  static LWXPanelHint panel_hints[] = {
    XpLABEL( 0, title_string ),

    XpGROUP_( ABVUI_GROUP_BONE_SETTINGS_TOGGLE ),
      XpH( ABVUI_ADJUST_BONE_SETTINGS ),
      XpH( 0 ),
    XpDIVREM( ABVUI_GROUP_BONE_SETTINGS_TOGGLE ),

    XpGROUP_( ABVUI_GROUP_BONE_SETTINGS ),
      XpH( ABVUI_USE_WEIGHT_MAP_ONLY ),
      XpH( ABVUI_WEIGHT_NORMALIZATION ),
      XpH( 0 ),

    XpENABLE_( ABVUI_ADJUST_BONE_SETTINGS ),
      XpH( ABVUI_USE_WEIGHT_MAP_ONLY ),
      XpH( ABVUI_WEIGHT_NORMALIZATION ),
      XpH( 0 ),

    XpENABLE_( ABVUI_USE_WEIGHT_MAP_ONLY ),
      XpH( ABVUI_WEIGHT_NORMALIZATION ),
      XpH( 0 ),

    XpBUTNOTIFY( ABVUI_ABOUT, HandleAbout ),

    XpEND };

  // Get Xpanels
  xpanels = (LWXPanelFuncs *)global( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  if( xpanels == NULL ) {
    retval = AFUNC_BADGLOBAL;
    return false;
  }

  // Create the Panel
  panel = (*xpanels->create)( LWXP_FORM, controls );
  if( panel == NULL ) {
    (*message->error)( "AssignBoneVMaps Error: Unable to create XPanels interface", NULL );
    is_ok = false;
    return false;
  }

  // Set up other panel data
  (*xpanels->describe)( panel, data, NULL, NULL ); // 0 == NULL
  (*xpanels->hint)( panel, 0,                          panel_hints       );

  // Set up initial values for the panel
  FSETINT( xpanels, panel, ABVUI_ADJUST_BONE_SETTINGS, adjust_bone_settings );
  FSETINT( xpanels, panel, ABVUI_USE_WEIGHT_MAP_ONLY,  vmap_only );
  FSETINT( xpanels, panel, ABVUI_WEIGHT_NORMALIZATION, normalize );

  // Open the panel
  if( (*xpanels->post)( panel ) != 1 ) { // User Cancel
    user_abort = true;
    return false;
  }

  return IsOK();
}

// HandleAbout():
void HandleAbout( LWXPanelID pan, int cid ) {
  LWAboutPanel_TMPro( abvui->panel_funcs, "AssignBoneVMaps", about_text, NULL, true );
}

// BuildItemList():
bool AssignBoneVMaps_Interface::BuildItemList() {
  if( ui->selItems[0] == NULL )
    return false;

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  return true;
}

// ObtainUIStates()
bool AssignBoneVMaps_Interface::ObtainUIStates() {
  if( panel == NULL ) 
    return false;

  adjust_bone_settings = (FGETINT( xpanels, panel, ABVUI_ADJUST_BONE_SETTINGS ) > 0) ? true : false;
  vmap_only            = (FGETINT( xpanels, panel, ABVUI_USE_WEIGHT_MAP_ONLY  ) > 0) ? true : false;
  normalize            = (FGETINT( xpanels, panel, ABVUI_WEIGHT_NORMALIZATION ) > 0) ? true : false;

  return true;
}

// TestInputs():
bool AssignBoneVMaps_Interface::TestInputs( ProcessMode process_mode ) {
  // Makes sure at least one item is selected
  if( ui->selItems[0] == NULL ) {
    char buffer[512];
    sprintf( buffer, "%s Error: At least one object or bone must be selected", ProcessModeStrings[ process_mode ] );
    (*message->error)( buffer, NULL );
    return false;
  }

  // Make sure that item is a bone or object
  if( (sel_type != LWI_OBJECT) && (sel_type != LWI_BONE) ) {
    char buffer[512];
    sprintf( buffer, "%s Error: At least one object or bone must be selected", ProcessModeStrings[ process_mode ] );
    (*message->error)( buffer, NULL );
    return false;
  }

  return true;
}

// Process():
//  Actually processes the bones.
bool AssignBoneVMaps_Interface::Process( ProcessMode process_mode ) {
  // Process the bones
  char buffer[1024];
  unsigned long i;
  LWItemID current_bone;

  if( sel_type == LWI_OBJECT ) {  // Bones in objects
    for( i=0; i < items.NumElements(); i++ ) {
      // Step 1:  Select the first object
      sprintf( buffer, "SelectItem %x", items[i] );
      (*generic->evaluate)( generic->data, buffer );

      // Step 2:  Get the bone list for the item
      current_bone = (*iteminfo->first)( LWI_BONE, items[i] );

      // Step 3:  Cycle through and process each bone
      if( current_bone != NULL ) {
        (*generic->evaluate)( generic->data, "EditBones" );

        do {
          ProcessBone( process_mode, current_bone );
          current_bone = (*iteminfo->next)( current_bone );
        } while( current_bone != LWITEM_NULL );
      }
      (*generic->evaluate)( generic->data, "EditObjects" );
    }
  } else {                        // Selected bones
    for( i=0; i < items.NumElements(); i++ )
      ProcessBone( process_mode, items[i] );
  }

  // Reselect the previously selected items
  ReselectItems();

  return true;
}

// ProcessBone():
//  Performs changes on the bone ID provided
bool AssignBoneVMaps_Interface::ProcessBone( ProcessMode process_mode, LWItemID id ) {
  char buffer[100];
  int flags;

  // Step 1:  Select the bone
  sprintf( buffer, "SelectItem %x", id );
  (*generic->evaluate)( generic->data, buffer );

  if( process_mode == ABV_PROCESS_VMAPS ) {
    const char *name;

    // VMap Step 2:  Assign the bone's weight map
    name = (iteminfo->name)( id );
    sprintf( buffer, "BoneWeightMapName %s", name );
    (*generic->evaluate)( generic->data, buffer );

    // VMap Step 3:  Toggle optional settings
    if( adjust_bone_settings ) {
      flags = (boneinfo->flags)( id );
      if( vmap_only != ((flags & LWBONEF_WEIGHT_MAP_ONLY) ? true : false) ) {
        (*generic->evaluate)( generic->data, "BoneWeightMapOnly" );

        // Note:  Normalize is only valid if vmap_only is true
        if( normalize != ((flags & LWBONEF_WEIGHT_NORM) ? true : false) )
          (*generic->evaluate)( generic->data, "BoneNormalization" );
      }
    }
    return true;
  }

  // Activate Step 2:  Set the bone's Activate state
  if( (process_mode == ABV_PROCESS_TOGGLE_ACTIVE) ||
      (process_mode == ABV_PROCESS_DEACTIVATE) ||
      (process_mode == ABV_PROCESS_ACTIVATE) ) {

    if( process_mode == ABV_PROCESS_TOGGLE_ACTIVE ) {
      (*generic->evaluate)( generic->data, "BoneActive" );
    } else {
      flags = (boneinfo->flags)( id );
      if( (process_mode == ABV_PROCESS_ACTIVATE) && ((flags & LWBONEF_ACTIVE) ? false : true ) )
        (*generic->evaluate)( generic->data, "BoneActive" );
      else if( (process_mode == ABV_PROCESS_DEACTIVATE) && ((flags & LWBONEF_ACTIVE) ? true : false ) )
        (*generic->evaluate)( generic->data, "BoneActive" );
    }

    return true;
  }

  return false;
}

// ReselectItems():
//  Reselects the previously selected items
void AssignBoneVMaps_Interface::ReselectItems() {
  if( sel_type == LWI_BONE )
    (*generic->evaluate)( generic->data, "EditBones" );
  else
    (*generic->evaluate)( generic->data, "EditObjects" );

  char buffer[100];
  for( unsigned long i=items.NumElements()-1; i != -1; i-- ) {
    sprintf( buffer, "AddToSelection %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}
