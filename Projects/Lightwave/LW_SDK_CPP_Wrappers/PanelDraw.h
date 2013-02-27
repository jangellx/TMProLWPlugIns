//
// PanelDraw.h
//
// This includes the PanelDraw class, which contains useful
// functions for drawing in Lightwave Panels.  This includes
// things like drawing a divider and drawing a checkerboard,
// as well as drawing an image from an image buffer to the
// display.
//
// All PanelDraw functons are static, and so the class never
// has to be instanced.
//
// Note that many of the DrawPanel functions require a pointer
// to a panel and to a CDrawFuncs class, as defined in
// Clwran.h.  This allows all functions to work from within
// both control and panel-level draw functions.
// 
// Note also that these functions do NOT check to make sure
// the width and height are within the control or panel area,
// in order to keep the functions open to both.  Only
// conditions <0 are checked.
//

#ifndef INCLUDE_PANEL_DRAW_HEADER
#define INCLUDE_PANEL_DRAW_HEADER

#include <portable\image\image.h>                                   // Needed for the image support functions
#include "LWWrappers_L6.h"

// Button Colors:  Use these in place of the Shine, Shadow and Fill arguments for Button()
#define LW_BUTTON_COLOR_PURPLE 222, 223, 239,  90, 93, 107,  156, 158, 173
#define LW_BUTTON_COLOR_GREEN  222, 235, 222,  90, 105, 90,  156, 170, 156

class PanelDraw;

class PanelDraw {
public:
  // Checkerboard:  Draws a checkerboard
  static bool Checkerboard( LWPanelID panel, DrawFuncs *df, int low_x, int low_y,         // New and Improved
                            int width, int height, int check_w, int check_h,
                            int odd_color, int even_color,
                            int check_mask = CHECKMASK_BOTH );

  static inline bool Checkerboard( LWPanelID panel, CDrawFuncs *df, int low_x, int low_y, // Original Version
                                   int width, int height, int check_w, int check_h,
                                   int odd_r,  int odd_g,  int odd_b,
                                   int even_r, int even_g, int even_b,
                                   int check_mask = CHECKMASK_BOTH );


  // HorizontalDivider:  Draws a Horizontal Divider
  static bool HorizontalDivider( LWPanelID panel, DrawFuncs *df,                          // New and Improved
                                 int x, int y, int width, int thickness );

  static inline bool HorizontalDivider( LWPanelID panel, CDrawFuncs *df,                  // Original Version
                                        int x, int y, int width, int thickness );


  // VerticalDivider:  Draws a Vertical Divider
  static bool VerticalDivider( LWPanelID panel, DrawFuncs *df,                            // New and Improved
                                 int x, int y, int width, int thickness );

  static inline bool VerticalDivider( LWPanelID panel, CDrawFuncs *df,                    // Original Version
                                        int x, int y, int width, int thickness );


  // EmptyBox:  Draws an empty box
  static bool EmptyBox( LWPanelID panel, DrawFuncs *df,                                   // New and Improved
                        int x, int y, int width, int height,
                        int x_thickness, int y_thickness,
                        int shine_color, int shadow_color );

  static inline bool EmptyBox( LWPanelID panel, CDrawFuncs *df,                           // Original Version
                               int x, int y, int width, int height,
                               int x_thickness, int y_thickness,
                               int shine_r,  int shine_g,  int shine_b,
                               int shadow_r, int shadow_g, int shadow_b );

  // FilledBox:  Draws a filled box
  static bool FilledBox( LWPanelID panel, DrawFuncs *df,                                  // New and Improved
                         int x, int y, int width, int height,
                         int x_thickness, int y_thickness,
                         int shine_color,  int shadow_color,  int fill_color );

  static inline bool FilledBox( LWPanelID panel, CDrawFuncs *df,                          // Original Version
                                int x, int y, int width, int height,
                                int x_thickness, int y_thickness,
                                int shine_r,  int shine_g,  int shine_b,
                                int shadow_r, int shadow_g, int shadow_b,
                                int fill_r,   int fill_g,   int fill_b );

