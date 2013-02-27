//
// Image.cpp
//

#include <stdlib.h>
#include <math.h>
#include <memory.h>

#include "ImageClass.h"
#include "IO/ImageIO_IDs.h"

// Constructor
//  Simple constructor.  Nothing is initialized until the Init()
//  function is called later on.
ImageClass::ImageClass( int _w, int _h, int _type, unsigned int _num_registers )
  : width(_w), height(_h), type(_type), error(IMAGE_NO_ERROR),
    pixel_size(0), zero_zero(IMAGE_TOPLEFT), num_registers(0),
    palette(NULL), image(NULL) {

  // Setup the image's stuff
  switch( type ) {
  case IMAGE_INDEXED:               // Palettized Image
    num_registers = _num_registers;
    if( num_registers < 1 ) {
      error = IMAGE_NOT_ENOUGH_REGISTERS;
      return;
    }

    palette = new IMG_BYTE[ num_registers  * sizeof(IMG_BYTE) * 3 ];
    pixel_size = 1;
    break;

  case IMAGE_GREY:                  // 8-bit Greyscale Image
    pixel_size = 1;
    break;

  case IMAGE_RGB:                   // 24-bit RGB Color Image
    pixel_size = 3;
    break;

  case IMAGE_RGBA:                  // 24-bit RGB Color Image with 8-bit Alpha Channel
    pixel_size = 4;
    break;

  default:                          // Invalid Image Type
    error = IMAGE_INVALID_TYPE;
    return;
  }

  image = new IMG_BYTE[ width * height * sizeof(IMG_BYTE) * pixel_size ];

  if( image == NULL ) {
    error = IMAGE_OUT_OF_RAM;
    return;
  }

  // Clear the image out to black (or index 0)
  switch( type ) {
  case IMAGE_INDEXED:
    ClearIndexed( 0 );
    break;

  case IMAGE_GREY:
    ClearGrey( 0 );
    break;

  case IMAGE_RGB:
    ClearRGB( 0, 0, 0 );
    break;

  case IMAGE_RGBA:
    ClearRGBA( 0, 0, 0, 0 );
    break;
  }

}

// Destructor
ImageClass::~ImageClass() {
  delete image;
  delete palette;
}

// FindPaletteIndex()
//  Returns the palette index that most closely matches the given
//  RGB value.  This only works in Indexed mode.  The optional low
//  and high values specify the low and hight palette indices to
//  to search (inclusive).
IMG_BYTE ImageClass::FindPaletteIndex( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE low, IMG_BYTE high ) {
  if( type != IMAGE_INDEXED )
    return 0;

  if( low > high )
    return 0;

  if( high > num_registers - 1 )
    return 0;

  IMG_BYTE  closest    =    0;   // Closest index to the target color
  int       how_close  = 1000;   // 0 == perfect match
  int       this_try;

  int step  = sizeof( IMG_BYTE ) * 3;
  IMG_BYTE * rgb = &(palette[ low ]);

  for( int i=low; i <= high; i++ ) {
    if( r > rgb[0] )  this_try = r - rgb[0];   // Find the red difference
    else              this_try = rgb[0] - r;

    if( g > rgb[1] )  this_try += g - rgb[1];  // Find the green difference
    else              this_try += rgb[1] - g;

    if( b > rgb[2] )  this_try += b - rgb[2];  // Find the blue difference
    else              this_try += rgb[2] - b;

    if( this_try < how_close ) {              // See if this is closer than the last match
      closest   = i;
      how_close = this_try;
    }

    rgb += step;
  }

  return closest;
}

