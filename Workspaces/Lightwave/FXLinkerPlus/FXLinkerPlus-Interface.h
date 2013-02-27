//
// FXLinkerPlus-Interface.h
//

#include "FXLinkerPlus.h"
#include "FXLinkerPlus-Item.h"

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>

class FXLinkerPlus_MotionInstance;

// FXlinkerPlus_Interface
class FXLinkerPlus_Interface {
public:
  FXLinkerPlus_Interface( void *local );
  ~FXLinkerPlus_Interface();

  bool CreateUI( FXLinkerPlus_MotionInstance *_inst );
  bool OpenUI();
  void CloseUI();

  void AquireVariableGlobals();
  void ReleaseVariableGlobals();
  void ReaquireVariableGlobals();

  int BuildParticleItemList();

  bool Process();

  void CopyKey( LWEnvelopeID env_to, LWEnvelopeID env_from, LWEnvKeyframeID from );
  void CopyEnvelope( LWEnvelopeID to, LWEnvelopeID from, double start_time, double end_time, bool inside );
  void ClearKeyRange( LWEnvelopeID env, double start_time, double end_time, bool inside );

public:
  // Activation Data
  static GlobalFunc *global;
  LWLayoutGeneric   *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWInterfaceInfo *ui;
  LWItemInfo      *iteminfo;
  LWObjectInfo    *objectinfo;
  LWObjectFuncs   *objectfuncs;
  LWSceneInfo     *sceneinfo;
  LWInstUpdate    *instupdate;
  LWPSysFuncs     *particle_funcs;
  LWChannelInfo   *chaninfo;
  LWEnvelopeFuncs *env_funcs;

  LWPanelFuncs    *panel_funcs;

  // Main UI
  LWPanelID        panel;

  LWControl       *about;
  LWControl       *from_mode;

  LWControl       *item_popup;
  LWControl       *file_req;

  LWControl       *attach_to_popup;        // Generic Only
  LWControl       *number_of_clones;       // Generic Only
  LWControl       *attach_as_popup;        // Generic Only

  LWControl       *instance_popup;         // Motion Only

  LWControl       *particle_index;
  LWControl       *particle_step;          // Generic Only
  LWControl       *particle_info;          // Motion Only

  LWControl       *start_frame;
  LWControl       *start_frame_random;     // Generic Only

  LWControl       *align_to_path_toggle;
  LWControl       *align_look_ahead;
  LWControl       *jitter_dampening;
  LWControl       *link_rotation_axes;

  LWControl       *random_rotation_h;
  LWControl       *random_rotation_p;
  LWControl       *random_rotation_b;

  LWControl       *random_rotation_h_var;
  LWControl       *random_rotation_p_var;
  LWControl       *random_rotation_b_var;

  LWControl       *random_rotation_h_vel;
  LWControl       *random_rotation_p_vel;
  LWControl       *random_rotation_b_vel;

  LWControl       *random_rotation_h_vel_var;
  LWControl       *random_rotation_p_vel_var;
  LWControl       *random_rotation_b_vel_var;

  LWControl       *size_effect_toggle;
  LWControl       *size_effect_multiplier;
  LWControl       *random_scale_x;
  LWControl       *random_scale_y;
  LWControl       *random_scale_z;

  LWControl       *random_scale_x_var;
  LWControl       *random_scale_y_var;
  LWControl       *random_scale_z_var;

  LWControl       *random_scale_x_vel;
  LWControl       *random_scale_y_vel;
  LWControl       *random_scale_z_vel;

  LWControl       *random_scale_x_vel_var;
  LWControl       *random_scale_y_vel_var;
  LWControl       *random_scale_z_vel_var;

  LWControl       *keep_scale_above_zero;
  LWControl       *link_scale_axes;

  LWControl       *add_position_button;
  LWControl       *add_x_button;
  LWControl       *add_y_button;
  LWControl       *add_z_button;

