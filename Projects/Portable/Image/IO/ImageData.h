//
// ImageData.h
//
// This file contains the ImageData class, which contains file format specific
//  image data that is accessed via a file format independant API.  Instances
//  of this class are returned by image loaders and are used by image savers.
//  It also includes a pointer to an Image class containing the pixel information.
//  The purpose of this class is to provide a single interface to the image loader/
//  saver functions, without needing to know the details of the file format.
//
// - STATIC FUNCTIONS -
// This class is designed as a file-format independant, and thus includes
//  general-purpose functions, such as GetExtensionList(), which returns a
//  list of the extensions for the file format specified.  These functions
//  allow you to learn about the file formats without having to know the
//  file format's name.
//
// - IMAGEDATA SECTIONS -
// The ImageData class contains the following "sections":
//  - A pointer to an Image class, which has been loaded or will be saved
//  - The type of image loader/saver employed/to employ
//  - File format specific data, such as DPI, thumbnail images, author, comments, etc.
//  - An optional pointer to a header for the specific file type, allowing the user
//    to input exact file-specific data into the file.   
//
// - WHAT GETS FREED -
// All data in the class is freed when it is destroyed except:
//  - The image itself
//  - The thumbnail image
//  - The file_data structure
// 
// The above must be explicitly freed by the user.  This is beacuse
//  the user may want this data at a later date (the image especially),
//  and thus it is bad to free this automatically.
// 
// For convenience, there is a static member function called
//  ImageData::DestroyAllButImage(), which destroys the provided
//  ImageData class and delete's everything except the image itself.
//
// - IMAGE I/O -
// This class is also used to load and save files.  Steps to save:
//  - Allocate an ImageData class
//  - SetImage() to the image you want to save
//  - SetFileFormat() with the enum you want to save as
//  - SetFileName() with the path to output
//  - Set the optional data members as you wish
//  - Call Save().  The file will be saved to disk.
//
// To load:
//  - Allocate an ImageData class
//  - SetFilename() with the path to input.
//  - Call Load().  The file will be loaded into the ImageData class.
//    Call accessors to get the image and other data.
//
// ImageData also features a one-line load/save constructor, where
//  you can instance and load a file in a single line.  Saving is
//  more limited due to the number of arguments that would be
//  required for all data.  For more control, use the accessors to
//  set more values.
//
// - FILE_DATA -
// Note about the file_data:  file_data is a file-format specific struct/class.
//  When you load a file through ImageData, the file_data contains the header
//  and similar information specific to that file format.  For example, if the
//  image loaded is a BMP, the file_data will be a BMPHeader class casted to a
//  void *.
//
// When you save a file through ImageData, the file_data can contain a header
//  before you call the Save() function.  The file_data is assumed to match the
//  file_format (ie: if the file_format is BMP, the file_data is a BMPHeader).
//  BE VERY CAREFUL WITH THIS; if you provide the wrong file_data, you will very
//  likely crash.
//
// If file_data is NULL when saving, the header will be created from the ImageData,
//  but file_data will remain NULL after the save operation is complete. If it is
//  non-NULL before saving, it will be modified as needed by the file-format specific
//  Save() function, and will override everthing in the ImageData.
//

//
// IMPORTANT NOTE:  If you add any more file formats, BE SURE to add
//                  the file format enum to ImageIOEnums.h,
//                  ImageIOStrings.h, and to the Load, Save and Recognize
//                  functions in ImageData.cpp
//

#ifndef IMAGEDATA_HEADER
#define IMAGEDATA_HEADER

#include <assert.h>
#include <memory.h>

#include "..\ImageClass.h"
#include "ImageIO.h"

#include "ImageIO_IDs.h"

#include "string.h"
#include "fstream.h"

// ImageData IO Types, to pass to the constructor
enum ImageDataIOTypes {
  IMGDATA_INIT = 0,
  IMGDATA_LOAD,
  IMGDATA_SAVE,

  IMGDATA_NUM_IO_TYPES
};

// ImageData Responses
//  These include all responses and error messages that ImageData
//   can return.
enum ImageDataResponses {
  IMGDATA_OK       = 0,            // No error, everything's OK
  IMGDATA_NO_ERROR = 0,            // No error, everything's OK
  IMGDATA_INDEX_OUT_OF_RANGE,      // Value passed in (enum?) is out of range
  IMGDATA_LOADER_ERROR,            // There was a loader error.  See the return value of Load() for more specific information
  IMGDATA_SAVER_ERROR,             // There was a saver error.  See the return value of Save() for more specific information
  IMGDATA_NO_IMAGE_TO_SAVE,        // Image pointer is NULL; nothing to save
  IMGDATA_BAD_FILE_FORMAT,         // File format provided for save is invalid or not a file format
  IMGDATA_NO_FILENAME,             // No filename provided

