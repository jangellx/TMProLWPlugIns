//
// IMP-FromPanelDraw.h
//
// Source borrowed from portable\lw_sdk_cpp_wrappers\paneldraw[.h|.cpp]
//

#include <lwpanel.h>

class PanelDraw {
public:
  // HorizontalDivider:  Draws a Horizontal Divider
  static bool HorizontalDivider( LWPanelID panel, DrawFuncs *df,                          
                                 int x, int y, int width, int thickness );

  // VerticalDivider:  Draws a Vertical Divider
  static bool VerticalDivider( LWPanelID panel, DrawFuncs *df,                            
                                 int x, int y, int width, int thickness );

  // EmptyBox:  Draws an empty box
  static bool EmptyBox( LWPanelID panel, DrawFuncs *df,                                   
                        int x, int y, int width, int height,
                        int x_thickness, int y_thickness,
                        int shine_color, int shadow_color );

  // FilledBox:  Draws a filled box
  static bool FilledBox( LWPanelID panel, DrawFuncs *df,                                  
                         int x, int y, int width, int height,
                         int x_thickness, int y_thickness,
                         int shine_color,  int shadow_color,  int fill_color );

  // ButtonBox:  Draws a 5.6-style button
  static bool ButtonBox( LWPanelID panel, DrawFuncs *df,                                  
                         int x, int y, int width, int height,
                         int x_thickness, int y_thickness,
                         int shine_r, int shadow_color, int fill_color );

  // Button:  Draws an L6-style button
  static bool Button( LWPanelID panel, DrawFuncs *df, int x, int y, int width, int height,
                      int brightest_r, int brightest_g, int brightest_b, bool invert = false );

  // DrawFoldIcon:  Draws a Fold/Unfold Hierarchy Icon
  enum FoldedType {
    triangle,
    plus_minus };

  static bool DrawFoldIcon( LWPanelID panel, DrawFuncs *df,
                            int x, int y, int w, int h, int color,
                            bool folded, enum FoldedType type );

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

  static bool DrawLWIcon( LWPanelID panel, DrawFuncs *df,             
                          int x, int y, enum IconType type,
                          int prime_color  = RGB_(   0,   0,   0 ),
                          int second_color = RGB_( 198, 195, 198 ),
                          enum IconMode mode = draw_L6 );

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

  static bool DrawArrow( LWPanelID panel, DrawFuncs *df,                              
                         int x, int y,
                         enum ArrowType type, enum ArrowMethod method = arrow_solid,
                         int color = RGB_( 0, 0, 0 ) );

  // DrawResizeHandle():  Draws a resize handle in the lower-right corner of a panel
  static bool DrawPanelResizeHandle( LWPanelFuncs *panel_funcs, LWPanelID panel, bool auto_hilight = true );
};

