//
// RasterDraw.cpp
//

#include "RasterDraw.h"

// DrawCheckerboard():
//  Draws an RGB checkerboard in the raster.  This is similar to the
//  ImageClass Checkerboard class, but doesn't require an image class;
//  instead, it is drawn directly to the display.  It also draws a
//  series of filled rectangles with DrawRGBBox(), rather than drawing
//  pixel-by-pixel, which is quite slow in LWrasters.
//  
//    Argument                Effect
//   df                      pointer to a CLWRasterFuncs instance
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd_color               odd check color, created with the LWrasters RGB_ macro
//   even_color              even check color, created with the LWrasters RGB_ macro
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
bool RasterDraw::Checkerboard( LWRasterID raster, LWRasterFuncs *df, int low_x, int low_y,       // New and Improved
                              int width, int height, int check_w, int check_h,
                              int odd_color, int even_color,
                              int check_mask ) {

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

      if( y_even ) {
        if( x_even ) {
          if( check_mask & CHECKMASK_EVEN )
            df->drawBox( raster, even_color, x, y, cdraw_w, cdraw_h );
        } else {
          if( check_mask & CHECKMASK_ODD )
            df->drawBox( raster, odd_color, x, y, cdraw_w, cdraw_h );
        }
      } else { // Invert even and odd
        if( x_even ) {
          if( check_mask & CHECKMASK_ODD)
            df->drawBox( raster, even_color, x, y, cdraw_w, cdraw_h );
        } else {
            df->drawBox( raster, odd_color, x, y, cdraw_w, cdraw_h );
        }
      }
    }
  }

  return true;
}

// HorizontalDivider():
//  Draws a horizontal divider centered at that y specified and left
//   aligned at the x specified with the given thickness.  Minimum
//   thickness is 3 pixels.
bool RasterDraw::HorizontalDivider( LWRasterID raster, LWRasterFuncs *df,               // New and Improved
                                   int x, int y, int width, int thickness ) {
  int line_thickness = thickness/3;
  if( line_thickness < 1 )
    line_thickness = 1;

  df->drawRGBBox( raster, 100, 100, 100, x+1, y-1, width-2, line_thickness );
  df->drawRGBBox( raster,   0,   0,   0, x,   y,   width,   line_thickness );
  df->drawRGBBox( raster, 200, 200, 200, x+1, y+1, width-2, line_thickness );

  return true;
}

// VerticalDivider():
//  Draws a vertical divider centered at that x specified and top
//   aligned at the y specified with the given thickness.  Minimum
//   thickness is 3 pixels.
bool RasterDraw::VerticalDivider( LWRasterID raster, LWRasterFuncs *df,                 // New and Improved
                                 int x, int y, int height, int thickness ) {
  int line_thickness = thickness/3;
  if( line_thickness < 1 )
    line_thickness = 1;

  df->drawRGBBox( raster, 100, 100, 100, x-1, y+1, line_thickness, height-2 );
  df->drawRGBBox( raster,   0,   0,   0, x,   y,   line_thickness, height   );
  df->drawRGBBox( raster, 200, 200, 200, x+1, y+1, line_thickness, height-2 );

  return true;
}

// EmptyBox():
//  Draws an empty box
bool RasterDraw::EmptyBox( LWRasterID raster, LWRasterFuncs *df,
                          int x, int y, int width, int height,
                          int x_thickness, int y_thickness,
                          int shine_color, int shadow_color ) {
  df->drawBox( raster, shine_color,  x,                     y,                      width,       x_thickness );
  df->drawBox( raster, shadow_color, x+width-y_thickness,   y,                      y_thickness, height      );
  df->drawBox( raster, shine_color,  x,                     y,                      y_thickness, height      );
  df->drawBox( raster, shadow_color, x,                     y+height-x_thickness,   width,       x_thickness );

  return true;
}

// FilledBox():
//  Draws a filled box
bool RasterDraw::FilledBox( LWRasterID raster, LWRasterFuncs *df,
                           int x, int y, int width, int height,
                           int x_thickness, int y_thickness,
                           int shine_color, int shadow_color, int fill_color ) {

  EmptyBox( raster, df, x, y, width, height, x_thickness, y_thickness, shine_color, shadow_color );
  df->drawBox( raster, fill_color, x+y_thickness, y+x_thickness, width-(y_thickness*2), height-(x_thickness*2) );

  return true;
}

// ButtonBox():
//  Draws a 5.6-style button.  This is the same as FilledBox(), but adds
//   a 1-pixel wide black border around it
bool RasterDraw::ButtonBox( LWRasterID raster, LWRasterFuncs *df,
                           int x, int y, int width, int height,
                           int x_thickness, int y_thickness,
                           int shine_color, int shadow_color, int fill_color ) {

  FilledBox( raster, df, x+1, y+1, width-2, height-2, x_thickness, y_thickness,
             shine_color, shadow_color, fill_color );

  EmptyBox( raster, df, x, y, width, height, 1, 1, RGB_( 0, 0, 0 ), RGB_( 0, 0, 0 ) );

  return true;
}

// Button():
//  Draws an L6-style button.  This calls EmptyBox() to draw a grey border,
//   filling it with a vertical gradient.
bool RasterDraw::Button( LWRasterID raster, LWRasterFuncs *df, int x, int y, int width, int height,
                        int brightest_r, int brightest_g, int brightest_b, bool invert ) {

  EmptyBox( raster, df, x, y, width, height, 1, 1, 0x01505050, 0x01505050 );

  if( invert ) {
    DrawVerticalGradient( raster, df, x+1, y+1, width-2, height-2,
                          brightest_r-60, brightest_g-60, brightest_b-60,
                          brightest_r, brightest_g, brightest_b );
  } else {
    DrawVerticalGradient( raster, df, x+1, y+1, width-2, height-2,
                          brightest_r, brightest_g, brightest_b,
                          brightest_r-44, brightest_g-44, brightest_b-44 );
  }

  return true;
}

// DrawImage():
//  Draws an ImageClass instance into a LWraster UI at the
//   coordinates specified.
bool RasterDraw::DrawImage( LWRasterID raster, LWRasterFuncs *df, ImageClass *image, int x, int y ) {
  if( image == NULL )
    return false;

  const IMG_BYTE * rgb;
  for( int iy = 0; iy < image->GetHeight(); iy++ ) {
    for( int ix = 0; ix < image->GetWidth(); ix++ ) {
      rgb = image->GetRGBAt( ix, iy );
      df->drawRGBPixel( raster, rgb[0], rgb[1], rgb[2], x+ix, y+iy );
    }
  }

  return true;
}



