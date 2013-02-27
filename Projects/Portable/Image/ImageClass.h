//
// ImageClass.h
//
// The ImageClass Class handles 8-bit, 24-bit and 32-bit
// images.  It is returned by an image class's loader,
// and passed into an image class's saver.
//
// The ImageClass Class can be indexed, RGB, RGBA or greyscale.
// Indexed images and greyscale images are stored with 1
// IMG_BYTE per pixel, RGB images with 3 IMG_BYTE's per pixel,
// and RGBA with 4 IMG_BYTE's per pixel.  The GetPixelSize()
// function will return the size in bytes of the current pixel.
// Since the image and line buffers are IMG_BYTE arrays, you
// should only index through the array in steps of
// GetPixelSize()
//
// Most Set and Get functions will work from other modes (ie:
// GetRGBAt() can be used in Indexed mode).  A few won't work
// for certain reasons ( GetAlphaAt() only works in RGBA mode,
// etc.)
//
// Use the ! operator to test for errors.  Use GetLastError()
// to get more specific information.  The value returned from
// GetLastError() can be used to index into the ImageErrors[]
// string array to get an error message to pass to the user.

//#pragma once
#ifndef IMAGECLASS_HEADER
#define IMAGECLASS_HEADER

#include <stdlib.h>

#include <portable/DynArray/DynArray.h>

#include "ImageIDs.h"

class ImageClass {
public:
  ImageClass( int _w, int _h, int _type, unsigned int _num_registers = 0 );
  ~ImageClass();

  // General Accessors
  inline int  GetType()       { return type;   }     // Get the image type
  inline int  GetWidth()      { return width;  }     // Get the image width
  inline int  GetHeight()     { return height; }     // Get the image height
  inline int  GetLastError()  { return error;  }     // Get the error state
         int  ResetErrorState();                     // Reset the error state

  // Buffer Options 
  inline int GetZeroZero()  {return zero_zero; }     // Get the Zero-Zero position (IMAGE_TOPLEFT, IMAGE_BOTTOMLEFT, etc.)
  inline int SetZeroZero( int zz );                  // Set the Zero-Zero position

  // Palette Accessors
  inline unsigned int GetNumRegisters() { return num_registers; }
  inline IMG_BYTE     GetPaletteIndexAt( int x, int y );
  inline IMG_BYTE     GetPaletteIndexAt( int point );
  inline IMG_BYTE *   GetPaletteColor(   unsigned int index );
  inline void         SetPaletteIndexAt( int x, int y, IMG_BYTE index );
  inline void         SetPaletteIndexAt( int point, IMG_BYTE index );
  inline bool         SetPaletteColor(   IMG_BYTE index, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b );
  inline bool         SetPaletteColor(   IMG_BYTE index, const IMG_BYTE *color );
         IMG_BYTE     FindPaletteIndex(  IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE low = 0, IMG_BYTE high = 255 );
  inline IMG_BYTE     FindPaletteIndex(  const IMG_BYTE * rgb, IMG_BYTE low, IMG_BYTE high );
         IMG_BYTE     FindPaletteIndex(  IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, DynArray<int> &array );
  inline IMG_BYTE     FindPaletteIndex(  const IMG_BYTE * rgb, DynArray<int> &array );
         bool         CopyPaletteFrom(   const IMG_BYTE * from );
  inline const IMG_BYTE * GetPalette() { return palette; }

  // Color, Value and Alpha Accessors
         const IMG_BYTE * GetRGBAt(   int x, int y );   // The RGB color.  Works in indexed and true-color mode.
  inline const IMG_BYTE * GetRGBAAt(  int x, int y );   // The RGB color and Alpha value  Works in true-color mode.
  inline const IMG_BYTE * GetHSVAt(   int x, int y );   // The HSV color.  Works in indexed and true-color mode.
  inline const IMG_BYTE * GetHSVAAt(  int x, int y );   // The HSV color and Alpha value.  Works in true-color mode.
  inline       IMG_BYTE   GetValueAt( int x, int y );   // The greyscale value of the color.  Works in all modes
  inline       IMG_BYTE   GetAlphaAt( int x, int y );   // The Alpha value.  Works in RGBA mode.

         const IMG_BYTE * GetRGBAt(   int point    );   // The RGB color at the point (point == x * y )
  inline const IMG_BYTE * GetRGBAAt(  int point    );   // The RGB color and Alpha at the point (point == x * y )
  inline const IMG_BYTE * GetHSVAt(   int point    );   // The HSV color at the point.  (point == x * y )
  inline const IMG_BYTE * GetHSVAAt(  int point    );   // The HSV color and Alpha value at the point.  (point == x * y )
  inline       IMG_BYTE   GetValueAt( int point    );   // The greyscale value of the color at the point. (point == x * y )
  inline       IMG_BYTE   GetAlphaAt( int point    );   // The Alpha value at the point. (point == x * y )

