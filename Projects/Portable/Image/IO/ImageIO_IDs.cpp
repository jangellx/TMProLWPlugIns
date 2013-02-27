//
// ImageIO_IDs.cpp
//
// Contains Error strings and enums returned by the various image loaders
// and savers.

extern const char *ImageLoaderErrorStrings[] = {
  "No Error",
  "Unknown Error",
  "Error Opening File; may be exclusively locked",
  "File Not Found",
  "Unrecognized File Format",
  "Unexpected EOF",
  "Unknown Image Compression",
  "Invalid User Data; check for NULL pointers and bad/out of range data",
  "File Format Mode Not Supported",
  "Invalid Filename",
  "Error Reading From File",
  "Error Reading Header From File"
};

extern const char *ImageSaverErrorStrings[] = {
  "No Error",
  "Unknown Error",
  "Error Opening File; path may not exist",
  "File Locked",
  "File is Read-Only or Write Protected",
  "Disk is Full",
  "Invalid User Data; check for NULL pointers and bad/out of range data",
  "File Format Mode Not Supported",
  "Invalid Filename",
  "Invalid Image Data",
  "Invalid File Format ID",
  "Error Writing To File",
  "Error Writing Header To File",
  "Unknown Saver File Format ID Specified"
};

extern const char *ImageFileFormatStrings[] = {
  "Unknown File Format",
  "Windows Bitmap",
  "Truevison Targa",
  "CSource Image"
  // INSERT NEW FILE FORMAT HERE (IMAGEFILRFORMATSTRINGS)
};

// This is the same as ImageFileFormatStrings[], but uses shorter,
//  easier-to-type strings, which is useful for command-line image
//  conversion programs.  Both arrays are indexed the same way.
extern const char *ImageFileFormatStrings_Short[] = {
  "Unknown",
  "BMP",
  "Targa",
  "CSource"
  // INSERT NEW FILE FORMAT HERE (IMAGEFILRFORMATSTRINGS)
};


extern const char *ImageCompressionModeStrings[] = {
  "Unknown Image Compression Mode",
  "No Image Compression (Uncompressed)",
  "Run-Length Encoding (RLE)"
};