// DrawFoldIcon():
//  Draws an fold icon if folded is true, and an unfold icon
//   if false at the given X and Y coordinates at the given
//   width and height.  The icon is not anti-aliased.   
//  If type is triangle, then a triangular, Mac-like fold
//   icon will be draw (pointing down when unfolded, and
//   pointing right when folded).  If it is plus_minus,
//   then a - is drawn if the unfolded, and a + if folded.
//   The later is similar to how Lightwave draws it's
//   hierarchy list.
//  The triangle mode is similar in shape and color to the
//   L6 Item Fold Icon.  Note that w and h are ignored for
//   this type.
bool RasterDraw::DrawFoldIcon( LWRasterID raster, LWRasterFuncs *df,
                              int x, int y, int w, int h, int color, 
                              bool folded, enum FoldedType type ) {
  if( (w < 1) || (h < 1) )
    return false;

  // Triangle
  if( type == triangle ) {
    if( folded )
      DrawArrow( raster, df, x, y, arrow_right_narrow, arrow_solid, color );
    else
      DrawArrow( raster, df, x, y, arrow_down_narrow, arrow_solid, color );
  }
  
  // Plus/Minus
  if( type == plus_minus ) {
    df->drawBox( raster, color, x + (int)(w*0.2)+1, y + (h/2), (int)(w * 0.6), 1 );     // Horiz Line
    if( folded )
      df->drawBox( raster, color, x + (w/2), y + (int)(h*0.2)+1, 1, (int)(h * 0.6) );   // Vert Line
  }

  return true;
}

// GetLWColor():
//  Fills in an array with the RGB color values matching the ItemColor
//   type passed in.  These correlate to the ShowObject colors in
//   Layout.
IMG_BYTE *RasterDraw::GetLWColor( IMG_BYTE *rgb, ItemColor color ) {
  switch( color ) {
  case black:
    rgb[0] = 0;
    rgb[1] = 0;
    rgb[2] = 0;
    break;

  case gray:
  case gray_2:
    rgb[0] = 198;
    rgb[1] = 195;
    rgb[2] = 198;
    break;

  case blue:
    rgb[0] =  57;
    rgb[1] = 125;
    rgb[2] = 255;
    break;

  case green:
    rgb[0] =  57;
    rgb[1] = 255;
    rgb[2] =  57;
    break;

  case cyan:
    rgb[0] =  57;
    rgb[1] = 255;
    rgb[2] = 255;
    break;

  case red:
    rgb[0] = 255;
    rgb[1] =  60;
    rgb[2] =  57;
    break;
    
  case purple:
    rgb[0] = 255;
    rgb[1] =  60;
    rgb[2] = 255;
    break;
    
  case orange:
    rgb[0] = 255;
    rgb[1] = 190;
    rgb[2] =  57;
    break;
  }

  return rgb;
}