  // ButtonBox:  Draws a 5.6-style button
  static bool ButtonBox( LWPanelID panel, DrawFuncs *df,                                  // New and Imporved
                         int x, int y, int width, int height,
                         int x_thickness, int y_thickness,
                         int shine_r, int shadow_color, int fill_color );

  static inline bool ButtonBox( LWPanelID panel, CDrawFuncs *df,                          // Original version
                                int x, int y, int width, int height,
                                int x_thickness, int y_thickness,
                                int shine_r,  int shine_g,  int shine_b,
                                int shadow_r, int shadow_g, int shadow_b,
                                int fill_r,   int fill_g,   int fill_b );

  // Button:  Draws an L6-style button
  static bool Button( LWPanelID panel, DrawFuncs *df, int x, int y, int width, int height,
                      int brightest_r, int brightest_g, int brightest_b, bool invert = false );

  // DrawImage:  Draws an ImageClass
  static bool DrawImage( LWPanelID panel, DrawFuncs *df,                           // New and Improved
                         ImageClass *image, int x, int y );

  static inline bool DrawImage( LWPanelID panel, CDrawFuncs *df,                          // Original Version
                                ImageClass *image, int x, int y );


  // DrawFoldIcon:  Draws a Fold/Unfold Hierarchy Icon
  enum FoldedType {
    triangle,
    plus_minus };

  static bool DrawFoldIcon( LWPanelID panel, DrawFuncs *df,
                            int x, int y, int w, int h, int color,
                            bool folded, enum FoldedType type );

  static inline bool DrawFoldIcon( LWPanelID panel, CDrawFuncs *df,
                                   int x, int y, int w, int h,
                                   bool folded, enum FoldedType type );

  // GetLWColor:  Returns the RGB values for a LW Item Color
  enum ItemColor {
    black = -1,
    grey   = 0,
    gray   = 0,
    blue,
    green,
    cyan,
    red,
    purple,
    orange,
    grey_2,               // Redundant; grey appears to be stored as 7 in the scene file
    gray_2 = grey_2,
    num_item_colors };

  static IMG_BYTE *GetLWColor( IMG_BYTE *rgb, ItemColor color );

  // DrawLWIcon:  Draws a Lightwave Icon (bone, camera, etc).
  //              Note this list matches the defines in PanelToolsGlobal.h, sans the
  //              Old Types at the bottom.  Some of the newer types are L6 only.
  enum IconType {
    camera,
    bone,
    object,
    light,
    check,
    dot,

    draw_bounding_box,
    draw_points_only,        // aka vertices
    draw_wireframe,
    draw_front_face,
    draw_solid,
    draw_textured,

    lock,
    visibility_eye,

    keyframe,
    vertical_slider_knob_ticks,
    horizontal_slider_knob_ticks,
    
    // Old Types
    draw_every_other_wire,
    draw_every_other_solid,
    };

  enum IconMode {
     draw_56,
     draw_L6 };

  static bool DrawLWIcon( LWPanelID panel, DrawFuncs *df,             // New and Improved
                          int x, int y, enum IconType type,
                          int prime_color  = RGB_(   0,   0,   0 ),
                          int second_color = RGB_( 198, 195, 198 ),
                          enum IconMode mode = draw_L6 );

  static inline bool DrawLWIcon( LWPanelID panel, CDrawFuncs *df,     // Old Version
                                 int x, int y, enum IconType type,
                                 int r = 198, int g = 195, int b = 198,
                                 enum IconMode mode = draw_56 );

  static inline bool DrawLWIcon( LWPanelID panel, CDrawFuncs *df,     // Old Version
                                 int x, int y, enum IconType type,
                                 enum ItemColor color = grey,
                                 enum IconMode mode = draw_56 );

  static int IconWidth( IconType type );
  static int IconHeight( IconType type );