// FindPaletteIndex()
//  Returns the palette index that most closely matches the given
//  RGB value.  This only works in Indexed mode.  The optional low
//  and high values specify the low and hight palette indices to
//  to search (inclusive).  This is similar to the other
//  FindPaletteIndex() functions, but also takes a pointer to a
//  DynArray of palette indices to match against.
IMG_BYTE ImageClass::FindPaletteIndex( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, DynArray<int> &array ) {
  if( type != IMAGE_INDEXED )
    return 0;

  if( array.NumElements() == 0 )
    return 0;

  IMG_BYTE  closest    =    0;   // Closest index to the target color
  int       how_close  = 1000;   // 0 == perfect match
  int       this_try;

  int step  = sizeof( IMG_BYTE ) * 3;
  IMG_BYTE * rgb = &(palette[ (array[0]) ]);

  for( unsigned int i=0; i < array.NumElements(); i++ ) {
    rgb = GetPaletteColor( array[i] );

    if( r > rgb[0] )  this_try = r - rgb[0];   // Find the red difference
    else              this_try = rgb[0] - r;

    if( g > rgb[1] )  this_try += g - rgb[1];  // Find the green difference
    else              this_try += rgb[1] - g;

    if( b > rgb[2] )  this_try += b - rgb[2];  // Find the blue difference
    else              this_try += rgb[2] - b;

    if( this_try < how_close ) {               // See if this is closer than the last match
      closest   = array[i];
      how_close = this_try;
    }
  }

  return closest;
}

// CopyPaletteFrom()
//  Copies the given palette into the image's palette.
//  Only valid in Indexed mode.  The from palette must
//  be an array of IMG_BYTES consisting of RGB tripls,
//  just like the ImageClass's palette.  The from palette
//  must also contain at least as many color indices as
//  this image class instance.
bool ImageClass::CopyPaletteFrom( const IMG_BYTE * from ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return false;
  }

  if( from == NULL )
    return false;

  if( from == palette )
    return true;

  memcpy( palette, from, num_registers * sizeof( IMG_BYTE ) * 3 );
  return true;
}

// Clear()
//  Clear the buffer to the given color.  The inputs default to 0
//  (black, solid alpha, closest palette match).
//    Mode           Effect
//   RGB            all pixels become r,g,b
//   RGBA           all pixels become r,g,b; Alpha becomes a
//   GREY           all pixels become the value of r,g,b
//   INDEXED        all pixels become the closest palette match to r,g,b
void ImageClass::Clear( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE a ) {
  switch( type ) {
  case IMAGE_INDEXED:
    ClearIndexed( FindPaletteIndex( r, g, b ) );
    break;
  case IMAGE_RGB:
    ClearRGB( r, g, b );
    break;
  case IMAGE_RGBA:
    ClearRGBA( r, g, b, a );
    break;
  case IMAGE_GREY:
    ClearGrey( ImageClass::GetValueOfRGB( r, g, b ) );
    break;
  }
}


// ClearRGB()
//  Clear the buffer to the given color.  The inputs default to 0
//  (black, or the closest palette match).  This does not affect the
//  Alpha in any way.
//    Mode           Effect
//   RGB            all pixels become r,g,b
//   RGBA           all pixels become r,g,b; Alpha is untouched
//   GREY           all pixels become the value of r,g,b
//   INDEXED        all pixels become the closest palette match to r,g,b
void ImageClass::ClearRGB( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b ) {
  unsigned long i;
  unsigned long num_bytes = (width * height * pixel_size);

  switch( type ) {
  case IMAGE_INDEXED:
    ClearIndexed( FindPaletteIndex( r, g, b ) );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    IMG_BYTE * pixel;
    for( i=0; i < num_bytes; i += pixel_size ) {
      pixel = &(image[i]);
      pixel[0] = r;
      pixel[1] = g;
      pixel[2] = b;
    }
    break;

  case IMAGE_GREY:
    ClearGrey( GetValueOfRGB( r, g, b ) );
    break;
  } 
}

