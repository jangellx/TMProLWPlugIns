//
// FXLinkerPlus-Interface.cpp
//

#ifndef NULL
  #define NULL 0
#endif

#include "FXLinkerPlus-MotionInstance.h"

#include <stdio.h>
#include <time.h>

#include <lightwave/lwaboutpanel/lwaboutpanel.h>
#include <lightwave/lwaboutpanel/Logos/FI_Logo.h>
#include <lightwave/lwprogresspanel/lwprogresspanel.h>

extern DynArray< FXLinkerPlus_MotionInstance * > instances;

FXLinkerPlus_MotionInstance *new_inst = NULL;

GlobalFunc * FXLinkerPlus_Interface::global = NULL;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "FX Linker Plus",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Enhanced Particle Linker for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Foundation Imaging",
  "http://www.foundation-i.com",
  LWABOUT_HDIV,
  "Written by Joe Angell",
  "jangell@tmproductions.com",
  "http://www.tmproductions.com",
  NULL };

const char *attach_to_strings[] = {
  "Clones of selected items", "Directly to selected items", NULL };

const char *from_mode_strings[] = {
  "Particle Services", "PFX File", "PSM File", NULL };

const char *attach_as_strings[] = {
  "FX Linker Plus Motion Plug-in",
  "Keyframes from Item's Motion",
  "Keyframes from Item's Channels",
  "Keyframes from Item's Channels After Modifiers",
  NULL };

// Constructor():
FXLinkerPlus_Interface::FXLinkerPlus_Interface( void *local )
  : panel(NULL), ctrl_down(false), shift_down(false),
    progress_panel(NULL), progress_total(1), progress_current(0), inst(NULL) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)(*global)( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  objectfuncs = (LWObjectFuncs   *)(*global)( LWOBJECTFUNCS_GLOBAL,   GFUSE_TRANSIENT );
  instupdate  = (LWInstUpdate    *)(*global)( LWINSTUPDATE_GLOBAL,    GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)(*global)( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );
  chaninfo    = (LWChannelInfo   *)(*global)( LWCHANNELINFO_GLOBAL,   GFUSE_TRANSIENT );
  env_funcs   = (LWEnvelopeFuncs *)(*global)( LWENVELOPEFUNCS_GLOBAL, GFUSE_TRANSIENT );

  AquireVariableGlobals();
}

// Destructor():
FXLinkerPlus_Interface::~FXLinkerPlus_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  if( progress_panel != NULL )
    (*panel_funcs->destroy)( progress_panel );

  items.Flush();
  ReleaseVariableGlobals();
}

// AquireVariableGlobals():
void FXLinkerPlus_Interface::AquireVariableGlobals() {
  ui             = (LWInterfaceInfo *)(*global)( LWINTERFACEINFO_GLOBAL, GFUSE_ACQUIRE );
  iteminfo       = (LWItemInfo      *)(*global)( LWITEMINFO_GLOBAL,      GFUSE_ACQUIRE );
  objectinfo     = (LWObjectInfo    *)(*global)( LWOBJECTINFO_GLOBAL,    GFUSE_ACQUIRE );
  sceneinfo      = (LWSceneInfo     *)(*global)( LWSCENEINFO_GLOBAL,     GFUSE_ACQUIRE );
  particle_funcs = (LWPSysFuncs     *)(*global)( LWPSYSFUNCS_GLOBAL,     GFUSE_ACQUIRE );
}

