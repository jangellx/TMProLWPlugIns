//
// PanelControls-MultiTree.h
//

#ifndef NULL
#define NULL 0
#endif

#include <portable/dynarray/dynarray.h>

#include "PanelControls-Scrollbar.h"
#include "PanelToolsGlobal.h"

typedef void                  * MultiTreeID;
typedef int                     MultiTree_Query          ( MultiTreeID mt, void *user_data, void *item, MultiTree_Queries query );
typedef MulitTree_EventStates   MultiTree_ClickEvent     ( MultiTreeID mt, void *user_data, void *item, int column, int mflags, int mx, int my, int double_click );
typedef MulitTree_EventStates   MultiTree_ItemEvent      ( MultiTreeID mt, void *user_data, void *item, int column, MultiTree_ItemEvents item_event );
typedef void                  * MultiTree_DragDropEvent  ( MultiTreeID mt, void *user_data, void *item, void *target, MultiTree_DragDrops *drop_as, int apply );
typedef const char            * MultiTree_CustomDraw     ( MultiTreeID mt, void *user_data, void *item, int column, LWRasterID raster, MultiTree_DrawStages stage, int x, int y, int w, int h );
typedef int                     MultiTree_AutosizeColumn ( MultiTreeID mt, void *user_data, int column );
typedef int                     MultiTree_ResizeColumn   ( MultiTreeID mt, void *user_data, int column, int w );
typedef void                  * MultiTree_NextChild      ( MultiTreeID mt, void *user_data, void *item, void *child );
typedef void                  * MultiTree_NthChild       ( MultiTreeID mt, void *user_data, void *item, int index );

enum MultiTree_FocusStates {
  MULTITREEFOCUS_NONE,
  MULTITREEFOCUS_CLICK,
  MULTITREEFOCUS_DRAG_DROP,
  MULTITREEFOCUS_RESIZE_COLUMN,
  MULTITREEFOCUS_FINISHED,
};

class PanelControls_MultiTreeItem;

const int title_height = 18;

// class PanelControls_MultiTree
class PanelControls_MultiTree {
public:
  PanelControls_MultiTree( GlobalFunc *_global, LWPanelID _panel, int w, int h, int *default_column_widths, int _tree_column, bool hscroll, bool vscroll );
  ~PanelControls_MultiTree();

  // Management
         void       * Handle( void * prev_focus, int flags, int mx, int my );

         void         Refresh();
         void         Redraw();
         void         BuildUnfoldedItemList( void *item, int depth );

         void         ScrollToItem( void *item );
         void         ScrollToIndex( int index );

  inline LWControl  * GetListControl()    { return list_control;    }

  inline void         SetUserData(       void                     *_user_data ) { user_data        = _user_data; }
  inline void         SetQuery(          MultiTree_Query          *_event     ) { query            = _event;     }
  inline void         SetClickEvent(     MultiTree_ClickEvent     *_event     ) { click_event      = _event;     }
  inline void         SetItemEvent(      MultiTree_ItemEvent      *_event     ) { item_event       = _event;     }
  inline void         SetDragDropEvent(  MultiTree_DragDropEvent  *_event     ) { drag_drop_event  = _event;     }
  inline void         SetCustomDraw(     MultiTree_CustomDraw     *_event     ) { custom_draw      = _event;     }
  inline void         SetAutosizeColumn( MultiTree_AutosizeColumn *_event     ) { autosize_column  = _event;     }
  inline void         SetResizeColumn(   MultiTree_ResizeColumn   *_event     ) { resize_column    = _event;     }
  inline void         SetNextChild(      MultiTree_NextChild      *_event     ) { next_child       = _event;     }
  inline void         SetNthChild(       MultiTree_NthChild       *_event     ) { nth_child        = _event;     }

  inline void *       GetFirstVisibleItem()               { int pos = hscroll_control->GetKnobPosition(); return ((pos < (int)items.NumElements()) ? items[0] : NULL); }

         int          GetNumVisibleItems();
  inline int          GetNumItems()                       { return (int)items.NumElements(); }

  inline int          GetRowHeight()                      { return (panel_funcs->drawFuncs->dispMetrics()->textHeight + 2); }
  inline int          GetTreeColumn()                     { return tree_column; }

  inline void         SetColumnWidth( int column, int w );
  inline int          GetColumnWidth( int column )        { return column_widths[ column ]; }

public:
  LWPanelFuncs              * panel_funcs;
  LWRasterFuncs             * raster_funcs;

  LWPanelID                   panel;
  LWControl                 * list_control;
  PanelControls_Scrollbar   * hscroll_control;
  PanelControls_Scrollbar   * vscroll_control;

  MultiTree_Query           * query;
  MultiTree_ClickEvent      * click_event;
  MultiTree_ItemEvent       * item_event;
  MultiTree_DragDropEvent   * drag_drop_event;
  MultiTree_CustomDraw      * custom_draw;
  MultiTree_AutosizeColumn  * autosize_column;
  MultiTree_ResizeColumn    * resize_column;
  MultiTree_NextChild       * next_child;
  MultiTree_NthChild        * nth_child;

  void                      * user_data;

  int                         tree_column;

  MultiTree_FocusStates       focus;

  void                      * drag_drop_source;
  void                      * drag_drop_target;
  MultiTree_DragDrops         drop_as;

  int                         mouse_down_x;
  int                         mouse_down_y;

  int                         prev_selected;
  unsigned long               prev_column;
  int                         prev_column_width;
  bool                        defer_selection;

  DynArray< int >                           column_widths;
  DynArray< PanelControls_MultiTreeItem * > items;

  GlobalFunc                * global;
};

// SetColumnWidth():
inline void PanelControls_MultiTree::SetColumnWidth( int column, int w ) {
  if( hscroll_control != NULL ) {
    int old_w = hscroll_control->GetMax();
    old_w -= column_widths[ column ];
    hscroll_control->SetMax( old_w + w );
  }

  column_widths[ column ] = w;
}

// Draw Functions
void MultiTree_ControlDraw( LWControl *con, void *data, DrMode mode );

// Event Handlers
void MultiTree_HScrollEvent( void *user_data, int position );
void MultiTree_VScrollEvent( void *user_data, int position );

// class PanelControls_MultiTreeItem
class PanelControls_MultiTreeItem {
public:
  PanelControls_MultiTreeItem( void *_item, int _depth, bool _is_foldable ) : item(_item), depth(_depth), is_foldable(_is_foldable) { ; }
  ~PanelControls_MultiTreeItem() { ; }

  // Accessors
  inline void  * GetItem()        { return item; }
  inline int     GetDepth()       { return depth; }
  inline bool    GetIsFoldable()  { return is_foldable; }

protected:
  void         * item;

  int            depth;
  bool           is_foldable;   // If true, the item can be folded
};