// ClearRGBA()
//  Clear the buffer to the given color.  The inputs default to 0
//  (black, solid alpha, closest palette match).
//    Mode           Effect
//   RGB            all pixels become r,g,b
//   RGBA           all pixels become r,g,b; Alpha becomes a
//   GREY           all pixels become the value of r,g,b
//   INDEXED        all pixels become the closest palette match to r,g,b
void ImageClass::ClearRGBA( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE a ) {
  unsigned long i;
  unsigned long num_bytes = (width * height * pixel_size);

  switch( type ) {
  case IMAGE_INDEXED:
    ClearIndexed( FindPaletteIndex( r, g, b ) );
    break;

  case IMAGE_RGB:
    ClearRGB( r, g, b );
    break;

  case IMAGE_RGBA:
    IMG_BYTE * pixel;
    for( i=0; i < num_bytes; i += pixel_size ) {
      pixel = &(image[i]);
      pixel[0] = r;
      pixel[1] = g;
      pixel[2] = b;
      pixel[3] = a;
    }
    break;

  case IMAGE_GREY:
    ClearGrey( GetValueOfRGB( r, g, b ) );
    break;
  } 
}

// ClearAlpha()
//  Clears the Alpha to the given value.  Defaults to 0
//  (solid alpha)
//    Mode           Effect
//   RGB            no effect
//   RGBA           alpha becomes the given value
//   GREY           no effect
//   INDEXED        no effect
void ImageClass::ClearAlpha( IMG_BYTE value ) {
  if( type != IMAGE_RGBA )
    return;

  unsigned long num_bytes = (width * height * pixel_size);
  IMG_BYTE * pixel;

  for( unsigned long i=0; i < num_bytes; i += pixel_size ) {
    pixel = &(image[i]);
    pixel[3] = value;
  }
}

// ClearIndexed()
//  Clears the buffer to the given index.  Defaults to 0.
//  Returns false if the index is out of range
//    Mode           Effect
//   RGB            no effect
//   RGBA           no effect
//   GREY           no effect
//   INDEXED        cleared to given index
bool ImageClass::ClearIndexed( IMG_BYTE index ) {
  if( type != IMAGE_INDEXED )
    return false;

  if( index > num_registers-1 )
    return false;

  unsigned long num_pixels = (width * height * pixel_size);
  for( unsigned long i=0; i < num_pixels; i += pixel_size ) {
    image[i] = index;
  }

  return true;
}


// ClearGrey()
//  Clears the alpha to the given value.  Defaults to 0
//  (black, or the closest palette index)
//    Mode           Effect
//   RGB            clears buffer to value
//   RGBA           clears buffer to value; Alpha is untouched
//   GREY           clears buffer to value
//   INDEXED        clears buffer to closest match to value
void ImageClass::ClearGrey( IMG_BYTE value ) {
  unsigned long i;
  unsigned long num_bytes = (width * height * pixel_size);

  switch( type ) {
  case IMAGE_INDEXED:
    ClearIndexed( FindPaletteIndex( value, value, value ) );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    ClearRGB( value, value, value );
    break;

  case IMAGE_GREY:
    for( i=0; i < num_bytes; i += pixel_size )
      image[i] = value;
    break;
  } 
}

// ConvertToImageZeroZero()
//  Converts the given coordinates from the given system to the
//  match the image's zero-zero setup, and returns the new
//  zero-zero type.
//
//  For example, if the image is 200 pixels tall
//  and set to IMAGE_BOTTOMLEFT, and the coordinates (15,30) are
//  given in IMAGE_TOPLEFT mode, the returned coordinates will be
//  (15,130) in the IMAGE_BOTTOMLEFT coordinate system.
int ImageClass::ConvertToImageZeroZero( int &x, int &y, int zz ) {
  if( (zz >  NUM_IMAGE_ZEROZERO) ||
      (zz <= IMAGE_ZEROZERO_INVALID ) )
    return IMAGE_ZEROZERO_INVALID;

  if( zz == zero_zero )               // Already in our zero-zero mode
    return type;

  if( (zz == IMAGE_BOTTOMLEFT) &&     // From Bottom-Left to Top-Left
      (zero_zero == IMAGE_TOPLEFT) ) {
    y = height - y;
  }

  if( (zz == IMAGE_TOPLEFT) &&        // From Top-Left to Bottom-Left 
      (zero_zero == IMAGE_BOTTOMLEFT) ) {
    y = height - y;
  }

  return zero_zero;
}

