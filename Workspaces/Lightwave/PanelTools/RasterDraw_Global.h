//
// RasterDraw_Global.h
//

extern TMP_RasterDraw raster_draw_functions;

extern "C" {
  int rd_checkerboard      ( LWRasterID raster, LWRasterFuncs *df, int x, int y,
                             int width, int height, int check_w, int check_h,
                             int odd_color, int even_color, TMP_CheckMask check_mask );

  int rd_horizontalDivider ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int thickness );

  int rd_verticalDivider   ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int thickness );

  int rd_emptyBox          ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color );

  int rd_filledBox         ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int rd_buttonBox         ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int x_thickness, int y_thickness,
                             int shine_color, int shadow_color, int fill_color );

  int rd_button            ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int brightest_r, int brightest_g, int brightest_b, int invert );

  int rd_icon              ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, TMP_IconTypes type,
                             int prime_color, int second_color );

  int rd_iconWidth         ( TMP_IconTypes type );

  int rd_iconHeight        ( TMP_IconTypes type );

  int rd_foldIcon          ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type );

  int rd_arrow             ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y,
                             TMP_ArrowTypes type, TMP_ArrowMethods method,
                             int color );

  int rd_horizontalGradient ( LWRasterID raster, LWRasterFuncs *df,
                              int x, int y, int width, int height,
                              int left_r, int left_g, int left_b,
                              int right_r, int right_g, int right_b );

  int rd_verticalGradient  ( LWRasterID raster, LWRasterFuncs *df,
                             int x, int y, int width, int height,
                             int top_r, int top_g, int top_b,
                             int bottom_r, int bottom_g, int bottom_b );
}