  IMGDATA_NUM_REPSONSES
};

// ImageData Strings
extern const char *ImageDataResponseStrings[];

class ImageData;

class ImageData {
public:
  ImageData( ImageDataIOTypes  _mode        = IMGDATA_INIT,  // Initialization Mode (see enums above)
             const char       *_filename    = NULL,          // Filename to load/save
             ImageFileFormats  _file_format = IMGFF_UNKNOWN, // File Format, both for loading and saving
             ImageClass       *_image       = NULL );        // Image Class; used only when saving.
  ~ImageData();

  static void DestroyAllButImage( ImageData *data );   // Deletes the provided ImageData class, plus the thumbnail and header parts of the class.  Only Image is left.
         void DestroyAllButImage();

  // Load/Save Functions
  int                   Load( const char       * _filename    = NULL );          // File to load; sets ImageData filename
  int                   Save( const char       * _filename    = NULL,            // File to save; sets ImageData filename
                              ImageFileFormats   _file_format = IMGFF_UNKNOWN ); // File format to save; sets ImageData file format.  If UNKNOWN or ANY, the format already specified in the instance is used

  // Class Options/Status
//  inline int            GetStatus() { return data_status; }

  inline bool           GetAutoExtensionState() { return auto_extension; }
  inline void           SetAutoExtensionState( bool new_ae ) { auto_extension = new_ae; }
  static const char **  GetExtensionList( ImageFileFormats ff );

  // Data Access Functions
  inline ImageClass *   GetImage() { return image; }
  inline void           SetImage( ImageClass * new_image ) { image = new_image; }
  
  inline int            GetFileFormat() { return file_format; }
  inline bool           SetFileFormat( ImageFileFormats new_ff );

  inline const char *   GetFilename() { if( filename == NULL ) return "";
                                        else return filename; }
  inline bool           SetFilename( const char *new_filename ) { return( SetString( filename, new_filename ) ); }

  inline unsigned long  GetFileSize() { return file_size; }
  inline void           SetFileSize( unsigned long new_fs ) { file_size = new_fs; }

  inline unsigned short GetBitsPerPixel() { return bits_per_pixel; }
  inline void           SetBitsPerPixel( unsigned short new_bpp ) { bits_per_pixel = new_bpp; }

  inline int            GetCompression() { return compression; }
  inline bool           SetCompression( ImageCompressionModes new_comp );

  inline unsigned long  GetDPIX() { return dpi_x; } 
  inline unsigned long  GetDPIY() { return dpi_y; } 
  inline void           SetDPI( unsigned long new_x, unsigned long new_y ) { dpi_x = new_x; dpi_y = new_y; } 
  inline void           SetDPIX( unsigned long new_x ) { dpi_x = new_x; } 
  inline void           SetDPIY( unsigned long new_y ) { dpi_y = new_y; } 

  inline unsigned short GetPixelAspectNum()   { return pixel_aspect_num;   }
  inline unsigned short GetPixelAspectDenom() { return pixel_aspect_denom; }
  inline void           SetPixelAspectNum(   unsigned short pan ) { pixel_aspect_num   = pan; }
  inline void           SetPixelAspectDenom( unsigned short pad ) { pixel_aspect_denom = pad; }
  inline void           SetPixelAspect( unsigned short pan, unsigned short pad ) {  pixel_aspect_num = pan; pixel_aspect_denom = pad; }

  inline unsigned short GetGammaNum()   { return gamma_num;   }
  inline unsigned short GetGammaDenom() { return gamma_denom; }
  inline void           SetGammaNum(   unsigned short gn ) { gamma_num   = gn; }
  inline void           SetGammaDenom( unsigned short gd ) { gamma_denom = gd; }
  inline void           SetGamma( unsigned short gn, unsigned short gd ) {  gamma_num = gn; gamma_denom = gd; }

  inline const char *   GetAuthor() { if( author == NULL ) return "";
                                      else return author; }
  inline bool           SetAuthor( const char *new_author ) { return( SetString( author, new_author ) ); }

  inline const char *   GetComment() { if( comment == NULL ) return "";
                                       else return comment; }
  inline bool           SetComment( const char *new_comment ) { return( SetString( comment, new_comment ) ); }

  inline const char *   GetCreator() { if( creator == NULL ) return "";
                                       else return creator; }
  inline bool           SetCreator( const char *new_creator ) { return( SetString( creator, new_creator ) ); }

  inline const char *   GetJobName() { if( job_name == NULL ) return "";
                                       else return job_name; }
  inline bool           SetJobName( const char *new_job_name ) { return( SetString( job_name, new_job_name ) ); }

  void                  SetJobTime( short * _job_time ) { memcpy( job_time, _job_time, 6 * sizeof(short) ); }
  inline short *        GetJobTime() { return job_time; }

