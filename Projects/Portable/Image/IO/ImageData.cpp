//
// ImageData.cpp
//
// This file impliments the ImageData class.  It's primary function is
//  to define the Load() and Save() functions.
//

//
// IMPORTANT NOTE:  If you add any more file formats, BE SURE to add
//                  the file format enum to ImageIO_IDs.cpp,
//                  ImageIO_IDs.h, and to the Load, Save, etc.
//                  functions in ImageData.cpp. Search for the comment
//                  "INSERT NEW FILE FORMAT HERE" (no quotes) for where
//                  to insert this data.
//

#include "IO.h"
#include "portable\dirtools\dirtools.h"

// ImageData Strings
const char *ImageDataResponseStrings[] = {
  "OK; No Errors Reported",
  "Index out of range",
  "File format not recognized by any image loader",
  "Unable to open file",
  "Error reading from file",
  "Error writing to file",
  "No image found to save",
  "Bad save file format",
  "No filename provided",
  NULL
};

// Constructor
//  IMPORTANT NOTE:  If using this constructor to do a one-line load/save
//   operation, be sure to check ImageData::GetStatus() to make sure the
//   operation actually worked.
ImageData::ImageData( ImageDataIOTypes   _mode,
                      const char       * _filename,
                      ImageFileFormats   _file_format,
                      ImageClass       * _image ) {

  // Set the Image
  image            = _image;

  // Set the File Format
  if( _mode == IMGDATA_INIT )
    file_format    = IMGFF_UNKNOWN;
  else
    file_format    = _file_format;

  // Set the filename
  if( _filename == NULL )
    filename = NULL;
  else
    filename = strdup( _filename );

  // Set the other members to the defaults.
  //  NOTE:  Creator data is static, and as such is not initialized here
//  data_status      = IMGDATA_OK;
  auto_extension   = true;

  file_size        = 0;
  bits_per_pixel   = 24;
  compression      = IMGCOMPRESSION_NONE;

  dpi_x            = 72;
  dpi_y            = 72;

  pixel_aspect_num   = 1;
  pixel_aspect_denom = 1;

  gamma_num        = 1;
  gamma_denom      = 0;

  author           = NULL;
  comment          = NULL;

  creator          = NULL;
  creator_version  = 1;
  creator_revision = 0;
  creator_subrev   = ' ';

  job_name         = NULL;
  job_time[0] = job_time[1] = job_time[2] = 0;

  date_stamp[0] = date_stamp[1] = date_stamp[2] = 0;
  date_stamp[3] = date_stamp[4] = date_stamp[5] = 0;


  thumbnail        = NULL;

  file_data        = NULL;

  // Handle Load/Save mode
  if( _mode == IMGDATA_LOAD )
    Load();
  else if( _mode == IMGDATA_SAVE )
    Save();
}

// Destructor
ImageData::~ImageData() {
  // Free the allocated strings
  if( filename != NULL )
    free( filename );

  if( author != NULL )
    free( author );

  if( comment != NULL )
    free( comment );
}

// DestroyAllButImage()
//  Deletes the provided ImageData instance as well as the file_data
//   and thumbnail images, if they exists.  This does NOT delete the
//   ImageClass instance.  This function is provided as a convenience,
//   as normally the thumbnail, image and file_data members are not
//   freed on destruction.
void ImageData::DestroyAllButImage( ImageData *data ) {
  data->DestroyAllButImage();
  delete data;
}

// DestroyAllButImage()
//  This vairent is non-static, and will do everything the static
//   version does except delete the ImageData instance.  Use this
//   when you didn't new the instance.
void ImageData::DestroyAllButImage() {
  if( thumbnail != NULL ) {
    delete thumbnail;
    thumbnail = NULL;
  }
  
  if( file_data != NULL ) {
    delete file_data;
    file_data = NULL;
  }
}

