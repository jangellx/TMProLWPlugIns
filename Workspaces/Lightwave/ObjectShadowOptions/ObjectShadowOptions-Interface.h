//
// ObjectShadowOptions-Interface.h
//

#include "ObjectShadowOptions.h"
#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>
#include <lightwave/lwprogresspanel/lwprogresspanel.h>

// ObjectShadowOptions_UI_IDs
enum ObjectShadowOptions_UI_IDs {
  OUI_ABOUT = 0x8000,
  OUI_LIMIT_TO,
  OUI_MATCH_TOGGLE,
  OUI_MATCH_STRING,
  OUI_SELF_SHADOW,
  OUI_CAST_SHADOW,
  OUI_RECEIVE_SHADOW,
  OUI_UNSEEN_BY_RAYS,
  OUI_UNSEEN_BY_CAMERA,
  
  OUI_GROUP_LIMIT,
  OUI_GROUP_MATCH,
  OUI_GROUP_SHADOW,
  OUI_GROUP_RENDER };

// class ObjectShadowOptions_Interface
class ObjectShadowOptions_Interface {
public:
  ObjectShadowOptions_Interface( GlobalFunc *global, void *local );
  ~ObjectShadowOptions_Interface();

  bool TestInputs();
  void ReselectItems();

  bool OpenUI();
  bool ObtainUIStates();

  bool IsOK()      { return ((retval != AFUNC_OK) ? false : is_ok); }
  int  GetRetval() { return retval; }
  bool UserAbort() { return user_abort; }

  bool Process();
  bool ProcessObject( LWItemID item );
  void ReselectItems( int count );
  
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

  // Interface States
  static int  limit_to;
  static int  match_toggle;
  static char match_string[ 512 ];

  static int  self_shadow;
  static int  receive_shadow;
  static int  cast_shadow;

  static int  unseen_by_camera;
  static int  unseen_by_rays;

  // Processing Variables
  LWItemType           sel_type;

  LWProgressPanel      progress;
};

void HandleAbout( LWXPanelID pan, int cid );

XCALL_ (int)          LimitCount( void *data );
XCALL_ (const char *) LimitName(  void *data, int idx );
