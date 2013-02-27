//
// KeyStrainer-Interface.h
//

#include <portable/dynarray/dynarray.h>

#include "KeyStrainer.h"

#include <portable/dirtools/dirtools.h>
#include <Lightwave/LWItemHierarchy/LWItemHierarchy.h>

// KeyStrainer_Interface
class KeyStrainer_Interface {
public:
  KeyStrainer_Interface( GlobalFunc *_global, void *local );
  ~KeyStrainer_Interface();

  bool BuildItemList();
  void ReselectItems();

  int  OpenUI();
  int  OpenProgressPanel();

  bool Process();
  bool ProcessItem( LWItemHierarchy *item, int mode, int *channels, bool all_other_channels );

  void ReduceChannel(              LWChannelID channel, int mode );
  void ReduceByKeepKill(           LWChannelID channel );
  void ReduceByIterativeThreshold( LWChannelID channel );
  void ReduceBySmartFreeze(        LWChannelID channel );
  void ReduceByTotalFreeze(        LWChannelID channel );

  void ReduceForPreview();

  void CopyKey(       LWEnvelopeID env_to, LWEnvelopeID env_from, LWEnvKeyframeID from );
  void CopyEnvelope(  LWEnvelopeID to, LWEnvelopeID from, double start_time, double end_time, bool inside );

  void ClearEnvelope( LWEnvelopeID env );
  void ClearKeyRange( LWEnvelopeID env, double start_time, double end_time, bool inside );

public:
  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWInterfaceInfo *ui;
  LWItemInfo      *iteminfo;
  LWObjectInfo    *objectinfo;
  LWSceneInfo     *sceneinfo;
  LWChannelInfo   *chaninfo;
  LWEnvelopeFuncs *env_funcs;
  LWRasterFuncs   *raster;

  LWPanelFuncs    *panel_funcs;

  // Main UI
  LWPanelID        panel;

  LWControl       *mode_popup;
  LWControl       *preview_toggle;

  LWControl       *key_from_float;
  LWControl       *key_to_float;

  LWControl       *kill_step_float;
  LWControl       *keep_step_float;

  LWControl       *reduction_threshold_float;

  LWControl       *freeze_step_float;

  LWControl       *freeze_modifiers_toggle;
  LWControl       *freeze_key_shape;

  LWControl       *for_popup;
  LWControl       *about_button;

  LWControl       *position_button;
  LWControl       *x_button;
  LWControl       *y_button;
  LWControl       *z_button;

  LWControl       *rotation_button;
  LWControl       *h_button;
  LWControl       *p_button;
  LWControl       *b_button;

  LWControl       *scale_button;
  LWControl       *sx_button;
  LWControl       *sy_button;
  LWControl       *sz_button;

  LWControl       *all_other_channels_button;

  LWControl       *preview_show_original_curve;
  LWControl       *preview_show_reduced_curve;
  LWControl       *preview_show_original_keys;
  LWControl       *preview_show_reduced_keys;

  LWControl       *preview_channel;
  LWControl       *preview_canvas;

  // Progress Panel
  LWPanelID        progress_panel;
  LWControl       *progress_field;

  // Keyboard States
  bool             ctrl_down;
  bool             shift_down;

  // Processing Variables
  DynArray< LWItemID >    items;
  DynArray< LWChannelID > preview_channels;

  bool                 do_process;

  bool                 progress_open;

  int                  progress_total;
  int                  progress_current;
  char                 progress_string[ MAX_PATH_LENGTH ];

  LWEnvelopeID         workspace;
  LWChanGroupID        workspace_group;

  // Static State Variables
  static int for_state;
};

// Draw Callbacks
void DrawPanel( LWPanelID panel, void *data, DrMode mode);
void DrawPreview( LWControl *con, void *data, DrMode mode );
void DrawProgressField( LWControl *con, void *data, DrMode mode );

// Handlers
void HandleFromFloat(          LWControl *con, void *data );
void HandleToFloat(            LWControl *con, void *data );
void HandlePreviewToggle(      LWControl *con, void *data );
void HandleAboutButton(        LWControl *con, void *data );

void HandleModePopup(          LWControl *con, void *data );
void HandleKeepFloat(          LWControl *con, void *data );
void HandleKillFloat(          LWControl *con, void *data );
void HandleReductionThreshold( LWControl *con, void *data );
void HandleFreezeFloat(        LWControl *con, void *data );
void HandleFreezeKeyShape(     LWControl *con, void *data );
void HandleFreezeModifiers(    LWControl *con, void *data );

void HandlePreviewShowOriginalCurve( LWControl *con, void *data );
void HandlePreviewShowReducedCurve(  LWControl *con, void *data );
void HandlePreviewShowOriginalKeys(  LWControl *con, void *data );
void HandlePreviewShowReducedKeys(   LWControl *con, void *data );
void HandlePreviewChannel(           LWControl *con, void *data );

void HandlePositionButton(           LWControl *con, void *data );
void HandleRotationButton(           LWControl *con, void *data );
void HandleScaleButton(              LWControl *con, void *data );
void HandleAllOtherChannelsButton(   LWControl *con, void *data );

void HandleKeys(   LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key );

class KSKey {
  public:
    KSKey( double _time, double _value ) { time=_time; value=_value; }
    double value;
    double time;
};
