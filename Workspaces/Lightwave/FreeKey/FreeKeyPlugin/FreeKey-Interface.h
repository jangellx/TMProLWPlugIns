//
// FreeKey-Interface.h
//

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>           // Required so we can use the multimedia timer
#endif

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "../FreeKey/FreeKey.h"
#include "FreeKeyInterface-Dongle.h"

#include "FreeKeyInterface-ItemListDrawData.h"

// Defines
#define num_selection_toggles 3
#define num_selection_modes   6
#define num_group_edit_modes  4

static const double default_fps = 30.0;

/*   */
const int group_cur_r             =  132;
const int group_cur_g             =  132;
const int group_cur_b             =  116;
const int group_not_r             =  116;
const int group_not_g             =  132;
const int group_not_b             =  132;

const int item_height             =   16;
const int item_group_column       =    0;
const int item_group_column_width =   15;
const int item_type_column        =  item_group_column + item_group_column_width;
const int item_type_column_width  =   14;
const int item_text_column        =  item_type_column + item_type_column_width;
const int item_fold_column_width  =   15;
const int item_fold_column_end    =  item_text_column + item_fold_column_width;
const int item_depth_offset       =   10;
const int item_name_max_scroll    =  500;

const int item_visibility_offset  =  -32;
const int item_lock_offset        =  -16;
const int item_midcol_left_offset = item_visibility_offset;

const int label_width             =   40;
/*   */

// Custom Control Enums
enum fk_clicked {
  CLICKED_NONE,
  CLICKED_DIVIDER,
  CLICKED_NAME_SCROLL,
  CLICKED_KEY_SCROLL,
  CLICKED_KEY_EDGE_LEFT,
  CLICKED_KEY_EDGE_RIGHT };

enum fk_mousestate {
  MSTATE_DOWN,
  MSTATE_MOVING,
  MSTATE_UP };

enum fk_mousebutton {
  MBUTTON_LEFT,
  MBUTTON_RIGHT,
  MBUTTON_MIDDLE };

enum fk_status_mode {
  STATUSMODE_INFO = 0,    // Information Message (scene processed OK, scene loaded, etc.) -- default BG, light grey text
  STATUSMODE_NOTICE,      // Non-fatal message to get the users attention (loading scene, warnings) -- Orange/Yellow BG, black text
  STATUSMODE_WARNING = STATUSMODE_NOTICE,
  STATUSMODE_ERROR,       // Fatal errors (error loading scene; processing failure, etc.) -- Red BG, black text
  NUM_FK_STATUS_MODES };

class FreeKeyInterface;

extern FreeKeyInterface *fk_int;
extern FreeKey          *fk;

// FreeKeyInterface Class
class FreeKeyInterface {
public:
  FreeKeyInterface( GlobalFunc    *_global,
                    LayoutGeneric *_local,
                    int           &retval );
  ~FreeKeyInterface();

  bool OpenMainPanel();                          // Open the Main Panel
  void CloseMainPanel();                         // Close the Main Panel

  bool OpenAboutPanel();                         // Open the About Panel
  void CloseAboutPanel();                        // Close the About Panel

  bool OpenKeysPanel();                          // Open the Keyboard Help Panel
  void CloseKeysPanel();                         // Close the Keyboard Help Panel

//  bool OpenProcessPanel();                       // Open the Process Panel
//  void CloseProcessPanel();                      // Close the Process Panel

public:
  bool MakeMainPanel();                          // Create the main panel
  bool MakeMainControls();
  bool MakeItemSelectionControls();
  bool MakeKeySelectionControls();
  bool MakeGroupControls();
  bool MakeNumericControls();
  bool MakeOptionControls();

  bool MakeAboutPanel();                         // Create the about panel
  bool MakeKeysPanel();                          // Create the keyboard help panel

  bool ShowToolOptionsTab( fk_tool_option_tab_ids tab_id );  // Switch which tool tab is visible
  bool ShowItemSelectionTab();
  bool HideItemSelectionTab();
  bool ShowKeySelectionTab();
  bool HideKeySelectionTab();
  bool ShowGroupTab();
  bool HideGroupTab();
  bool ShowNumericTab();
  bool HideNumericTab();
  bool ShowOptionTab();
  bool HideOptionTab();