// ConvertTypeTo()
//  Converts the image from it's current type to the new type.  This
//   is NOT an in-place operation; rather, it creates and returns a
//   new ImageClass, which must be freed by the main program.  The
//   original ImageClass is not modified in any way.  For conversion
//   to indexed images, an optional palette can also be passed in.
//   Images will be quantized to match the palette as needed.
//   This function is most useful for image savers and device-dependant
//   output of images.
//  Note that if the source image doesn't need to be converted, this
//   function will return a pointer to the existing image class, so
//   be sure to check for this.  A returned value of NULL means some
//   error occured
ImageClass * ImageClass::ConvertTypeTo( int new_type,
                                        int num_reg, IMG_BYTE *pal ) {
  if( new_type == IMAGE_INVALID_TYPE )      // Invalid type; fail
    return NULL;

  if( new_type == type )                   // Already in the new mode; return this image
    return this;

  ImageClass * new_image = NULL;
  int x, y;
  IMG_BYTE       *new_pal;
  const IMG_BYTE *rgb;
  IMG_BYTE        value;

  switch( new_type ) {
  case IMAGE_RGB:       // Convert to RGB
    new_image = new ImageClass( width, height, IMAGE_RGB );
    if( !new_image || !(*new_image) ) {
      SetErrorState( IMAGE_OUT_OF_RAM );
      delete new_image;
      return NULL;
    }

    for( y=0; y < height; y++ ) {
      for( x=0; x < width; x++ ) {
        rgb = GetRGBAt( x, y );
        new_image->SetRGBAt( x, y, rgb );
      }
    }

    return new_image;
    break;

  case IMAGE_RGBA:      // Convert to RGBA
    new_image = new ImageClass( width, height, IMAGE_RGBA );
    if( !new_image || !(*new_image) ) {
      SetErrorState( IMAGE_OUT_OF_RAM );
      delete new_image;
      return NULL;
    }

    for( y=0; y < height; y++ ) {
      for( x=0; x < width; x++ ) {
        rgb = GetRGBAt( x, y );
        new_image->SetRGBAt( x, y, rgb );
      }
    }
    new_image->ClearAlpha();

    return new_image;
    break;

  case IMAGE_GREY:      // Convert to Greyscale
    new_image = new ImageClass( width, height, IMAGE_GREY );
    if( !new_image || !(*new_image) ) {
      SetErrorState( IMAGE_OUT_OF_RAM );
      delete new_image;
      return NULL;
    }

    for( y=0; y < height; y++ ) {
      for( x=0; x < width; x++ ) {
        value = GetValueAt( x, y );
        new_image->SetValueAt( x, y, value );
      }
    }

    return new_image;
    break;
  
  case IMAGE_INDEXED:   // Convert to Indexed
    new_image = new ImageClass( width, height, IMAGE_INDEXED, num_reg );
    if( !new_image || !(*new_image) ) {
      SetErrorState( IMAGE_OUT_OF_RAM );
      delete new_image;
      return NULL;
    }

    if( pal != NULL )   // Copy the new palette in, if applicable
      new_image->CopyPaletteFrom( pal );
    else if ( type == IMAGE_GREY ) {  // Generate a greyscale palette
      for( int i=0; i < 256; i++ ) {
        new_image->SetPaletteColor( i, i, i, i );
      }
    } else {              // Auto-generate a palette, if needed.
      new_pal = GeneratePalette( num_reg );
      new_image->CopyPaletteFrom( new_pal );
      delete new_pal;
    }

    for( y=0; y < height; y++ ) {
      for( x=0; x < width; x++ ) {
        if( type == IMAGE_GREY )
          value = GetValueAt( x, y );
        else {
          rgb   = GetRGBAt( x, y );
          value = new_image->FindPaletteIndex( rgb, 0, num_registers-1 );  // low/high must be a char, hence the -1.
        }

        new_image->SetPaletteIndexAt( x, y, value );
      }
    }

    return new_image;
    break;
  
  default:
    return NULL;
  }

}


