//
// ImageIDs.cpp
//
// Contains the strings that ImageIDs.h point to
//

// Image Type Strings
//  Thes match up to the ImageTypes enum
extern const char *ImageTypeStrings[] = {
  "Indexed",
  "Greyscale",
  "24-bit RGB",
  "24-bit RGB with 8-bit Alhpa",
  "Unknown Image Mode"
};

// Image Error Strings
//  These match up to the ImageErrorIDs enum
extern const char *ImageErrors[] = {
  "No Error",
  "Insufficient Memory",
  "Value or Index is Out Of Range",
  "Invalid Image Type",
  "Not Enough Palette Registers",
  "Invalid Zero-Zero Type",
  "Function Failed:  Image is in the wrong mode"
};
