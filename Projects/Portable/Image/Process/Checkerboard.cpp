//
// Checkerboard.cpp
//
// Note that these functions are pretty wasteful, duplicating
// all their code that should otherwise be shared.  This is because
// I didn't want to spend the time to figure out an elegant way
// to handle all this.  Maybe later...
//

#include <stdlib.h>
#include "CheckerBoard.h"

// Draw():
//  Draws a checkerboard in the image buffer.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
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
//    Mode                    Effect
//   IMAGE_RGB               full-color checks
//   IMAGE_RGBA              full-color checks.  Alpha is unaffected
//   IMAGE_GREY              grey checks
//   IMAGE_INDEXED           check colored to closest matching palette indices
//
bool Checkerboard::Draw( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,
                 IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b,
                 int check_mask ) {
  if( image == NULL )
    return false;

  // Handle all posible image modes
  IMG_BYTE odd, even;
  switch( image->GetType() ) {
  case IMAGE_GREY:
    odd  = ImageClass::GetValueOfRGB( odd_r,  odd_g,  odd_b  );
    even = ImageClass::GetValueOfRGB( even_r, even_g, even_b );
    return Checkerboard::DrawGrey( image, low_x, low_y,
                        width, height, check_w, check_h,
                        odd, even, check_mask );
    break;

  case IMAGE_INDEXED:
    odd  = image->FindPaletteIndex( odd_r,  odd_g,  odd_b  );
    even = image->FindPaletteIndex( even_r, even_g, even_b );
    return Checkerboard::DrawIndexed( image, low_x, low_y,
                        width, height, check_w, check_h,
                        odd, even, check_mask );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    return Checkerboard::DrawRGB( image, low_x, low_y,
                        width, height, check_w, check_h,
                        odd_r, odd_g, odd_b, even_r, even_g, even_b,
                        check_mask );
    break;
  }

  // Unknown type; fail
  return false;
}


// DrawRGB():
//  Draws an RGB checkerboard in the image buffer.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
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
//    Mode                    Effect
//   IMAGE_RGB               full-color checks
//   IMAGE_RGBA              full-color checks.  Alpha is unaffected
//   IMAGE_GREY              grey checks
//   IMAGE_INDEXED           check colored to closest matching palette indices
//
bool Checkerboard::DrawRGB( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,
                 IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b,
                 int check_mask ) {
  if( ( low_x + width  > image->GetWidth()  ) ||  // Out of the buffer; do nothing, but return true
      ( low_y + height > image->GetHeight() ) )
      return true;
  
  // Handle all posible image modes
  IMG_BYTE _odd, _even;
  switch( image->GetType() ) {
  case IMAGE_GREY:
    _odd  = ImageClass::GetValueOfRGB( odd_r,  odd_g,  odd_b  );
    _even = ImageClass::GetValueOfRGB( even_r, even_g, even_b );
    return Checkerboard::DrawGrey( image, low_x, low_y,
                        width, height, check_w, check_h,
                        _odd, _even );
    break;

  case IMAGE_INDEXED:
    _odd  = image->FindPaletteIndex( odd_r,  odd_g,  odd_b  );
    _even = image->FindPaletteIndex( even_r, even_g, even_b );
    return Checkerboard::DrawIndexed( image, low_x, low_y,
                        width, height, check_w, check_h,
                        _odd, _even );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    // Setup some variables
    IMG_BYTE * line = NULL;
    IMG_BYTE   odd[ 3] = { odd_r,  odd_g,  odd_b  };
    IMG_BYTE   even[3] = { even_r, even_g, even_b };

    // Set the image to top-left mode
    int old_zz = image->SetZeroZero( IMAGE_TOPLEFT );

    // Convert the coordinates into Top-Left zero-zero mode
    int high_x = low_x + width;
    int high_y = low_y + height;
    image->ConvertToImageZeroZero( high_x, high_y, old_zz );
    image->ConvertToImageZeroZero( low_x, low_y, old_zz );

    // Make sure width and height are in-bounds
    high_x = high_x < 0 ? 0 : high_x;
    high_y = high_y < 0 ? 0 : high_y;
    high_x = high_x > image->GetWidth()  ? image->GetWidth()  : high_x;
    high_y = high_y > image->GetHeight() ? image->GetHeight() : high_y;

    // Get ready for the loop
    bool x_even = false;
    bool y_even = false;
    int  next_toggle_x = check_w + low_x;
    int  next_toggle_y = check_h + low_y;
    int  x, y;

    for( y = low_y; y < high_y; y++ ) {
      line = image->GetLine(y);
      if( y == next_toggle_y ) {             // See it's time to draw the other check
        y_even = !y_even;
        next_toggle_y += check_h;
      }

      next_toggle_x = check_w + low_x;
      x_even = false;
      for( x = low_x; x < high_x; x++ ) {
        if( x == next_toggle_x ) {           // See it's time to draw the other check
          x_even = !x_even;
          next_toggle_x += check_w;
        }

        if( y_even ) {
          if( x_even ) {
            if( check_mask & CHECKMASK_EVEN ) {
              line[0] = even[0];
              line[1] = even[1];
              line[2] = even[2];
            }
          } else {
            if( check_mask & CHECKMASK_ODD ) {
              line[0] = odd[0];
              line[1] = odd[1];
              line[2] = odd[2];
            }
          }
        } else { // Invert even and odd
          if( x_even ) {
            if( check_mask & CHECKMASK_ODD) {
              line[0] = odd[0];
              line[1] = odd[1];
              line[2] = odd[2];
            }
          } else {
            if( check_mask & CHECKMASK_EVEN ) {
              line[0] = even[0];
              line[1] = even[1];
              line[2] = even[2];
            }
          }
        }

        line += image->GetPixelSize();       // Jump to the next pixel
      }
    }

    // Set the image back to the mode it was in before
    image->SetZeroZero( old_zz );
    return true;
  }

  // Unknown type; fail
  return false;
}