  inline void SetRGBAt(   int x, int y, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b );
  inline void SetRGBAt(   int x, int y, const IMG_BYTE * rgb );
  inline void SetRGBAAt(  int x, int y, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE a );
  inline void SetRGBAAt(  int x, int y, const IMG_BYTE * rgba );
  inline void SetValueAt( int x, int y, IMG_BYTE v );
  inline void SetAlphaAt( int x, int y, IMG_BYTE a );

  // Image Buffer Accessors
  inline IMG_BYTE * GetBuffer() { return image; } // Return a pointer to the image buffer.  Use GetIndexAt() to get a pixel at a coordinate in the buffer.
  inline IMG_BYTE * GetLine( int y );             // Return a pointer to a line of the image.  Index into the array with GetPixelSize() * x.

  // Buffer-Level Access
  inline int GetIndexAt( int x, int y );           // Returns the index into the image array matching the given coordinates
  inline int GetIndexAt( int point    );           // Returns the index into the image array matching the given point (point == x * y )
  inline int GetPixelSize() { return pixel_size; } // Returns the size of a pixel in IMG_BYTEs.  Use to index into the image array or a line in the image

  // Clear Functions
  void Clear(        IMG_BYTE r = 0, IMG_BYTE g = 0, IMG_BYTE b = 0, IMG_BYTE a = 0 );  // "Smart" Clear; Clear the buffer to the given color and alpha in any mode
  void ClearRGB(     IMG_BYTE r, IMG_BYTE g, IMG_BYTE b );              // Clear the buffer to the given color.  Won't affect the alpha
  void ClearRGBA(    IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE a );  // Clear the buffer to the given color and alpha
  void ClearAlpha(   IMG_BYTE value = 0 );                              // Clear the Alpha to the given value
  bool ClearIndexed( IMG_BYTE index = 0 );                              // Clear to the given palette index
  void ClearGrey(    IMG_BYTE value = 0 );                              // Clear to the given value

  // Utility Functions
  int          ConvertToImageZeroZero( int &x, int &y, int zz );                      // Convert the given coordinates in the given zero-zero mode to match the image's zero-zero mode
  ImageClass * ConvertTypeTo( int new_type, int num_reg = 0, IMG_BYTE *pal = NULL );  // Convert a the image to a the new type.  This is NOT an in-place change; it returns a pointer to the new image.
  IMG_BYTE   * GeneratePalette( unsigned int num_reg);                                // Generates a palette for the image.

  // Static Utility Functions
  inline static IMG_BYTE GetValueOfRGB( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b );
         static void     RGB2HSV( IMG_BYTE _r, IMG_BYTE _g, IMG_BYTE _b, IMG_BYTE &_h, IMG_BYTE &_s, IMG_BYTE &_v );  // Converts an RGB color into an HSV color
  inline static void     RGB2HSV( IMG_BYTE *rgb, IMG_BYTE *hsv);                                                      // Converts an RGB color into an HSV color
         static void     HSV2RGB( IMG_BYTE _h, IMG_BYTE _s, IMG_BYTE _v, IMG_BYTE &_r, IMG_BYTE &_g, IMG_BYTE &_b );  // Converts an HSV color into an RGB color
  inline static void     HSV2RGB( IMG_BYTE *hsv, IMG_BYTE *rgb);                                                      // Converts an HSV color into an RGB color
  inline static bool     CompareRGB( IMG_BYTE *one, IMG_BYTE *two );

  // Error Functions
  inline bool operator!();

protected:
  inline void SetErrorState( int state );

  int type;                   // Image Type
  int error;                  // Error State

  // Image Size
  int width;
  int height;
  
  int pixel_size;             // Number of elements in a pixel array (RGBA == 4; RGB == 3; Indexed == 1)
  int zero_zero;              // 0,0 in the buffer

  // Palette Colors
  unsigned int num_registers; // Number of palette registers
  IMG_BYTE *   palette;       // Palette Colors Array
  
  // Image Data
  IMG_BYTE * image;           // A 2-dimensional array (x and y) of palette indices, grey values or RGB/RGBA arrays

};

// ---
// ------------------------------------- Inline Functions ---
// ---

// ResetErrorState()
//  Resets the error state to IMAGE_NO_ERROR.  Returns
//  the previous error state.
inline int ImageClass::ResetErrorState() {
  int old_state = error;
  error = IMAGE_NO_ERROR;
  return old_state;
}

