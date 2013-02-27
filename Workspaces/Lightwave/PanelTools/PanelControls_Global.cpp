//
// PanelControls_Global.cpp
//

#include "PanelToolsGlobal.h"
#include "PanelControls_Global.h"

TMP_PanelControls panel_control_functions = {
  pc_wPopdown, pc_wPopdownCustom, pc_wPopdownSetEvent, pc_wPopdownOpen, pc_wPopdownGetControl, pc_wPopdownDestroy,

  pc_hScrollbar, pc_vScrollbar, pc_scrollbarSetEvent, pc_scrollbarGetKnobPosition, pc_scrollbarSetKnobPosition,
  pc_scrollbarGetKnobSize, pc_scrollbarSetKnobSize, pc_scrollbarGetMin, pc_scrollbarGetMax, pc_scrollbarSetMin,
  pc_scrollbarSetMax, pc_scrollbarSetMinMax, pc_scrollbarHandle, pc_scrollbarGetControl, pc_scrollbarDestroy,

  pc_multiTree, pc_multiTreeSetUserData, pc_multiTree_Query, pc_multiTree_ClickEvent, pc_multiTree_ItemEvent,
  pc_multiTree_DragDropEvent, pc_multiTree_CustomDraw, pc_multiTree_AutosizeColumn, pc_multiTree_ResizeColumn,
  pc_multiTree_NextChild, pc_multiTree_NthChild, pc_multiTreeRefresh, pc_multiTreeRedraw, pc_multiTreeGetNumItems,
  pc_multiTreeGetFirstVisibleItem, pc_multiTreeGetNumVisibleItems,
  pc_multiTreeScrollToItem, pc_multiTreeScrollToIndex, pc_multiTreeGetRowHeight, pc_multiTreeGetColumnWidth,
  pc_multiTreeSetColumnWidth, pc_multiTreeHandle, pc_multiTreeGetControl, pc_multiTreeDestroy };


//
// -- WPopdown
//

// pc_wPopdown():
WPopdownID pc_wPopdown( GlobalFunc *global, LWPanelID panel, int width,
                          const char *title, const char **strings ) {
  return new PanelControls_WPopdown(global, panel, width, title, strings );
}

// pc_wPopdownCustom():
WPopdownID pc_wPopdownCustom( GlobalFunc *global, LWPanelID panel, int width,
                                const char *title, WPopdown_Name *name, WPopdown_Count *count ) {
  return new PanelControls_WPopdown(global, panel, width, title, name, count );
}

// pc_wPopdownSetEvent():
void pc_wPopdownSetEvent( WPopdownID priv, WPopdown_Event *event, void *user_data ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)priv;
  popdown->SetEvent( event, user_data );
}

// pc_wPopdownOpen();
void pc_wPopdownOpen( WPopdownID priv, int force ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)priv;
  popdown->OpenMenu( (force != 0) );
}

// pc_wPopdownGetControl():
LWControlID pc_wPopdownGetControl( WPopdownID priv ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)priv;
  return popdown->GetControl();
}

// pc_wPopdownDestroy():
void pc_wPopdownDestroy( WPopdownID priv ) {
  PanelControls_WPopdown *popdown = (PanelControls_WPopdown *)priv;
  delete popdown;
}

//
// -- Scrollbar
//

// pc_hScrollbar():
ScrollbarID pc_hScrollbar( GlobalFunc *global, LWPanelID panel, int length ) {
  return new PanelControls_Scrollbar( global, panel, length, true );
}

// pc_vScrollbar():
ScrollbarID pc_vScrollbar( GlobalFunc *global, LWPanelID panel, int length ) {
  return new PanelControls_Scrollbar( global, panel, length, false );
}

// pc_scrollbarSetEvent():
void pc_scrollbarSetEvent( ScrollbarID priv, Scrollbar_Event *event, void *user_data ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetEvent( event, user_data );
}

// pc_scrollbarGetKnobPosition():
int pc_scrollbarGetKnobPosition( ScrollbarID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->GetKnobPosition();
}

// pc_scrollbarSetKnobPosition():
void pc_scrollbarSetKnobPosition( ScrollbarID priv, int position ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetKnobPosition( position );
}

// pc_scrollbarGetKnobSize():
int pc_scrollbarGetKnobSize( ScrollbarID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->GetKnobSize();
}

// pc_scrollbarSetKnobSize():
void pc_scrollbarSetKnobSize( ScrollbarID priv, int size ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetKnobSize( size );
}

// pc_scrollbarGetMin():
int pc_scrollbarGetMin( ScrollbarID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->GetMin();
}

// pc_scrollbarGetMax():
int pc_scrollbarGetMax( ScrollbarID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->GetMax();
}

// pc_scrollbarSetMin():
void pc_scrollbarSetMin( ScrollbarID priv, int min ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetMin( min );
}

// pc_scrollbarSetMax():
void pc_scrollbarSetMax( ScrollbarID priv, int max ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetMax( max );
}

// pc_scrollbarSetMinMax():
void pc_scrollbarSetMinMax( ScrollbarID priv, int min, int max ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  scrollbar->SetMinMax( min, max );
}

