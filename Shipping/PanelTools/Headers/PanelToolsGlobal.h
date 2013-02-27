/*
 * PanelToolsGlobal.h
 */

#ifndef TMP_PANEL_TOOLS_GLOBAL_HEADER
#define TMP_PANEL_TOOLS_GLOBAL_HEADER

#include <lwpanel.h>

#define TMP_PANELTOOLS_GLOBAL    "TM-P_PanelTools"
#define TMP_PANELCONTROLS_GLOBAL "TM-P_PanelControls"
#define TMP_PANELPROGRESS_GLOBAL "TM-P_PanelProgress"
#define TMP_PANELDRAW_GLOBAL     "TM-P_PanelDraw"
#define TMP_RASTERDRAW_GLOBAL    "TM-P_RasterDraw"

/*
 *   TMP_PanelTools:
 *    This structure is returned by the TMP_PanelTools global.  Use these functions
 *     to align controls relative to other controls.
 */
typedef struct st_TMP_PanelTools {
  int (*alignLabels)               ( LWControlID reference, LWControlID, int padding );
  int (*alignLabelsArray)          ( LWControlID *array, int padding );

  int (*alignWith)                 ( LWControlID reference, LWControlID  );
  int (*alignWithArray)            ( LWControlID *array );

  int (*centerUnder)               ( LWControlID reference, LWControlID, int padding, int put_under );
  int (*centerUnderArray)          ( LWControlID *array, int padding );

  int (*putUnder)                  ( LWControlID reference, LWControlID, int padding );
  int (*putUnderArray)             ( LWControlID *array, int padding );

  int (*putOnLineWith)             ( LWControlID reference, LWControlID, int padding );
  int (*putOnLineWithArray)        ( LWControlID *array, int padding );

  int (*rightAlign)                ( LWControlID reference, LWControlID, int h_padding, int put_under, int v_padding );
  int (*rightAlignArray)           ( LWControlID *array, int h_padding, int put_under, int v_padding );

  int (*rightAlignWithPanel)       ( LWPanelFuncs *, LWPanelID, LWControlID, int h_padding );
  int (*rightAlignWithPanelArray)  ( LWPanelFuncs *, LWPanelID, LWControlID *array, int h_padding, int put_under, int v_padding );

  int (*bottomAlign)               ( LWControlID reference, LWControlID, int padding );
  int (*bottomAlignArray)          ( LWControlID *array, int padding );

  int (*putOver)                   ( LWControlID reference, LWControlID, int padding );
  int (*putOverArray)              ( LWControlID *array, int padding );

  int (*nudge)                     ( LWControlID control, int x, int y );
  int (*nudgeArray)                ( LWControlID *array, int x, int y );
} TMP_PanelTools;

/*
 *   TMP_PanelControls:
 *    This structure is returned by the TMP_PanelControls global.
 */
typedef void * WPopdownID;
typedef int    WPopdown_Count ( WPopdownID popdown, void *user_data );
typedef char * WPopdown_Name  ( WPopdownID popdown, void *user_data, int index );
typedef void   WPopdown_Event ( WPopdownID popdown, void *user_data, int index );

typedef void * ScrollbarID;
typedef void   Scrollbar_Event ( ScrollbarID user_data, int position );

typedef enum en_MultiTree_DrawStages {
  MULTITREEDRAW_FIRST,                   /* Before anything has been drawn, but after the raster has been cleared.  Raster is the size of the entire row in this column.  Return non-NULL if you don't want the list to do any more drawing for this item                   */
  MULTITREEDRAW_ICON,                    /* Icon between the fold icons and the text.  Return non-NULL to reserve the space for the icon if you want to draw there, or NULL if you don't want an icon.  Raster is the size of the icon                                      */
  MULTITREEDRAW_TEXT,                    /* Return NULL to draw the text the text yourself, or a pointer to an ASCII string if the control should draw it.  Raster is the size of everything left of the fold icons and the icon, if applicable, to the edge of the column. */
  MULTITREEDRAW_LAST,                    /* After everything has been drawn.                                                                                                                                                                                                */
} MultiTree_DrawStages;