// SetErrorState()
//  Set the error state to the new value only if the old
//  error state is IMAGE_NO_ERROR.
inline void ImageClass::SetErrorState( int state ) {
  if( error != IMAGE_NO_ERROR )          // Make sure the state isn't already set
    return;

  if( state > NUM_IMAGE_ERRORS )         // Make sure the state is in range
    return;

  error = state;
}

// SetZeroZero()
//  Set the zero-zero position of the buffer.  Defaults to
//  top-left (IMAGE_TOPLEFT).  Returns the previous zero-zero
//  position.
inline int ImageClass::SetZeroZero( int zz ){ 
  if( ( zz >  NUM_IMAGE_ZEROZERO ) ||      // Invalid type
      ( zz <= IMAGE_ZEROZERO_INVALID ) ) {
    SetErrorState( IMAGE_INVALID_ZEROZERO );
    return zero_zero;
  }

  int old_zz = zero_zero;
  zero_zero = zz;
  return old_zz;
}

// GetPaletteIndexAt( x, y )
//  Return the palette index at the given coordinates.
//  Only returns a non-zero index if the image is in
//  indexed mode.
inline IMG_BYTE ImageClass::GetPaletteIndexAt( int x, int y ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return 0;
  }

  if( (x > width) ||
      (y > height) )
    return 0;

  return( image[ GetIndexAt( x, y ) ] );
}

// GetPaletteIndexAt( point )
//  Return the palette index at the given coordinates.
//  Only returns a non-zero index if the image is in
//  indexed mode.
inline IMG_BYTE ImageClass::GetPaletteIndexAt( int point ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return 0;
  }

  if( point > width * height )
    return 0;

  return( image[ GetIndexAt( point ) ] );
}

// GetPaletteColor()
//  Return the RGB color of the given palette index.
//  Only returns non-NULL if the image is in indexed
//  mode.
inline IMG_BYTE * ImageClass::GetPaletteColor( unsigned int index ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return NULL;
  }

  if( ! (index < num_registers) ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return NULL;
  }

  return &( palette[ index * sizeof( IMG_BYTE ) * 3 ] );
}

// SetPaletteColor()
//  Set the RGB color of the given palette index.
//  Only returns true if the image is in indexed
//  mode.
inline bool ImageClass::SetPaletteColor( IMG_BYTE index, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return false;
  }

  if( ! (index < num_registers) ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return false;
  }
  
  IMG_BYTE *rgb = &(palette[index * sizeof( IMG_BYTE ) * 3]);
  rgb[0] = r;
  rgb[1] = g;
  rgb[2] = b;

  return true;
}

// SetPaletteColor()
//  Set the RGB color of the given palette index.
//  Only returns true if the image is in indexed
//  mode.
inline bool ImageClass::SetPaletteColor( IMG_BYTE index, const IMG_BYTE *color ) {
  if( type != IMAGE_INDEXED ) {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return false;
  }

  if( ! (index < num_registers) ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return false;
  }
  
  IMG_BYTE *rgb = &(palette[index * sizeof( IMG_BYTE ) * 3]);
  rgb[0] = color[0];
  rgb[1] = color[1];
  rgb[2] = color[2];

  return true;
}


// SetPaletteIndexAt( x, y )
//  Set the palette index at the given coordinates.
//  Only does something if the image is in indexed mode.
inline void ImageClass::SetPaletteIndexAt( int x, int y, IMG_BYTE index ) {
  if( type == IMAGE_INDEXED )
    image[ GetIndexAt( x, y ) ] = index;
  else
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
}

// SetPaletteIndexAt( point )
//  Set the palette index at the given coordinates.
//  Only does something if the image is in indexed mode.
inline void ImageClass::SetPaletteIndexAt( int point, IMG_BYTE index ) {
  if( type == IMAGE_INDEXED )
    image[ GetIndexAt( point ) ] = index;
  else
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
}


// FindPaletteIndex()
//  Same as the other FindPaletteindex(), but takes an pointer
//  to a color array instead of individual color channels.
inline IMG_BYTE ImageClass::FindPaletteIndex( const IMG_BYTE * rgb, IMG_BYTE low, IMG_BYTE high ) {
  return FindPaletteIndex( rgb[0], rgb[1], rgb[2], low, high );
}

// FindPaletteIndex()
//  Same as the other FindPaletteindex(), but takes an pointer
//  to a color array instead of individual color channels.
inline IMG_BYTE ImageClass::FindPaletteIndex( const IMG_BYTE * rgb, DynArray<int> &array ) {
  return FindPaletteIndex( rgb[0], rgb[1], rgb[2], array );
}

