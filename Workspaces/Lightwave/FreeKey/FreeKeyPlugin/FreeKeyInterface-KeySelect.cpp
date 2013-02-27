//
// FreeKeyInterface-KeySelect.cpp
//
// Contains all the functions used to draw the keyframe selection tab
//  and handles all interface event within that tab, plus various
//  keyframe selection routines.
//

#include "FreeKey-Interface.h"

// DrawKeySelectInfo():
//  Displays some information about keyframe selections
void DrawKeySelectInfo( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int cw = CON_HOTW( ctl );
  int ch = CON_HOTH( ctl );

  // Erase the control
  df->DrawRGBBox( fk_int->main_panel, 128, 128, 128, cx, cy, cw, ch );

  char buffer[128];

  // Write the number of keys selected
  if( !fk->IsCurrentItemValid() )
    sprintf( buffer, "%d", 0 );
  else
    sprintf( buffer, "%d", fk_int->sorted_items[ fk->GetCurrentItem() ]->GetMotion().GetChannel(0).GetSelected().NumElements() );

  df->WriteText( fk_int->main_panel, "Selected:", COLOR_BLACK, cx+10, cy+2 );
  df->WriteText( fk_int->main_panel, buffer,      COLOR_BLACK, cx+70, cy+2 );

  // Write the total number of keys
  if( !fk->IsCurrentItemValid() )
    sprintf( buffer, "%d", 0 );
  else
    sprintf( buffer, "%d", fk_int->sorted_items[ fk->GetCurrentItem() ]->GetMotion().GetChannel(0).GetKeys().NumElements() );

  df->WriteText( fk_int->main_panel, "Total:",    COLOR_BLACK, cx+10, cy + 19 );
  df->WriteText( fk_int->main_panel, buffer,      COLOR_BLACK, cx+70, cy + 19 );
}