// DrawLWIcon():
//  Draws a Lightwave icon into the interface.  The icon drawn is
//   determined by the type passed in.  Icons are 13x11 pixels,
//   drawn in the color specified.
bool RasterDraw::DrawLWIcon( LWRasterID raster, LWRasterFuncs *df,
                            int x, int y, enum IconType type,
                            int prime_color, int second_color,
                            IconMode mode ) {
  switch( type ) {
    case object:
      if( mode == draw_56 ) {
        df->drawBox( raster, prime_color, x+6, y+3, 3, 1 );
        df->drawBox( raster, prime_color, x+5, y+4, 5, 3 );
        df->drawBox( raster, prime_color, x+6, y+7, 3, 1 );
      } else {
        df->drawBox(   raster, prime_color, x+ 5, y+ 2,  7, 1 );
        df->drawPixel( raster, prime_color, x+ 4, y+ 3 );
        df->drawPixel( raster, prime_color, x+10, y+ 3 );
        df->drawBox(   raster, prime_color, x+ 4, y+ 4,  5, 1 );
        df->drawBox(   raster, prime_color, x+ 3, y+ 4,  1, 7 );
        df->drawBox(   raster, prime_color, x+ 4, y+10,  5, 1 );
        df->drawBox(   raster, prime_color, x+ 9, y+ 4,  1, 7 );
        df->drawBox(   raster, prime_color, x+ 9, y+ 4,  1, 7 );
        df->drawBox(   raster, prime_color, x+11, y+ 3,  1, 7 );
        df->drawPixel( raster, prime_color, x+10, y+9 );

        df->drawBox(   raster, second_color, x+ 5, y+ 3, 5, 1 );
        df->drawBox(   raster, second_color, x+10, y+ 4, 1, 5 );
        df->drawBox(   raster, second_color, x+ 4, y+ 5, 5, 5 );
      }
      break;

    case light:
      if( mode == draw_56 ) {
        df->drawPixel( raster, prime_color, x+11, y+2 );
        df->drawPixel( raster, prime_color, x+10, y+3 );
        df->drawBox(   raster, prime_color, x+ 5, y+4, 5, 3 );
        df->drawPixel( raster, prime_color, x+10, y+7 );
        df->drawPixel( raster, prime_color, x+11, y+8 );
      } else {
        df->drawPixel( raster, prime_color, x+11, y+ 2 );
        df->drawPixel( raster, prime_color, x+10, y+ 3 );
        df->drawBox(   raster, prime_color, x+ 4, y+ 4, 5, 1 );
        df->drawBox(   raster, prime_color, x+ 3, y+ 4, 1, 5 );
        df->drawBox(   raster, prime_color, x+ 9, y+ 4, 1, 5 );
        df->drawBox(   raster, prime_color, x+ 4, y+ 8, 5, 1 );
        df->drawBox(   raster, second_color, x+ 4, y+ 5, 5, 3 );
        df->drawPixel( raster, prime_color, x+10, y+ 9 );
        df->drawPixel( raster, prime_color, x+11, y+10 );
      }
      break;

    case camera:
      if( mode == draw_56 ) {
        df->drawPixel( raster, prime_color, x+ 5, y+2 );
        df->drawPixel( raster, prime_color, x+ 9, y+2 );
        df->drawBox(   raster, prime_color, x+ 4, y+3, 3, 1 );
        df->drawBox(   raster, prime_color, x+ 8, y+3, 3, 1 );
        df->drawPixel( raster, prime_color, x+ 5, y+4 );
        df->drawPixel( raster, prime_color, x+ 9, y+4 );
        df->drawBox(   raster, prime_color, x+ 5, y+5, 5, 3 );
        df->drawBox(   raster, prime_color, x+10, y+6, 2, 1 );
      } else {
        df->drawLine(  raster, prime_color, x+ 4, y+2, x+ 6, y+2 );
        df->drawLine(  raster, prime_color, x+ 8, y+2, x+10, y+2 );
        df->drawLine(  raster, prime_color, x+ 3, y+3, x+ 3, y+5 );
        df->drawLine(  raster, prime_color, x+11, y+3, x+11, y+5 );

        df->drawBox(   raster, second_color, x+ 4, y+3, 3, 3 );
        df->drawBox(   raster, second_color, x+ 8, y+3, 3, 3 );

        df->drawPixel( raster, prime_color,  x+ 7, y+3 );
        df->drawLine(  raster, second_color, x+ 7, y+4, x+7, y+5 );

        df->drawBox(   raster, prime_color,  x+ 4, y+ 6, 7, 1 );
        df->drawBox(   raster, prime_color,  x+ 4, y+10, 7, 1 );
        df->drawBox(   raster, prime_color,  x+ 4, y+ 6, 1, 5 );
        df->drawBox(   raster, prime_color,  x+10, y+ 6, 1, 5 );
        df->drawBox(   raster, second_color, x+ 5, y+ 7, 5, 3 );

        df->drawBox(   raster, prime_color,  x+11, y+8, 2, 1 );
        df->drawBox(   raster, prime_color,  x+13, y+7, 1, 3 );
      }
      break;

    case bone:
      if( mode == draw_56 ) {
        df->drawPixel( raster, prime_color, x+ 4, y+4 );
        df->drawPixel( raster, prime_color, x+10, y+4 );
        df->drawBox(   raster, prime_color, x+ 5, y+5, 5, 1 );
        df->drawPixel( raster, prime_color, x+ 4, y+6 );
        df->drawPixel( raster, prime_color, x+10, y+6 );
      } else {
        df->drawPixel( raster, prime_color, x+ 5, y+2 );
        df->drawPixel( raster, prime_color, x+ 4, y+3 );
        df->drawPixel( raster, prime_color, x+ 3, y+4 );
        df->drawBox(   raster, prime_color, x+ 6, y+3, 1, 2 );
        df->drawBox(   raster, prime_color, x+ 4, y+5, 2, 1 );

        df->drawPixel( raster, prime_color, x+ 7, y+5 );
        df->drawPixel( raster, prime_color, x+ 6, y+6 );
        df->drawPixel( raster, prime_color, x+ 8, y+6 );
        df->drawPixel( raster, prime_color, x+ 7, y+7 );

        df->drawBox(   raster, prime_color, x+ 9, y+ 7, 2, 1 );
        df->drawBox(   raster, prime_color, x+ 8, y+ 8, 1, 2 );
        df->drawPixel( raster, prime_color, x+ 9, y+10 );
        df->drawPixel( raster, prime_color, x+10, y+ 9 );
        df->drawPixel( raster, prime_color, x+11, y+ 8 );

        df->drawPixel( raster, second_color, x+ 5, y+ 3 );
        df->drawBox(   raster, second_color, x+ 4, y+ 4, 2, 1 );
        df->drawPixel( raster, second_color, x+ 6, y+ 5 );
        df->drawPixel( raster, second_color, x+ 7, y+ 6 );
        df->drawPixel( raster, second_color, x+ 8, y+ 7 );
        df->drawBox(   raster, second_color, x+ 9, y+ 8, 2, 1 );
        df->drawPixel( raster, second_color, x+ 9, y+ 9 );
      }
      break;

    case check:
      df->drawPixel( raster, prime_color, x+11, y+ 3 );
      df->drawBox(   raster, prime_color, x+10, y+ 4, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 9, y+ 5, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 2, y+ 6, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 8, y+ 6, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 7, y+ 7, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 2, y+ 7, 3, 1 );
      df->drawBox(   raster, prime_color, x+ 3, y+ 8, 5, 1 );
      df->drawBox(   raster, prime_color, x+ 4, y+ 9, 3, 1 );
      df->drawPixel( raster, prime_color, x+ 5, y+10 );
      break;

    case dot:
      df->drawBox( raster, prime_color, x+5, y+4, 3, 1 );
      df->drawBox( raster, prime_color, x+4, y+5, 5, 3 );
      df->drawBox( raster, prime_color, x+5, y+8, 3, 1 );
      break;

    case draw_bounding_box:
      EmptyBox( raster, df, x+3, y+2, 9, 9, 1, 1,
                prime_color,  prime_color );
      break;

    case draw_every_other_solid:
      // Not supprted in L6; use the same draw routine for both
      df->drawBox( raster, prime_color, x+ 3, y+2, 4, 4 );
      df->drawBox( raster, prime_color, x+ 6, y+5, 4, 4 );
      break;

    case draw_every_other_wire:
      // Not supprted in L6; use the same draw routine for both
      EmptyBox( raster, df, x+3, y+2, 4, 4, 1, 1,
                prime_color,  prime_color );
      EmptyBox( raster, df, x+6, y+5, 4, 4, 1, 1,
                prime_color,  prime_color );
      break;

    case draw_front_face:
      if( mode == draw_56 ) {
        EmptyBox( raster, df, x+3, y+2, 7, 7, 1, 1,
                  second_color,  second_color );
        df->drawPixel( raster, prime_color, x+5, y+3 );
        df->drawPixel( raster, prime_color, x+7, y+3 );
        df->drawPixel( raster, prime_color, x+9, y+3 );

        df->drawPixel( raster, prime_color, x+4, y+4 );
        df->drawPixel( raster, prime_color, x+6, y+4 );
        df->drawPixel( raster, prime_color, x+8, y+4 );

        df->drawPixel( raster, prime_color, x+5, y+5 );
        df->drawPixel( raster, prime_color, x+7, y+5 );
        df->drawPixel( raster, prime_color, x+9, y+5 );

        df->drawPixel( raster, prime_color, x+4, y+6 );
        df->drawPixel( raster, prime_color, x+6, y+6 );
        df->drawPixel( raster, prime_color, x+8, y+6 );

        df->drawPixel( raster, prime_color, x+5, y+7 );
        df->drawPixel( raster, prime_color, x+7, y+7 );
        df->drawPixel( raster, prime_color, x+9, y+7 );
      } else {
        EmptyBox( raster, df, x+2, y+2, 9, 9, 1, 1,
                  prime_color,  prime_color );
        df->drawLine( raster, prime_color, x+6, y+3, x+ 6, y+9 );
        df->drawLine( raster, prime_color, x+3, y+6, x+10, y+6 );
      }
      break;

    case draw_points_only:
      if( mode == draw_56 ) {
        df->drawPixel( raster, prime_color, x+4, y+2 );
        df->drawPixel( raster, prime_color, x+6, y+2 );
        df->drawPixel( raster, prime_color, x+8, y+2 );

        df->drawPixel( raster, prime_color, x+4, y+4 );
        df->drawPixel( raster, prime_color, x+6, y+4 );
        df->drawPixel( raster, prime_color, x+8, y+4 );

        df->drawPixel( raster, prime_color, x+4, y+6 );
        df->drawPixel( raster, prime_color, x+6, y+6 );
        df->drawPixel( raster, prime_color, x+8, y+6 );
      } else {
        df->drawPixel( raster, prime_color, x+ 3, y+2 );
        df->drawPixel( raster, prime_color, x+ 7, y+2 );
        df->drawPixel( raster, prime_color, x+11, y+2 );

        df->drawPixel( raster, prime_color, x+ 5, y+4 );
        df->drawPixel( raster, prime_color, x+ 9, y+4 );

        df->drawPixel( raster, prime_color, x+ 3, y+6 );
        df->drawPixel( raster, prime_color, x+ 7, y+6 );
        df->drawPixel( raster, prime_color, x+11, y+6 );

        df->drawPixel( raster, prime_color, x+ 5, y+8 );
        df->drawPixel( raster, prime_color, x+ 9, y+8 );

        df->drawPixel( raster, prime_color, x+ 3, y+10 );
        df->drawPixel( raster, prime_color, x+ 7, y+10 );
        df->drawPixel( raster, prime_color, x+11, y+10 );
      }
      break;

    case draw_solid:
      if( mode == draw_56 )
        df->drawBox( raster, prime_color, x+ 3, y+2, 7, 7 );
      else
        df->drawBox( raster, prime_color, x+ 2, y+2, 9, 9 );
      break;

    case draw_textured:
      if( mode == draw_56 ) {
        df->drawBox( raster, prime_color,  x+ 3, y+2, 7, 7 );
        df->drawBox( raster, second_color, x+ 4, y+3, 5, 1 );
        df->drawBox( raster, second_color, x+ 6, y+3, 1, 5 );
      } else {
        df->drawBox( raster, prime_color,  x+ 3, y+2, 9, 9 );
        df->drawBox( raster, second_color, x+ 5, y+4, 5, 1 );
        df->drawBox( raster, second_color, x+ 7, y+4, 1, 5 );
      }
      break;

    case draw_wireframe:
      if( mode == draw_56 ) {
        EmptyBox( raster, df, x+3, y+2, 7, 7, 1, 1,
                  prime_color,  prime_color );
        df->drawBox( raster, prime_color, x+ 6, y+3, 1, 5 );
        df->drawBox( raster, prime_color, x+ 4, y+5, 5, 1 );
      } else {
        EmptyBox( raster, df, x+2, y+2, 9, 9, 1, 1,
                  prime_color,  prime_color );
        df->drawLine( raster, prime_color, x+6, y+3, x+6, y+9 );
        df->drawLine( raster, prime_color, x+3, y+6, x+9, y+6 );

        df->drawPixel( raster, prime_color, x+ 3, y+ 3 );
        df->drawPixel( raster, prime_color, x+ 4, y+ 4 );
        df->drawPixel( raster, prime_color, x+ 5, y+ 5 );
        df->drawPixel( raster, prime_color, x+ 7, y+ 5 );
        df->drawPixel( raster, prime_color, x+ 8, y+ 4 );
        df->drawPixel( raster, prime_color, x+ 9, y+ 3 );

        df->drawPixel( raster, prime_color, x+ 5, y+ 7 );
        df->drawPixel( raster, prime_color, x+ 4, y+ 8 );
        df->drawPixel( raster, prime_color, x+ 3, y+ 9 );
        df->drawPixel( raster, prime_color, x+ 9, y+ 9 );
        df->drawPixel( raster, prime_color, x+ 8, y+ 8 );
        df->drawPixel( raster, prime_color, x+ 7, y+ 7 );
      }
      break;

    case lock:
      // Not in 5.6; use the same draw routine for both
      df->drawBox( raster, prime_color, x+5, y+2, 3, 1 );
      df->drawBox( raster, prime_color, x+4, y+3, 1, 3 );
      df->drawBox( raster, prime_color, x+8, y+3, 1, 3 );
      df->drawBox( raster, prime_color, x+3, y+6, 7, 5 );
      break;

    case visibility_eye:
      // Not in 5.6; use the same draw routine for both
      df->drawBox(   raster, prime_color, x+ 1, y+4, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 3, y+3, 6, 1 );
      df->drawBox(   raster, prime_color, x+ 9, y+4, 2, 1 );
      df->drawPixel( raster, prime_color, x+11, y+5 );

      df->drawPixel( raster, prime_color, x+ 1, y+7 );
      df->drawBox(   raster, prime_color, x+ 2, y+6, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 4, y+5, 1, 4 );
      df->drawBox(   raster, prime_color, x+ 5, y+5, 3, 1 );
      df->drawBox(   raster, prime_color, x+ 6, y+6, 4, 1 );
      df->drawBox(   raster, prime_color, x+ 6, y+7, 2, 1 );
      df->drawBox(   raster, prime_color, x+ 5, y+8, 3, 1 );
      df->drawBox(   raster, prime_color, x+ 5, y+9, 2, 1 );

      df->drawBox(   raster, prime_color, x+ 9, y+7, 2, 1 );
      df->drawPixel( raster, prime_color, x+10, y+8 );
      df->drawPixel( raster, prime_color, x+ 9, y+9 );
      df->drawBox(   raster, prime_color, x+ 3, y+10, 6, 1 );
      df->drawPixel( raster, prime_color, x+ 2, y+9 );
      break;

    case keyframe:
      df->drawBox( raster, prime_color, x-2,   y, 5, 1 );
      df->drawBox( raster, prime_color, x,   y-2, 1, 5 );
      break;

    case vertical_slider_knob_ticks: {
      int tick_start = -8;
      for( int i=0; i < 4; i++ ) {
        df->drawLine( raster, prime_color,  x-5, y + tick_start,   x+5, y+tick_start   );
        df->drawLine( raster, second_color, x-5, y + tick_start+1, x+5, y+tick_start+1 );
        tick_start += 5;
      }
    }  break;

    case horizontal_slider_knob_ticks: {
      int tick_start = -8;
      for( int i=0; i < 4; i++ ) {
        df->drawLine( raster, prime_color,  x + tick_start,   y-5, x+tick_start,   y+5 );
        df->drawLine( raster, second_color, x + tick_start+1, y-5, x+tick_start+1, y+5 );
        tick_start += 5;
      }
    }  break;
  }

  return true;
}

