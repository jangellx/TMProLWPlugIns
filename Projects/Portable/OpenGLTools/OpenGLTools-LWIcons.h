//
// OpenGLTools-LWIcons.h
//

#ifndef OPENGLTOOLS_LWICONS_HEADER
#define OPENGLTOOLS_LWICONS_HEADER

#ifdef WIN32
#include <windows.h>
#endif

#include <gl/gl.h>

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers.h>

#include "OpenGLTools-Draw2D.h"

class GLTLWIcons {
public:
  // HorizontalDivider:  Draws a Horizontal Divider
  static bool HorizontalDivider( int x, int y, int width,
                                 int thickness );

  // VerticalDivider:  Draws a Vertical Divider
  static bool VerticalDivider( int x, int y, int height,
                               int thickness );

  // DrawFoldIcon:  Draws a Fold/Unfold Hierarchy Icon
  static void DrawFoldIcon( int x, int y, int w, int h,
                            bool folded, enum PanelDraw::FoldedType type );

  // GetLWColor:  Returns the RGB values for a LW Item Color
  static IMG_BYTE *GetLWColor( IMG_BYTE *rgb, PanelDraw::ItemColor color );

  // DrawLWIcon:  Draws a Lightwave Icon (bone, camera, etc).
  static void DrawLWIcon( int x, int y, enum PanelDraw::IconType type,
                          int r, int g, int b,
                          enum PanelDraw::IconMode mode = PanelDraw::draw_L6 );

  static void DrawLWIcon( int x, int y, enum PanelDraw::IconType type,
                          enum PanelDraw::ItemColor color = PanelDraw::grey,
                          enum PanelDraw::IconMode mode = PanelDraw::draw_56 );

  // DrawArrow:  Draws an arrow shape in a variety of styles and directions
  static void DrawArrow( int x, int y,
                         enum PanelDraw::ArrowType type,
                         enum PanelDraw::ArrowMethod method = PanelDraw::arrow_solid,
                         int r=0, int g=0, int b=0 );

  // SetLegacyOffset():
  //  Enable this for old OpenGL cards, like the 3D Labs drivers for Oxygen 402, etc. graphics cards
  static void SetLegacyOffset( bool state ) { legacy_offset = (state) ? -1 : 0; }
  static bool GetLegacyOffset() { return (legacy_offset == -1) ? true : false; }
  static int  GetLegacyOffsetAmount() { return legacy_offset; }

protected:
  static int legacy_offset;
};

#endif
