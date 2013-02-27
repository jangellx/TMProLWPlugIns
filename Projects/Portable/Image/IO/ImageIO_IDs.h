//
// ImageIOEnums.h
//
// Constains error constants returned by many of the image
// loaders and savers. These can be passed into the
// appropriate string arrays in ImageIOStrings.h to get
// an error message to present to the user.
//
// This also contains enums defining all supported file formats,
// compression modes, etc.
//

#ifndef IMAGEIOENUMS_HEADER
#define IMAGEIOENUMS_HEADER

// Strings (as defined in ImageIO_IDs.cpp)
extern const char *ImageLoaderErrorStrings[];
extern const char *ImageSaverErrorStrings[];
extern const char *ImageFileFormatStrings[];
extern const char *ImageFileFormatStrings_Short[];
extern const char *ImageCompressionModeStrings[];


// Loader Error IDs
enum ImageLoaderErrorIDs {
  IMGLOADER_NO_ERROR = 0,              // No Error
  IMGLOADER_UNKNOWN_ERROR,             // Unknown Error
  IMGLOADER_ERROR_OPENING_FILE,        // Error opening file; may be exclusively locked
  IMGLOADER_FILE_NOT_FOUND,            // File Not Found
  IMGLOADER_UNRECOGNIZED_FILE_FORMAT,  // Unknown Header ID Type (ie:  trying to load an IFF (FROM????ILBM) with the BMP loader (BM))
  IMGLOADER_UNEXPECTED_EOF,            // Unexpected EOF
  IMGLOADER_UNKNOWN_COMPRESSION,       // Unknown Compression Type
  IMGLOADER_INVALID_USER_DATA,         // Invalid data passed into function/header/etc. (null pointer, etc.)
  IMGLOADER_MODE_NOT_SUPPORTED,        // Mode not supported; usually this is because the image loader for this variant of the file format hasn't been written yet
  IMGLOADER_INVALID_FILENAME,          // Invalid filename passed in (possibly NULL)
  IMGLOADER_ERROR_READING_FILE,        // Error reading from the file
  IMGLOADER_ERROR_READING_HEADER,      // Error reading header

  IMGLOADER_NUM_ERRORS                 // Number of error messages
};

// Saver Error States
enum ImageSaverErrorIDs {
  IMGSAVER_NO_ERROR = 0,               // No Error
  IMGSAVER_UNKNOWN_ERROR,              // Unknown Error
  IMGSAVER_ERROR_OPENING_FILE,         // Error opening file; path may not exist
  IMGSAVER_FILE_LOCKED,                // Output file is locked
  IMGSAVER_FILE_READ_ONLY,             // Output file is read-only
  IMGSAVER_DISK_FULL,                  // Out of disk space
  IMGSAVER_INVALID_USER_DATA,          // Invalid data passed into function/header/etc. (null pointer, etc.)
  IMGSAVER_MODE_NOT_SUPPORTED,         // Mode not supported; usually this is because the image saver for this variant of the file format hasn't been written yet
  IMGSAVER_INVALID_FILENAME,           // Invalid filename passed in (possibly NULL)
  IMGSAVER_INVALID_IMAGE,              // Invalid ImageClass instance passed in (possibly NULL)
  IMGSAVER_INVALID_FILE_FORMAT,        // Invliad/Unknown File Format enum passed to saver
  IMGSAVER_ERROR_WRITING_FILE,         // Error writing to the file
  IMGSAVER_ERROR_WRITING_HEADER,       // Error reading header
  IMGSAVER_UNRECOGNIZED_FILE_FORMAT,   // Unknown File Format ID passed in

  IMGSAVER_NUM_ERRORS                  // Number of error messages
};

// Image File Formats.  Values >0 are explicit file formats
enum ImageFileFormats {
  IMGFF_ANY     =  0,           // Any file format; used when loading
  IMGFF_UNKNOWN =  0,           // Unknown file format
  IMGFF_BMP,                    // Windows BMP
  IMGFF_TARGA,                  // Targa Image File
  IMGFF_CSOURCE,                // Image as C++ Source
  // INSERT NEW FILE FORMAT HERE (IMAGEFILRFORMATS)
  IMGFF_NUM_FILE_FORMATS
};

// Image Compression Modes.  If >0, comression is active.
enum ImageCompressionModes {
  IMGCOMPRESSION_UNKNOWN =  0,
  IMGCOMPRESSION_NONE    =  1,
  IMGCOMPRESSION_RLE     =  2,
  IMGCOMPRESSION_NUM_MODES
};

#endif