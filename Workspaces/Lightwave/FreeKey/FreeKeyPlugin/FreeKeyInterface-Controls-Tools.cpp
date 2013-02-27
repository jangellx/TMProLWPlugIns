//
// FreeKeyInterface-Controls-Tools.cpp
//
// This file contains the code to create the tool panel controls
//  on the FreeKey Interface, including all the sub-panels
//

#include "FreeKey-Interface.h"

// Misc UI Strings
const char *tool_class_text[] = {
  "Temporal Shift",      // Shift Keys
  "Temporal Scale",      // Scale Keys
  "Spatial Shift",       // Shift Values
  "Spatial Scale",       // Scale Values
  "Spline Tools",        // Spline Options
  "Key Management",      // Delete Step, Create Keys, Freeze Motion, etc.
  "Item Properties",     // Shadow Options, Color, Visibility, etc.
  NULL };

// MakeToolControls():
bool FreeKeyInterface::MakeToolControls() {
  try {
    int x, y;

    if( !( tool_class_popup = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "", tool_class_text, 120 ) ) )
      throw false;

    PanelTools::PutUnder( source_scene, tool_class_popup );
    x = CON_X( tool_class_popup );
    y = CON_Y( tool_class_popup );
    MOVE_CON( tool_class_popup, x-3, y+10 );
    PanelTools::PutUnder( tool_class_popup, tool_class_popup );
    PanelTools::PutUnder( tool_class_popup, tool_class_popup );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// DrawToolsPane():
//  Redraws the tools pane.  If do_controls is true,
//   then the buttons will also be redrawn (this is
//   specifically for when drawing the item list)
void DrawToolsPane( bool do_controls ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  if( do_controls ) {
    ; // Redraw the controls here
  }

  // Tools Box
  int y  = CON_HOTY( fk_int->tool_class_popup );
  int h  = CON_HOTH( fk_int->tool_class_popup );
  int x  = CON_HOTX( fk_int->tool_class_popup );
  int w  = CON_HOTW( fk_int->tool_class_popup );
  int h2 = CON_HOTH( fk_int->item_list );
  int y2 = CON_HOTY( fk_int->item_list );

  int height = y2+h2 - y;

  PanelDraw::EmptyBox( fk_int->main_panel, df, x-2, y-2, w+4, height+2, 1, 1,
                       100, 100, 100,   172, 172, 172 );

  PanelDraw::EmptyBox( fk_int->main_panel, df, x-3, y-3, w+6, height+4, 1, 1,
                       50, 50, 50,   50, 50, 50 );

  // "Tools" Text
  df->WriteText( fk_int->main_panel, "Tools", COLOR_BLACK, (x+w)/2 - (df->TextWidth( fk_int->main_panel, "Tools" )/2 ), y-h );
}