// DrawRGBA():
//  Draws a checkerboard in the image buffer and the alpha channel.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd_r, odd_g, odd_b     odd check color
//   odd_a                   odd check alpha
//   even_r, even_g, even_b  even check color
//   even_a                  even check alpha
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
//    Mode                    Effect
//   IMAGE_RGB               full-color checks
//   IMAGE_RGBA              full-color and alpha checks
//   IMAGE_GREY              grey checks
//   IMAGE_INDEXED           check colored to closest matching palette indices
//
bool Checkerboard::DrawRGBA( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,  IMG_BYTE odd_a,
                 IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b, IMG_BYTE even_a,
                 int check_mask ) {
  if( ( low_x + width  > image->GetWidth()  ) ||  // Out of the buffer; do nothing, but return true
      ( low_y + height > image->GetHeight() ) )
      return true;
  
  // Handle all posible image modes
  switch( image->GetType() ) {
  IMG_BYTE _odd, _even;
  case IMAGE_GREY:
    _odd  = ImageClass::GetValueOfRGB( odd_r,  odd_g,  odd_b  );
    _even = ImageClass::GetValueOfRGB( even_r, even_g, even_b );
    return Checkerboard::DrawGrey( image, low_x, low_y,
                        width, height, check_w, check_h,
                        _odd, _even, check_mask );
    break;

  case IMAGE_INDEXED:
    _odd  = image->FindPaletteIndex( odd_r,  odd_g,  odd_b  );
    _even = image->FindPaletteIndex( even_r, even_g, even_b );
    return Checkerboard::DrawIndexed( image, low_x, low_y,
                        width, height, check_w, check_h,
                        _odd, _even, check_mask );
    break;

  case IMAGE_RGB:
    return Checkerboard::DrawRGB( image, low_x, low_y,
                        width, height, check_w, check_h,
                        odd_r, odd_g, odd_b, even_r, even_g, even_b,
                        check_mask );
    break;

  case IMAGE_RGBA:
    // Setup some variables
    IMG_BYTE * line = NULL;
    IMG_BYTE   odd[ 4] = { odd_r,  odd_g,  odd_b,  odd_a  };
    IMG_BYTE   even[4] = { even_r, even_g, even_b, even_a };

    // Set the image to top-left mode
    int old_zz = image->SetZeroZero( IMAGE_TOPLEFT );

    // Convert the coordinates into Top-Left zero-zero mode
    int high_x = low_x + width;
    int high_y = low_y + height;
    image->ConvertToImageZeroZero( high_x, high_y, old_zz );
    image->ConvertToImageZeroZero( low_x, low_y, old_zz );

    // Make sure width and height are in-bounds
    high_x = high_x < 0 ? 0 : high_x;
    high_y = high_y < 0 ? 0 : high_y;
    high_x = high_x > image->GetWidth()  ? image->GetWidth()  : high_x;
    high_y = high_y > image->GetHeight() ? image->GetHeight() : high_y;

    // Get ready for the loop
    bool x_even = false;
    bool y_even = false;
    int  next_toggle_x = check_w + low_x;
    int  next_toggle_y = check_h + low_y;
    int  x, y;

    for( y = low_y; y < high_y; y++ ) {
      line = image->GetLine(y);
      if( y == next_toggle_y ) {             // See it's time to draw the other check
        y_even = !y_even;
        next_toggle_y += check_h;
      }

      next_toggle_x = check_w + low_x;
      x_even = false;
      for( x = low_x; x < high_x; x++ ) {
        if( x == next_toggle_x ) {           // See it's time to draw the other check
          x_even = !x_even;
          next_toggle_x += check_w;
        }

        if( y_even ) {
          if( x_even ) {
            if( check_mask & CHECKMASK_EVEN ) {
              line[0] = even[0];
              line[1] = even[1];
              line[2] = even[2];
              line[3] = even[3];
            }
          } else {
            if( check_mask & CHECKMASK_ODD ) {
              line[0] = odd[0];
              line[1] = odd[1];
              line[2] = odd[2];
              line[3] = odd[3];
            }
          }
        } else { // Invert even and odd
          if( x_even ) {
            if( check_mask & CHECKMASK_ODD) {
              line[0] = odd[0];
              line[1] = odd[1];
              line[2] = odd[2];
              line[3] = odd[3];
            }
          } else {
            if( check_mask & CHECKMASK_EVEN ) {
              line[0] = even[0];
              line[1] = even[1];
              line[2] = even[2];
              line[3] = even[3];
            }
          }
        }

        line += image->GetPixelSize();       // Jump to the next pixel
      }
    }

    // Set the image back to the mode it was in before
    image->SetZeroZero( old_zz );
    return true;
  }

  // Unknown type; fail
  return false;
}


