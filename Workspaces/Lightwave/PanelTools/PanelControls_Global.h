//
// PanelControls_Global.h
//

#include "PanelControls-WPopdown.h"
#include "PanelControls-MultiTree.h"

extern TMP_PanelControls panel_control_functions;

extern "C" {
  // WPopdown
  WPopdownID        pc_wPopdown                  ( GlobalFunc *global, LWPanelID panel, int width,
                                                   const char *title, const char **strings );
  WPopdownID        pc_wPopdownCustom            ( GlobalFunc *global, LWPanelID panel, int width,
                                                   const char *title, WPopdown_Name *name, WPopdown_Count *count );
  void              pc_wPopdownSetEvent          ( WPopdownID priv, WPopdown_Event *event, void *user_data );

  void              pc_wPopdownOpen              ( WPopdownID priv, int force );

  LWControlID       pc_wPopdownGetControl        ( WPopdownID priv );
  void              pc_wPopdownDestroy           ( WPopdownID priv );

  // Scrollbar
  ScrollbarID       pc_hScrollbar                ( GlobalFunc *global, LWPanelID panel, int length );
  ScrollbarID       pc_vScrollbar                ( GlobalFunc *global, LWPanelID panel, int length );

  void              pc_scrollbarSetEvent         ( ScrollbarID priv, Scrollbar_Event *event, void *user_data );

  int               pc_scrollbarGetKnobPosition  ( ScrollbarID priv );
  void              pc_scrollbarSetKnobPosition  ( ScrollbarID priv, int position );

  int               pc_scrollbarGetKnobSize      ( ScrollbarID priv );
  void              pc_scrollbarSetKnobSize      ( ScrollbarID priv, int size );

  int               pc_scrollbarGetMin           ( ScrollbarID priv );
  int               pc_scrollbarGetMax           ( ScrollbarID priv );
  void              pc_scrollbarSetMin           ( ScrollbarID priv, int min );
  void              pc_scrollbarSetMax           ( ScrollbarID priv, int max );
  void              pc_scrollbarSetMinMax        ( ScrollbarID priv, int min, int max );

  void *            pc_scrollbarHandle           ( ScrollbarID priv, void *prev_focus, int mx, int my, int flags );

  LWControlID       pc_scrollbarGetControl       ( ScrollbarID priv );
  void              pc_scrollbarDestroy          ( ScrollbarID priv );

  // MultiTree
  MultiTreeID       pc_multiTree                 ( GlobalFunc *global, LWPanelID panel, int w, int h,
                                                   int *default_column_widths, int tree_column, int hscroll, int vscroll );

  void              pc_multiTreeSetUserData      ( MultiTreeID priv, void *user_data );
  void              pc_multiTree_Query           ( MultiTreeID priv, MultiTree_Query          event );
  void              pc_multiTree_ClickEvent      ( MultiTreeID priv, MultiTree_ClickEvent     event );
  void              pc_multiTree_ItemEvent       ( MultiTreeID priv, MultiTree_ItemEvent      event );
  void              pc_multiTree_DragDropEvent   ( MultiTreeID priv, MultiTree_DragDropEvent  event );
  void              pc_multiTree_CustomDraw      ( MultiTreeID priv, MultiTree_CustomDraw     event );
  void              pc_multiTree_AutosizeColumn  ( MultiTreeID priv, MultiTree_AutosizeColumn event );
  void              pc_multiTree_ResizeColumn    ( MultiTreeID priv, MultiTree_ResizeColumn   event );
  void              pc_multiTree_NextChild       ( MultiTreeID priv, MultiTree_NextChild      event );
  void              pc_multiTree_NthChild        ( MultiTreeID priv, MultiTree_NthChild       event );

  void              pc_multiTreeRefresh          ( MultiTreeID priv );
  void              pc_multiTreeRedraw           ( MultiTreeID priv );

  int               pc_multiTreeGetNumItems      ( MultiTreeID priv );

  void *            pc_multiTreeGetFirstVisibleItem ( MultiTreeID priv );
  int               pc_multiTreeGetNumVisibleItems  ( MultiTreeID priv );

  void              pc_multiTreeScrollToItem     ( MultiTreeID priv, void *item );
  void              pc_multiTreeScrollToIndex    ( MultiTreeID priv, int index );

  int               pc_multiTreeGetRowHeight     ( MultiTreeID priv );

  int               pc_multiTreeGetColumnWidth   ( MultiTreeID priv, int column );
  void              pc_multiTreeSetColumnWidth   ( MultiTreeID priv, int column, int width );

  void *            pc_multiTreeHandle           ( MultiTreeID priv, void *prev_focus, int flags, int mx, int my );

  LWControlID       pc_multiTreeGetControl       ( MultiTreeID priv );

  void              pc_multiTreeDestroy          ( MultiTreeID priv );
}

