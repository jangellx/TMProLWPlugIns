//
// RasterDraw_Global.cpp
//

#define NULL 0

#include "PanelToolsGlobal.h"
#include "RasterDraw_Global.h"
#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

TMP_RasterDraw raster_draw_functions = {
  rd_checkerboard, rd_horizontalDivider, rd_verticalDivider, rd_emptyBox, rd_filledBox,
  rd_buttonBox, rd_button, rd_icon, rd_iconWidth, rd_iconHeight, rd_foldIcon, rd_arrow,
  rd_horizontalGradient, rd_verticalGradient
};

// checkerboard():
int rd_checkerboard( LWRasterID raster, LWRasterFuncs *df, int x, int y,
                     int width, int height, int check_w, int check_h,
                     int odd_color, int even_color, TMP_CheckMask check_mask ) {

  return (RasterDraw::Checkerboard( raster, df, x, y, width, height, check_w, check_h, odd_color, even_color, check_mask ) ? 1 : 0);
}

// horizontalDivider():
int rd_horizontalDivider( LWRasterID raster, LWRasterFuncs *df,
                          int x, int y, int width, int thickness ) {
  return (RasterDraw::HorizontalDivider( raster, df, x, y, width, thickness ) ? 1 : 0);
}

// verticalDivider():
int rd_verticalDivider( LWRasterID raster, LWRasterFuncs *df,
                        int x, int y, int width, int thickness ) {
  return (RasterDraw::VerticalDivider( raster, df, x, y, width, thickness ) ? 1 : 0);
}

// emptyBox():
int rd_emptyBox( LWRasterID raster, LWRasterFuncs *df,
                 int x, int y, int width, int height,
                 int x_thickness, int y_thickness,
                 int shine_color, int shadow_color ) {
  return (RasterDraw::EmptyBox( raster, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color ) ? 1 : 0);
}

// filledBox():
int rd_filledBox( LWRasterID raster, LWRasterFuncs *df,
                  int x, int y, int width, int height,
                  int x_thickness, int y_thickness,
                  int shine_color, int shadow_color, int fill_color ) {
  return (RasterDraw::FilledBox( raster, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color, fill_color ) ? 1 : 0);
}

// buttonBox():
int rd_buttonBox( LWRasterID raster, LWRasterFuncs *df,
                  int x, int y, int width, int height,
                  int x_thickness, int y_thickness,
                  int shine_color, int shadow_color, int fill_color ) {
  return (RasterDraw::ButtonBox( raster, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color, fill_color ) ? 1 : 0);
}

// button():
int rd_button( LWRasterID raster, LWRasterFuncs *df,
               int x, int y, int width, int height,
               int brightest_r, int brightest_g, int brightest_b, int invert ) {
  return (RasterDraw::Button( raster, df, x, y, width, height, brightest_r, brightest_g, brightest_b, ((invert == 0) ? 0 : 1) ) ? 1 : 0);
}

// icon():
int rd_icon( LWRasterID raster, LWRasterFuncs *df,
             int x, int y, TMP_IconTypes type,
             int prime_color, int second_color ) {
  return (RasterDraw::DrawLWIcon( raster, df, x, y, (RasterDraw::IconType)type,
                                 prime_color, second_color, RasterDraw::draw_L6 ) ? 1 : 0);
}

// iconWidth():
int rd_iconWidth( TMP_IconTypes type ) {
  return RasterDraw::IconWidth( (RasterDraw::IconType)type );
}

// iconHeight():
int rd_iconHeight( TMP_IconTypes type ) {
  return RasterDraw::IconHeight( (RasterDraw::IconType)type );
}

// foldIcon():
int rd_foldIcon( LWRasterID raster, LWRasterFuncs *df,
                 int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type ) {
  return (RasterDraw::DrawFoldIcon( raster, df, x, y, width, height, color, (folded != 0), (RasterDraw::FoldedType)type ) ? 1 : 0);
}

// arrow():
int rd_arrow( LWRasterID raster, LWRasterFuncs *df,
              int x, int y, TMP_ArrowTypes type, TMP_ArrowMethods method, int color ) {
  return (RasterDraw::DrawArrow( raster, df, x, y, (RasterDraw::ArrowType)type, (RasterDraw::ArrowMethod)method, color ) ? 1 : 0);
}

// horizontalGradient():
int rd_horizontalGradient( LWRasterID raster, LWRasterFuncs *df,
                           int x, int y, int width, int height,
                           int left_r, int left_g, int left_b,
                           int right_r, int right_g, int right_b ) {
  return (RasterDraw::DrawHorizontalGradient( raster, df, x, y, width, height, left_r, left_g, left_b, right_r, right_g, right_b ) ? 1 : 0);
}

// verticalGradient():
int rd_verticalGradient( LWRasterID raster, LWRasterFuncs *df,
                         int x, int y, int width, int height,
                         int top_r, int top_g, int top_b,
                         int bottom_r, int bottom_g, int bottom_b ) {
  return (RasterDraw::DrawVerticalGradient( raster, df, x, y, width, height, top_r, top_g, top_b, bottom_r, bottom_g, bottom_b ) ? 1 : 0);
}

