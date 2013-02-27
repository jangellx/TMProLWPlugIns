//
// PanelTools-Draw.cpp
//

#define NULL 0

#include "PanelToolsGlobal.h"
#include "PanelDraw_Global.h"
#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

TMP_PanelDraw panel_draw_functions = {
  pd_checkerboard, pd_horizontalDivider, pd_verticalDivider, pd_emptyBox, pd_filledBox,
  pd_buttonBox, pd_button, pd_icon, pd_iconWidth, pd_iconHeight, pd_foldIcon, pd_arrow,
  pd_horizontalGradient, pd_verticalGradient, pd_panelResizeHandle
};

// checkerboard():
int pd_checkerboard( LWPanelID panel, DrawFuncs *df, int x, int y,
                     int width, int height, int check_w, int check_h,
                     int odd_color, int even_color, TMP_CheckMask check_mask ) {

  return (PanelDraw::Checkerboard( panel, df, x, y, width, height, check_w, check_h, odd_color, even_color, check_mask ) ? 1 : 0);
}

// horizontalDivider():
int pd_horizontalDivider( LWPanelID panel, DrawFuncs *df,
                          int x, int y, int width, int thickness ) {
  return (PanelDraw::HorizontalDivider( panel, df, x, y, width, thickness ) ? 1 : 0);
}

// verticalDivider():
int pd_verticalDivider( LWPanelID panel, DrawFuncs *df,
                        int x, int y, int width, int thickness ) {
  return (PanelDraw::VerticalDivider( panel, df, x, y, width, thickness ) ? 1 : 0);
}

// emptyBox():
int pd_emptyBox( LWPanelID panel, DrawFuncs *df,
                 int x, int y, int width, int height,
                 int x_thickness, int y_thickness,
                 int shine_color, int shadow_color ) {
  return (PanelDraw::EmptyBox( panel, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color ) ? 1 : 0);
}

// filledBox():
int pd_filledBox( LWPanelID panel, DrawFuncs *df,
                  int x, int y, int width, int height,
                  int x_thickness, int y_thickness,
                  int shine_color, int shadow_color, int fill_color ) {
  return (PanelDraw::FilledBox( panel, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color, fill_color ) ? 1 : 0);
}

// buttonBox():
int pd_buttonBox( LWPanelID panel, DrawFuncs *df,
                  int x, int y, int width, int height,
                  int x_thickness, int y_thickness,
                  int shine_color, int shadow_color, int fill_color ) {
  return (PanelDraw::ButtonBox( panel, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color, fill_color ) ? 1 : 0);
}

// button():
int pd_button( LWPanelID panel, DrawFuncs *df,
               int x, int y, int width, int height,
               int brightest_r, int brightest_g, int brightest_b, int invert ) {
  return (PanelDraw::Button( panel, df, x, y, width, height, brightest_r, brightest_g, brightest_b, ((invert == 0) ? 0 : 1) ) ? 1 : 0);
}

// icon():
int pd_icon( LWPanelID panel, DrawFuncs *df,
             int x, int y, TMP_IconTypes type,
             int prime_color, int second_color ) {
  return (PanelDraw::DrawLWIcon( panel, df, x, y, (PanelDraw::IconType)type,
                                 prime_color, second_color, PanelDraw::draw_L6 ) ? 1 : 0);
}

// iconWidth():
int pd_iconWidth( TMP_IconTypes type ) {
  return PanelDraw::IconWidth( (PanelDraw::IconType)type );
}

// iconHeight():
int pd_iconHeight( TMP_IconTypes type ) {
  return PanelDraw::IconHeight( (PanelDraw::IconType)type );
}

// foldIcon():
int pd_foldIcon( LWPanelID panel, DrawFuncs *df,
                 int x, int y, int width, int height, int color, int folded, TMP_FoldIconTypes type ) {
  return (PanelDraw::DrawFoldIcon( panel, df, x, y, width, height, color, (folded != 0), (PanelDraw::FoldedType)type ) ? 1 : 0);
}

// arrow():
int pd_arrow( LWPanelID panel, DrawFuncs *df,
              int x, int y, TMP_ArrowTypes type, TMP_ArrowMethods method, int color ) {
  return (PanelDraw::DrawArrow( panel, df, x, y, (PanelDraw::ArrowType)type, (PanelDraw::ArrowMethod)method, color ) ? 1 : 0);
}

// horizontalGradient():
int pd_horizontalGradient( LWPanelID panel, DrawFuncs *df,
                           int x, int y, int width, int height,
                           int left_r, int left_g, int left_b,
                           int right_r, int right_g, int right_b ) {
  return (PanelDraw::DrawHorizontalGradient( panel, df, x, y, width, height, left_r, left_g, left_b, right_r, right_g, right_b ) ? 1 : 0);
}

// verticalGradient():
int pd_verticalGradient( LWPanelID panel, DrawFuncs *df,
                         int x, int y, int width, int height,
                         int top_r, int top_g, int top_b,
                         int bottom_r, int bottom_g, int bottom_b ) {
  return (PanelDraw::DrawVerticalGradient( panel, df, x, y, width, height, top_r, top_g, top_b, bottom_r, bottom_g, bottom_b ) ? 1 : 0);
}

// panelResizeHandle():
int pd_panelResizeHandle( LWPanelFuncs * panel_funcs, LWPanelID panel, int auto_hilight ) {
  return (PanelDraw::DrawPanelResizeHandle( panel_funcs, panel, ((auto_hilight == 0) ? 0: 1) ) ? 1 : 0);
}
