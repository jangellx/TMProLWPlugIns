//
// ImageIDs.h
//
// This file contains a number of enumarations and defines
// used by the ImageClass, as well as others.
//

#ifndef IMAGEENUMS_HEADER
#define IMAGEENUMS_HEADER

// Strings (as defined in ImageIDs.cpp)
extern const char *ImageTypeStrings[];
extern const char *ImageErrors[];

// Image Types
enum ImageTypes {
  IMAGE_INDEXED = 0,  // Palette-based image with up to 256 colors
  IMAGE_GREY    = 1,  // 8-bit greyscale image
  IMAGE_GRAY    = 1,  // 8-bit greyscale image
  IMAGE_RGB,          // 24-bit color image
  IMAGE_RGBA,         // 24-bit color image with an 8-bit alpha channel

  NUM_IMAGE_TYPES     // Number of Image Types
};

// Image Errors
enum ImageErrorIDs {
  IMAGE_NO_ERROR = 0,             // Returned if there is no error
  IMAGE_OUT_OF_RAM,               // Returned if there was insufficent RAM for an operation
  IMAGE_VALUE_OUT_OF_RANGE,       // Returned if the value passed in was out of range
  IMAGE_INVALID_TYPE,             // Returned if an invalid image type is passed to the constructor
  IMAGE_NOT_ENOUGH_REGISTERS,     // Returned if not enough registers (ie: < 1) are passed to the constructor
  IMAGE_INVALID_ZEROZERO,         // Returned if an invalid zero-zero state is pased in.

  IMAGE_WRONG_MODE_FOR_FUNCTION,  // Returned if the image is in the wrong mode for the function

  NUM_IMAGE_ERRORS                // Number of Image Errors
};

// Zero-Zero modes
enum ImageZeroZero {
  IMAGE_ZEROZERO_INVALID = 0,  // Invalid Mode
  IMAGE_TOPLEFT,               // Top-Left of the buffer is 0, 0
  IMAGE_BOTTOMLEFT,            // Bottom-left of the buffer is 0, 0

  NUM_IMAGE_ZEROZERO           // Number of Image Zero-Zero modes     
};

// TypeDefs
typedef unsigned char IMG_BYTE;         // Used to represent a single color channel

#endif