// IconWidth():
int RasterDraw::IconWidth( IconType type ) {
  switch( type ) {
    case camera:
    case bone:
    case object:
    case light:
    case check:
    case dot:
    case draw_bounding_box:
    case draw_points_only:
    case draw_wireframe:
    case draw_front_face:
    case draw_solid:
    case draw_textured:
    case lock:
    case visibility_eye:
    case draw_every_other_wire:
    case draw_every_other_solid:
      return 14;
      break;

    case keyframe:
      return 5;
      break;

    case vertical_slider_knob_ticks:
      return 10;
      break;

    case horizontal_slider_knob_ticks:
      return 21;
      break;

    default:
      return 0;
  }
}

// IconHeight():
int RasterDraw::IconHeight( IconType type ) {
  switch( type ) {
    case camera:
    case bone:
    case object:
    case check:
    case dot:
    case draw_bounding_box:
    case draw_points_only:
    case draw_wireframe:
    case draw_front_face:
    case draw_solid:
    case draw_textured:
    case lock:
    case visibility_eye:
    case draw_every_other_wire:
    case draw_every_other_solid:
      return 14;
      break;

    case keyframe:
      return 5;
      break;

    case vertical_slider_knob_ticks:
      return 21;
      break;

    case horizontal_slider_knob_ticks:
      return 10;
      break;

    default:
      return 0;
  }
}

// DrawHorizontalGradient():
bool RasterDraw::DrawHorizontalGradient( LWRasterID raster, LWRasterFuncs *df,
                                        int x, int y, int w, int h,
                                        int left_r, int left_g, int left_b,
                                        int right_r, int right_g, int right_b ) {
  for( int i=0; i < w; i++ ) {
    df->drawRGBBox( raster, left_r + (int)( (float)(right_r - left_r) * ((float)i/(float)w) ),
                           left_g + (int)( (float)(right_g - left_g) * ((float)i/(float)w) ),
                           left_b + (int)( (float)(right_b - left_b) * ((float)i/(float)w) ),
                           x+i, y, 1, h );
  }

  return true;
}