  LWControl       *add_rotation_button;
  LWControl       *add_h_button;
  LWControl       *add_p_button;
  LWControl       *add_b_button;

  LWControl       *add_scale_button;
  LWControl       *add_sx_button;
  LWControl       *add_sy_button;
  LWControl       *add_sz_button;

  DynArray< FXLinkerPlus_Item * > items;
  DynArray< LWItemID            > selected;

  // Progress Panel
  LWPanelID        progress_panel;
  LWControl       *progress_field;

  int              progress_total;
  int              progress_current;
  char             progress_string[ MAX_PATH_LENGTH ];

  // Keyboard States
  bool             ctrl_down;
  bool             shift_down;

  FXLinkerPlus_MotionInstance *inst;
};

// Popup Callbacks
char * ItemPopup_Names( void * data, int index );
int    ItemPopup_Count( void * data );

char * InstPopup_Names( void * data, int index );
int    InstPopup_Count( void * data );

// Panel Callbacks
void HandlePanelOpen(     LWPanelID panel, void *data );
void HandlePanelClose(    LWPanelID panel, void *data );
void HandlePanelActivate( LWPanelID pan, void *data );
void HandleDrawPanel(     LWPanelID panel, void *data, DrMode mode);
void HandlePanelMouse(    LWPanelID, void *data, int flags, int x, int y );
void HandleKeys(          LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(        LWPanelID panel, void *data, LWDualKey key );

// Handlers
void HandleInstancePopup(        LWControl *con, void *data );

void HandleAbout(                LWControl *con, void *data );

void HandleFromMode(             LWControl *con, void *data );
void HandleItemPopup(            LWControl *con, void *data );
void HandleFileReq(              LWControl *con, void *data );

void HandleAttachTo(             LWControl *con, void *data );
void HandleAttachAs(             LWControl *con, void *data );

void HandleParticleIndex(        LWControl *con, void *data );

void HandleStartFrame(           LWControl *con, void *data );

void HandleAlignToPathToggle(    LWControl *con, void *data );
void HandleAlignLookAhead(       LWControl *con, void *data );
void HandleJitterDampening(      LWControl *con, void *data );

void HandleRandomRotationH(      LWControl *con, void *data );
void HandleRandomRotationP(      LWControl *con, void *data );
void HandleRandomRotationB(      LWControl *con, void *data );

void HandleRandomRotationHVel(   LWControl *con, void *data );
void HandleRandomRotationPVel(   LWControl *con, void *data );
void HandleRandomRotationBVel(   LWControl *con, void *data );

void HandleSizeEffectToggle(     LWControl *con, void *data );
void HandleSizeEffectMultiplier( LWControl *con, void *data );

void HandleRandomScaleX(         LWControl *con, void *data );
void HandleRandomScaleY(         LWControl *con, void *data );
void HandleRandomScaleZ(         LWControl *con, void *data );

void HandleRandomScaleXVel(      LWControl *con, void *data );
void HandleRandomScaleYVel(      LWControl *con, void *data );
void HandleRandomScaleZVel(      LWControl *con, void *data );

void HandleAddPosition(          LWControl *con, void *data );
void HandleAddPositionX(         LWControl *con, void *data );
void HandleAddPositionY(         LWControl *con, void *data );
void HandleAddPositionZ(         LWControl *con, void *data );

void HandleAddRotation(          LWControl *con, void *data );
void HandleAddRotationH(         LWControl *con, void *data );
void HandleAddRotationP(         LWControl *con, void *data );
void HandleAddRotationB(         LWControl *con, void *data );

void HandleAddScale(             LWControl *con, void *data );
void HandleAddScaleX(            LWControl *con, void *data );
void HandleAddScaleY(            LWControl *con, void *data );
void HandleAddScaleZ(            LWControl *con, void *data );

void HandleLinkRotation(         LWControl *con, void *data );
void HandleLinkScale(            LWControl *con, void *data );