// GetValueAt( x, y )
//  Return the Value at the given coordinate.
inline IMG_BYTE ImageClass::GetValueAt( int x, int y ) {
  switch( type ) {
  case IMAGE_INDEXED:                             // Indexed Image
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;   // Fix Me:  Set to something meaningful

  case IMAGE_RGB:                                 // True-Color Image
  case IMAGE_RGBA:
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;   // Fix Me:  Set to something meaningful

  case IMAGE_GREY:                                // Greyscale image
    return image[ GetIndexAt( x, y ) ];

  default:
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;
  }
}

// GetValueAt( point )
//  Return the Value at the given point.
inline IMG_BYTE ImageClass::GetValueAt( int point ) {
  switch( type ) {
  case IMAGE_INDEXED:                             // Indexed Image
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;   // Fix Me:  Set to something meaningful

  case IMAGE_RGB:                                 // True-Color Image
  case IMAGE_RGBA:
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;   // Fix Me:  Set to something meaningful

  case IMAGE_GREY:                                // Greyscale image
    return image[ GetIndexAt( point ) ];

  default:
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;
  }
}


// GetAlphaAt( x, y )
//  Return the Alpha at the given coordinate.  Only
//  valid in RGBA mode.
inline IMG_BYTE ImageClass::GetAlphaAt( int x, int y ) {
  if( type == IMAGE_RGBA )
    return (&image[ GetIndexAt( x, y ) ])[4];
  else {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;
  }
}

// GetAlphaAt( point )
inline IMG_BYTE ImageClass::GetAlphaAt( int point ) {
  if( type == IMAGE_RGBA )
    return (&image[ GetIndexAt( point ) ])[4];
  else {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return 0;
  }
}

// SetRGBAt()
//  Set the RGB value at the given coordinate.
inline void ImageClass::SetRGBAt( int x, int y, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b ) {
  IMG_BYTE *pixel;

  switch( type ) {
  case IMAGE_INDEXED:
    image[ GetIndexAt( x, y ) ] = FindPaletteIndex( r, g, b );
    break;

  case IMAGE_GREY:
    image[ GetIndexAt( x, y ) ] = ImageClass::GetValueOfRGB( r, g, b );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    break;
  }
}

// SetRGBAt()
//  Set the RGB value at the given coordinate.
inline void ImageClass::SetRGBAt( int x, int y, const IMG_BYTE * rgb ) {
  IMG_BYTE *pixel;

  switch( type ) {
  case IMAGE_INDEXED:
    image[ GetIndexAt( x, y ) ] = FindPaletteIndex( rgb[0], rgb[1], rgb[2] );
    break;

  case IMAGE_GREY:
    image[ GetIndexAt( x, y ) ] = ImageClass::GetValueOfRGB( rgb[0], rgb[1], rgb[2] );
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = rgb[0];
    pixel[1] = rgb[1];
    pixel[2] = rgb[2];
    break;
  }
}


// SetRGBAAt()
//  Set the RGBA value at the given coordinate.
inline void ImageClass::SetRGBAAt( int x, int y, IMG_BYTE r, IMG_BYTE g, IMG_BYTE b, IMG_BYTE a ) {
  IMG_BYTE *pixel;

  switch( type ) {
  case IMAGE_INDEXED:
    image[ GetIndexAt( x, y ) ] = FindPaletteIndex( r, g, b );
    break;

  case IMAGE_GREY:
    image[ GetIndexAt( x, y ) ] = ImageClass::GetValueOfRGB( r, g, b );
    break;

  case IMAGE_RGB:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    break;

  case IMAGE_RGBA:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = r;
    pixel[1] = g;
    pixel[2] = b;
    pixel[3] = a;
    break;
  }
}

// SetRGBAAt()
//  Set the RGBA value at the given coordinate.  Only
//  valid in RGBA mode.
inline void ImageClass::SetRGBAAt( int x, int y, const IMG_BYTE * rgba ) {
  IMG_BYTE *pixel;

  switch( type ) {
  case IMAGE_INDEXED:
    image[ GetIndexAt( x, y ) ] = FindPaletteIndex( rgba[0], rgba[1], rgba[2] );
    break;

  case IMAGE_GREY:
    image[ GetIndexAt( x, y ) ] = ImageClass::GetValueOfRGB( rgba[0], rgba[1], rgba[2] );
    break;

  case IMAGE_RGB:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = rgba[0];
    pixel[1] = rgba[1];
    pixel[2] = rgba[2];
    break;

  case IMAGE_RGBA:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = rgba[0];
    pixel[1] = rgba[1];
    pixel[2] = rgba[2];
    pixel[3] = rgba[3];
    break;
  }
}

