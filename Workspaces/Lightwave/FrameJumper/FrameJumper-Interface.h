//
// FrameJumper-Interface.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

class FrameJumper_Interface {
public:
  FrameJumper_Interface( GlobalFunc *_global = NULL );
  ~FrameJumper_Interface();

  void CreateUI();

  void OpenUI();

  bool IsOpen() { return is_open; }

  AquireVariableGlobals();
  ReleaseVariableGlobals();
  ReaquireVariableGlobals();

public:
  static GlobalFunc * global;

  LWInstUpdate      * inst_update;
  LWSceneInfo       * scene_info;
  LWInterfaceInfo   * ui;
  LWMessageFuncs    * message;
  LWPanelFuncs      * panel_funcs;
  
  LWPanelID           panel;

  LWControl         * hide_disabled;
  LWControl         * units;

  LWControl         * group_name;
  LWControl         * group_popup;

  LWControl         * list;
  LWControl         * about;
  LWControl         * modify;

  LWControl         * add;
  LWControl         * remove;

  LWControl         * name;
  LWControl         * key;
  LWControl         * time;
  LWControl         * enable;
  LWControl         * in_group;

  const char        * checkmark_string;

  bool                is_open;

  bool                ctrl_down;
  bool                shift_down;

  // Interface States
  int                 list_height;
  int                 panel_x;
  int                 panel_y;
  int                 modify_state;
  int                 units_state;
  int                 current_marker;

  bool                return_true_count;
};

// Multilist Functions
int    ListCount(        LWPanelID panel );
char * ListNames(        LWPanelID panel, int item, int col );
int    ListColumnWidths( LWPanelID panel, int col );

// Panel Callbacks
void HandlePanelOpen(     LWPanelID panel, void *data );
void HandlePanelClose(    LWPanelID panel, void *data );
void HandlePanelActivate( LWPanelID pan, void *data );
void HandleDrawPanel(     LWPanelID panel, void *data, DrMode mode);
void HandlePanelMouse(    LWPanelID, void *data, int flags, int x, int y );
void HandleKeys(          LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(        LWPanelID panel, void *data, LWDualKey key );

// Event Handlers
void HandleUnits(      LWControl *con, void *data );

void HandleGroupPopup( LWControl *con, void *data );
void HandleGroupName(  LWControl *con, void *data );

void HandleList(       LWControl *con, void *data );
void HandleAbout(      LWControl *con, void *data );
void HandleModify(     LWControl *con, void *data );

void HandleAdd(        LWControl *con, void *data );
void HandleRemove(     LWControl *con, void *data );

void HandleName(       LWControl *con, void *data );
void HandleKey(        LWControl *con, void *data );
void HandleTime(       LWControl *con, void *data );
void HandleEnable(     LWControl *con, void *data );
void HandleInGroup(    LWControl *con, void *data );

// Popup Callbacks
int    GroupCount( void * data );
char * GroupName(  void * data, int index );

int    InGroupCount( void * data );
char * InGroupName(  void * data, int index );

// QSort Callbacks
int (__cdecl SortMarkersByTime)( const void *arg1, const void *arg2 );
