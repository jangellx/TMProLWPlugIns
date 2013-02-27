//
// SelectOMatic-Interface.h
//

#ifndef NULL
#define NULL 0
#endif

#include "SelectOMatic.h"
#include "SelectOMatic-Data.h"

#include  <lwpanel.h>

#include "../paneltools/paneltoolsglobal.h"

typedef int LWCommandFunc( const char *cmd );

#define CONTEXT_MENU( cmenu, name, num, userdata ) (desc.type=LWT_POPUP, desc.popup.nameFn=name, desc.popup.countFn=num,\
                                                   (*cmenu->cmenuCreate)( &desc.popup, userdata ))

// class SelectOMatic_Interface
class SelectOMatic_Interface {
public:
  SelectOMatic_Interface( GlobalFunc *_global = NULL );
  ~SelectOMatic_Interface();

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
  LWItemInfo        * item_info;
  LWInterfaceInfo   * ui;
  LWMessageFuncs    * message;
  LWPanelFuncs      * panel_funcs;
  LWXPanelFuncs     * xpanels;
  LWRasterFuncs     * raster_funcs;
  TMP_PanelControls * panel_controls;
  TMP_RasterDraw    * raster_draw;
  LWCommandFunc     * command;
  ContextMenuFuncs  * context_funcs;

  LWPanelID           panel;

  LWControl         * display_mode_choice;

  LWControl         * sort_mode;
  LWControl         * sort_reverse;

  MultiTreeID         list; 

  LWControl         * select_tools;
  LWControl         * fetch;
  LWControl         * about;

  LWControl         * select_mode;

  LWControl         * reset;

  LWControl         * match_popup;
  LWControl         * match_case;
  LWControl         * match_string;

  LWControl         * skip_amount;
  LWControl         * keep_amount;

  LWControl         * random;

  LWControl         * execute;

  LWContextMenuID     list_menu;

  bool                is_open;

  bool                ctrl_down;
  bool                shift_down;

  // Interface States
  int                 list_width;
  int                 list_height;
  int                 panel_x;
  int                 panel_y;

  int                 select_tools_state;
  int                 select_mode_state;
  int                 match_popup_state;
  int                 match_case_state;
  char                match_string_state[ 256 ];
  int                 skip_state;
  int                 keep_state;
  double              random_state;

  bool                display_groups;

  int                 sort_mode_state;
  int                 sort_reverse_state;

  void              * prev_focus;
};

// Popup Callbacks
int    GroupCount( void * data );
char * GroupName(  void * data, int index );

// Event Handlers
void HandleDisplayMode( LWControl *con, void *data );
void HandleSortMode(    LWControl *con, void *data );
void HandleSelectTools( LWControl *con, void *data );
void HandleFetch(       LWControl *con, void *data );
void HandleAbout(       LWControl *con, void *data );

void HandleReset(       LWControl *con, void *data );
void HandleMatchPopup(  LWControl *con, void *data );

void HandleExecute(     LWControl *con, void *data );

// Panel Callbacks
void HandlePanelOpen(     LWPanelID panel, void *data );
void HandlePanelClose(    LWPanelID panel, void *data );
void HandlePanelActivate( LWPanelID pan, void *data );
void HandleDrawPanel(     LWPanelID panel, void *data, DrMode mode);
void HandlePanelMouse(    LWPanelID, void *data, int flags, int x, int y );
void HandleKeys(          LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(        LWPanelID panel, void *data, LWDualKey key );

// Multilist Callbacks
int                     List_Query(         MultiTreeID mt, void *user_data, void *item, MultiTree_Queries query );
void                  * List_NthChild(      MultiTreeID mt, void *user_data, void *item, int index );
const char            * List_CustomDraw(    MultiTreeID mt, void *user_data, void *item, int column, LWRasterID raster, MultiTree_DrawStages stage, int x, int y, int w, int h );
MulitTree_EventStates   List_ItemEvent(     MultiTreeID mt, void *user_data, void *item, int column, MultiTree_ItemEvents item_event );
MulitTree_EventStates   List_ClickEvent(    MultiTreeID mt, void *user_data, void *item, int column, int mflags, int mx, int my, int double_click );
void                  * List_DragDropEvent( MultiTreeID mt, void *user_data, void *item, void *target, MultiTree_DragDrops *drop_as, int apply );

// Sorting Functions
int (__cdecl QSortItemsByID)(           const void *arg1, const void *arg2 );
int (__cdecl QSortItemsByIDReverse)(    const void *arg1, const void *arg2 );
int (__cdecl QSortItemsByName)(         const void *arg1, const void *arg2 );
int (__cdecl QSortItemsByNameReverse)(  const void *arg1, const void *arg2 );
int (__cdecl QSortItemsByOrder)(        const void *arg1, const void *arg2 );
int (__cdecl QSortItemsByOrderReverse)( const void *arg1, const void *arg2 );

// Context Menu Callbacks
int    ListMenuCount( void *userdata );
char * ListMenuName(  void *userdata, int index );

void ChangeSelectionByItem( const char *first_command, const char *successive_command, SelectOMatic_Item *the_item );