// DestroyFileData():
//  Deletes the file_data member and sets it to NULL.
void ImageData::DestroyFileData() {
  if( file_data != NULL ) {
    delete file_data;
    file_data = NULL;
  }
}


// Save()
//  Save the file referenced within this class.  Returns whatever
//   the GetLastError() function of the file format loader returned.
//  If a filename is provided, it will replace the filename already
//   existing in the instance.  A new file format can also be passed in,
//   which will replace the existing file format.
int ImageData::Save( const char      * _filename,
                     ImageFileFormats  _file_format ) {
 // Check for a valid filename
  if( filename == NULL )
    return IMGSAVER_INVALID_FILENAME;

  // Make sure there is an image to save
  if( image == NULL )
    return IMGSAVER_INVALID_IMAGE;

  if( _file_format != IMGFF_UNKNOWN )
    SetFileFormat( _file_format );

  // Make sure the file format is valid
  if( (GetFileFormat() < 1) || (GetFileFormat() > IMGFF_NUM_FILE_FORMATS) ) {
    return IMGSAVER_INVALID_FILE_FORMAT;
  }

  // Auto-Add the extension and use the new filename, if applicable
  char new_filename[ MAX_PATH_LENGTH ];
  if( _filename == NULL )
    strcpy( new_filename, filename );     // Use the old name
  else
    strcpy( new_filename, _filename );    // Use the new name

  if( auto_extension ) {                  // Use the first extension, if applicable
    DirStrings::ChangeExtension( new_filename, GetExtensionList( _file_format )[0] );
    SetFilename( new_filename );
  }

  // Figure out which file format to save in
  //  NOTE:  If you add a new file format, you MUST insert the file
  //         format's information here.  This should consist of copying
  //         one of the existing cases and replacing references to the
  //         original file format to your new on.
  switch( file_format ) {
  case IMGFF_BMP:                     // BMP Image Saver
    BMP::Save( filename, this );
    return BMP::GetLastError();
    break;

  case IMGFF_TARGA:                   // Targa Image Saver
//    Targa::Save( file_name );
//    return Targa::GetLastError();
    break;

  case IMGFF_CSOURCE:                  // CSource Image Saver
    CSource::Save( filename, this );
    return CSource::GetLastError();
    break;

  // INSERT NEW FILE FORMAT HERE (SAVER)
  }

  // Unrecognized File Format; fail
  return IMGSAVER_UNRECOGNIZED_FILE_FORMAT;
}

// Load()
//  Load the file referenced within this class.  Fills in the ImageData
//   class with all pertenant data from the loaded file.  Returns
//   whatever the GetLastError() Function of the format's loader returned.
//  If a filename is provided, it will replace the filename already
//   existing in the instance.
int ImageData::Load( const char *_filename ) {
  // See if a new filename was provided
  if( _filename != NULL )
     SetFilename( _filename );

  // Check for a valid filename
  if( filename == NULL ) {
    return IMGLOADER_INVALID_FILENAME;
  }

  // Load the file with the file format that matches best
  //  NOTE:  If you add a new file format, you MUST insert the file
  //         format's information here.  This should consist of copying
  //         one of the existing cases and replacing references to the
  //         original file format to your new one.
  if( BMP::IsRecognized( filename ) != IMGIOREC_NOT_RECOGNIZED ) {
    image = BMP::Load( filename, this );       // BMP Image Loader
    return BMP::GetLastError();
  }

  if( Targa::IsRecognized( filename ) != IMGIOREC_NOT_RECOGNIZED ) {
    image = Targa::Load( filename, this );       // Targa Image Loader
    return Targa::GetLastError();
  }

  // INSERT NEW FILE FORMAT HERE (RECOGNIZE)

  // Unrecognized File Format; fail
  return IMGLOADER_UNRECOGNIZED_FILE_FORMAT;
}