// ReleaseVariableGlobals():
void FXLinkerPlus_Interface::ReleaseVariableGlobals() {
  (LWInterfaceInfo *)(*global)( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
  (LWItemInfo      *)(*global)( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  (LWObjectInfo    *)(*global)( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  (LWSceneInfo     *)(*global)( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  (LWPSysFuncs     *)(*global)( LWPSYSFUNCS_GLOBAL,     GFUSE_TRANSIENT );

  global( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
  global( LWITEMINFO_GLOBAL,      GFUSE_RELEASE );
  global( LWOBJECTINFO_GLOBAL,    GFUSE_RELEASE );
  global( LWSCENEINFO_GLOBAL,     GFUSE_RELEASE );
  global( LWPSYSFUNCS_GLOBAL,     GFUSE_RELEASE );

  ui             = NULL;
  iteminfo       = NULL;
  sceneinfo      = NULL;
  objectinfo     = NULL;
  particle_funcs = NULL;
}

// ReaquireVariableGlobals():
void FXLinkerPlus_Interface::ReaquireVariableGlobals() {
  ReleaseVariableGlobals();
  AquireVariableGlobals();
}


// CreateUI():
//  If inst is non-NULL, then this is an instance UI, and certain controls
//   are added or removed.  If inst is NULL, this is a global UI with some
//   different controls and a modal panel.
bool FXLinkerPlus_Interface::CreateUI( FXLinkerPlus_MotionInstance *_inst ) {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "FX Linker Plus -- %c2001 Foundation Imaging, written by Joe Angell", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  inst = _inst;
  if( inst != NULL ) {
    // Instance Popup
    instance_popup = CUSTPOPUP_CTL( panel_funcs, panel, "Instance", 350, InstPopup_Names, InstPopup_Count );
  }

  // From Type Popup
  from_mode = WPOPUP_CTL( panel_funcs, panel, "From", from_mode_strings, 150 );
  if( inst != NULL )
    PanelTools::PutUnder( instance_popup, from_mode, 20 );

  // Item Popup
  item_popup = CUSTPOPUP_CTL( panel_funcs, panel, "Item", 150, ItemPopup_Names, ItemPopup_Count );
  PanelTools::PutOnLineWith( from_mode, item_popup, ((inst == NULL) ? 40 : 10 ) );

  // File Requester
  file_req = LOAD_CTL( panel_funcs, panel, "File", 30 );
  PanelTools::PutOnLineWith( from_mode, file_req, ((inst == NULL) ? 40 : 10 ) );
  GHOST_CON( file_req );
  ERASE_CON( file_req );

  if( inst == NULL ) {
    // First Particle Index
    particle_index = MINISLIDER_CTL( panel_funcs, panel, "First Particle", 40, 0, 100000 );
    PanelTools::AlignLabels( from_mode, particle_index );

    // Attach To Popup
    attach_to_popup = WPOPUP_CTL( panel_funcs, panel, "Attach To", attach_to_strings, 150 );
    PanelTools::AlignLabels( particle_index, attach_to_popup );

    // Attach As Popup
    attach_as_popup = WPOPUP_CTL( panel_funcs, panel, "Attach As", attach_as_strings, 250 );
    PanelTools::AlignLabels( attach_to_popup, attach_as_popup );

    // Particle Step
    particle_step = MINISLIDER_CTL( panel_funcs, panel, "Step", 40, -100000, 100000 );
    PanelTools::AlignLabels( item_popup, particle_step );
    SET_INT( particle_step, 1 );

    // Number Of Clones
    number_of_clones = MINISLIDER_CTL( panel_funcs, panel, "Number of Clones", 40, 1, 100000 );
    PanelTools::AlignLabels( particle_step, number_of_clones );
    SET_INT( number_of_clones, 10 );

  } else {
    // Particle Index
    particle_index = INT_CTL( panel_funcs, panel, "Particle Index" );
    PanelTools::PutOnLineWith( from_mode, particle_index, 10 );
    PanelTools::PutUnder( particle_index, particle_index );
  }

  // Start Frame
  start_frame = MINISLIDER_CTL( panel_funcs, panel, "Start Frame", 40, -20000, 20000 );
  PanelTools::PutUnder( ((inst == NULL) ? attach_as_popup : particle_index), start_frame, 24 );
  if( inst == NULL ) {
    start_frame_random = MINISLIDER_CTL( panel_funcs, panel, "Randomize Start Frame +/-", 40, 0, 20000 );
    PanelTools::PutOnLineWith( start_frame, start_frame_random, 10 );
  } else {
    // Particle Info
    particle_info = STRRO_CTL( panel_funcs, panel, "Info", 40 );
    PanelTools::PutOnLineWith( start_frame, particle_info,  7 );
  }

  if( inst == NULL ) {
    GHOST_CON( start_frame );
    GHOST_CON( start_frame_random );
  } else {
    if( inst->GetFromMode() == FXFROM_ITEM ) {
      GHOST_CON( start_frame );
    }
  }

  // Random Rotation
  align_to_path_toggle   = BOOL_CTL( panel_funcs, panel,  "Use Align To Path" );
  align_look_ahead       = INT_CTL( panel_funcs, panel,   "Align To Path Look Ahead" );
  jitter_dampening       = INT_CTL(   panel_funcs, panel, "Jitter Dampening Frames (+/-)" );
  random_rotation_h      = FLOAT_CTL( panel_funcs, panel, "Initial H" );
  random_rotation_p      = FLOAT_CTL( panel_funcs, panel, "P" );
  random_rotation_b      = FLOAT_CTL( panel_funcs, panel, "B" );

  link_rotation_axes     = BOOL_CTL(  panel_funcs, panel, "Link Axes" );

  if( inst == NULL ) {
    random_rotation_h_var  = FLOAT_CTL( panel_funcs, panel, "+/- H" );
    random_rotation_p_var  = FLOAT_CTL( panel_funcs, panel, "P" );
    random_rotation_b_var  = FLOAT_CTL( panel_funcs, panel, "B" );
  }

  random_rotation_h_vel  = FLOAT_CTL( panel_funcs, panel, "Velocity H" );
  random_rotation_p_vel  = FLOAT_CTL( panel_funcs, panel, "P" );
  random_rotation_b_vel  = FLOAT_CTL( panel_funcs, panel, "B" );

  if( inst == NULL ) {
    random_rotation_h_vel_var  = FLOAT_CTL( panel_funcs, panel, "+/- H" );
    random_rotation_p_vel_var  = FLOAT_CTL( panel_funcs, panel, "P" );
    random_rotation_b_vel_var  = FLOAT_CTL( panel_funcs, panel, "B" );
  }

  // Random Scale
  size_effect_toggle     = BOOL_CTL(  panel_funcs, panel, "Use Size Effect" );
  size_effect_multiplier = FLOAT_CTL( panel_funcs, panel, "Size Effect Multiplier" );
  keep_scale_above_zero  = BOOL_CTL(  panel_funcs, panel, "Keep Scale Above Zero" );
  random_scale_x         = FLOAT_CTL( panel_funcs, panel, "Initial X" );
  random_scale_y         = FLOAT_CTL( panel_funcs, panel, "Y" );
  random_scale_z         = FLOAT_CTL( panel_funcs, panel, "Z" );

  link_scale_axes        = BOOL_CTL(  panel_funcs, panel, "Link Axes" );

  if( inst == NULL ) {
    random_scale_x_var = FLOAT_CTL( panel_funcs, panel, "+/- X" );
    random_scale_y_var = FLOAT_CTL( panel_funcs, panel, "Y" );
    random_scale_z_var = FLOAT_CTL( panel_funcs, panel, "Z" );
  }

  random_scale_x_vel  = FLOAT_CTL( panel_funcs, panel, "Velocity X" );
  random_scale_y_vel  = FLOAT_CTL( panel_funcs, panel, "Y" );
  random_scale_z_vel  = FLOAT_CTL( panel_funcs, panel, "Z" );

  if( inst == NULL ) {
    random_scale_x_vel_var = FLOAT_CTL( panel_funcs, panel, "+/- X" );
    random_scale_y_vel_var = FLOAT_CTL( panel_funcs, panel, "Y" );
    random_scale_z_vel_var = FLOAT_CTL( panel_funcs, panel, "Z" );
  }

  // Align the Random Rotation/Scale controls
  PanelTools::PutUnder( start_frame, align_to_path_toggle, 24 );
  PanelTools::PutOnLineWith( align_to_path_toggle, align_look_ahead,       65 );
  PanelTools::PutOnLineWith( size_effect_toggle,   size_effect_multiplier, 65 );

  PanelTools::PutOnLineWith( align_to_path_toggle, random_rotation_h, -20 );
  PanelTools::PutUnder( random_rotation_h, random_rotation_h );
  PanelTools::PutUnder( random_rotation_h, random_rotation_h );
  PanelTools::PutOnLineWith( random_rotation_h, random_rotation_p, 20 );
  PanelTools::PutOnLineWith( random_rotation_p, random_rotation_b,  5 );

  PanelTools::AlignLabels( align_to_path_toggle, link_rotation_axes );
  PanelTools::AlignLabels( link_rotation_axes, link_rotation_axes );

  if( inst == NULL ) {
    LWControl *align1[] = { random_rotation_h, random_rotation_h_var, random_rotation_h_vel, random_rotation_h_vel_var, random_scale_x, random_scale_x,
                            random_scale_x, random_scale_x, random_scale_x_var, random_scale_x_vel, random_scale_x_vel_var, NULL };

    LWControl *align2[] = { random_rotation_p, random_rotation_p_var, random_rotation_p_vel, random_rotation_p_vel_var, random_scale_y, random_scale_y,
                            keep_scale_above_zero, random_scale_y, random_scale_y_var, random_scale_y_vel, random_scale_y_vel_var, NULL };

    LWControl *align3[] = { align_look_ahead, jitter_dampening, random_rotation_b, random_rotation_b_var, random_rotation_b_vel, random_rotation_b_vel_var, random_scale_z_var,
                            size_effect_multiplier, random_scale_z, random_scale_z, random_scale_z_var, random_scale_z_vel, random_scale_z_vel_var, NULL };
    PanelTools::AlignLabels( align1 );
    PanelTools::AlignLabels( align2 );
    PanelTools::AlignLabels( align3 );
  } else {
    LWControl *align1[] = { random_rotation_h, random_rotation_h_vel, random_scale_x, random_scale_x,
                            random_scale_x, random_scale_x, random_scale_x_vel, NULL };

    LWControl *align2[] = { random_rotation_p, random_rotation_p_vel, random_scale_y, random_scale_y,
                            random_scale_y, random_scale_y, random_scale_y_vel, NULL };

    LWControl *align3[] = { align_look_ahead, jitter_dampening, random_rotation_b, random_rotation_b_vel, random_scale_z,
                            size_effect_multiplier, random_scale_z, random_scale_z, random_scale_z_vel, NULL };
    PanelTools::AlignLabels( align1 );
    PanelTools::AlignLabels( align2 );
    PanelTools::AlignLabels( align3 );
  }

  PanelTools::AlignLabels( align_to_path_toggle, size_effect_toggle );
  PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );
  PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );
  PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );
  PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );

  if( inst == NULL ) {
    PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );
    PanelTools::AlignLabels( size_effect_toggle, size_effect_toggle );
  }

  PanelTools::AlignLabels( size_effect_toggle, keep_scale_above_zero );
  PanelTools::AlignLabels( keep_scale_above_zero, link_scale_axes );

  // Additive Toggles
  add_position_button       = WBUTTON_CTL(    panel_funcs, panel, "Position", 67 );
  add_x_button              = BOOLBUTTON_CTL( panel_funcs, panel, "X" );
  add_y_button              = BOOLBUTTON_CTL( panel_funcs, panel, "Y" );
  add_z_button              = BOOLBUTTON_CTL( panel_funcs, panel, "Z" );

  add_rotation_button       = WBUTTON_CTL(    panel_funcs, panel, "Rotation", 67 );
  add_h_button              = BOOLBUTTON_CTL( panel_funcs, panel, "H" );
  add_p_button              = BOOLBUTTON_CTL( panel_funcs, panel, "P" );
  add_b_button              = BOOLBUTTON_CTL( panel_funcs, panel, "B" );

  add_scale_button          = WBUTTON_CTL(    panel_funcs, panel, "Scale",    67 );
  add_sx_button             = BOOLBUTTON_CTL( panel_funcs, panel, "X" );
  add_sy_button             = BOOLBUTTON_CTL( panel_funcs, panel, "Y" );
  add_sz_button             = BOOLBUTTON_CTL( panel_funcs, panel, "Z" );

  if( inst == NULL )
    PanelTools::AlignLabels( random_scale_x_vel_var, add_position_button, 18 );
  else
    PanelTools::AlignLabels( random_scale_x_vel, add_position_button, 18 );

  PanelTools::PutOnLineWith( add_position_button, add_rotation_button,  3 );
  PanelTools::PutOnLineWith( add_rotation_button, add_scale_button,     3 );

  PanelTools::AlignLabels(   add_position_button, add_x_button );
  PanelTools::PutOnLineWith( add_x_button, add_y_button, -5 );
  PanelTools::PutOnLineWith( add_y_button, add_z_button, -5 );

  PanelTools::AlignLabels(   add_rotation_button, add_h_button );
  PanelTools::PutOnLineWith( add_h_button, add_p_button, -5 );
  PanelTools::PutOnLineWith( add_p_button, add_b_button, -5 );

  PanelTools::AlignLabels(   add_scale_button, add_sx_button );
  PanelTools::PutOnLineWith( add_sx_button, add_sy_button, -5 );
  PanelTools::PutOnLineWith( add_sy_button, add_sz_button, -5 );

  // About Button
  about = WBUTTON_CTL( panel_funcs, panel, "About", 65 );
  PanelTools::AlignLabels( size_effect_toggle, about );
  PanelTools::PutUnder( add_position_button, about );

  int y = CON_Y( add_sy_button );
  int h = CON_H( add_sy_button );
  PAN_SETH( panel_funcs, panel, (y + h + 5) );

  // Set some default values
  SET_INT( align_look_ahead, 1 );
  SET_INT( jitter_dampening, 0 );

  SET_FLOAT( random_rotation_h, 0.0 );
  SET_FLOAT( random_rotation_p, 0.0 );
  SET_FLOAT( random_rotation_b, 0.0 );

  if( inst == NULL ) {
    SET_FLOAT( random_rotation_h_var, 0.0 );
    SET_FLOAT( random_rotation_p_var, 0.0 );
    SET_FLOAT( random_rotation_b_var, 0.0 );
  }

  SET_FLOAT( random_rotation_h_vel, 0.0 );
  SET_FLOAT( random_rotation_p_vel, 0.0 );
  SET_FLOAT( random_rotation_b_vel, 0.0 );

  // - Scale Options
  SET_FLOAT( size_effect_multiplier, 1.0 );

  SET_FLOAT( random_scale_x, 1.0 );
  SET_FLOAT( random_scale_y, 1.0 );
  SET_FLOAT( random_scale_z, 1.0 );

  if( inst == NULL ) {
    SET_FLOAT( random_scale_x_var, 0.0 );
    SET_FLOAT( random_scale_y_var, 0.0 );
    SET_FLOAT( random_scale_z_var, 0.0 );
  }

  SET_FLOAT( random_scale_x_vel, 0.0 );
  SET_FLOAT( random_scale_y_vel, 0.0 );
  SET_FLOAT( random_scale_z_vel, 0.0 );

  // - Additive Options
  SET_INT( add_x_button,  0 );
  SET_INT( add_y_button,  0 );
  SET_INT( add_z_button,  0 );
  SET_INT( add_h_button,  0 );
  SET_INT( add_p_button,  0 );
  SET_INT( add_b_button,  0 );
  SET_INT( add_sx_button, 0 );
  SET_INT( add_sy_button, 0 );
  SET_INT( add_sz_button, 0 );

  // Event Handlers
  if( inst != NULL ) {
    CON_SETEVENT( instance_popup,         HandleInstancePopup,        this );
  } else {
    CON_SETEVENT(   attach_to_popup,        HandleAttachTo,             this );
  }

  CON_SETEVENT(   from_mode,              HandleFromMode,             this );
  CON_SETEVENT(   item_popup,             HandleItemPopup,            this );
  CON_SETEVENT(   file_req,               HandleFileReq,              this );

  CON_SETEVENT(   particle_index,         HandleParticleIndex,        this );

  CON_SETEVENT(   about,                  HandleAbout,                this );

  CON_SETEVENT(   align_to_path_toggle,   HandleAlignToPathToggle,    this );
  CON_SETEVENT(   align_look_ahead,       HandleAlignLookAhead,       this );
  CON_SETEVENT(   jitter_dampening,       HandleJitterDampening,      this );

  CON_SETEVENT(   random_rotation_h,      HandleRandomRotationH,      this );
  CON_SETEVENT(   random_rotation_p,      HandleRandomRotationP,      this );
  CON_SETEVENT(   random_rotation_b,      HandleRandomRotationB,      this );

  CON_SETEVENT(   random_rotation_h_vel,  HandleRandomRotationHVel,   this );
  CON_SETEVENT(   random_rotation_p_vel,  HandleRandomRotationPVel,   this );
  CON_SETEVENT(   random_rotation_b_vel,  HandleRandomRotationBVel,   this );

  CON_SETEVENT(   size_effect_toggle,     HandleSizeEffectToggle,     this );
  CON_SETEVENT(   size_effect_multiplier, HandleSizeEffectMultiplier, this );

  CON_SETEVENT(   random_scale_x,         HandleRandomScaleX,         this );
  CON_SETEVENT(   random_scale_y,         HandleRandomScaleY,         this );
  CON_SETEVENT(   random_scale_z,         HandleRandomScaleZ,         this );

  CON_SETEVENT(   random_scale_x_vel,     HandleRandomScaleXVel,      this );
  CON_SETEVENT(   random_scale_y_vel,     HandleRandomScaleYVel,      this );
  CON_SETEVENT(   random_scale_z_vel,     HandleRandomScaleZVel,      this );

  CON_SETEVENT(   add_position_button,    HandleAddPosition,          this );
  CON_SETEVENT(   add_x_button,           HandleAddPositionX,         this );
  CON_SETEVENT(   add_y_button,           HandleAddPositionY,         this );
  CON_SETEVENT(   add_z_button,           HandleAddPositionZ,         this );

  CON_SETEVENT(   add_rotation_button,    HandleAddRotation,          this );
  CON_SETEVENT(   add_h_button,           HandleAddRotationH,         this );
  CON_SETEVENT(   add_p_button,           HandleAddRotationP,         this );
  CON_SETEVENT(   add_b_button,           HandleAddRotationB,         this );

  CON_SETEVENT(   add_scale_button,       HandleAddScale,             this );
  CON_SETEVENT(   add_sx_button,          HandleAddScaleX,            this );
  CON_SETEVENT(   add_sy_button,          HandleAddScaleY,            this );
  CON_SETEVENT(   add_sz_button,          HandleAddScaleZ,            this );

  CON_SETEVENT(   link_scale_axes,        HandleLinkScale,            this );
  CON_SETEVENT(   link_rotation_axes,     HandleLinkRotation,         this );

  // Panel-Level Callbacks
  (*panel_funcs->set)( panel, PAN_USERDATA,      this                );
  (*panel_funcs->set)( panel, PAN_USERCLOSE,     HandlePanelClose    );
  (*panel_funcs->set)( panel, PAN_USEROPEN,      HandlePanelOpen     );
  (*panel_funcs->set)( panel, PAN_USERDRAW,      HandleDrawPanel     );
  (*panel_funcs->set)( panel, PAN_USERKEYS,      HandleKeys          );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS,    HandleKeyUps        );
  (*panel_funcs->set)( panel, PAN_USERACTIVATE,  HandlePanelActivate );

  if( inst == NULL ) {
    HandleAlignToPathToggle( align_to_path_toggle,  this );
    HandleSizeEffectToggle(  size_effect_toggle,    this );
    HandleAttachTo( attach_to_popup, this );
  }

  return true;
}

