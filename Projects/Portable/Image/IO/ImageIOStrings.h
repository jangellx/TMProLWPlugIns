//
// ImageIOStrings.h
//
// Contains Error strings returned by the various image loaders
// and savers.  Strings in the array are mapped to the enums in
// ImageIOEnums.h
//

#ifndef IMAGEIOSTRINGS_HEADER
#define IMAGEIOSTRINGS_HEADER

extern const char *ImageLoaderErrors[] = {
  "No Error",
  "Unknown Error",
  "File Not Found",
  "Unknown Header ID",
  "Unexpected EOF",
  "Unknown Image Compression"
};

extern const char *ImageSaverErrors[] = {
  "No Error",
  "Unknown Error",
  "File Locked",
  "File is Read-Only or Write Protected",
  "Disk is Full"
};

#endif