  bool MakeToolControls();

  void UpdateSelectedCount();

  void SetStatusText( const char *text, enum fk_status_mode mode = STATUSMODE_INFO, bool draw_now = false );

  void UpdateItemListType( bool use_opengl_item_list );      // Updates the item list drawing control (OpenGL or Panels)
  void RedrawItemList();                                     // Redraws the item list (both OpenGL and Panels)

  GlobalFunc    &Global;                         // Global Function
  LayoutGeneric *local;                          // Local data (LayoutGeneric)

  CLWPanelFuncs *panel_funcs;                    // Lightwave Panel Functions
  CLWSceneInfo  *scene_info;                     // Lightwave Scene Info
  CMessage      *message;                        // Lightwave User Message Functions

  LWPanelID      main_panel;                     // The Main Panel
  LWPanelID      about_panel;                    // The About Panel
  LWPanelID      keys_panel;                     // The Keyboard Help Panel

  // Main Panel Controls
  LWControl     *title_image;

  LWControl     *source_scene;
  LWControl     *source_popup;
  LWControl     *output_scene;
  LWControl     *output_popup;

  LWControl     *list_fold;
  LWControl     *list_unfold;
  LWControl     *select_count;
  LWControl     *display_key_mode;
  LWControl     *display_item_mode;

  LWControl     *item_first_frame;
  LWControl     *item_current_frame;
  LWControl     *item_jump_to_frame;
  LWControl     *item_last_frame;
  LWControl     *item_range_presets;

  LWControl     *opengl_item_list;
  LWControl     *item_list;
  LWControl     *item_scroll;

  LWControl     *item_zoom;
  LWControl     *item_multiscroll;

  LWControl     *tool_options_tabs;
  LWControl     *group_list;

  LWControl     *about;
  LWControl     *f1;

  LWControl     *status_line;
  LWControl     *freekey;

  // Tools
  LWControl     *tool_class_popup;

  // - Itme Selection Tab
  LWControl     *select_item_name;
  LWControl     *select_name_mode;
  LWControl     *select_children_toggle;
  LWControl     *select_toggle;
  LWControl     *select_item_mode;
  LWControl     *apply_select_item_name;
  LWControl     *select_info_area;

  // - Key Selection Tab
  LWControl     *key_select_info_area;

  // - Groups Tab
  LWControl     *group_name;
  LWControl     *group_delete;
  LWControl     *group_edit_mode;
  LWControl     *group_edit_apply;

  // - Options Tab
  LWControl     *opt_fractional_frames;
  LWControl     *opt_undo_levels;
  LWControl     *opt_draw_style;
  LWControl     *opt_use_opengl_item_list;
  LWControl     *opt_use_opengl_legacy;

  // Group-Level Options (soon to be obsolute)
  LWControl     *range_low;
  LWControl     *range_high;

  LWControl     *remove_mode;

  LWControl     *step_amount;
  LWControl     *step_mode;

  LWControl     *delta_amount;
  LWControl     *delta_mode;

  LWControl     *scale_amount;
  LWControl     *scale_mode;

  LWControl     *direction_mode;

  // Misc. Variables
  char           main_title[256];                   // Window Title Bar
  const char    *content_dir;

  DynArray< LWItem * >            sorted_items;     // Sorted version of the Item List stored in the FreeKey instance

  DualKey                         last_key_down;    // ID of the last key pressed
  bool                            shift_down;       // True if the shift key is currently being held down
  bool                            ctrl_down;        // True if the ctrl key is currently being held down
  bool                            alt_down;         // True if the alt key is currently being held down

  bool                            quickdraw_item_list;  // Quick-draws the item list in Keyframe mode (doesn't draw any keys) when scrolling

  int                             mouse_delta_y;    // Amount the mouse has moved on the y since it was clicked
  int                             mouse_delta_x;    // Amount the mouse has moved on the x since it was clicked
  int                             mouse_count;      // Number of times the mouse has moved since it was clicked
  int                             mouse_down_x;     // X position of the mouse when it was clicked
  int                             mouse_down_y;     // Y position of the mouse when it was clicked
  fk_mousestate                   mouse_state;      // State of the mouse (moving, down or up)
  fk_mousebutton                  mouse_button;     // The most recently depressed mouse button