// OpenUI():
bool FXLinkerPlus_Interface::OpenUI() {
  if( inst == NULL ) {
    // Make a copy of the selected items
    selected.Reset();
    for( int i=0; ui->selItems[i] != NULL; i++ )
      selected.Add( ui->selItems[i] );

    // Open the Panel repeatedly until the user has decided to cancel or processing
    //  completed successfully.
    do {
      if( (*panel_funcs->open)( panel, PANF_BLOCKING | PANF_CANCEL | PANF_PASSALLKEYS ) == 0 )
        break;

      if( Process() )
        break;
    } while( true );
  } else {
    // Open non-modal for the motion plug-in UI
    (*panel_funcs->open)( panel, PANF_PASSALLKEYS );
  }

  return true;
}

// CloseUI():
void FXLinkerPlus_Interface::CloseUI() {
  (*panel_funcs->close)( panel );
}

// BuildParticleItemList():
//  Builds a list of items that particle systems are attached to.
int FXLinkerPlus_Interface::BuildParticleItemList() {
  items.Flush();

  const char *name;
  for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
    if( ((*particle_funcs->getPSys)( item ))[0] != NULL ) {
      name = (*iteminfo->name)( item );
      items.Add( new FXLinkerPlus_Item( item, name ) );
    }
  }

  return items.NumElements();
}