typedef enum en_MultiTree_ItemEvents {
  MULTITREEITEM_SELECT,                  /* Select only this item; clear the existing selection */
  MULTITREEITEM_ADD_TO_SELECTION,        /* Add this item to the selection                      */
  MULTITREEITEM_DESELECT,                /* Remove this item from the selection                 */
  MULTITREEITEM_FOLD,                    /* Fold this item (hide children)                      */
  MULTITREEITEM_UNFOLD,                  /* Unfold this item (show children)                    */
  MULTITREEITEM_DONE_SELECTING,          /* Sent after all selection events have completed      */
} MultiTree_ItemEvents;

typedef enum en_MultiTree_Queries {
  MULTITREEQUERY_IS_SELECTED,            /* Return 1 if selected, and 0 if not                                             */
  MULTITREEQUERY_IS_FOLDABLE,            /* Return 1 if this item can be folded (ie: it has children)                      */
  MULTITREEQUERY_IS_FOLDED,              /* Return 1 if folded (children aren't visible or no children), and 0 if unfolded */
} MultiTree_Queries;

typedef enum en_MultiTree_DragDrops {
  MULTITREEDRAGDROP_AS_CHILD,            /* Insert the item as a child of the target */
  MULTITREEDRAGDROP_AS_INSERTION_AFTER,  /* Insert the item after the target         */
  MULTITREEDRAGDROP_AS_INSERTION_BEFORE, /* Insert the item before the target        */
  MULTITREEDRAGDROP_NONE,                /* Don't drag/drop item                     */
} MultiTree_DragDrops;

enum MulitTree_EventStates {
  MULTITREEEVENT_CONTINUE,               /* Returned if the callback didn't exclusively handle the event                                                                                */
  MULTITREEEVENT_HANDLED,                /* Returned if the event has been handled and event processing should stop here, but dragging events should still occur                        */
  MULTITREEEVENT_FINISHED,               /* Returned if the callback handled the event completely, and doesn't want any kind of drag or other events to occur (like dragging)           */
  MULTITREEEVENT_LOSE_FOCUS,             /* Returned if the the list should lose focus as though the mouse button were released. Use this after opening a context menu from a callback. */
};

#define MULTITREE_CHECKMARK ((const char *)0xFFFFFFFF)

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