// GeneratePalette()
//  Generates a new palette for the current image.  If the image
//  is already in Indexed mode with the required number of , the returned palette is a copy
//  of the original palette.  Otherwise, the image is quantized
//  to the number of registers provided and returned.  This returned
//  palette must be freed by the user.  If something goes wrong,
//  NULL is returned.  To reduce an image to the new palette, call
//  ConvertTo() with IMAGE_INDEXED and the paletter returned by this
//  function.
IMG_BYTE *ImageClass::GeneratePalette( unsigned int num_reg ) {
  if( (num_reg < 1) || (num_reg > 256)  )
    return NULL;

  IMG_BYTE *new_pal = NULL;
  if( type == IMAGE_INDEXED ) {             // Image is already in Indexed mode with the right number of registers
    if( num_reg == num_registers ) {
    memcpy( new_pal, palette, num_reg );
    return new_pal;
    }
  }

  new_pal = new IMG_BYTE[ num_reg * sizeof( IMG_BYTE ) * 3 ];
  
  // Right now, we just make a greyscale palette
  // -- Do some REAL quantizing here --
  int rgb_step     = 256/num_reg;
  int new_rgb      = 0;
  int palette_step = sizeof( IMG_BYTE ) * 3;
  IMG_BYTE * rgb   = new_pal;

  for( unsigned int i=0; i < (num_reg * palette_step); i+=palette_step ) {
    rgb[i]   = new_rgb;
    rgb[i+1] = new_rgb;
    rgb[i+2] = new_rgb;

    new_rgb += rgb_step;
  }


  return new_pal;
}

// RGB2HSV()
//  Converts an RGB color into an HSV Color
//   Based on RGB_To_HSV from Computer Graphics: Principles and
//   Practices, page 592
void ImageClass::RGB2HSV( IMG_BYTE _r, IMG_BYTE _g, IMG_BYTE _b, IMG_BYTE &_h, IMG_BYTE &_s, IMG_BYTE &_v ) {
  double min, max, r, g, b, h, s, v;

  // Normalize R, G and B
  r = (double) _r / 255.0;
  g = (double) _g / 255.0;
  b = (double) _b / 255.0;

  // V = the R, G or B; whichever is greater
  v = ( r > g ) ? r : g;
  v = ( v > b ) ? v : b;
  max = v;
  min = (r < g ) ? r : g;
  min = (min < b ) ? min : b;

  // S
  s = !(max < 0.0001) ? ( (max - min) / max ) : 0.0;
  if( s == 0.0 ) {
    h = 0;  // Color is grey; hue is undefined
  } else {
    double delta = max - min;
    if( r == max )
      h = (g - b) / delta;
    else if( g == max )
      h = 2.0 + ((b - r) / delta);
    else if( b == max )
      h = 4.0 + ((r - g) / delta);

    h *= 60;
    if( h < 0.0 )  // Make sure hue is >= 0
      h += 360.0;
    
    h = h/360;
  }

  // Convert from normalized H, S and V to 8-bit H, S and V
  _h = (IMG_BYTE)(h * 255);
  _s = (IMG_BYTE)(s * 255);
  _v = (IMG_BYTE)(v * 255);
}