  int                             bbox_item_base;   // The item the user clicked on when starting to bounding-box select
  int                             bbox_y1;          // Y position when the user started dragging, relative to the bbox_item_base item
  double                          bbox_t1;          // Start time when the user started dragging
  double                          bbox_t2;          // End time, where the user is now
  int                             bbox_y2;          // Y position when the user stopped dragging, relative to the top of the control
  bool                            draw_bbox;        // true if the bounding box should be drawn
  bool                            now_scrolling;    // true if the list is scrolling due to dragging the size of the bounding box near the edge of the item list

  // Processing Progress
  bool                            processing;       // True if currently processing a scene
  bool                            stop_processing;  // True if the processing has been aborted
  int                             progress_cur;     // Index of the current item being processed
  int                             progress_total;   // Total number of items to process

  char                            status_text[256]; // Status line text
  fk_status_mode                  status_mode;      // Status line mode (see enums above)

  // Misc. Functions
  void RefreshSource( const char * buffer, bool reload );  // Refreshes the interface with the latest source file

  void FillInFreeKeyGroup( unsigned long group );          // Fills a FreeKey instance with the current UI settings
  void FillWithFreeKeyGroup( unsigned long group );        // Fills in the UI settings with a FreeKey instance

  // Dongle Routines
  FreeKeyDongle                   *fkd;

  FKItemListDrawData               dd;              // Item List Draw Data
};


// Draw Functions
void DrawMainPanel(       LWPanelID panel, void *data, DrMode mode );
void DrawAboutPanel(      LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode );
void DrawKeysPanel(       LWPanelID panel, void *data, DrMode mode );

void DrawToolsPane( bool do_controls = false );