typedef struct st_TMP_PanelControls {
  /* WPopdown Controls  */
  WPopdownID        (*wPopdown)                 ( GlobalFunc *global, LWPanelID panel, int width,                  /* Pop-down control with a user-defined width.  Return value should be passed to the other popdown functions  */
                                                  const char *title, const char **strings );
  WPopdownID        (*wPopdownCustom)           ( GlobalFunc *global, LWPanelID panel, int width,                  /* wPopdown control using callbacks for the list contents.                                                    */
                                                  const char *title, WPopdown_Name *name, WPopdown_Count *count );
  void              (*wPopdownSetEvent)         ( WPopdownID priv, WPopdown_Event *event, void *user_data );       /* Set the event handler and user data                                                  */
  void              (*wPopdownOpen)             ( WPopdownID priv, int force );                                    /* Opens the popdown as though the user had clicked on it                               */
  LWControlID       (*wPopdownGetControl)       ( WPopdownID priv );                                               /* Returns a LWControlID, which can be used to ghost, erase or reposition the control   */
  void              (*wPopdownDestroy)          ( WPopdownID priv );                                               /* Destroy a popdown.  Call this after destroying your panel                            */

  /* Scrollbar Controls */
  ScrollbarID       (*hScrollbar)               ( GlobalFunc *global, LWPanelID panel, int length );               /* Create a horizontal scrollbar */
  ScrollbarID       (*vScrollbar)               ( GlobalFunc *global, LWPanelID panel, int length );               /* Create a vertical scrollbar   */

  void              (*scrollbarSetEvent)        ( ScrollbarID priv, Scrollbar_Event *event, void *user_data );     /* Set the event handler and user data   */

  int               (*scrollbarGetKnobPosition) ( ScrollbarID priv );                                              /* Get the current scrollbar position (topmost visible item in a list) */
  void              (*scrollbarSetKnobPosition) ( ScrollbarID priv, int position );                                /* Set the current scrollbar position                                  */

  int               (*scrollbarGetKnobSize)     ( ScrollbarID priv );                                              /* Get the current scrollbar position (topmost visible item in a list) */
  void              (*scrollbarSetKnobSize)     ( ScrollbarID priv, int size );                                    /* Set the current scrollbar position                                  */

  int               (*scrollbarGetMin)          ( ScrollbarID priv );                                              /* Get the minimum allowed scrollbar position                          */
  int               (*scrollbarGetMax)          ( ScrollbarID priv );                                              /* Get the maximum allowed scrollbar position                          */
  void              (*scrollbarSetMin)          ( ScrollbarID priv, int min );                                     /* Set the minimum allowed scrollbar position                          */
  void              (*scrollbarSetMax)          ( ScrollbarID priv, int max );                                     /* Set the maximum allowed scrollbar position                          */
  void              (*scrollbarSetMinMax)       ( ScrollbarID priv, int min, int max );                            /* Set the minimum and maximum allowed scrollbar positions             */

  void *            (*scrollbarHandle)          ( ScrollbarID priv, void *prev_focus, int flags, int mx, int my ); /* Handle scrollbar mouse input.  Call this from your panel mouse callback. */

  LWControlID       (*scrollbarGetControl)      ( ScrollbarID priv );                                              /* Get the control for the scrollbar, for aligning and such            */
  void              (*scrollbarDestroy)         ( ScrollbarID priv );                                              /* Destroy the scrollbar                                               */

  /* MultiTree Control */
  MultiTreeID       (*multiTree)                  ( GlobalFunc *global, LWPanelID panel, int w, int h,             /* Create a new multi-list.  hscroll and vscroll are 1 to add those scrollbars and 0 to not.  default_column_widths is a 0-terminated int array. */
                                                    int *default_column_widths, int tree_column, int hscroll, int vscroll );

  void              (*multiTreeSetUserData)       ( MultiTreeID priv, void *user_data );                           /* Set the user data                                                                                                                 */
  void              (*multiTreeQuery)             ( MultiTreeID priv, MultiTree_Query          event );            /* Set the query function.  Will be called to ask if the item is selected, folded, etc.  Can be NULL for read-only, unfoldable lists */
  void              (*multiTreeClickEvent)        ( MultiTreeID priv, MultiTree_ClickEvent     event );            /* Set the click events function.  Column-oriented coordinates and flags will be provided so you can do specific tests (handle right clicks, detect clicks in zones, etc). Can be NULL if you want to handle only item events */
  void              (*multiTreeItemEvent)         ( MultiTreeID priv, MultiTree_ItemEvent      event );            /* Set the item event.  Called when an item is selected, deselected or folded.  Can be NULL if you want to handle only click events  */
  void              (*multiTreeDragDropEvent)     ( MultiTreeID priv, MultiTree_DragDropEvent  event );            /* Set the drag/drop event.  Called when the user drags and drops one item onto anyther.  Can be NULL to disable drag/drop           */
  void              (*multiTreeCustomDraw)        ( MultiTreeID priv, MultiTree_CustomDraw     event );            /* Set the custom draw function.  Called numerous times during list drawing                                                          */
  void              (*multiTreeAutosizeColumn)    ( MultiTreeID priv, MultiTree_AutosizeColumn event );            /* Set the autosize column function.  Should return the minimum width of the column to completely fit it's contents.                 */
  void              (*multiTreeResizeColumn)      ( MultiTreeID priv, MultiTree_ResizeColumn   event );            /* Set the resize column event.  Called as a column is resized                                                                       */
  void              (*multiTreeNextChild)         ( MultiTreeID priv, MultiTree_NextChild      event );            /* Set the next child callback.  Called to build a flattened list of items.  Use for linked list style data structures               */
  void              (*multiTreeNthChild)          ( MultiTreeID priv, MultiTree_NthChild       event );            /* Set the Nth child callback.  Called to build a flattened list of items.  Use for array style data structures                      */

  void              (*multiTreeRefresh)           ( MultiTreeID priv );                                            /* Refresh the item list's contents.  Call this when the item list structure has changed (items added/removed/folded)                */
  void              (*multiTreeRedraw)            ( MultiTreeID priv );                                            /* Redraw the list.  Call this when the item list's names, selection or other item drawing events have changed.                      */

  int               (*multiTreeGetNumItems)       ( MultiTreeID priv );                                            /* Get the number of items in the flattened item list    */

  void *            (*multiTreeGetFirstVisibleItem) ( MultiTreeID priv );                                          /* Get the first visible item in the flattened item list     */
  int               (*multiTreeGetNumVisibleItems)  ( MultiTreeID priv );                                          /* Get the number of visible item in the flattened item list (**/

  void              (*multiTreeScrollToItem)      ( MultiTreeID priv, void *item );                                /* Scroll to a specific visible item.  Returns NULL if the item isn't visible (ie: child of a folded item). */
  void              (*multiTreeScrollToIndex)     ( MultiTreeID priv, int index );                                 /* Scrool to a specific index in the flattened item list.                                                   */

  int               (*multiTreeGetRowHeight)      ( MultiTreeID priv );                                            /* Get height of an individual row */

  int               (*multiTreeGetColumnWidth)    ( MultiTreeID priv, int column );                                /* Get a column's width */
  void              (*multiTreeSetColumnWidth)    ( MultiTreeID priv, int column, int width );                     /* Set a column's width */

  void *            (*multiTreeHandle)            ( MultiTreeID priv, void *prev_focus, int flags, int mx, int my ); /* Handle scrollbar mouse input.  Call this from your panel mouse callback. */

  LWControlID       (*multiTreeGetControl)        ( MultiTreeID priv );                                            /* Get the list control                 */

  void              (*multiTreeDestroy)           ( MultiTreeID priv );                                            /* Destroy the list */

} TMP_PanelControls;