//
// -- Panel Callbacks ---------------------------------------------------------
//

// HandleDrawPanel() {
void HandleDrawPanel( LWPanelID panel, void *data, DrMode mode) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  CDrawFuncs df( lint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( lint->panel_funcs, lint->panel );
  int pw = PAN_GETW( lint->panel_funcs, lint->panel );

  int y = CON_Y( lint->start_frame );
  int h = CON_H( lint->start_frame );
  PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 4, pw, 1 );

  if( lint->inst == NULL ) {
    y = CON_Y( lint->attach_as_popup );
    h = CON_H( lint->attach_as_popup );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 4, pw, 1 );

    y = CON_Y( lint->random_rotation_h_vel_var );
    h = CON_H( lint->random_rotation_h_vel_var );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 7, pw, 1 );

    y = CON_Y( lint->random_scale_x_vel_var );
    h = CON_H( lint->random_scale_x_vel_var );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 2, pw, 1 );
 } else {
    y = CON_Y( lint->instance_popup );
    h = CON_H( lint->instance_popup );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 4, pw, 1 );

    y = CON_Y( lint->particle_index );
    h = CON_H( lint->particle_index );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 4, pw, 1 );

    y = CON_Y( lint->random_rotation_h_vel );
    h = CON_H( lint->random_rotation_h_vel );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 7, pw, 1 );

    y = CON_Y( lint->random_scale_x_vel );
    h = CON_H( lint->random_scale_x_vel );
    PanelDraw::HorizontalDivider( lint->panel, &df, 0, y + h + 2, pw, 1 );
  }

  int x = CON_X( lint->add_position_button );
  y = CON_HOTY( lint->add_position_button );
  df.WriteText( lint->panel, "Additive", COLOR_BLACK, x-43, y+3 );
}

// HandlePanelClose():
//  Called when the panel is closed by the user.
void HandlePanelClose( LWPanelID pan, void *data ) {
  ;
/*
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL )
    lint->inst->CloseUI();
*/
}

// HandlePanelOpen():
//  Called when the panel is opened by the user.
void HandlePanelOpen( LWPanelID pan, void *data ) {
  ;
}

// HandlePanelActivate():
//  Called when the panel is opened by the user.
void HandlePanelActivate( LWPanelID pan, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  lint->BuildParticleItemList();
  HandleInstancePopup( lint->instance_popup, data );
  lint->ReaquireVariableGlobals();
}

// HandleKeys():
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
//  int state;

  switch( key ) {
    case LWDK_CTRL:             // Control
      lint->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      lint->shift_down = true;
      break;
  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  switch( key ) {
    case LWDK_CTRL:             // Control
      lint->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      lint->shift_down = false;
      break;
  }
}

//
// -- Popup Callbacks ---------------------------------------------------------
//

// ItemPopup_Names():
char * ItemPopup_Names( void * data, int index ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  lint->BuildParticleItemList();  // This could change in the non-modal UI, so rebuild it each time

  if( index < 0 )
    return "";

  if( index < (int)lint->items.NumElements() ) {
    const char * name = lint->items[ index ]->GetName();
    return (char *)((name == NULL) ? "(unknown)" : name );
  }

  return NULL;
}

// ItemPopup_Count():
int ItemPopup_Count( void * data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  return lint->items.NumElements();
}

// InstPopup_Names():
char * InstPopup_Names( void * data, int index ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  assert( lint->inst != NULL );

  if( index < 0 )
    return "";

  if( index < (int)instances.NumElements() ) {
    const char * name = (*lint->iteminfo->name)( instances[ index ]->GetAppliedItem() );
    return (char *)name;
  }

  return NULL;
}

// InstPopup_Count():
int InstPopup_Count( void * data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  assert( lint->inst != NULL );
  return instances.NumElements();
}

//
// -- Handlers ---------------------------------------------
//

// HandleAttachTo():
void HandleAttachTo( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->attach_to_popup, value );

  if( value == 0 ) {
    UNGHOST_CON( lint->number_of_clones );
  } else {
    GHOST_CON( lint->number_of_clones );
  }
}

// HandleInstancePopup():
void HandleInstancePopup( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst == NULL )
    return;

  assert( instances.NumElements() != 0 );

  // Set which instance we are
  int inst_index;
  GET_INT( lint->instance_popup, inst_index );
  if( inst_index >= (int)instances.NumElements() )
    inst_index = (int)instances.NumElements() - 1;

  SET_INT( lint->instance_popup, inst_index );
  lint->inst = instances[ inst_index ];

  // From Type Popup
  SET_INT( lint->from_mode, lint->inst->GetFromMode() );

  GHOST_CON( lint->file_req );
  ERASE_CON( lint->file_req );

  GHOST_CON( lint->item_popup );
  ERASE_CON( lint->item_popup );

  if( lint->inst->GetFromMode() == FXFROM_ITEM ) {
    UNGHOST_CON( lint->item_popup );
  } else {
    UNGHOST_CON( lint->file_req );
  }

  // Particles From Pop-up
  for( unsigned long i=0; i < lint->items.NumElements(); i++ ) {
    if( lint->items[i]->GetID() == lint->inst->GetParticleItem() )
      break;
  }

  SET_INT( lint->item_popup, ((i == lint->items.NumElements()) ? -1 : i) );

  // Particle Index
  SET_INT( lint->particle_index, lint->inst->GetParticleIndex() );
  if(  lint->inst->GetParticleFile() == NULL ) {
    SET_STR( lint->file_req, "", 0 );
  } else {
    SET_STR( lint->file_req, (char *)lint->inst->GetParticleFile(), strlen( lint->inst->GetParticleFile() ) );
  }

  // Start Frame
  SET_INT( lint->start_frame, lint->inst->GetStartFrame() );

  // Particle Info
  if( lint->inst->GetParticles() == NULL ) {
    SET_STR( lint->particle_info, "", 0 );
  } else {
    char buffer[ 100 ];
    sprintf( buffer, "Frame Range %d to %d", lint->inst->GetParticles()->GetFirstFrame(), lint->inst->GetParticles()->GetLastFrame() );
    SET_STR( lint->particle_info, buffer, strlen(buffer) );
  }

  // Align To Path
  SET_INT( lint->align_to_path_toggle, (lint->inst->GetUseAlignToPath() ? 1 : 0) );
  SET_INT( lint->align_look_ahead, lint->inst->GetAlignLookAhead() );
  SET_INT( lint->jitter_dampening, lint->inst->GetJitterDampening() );

  SET_INT( lint->link_rotation_axes, (lint->inst->GetLinkRotationAxes() ? 1 : 0) );

  // Rotation
  SET_FLOAT( lint->random_rotation_h, lint->inst->GetRotationVec()[0] );
  SET_FLOAT( lint->random_rotation_p, lint->inst->GetRotationVec()[1] );
  SET_FLOAT( lint->random_rotation_b, lint->inst->GetRotationVec()[2] );

  SET_FLOAT( lint->random_rotation_h_vel, lint->inst->GetRotationVelocityVec()[0] );
  SET_FLOAT( lint->random_rotation_p_vel, lint->inst->GetRotationVelocityVec()[1] );
  SET_FLOAT( lint->random_rotation_b_vel, lint->inst->GetRotationVelocityVec()[2] );

  // Size Effect
  SET_INT( lint->size_effect_toggle, (lint->inst->GetUseSizeEffect() ? 1 : 0) );
  SET_FLOAT( lint->size_effect_multiplier, lint->inst->GetSizeEffectMultiplier() );

  SET_INT( lint->link_scale_axes, (lint->inst->GetLinkScaleAxes() ? 1 : 0) );
  SET_INT( lint->keep_scale_above_zero, (lint->inst->GetKeepScaleAboveZero() ? 1 : 0) );

  // Scale
  SET_FLOAT( lint->random_scale_x, lint->inst->GetScaleVec()[0] );
  SET_FLOAT( lint->random_scale_y, lint->inst->GetScaleVec()[1] );
  SET_FLOAT( lint->random_scale_z, lint->inst->GetScaleVec()[2] );

  SET_FLOAT( lint->random_scale_x_vel, lint->inst->GetScaleVelocityVec()[0] );
  SET_FLOAT( lint->random_scale_y_vel, lint->inst->GetScaleVelocityVec()[1] );
  SET_FLOAT( lint->random_scale_z_vel, lint->inst->GetScaleVelocityVec()[2] );

  // Additive
  SET_INT( lint->add_x_button, (lint->inst->GetAdditive( FXLPADD_X ) ? 1 : 0) );
  SET_INT( lint->add_y_button, (lint->inst->GetAdditive( FXLPADD_Y ) ? 1 : 0) );
  SET_INT( lint->add_z_button, (lint->inst->GetAdditive( FXLPADD_Z ) ? 1 : 0) );

  SET_INT( lint->add_h_button, (lint->inst->GetAdditive( FXLPADD_H ) ? 1 : 0) );
  SET_INT( lint->add_p_button, (lint->inst->GetAdditive( FXLPADD_P ) ? 1 : 0) );
  SET_INT( lint->add_b_button, (lint->inst->GetAdditive( FXLPADD_B ) ? 1 : 0) );

  SET_INT( lint->add_sx_button, (lint->inst->GetAdditive( FXLPADD_SX ) ? 1 : 0) );
  SET_INT( lint->add_sy_button, (lint->inst->GetAdditive( FXLPADD_SY ) ? 1 : 0) );
  SET_INT( lint->add_sz_button, (lint->inst->GetAdditive( FXLPADD_SZ ) ? 1 : 0) );

  HandleAlignToPathToggle( lint->align_to_path_toggle, lint );
  HandleSizeEffectToggle(  lint->size_effect_toggle,   lint );
  HandleLinkRotation(      lint->link_rotation_axes,   lint );
  HandleLinkScale(         lint->link_scale_axes,      lint );
}