// DrawAlpha():
//  Draws the checkerboard in the alpha channel.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd                     odd check alpha
//   even                    even check alpha
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
//    Mode                    Effect
//   IMAGE_RGB               no effect
//   IMAGE_RGBA              alpha checks
//   IMAGE_GREY              no effect
//   IMAGE_INDEXED           no effect
//
bool Checkerboard::DrawAlpha( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd,  IMG_BYTE even, int check_mask ) {
  if( ( low_x + width  > image->GetWidth()  ) ||  // Out of the buffer; do nothing, but return true
      ( low_y + height > image->GetHeight() ) )
      return true;
  
  if( image->GetType() != IMAGE_RGBA )            // Not in RGBA mode
    return true;

  // Setup some variables
  IMG_BYTE * line = NULL;

  // Set the image to top-left mode
  int old_zz = image->SetZeroZero( IMAGE_TOPLEFT );

  // Convert the coordinates into Top-Left zero-zero mode
  int high_x = low_x + width;
  int high_y = low_y + height;
  image->ConvertToImageZeroZero( high_x, high_y, old_zz );
  image->ConvertToImageZeroZero( low_x,  low_y,  old_zz );

  // Make sure width and height are in-bounds
  high_x = high_x < 0 ? 0 : high_x;
  high_y = high_y < 0 ? 0 : high_y;
  high_x = high_x > image->GetWidth()  ? image->GetWidth()  : high_x;
  high_y = high_y > image->GetHeight() ? image->GetHeight() : high_y;

  // Get ready for the loop
  bool x_even = false;
  bool y_even = false;
  int  next_toggle_x = check_w + low_x;
  int  next_toggle_y = check_h + low_y;
  int  x, y;

  for( y = low_y; y < high_y; y++ ) {
    line = image->GetLine(y);
    if( y == next_toggle_y ) {             // See it's time to draw the other check
      y_even = !y_even;
      next_toggle_y += check_h;
    }

    next_toggle_x = check_w + low_x;
    x_even = false;
    for( x = low_x; x < high_x; x++ ) {
      if( x == next_toggle_x ) {           // See it's time to draw the other check
        x_even = !x_even;
        next_toggle_x += check_w;
      }

        if( y_even ) {
          if( x_even ) {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          } else {
            if( check_mask & CHECKMASK_ODD ) {
              *line = odd;
            }
          }
        } else { // Invert even and odd
          if( x_even ) {
            if( check_mask & CHECKMASK_ODD) {
              *line = odd;
            }
          } else {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          }
        }

      line += image->GetPixelSize();       // Jump to the next pixel
    }
  }

  // Set the image back to the mode it was in before
  image->SetZeroZero( old_zz );
  return true;
}

