//
// Motify-Interface.h
//

#include <portable/dynarray/dynarray.h>

#include "Motify.h"

#include <portable/dirtools/dirtools.h>
#include <Lightwave/LWItemHierarchy/LWItemHierarchy.h>

// Motify_Interface
class Motify_Interface {
public:
  Motify_Interface( GlobalFunc *_global, void *local );
  ~Motify_Interface();

  bool BuildItemList();
  void ReselectItems();

  int  OpenUI();
  int  OpenProgressPanel();

  bool Process();
  bool ProcessItem( LWItemHierarchy *item, int mode, int *channels,
                    double from, double to, double threshold,
                    int protection, bool all_other_channels );

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

  LWPanelFuncs    *panel_funcs;

  // Main UI
  LWPanelID        panel;

  LWControl       *mode_popup;
  LWControl       *protect_popup;

  LWControl       *delete_key_from_float;
  LWControl       *delete_key_to_float;
  LWControl       *delete_key_threshold_float;
  LWControl       *delete_key_thresholds_popdown;

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

  // Progress Panel
  LWPanelID        progress_panel;
  LWControl       *progress_field;

  // Keyboard States
  bool             ctrl_down;
  bool             shift_down;

  // Processing Variables
  DynArray< LWItemID > items;
  LWItemType           sel_type;

  bool                 do_process;

  bool                 progress_open;

  int                  progress_total;
  int                  progress_current;
  char                 progress_string[ MAX_PATH_LENGTH ];

  // Static State Variables
  static int for_state;
  static int protect_state;
};

// Draw Callbakcs
void DrawPanel( LWPanelID panel, void *data, DrMode mode);
void DrawProgressField( LWControl *con, void *data, DrMode mode );

// Event Handlers
void HandleModePopup(        LWControl *con, void *data );
void HandleFromFloat(        LWControl *con, void *data );
void HandleToFloat(          LWControl *con, void *data );
void HandleThresholdPopdown( LWControl *con, void *data );
void HandleAboutButton(      LWControl *con, void *data );
void HandlePositionButton(   LWControl *con, void *data );
void HandleRotationButton(   LWControl *con, void *data );
void HandleScaleButton(      LWControl *con, void *data );
void HandleAllOtherChannelsButton( LWControl *con, void *data );

void HandleKeys(             LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(           LWPanelID panel, void *data, LWDualKey key );