// DrawVerticalGradient():
bool RasterDraw::DrawVerticalGradient( LWRasterID raster, LWRasterFuncs *df,
                                      int x, int y, int w, int h,
                                      int top_r, int top_g, int top_b,
                                      int bottom_r, int bottom_g, int bottom_b ) {
  for( int i=0; i < h; i++ ) {
    df->drawRGBBox( raster, top_r + (int)( (float)(bottom_r - top_r) * ((float)i/(float)h) ),
                           top_g + (int)( (float)(bottom_g - top_g) * ((float)i/(float)h) ),
                           top_b + (int)( (float)(bottom_b - top_b) * ((float)i/(float)h) ),
                           x, y+i, w, 1 );
  }

  return true;
}

// DrawArrow():
//  Draws different triangular arrow shapes.  The type determines
//   the type of arrow that is drown (up, left, pointing away, etc.),
//   and the method detrmines the syle it is drawn in (solid, empty,
//   etc.)  In this case, x and y are the center of the arrow to be
//   drawn.
bool RasterDraw::DrawArrow( LWRasterID raster, LWRasterFuncs *df,
                           int x, int y, enum ArrowType type, enum ArrowMethod method,
                           int color ) {
  switch( type ) {
  // Arrow Up
  case arrow_up:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x, y-2 );
      df->drawBox( raster, color, x-1, y-1, 3, 1 );
      df->drawBox( raster, color, x-2, y,   5, 1 );
      df->drawBox( raster, color, x-3, y+1, 7, 1 );
      df->drawBox( raster, color, x-4, y+2, 9, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-5,  y+2, 10, 1 );
      df->drawBox( raster, color, x-6,  y+3, 12, 1 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y-3, 2, 1 );
      df->drawBox( raster, color, x-2,  y-2, 2, 1 );
      df->drawBox( raster, color, x+0,  y-2, 2, 1 );
      df->drawBox( raster, color, x-3,  y-1, 2, 1 );
      df->drawBox( raster, color, x+1,  y-1, 2, 1 );
      df->drawBox( raster, color, x-4,  y,   2, 1 );
      df->drawBox( raster, color, x+2,  y,   2, 1 );
      df->drawBox( raster, color, x-5,  y+1, 2, 1 );
      df->drawBox( raster, color, x+3,  y+1, 2, 1 );
      df->drawBox( raster, color, x-6,  y+2, 2, 1 );
      df->drawBox( raster, color, x+4,  y+2, 2, 1 );
      df->drawBox( raster, color, x-7,  y+3, 2, 1 );
      df->drawBox( raster, color, x+5,  y+3, 2, 1 );
      break;
    }
    break;

  // Arrow Down
  case arrow_down:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x, y+2 );
      df->drawBox( raster, color, x-1,  y+1, 3, 1 );
      df->drawBox( raster, color, x-2,  y,   5, 1 );
      df->drawBox( raster, color, x-3,  y-1, 7, 1 );
      df->drawBox( raster, color, x-4,  y-2, 9, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-5,  y-2, 10, 1 );
      df->drawBox( raster, color, x-6,  y-3, 12, 1 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y+3, 2, 1 );
      df->drawBox( raster, color, x-2,  y+2, 2, 1 );
      df->drawBox( raster, color, x+0,  y+2, 2, 1 );
      df->drawBox( raster, color, x-3,  y+1, 2, 1 );
      df->drawBox( raster, color, x+1,  y+1, 2, 1 );
      df->drawBox( raster, color, x-4,  y,   2, 1 );
      df->drawBox( raster, color, x+2,  y,   2, 1 );
      df->drawBox( raster, color, x-5,  y-1, 2, 1 );
      df->drawBox( raster, color, x+3,  y-1, 2, 1 );
      df->drawBox( raster, color, x-6,  y-2, 2, 1 );
      df->drawBox( raster, color, x+4,  y-2, 2, 1 );
      df->drawBox( raster, color, x-7,  y-3, 2, 1 );
      df->drawBox( raster, color, x+5,  y-3, 2, 1 );
      break;
    }
    break;

  // Arrow Right
  case arrow_right:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x+2, y );
      df->drawBox( raster, color, x+1, y-1, 1,  3 );
      df->drawBox( raster, color, x,   y-2, 1,  5 );
      df->drawBox( raster, color, x-1, y-3, 1,  7 );
      df->drawBox( raster, color, x-2, y-4, 1,  9 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x-2, y-6, x-2, y+5 );
      df->drawLine( raster, color, x-3, y-7, x-3, y+6 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawLine( raster, color, x-2, y-6, x+3, y-1 );
      df->drawLine( raster, color, x-2, y-5, x+3, y );
      df->drawLine( raster, color, x-2, y+4, x+3, y-1 );
      df->drawLine( raster, color, x-2, y+5, x+3, y );
      break;
    }
    break;

  // Arrow Left
  case arrow_left:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x-2, y );
      df->drawBox( raster, color, x-1,  y-1, 1,  3 );
      df->drawBox( raster, color, x,    y-2, 1,  5 );
      df->drawBox( raster, color, x+1,  y-3, 1,  7 );
      df->drawBox( raster, color, x+2,  y-4, 1,  9 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x+2, y-6, x+2, y+5 );
      df->drawLine( raster, color, x+3, y-7, x+3, y+6 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawLine( raster, color, x+2, y-6, x-3, y-1 );
      df->drawLine( raster, color, x+2, y-5, x-3, y );
      df->drawLine( raster, color, x+2, y+4, x-3, y-1 );
      df->drawLine( raster, color, x+2, y+5, x-3, y );
      break;
    }
    break;

  // Arrow Up Narrow
  case arrow_up_narrow:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x,    y-2,  1, 2 );
      df->drawBox( raster, color, x-1,  y,    3, 2 );
      df->drawBox( raster, color, x-2,  y+2,  5, 2 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x-2, y+3, x+2, y+3 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawPixel( raster, color, x-2, y+2 );
      df->drawPixel( raster, color, x+2, y+2 );

      df->drawPixel( raster, color, x-2, y+1 );
      df->drawPixel( raster, color, x+2, y+1 );

      df->drawPixel( raster, color, x-1, y );
      df->drawPixel( raster, color, x+1, y );

      df->drawPixel( raster, color, x-1, y-1 );
      df->drawPixel( raster, color, x+1, y-1 );

      df->drawPixel( raster, color, x, y-2 );
      df->drawPixel( raster, color, x, y-3 );
      break;
    }
    break;

  // Arrow Down Narrow
  case arrow_down_narrow:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x,    y+2,  1, 2 );
      df->drawBox( raster, color, x-1,  y,    3, 2 );
      df->drawBox( raster, color, x-2,  y-2,  5, 2 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x-2, y-3, x+2, y-3 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawPixel( raster, color, x-2, y-2 );
      df->drawPixel( raster, color, x+2, y-2 );

      df->drawPixel( raster, color, x-2, y-1 );
      df->drawPixel( raster, color, x+2, y-1 );

      df->drawPixel( raster, color, x-1, y );
      df->drawPixel( raster, color, x+1, y );

      df->drawPixel( raster, color, x-1, y+1 );
      df->drawPixel( raster, color, x+1, y+1 );

      df->drawPixel( raster, color, x, y+2 );
      df->drawPixel( raster, color, x, y+3 );
      break;
    }
    break;

  // Arrow Right
  case arrow_right_narrow:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x+2,  y,    2, 1 );
      df->drawBox( raster, color, x,    y-1,  2, 3 );
      df->drawBox( raster, color, x-2,  y-2,  2, 5 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x-3, y-2, x-3, y+2 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawPixel( raster, color, x-2, y-2 );
      df->drawPixel( raster, color, x-2, y+2 );

      df->drawPixel( raster, color, x-1, y-2 );
      df->drawPixel( raster, color, x-1, y+2 );

      df->drawPixel( raster, color, x, y-1 );
      df->drawPixel( raster, color, x, y+1 );

      df->drawPixel( raster, color, x+1, y-1 );
      df->drawPixel( raster, color, x+1, y+1 );

      df->drawPixel( raster, color, x+2, y );
      df->drawPixel( raster, color, x+3, y );
      break;
    }
    break;

  // Arrow Left
  case arrow_left_narrow:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-2,  y,    2, 1 );
      df->drawBox( raster, color, x,    y-1,  2, 3 );
      df->drawBox( raster, color, x+2,  y-2,  2, 5 );
      break;

    case arrow_empty:
      df->drawLine( raster, color, x+3, y-2, x+3, y+2 );
      // No break; we use the code below for the rest of the arrow

    case arrow_two_edges:
      df->drawPixel( raster, color, x+2, y-2 );
      df->drawPixel( raster, color, x+2, y+2 );

      df->drawPixel( raster, color, x+1, y-2 );
      df->drawPixel( raster, color, x+1, y+2 );

      df->drawPixel( raster, color, x, y-1 );
      df->drawPixel( raster, color, x, y+1 );

      df->drawPixel( raster, color, x-1, y-1 );
      df->drawPixel( raster, color, x-1, y+1 );

      df->drawPixel( raster, color, x-2, y );
      df->drawPixel( raster, color, x-3, y );
      break;
    }
    break;

  // Arrow Double Up
  case arrow_double_up:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-4,  y-1,  8, 1 );
      df->drawBox( raster, color, x-3,  y-2,  6, 1 );
      df->drawBox( raster, color, x-2,  y-3,  4, 1 );
      df->drawBox( raster, color, x-1,  y-4,  2, 1 );

      df->drawBox( raster, color, x-4,  y+4,  8, 1 );
      df->drawBox( raster, color, x-3,  y+3,  6, 1 );
      df->drawBox( raster, color, x-2,  y+2,  4, 1 );
      df->drawBox( raster, color, x-1,  y+1,  2, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-2,  y+4, 5, 1 );
      df->drawBox( raster, color, x-2,  y-1, 5, 1 );

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y-4, 2, 1 );
      df->drawBox( raster, color, x-2,  y-3, 2, 1 );
      df->drawBox( raster, color, x+0,  y-3, 2, 1 );
      df->drawBox( raster, color, x-3,  y-2, 2, 1 );
      df->drawBox( raster, color, x+1,  y-2, 2, 1 );
      df->drawBox( raster, color, x-4,  y-1, 2, 1 );
      df->drawBox( raster, color, x+2,  y-1, 2, 1 );

      df->drawBox( raster, color, x+2,  y+4, 2, 1 );
      df->drawBox( raster, color, x-4,  y+4, 2, 1 );
      df->drawBox( raster, color, x+1,  y+3, 2, 1 );
      df->drawBox( raster, color, x-3,  y+3, 2, 1 );
      df->drawBox( raster, color, x+0,  y+2, 2, 1 );
      df->drawBox( raster, color, x-2,  y+2, 2, 1 );
      df->drawBox( raster, color, x-1,  y+1, 2, 1 );
      break;
    }
    break;

  // Arrow Double Down
  case arrow_double_down:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-4,  y-4,  8, 1 );
      df->drawBox( raster, color, x-3,  y-3,  6, 1 );
      df->drawBox( raster, color, x-2,  y-2,  4, 1 );
      df->drawBox( raster, color, x-1,  y-1,  2, 1 );

      df->drawBox( raster, color, x-4,  y+1,  8, 1 );
      df->drawBox( raster, color, x-3,  y+2,  6, 1 );
      df->drawBox( raster, color, x-2,  y+3,  4, 1 );
      df->drawBox( raster, color, x-1,  y+4,  2, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-2,  y-4, 5, 1 );
      df->drawBox( raster, color, x-2,  y+1, 5, 1 );

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y-1, 2, 1 );
      df->drawBox( raster, color, x-2,  y-2, 2, 1 );
      df->drawBox( raster, color, x+0,  y-2, 2, 1 );
      df->drawBox( raster, color, x-3,  y-3, 2, 1 );
      df->drawBox( raster, color, x+1,  y-3, 2, 1 );
      df->drawBox( raster, color, x-4,  y-4, 2, 1 );
      df->drawBox( raster, color, x+2,  y-4, 2, 1 );

      df->drawBox( raster, color, x+2,  y+1, 2, 1 );
      df->drawBox( raster, color, x-4,  y+1, 2, 1 );
      df->drawBox( raster, color, x+1,  y+2, 2, 1 );
      df->drawBox( raster, color, x-3,  y+2, 2, 1 );
      df->drawBox( raster, color, x+0,  y+3, 2, 1 );
      df->drawBox( raster, color, x-2,  y+3, 2, 1 );
      df->drawBox( raster, color, x-1,  y+4, 2, 1 );
      break;
    }
    break;

  // Arrow Double Left
  case arrow_double_left:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x+1, y );
      df->drawBox( raster, color, x+2,  y-1,  1, 3 );
      df->drawBox( raster, color, x+3,  y-2,  1, 5 );
      df->drawBox( raster, color, x+4,  y-3,  1, 7 );
      df->drawBox( raster, color, x+5,  y-4,  1, 9 );

      df->drawPixel( raster, color, x-5, y );
      df->drawBox( raster, color, x-4,  y-1,  1, 3 );
      df->drawBox( raster, color, x-3,  y-2,  1, 5 );
      df->drawBox( raster, color, x-2,  y-3,  1, 7 );
      df->drawBox( raster, color, x-1,  y-4,  1, 9 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x+5,  y-4, 1, 8 );
      df->drawBox( raster, color, x-1,  y-4, 1, 8 );

    case arrow_two_edges:
      df->drawPixel( raster, color, x+5, y-4 );
      df->drawPixel( raster, color, x+5, y-3 );
      df->drawPixel( raster, color, x+5, y+4 );
      df->drawPixel( raster, color, x+5, y+3 );

      df->drawPixel( raster, color, x+4, y-3 );
      df->drawPixel( raster, color, x+4, y-2 );
      df->drawPixel( raster, color, x+4, y+3 );
      df->drawPixel( raster, color, x+4, y+2 );

      df->drawPixel( raster, color, x+3, y-2 );
      df->drawPixel( raster, color, x+3, y-1 );
      df->drawPixel( raster, color, x+3, y+2 );
      df->drawPixel( raster, color, x+3, y+1 );

      df->drawPixel( raster, color, x+2, y-1 );
      df->drawPixel( raster, color, x+2, y   );
      df->drawPixel( raster, color, x+2, y+1 );
      df->drawPixel( raster, color, x+2, y   );

      df->drawPixel( raster, color, x+1, y );
      df->drawPixel( raster, color, x+1, y );


      df->drawPixel( raster, color, x-1, y-4 );
      df->drawPixel( raster, color, x-1, y-3 );
      df->drawPixel( raster, color, x-1, y+4 );
      df->drawPixel( raster, color, x-1, y+3 );

      df->drawPixel( raster, color, x-2, y-3 );
      df->drawPixel( raster, color, x-2, y-2 );
      df->drawPixel( raster, color, x-2, y+3 );
      df->drawPixel( raster, color, x-2, y+2 );

      df->drawPixel( raster, color, x-3, y-2 );
      df->drawPixel( raster, color, x-3, y-1 );
      df->drawPixel( raster, color, x-3, y+2 );
      df->drawPixel( raster, color, x-3, y+1 );

      df->drawPixel( raster, color, x-4, y-1 );
      df->drawPixel( raster, color, x-4, y   );
      df->drawPixel( raster, color, x-4, y+1 );
      df->drawPixel( raster, color, x-4, y   );

      df->drawPixel( raster, color, x-5, y );
      df->drawPixel( raster, color, x-5, y );
      break;
    }
    break;

  // Arrow Double Right
  case arrow_double_right:
    switch( method ) {
    case arrow_solid:
      df->drawPixel( raster, color, x+5, y );
      df->drawBox( raster, color, x+4,  y-1,  1, 3 );
      df->drawBox( raster, color, x+3,  y-2,  1, 5 );
      df->drawBox( raster, color, x+2,  y-3,  1, 7 );
      df->drawBox( raster, color, x+1,  y-4,  1, 9 );

      df->drawPixel( raster, color, x-1, y );
      df->drawBox( raster, color, x-2,  y-1,  1, 3 );
      df->drawBox( raster, color, x-3,  y-2,  1, 5 );
      df->drawBox( raster, color, x-4,  y-3,  1, 7 );
      df->drawBox( raster, color, x-5,  y-4,  1, 9 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-5,  y-4, 1, 8 );
      df->drawBox( raster, color, x+1,  y-4, 1, 8 );

    case arrow_two_edges:
      df->drawPixel( raster, color, x+1, y-4 );
      df->drawPixel( raster, color, x+1, y-3 );
      df->drawPixel( raster, color, x+1, y+4 );
      df->drawPixel( raster, color, x+1, y+3 );

      df->drawPixel( raster, color, x+2, y-3 );
      df->drawPixel( raster, color, x+2, y-2 );
      df->drawPixel( raster, color, x+2, y+3 );
      df->drawPixel( raster, color, x+2, y+2 );

      df->drawPixel( raster, color, x+3, y-2 );
      df->drawPixel( raster, color, x+3, y-1 );
      df->drawPixel( raster, color, x+3, y+2 );
      df->drawPixel( raster, color, x+3, y+1 );

      df->drawPixel( raster, color, x+4, y-1 );
      df->drawPixel( raster, color, x+4, y   );
      df->drawPixel( raster, color, x+4, y+1 );
      df->drawPixel( raster, color, x+4, y   );

      df->drawPixel( raster, color, x+5, y );
      df->drawPixel( raster, color, x+5, y );


      df->drawPixel( raster, color, x-5, y-4 );
      df->drawPixel( raster, color, x-5, y-3 );
      df->drawPixel( raster, color, x-5, y+4 );
      df->drawPixel( raster, color, x-5, y+3 );

      df->drawPixel( raster, color, x-4, y-3 );
      df->drawPixel( raster, color, x-4, y-2 );
      df->drawPixel( raster, color, x-4, y+3 );
      df->drawPixel( raster, color, x-4, y+2 );

      df->drawPixel( raster, color, x-3, y-2 );
      df->drawPixel( raster, color, x-3, y-1 );
      df->drawPixel( raster, color, x-3, y+2 );
      df->drawPixel( raster, color, x-3, y+1 );

      df->drawPixel( raster, color, x-2, y-1 );
      df->drawPixel( raster, color, x-2, y   );
      df->drawPixel( raster, color, x-2, y+1 );
      df->drawPixel( raster, color, x-2, y   );

      df->drawPixel( raster, color, x-1, y );
      df->drawPixel( raster, color, x-1, y );
      break;
    }
    break;

  // Arrow Away Vertical (<> rotated 90 degrees)
  case arrow_away_vert:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-1,  y-1,  2, 1 );
      df->drawBox( raster, color, x-2,  y-2,  4, 1 );
      df->drawBox( raster, color, x-3,  y-3,  6, 1 );
      df->drawBox( raster, color, x-4,  y-4,  8, 1 );

      df->drawBox( raster, color, x-4,  y+4,  8, 1 );
      df->drawBox( raster, color, x-3,  y+3,  6, 1 );
      df->drawBox( raster, color, x-2,  y+2,  4, 1 );
      df->drawBox( raster, color, x-1,  y+1,  2, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-2,  y-1, 5, 1 );
      df->drawBox( raster, color, x-2,  y+1, 5, 1 );

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y-4, 2, 1 );
      df->drawBox( raster, color, x-2,  y-3, 2, 1 );
      df->drawBox( raster, color, x+0,  y-3, 2, 1 );
      df->drawBox( raster, color, x-3,  y-2, 2, 1 );
      df->drawBox( raster, color, x+1,  y-2, 2, 1 );
      df->drawBox( raster, color, x-4,  y-1, 2, 1 );
      df->drawBox( raster, color, x+2,  y-1, 2, 1 );

      df->drawBox( raster, color, x+2,  y+1, 2, 1 );
      df->drawBox( raster, color, x-4,  y+1, 2, 1 );
      df->drawBox( raster, color, x+1,  y+2, 2, 1 );
      df->drawBox( raster, color, x-3,  y+2, 2, 1 );
      df->drawBox( raster, color, x+0,  y+3, 2, 1 );
      df->drawBox( raster, color, x-2,  y+3, 2, 1 );
      df->drawBox( raster, color, x-1,  y+4, 2, 1 );
      break;
    }
    break;

  // Arrow Towards Vertical (>< rotated 90 degrees)
  case arrow_toward_vert:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-1,  y-4,  2, 1 );
      df->drawBox( raster, color, x-2,  y-3,  4, 1 );
      df->drawBox( raster, color, x-3,  y-2,  6, 1 );
      df->drawBox( raster, color, x-4,  y-1,  8, 1 );

      df->drawBox( raster, color, x-4,  y+1,  8, 1 );
      df->drawBox( raster, color, x-3,  y+2,  6, 1 );
      df->drawBox( raster, color, x-2,  y+3,  4, 1 );
      df->drawBox( raster, color, x-1,  y+4,  2, 1 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-2,  y-4, 5, 1 );
      df->drawBox( raster, color, x-2,  y+4, 5, 1 );

    case arrow_two_edges:
      df->drawBox( raster, color, x-1,  y-1, 2, 1 );
      df->drawBox( raster, color, x-2,  y-2, 2, 1 );
      df->drawBox( raster, color, x+0,  y-2, 2, 1 );
      df->drawBox( raster, color, x-3,  y-3, 2, 1 );
      df->drawBox( raster, color, x+1,  y-3, 2, 1 );
      df->drawBox( raster, color, x-4,  y-4, 2, 1 );
      df->drawBox( raster, color, x+2,  y-4, 2, 1 );

      df->drawBox( raster, color, x+2,  y+4, 2, 1 );
      df->drawBox( raster, color, x-4,  y+4, 2, 1 );
      df->drawBox( raster, color, x+1,  y+3, 2, 1 );
      df->drawBox( raster, color, x-3,  y+3, 2, 1 );
      df->drawBox( raster, color, x+0,  y+2, 2, 1 );
      df->drawBox( raster, color, x-2,  y+2, 2, 1 );
      df->drawBox( raster, color, x-1,  y+1, 2, 1 );
      break;
    }
    break;

  // Arrow Away Horiz (<>)
  case arrow_away_horiz:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-1,  y-4,  1, 8 );
      df->drawBox( raster, color, x-2,  y-3,  1, 6 );
      df->drawBox( raster, color, x-3,  y-2,  1, 4 );
      df->drawBox( raster, color, x-4,  y-1,  1, 2 );

      df->drawBox( raster, color, x+4,  y-1,  1, 2 );
      df->drawBox( raster, color, x+3,  y-2,  1, 4 );
      df->drawBox( raster, color, x+2,  y-3,  1, 6 );
      df->drawBox( raster, color, x+1,  y-4,  1, 8 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-1,  y-4, 1, 8 );
      df->drawBox( raster, color, x+1,  y-4, 1, 8 );

    case arrow_two_edges:
      df->drawPixel( raster, color, x+1, y-4 );
      df->drawPixel( raster, color, x+1, y-3 );
      df->drawPixel( raster, color, x+1, y+4 );
      df->drawPixel( raster, color, x+1, y+3 );

      df->drawPixel( raster, color, x+2, y-3 );
      df->drawPixel( raster, color, x+2, y-2 );
      df->drawPixel( raster, color, x+2, y+3 );
      df->drawPixel( raster, color, x+2, y+2 );

      df->drawPixel( raster, color, x+3, y-2 );
      df->drawPixel( raster, color, x+3, y-1 );
      df->drawPixel( raster, color, x+3, y+2 );
      df->drawPixel( raster, color, x+3, y+1 );

      df->drawPixel( raster, color, x+4, y-1 );
      df->drawPixel( raster, color, x+4, y   );
      df->drawPixel( raster, color, x+4, y+1 );
      df->drawPixel( raster, color, x+4, y   );

      df->drawPixel( raster, color, x+5, y );
      df->drawPixel( raster, color, x+5, y );


      df->drawPixel( raster, color, x-1, y-4 );
      df->drawPixel( raster, color, x-1, y-3 );
      df->drawPixel( raster, color, x-1, y+4 );
      df->drawPixel( raster, color, x-1, y+3 );

      df->drawPixel( raster, color, x-2, y-3 );
      df->drawPixel( raster, color, x-2, y-2 );
      df->drawPixel( raster, color, x-2, y+3 );
      df->drawPixel( raster, color, x-2, y+2 );

      df->drawPixel( raster, color, x-3, y-2 );
      df->drawPixel( raster, color, x-3, y-1 );
      df->drawPixel( raster, color, x-3, y+2 );
      df->drawPixel( raster, color, x-3, y+1 );

      df->drawPixel( raster, color, x-4, y-1 );
      df->drawPixel( raster, color, x-4, y   );
      df->drawPixel( raster, color, x-4, y+1 );
      df->drawPixel( raster, color, x-4, y   );

      df->drawPixel( raster, color, x-5, y );
      df->drawPixel( raster, color, x-5, y );

      break;
    }

    break;

  // Arrow Towards Horiz (><)
  case arrow_toward_horiz:
    switch( method ) {
    case arrow_solid:
      df->drawBox( raster, color, x-1,  y-1,  1, 2 );
      df->drawBox( raster, color, x-2,  y-2,  1, 4 );
      df->drawBox( raster, color, x-3,  y-3,  1, 6 );
      df->drawBox( raster, color, x-4,  y-4,  1, 8 );

      df->drawBox( raster, color, x+4,  y-4,  1, 8 );
      df->drawBox( raster, color, x+3,  y-3,  1, 6 );
      df->drawBox( raster, color, x+2,  y-2,  1, 4 );
      df->drawBox( raster, color, x+1,  y-1,  1, 2 );
      break;

    case arrow_empty:
      df->drawBox( raster, color, x-5,  y-4, 1, 8 );
      df->drawBox( raster, color, x+5,  y-4, 1, 8 );

    case arrow_two_edges:
      df->drawPixel( raster, color, x+5, y-4 );
      df->drawPixel( raster, color, x+5, y-3 );
      df->drawPixel( raster, color, x+5, y+4 );
      df->drawPixel( raster, color, x+5, y+3 );

      df->drawPixel( raster, color, x+4, y-3 );
      df->drawPixel( raster, color, x+4, y-2 );
      df->drawPixel( raster, color, x+4, y+3 );
      df->drawPixel( raster, color, x+4, y+2 );

      df->drawPixel( raster, color, x+3, y-2 );
      df->drawPixel( raster, color, x+3, y-1 );
      df->drawPixel( raster, color, x+3, y+2 );
      df->drawPixel( raster, color, x+3, y+1 );

      df->drawPixel( raster, color, x+2, y-1 );
      df->drawPixel( raster, color, x+2, y   );
      df->drawPixel( raster, color, x+2, y+1 );
      df->drawPixel( raster, color, x+2, y   );

      df->drawPixel( raster, color, x+1, y );
      df->drawPixel( raster, color, x+1, y );


      df->drawPixel( raster, color, x-5, y-4 );
      df->drawPixel( raster, color, x-5, y-3 );
      df->drawPixel( raster, color, x-5, y+4 );
      df->drawPixel( raster, color, x-5, y+3 );

      df->drawPixel( raster, color, x-4, y-3 );
      df->drawPixel( raster, color, x-4, y-2 );
      df->drawPixel( raster, color, x-4, y+3 );
      df->drawPixel( raster, color, x-4, y+2 );

      df->drawPixel( raster, color, x-3, y-2 );
      df->drawPixel( raster, color, x-3, y-1 );
      df->drawPixel( raster, color, x-3, y+2 );
      df->drawPixel( raster, color, x-3, y+1 );

      df->drawPixel( raster, color, x-2, y-1 );
      df->drawPixel( raster, color, x-2, y   );
      df->drawPixel( raster, color, x-2, y+1 );
      df->drawPixel( raster, color, x-2, y   );

      df->drawPixel( raster, color, x-1, y );
      df->drawPixel( raster, color, x-1, y );

      break;
    }
    break;

  case arrow_popup:
    df->drawPixel( raster, color, x, y+2 );
    df->drawBox( raster, color, x-1,  y+1,  3, 1 );
    df->drawBox( raster, color, x-2,  y,    5, 1 );
    df->drawBox( raster, color, x-3,  y-1,  7, 1 );
    break;
  }

  return true;
}