// HandleFromMode():
void HandleFromMode( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int state;
  GET_INT( lint->from_mode, state );

  if( state == FXFROM_ITEM ) {
    GHOST_CON( lint->file_req );
    ERASE_CON( lint->file_req );

    UNGHOST_CON( lint->item_popup );

    GHOST_CON( lint->start_frame );

    if( lint->inst == NULL ) {
      GHOST_CON( lint->start_frame_random );
    }

  } else {
    GHOST_CON( lint->item_popup );
    ERASE_CON( lint->item_popup );

    UNGHOST_CON( lint->file_req );

    UNGHOST_CON( lint->start_frame );

    if( lint->inst == NULL ) {
      UNGHOST_CON( lint->start_frame_random );
    }
  }

  if( lint->inst != NULL ) {
    lint->inst->SetFromMode( (FXLP_FromModes)state );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );

    lint->inst->UpdatePFX();

    // Particle Info
    if( lint->inst->GetParticles() == NULL ) {
      SET_STR( lint->particle_info, "", 0 );
    } else {
      char buffer[ 100 ];
      sprintf( buffer, "Frame Range %d to %d", lint->inst->GetParticles()->GetFirstFrame(), lint->inst->GetParticles()->GetLastFrame() );
      SET_STR( lint->particle_info, buffer, strlen( buffer ) );
    }
  }
}

// HandleFileReq
void HandleFileReq( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  if( lint->inst != NULL ) {
    char buffer[ MAX_PATH_LENGTH ];
    GET_STR( lint->file_req, buffer, MAX_PATH_LENGTH );

    if( buffer[0] == '\0' )
      return;

    lint->inst->SetParticleFile( buffer, lint->message );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );

    // Particle Info
    if( lint->inst->GetParticles() == NULL ) {
      SET_STR( lint->particle_info, "", 0 );
    } else {
      char buffer[ 100 ];
      sprintf( buffer, "Frame Range %d to %d", lint->inst->GetParticles()->GetFirstFrame(), lint->inst->GetParticles()->GetLastFrame() );
      SET_STR( lint->particle_info, buffer, strlen( buffer ) );

      (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
    }
  }
}

// HandleItemPopup
void HandleItemPopup( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int state;
  GET_INT( lint->item_popup, state );

  if( lint->inst != NULL ) {
    lint->inst->SetParticleItem( lint->items[ state ]->GetID() );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  FI_Logo logo;
  LWAboutPanel( lint->panel_funcs, "Frame Linker Plus", about_text, &logo, lint->panel, true, NULL /*keys_text*/ );
}

// HandleParticleIndex():
void HandleParticleIndex( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->particle_index, value );
    lint->inst->SetParticleIndex( value );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleStartFrame():
void HandleStartFrame( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->start_frame, value );
    lint->inst->SetStartFrame( value );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAlignToPathToggle():
void HandleAlignToPathToggle( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->align_to_path_toggle, value );

  if( lint->inst != NULL ) {
    lint->inst->SetUseAlignToPath( (value == 0) ? false : true );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }

  if( value == 0 ) {
    GHOST_CON( lint->align_look_ahead );
    GHOST_CON( lint->jitter_dampening );
  } else {
    UNGHOST_CON( lint->align_look_ahead );
    UNGHOST_CON( lint->jitter_dampening );
  }
}

// HandleAlignLookAhead():
void HandleAlignLookAhead( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  int value;
  GET_INT( lint->align_look_ahead, value );
  if( value < 0 )
    value = 1;

  if( lint->inst != NULL ) {
    lint->inst->SetAlignLookAhead( (short)value );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }

  SET_INT( lint->align_look_ahead, value );
}

// HandleJitterDampening():
void HandleJitterDampening( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  int value;
  GET_INT( lint->jitter_dampening, value );
  if( value < 0 )
    value = 0;

  if( lint->inst != NULL ) {
    lint->inst->SetJitterDampening( (short)value );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }

  SET_INT( lint->jitter_dampening, value );
}

// HandleRandomRotationH():
void HandleRandomRotationH( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_h, value );
    lint->inst->GetRotationVec()[ 0 ] = value;
  }
}

