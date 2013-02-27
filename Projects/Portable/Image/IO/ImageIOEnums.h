//
// ImageIOEnums.h
//
// Constains error constants returned by many of the image
// loaders and savers. These can be passed into the
// appropriate string arrays in ImageIOStrings.h to get
// an error message to present to the user.
//

#ifndef IMAGEIOENUMS_HEADER
#define IMAGEIOENUMS_HEADER

// Loader Error IDs
enum ImageLoaderErrorIDs {
  IMGLOADER_NO_ERROR = 0,         // No Error
  IMGLOADER_UNKNOWN_ERROR,        // Unknown Error
  IMGLOADER_FILE_NOT_FOUND,       // File Not Found
  IMGLOADER_UNKNOWN_HEADER_ID,    // Unknown Header ID Type (ie:  trying to load an IFF (FROM????ILBM) with the BMP loader (BM))
  IMGLOADER_UNEXPECTED_EOF,       // Unexpected EOF
  IMGLOADER_UNKNOWN_COMPRESSION,  // Unknown Compression Type

  IMGLOADER_NUM_ERRORS            // Number of error messages
};

// Saver Error States
enum ImageSaverErrorIDs {
  IMGSAVER_NO_ERROR = 0,        // No Error
  IMGSAVER_UNKNOWN_ERROR,       // Unknown Error
  IMGSAVER_FILE_LOCKED,         // Output file is locked
  IMGSAVER_FILE_READ_ONLY,      // Output file is read-only
  IMGSAVER_DISK_FULL,           // Out of disk space

  IMGSAVER_NUM_ERRORS           // Number of error messages
};

#endif