// GetExtensionList()
//  Returns a list of extensions for the file format specified.  This
//   is static.  You can use this to itterate through all the file
//   formats (ie: use a for() loop, starting from 1 and counting
//   through IMGFF_NUM_FILE_FORMATS).  Returns NULL if the file format
//   ID is invalid.
const char ** ImageData::GetExtensionList( ImageFileFormats ff ) {
  switch( ff ) {
  case IMGFF_BMP:
    return BMP::GetExtensionList();
    break;

  case IMGFF_TARGA:
    return Targa::GetExtensionList();
    break;

  case IMGFF_CSOURCE:
    return CSource::GetExtensionList();

  // INSERT NEW FILE FORMAT HERE (EXTENSION LIST)

  default:
    return NULL;
  }
}


// OutputToStream()
//  Outputs an ImageData instance as ASCII text to the stream provided.
void ImageData::OutputToStream( ostream &out ) {
  out << " Filename:             \"" << GetFilename() << "\"" << endl;
  out << "  File Format:         " << GetFileFormat() << "; \"" << ImageFileFormatStrings[ GetFileFormat() ] << "\"" << endl;
  out << "  File Size:           " << GetFileSize() << " bytes " << endl;
  out << endl;
  out << " File Data:" << endl;
  out << "  Bits Per Pixel:      " << GetBitsPerPixel() << endl;
  out << "  Compression:         " << GetCompression() << "; \"" << ImageCompressionModeStrings[ GetCompression() ] << "\"" << endl;
  out << "  DPI X/Y:             " << GetDPIX() << "/" <<  GetDPIY() << endl;
  out << "  Pixel Aspect Ration: " << GetPixelAspectNum() << "/" <<  GetPixelAspectDenom() << endl;
  out << "  Gamma Correction:    " << GetGammaNum() << "/" <<  GetGammaDenom() << endl;
  out << "  Thumbnail:           ";
  if( GetThumbnail() == NULL )
    out << "No" << endl;
  else
    out << "Yes" << endl;

  out << endl;
  out << " Creator Data:" << endl;
  out << "  Author:              \"" << GetAuthor() << "\"" << endl;
  out << "  Creator Program:     \"" << GetCreator() << "\"" << endl;
  out << "  Creator Version:     "   << GetCreatorVersion() << "." << GetCreatorRevision() << GetCreatorSubRev() << endl;
  out << "  Comment:             \"" << GetComment() << "\"" << endl;
  out << "  Job Name:            \"" << GetJobName() << "\"" << endl;
  out << "  Job Time:            "   << GetJobTime()[0] << ":" << GetJobTime()[1] << ":" << GetJobTime()[2] << endl;
  out << "  Date Stamp:          "   << GetDateStamp()[0] << "/" << GetDateStamp()[1] << "/" << GetDateStamp()[2] << "  "
                                     << GetDateStamp()[3] << ":" << GetDateStamp()[4] << ":" << GetDateStamp()[5] << endl;
  out << endl;

  out << " Image Data:" << endl;
  out << "  Width:               " << GetImage()->GetWidth() << endl;
  out << "  Height:              " << GetImage()->GetHeight() << endl;
  out << "  Type:                " << GetImage()->GetType() << "; \"" << ImageTypeStrings[ GetImage()->GetType() ] << "\"" << endl;
  out << "  Num Registers:       ";
  if( GetImage()->GetType() == IMAGE_INDEXED )
    out << GetImage()->GetNumRegisters() << endl;
  else
    out << "N/A" << endl;

}

// SetString()
//  Changes string into new_string.  Used to set strings in the ImageData class.
bool ImageData::SetString( char *&string, const char *new_string ) {
  // Both strings are the same; do nothing
  if( string == new_string )
    return true;

  // Free the current string
  if( string != NULL ) {
    free( string );
    string = NULL;
  }

  // Check for an empty strings
  if( new_string == NULL )
    return true;

  if( new_string[0] == '\0' )
    return true;

  // Copy the new string
  string = strdup( new_string );
  if( string == NULL )
    return false;

  return true;
}
