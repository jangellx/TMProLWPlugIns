//
// FreeKeyInteface-Lister-OLD.cpp
//
// Some old routines for the lister.  Mainly multiscroll stuff
//

// DrawMSZoom():
//  Draws the lower scroll bar in the MultiScroll
void DrawMSZoom() {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( fk_int->item_multiscroll );
  int cy = CON_HOTY( fk_int->item_multiscroll );
  int ch = CON_HOTH( fk_int->item_multiscroll );

  // Zoom Out
  PanelDraw::Button( fk_int->main_panel, df, cx+fk_int->ms_divider-25, cy, 20, ch, 1, 1,
                     LW_BUTTON_COLOR_PURPLE );
  int bx = cx+fk_int->ms_divider-25;
  PanelDraw::DrawArrow( fk_int->main_panel, df, bx+10, cy+(ch/2),
                        PanelDraw::arrow_toward_vert, PanelDraw::arrow_two_edges );

  // Zoom Slider
  PanelDraw::Button( fk_int->main_panel, df, cx+fk_int->ms_divider-105, cy, 80, ch, 1, 1,
                     LW_BUTTON_COLOR_GREEN );
  bx = cx+fk_int->ms_divider-105;

  df->DrawRGBBox( fk_int->main_panel,  90, 105,  90, bx+8, cy+3, 64, 1 );
  df->DrawRGBBox( fk_int->main_panel,  30,  60,  30, bx+8, cy+4, 64, 1 );
  df->DrawRGBBox( fk_int->main_panel, 222, 235, 222, bx+8, cy+5, 64, 1 );

  PanelDraw::DrawArrow( fk_int->main_panel, df, bx+70 - (int)( fk_int->ms_zoom * 60.0 ),
                        cy+(ch/2)+1, PanelDraw::arrow_up, PanelDraw::arrow_empty );

  // Zoom In
  PanelDraw::Button( fk_int->main_panel, df, cx+fk_int->ms_divider-125, cy, 20, ch, 1, 1,
                     LW_BUTTON_COLOR_PURPLE );
  bx = cx+fk_int->ms_divider-125;
  PanelDraw::DrawArrow( fk_int->main_panel, df, bx+10, cy+(ch/2),
                        PanelDraw::arrow_away_vert, PanelDraw::arrow_two_edges );

  // Zoom Factor Text
  char zoom_text[20];
  sprintf( zoom_text, "%g%%", fk_int->ms_zoom * 100.0 );
  df->WriteText( fk_int->main_panel, zoom_text, COLOR_LT_YELLOW, cx+3, cy );
}

// OnItemMultiScroll():
//  Called to handle the custom scroll bar under the lister
void OnItemMultiScroll( LWControl *ctl, int mx, fk_mousestate mouse_state ) {
  static last_clicked = CLICKED_NONE;
  static down_pos     = 0;
  static item_pos     = 0;

  // Mouse Released; Reset
  if( mouse_state == MSTATE_UP ) {
    SET_STR( fk_int->select_item_name, "MS:  Mouse Released", strlen( "MS:  Mouse Released" ) );
    last_clicked = CLICKED_NONE;
    REDRAW_CON( fk_int->item_multiscroll );
    return;
  }

  // Mouse was just clicked; see what was clicked
  if( mouse_state == MSTATE_DOWN ) {
    down_pos = mx;
    mx -= CON_HOTX( ctl );

    // Test the Divider
    int div_near = mx > fk_int->ms_divider ? mx-fk_int->ms_divider: fk_int->ms_divider-mx;
    if( div_near < 6 ) {
      last_clicked = CLICKED_DIVIDER;
      item_pos = fk_int->ms_divider;

      SET_STR( fk_int->select_item_name, "MS:  Clicked Divider", strlen( "MS:  Clicked Divider" ) );
    }

    return;
  }

  // Mouse Moved:  Change some value
  if( mouse_state == MSTATE_MOVING ) {
    if( last_clicked == CLICKED_DIVIDER )
      fk_int->ms_divider = mx;
//      fk_int->ms_divider = down_pos - item_pos + mx - item_pos;

    char buffer[ 64 ];
    sprintf( buffer, "MS:  Moved Divider to %d", fk_int->ms_divider );
    SET_STR( fk_int->select_item_name, buffer, strlen( buffer ) );

    REDRAW_CON( ctl );
  }
}


// From OnItemMultiScroll2():
/*
    // Test Zoom In
    } else if( ( mx > (cx+fk_int->ms_divider - 25) ) &&
               ( mx < (cx+fk_int->ms_divider - 5) ) ) {
      last_clicked = CLICKED_ZOOM_IN;

      fk_int->ms_zoom /= 2.0;
      if( fk_int->ms_zoom < .01 )
        fk_int->ms_zoom = .01;

      sprintf( buffer, "Zin:  Factor:  %g", fk_int->ms_zoom );
      REDRAW_CON( ctl );
      REDRAW_CON( fk_int->item_list );

    // Test Zoom Out
    } else if( ( mx > (cx+fk_int->ms_divider - 125) ) &&
               ( mx < (cx+fk_int->ms_divider - 105) ) ) {
      last_clicked = CLICKED_ZOOM_OUT;

      fk_int->ms_zoom *= 2.0;
      if( fk_int->ms_zoom > 1.0 )
        fk_int->ms_zoom = 1.0;

      sprintf( buffer, "Zout:  Factor:  %g", fk_int->ms_zoom );
      REDRAW_CON( ctl );
      REDRAW_CON( fk_int->item_list );

    // No Item Clicked
*/

// Originally from OnMouse():
  int cx = CON_HOTX( fk_int->item_multiscroll );
  int cy = CON_HOTY( fk_int->item_multiscroll );
  int cw = CON_HOTW( fk_int->item_multiscroll );
  int ch = CON_HOTH( fk_int->item_multiscroll );

  // Test for Item MutliScroll
  if( (mx > cx) && (mx < (cx+cw)) && (my > cy) && (my < (cy+ch) ) )
    OnItemMultiScroll( fk_int->item_multiscroll, mx, state );