/*
 *   TMP_PanelProgress:
 *    This structure is returned by the TMP_PanelProgress global.  This can be used
 *     to open progress panels and to create progress controls.
 */
typedef void * ProgressControlID;
typedef void * ProgressPanelID;

typedef struct st_TMP_PanelProgress {
  /* ProgressControl  */
  ProgressControlID (*progressControl)            ( GlobalFunc *global, LWPanelID panel, int width );              /* Progress Control                         */

  void              (*progressControlRedraw)        ( ProgressControlID priv );                                    /* Handle input.  Returns 0 if the the user canceled the operation */

  int               (*progressControlGetTotal)    ( ProgressControlID priv );                                      /* Get the total number of steps */
  void              (*progressControlSetTotal)    ( ProgressControlID priv, int total );                           /* Set the total number of steps */

  int               (*progressControlGetCurrent)  ( ProgressControlID priv );                                      /* Get the current step */
  void              (*progressControlSetCurrent)  ( ProgressControlID priv, int current );                         /* Set the current step */

  const char *      (*progressControlGetString)   ( ProgressControlID priv );                                      /* Get the progress string */
  void              (*progressControlSetString)   ( ProgressControlID priv, const char *string );                  /* Set the progress string */

  void              (*progressControlIncrementBar)       ( ProgressControlID priv );                               /* Increment the progress bar                       */
  void              (*progressControlIncrementBarString) ( ProgressControlID priv, const char *_string );          /* Increment the progress bar and change the string */

  void              (*progressControlSetBar)      ( ProgressPanelID priv, int current, const char *_string );      /* Set the progress bar and the string */

  LWControlID       (*progressControlGetControl)  ( ProgressControlID priv );                                      /* Get the underlying control               */

  void              (*progressControlDestroy)     ( ProgressControlID priv );                                      /* Destroy the progress control             */

  /* ProgressPanel    */
  ProgressPanelID   (*progressPanel)              ( GlobalFunc *global );                                          /* Progress Panel                           */

  int               (*progressPanelOpen)          ( ProgressPanelID priv, const char *title );                     /* Open the progress panel                  */
  void              (*progressPanelClose)         ( ProgressPanelID priv);                                         /* Close the progress panel                 */
  int               (*progressPanelIsOpen)        ( ProgressPanelID priv );                                        /* Returns 1 if the panel is open           */

  int               (*progressPanelHandle)        ( ProgressPanelID priv );                                        /* Handle input.  Returns 0 if the the user canceled the operation                             */
  int               (*progressPanelHandleProgress)( ProgressPanelID priv, int progress, const char *string );      /* Handle input and update the progress bar.  Returns 0 if the the user canceled the operation */
  void              (*progressPanelAbortMessage)  ( ProgressPanelID priv, const char *title, const char *string1, const char *string2 ); /* Message to present the user on abort                                  */

  int               (*progressPanelGetTotal)      ( ProgressPanelID priv );                                        /* Get the total number of steps */
  void              (*progressPanelSetTotal)      ( ProgressPanelID priv, int total );                             /* Set the total number of steps */

  int               (*progressPanelGetCurrent)    ( ProgressPanelID priv );                                        /* Get the current step */
  void              (*progressPanelSetCurrent)    ( ProgressPanelID priv, int current );                           /* Set the current step */

  const char *      (*progressPanelGetString)     ( ProgressPanelID priv );                                        /* Get the progress string */
  void              (*progressPanelSetString)     ( ProgressPanelID priv, const char *string );                    /* Set the progress string */

  void              (*progressPanelIncrementBar)       ( ProgressPanelID priv );                                   /* Increment the progress bar                       */
  void              (*progressPanelIncrementBarString) ( ProgressPanelID priv, const char *_string );              /* Increment the progress bar and change the string */

  void              (*progressPanelSetBar)        ( ProgressPanelID priv, int current, const char *_string );      /* Set the progress bar and the string */

  LWPanelID         (*progressPanelGetPanel)      ( ProgressPanelID priv );                                        /* Get the panel                            */

  void              (*progressPanelDestroy)       ( ProgressPanelID priv );                                        /* Destroy the progress panel               */

} TMP_PanelProgress;