// HandleRandomRotationP():
void HandleRandomRotationP( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_p, value );
    lint->inst->GetRotationVec()[ 1 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomRotationB():
void HandleRandomRotationB( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_b, value );
    lint->inst->GetRotationVec()[ 2 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomRotationHVel():
void HandleRandomRotationHVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_h_vel, value );
    lint->inst->GetRotationVelocityVec()[ 0 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomRotationPVel():
void HandleRandomRotationPVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_p_vel, value );
    lint->inst->GetRotationVelocityVec()[ 1 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomRotationBVel():
void HandleRandomRotationBVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_rotation_b_vel, value );
    lint->inst->GetRotationVelocityVec()[ 2 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleSizeEffectToggle():
void HandleSizeEffectToggle( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->size_effect_toggle, value );

  if( lint->inst != NULL ) {
    lint->inst->SetUseSizeEffect( (value == 0) ? false : true );
    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }

  if( value == 0 ) {
    GHOST_CON( lint->size_effect_multiplier );
  } else {
    UNGHOST_CON( lint->size_effect_multiplier );
  }
}

// HandleSizeEffectMultiplier():
void HandleSizeEffectMultiplier( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_INT( lint->size_effect_multiplier, value );
    lint->inst->SetSizeEffectMultiplier( value );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleX():
void HandleRandomScaleX( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_x, value );
    lint->inst->GetScaleVec()[ 0 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleY():
void HandleRandomScaleY( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_y, value );
    lint->inst->GetScaleVec()[ 1 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleZ():
void HandleRandomScaleZ( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_z, value );
    lint->inst->GetScaleVec()[ 2 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleXVel():
void HandleRandomScaleXVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_x_vel, value );
    lint->inst->GetScaleVelocityVec()[ 0 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleYVel():
void HandleRandomScaleYVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_y_vel, value );
    lint->inst->GetScaleVelocityVec()[ 1 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleRandomScaleZVel():
void HandleRandomScaleZVel( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    double value;
    GET_FLOAT( lint->random_scale_z_vel, value );
    lint->inst->GetScaleVelocityVec()[ 2 ] = value;

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddPosition():
void HandleAddPosition( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->add_x_button, value );
  if( !lint->shift_down ) {
    SET_INT( lint->add_x_button, !value );
    SET_INT( lint->add_y_button, !value );
    SET_INT( lint->add_z_button, !value );
  } else {
    GET_INT( lint->add_x_button,  value );
    SET_INT( lint->add_x_button, !value );

    GET_INT( lint->add_y_button,  value );
    SET_INT( lint->add_y_button, !value );

    GET_INT( lint->add_z_button,  value );
    SET_INT( lint->add_z_button, !value );
  }

  if( lint->inst != NULL ) {
    GET_INT( lint->add_x_button, value );
    lint->inst->SetAdditive( FXLPADD_X, (value == 0) ? false : true );

    GET_INT( lint->add_y_button, value );
    lint->inst->SetAdditive( FXLPADD_Y, (value == 0) ? false : true );

    GET_INT( lint->add_z_button, value );
    lint->inst->SetAdditive( FXLPADD_Z, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddPositionX():
void HandleAddPositionX( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_x_button, value );
    lint->inst->SetAdditive( FXLPADD_X, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddPositionY():
void HandleAddPositionY( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_y_button, value );
    lint->inst->SetAdditive( FXLPADD_Y, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddPositionZ():
void HandleAddPositionZ( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_z_button, value );
    lint->inst->SetAdditive( FXLPADD_Z, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddRotation():
void HandleAddRotation( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->add_h_button, value );
  if( !lint->shift_down ) {
    SET_INT( lint->add_h_button, !value );
    SET_INT( lint->add_p_button, !value );
    SET_INT( lint->add_b_button, !value );
  } else {
    GET_INT( lint->add_h_button,  value );
    SET_INT( lint->add_h_button, !value );

    GET_INT( lint->add_p_button,  value );
    SET_INT( lint->add_p_button, !value );

    GET_INT( lint->add_b_button,  value );
    SET_INT( lint->add_b_button, !value );
  }

  if( lint->inst != NULL ) {
    GET_INT( lint->add_h_button, value );
    lint->inst->SetAdditive( FXLPADD_H, (value == 0) ? false : true );

    GET_INT( lint->add_p_button, value );
    lint->inst->SetAdditive( FXLPADD_P, (value == 0) ? false : true );

    GET_INT( lint->add_b_button, value );
    lint->inst->SetAdditive( FXLPADD_B, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddRotationH():
void HandleAddRotationH( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_h_button, value );
    lint->inst->SetAdditive( FXLPADD_H, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddRotationP():
void HandleAddRotationP( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_p_button, value );
    lint->inst->SetAdditive( FXLPADD_P, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddRotationB():
void HandleAddRotationB( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_b_button, value );
    lint->inst->SetAdditive( FXLPADD_B, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddScale():
void HandleAddScale( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;

  int value;
  GET_INT( lint->add_sx_button, value );
  if( !lint->shift_down ) {
    SET_INT( lint->add_sx_button, !value );
    SET_INT( lint->add_sy_button, !value );
    SET_INT( lint->add_sz_button, !value );
  } else {
    GET_INT( lint->add_sx_button,  value );
    SET_INT( lint->add_sx_button, !value );

    GET_INT( lint->add_sy_button,  value );
    SET_INT( lint->add_sy_button, !value );

    GET_INT( lint->add_sz_button,  value );
    SET_INT( lint->add_sz_button, !value );
  }

  if( lint->inst != NULL ) {
    GET_INT( lint->add_sx_button, value );
    lint->inst->SetAdditive( FXLPADD_SX, (value == 0) ? false : true );

    GET_INT( lint->add_sy_button, value );
    lint->inst->SetAdditive( FXLPADD_SY, (value == 0) ? false : true );

    GET_INT( lint->add_sz_button, value );
    lint->inst->SetAdditive( FXLPADD_SZ, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddScaleX():
void HandleAddScaleX( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_sx_button, value );
    lint->inst->SetAdditive( FXLPADD_SX, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddScaleY():
void HandleAddScaleY( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_sy_button, value );
    lint->inst->SetAdditive( FXLPADD_SY, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleAddScaleZ():
void HandleAddScaleZ( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  if( lint->inst != NULL ) {
    int value;
    GET_INT( lint->add_sz_button, value );
    lint->inst->SetAdditive( FXLPADD_SZ, (value == 0) ? false : true );

    (*lint->instupdate)( MOTION_PLUGIN_NAME, data );
  }
}

// HandleLinkRotation():
void HandleLinkRotation( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  int state;
  GET_INT( lint->link_rotation_axes, state );

  if( state == 0 ) {
    UNGHOST_CON( lint->random_rotation_p );
    UNGHOST_CON( lint->random_rotation_b );

    if( lint->inst == NULL ) {
      UNGHOST_CON( lint->random_rotation_p_var );
      UNGHOST_CON( lint->random_rotation_b_var );
    }

    UNGHOST_CON( lint->random_rotation_p_vel );
    UNGHOST_CON( lint->random_rotation_b_vel );

    if( lint->inst == NULL ) {
      UNGHOST_CON( lint->random_rotation_p_vel_var );
      UNGHOST_CON( lint->random_rotation_b_vel_var );
    }

  } else {
    GHOST_CON( lint->random_rotation_p );
    GHOST_CON( lint->random_rotation_b );

    if( lint->inst == NULL ) {
      GHOST_CON( lint->random_rotation_p_var );
      GHOST_CON( lint->random_rotation_b_var );
    }

    GHOST_CON( lint->random_rotation_p_vel );
    GHOST_CON( lint->random_rotation_b_vel );

    if( lint->inst == NULL ) {
      GHOST_CON( lint->random_rotation_p_vel_var );
      GHOST_CON( lint->random_rotation_b_vel_var );
    }
  }

  if( lint->inst != NULL )
    lint->inst->SetLinkRotationAxes( (state != 0) );
}

// HandleLinkScale():
void HandleLinkScale( LWControl *con, void *data ) {
  FXLinkerPlus_Interface *lint = (FXLinkerPlus_Interface *)data;
  int state;
  GET_INT( lint->link_scale_axes, state );

  if( state == 0 ) {
    UNGHOST_CON( lint->random_scale_y );
    UNGHOST_CON( lint->random_scale_z );

    if( lint->inst == NULL ) {
      UNGHOST_CON( lint->random_scale_y_var );
      UNGHOST_CON( lint->random_scale_z_var );
    }

    UNGHOST_CON( lint->random_scale_y_vel );
    UNGHOST_CON( lint->random_scale_z_vel );

    if( lint->inst == NULL ) {
      UNGHOST_CON( lint->random_scale_y_vel_var );
      UNGHOST_CON( lint->random_scale_z_vel_var );
    }
  } else {
    GHOST_CON( lint->random_scale_y );
    GHOST_CON( lint->random_scale_z );

    if( lint->inst == NULL ) {
      GHOST_CON( lint->random_scale_y_var );
      GHOST_CON( lint->random_scale_z_var );
    }

    GHOST_CON( lint->random_scale_y_vel );
    GHOST_CON( lint->random_scale_z_vel );

    if( lint->inst == NULL ) {
      GHOST_CON( lint->random_scale_y_vel_var );
      GHOST_CON( lint->random_scale_z_vel_var );
    }
  }

  if( lint->inst != NULL )
    lint->inst->SetLinkScaleAxes( (state != 0) );
}

// Process():
bool FXLinkerPlus_Interface::Process() {
  // Get the mode
  int attach_mode, attach_as;
  GET_INT( attach_to_popup, attach_mode );
  GET_INT( attach_as_popup, attach_as   );

  // Grab the interface options
  int item_index, from_mode_value, particle_index_value, particle_step_value;
  GET_INT( from_mode,      from_mode_value );
  GET_INT( item_popup,     item_index );
  GET_INT( particle_index, particle_index_value );
  GET_INT( particle_step,  particle_step_value );

  int start_frame_value, start_frame_random_value;
  GET_INT( start_frame,        start_frame_value );
  GET_INT( start_frame_random, start_frame_random_value );

  char particle_file_value[ MAX_PATH_LENGTH ] = "";
  GET_STR( file_req, particle_file_value, MAX_PATH_LENGTH );

  // - Rotation Options
  int align_to_path_value, align_look_ahead_value, jitter_dampening_value, link_rotation_axes_value;
  GET_INT( align_to_path_toggle, align_to_path_value );
  GET_INT( align_look_ahead, align_look_ahead_value );
  GET_INT( jitter_dampening, jitter_dampening_value );
  GET_INT( link_rotation_axes, link_rotation_axes_value );

  double h, p, b;
  double h_var, p_var, b_var;
  double h_vel, p_vel, b_vel;
  double h_vel_var, p_vel_var, b_vel_var;

  GET_FLOAT( random_rotation_h, h );
  GET_FLOAT( random_rotation_p, p );
  GET_FLOAT( random_rotation_b, b );

  GET_FLOAT( random_rotation_h_var, h_var );
  GET_FLOAT( random_rotation_p_var, p_var );
  GET_FLOAT( random_rotation_b_var, b_var );

  GET_FLOAT( random_rotation_h_vel, h_vel );
  GET_FLOAT( random_rotation_p_vel, p_vel );
  GET_FLOAT( random_rotation_b_vel, b_vel );

  GET_FLOAT( random_rotation_h_vel_var, h_vel_var );
  GET_FLOAT( random_rotation_p_vel_var, p_vel_var );
  GET_FLOAT( random_rotation_b_vel_var, b_vel_var );

  // - Scale Options
  int size_effect_value, keep_scale_above_zero_value, link_scale_axes_value;
  double size_effect_multiplier_value;
  GET_INT( size_effect_toggle, size_effect_value );
  GET_INT( keep_scale_above_zero, keep_scale_above_zero_value );
  GET_INT( link_scale_axes, link_scale_axes_value );
  GET_FLOAT( size_effect_multiplier, size_effect_multiplier_value );

  double sx, sy, sz;
  double sx_var, sy_var, sz_var;
  double sx_vel, sy_vel, sz_vel;
  double sx_vel_var, sy_vel_var, sz_vel_var;

  GET_FLOAT( random_scale_x, sx );
  GET_FLOAT( random_scale_y, sy );
  GET_FLOAT( random_scale_z, sz );

  GET_FLOAT( random_scale_x_var, sx_var );
  GET_FLOAT( random_scale_y_var, sy_var );
  GET_FLOAT( random_scale_z_var, sz_var );

  GET_FLOAT( random_scale_x_vel, sx_vel );
  GET_FLOAT( random_scale_y_vel, sy_vel );
  GET_FLOAT( random_scale_z_vel, sz_vel );

  GET_FLOAT( random_scale_x_vel_var, sx_vel_var );
  GET_FLOAT( random_scale_y_vel_var, sy_vel_var );
  GET_FLOAT( random_scale_z_vel_var, sz_vel_var );

  // - Additive Options
  int additive[ FXLPADD_NUM_SLOTS ];
  GET_INT( add_x_button,  additive[ FXLPADD_X  ] );
  GET_INT( add_y_button,  additive[ FXLPADD_Y  ] );
  GET_INT( add_z_button,  additive[ FXLPADD_Z  ] );
  GET_INT( add_h_button,  additive[ FXLPADD_H  ] );
  GET_INT( add_p_button,  additive[ FXLPADD_P  ] );
  GET_INT( add_b_button,  additive[ FXLPADD_B  ] );
  GET_INT( add_sx_button, additive[ FXLPADD_SX ] );
  GET_INT( add_sy_button, additive[ FXLPADD_SY ] );
  GET_INT( add_sz_button, additive[ FXLPADD_SZ ] );

  // Figure out how many objects we're processing
  int max_count;

  if( attach_mode == 0 ) {
    GET_INT( number_of_clones, max_count );
  } else {
    max_count = selected.NumElements();
  }

  // Loop through each one
  unsigned long current_selected = 0;
  char buffer[ 1024 ];
  const char *server_name;
  int i, j;

  LWProgressPanel progress( global, max_count );
  progress.SetAbortMessage( "Cancel Linking?", "Do you really want to abort linking?",
                            "Some objects may have already been affected" );
  progress.Open( "FX Linker Plus Linking..." );

  // Turn off Dynamic Update for speed
  int dynamic_update = ui->dynaUpdate;
  (*generic->evaluate)( generic->data, "DynamicUpdate 0" );

  srand( time( NULL ) );
  for( i=0; i < max_count; i++ ) {
    sprintf( buffer, "Linking Particle %d...", particle_index_value );
    if( !progress.Handle( i, buffer ) ) {
      (*message->info)( "FX Linker Plus:  Linking aborted by user", NULL );
      break;
    }

    // Select the item to apply the plug-in to
    sprintf( buffer, "SelectItem %x", selected[ current_selected ] );
    (*generic->evaluate)( generic->data, buffer );

    if( attach_mode == 0 )    // Clone the item, if needed
      (*generic->evaluate)( generic->data, "Clone 1" );

    // Re-obtain the UI state
    (LWInterfaceInfo *)(*global)( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
    ui = (LWInterfaceInfo *)(*global)( LWINTERFACEINFO_GLOBAL, GFUSE_ACQUIRE );

    // Look for FX Linker Plus Motion on the current object and remove it
    server_name = (const char *)-1;
    for( j=1; server_name != NULL; j++ ) {
      server_name = (*iteminfo->server)( ui->selItems[0], LWITEMMOTION_HCLASS, j );
      if( server_name == NULL )
        break;

      if( stricmp( server_name, MOTION_PLUGIN_NAME ) == 0 ) {
        sprintf( buffer, "RemoveServer %s %d", LWITEMMOTION_HCLASS, j );
        (*generic->evaluate)( generic->data, buffer );
        j--;      // Decrement since we just removed this server
      }
    }

    // Set up an FX Linker Instance and fill it in
    new_inst = new FXLinkerPlus_MotionInstance( LWITEM_NULL );

    new_inst->SetFromMode( (FXLP_FromModes)from_mode_value );
    new_inst->SetParticleItem( (item_index < (int)items.NumElements()) ? items[ item_index ]->GetID() : LWITEM_NULL );
    new_inst->SetParticleIndex( particle_index_value + (i * particle_step_value) );

    new_inst->SetStartFrame( start_frame_value - (long)( ((double)(start_frame_random_value * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );

    new_inst->SetParticleFile( particle_file_value, NULL );

    for( j=0; j < FXLPADD_NUM_SLOTS; j++ )
      new_inst->SetAdditive( (FXLP_AdditiveSlots)j, ((additive[j] == 0) ? false : true) );

    new_inst->SetUseAlignToPath( (align_to_path_value == 0 ? false : true) );
    new_inst->SetAlignLookAhead( align_look_ahead_value );
    new_inst->SetJitterDampening( jitter_dampening_value );
    new_inst->SetLinkRotationAxes( (link_rotation_axes_value == 0) ? false : true );

    new_inst->GetRotationVec()[0] = h + (h_var - ( ((double)(h_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetRotationVec()[1] = p + (p_var - ( ((double)(p_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetRotationVec()[2] = b + (b_var - ( ((double)(b_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );

    new_inst->GetRotationVelocityVec()[0] = (h_vel_var - ( ((double)(h_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetRotationVelocityVec()[1] = (p_vel_var - ( ((double)(p_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetRotationVelocityVec()[2] = (b_vel_var - ( ((double)(b_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );

    new_inst->SetUseSizeEffect( (size_effect_value == 0 ? false : true) );
    new_inst->SetSizeEffectMultiplier( size_effect_multiplier_value );
    new_inst->SetLinkScaleAxes( (link_scale_axes_value == 0) ? false : true );
    new_inst->SetKeepScaleAboveZero( (keep_scale_above_zero_value == 0) ? false : true );

    new_inst->GetScaleVec()[0] = sx + (sx_var - ( ((double)(sx_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetScaleVec()[1] = sy + (sy_var - ( ((double)(sy_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetScaleVec()[2] = sz + (sz_var - ( ((double)(sz_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );

    new_inst->GetScaleVelocityVec()[0] = (sx_vel_var - ( ((double)(sz_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetScaleVelocityVec()[1] = (sy_vel_var - ( ((double)(sy_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );
    new_inst->GetScaleVelocityVec()[2] = (sz_vel_var - ( ((double)(sz_vel_var * 2)) * ( ((double)rand()) /( (double)RAND_MAX) ) ) );

    if( attach_as == 0 ) {            // As Plug-in
      // Apply the plug-in to the object
      sprintf( buffer, "ApplyServer %s %s", LWITEMMOTION_HCLASS, MOTION_PLUGIN_NAME );
      (*generic->evaluate)( generic->data, buffer );
    } else {
      LWEnvelopeID envs[9], temp_envs[9];
      LWChannelID channels[9];
      const char *channel_names[]  = { "Position.X", "Position.Y", "Position.Z", "Rotation.H", "Rotation.P", "Rotation.B", "Scale.X", "Scale.Y", "Scale.Z" };
      const char *temp_env_names[] = { "FXLinkerPlusTempPosition.X", "FXLinkerPlusTempPosition.Y", "FXLinkerPlusTempPosition.Z", "FXLinkerPlusTempRotation.H", "FXLinkerPlusTempRotation.P", "FXLinkerPlusTempRotation.B", "FXLinkerPlusTempScale.X", "FXLinkerPlusTempScale.Y", "FXLinkerPlusTempScale.Z" };

      // Fetch the channel and envelope IDs
      LWChanGroupID group = (*iteminfo->chanGroup)( ui->selItems[0] );
      int i;
      for( LWChannelID next = (*chaninfo->nextChannel)( group, NULL ); next != NULL; next = (*chaninfo->nextChannel)( group, next) ) {
        const char *name = (*chaninfo->channelName)( next );
        assert( name != NULL );

        for( i=0; i < 9; i++ ) {
          if( strcmp( name, channel_names[i] ) == 0 ) {
            channels[i] = next;
            envs[i] = (*chaninfo->channelEnvelope)( channels[i] );
            break;
          }
        }
      }

      // Create the temp envs
      for( i=0; i < 9; i++ )
        temp_envs[i] = (*env_funcs->create)( group, temp_env_names[i], LWET_FLOAT );

      // Create keys in the temp envelopes
      double now;
      LWDVector position, rotation, scale;
      for( i = ui->previewStart; i <= ui->previewEnd; i += ui->previewStep ) {
        now = ((double)i)/sceneinfo->framesPerSecond;
        if( attach_as == 1 ) {           // As Keys from Motion
          (*iteminfo->param)( ui->selItems[0], LWIP_POSITION, now, position );
          (*iteminfo->param)( ui->selItems[0], LWIP_ROTATION, now, rotation );
          (*iteminfo->param)( ui->selItems[0], LWIP_SCALING,  now, scale    );
          EvaluateInstance( new_inst, now, i, position, rotation, scale );

        } else if( attach_as == 2 ) {    // As Keys from Channel
          position[0] = (*env_funcs->evaluate)( envs[0], now );
          position[1] = (*env_funcs->evaluate)( envs[1], now );
          position[2] = (*env_funcs->evaluate)( envs[2], now );
          rotation[0] = (*env_funcs->evaluate)( envs[3], now );
          rotation[1] = (*env_funcs->evaluate)( envs[4], now );
          rotation[2] = (*env_funcs->evaluate)( envs[5], now );
          scale[0]    = (*env_funcs->evaluate)( envs[6], now );
          scale[1]    = (*env_funcs->evaluate)( envs[7], now );
          scale[2]    = (*env_funcs->evaluate)( envs[8], now );

        } else if( attach_as == 3 ) {    // As Keys from Channel after Modifiers
          position[0] = (*chaninfo->channelEvaluate)( channels[0], now );
          position[1] = (*chaninfo->channelEvaluate)( channels[1], now );
          position[2] = (*chaninfo->channelEvaluate)( channels[2], now );
          rotation[0] = (*chaninfo->channelEvaluate)( channels[3], now );
          rotation[1] = (*chaninfo->channelEvaluate)( channels[4], now );
          rotation[2] = (*chaninfo->channelEvaluate)( channels[5], now );
          scale[0]    = (*chaninfo->channelEvaluate)( channels[6], now );
          scale[1]    = (*chaninfo->channelEvaluate)( channels[7], now );
          scale[2]    = (*chaninfo->channelEvaluate)( channels[8], now );
        }

        (*env_funcs->createKey)( temp_envs[0], now, position[0] );
        (*env_funcs->createKey)( temp_envs[1], now, position[1] );
        (*env_funcs->createKey)( temp_envs[2], now, position[2] );
        (*env_funcs->createKey)( temp_envs[3], now, rotation[0] );
        (*env_funcs->createKey)( temp_envs[4], now, rotation[4] );
        (*env_funcs->createKey)( temp_envs[5], now, rotation[5] );
        (*env_funcs->createKey)( temp_envs[6], now, scale[0]    );
        (*env_funcs->createKey)( temp_envs[7], now, scale[1]    );
        (*env_funcs->createKey)( temp_envs[8], now, scale[2]    );
      }

      // Copy the temp envelopes back into the channels and destroy the temp envs
      for( i=0; i < 9; i++ ) {
        CopyEnvelope( envs[i], temp_envs[i], (ui->previewStart * sceneinfo->framesPerSecond), (ui->previewEnd * sceneinfo->framesPerSecond), true );
        (*env_funcs->destroy)( temp_envs[i] );
      }

      delete new_inst;
      new_inst = NULL;
    }

    // Update the currently selected item
    if( ++current_selected >= selected.NumElements() )
      current_selected = 0;
  }

  // Restore Dynamic Update to it's previous state
  sprintf( buffer, "DynamicUpdate %d", dynamic_update );
  (*generic->evaluate)( generic->data, buffer );

  return true;
}

// CopyKey():
//  Creates a copy of a keyframe from one envelope into another envelope.
void FXLinkerPlus_Interface::CopyKey( LWEnvelopeID env_to, LWEnvelopeID env_from, LWEnvKeyframeID from ) {
  // Get the time and value
  double time, value;
  (*env_funcs->keyGet)( env_from, from, LWKEY_TIME,  &time  );
  (*env_funcs->keyGet)( env_from, from, LWKEY_VALUE, &value );

  // Create the new keyframe
  LWEnvKeyframeID to = (*env_funcs->createKey)( env_to, time, value );

  // Copy the key's shape
  int shape;
  (*env_funcs->keyGet)( env_from, from, LWKEY_SHAPE, &shape );
  (*env_funcs->keySet)( env_to,   to,   LWKEY_SHAPE, &shape );

  switch( shape ) {
    case 0:        // TCB
      (*env_funcs->keyGet)( env_from, from, LWKEY_TENSION, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_TENSION, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_CONTINUITY, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_CONTINUITY, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_BIAS, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_BIAS, &value );
      break;

    case 1:        // Hermite
    case 2:        // Bezier
      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_0, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_0, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_1, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_1, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_2, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_2, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_3, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_3, &value );
      break;

    case 3:        // Linear
      break;

    case 4:        // Stepped
      break;
  }
}

// CopyEnvelope():
//  Copies all the keyframes from one envelope into another between
//   start_time and end_time inclusive.  All existing keyframes within
//   the range will be deleted.  The inside flag inverts the range.
void FXLinkerPlus_Interface::CopyEnvelope( LWEnvelopeID to, LWEnvelopeID from, double start_time, double end_time, bool inside ) {
  ClearKeyRange( to, start_time, end_time, inside );

  double now;
  for( LWEnvKeyframeID key = (*env_funcs->nextKey)( from, NULL ); key != NULL; key = (*env_funcs->nextKey)( from, key ) ) {
    (*env_funcs->keyGet)( from, key, LWKEY_TIME, &now );
    if( ((now < start_time) &&  inside) ||
        ((now > start_time) && !inside) ) {
      continue;
    }

    if( ((now > end_time) &&  inside) ||
        ((now < end_time) && !inside) ) {
      continue;
    }

    CopyKey( to, from, key );
  }
}

// ClearKeyRange():
//  Deletes all keys in a channel between start_time and end_time
//   inclusive.  The inside flag inverts the range.
void FXLinkerPlus_Interface::ClearKeyRange( LWEnvelopeID env, double start_time, double end_time, bool inside ) {
  assert( start_time <= end_time );

  LWEnvKeyframeID next_key;
  double when;

  for( LWEnvKeyframeID key = (*env_funcs->nextKey)( env, NULL ); key != NULL; key = next_key ) {
    next_key = (*env_funcs->nextKey)( env, key );   // Get the next key now, since we might delete this one soon

    // See
    when = (*env_funcs->keyGet)( env, key, LWKEY_TIME, &when );
    if( ((when < start_time) &&  inside) ||
        ((when > start_time) && !inside) ) {
      continue;
    }

    if( ((when > end_time) &&  inside) ||
        ((when < end_time) && !inside) ) {
      continue;
    }

    (*env_funcs->destroyKey)( env, key );
  }
}
