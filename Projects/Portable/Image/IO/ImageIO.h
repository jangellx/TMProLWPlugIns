//
// ImageIO.h
//
// This file contains the base class for all Image Classes.  Mainly, it is
//  collection of virtual functions, which are overridden by each Image I/O
//  class.  This allows a single interface to be used with all image i/o
//  classes.
//
// Note about the Recognize functions:  Certain file formats do not support
//  certain recognition modes.  For example, Targa image files will return
//  IMGIOREC_NOT_AVAILABLE when you call IsRecognizedHeader, since Targas
//  may not have any ID, and if it does it is not in the header of the file.
//  IsRecognized() will do checks on the filename and scan the file for an
//  ID, and as such is the most robust, althought it may not be the fastest.
//  IsRecognizedExtension() will check the extension only of the filename
//  passed in.  IsRecognizedHeader() will check the NULL-terminated header
//  string passed in to see if it matches.  If the header is too short, it
//  will return IMGIOREC_HEADER_TOO_SHORT.  You can check Recognize error
//  by testing if the return value is < 0.  See the ImageIORecognized enum
//  for more information.
//  

#ifndef HEADER_IMAGEIO
#define HEADER_IMAGEIO

#include "..\ImageClass.h"
#include "..\ImageIDs.h"

// Recognized Responses.
//  A value >0 means success; 0 means not recognized.
//   <0 means error.
enum ImageIORecognized {
  IMGIOREC_ERROR             = -3,    // Error testing the header ID; returned due to file I/O errors when using IsRecognized()
  IMGIOREC_NOT_AVAIlABLE     = -2,    // Not available; returned when IsRecognizedHeader() is not supported by the file format
  IMGIOREC_HEADER_TOO_SHORT  = -1,    // Returned by IsRecognizedHeader() if the Header is too short for a sucessful test
  IMGIOREC_NOT_RECOGNIZED    =  0,    // Recognition failed; file isn't recognized
  IMGIOREC_RECOGNIZED_DATA,           // Returned if the data in the file is recognized
  IMGIOREC_RECOGNIZED_HEADER,         // Returned if the header is recognized
  IMGIOREC_RECOGNIZED_EXTENSION,      // Returned if the extension is recognized
};


class ImageIO;
class ImageIOFileFormatFunctions;
class ImageData;

// ImageIO Class
//  This is the base class for all image loaders and savers.  Many functions are
//   do not have definitions, and must be overloaded by the inheriting class.
class ImageIO {
public:
  // Required load/save functions.  Feel free to add
  //  more as needed in your sub-class.  For example, a Save()
  //  function that takes an ImageClass would be useful for
  //  users that don't want to deal with an ImageData class.
  static ImageClass *Load( const char * filename, ImageData *data = NULL );  // Load an image
  static bool        Save( const char * filename, ImageData *data );  // Save an image

  // Recognizer functions, to see if the file format
  //  is know to this loader
  static int IsRecognized(          const char *filename );      // Returns true if the filename passed in is recognized by this loader (extension, header/footer scan, etc.)
  static int IsRecognizedExtension( const char *filename );      // Returns true if the extension of the filename passed in is recognized by this loader
  static int IsRecognizedHeader(    const char *header   );      // Returns true if the file header ID passed in is recognized by this loader

  // Error-Checking/Status
  static  int  GetLastError() { return last_error; }             // Returns the last error encountered

  // Common Accessor Functions
  static const char ** GetExtentionList();                       // Returns a NULL-terminated list of all known extensions for this file type
  // static const ???  GetFileFormatHeaderID();                  // Optional:  Returns a file format ID.  The return type is file-format specific, and as such is not defined here.  This is purely optional.
  static const char *  GetFileFormatHeader();                    // Returns a string containing the header ID
  static const char *  GetFileFormatName();                      // Returns a string containing the name of the file format
  static int           GetFileFormatID();                        // Returns the ImageFileFormat enum associated with this ID.

protected:
  static int last_error;
};


// ImageIOFileFormatFunctions Class
//  This function is returned by the GetFileFormatFunctiosn() call in ImageIO Class.
//   It contains pointers to the functions of a specific file format.  It is used
//   internally by ImageIO to avoid having to change a a dozen functions when you
//   add a new file format.
class ImageIOFileFormatFunctions {
public:
  ImageIOFileFormatFunctions()
    : Load(NULL), Save(NULL),
      IsRecognized(NULL), IsRecognizedExtension(NULL),
      IsRecognizedHeader(NULL),
      FillInImageData(NULL), FillInHeader(NULL),
      GetLastError(NULL),
      GetExtensionList(NULL) { ; }

  ImageClass *   (*Load)  ( const char *filename );
  bool           (*Save)  ( const char *filename, ImageData *image );

  int            (*IsRecognized)          ( const char *filename );
  int            (*IsRecognizedExtension) ( const char *filename );
  int            (*IsRecognizedHeader)    ( const char *header   );

  bool           (*FillInImageData)       ( ImageData *data );
  bool           (*FillInHeader)          ( ImageData *data );

  int            (*GetLastError)            ();

  const char  **       (*GetExtensionList)  ();
  static const char *  (*GetFileFormatName) ();
  static int           (*GetFileFormatID)   ();
};

#endif