/* Common Icon Colors (ie: Item Colors) */
#define TMPICONCOLOR_BLACK        RGB_(   0,   0,   0 )   /* Black       */
#define TMPICONCOLOR_DARK_BLUE    RGB_(   0,  48, 128 )   /* Dark Blue   */
#define TMPICONCOLOR_DARK_GREEN   RGB_(   0,  96,   0 )   /* Dark Green  */
#define TMPICONCOLOR_DARK_CYAN    RGB_(  32,  96, 112 )   /* Dark Cyan   */
#define TMPICONCOLOR_DARK_RED     RGB_( 112,   0,   0 )   /* Dark Red    */
#define TMPICONCOLOR_PURPLE       RGB_( 112,  32, 112 )   /* Purple      */
#define TMPICONCOLOR_BROWN        RGB_( 112,  80,   0 )   /* Brown       */
#define TMPICONCOLOR_GREY         RGB_( 176, 176, 176 )   /* Grey        */
#define TMPICONCOLOR_BLUE         RGB_(  32, 160, 240 )   /* Blue        */
#define TMPICONCOLOR_GREEN        RGB_(  32, 224,  32 )   /* Green       */
#define TMPICONCOLOR_CYAN         RGB_(  96, 224, 240 )   /* Cyan        */
#define TMPICONCOLOR_RED          RGB_( 240,  32,  32 )   /* Red         */
#define TMPICONCOLOR_MAGENTA      RGB_( 240,  96, 240 )   /* Magenta     */
#define TMPICONCOLOR_ORANGE       RGB_( 240, 192,  32 )   /* Orange      */
#define TMPICONCOLOR_WHITE        RGB_( 240, 240, 240 )   /* White       */

/* Common colors in a conveniently-indexable array */
const int TMP_IconColors[] = {
  TMPICONCOLOR_BLACK,
  TMPICONCOLOR_DARK_BLUE,
  TMPICONCOLOR_DARK_GREEN,
  TMPICONCOLOR_DARK_CYAN,
  TMPICONCOLOR_DARK_RED,
  TMPICONCOLOR_PURPLE,
  TMPICONCOLOR_BROWN,
  TMPICONCOLOR_GREY,
  TMPICONCOLOR_BLUE,
  TMPICONCOLOR_GREEN,
  TMPICONCOLOR_CYAN,
  TMPICONCOLOR_RED,
  TMPICONCOLOR_MAGENTA,
  TMPICONCOLOR_WHITE,
};

/* Fold Icon Types */
typedef enum en_TMP_FoldIconTypes {
  TMPICON_FOLD_TRIANGLE,
  TMPICON_FOLD_PLUS_MINUS
} TMP_FoldIconTypes;

