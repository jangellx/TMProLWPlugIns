//
// AssignBoneVMaps-Interface.h
//

#define NULL 0        // Keeps LW SDK from chaing NULl to a void *

#include "AssignBoneVmaps.h"
#include <lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>

#include <lwxpanel.h>

#include <portable/dynarray/DynArray.h>

// AssignBoneVMaps_UI_IDs
enum AssignBoneVMaps_UI_IDs {
  ABVUI_ABOUT = 0x8000,
  ABVUI_ADJUST_BONE_SETTINGS,
  ABVUI_USE_WEIGHT_MAP_ONLY,
  ABVUI_WEIGHT_NORMALIZATION,

  ABVUI_GROUP_BONE_SETTINGS_TOGGLE,
  ABVUI_GROUP_BONE_SETTINGS };

// ProcessMode
enum ProcessMode {
  ABV_PROCESS_VMAPS = 0,
  ABV_PROCESS_ACTIVATE,
  ABV_PROCESS_DEACTIVATE,
  ABV_PROCESS_TOGGLE_ACTIVE,
  ABV_PROCESS_RECORD_PIVOTS,
  ABV_PROCESS_REST };

// ProcessModeStrings
static const char * ProcessModeStrings[] = {
  "AssignBoneVMaps",
  "Activate Bones",
  "Deactivate Bones",
  "Toggle Activate Bones" };

// AssignBoneVMaps_Interface():
class AssignBoneVMaps_Interface {
public:
  AssignBoneVMaps_Interface( GlobalFunc *_global, void *local );
  ~AssignBoneVMaps_Interface();

  bool TestInputs( ProcessMode process_mode );
  bool BuildItemList();
  void ReselectItems();

  // AssignBoneVMaps
  bool OpenUI();
  bool ObtainUIStates();

  bool IsOK()      { return ((retval != AFUNC_OK) ? false : is_ok); }
  int  GetRetval() { return retval; }
  bool UserAbort() { return user_abort; }

  // Activate/Deactivate/Toggle Bones
  bool Process( ProcessMode process_mode );
  bool ProcessBone( ProcessMode process_mode, LWItemID id );

public:
  int  retval;
  bool is_ok;
  bool user_abort;

  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWInterfaceInfo *ui;
  LWItemInfo      *iteminfo;
  LWBoneInfo      *boneinfo;
  LWObjectInfo    *objectinfo;

  LWXPanelFuncs   *xpanels;
  LWPanelFuncs    *panel_funcs;

  LWXPanelID       panel;

  TMPro_MiniLogo  *tmpro_logo;

  // Interface States (AssignBoneVMap)
  static bool adjust_bone_settings;
  static bool vmap_only;
  static bool normalize;

  // Processing Variables
  DynArray< LWItemID > items;
  LWItemType           sel_type;
};

void HandleAbout( LWXPanelID pan, int cid );