// pc_scrollbarHandle():
void * pc_scrollbarHandle( ScrollbarID priv, void *prev_focus, int flags, int mx, int my ) {
  if( priv == NULL )
    return NULL;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->Handle( prev_focus, flags, mx, my );
}

// pc_scrollbarGetControl():
LWControlID pc_scrollbarGetControl( ScrollbarID priv ) {
  if( priv == NULL )
    return NULL;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  return scrollbar->GetControl();
}

// pc_scrollbarDestroy():
void pc_scrollbarDestroy( ScrollbarID priv ) {
  if( priv == NULL )
    return;

  PanelControls_Scrollbar *scrollbar = (PanelControls_Scrollbar *)priv;
  delete scrollbar;
}

//
// -- MultiTree
//

// pc_multiTreeSetUserData():
MultiTreeID pc_multiTree ( GlobalFunc *global, LWPanelID panel, int w, int h,
                           int *default_column_widths, int tree_column, int hscroll, int vscroll ) {
  return new PanelControls_MultiTree( global, panel, w, h, default_column_widths, tree_column, (hscroll != 0), (vscroll != 0) );
}

// pc_multiTreeSetUserData():
void pc_multiTreeSetUserData( MultiTreeID priv, void *user_data ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetUserData( user_data );
}

// pc_multiTree_Query():
void pc_multiTree_Query( MultiTreeID priv, MultiTree_Query event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetQuery( event );
}

// pc_multiTree_ClickEvent():
void pc_multiTree_ClickEvent( MultiTreeID priv, MultiTree_ClickEvent event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetClickEvent( event );
}

// pc_multiTree_ItemEvent():
void pc_multiTree_ItemEvent( MultiTreeID priv, MultiTree_ItemEvent event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetItemEvent( event );
}

// pc_multiTree_DragDropEvent():
void pc_multiTree_DragDropEvent( MultiTreeID priv, MultiTree_DragDropEvent event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetDragDropEvent( event );
}

// pc_multiTree_CustomDraw():
void pc_multiTree_CustomDraw( MultiTreeID priv, MultiTree_CustomDraw event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetCustomDraw( event );
}

// pc_multiTree_AutosizeColumn():
void pc_multiTree_AutosizeColumn( MultiTreeID priv, MultiTree_AutosizeColumn event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetAutosizeColumn( event );
}

// pc_multiTree_ResizeColumn():
void pc_multiTree_ResizeColumn( MultiTreeID priv, MultiTree_ResizeColumn event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetResizeColumn( event );
}

// pc_multiTree_NextChild():
void pc_multiTree_NextChild( MultiTreeID priv, MultiTree_NextChild event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetNextChild( event );
}

// pc_multiTree_NthChild():
void pc_multiTree_NthChild( MultiTreeID priv, MultiTree_NthChild event ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetNthChild( event );
}

// pc_multiTreeRefresh():
void pc_multiTreeRefresh( MultiTreeID priv ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->Refresh();
}

// pc_multiTreeRedraw():
void pc_multiTreeRedraw( MultiTreeID priv ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->Redraw();
}

// pc_multiTreeGetNumItems():
int pc_multiTreeGetNumItems( MultiTreeID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetNumItems();
}

// pc_multiTreeGetFirstVisibleItem():
void * pc_multiTreeGetFirstVisibleItem( MultiTreeID priv ) {
  if( priv == NULL )
    return NULL;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetFirstVisibleItem();
}

// pc_multiTreeGetNumVisibleItems():
int pc_multiTreeGetNumVisibleItems( MultiTreeID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetNumVisibleItems();
}

// pc_multiTreeScrollToItem():
void pc_multiTreeScrollToItem( MultiTreeID priv, void *item ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->ScrollToItem( item );
}

// pc_multiTreeScrollToIndex():
void pc_multiTreeScrollToIndex( MultiTreeID priv, int index ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->ScrollToIndex( index );
}

// pc_multiTreeGetRowHeight():
int pc_multiTreeGetRowHeight( MultiTreeID priv ) {
  if( priv == NULL )
    return 0;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetRowHeight();
}

// pc_multiTreeGetColumnWidth():
int pc_multiTreeGetColumnWidth( MultiTreeID priv, int column ) {
  if( priv == NULL )
    return 0;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetColumnWidth( column );
}

// pc_multiTreeSetColumnWidth():
void pc_multiTreeSetColumnWidth( MultiTreeID priv, int column, int width ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  multitree->SetColumnWidth( column, width );
}

// pc_multiTreeHandle():
void * pc_multiTreeHandle( MultiTreeID priv, void *prev_focus, int flags, int mx, int my ) {
  if( priv == NULL )
    return NULL;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->Handle( prev_focus, flags, mx, my );
}

// pc_multiTreeGetControl():
LWControlID pc_multiTreeGetControl( MultiTreeID priv ) {
  if( priv == NULL )
    return NULL;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  return multitree->GetListControl();
}

// pc_multiTreeDestroy():
void pc_multiTreeDestroy( MultiTreeID priv ) {
  if( priv == NULL )
    return;

  PanelControls_MultiTree *multitree = (PanelControls_MultiTree *)priv;
  delete multitree;
}
