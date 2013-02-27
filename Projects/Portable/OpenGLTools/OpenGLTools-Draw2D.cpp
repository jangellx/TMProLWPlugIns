//
// OpenGLTools-Draw2D.cpp
//

#include "OpenGLTools-Draw2D.h"

// DrawCheckerboard():
//  Draws an RGB checkerboard in the panel.  This is similar to the
//  ImageClass Checkerboard class, but doesn't require an image class;
//  instead, it is drawn directly to the display.  It also draws a
//  series of filled rectangles with DrawRGBBox(), rather than drawing
//  pixel-by-pixel, which is quite slow in LWPanels.
//  
//    Argument                Effect
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd_r, odd_g, odd_b     odd check color
//   even_r, even_g, even_b  even check color
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
bool GLTDraw2D::Checkerboard( int low_x, int low_y,
                              int width, int height, int check_w, int check_h,
                              int odd_r,  int odd_g,  int odd_b,
                              int even_r, int even_g, int even_b,
                              int check_mask ) {

  // Setup some variables
  int   odd[ 3] = { odd_r,  odd_g,  odd_b  };
  int   even[3] = { even_r, even_g, even_b };

  // Convert the coordinates into Top-Left zero-zero mode
  int high_x = low_x + width;
  int high_y = low_y + height;

  // Make sure width and height are in-bounds
  high_x = high_x < 0 ? 0 : high_x;
  high_y = high_y < 0 ? 0 : high_y;

  // Get ready for the loop
  bool x_even = false;
  bool y_even = true;
  int  next_toggle_x = check_w + low_x;
  int  next_toggle_y = check_h + low_y;
  int  x, y, cdraw_w, cdraw_h;

  for( y = low_y; y < high_y; y+=check_h ) {
    y_even = !y_even;

    // Compute the check height (check for edge of image)
    cdraw_h = check_h;
    if( (y + check_h) > high_y )
      cdraw_h = high_y - y;

    x_even = y_even;
    for( x = low_x; x < high_x; x+=check_w ) {
      x_even = !x_even;
      // Compute the check width (check for edge of image)
      cdraw_w = check_w;
      if( (x + check_w) > high_x )
        cdraw_w = high_x - x;

      // Determine the check color
      if( y_even ) {
        if( x_even ) {
          if( check_mask & CHECKMASK_EVEN )
            glColor3ub( even[0], even[1], even[2] );
        } else {
          if( check_mask & CHECKMASK_ODD )
            glColor3ub( odd[0], odd[1], odd[2] );
        }
      } else { // Invert even and odd
        if( x_even ) {
          if( check_mask & CHECKMASK_ODD)
            glColor3ub( even[0], even[1], even[2] );
        } else
          glColor3ub( odd[0], odd[1], odd[2] );
      }

      // Draw the check
      glRecti( x, y, x+cdraw_w, y+cdraw_h );
    }
  }

  return true;
}

// EmptyBox():
//  Draws an empty box
bool GLTDraw2D::EmptyBox( int x, int y, int width, int height,
                          int x_thickness, int y_thickness,
                          int shine_r,  int shine_g,  int shine_b,
                          int shadow_r, int shadow_g, int shadow_b ) {
  glColor3ub( shine_r,  shine_g,  shine_b );
  glRecti( x, y, x + width, y + x_thickness );                          // Top
  glRecti( x, y, x + y_thickness, y + height );                         // Left

  glColor3ub( shadow_r, shadow_g, shadow_b );
  glRecti( x + width - y_thickness, y, x + width, y + height );         // Right
  glRecti( x, y + height - x_thickness, x + width, y + height );        // Bottom

  return true;
}

// FilledBox():
//  Draws a filled box
bool GLTDraw2D::FilledBox( int x, int y, int width, int height,
                           int x_thickness, int y_thickness,
                           int shine_r,  int shine_g,  int shine_b,
                           int shadow_r, int shadow_g, int shadow_b,
                           int fill_r,   int fill_g,   int fill_b ) {

  EmptyBox( x, y, width, height, x_thickness, y_thickness,
            shine_r, shine_g, shine_b, shadow_r, shadow_g, shadow_b );

  glColor3ub( fill_r, fill_g, fill_b );
  glRecti( x+x_thickness, y+y_thickness,
           x + width - x_thickness, y + height - y_thickness );

  return true;
}

// Button():
//  Draws a button.  This is the same as FilledBox(), but adds
//   a 1-pixel wide black border around it
bool GLTDraw2D::Button( int x, int y, int width, int height,
                        int x_thickness, int y_thickness,
                        int shine_r,  int shine_g,  int shine_b,
                        int shadow_r, int shadow_g, int shadow_b,
                        int fill_r,   int fill_g,   int fill_b ) {

  FilledBox( x+1, y+1, width-2, height-2, x_thickness, y_thickness,
             shine_r, shine_g, shine_b, shadow_r, shadow_g, shadow_b,
             fill_r, fill_g, fill_b );

  EmptyBox( x, y, width, height, 1, 1,
            0, 0, 0,   0, 0, 0 );

  return true;
}


// DrawImage():
//  Draws an ImageClass instance into a LWPanel UI at the
//   coordinates specified.  Note that the image will clip
//   per standard OpenGL bitmap drawing rules.  Currently
//   this only handles RGB images correctly, returning
//   false otherwise.
bool GLTDraw2D::DrawImage( ImageClass *image ) {
  if( image == NULL )
    return false;

  // Change this when more types are supported
  if( image->GetType() != IMAGE_RGB )
    return false;

  glDrawPixels( image->GetWidth(), image->GetHeight(),
                GL_RGB, GL_UNSIGNED_BYTE, image->GetBuffer() );

  return true;
}