// SetValueAt()
//  Set the greyscale value at the given coordinate.
//  Valid only in Greyscale mode.
inline void ImageClass::SetValueAt( int x, int y, IMG_BYTE v ) {
  IMG_BYTE *pixel;

  switch( type ) {
  case IMAGE_INDEXED:
    image[ GetIndexAt( x, y ) ] = FindPaletteIndex( v, v, v );
    break;

  case IMAGE_GREY:
    image[ GetIndexAt( x, y ) ] = v;
    break;

  case IMAGE_RGB:
  case IMAGE_RGBA:
    pixel = &( image[ GetIndexAt( x, y ) ] );
    pixel[0] = v;
    pixel[1] = v;
    pixel[2] = v;
    break;
  }
}

// SetAlphaAt()
//  Set the greyscale value at the given coordinate.
//  Valid only in Greyscale mode.
inline void ImageClass::SetAlphaAt( int x, int y, IMG_BYTE a ) {
  if( type != IMAGE_RGBA ) {
    SetErrorState( IMAGE_WRONG_MODE_FOR_FUNCTION );
    return;
  }

  (&image[ GetIndexAt( x, y ) ])[4] = a;
}

// GetLineAt()
//  Returns a pointer to a line in the image buffer.
inline IMG_BYTE * ImageClass::GetLine( int y ) {
  if( y > height ) {
    SetErrorState( IMAGE_VALUE_OUT_OF_RANGE );
    return NULL;
  }

  return &( image[ GetIndexAt( 0, y ) ] );
}

// GetIndexAt( x, y )
//  Returns the index into the image array matching
//  the given coordinates.  This also handles the
//  zero-zero modes.
inline int ImageClass::GetIndexAt( int x, int y ) {
  if( ( x >= width ) || 
      ( y >= height ) )
    return 0;

  if( zero_zero == IMAGE_TOPLEFT )
    return( (y*width + x) * sizeof(IMG_BYTE) * pixel_size ); 
  else
    return( ((height-y-1)*width + x) * sizeof(IMG_BYTE) * pixel_size ); // The extra -1 is so we get the right line.  Without it, we never get line 80 (if 80 is the last line) and line 0 is past the buffer
}

// GetIndexAt( point )
//  Returns the index into the image array matching
//  the given point.  This also handles the
//  zero-zero modes.
inline int ImageClass::GetIndexAt( int point ) {
  if( point >= width * height )
    return 0;

  if( zero_zero == IMAGE_TOPLEFT )
    return( point * sizeof(IMG_BYTE) * pixel_size ); 
  else {
    int mod = point % width;
    int retval = ((width * height) - point + (mod * 2) - width ) * sizeof(IMG_BYTE) * pixel_size;
    
    // These are here to test the above routine.  If they fail, the above line is bad.
    assert( retval < width * height );
    assert( retval >= 0 );

    return retval;
  }
}


// GetValueOfRGB()
//  Returns the value of the RGB color passed in.
//  Currently, this just averages the RGBs, but it should
//  someday do a nicer, more accurate value conversion.
inline IMG_BYTE ImageClass::GetValueOfRGB( IMG_BYTE r, IMG_BYTE g, IMG_BYTE b ) {
  return ( (r+g+b)/3 );
}

// Operator !
//  Returns true if there are no errors, and false if
//  there are.
inline bool ImageClass::operator!() {
  if( error == IMAGE_NO_ERROR )
    return false;

  return true;
}

// RGB2HSV():
//  Converts an RGB color into an HSV color
void ImageClass::RGB2HSV( IMG_BYTE *rgb, IMG_BYTE *hsv) {
  RGB2HSV( rgb[0], rgb[1], rgb[2], hsv[0], hsv[1], hsv[2] );
}

// HSV2RGB():
//  Converts an HSV color into an RGB color
void ImageClass::HSV2RGB( IMG_BYTE *hsv, IMG_BYTE *rgb) {
  HSV2RGB( hsv[0], hsv[1], hsv[2], rgb[0], rgb[1], rgb[2] );
}

// CompareRGB()
//  Compares two RGB values.  Returns true if they are exactly equal.
//   Useful for figuring out RLE compression.
inline bool CompareRGB( IMG_BYTE *one, IMG_BYTE *two ) {
  if( (one[0] == two[0]) &&
      (one[1] == two[1]) &&
      (one[2] == two[2] ) )
    return true;

  return false;
}

#endif