// HSV2RGB()
//  Converts an HSV color into an RGB Color.
//   Based on HSV_To_RGB from Computer Graphics: Principles and
//   Practices, page 593
void ImageClass::HSV2RGB( IMG_BYTE _h, IMG_BYTE _s, IMG_BYTE _v, IMG_BYTE &_r, IMG_BYTE &_g, IMG_BYTE &_b ) {
  // Normalize H, S and V
  double h = (_h/255.0)*360.0;
  double s = _s/255.0;
  double v = _v/255.0;
  double r, g, b;

  if( s < 0.0001 ) {
    // S == 0; Greyscale
    r = v;
    g = v;
    b = v;
  } else {
    if( h > 359.9999 )            // Keep HSV between 0 and 360
      h -= 359.9999;

    h /= 60;                      // H is now between 0 and 6
    int    i = (int) floor( h );  // Get the largest int <= H
    double f = h - i;             // The fractional part of H
    double p = v * (1.0 - s);
    double q = v * (1.0 - (s * f ));
    double t = v * (1.0 - (s * (1.0 - f )));

    switch( i ) {
      case 0:
        r = v;
        g = t;
        b = p;
        break;

      case 1:
        r = q;
        g = v;
        b = p;
        break;

      case 2:
        r = p;
        g = v;
        b = t;
        break;

      case 3:
        r = p;
        g = q;
        b = v;
        break;

      case 4:
        r = t;
        g = p;
        b = v;
        break;

      case 5:
        r = v;
        g = p;
        b = q;
        break;
    }
  }

  _r = (IMG_BYTE) (r * 255);
  _g = (IMG_BYTE) (g * 255);
  _b = (IMG_BYTE) (b * 255);
}

// HSV2RGB()
//  Converts an HSV color into an RGB Color
void HSV2RGB_old( IMG_BYTE _h, IMG_BYTE _s, IMG_BYTE _l, IMG_BYTE &_r, IMG_BYTE &_g, IMG_BYTE &_b ) {
  double h, s, l, r, g, b, v;

  h = (double)_h / 255.0;
  s = (double)_s / 255.0;
  l = (double)_l / 255.0;

  v = (l <= 0.5) ? (l * (1.0 + s)) : (l + s - l * s);
  if (v <= 0)
    r = g = b = 0;
  else
    {
      double m, sv, fract, vsf, mid1, mid2;
      int sextant;

      m = l + l - v;
      sv = (v - m) / v;
      h *= 6.0;
      sextant = (int)h;
      if (sextant == 6)
        sextant--;
      fract = (double)(h - sextant);
      vsf = v * sv * fract;
      mid1 = m + vsf;
      mid2 = v - vsf;
      switch (sextant) {
        case 0: r = v; g = mid1; b = m; break;
        case 1: r = mid2; g = v; b = m; break;
        case 2: r = m; g = v; b = mid1; break;
        case 3: r = m; g = mid2; b = v; break;
        case 4: r = mid1; g = m; b = v; break;
        case 5: r = v; g = m; b = mid2; break;
        }
    }
  _r = (int)(r * 255);
  _g = (int)(g * 255);
  _b = (int)(b * 255);
}

// GetRGBAt( x, y )
//  Return the RGB color at the given coordinate.
//  Invalid in Greyscale mode
const IMG_BYTE * ImageClass::GetRGBAt( int x, int y ) {
  int index;

  if( type == IMAGE_INDEXED ) {         // Indexed Image
    index = image[ GetIndexAt( x, y ) ];
    return GetPaletteColor( index );
  } else if( type == IMAGE_GREY ) {     // Grey; Invalid
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return NULL;
  } else if( (type == IMAGE_RGBA) || ( type = IMAGE_RGB ) ) {  // True-Color Image
    return &( image[ GetIndexAt( x, y ) ] );
  }

  // Unknown; return 0
  return 0;
}

// GetRGBAt( point )
//  Return the RGB color at the given point.
//  Invalid in Greyscale mode
const IMG_BYTE * ImageClass::GetRGBAt( int point ) {
  int index;

  if( type == IMAGE_INDEXED ) {         // Indexed Image
    index = image[ GetIndexAt( point ) ];
    return GetPaletteColor( index );
  } else if( type == IMAGE_GREY ) {     // Grey; Invalid
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return NULL;
  } else if( (type == IMAGE_RGBA) || ( type = IMAGE_RGB ) ) {  // True-Color Image
    return &( image[ GetIndexAt( point ) ] );
  }

  // Unknown; return 0
  return 0;
}