// DrawIndexed():
//  Draws the checkerboard in the image buffer with the given index.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd                     odd check color index
//   even                    even check color index
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
//    Mode                    Effect
//   IMAGE_RGB               no effect
//   IMAGE_RGBA              no effect
//   IMAGE_GREY              no effect
//   IMAGE_INDEXED           index-colored checks
//
bool Checkerboard::DrawIndexed( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd,  IMG_BYTE even, int check_mask ) {
  if( ( low_x + width  > image->GetWidth()  ) ||  // Out of the buffer; do nothing, but return true
      ( low_y + height > image->GetHeight() ) )
      return true;
  
  if( image->GetType() != IMAGE_INDEXED )         // Not in Indexed mode
    return true;

  if( (odd  > image->GetNumRegisters()-1 ) ||
      (even > image->GetNumRegisters()-1 ) )
    return false;

  // Setup some variables
  IMG_BYTE * line = NULL;

  // Set the image to top-left mode
  int old_zz = image->SetZeroZero( IMAGE_TOPLEFT );

  // Convert the coordinates into Top-Left zero-zero mode
  int high_x = low_x + width;
  int high_y = low_y + height;
  image->ConvertToImageZeroZero( high_x, high_y, old_zz );
  image->ConvertToImageZeroZero( low_x,  low_y,  old_zz );

  // Make sure width and height are in-bounds
  high_x = high_x < 0 ? 0 : high_x;
  high_y = high_y < 0 ? 0 : high_y;
  high_x = high_x > image->GetWidth()  ? image->GetWidth()  : high_x;
  high_y = high_y > image->GetHeight() ? image->GetHeight() : high_y;

  // Get ready for the loop
  bool x_even = false;
  bool y_even = false;
  int  next_toggle_x = check_w + low_x;
  int  next_toggle_y = check_h + low_y;
  int  x, y;

  for( y = low_y; y < high_y; y++ ) {
    line = image->GetLine(y);
    if( y == next_toggle_y ) {             // See it's time to draw the other check
      y_even = !y_even;
      next_toggle_y += check_h;
    }

    next_toggle_x = check_w + low_x;
    x_even = false;
    for( x = low_x; x < high_x; x++ ) {
      if( x == next_toggle_x ) {           // See it's time to draw the other check
        x_even = !x_even;
        next_toggle_x += check_w;
      }

        if( y_even ) {
          if( x_even ) {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          } else {
            if( check_mask & CHECKMASK_ODD ) {
              *line = odd;
            }
          }
        } else { // Invert even and odd
          if( x_even ) {
            if( check_mask & CHECKMASK_ODD) {
              *line = odd;
            }
          } else {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          }
        }

      line += image->GetPixelSize();       // Jump to the next pixel
    }
  }

  // Set the image back to the mode it was in before
  image->SetZeroZero( old_zz );
  return true;
}