/* Icon Types */
typedef enum st_TMP_IconTypes {        /* Colors -   Origin  - Description                                                                  */
  TMPICON_CAMERA,                      /*  P/S   -  Top Left -  Camera Icon, similar to the one in the Scene Editor                         */
  TMPICON_BONE,                        /*  P/S   -  Top Left -  Bone Icon, similar to the one in the Scene Editor                           */
  TMPICON_OBJECT,                      /*  P/S   -  Top Left -  Object Icon, similar to the one in the Scene Editor                         */
  TMPICON_LIGHT,                       /*  P/S   -  Top Left -  Light Icon, similar to the one in the Scene Editor                          */
  TMPICON_CHECK,                       /*  P     -  Top Left -  Checkmark, similar to the one in lists to enabled/disable plug-ins          */
  TMPICON_DOT,                         /*  P     -  Top Left -  An alternate checkmark that looks like a dot                                */
  TMPICON_BOUNDING_BOX,                /*  P     -  Top Left -  Bounding Box Icon, for object visibility                                    */
  TMPICON_VERTICES,                    /*  P     -  Top Left -  Vertices/Points Only Icon, for object visibility                            */
  TMPICON_WIREFRAME,                   /*  P     -  Top Left -  Wireframe Icon, for object visibility                                       */
  TMPICON_FRONTFACE,                   /*  P     -  Top Left -  Frontface Icon, for object visibility                                       */
  TMPICON_SHADED_SOLID,                /*  P     -  Top Left -  Shaded Solid Icon, for object visibility                                    */
  TMPICON_TEXTURED_SHADED,             /*  P/S   -  Top Left -  Textured Shaded Solid Icon, for object visibility                           */
  TMPICON_LOCK,                        /*  P     -  Top Left -  Lock Icon                                                                   */
  TMPICON_VISIBILITY_EYE,              /*  P     -  Top Left -  Visibility Eye Icon                                                         */
  TMPICON_KEYFRAME,                    /*  P     -  Center   -  A plus (+) shape representing a keyframe                                    */
  TMPICON_SLIDER_KNOB_TICK_VERTICAL,   /*  P/S   -  Center   -  Four horizontal ticks, similar to those in a vertical slider's knob         */
  TMPICON_SLIDER_KNOB_TICK_HORIZONTAL  /*  P/S   -  Center   -  Four vertical ticks, similar to those in a vertical slider's knob           */
} TMP_IconTypes;

/* Arrow Types */
typedef enum st_TMP_ArrowTypes {
  TMPARROW_UP,            /* Single upward-pointing arrow                                                */
  TMPARROW_DOWN,          /* Single downward-pointing arrow                                              */
  TMPARROW_LEFT,          /* Single left-pointing arrow                                                  */
  TMPARROW_RIGHT,         /* Single right-pointing arrow                                                 */
  TMPARROW_UP_NARROW,     /* Single narrow upward-pointing arrow                                         */
  TMPARROW_DOWN_NARROW,   /* Single narrow downward-pointing arrow                                       */
  TMPARROW_LEFT_NARROW,   /* Single narrow left-pointing arrow                                           */
  TMPARROW_RIGHT_NARROW,  /* Single narrow right-pointing arrow                                          */
  TMPARROW_DOUBLE_UP,     /* Double upward-pointing arrow                                                */
  TMPARROW_DOUBLE_DOWN,   /* Double downward-pointing arrow                                              */
  TMPARROW_DOUBLE_LEFT,   /* Double left-pointing arrow                                                  */
  TMPARROW_DOUBLE_RIGHT,  /* Double right-pointing arrow                                                 */
  TMPARROW_AWAY_VERT,     /* One arrow pointing up over an arrow pointing down (<> rotated 90 degrees)   */
  TMPARROW_AWAY_HORIZ,    /* One arrow pointing left on the left of an arrow pointing right (<>)         */
  TMPARROW_TOWARD_HORIZ,  /* One arrow pointing up under an arrow pointing down (>< rotated 90 degrees)  */
  TMPARROW_TOWARD_VERT,   /* One arrow pointing left on the right of an arrow pointing right (><)        */
  TMPARROW_4WAY,          /* 4 arrows pointing away from each other                                      */
  TMPARROW_POPUP          /* A downward-pointing arrow like those used by pop-up controls.  Always solid */
} TMP_ArrowTypes;