  // DrawHorizontalGradient():
  static bool DrawHorizontalGradient( LWPanelID panel, DrawFuncs *df,
                                      int x, int y, int w, int h,
                                      int left_r, int left_g, int left_b,
                                      int right_r, int right_g, int right_b );

  // DrawVerticalGradient():
  static bool DrawVerticalGradient( LWPanelID panel, DrawFuncs *df,
                                    int x, int y, int w, int h,
                                    int top_r, int top_g, int top_b,
                                    int bottom_r, int bottom_g, int bottom_b );


  // DrawArrow():  Draws different triangular arrows
  //               Note this list matches the defines in PanelToolsGlobal.h
  enum ArrowType {
    arrow_up,           // Single upward-pointing arrow
    arrow_down,         // Single downward-pointing arrow
    arrow_left,         // Single left-pointing arrow
    arrow_right,        // Single right-pointing arrow
    arrow_up_narrow,    // Single narrow upward-pointing arrow
    arrow_down_narrow,  // Single narrow downward-pointing arrow
    arrow_left_narrow,  // Single narrow left-pointing arrow
    arrow_right_narrow, // Single narrow right-pointing arrow
    arrow_double_up,    // Double upward-pointing arrow
    arrow_double_down,  // Double downward-pointing arrow
    arrow_double_left,  // Double left-pointing arrow
    arrow_double_right, // Double right-pointing arrow
    arrow_away_vert,    // One arrow pointing up over an arrow pointing down (<> rotated 90 degrees)
    arrow_away_horiz,   // One arrow pointing left on the left of an arrow pointing right (<>)
    arrow_toward_vert,  // One arrow pointing up under an arrow pointing down (>< rotated 90 degrees)
    arrow_toward_horiz, // One arrow pointing left on the right of an arrow pointing right (><)
    arrow_4way,         // 4 arrows pointing away from each other
    arrow_popup };      // A downward-pointing arrow like those used by pop-up controls

  enum ArrowMethod {
    arrow_solid,
    arrow_empty,
    arrow_two_edges };

  static bool DrawArrow( LWPanelID panel, DrawFuncs *df,                              // New and Improved
                         int x, int y,
                         enum ArrowType type, enum ArrowMethod method = arrow_solid,
                         int color = RGB_( 0, 0, 0 ) );

  static bool DrawArrow( LWPanelID panel, CDrawFuncs *df,
                         int x, int y,
                         enum ArrowType type, enum ArrowMethod method = arrow_solid,  // Original Version
                         int r=0, int g=0, int b=0 );

  // DrawResizeHandle():  Draws a resize handle in the lower-right corner of a panel
  static bool DrawPanelResizeHandle( LWPanelFuncs *panel_funcs, LWPanelID panel, bool auto_hilight = true );
};

// Checkerboard (original)
inline bool PanelDraw::Checkerboard( LWPanelID panel, CDrawFuncs *df, int low_x, int low_y,      // Original Version
                                     int width, int height, int check_w, int check_h,
                                     int odd_r,  int odd_g,  int odd_b,
                                     int even_r, int even_g, int even_b, int check_mask ) {
  return PanelDraw::Checkerboard( panel, df->orig_struct, low_x, low_y, width, height,
                                  check_w, check_h, RGB_( odd_r, odd_g, odd_b ),
                                  RGB_( even_r, even_g, even_b ), check_mask );

}

// HorizontalDivider (original)
inline bool PanelDraw::HorizontalDivider( LWPanelID panel, CDrawFuncs *df,
                                          int x, int y, int width, int thickness ) {
  return HorizontalDivider( panel, df->orig_struct, x, y, width, thickness );
}

// VerticalDivider (original)
inline bool PanelDraw::VerticalDivider( LWPanelID panel, CDrawFuncs *df,
                                          int x, int y, int width, int thickness ) {
  return VerticalDivider( panel, df->orig_struct, x, y, width, thickness );
}

