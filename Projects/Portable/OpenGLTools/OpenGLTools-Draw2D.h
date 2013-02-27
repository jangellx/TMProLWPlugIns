//
// OpenGLTools-Draw2D.h
//
// This contains the GLTDraw2D class, which contains useful 2D drawing functions.
//

#ifndef OPENGLTOOLS_DRAW2D_HEADER
#define OPENGLTOOLS_DRAW2D_HEADER

#ifdef WIN32
#include <windows.h>
#endif

#include <gl/gl.h>

#include <portable\image\image.h>                                   // Needed for the image support functions

class GLTDraw2D;

class GLTDraw2D {
public:
  // Checkerboard:  Draws a checkerboard
  static bool Checkerboard( int low_x, int low_y,
                            int width, int height, int check_w, int check_h,
                            int odd_r,  int odd_g,  int odd_b,
                            int even_r, int even_g, int even_b,
                            int check_mask = CHECKMASK_BOTH );


  // EmptyBox:  Draws an empty box
  static bool EmptyBox( int x, int y, int width, int height,
                        int x_thickness, int y_thickness,
                        int shine_r,  int shine_g,  int shine_b,
                        int shadow_r, int shadow_g, int shadow_b );

  // FilledBox:  Draws a filled box
  static bool FilledBox( int x, int y, int width, int height,
                         int x_thickness, int y_thickness,
                         int shine_r,  int shine_g,  int shine_b,
                         int shadow_r, int shadow_g, int shadow_b,
                         int fill_r,   int fill_g,   int fill_b );

  // Button:  Draws a Button
  static bool Button( int x, int y, int width, int height,
                      int x_thickness, int y_thickness,
                      int shine_r,  int shine_g,  int shine_b,
                      int shadow_r, int shadow_g, int shadow_b,
                      int fill_r,   int fill_g,   int fill_b );

  // DrawImage:  Draws an ImageClass
  static bool DrawImage( ImageClass *image );
};

#endif
