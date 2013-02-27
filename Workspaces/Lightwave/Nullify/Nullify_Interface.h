//
// Nullify-Interface.h
//

#define NULL 0        // Keeps LW SDK from chaing NULl to a void *

#include "Nullify.h"

#include <lwxpanel.h>

#include <portable/dynarray/DynArray.h>

#include <Lightwave\LWItemHierarchy\LWItemHierarchy.h>
#include <lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>
#include <lightwave/lwprogresspanel/lwprogresspanel.h>

// Nullify_UI_IDs
enum Nullify_UI_IDs {
  NUI_ABOUT = 0x8000,
  NUI_PREFIX,
  NUI_SUFFIX,
  NUI_NULLIFY_DECENDANTS,
  NUI_REMOVE_BONES,
  NUI_DISABLE_SHADOWS,
  NUI_STORE_ORIGINAL_NAME,
  NUI_REMOVE_MORPH_MIXER,
  
  NUI_GROUP_NAME,
  NUI_GROUP_OPTIONS };

// Nullify_Interface():
class Nullify_Interface {
public:
  Nullify_Interface( GlobalFunc *_global, void *local );
  ~Nullify_Interface();

  bool TestInputs();
  bool BuildItemList();
  void ReselectItems();

  // Nullify
  bool OpenUI();
  bool ObtainUIStates();

  bool IsOK()      { return ((retval != AFUNC_OK) ? false : is_ok); }
  int  GetRetval() { return retval; }
  bool UserAbort() { return user_abort; }

  // Activate/Deactivate/Toggle Bones
  bool Process();
  bool ProcessObject( LWItemHierarchy *object );

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
  static bool nullify_decendants;
  static bool remove_bones;

  static bool disable_shadows;

  static char prefix[256];
  static char suffix[256];

  static bool store_original_name;

  static bool remove_morph_mixer;

  // Processing Variables
  DynArray< LWItemID > items;
  LWItemType           sel_type;

  LWProgressPanel      progress;
};

void HandleAbout( LWXPanelID pan, int cid );