// EmptyBox (original)
inline bool PanelDraw::EmptyBox( LWPanelID panel, CDrawFuncs *df,
                                 int x, int y, int width, int height,
                                 int x_thickness, int y_thickness,
                                 int shine_r,  int shine_g,  int shine_b,
                                 int shadow_r, int shadow_g, int shadow_b ) {
  return EmptyBox( panel, df->orig_struct, x, y, width, height, x_thickness, y_thickness,
                   RGB_( shine_r, shine_g, shine_b ), RGB_( shadow_r, shadow_g, shadow_b ) );
}

// FilledBox (original)
inline bool PanelDraw::FilledBox( LWPanelID panel, CDrawFuncs *df,
                                  int x, int y, int width, int height,
                                  int x_thickness, int y_thickness,
                                  int shine_r,  int shine_g,  int shine_b,
                                  int shadow_r, int shadow_g, int shadow_b,
                                  int fill_r,   int fill_g,   int fill_b ) {
  return FilledBox( panel, df->orig_struct, x, y, width, height, x_thickness, y_thickness,
                    RGB_( shine_r, shine_g, shine_b ), RGB_( shadow_r, shadow_g, shadow_b ),
                    RGB_( fill_r, fill_g, fill_b ) );
}

// ButtonBox (original)
inline bool PanelDraw::ButtonBox( LWPanelID panel, CDrawFuncs *df,
                                  int x, int y, int width, int height,
                                  int x_thickness, int y_thickness,
                                  int shine_r,  int shine_g,  int shine_b,
                                  int shadow_r, int shadow_g, int shadow_b,
                                  int fill_r,   int fill_g,   int fill_b ) {
  return ButtonBox( panel, df->orig_struct, x, y, width, height, x_thickness, y_thickness,
                    RGB_( shine_r, shine_g, shine_b ), RGB_( shadow_r, shadow_g, shadow_b ),
                    RGB_( fill_r, fill_g, fill_b ) );
                  
}

// DrawImage (original)
inline bool PanelDraw::DrawImage( LWPanelID panel, CDrawFuncs *df,
                                  ImageClass *image, int x, int y ) {
  return DrawImage( panel, df->orig_struct, image, x, y );
}

// DrawFoldIcon (original)
inline bool PanelDraw::DrawFoldIcon( LWPanelID panel, CDrawFuncs *df,
                                     int x, int y, int w, int h, bool folded, enum FoldedType type ) {
  if( type == triangle )
    return DrawFoldIcon( panel, df->orig_struct, x, y, w, h, RGB_( 200, 200, 200 ), folded, type );
  else
    return DrawFoldIcon( panel, df->orig_struct, x, y, w, h, RGB_( 0, 0, 0 ), folded, type );
}

// DrawLWIcon (original)
inline bool PanelDraw::DrawLWIcon( LWPanelID panel, CDrawFuncs *df,
                                   int x, int y, enum IconType type,
                                   int r, int g, int b, IconMode mode ) {
  return DrawLWIcon( panel, df->orig_struct, x, y, type, RGB_( r, g, b ), mode );
}

// DrawLWIcon(): (original)
//  Alternate DrawLWIcon() that takes an ItemColor instead
//   of RGB values
inline bool PanelDraw::DrawLWIcon( LWPanelID panel, CDrawFuncs *df,
                                   int x, int y, enum IconType type,
                                   enum ItemColor color, IconMode mode ) {
  IMG_BYTE rgb[3];
  GetLWColor( rgb, color );

  return DrawLWIcon( panel, df, x, y, type, rgb[0], rgb[1], rgb[2], mode );
}

// DrawArrow (original)
inline bool PanelDraw::DrawArrow( LWPanelID panel, CDrawFuncs *df,
                                  int x, int y,
                                  enum ArrowType type, enum ArrowMethod method,
                                  int r, int g, int b ) {
  return DrawArrow( panel, df->orig_struct, x, y, type, method, RGB_( r, g, b ) );
}

#endif