void DrawItemJumpToFrame(   LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawItemListbox(       LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawOpenGLItemListbox( LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawItemMultiScroll(   LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawStatusLine(        LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawSelectInfo(        LWControl *ctl, FreeKeyInterface *si, DrMode mode );
void DrawKeySelectInfo(     LWControl *ctl, FreeKeyInterface *si, DrMode mode );

void DrawMSDivider();
void DrawMSScrollLow();
bool DrawExtent( LWPanelID panel, CDrawFuncs *df,
                 int x, int y, int width, int height,
                 int min_x, int min_w,
                 int x_thickness, int y_thickness,
                 int shine_r,  int shine_g,  int shine_b,
                 int shadow_r, int shadow_g, int shadow_b,
                 int fill_r,   int fill_g,   int fill_b );
bool DrawKeyframeMark( LWPanelID panel, CDrawFuncs *df,
                       int x, int y, bool selected );

void DrawTitleImage( LWControl *ctl, FreeKeyInterface *si, DrMode mode );

// Listener Functions
void OnSourceFile(       LWControl *ctl, FreeKeyInterface *panel );
void OnSourcePopup(      LWControl *ctl, FreeKeyInterface *panel );
void OnOutputFile(       LWControl *ctl, FreeKeyInterface *panel );
void OnOutputPopup(      LWControl *ctl, FreeKeyInterface *panel );
void OnAbout(            LWControl *ctl, FreeKeyInterface *panel );
void OnFirstFrame(       LWControl *ctl, FreeKeyInterface *panel );
void OnLastFrame(        LWControl *ctl, FreeKeyInterface *panel );
void OnItemList(         LWControl *ctl, FreeKeyInterface *panel );
void OnOpenGLItemList(   LWControl *ctl, FreeKeyInterface *panel );
void OnItemScroll(       LWControl *ctl, FreeKeyInterface *panel );
void OnItemMultiScroll2( LWControl *ctl, FreeKeyInterface *panel );
void OnItemZoom(         LWControl *ctl, FreeKeyInterface *panel );
void OnFoldList(         LWControl *ctl, FreeKeyInterface *panel );
void OnUnFoldList(       LWControl *ctl, FreeKeyInterface *panel );
void OnDisplayMode(      LWControl *ctl, FreeKeyInterface *panel );
void OnKeyFrameMode(     LWControl *ctl, FreeKeyInterface *panel );
void OnToolOptionsTabs(  LWControl *ctl, FreeKeyInterface *panel );

void OnRangeLow(         LWControl *ctl, FreeKeyInterface *panel );
void OnRangeHigh(        LWControl *ctl, FreeKeyInterface *panel );
void OnItemRangePresets( LWControl *ctl, FreeKeyInterface *panel );
void OnRemoveMode(       LWControl *ctl, FreeKeyInterface *panel );
void OnStepAmount(       LWControl *ctl, FreeKeyInterface *panel );
void OnStepMode(         LWControl *ctl, FreeKeyInterface *panel );
void OnDeltaAmount(      LWControl *ctl, FreeKeyInterface *panel );
void OnDeltaMode(        LWControl *ctl, FreeKeyInterface *panel );
void OnScaleAmount(      LWControl *ctl, FreeKeyInterface *panel );
void OnScaleMode(        LWControl *ctl, FreeKeyInterface *panel );
void OnDirectionMode(    LWControl *ctl, FreeKeyInterface *panel );

void OnF1(               LWControl *ctl, FreeKeyInterface *panel );
void OnFreeKey(          LWControl *ctl, FreeKeyInterface *panel );

void OnSelectionMode(    LWControl *ctl, FreeKeyInterface *panel );
void OnApplySelection(   LWControl *ctl, FreeKeyInterface *panel );

void OnGroupList(        LWControl *ctl, FreeKeyInterface *panel );
void OnGroupName(        LWControl *ctl, FreeKeyInterface *panel );
void OnGroupDel(         LWControl *ctl, FreeKeyInterface *panel );
void OnGroupEditApply(   LWControl *ctl, FreeKeyInterface *panel );

void OnOptDrawStyle(         LWControl *ctl, FreeKeyInterface *panel );
void OnOptUseOpenGLItemList( LWControl *ctl, FreeKeyInterface *panel );
void OnOptUseOpenGLLegacy(   LWControl *ctl, FreeKeyInterface *panel );

void OnMouse(            LWPanelID panel, void *data, int click, int mx, int my );
void OnKeyDown(          LWPanelID panel, void *data, DualKey key);
void OnKeyUp(            LWPanelID panel, void *data, DualKey key);

// List/Popup Functions
char * ItemListerNames( void *dat, int index );
int    ItemListerCount( void *dat );
char * GroupNames( void *dat, int index );
int    GroupCount( void *dat );

void SetFoldState( LWItem *item, bool state, bool children );

// Selection Functions
void ToggleItem(               LWItem *item,  bool children, bool match_parent = true );
void SelectItem(               LWItem *item,  bool children );
void DeselectItem(             LWItem *item,  bool children );
void SelectItemByNameExact(    char *name,    int select,   bool children);
void SelectItemByNameStart(    char *name,    int select,   bool children);
void SelectItemByNameEnd(      char *name,    int select,   bool children);
void SelectItemByNameContain(  char *name,    int select,   bool children);
void InvertSelection();
void SelectAll();
void DeselectAll();
void SelectAllInGroup(         int group,     bool children );
void DeselectAllInGroup(       int group,     bool children );
void InvertAllInGroup(         int group,     bool children );
void SelectAllSelected(        bool children );
void DeselectAllSelected(      bool children );
void SelectAllUnselected(      bool children );
void DeselectAllUnselected(    bool children );
void ToggleRange(              int first, int last );
void SelectChildren();
void DeselectChildren();

// Group Functions
void SetSelectionToGroup( unsigned long group );
void SetItemToGroup( LWItem *item, unsigned long group,  bool children );
void ChangeGroup( LWItem *item, unsigned long old_group, unsigned long new_group, bool children );
void UpdateGroupGhosts( unsigned long group );

// Threads
//void __cdecl ProcessScene( void *dummy );
bool __cdecl UpdateProgress( char *name, int current, int total );

#ifdef WIN32
void CALLBACK ScrollItemListVert( UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 );
#endif

bool AnyAssignedToGroups( LWItem *item );