  void                  SetDateStamp( short * _date_stamp ) { memcpy( date_stamp, _date_stamp, 6 * sizeof(short) ); }
  inline short *        GetDateStamp() { return date_stamp; }

  inline int            GetCreatorVersion() { return creator_version; }
  inline int            GetCreatorRevision() { return creator_revision; }
  inline char           GetCreatorSubRev() { return creator_subrev; }
  inline void           SetCreatorVersion( int new_ver ) { creator_version = new_ver; }
  inline void           SetCreatorRevision( int new_rev ) { creator_revision = new_rev; }
  inline void           SetCreatorSubRev( char new_rev );

  inline ImageClass *   GetThumbnail() { return thumbnail; }
  inline void           SetThumbnail( ImageClass * new_thumb ) { thumbnail = new_thumb; }

  inline const void *   GetFileData() { return file_data; }
  inline void           SetFileData( void * new_data ) { file_data = new_data; }
         void           DestroyFileData();

  // Debugging Functions
  void                  OutputToStream( ostream &out );

protected:
  bool ImageData::SetString( char *&string, const char *new_string );

  // Status/Option Members
//  int             data_status;            // Last message, both errors and confirmations.  See the ImageDataResponses enum for more information

  bool             auto_extension;         // Toggle to automatically add an extension to the file when saving.  Full filename, with extension, must be provided when loading

  // Image Data Members (used when loading/saving file)
  ImageClass *     image;                  // Actual image data.  This is NOT freed when the class is destroyed.

  ImageFileFormats file_format;            // File Format enum, as defined in ImageIOEnums.h
  char *           filename;               // File name loaded from/saved to

  unsigned long    file_size;              // Size of the file, as stored in the file when loading.  Otherwise, it should be 0.

  unsigned short   bits_per_pixel;         // Number of bits per pixel (1, 2, 4, 8, 15, 16, 24, 32, etc.) as storeed in the file
  
  int              compression;            // Compression Mode enum, as defined in ImageIOEnums.h

  unsigned long    dpi_x;                  // DPI on the x, as stored in file
  unsigned long    dpi_y;                  // DPI on the y, as stored in file

  unsigned short   pixel_aspect_num;       // Pixel Aspect Ratio Numerator (x)
  unsigned short   pixel_aspect_denom;     // Pixel Aspect Ratio Denominator (y)

  unsigned short   gamma_num;              // Gamma Correction Ration Numerator (x)
  unsigned short   gamma_denom;            // Gamma Correction Ration Denominator (y)
  
  char *           author;                 // Author string; NULL if there is no author string
  char *           comment;                // Comment string; NULL if there is no comment string

  char *           creator;                // Program Creation string (program that created it); NULL if there is no creator string.
  int              creator_version;        // Version of the Creator Program
  int              creator_revision;       // Revision of the Creator Program
  char             creator_subrev;         // Sub-Revision (single letter) of the Creator Program.

  char *           job_name;               // Job Name; NULL if there is no creator string.
  short            job_time[3];            // Job Time

  short            date_stamp[6];          // Date Stamp.  Indices 0-5 represent Month, Day, Year, Hours, Minutes and Seconds, respectively
  
  ImageClass *     thumbnail;              // Thumbnail image; NULL if there is no thumbnail.  This is NOT freed when the class is destroyed.

  void       *     file_data;              // File Format-specific header/footer data, as found in the file.  Use this to control specific attributes of the file format.  Can be NULL when used for an Image Saver.  This is NOT freed when the class is destroyed, and should NOT be changed to/from NULL by an Image Saver
};


// SetFileFormat()
//  Set the file format.  Automatically bounds checks for illegal values.
//   See ImageIOEnums.h for more information.
inline bool ImageData::SetFileFormat( ImageFileFormats new_ff ) {
  if( (new_ff <= IMGFF_NUM_FILE_FORMATS) && (new_ff >= 0) ) {
    file_format = new_ff;
    return true;
  } else
    return false;
}

// SetCompression()
//  Set the compression mode.  Automatically bounds checks for illegal values.
//   See ImageIOEnums.h for more information.
inline bool ImageData::SetCompression( ImageCompressionModes new_comp ) {
  if( (new_comp <= IMGCOMPRESSION_NUM_MODES) && (new_comp >= 0) ) {
    compression = new_comp;
    return true;
  } else
    return false;
}

// SetCreatorSubRev()
//  Set the creator sub revision.  This must be a letter (upper or lower), or
//   space for no value.  If an illegal value is passed in, it defaults to 
inline void ImageData::SetCreatorSubRev( char new_rev ) {
  if( ((new_rev > 64) && (new_rev < 91)) ||
      ((new_rev > 97) && (new_rev < 123)) )
    creator_subrev = new_rev;
  else
    new_rev = ' ';
}

#endif