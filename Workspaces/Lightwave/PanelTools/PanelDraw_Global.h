//
// PanelDraw_Global.h
//

extern TMP_PanelDraw panel_draw_functions;

extern "C" {
  int pd_checkerboard      ( LWPanelID panel, DrawFuncs *df, int x, int y,
                             int width, int height, int check_w, int check_h,
                             int odd_color, int even_color, TMP_CheckMask check_mask );

  int pd_horizontalDivider ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int thickness );

  int pd_verticalDivider   ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int thickness );

  int pd_emptyBox          ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color );

  int pd_filledBox         ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int pd_buttonBox         ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int pd_button            ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height,
                             int brightest_r, int brightest_g, int brightest_b, int invert );

  int pd_icon              ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, TMP_IconTypes type,
                             int prime_color, int second_color );

  int pd_iconWidth         ( TMP_IconTypes type );

  int pd_iconHeight        ( TMP_IconTypes type );

  int pd_foldIcon          ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type );

  int pd_arrow             ( LWPanelID panel, DrawFuncs *df,
                             int x, int y,
                             TMP_ArrowTypes type, TMP_ArrowMethods method,
                             int color );

  int pd_horizontalGradient ( LWPanelID panel, DrawFuncs *df,
                              int x, int y, int w, int h,
                              int left_r, int left_g, int left_b,
                              int right_r, int right_g, int right_b );

  int pd_verticalGradient  ( LWPanelID panel, DrawFuncs *df,
                             int x, int y, int w, int h,
                             int top_r, int top_g, int top_b,
                             int bottom_r, int bottom_g, int bottom_b );

  int pd_panelResizeHandle ( LWPanelFuncs *panel_funcs, LWPanelID panel, int auto_hilight );
}