// DrawGrey():
//  Draws a grey checkerboard in the image buffer.
//  
//    Argument                Effect
//   image                   pointer to the image buffer to affect
//   x, y                    X and Y coordinates marking the corner
//                           of the effect, given in the zero-zero of
//                           the image.
//   width, height           width and height of the checkerboard efect
//   check_w, check_h        width and heigh of each check
//   odd                     odd check vale
//   even                    even check vale
//   check_mask              mask deciding if the odd, even or both
//                           checks should be draw (ie: "transparent"
//                           checks)
//
//    Mode                    Effect
//   IMAGE_RGB               grey checks
//   IMAGE_RGBA              grey checks; alpha is unchanged
//   IMAGE_GREY              grey checks
//   IMAGE_INDEXED           grey checks, or closest approximation
//
bool Checkerboard::DrawGrey( ImageClass *image, int low_x, int low_y,
                 int width, int height, int check_w, int check_h,
                 IMG_BYTE odd,  IMG_BYTE even, int check_mask ) {
  if( ( low_x + width  > image->GetWidth()  ) ||  // Out of the buffer; do nothing, but return true
      ( low_y + height > image->GetHeight() ) )
      return true;
  
  // Handle all posible image modes
  switch( image->GetType() ) {
  IMG_BYTE oddi, eveni;
  case IMAGE_INDEXED:
    oddi  = image->FindPaletteIndex( odd,  odd,  odd  );
    eveni = image->FindPaletteIndex( even, even, even );
    return Checkerboard::DrawIndexed( image, low_x, low_y,
                        width, height, check_w, check_h,
                        oddi, eveni, check_mask );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    return Checkerboard::DrawRGB( image, low_x, low_y,
                        width, height, check_w, check_h,
                        odd, odd, odd, even, even, even, check_mask );
    break;

  case IMAGE_GREY:
    // Setup some variables
    IMG_BYTE * line = NULL;

    // Set the image to top-left mode
    int old_zz = image->SetZeroZero( IMAGE_TOPLEFT );

    // Convert the coordinates into Top-Left zero-zero mode
    int high_x = low_x + width;
    int high_y = low_y + height;
    image->ConvertToImageZeroZero( high_x, high_y, old_zz );
    image->ConvertToImageZeroZero( low_x,  low_y,  old_zz );

    // Make sure width and height are in-bounds
    high_x = high_x < 0 ? 0 : high_x;
    high_y = high_y < 0 ? 0 : high_y;
    high_x = high_x > image->GetWidth()  ? image->GetWidth()  : high_x;
    high_y = high_y > image->GetHeight() ? image->GetHeight() : high_y;

    // Get ready for the loop
    bool x_even = false;
    bool y_even = false;
    int  next_toggle_x = check_w + low_x;
    int  next_toggle_y = check_h + low_y;
    int  x, y;

    for( y = low_y; y < high_y; y++ ) {
      line = image->GetLine(y);
      if( y == next_toggle_y ) {             // See it's time to draw the other check
        y_even = !y_even;
        next_toggle_y += check_h;
      }

      next_toggle_x = check_w + low_x;
      x_even = false;
      for( x = low_x; x < high_x; x++ ) {
        if( x == next_toggle_x ) {           // See it's time to draw the other check
          x_even = !x_even;
          next_toggle_x += check_w;
        }

        if( y_even ) {
          if( x_even ) {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          } else {
            if( check_mask & CHECKMASK_ODD ) {
              *line = odd;
            }
          }
        } else { // Invert even and odd
          if( x_even ) {
            if( check_mask & CHECKMASK_ODD) {
              *line = odd;
            }
          } else {
            if( check_mask & CHECKMASK_EVEN ) {
              *line = even;
            }
          }
        }

        line += image->GetPixelSize();       // Jump to the next pixel
      }
    }

    // Set the image back to the mode it was in before
    image->SetZeroZero( old_zz );
    return true;
  }

  // Unknown type; fail
  return false;
}