/* Arrow Draw Methods */
typedef enum en_TMP_ArrowMethods {
  TMPARROW_SOLID,         /* Filled shape                                           */
  TMPARROW_EMPTY,         /* Empty shape                                            */
  TMPARROW_TWO_EDGES      /* Two lines representing an arrow shape (like this: > )  */
} TMP_ArrowMethods;

/* Checkerboard Checkmasks */
typedef enum en_TMP_CheckMask {
  TMPCHECKMASK_EVEN = 0x01,                                 /* Do just the even checks          */
  TMPCHECKMASK_ODD  = 0x02,                                 /* Do just the odd checks           */
  TMPCHECKMASK_BOTH = TMPCHECKMASK_EVEN | TMPCHECKMASK_ODD  /* Do both the even and odd checks  */
} TMP_CheckMask;

/*
 *   TMP_PanelDraw:
 *    This structure is returned by the TMP_PanelDraw global.  This can be used
 *     to draw directly into LWPanels.
 */
typedef struct TMP_PanelDraw {
  int (*checkerboard)      ( LWPanelID panel, DrawFuncs *df, int x, int y,
                             int width, int height, int check_w, int check_h,
                             int odd_color, int even_color, TMP_CheckMask check_mask );

  int (*horizontalDivider) ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int thickness );

  int (*verticalDivider)   ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int height, int thickness );

  int (*emptyBox)          ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color );

  int (*filledBox)         ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int (*buttonBox)         ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int (*button)            ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int brightest_r, int brightest_g, int brightest_b, int invert );

  int (*icon)              ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, TMP_IconTypes type,
                             int prime_color, int second_color );

  int (*iconWidth)         ( TMP_IconTypes type );

  int (*iconHeight)        ( TMP_IconTypes type );

  int (*foldIcon)          ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type );

  int (*arrow)             ( LWPanelID panel, DrawFuncs *df,
                             int x, int y,
                             TMP_ArrowTypes type, TMP_ArrowMethods method,
                             int color );

  int (*horizontalGradient) ( LWPanelID panel, DrawFuncs *df,
                              int x, int y, int width, int height,
                              int left_r, int left_g, int left_b,
                              int right_r, int right_g, int right_b );

  int (*verticalGradient)  ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int top_r, int top_g, int top_b,
                             int bottom_r, int bottom_g, int bottom_b );

  int (*panelResizeHandle) ( LWPanelFuncs *panel_funcs, LWPanelID panel, int auto_hilight );
} TMP_PanelDraw;


/*
 *   TMP_RasterDraw:
 *    This structure is returned by the TMP_RasterDraw global.  This can be used
 *     to draw directly into a raster image.

 */
typedef struct st_TMP_RasterDraw {
  int (*checkerboard)      ( LWRasterID raster, LWRasterFuncs *df, int x, int y,
                             int width, int height, int check_w, int check_h,
                             int odd_color, int even_color, TMP_CheckMask check_mask );

  int (*horizontalDivider) ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int thickness );

  int (*verticalDivider)   ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int thickness );

  int (*emptyBox)          ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color );

  int (*filledBox)         ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int (*buttonBox)         ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int (*button)            ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int brightest_r, int brightest_g, int brightest_b, int invert );

  int (*icon)              ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, TMP_IconTypes type,
                             int prime_color, int second_color );

  int (*iconWidth)         ( TMP_IconTypes type );

  int (*iconHeight)        ( TMP_IconTypes type );

  int (*foldIcon)          ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type );

  int (*arrow)             ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y,
                             TMP_ArrowTypes type, TMP_ArrowMethods method,
                             int color );

  int (*horizontalGradient) ( LWRasterID raster, LWRasterFuncs *df,
                              int x, int y, int width, int height,
                              int left_r, int left_g, int left_b,
                              int right_r, int right_g, int right_b );

  int (*verticalGradient)  ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int top_r, int top_g, int top_b,
                             int bottom_r, int bottom_g, int bottom_b );
} TMP_RasterDraw;

#endif

