//
// BMP.h
//
// The BMP class deals with loading and saving BMP images/
// It can only handle 8-bit images right now, but should be
// able to handle 24-bit and 32-bit (24-bit color and 8-bit
// alpha) images in the future.
//
// All functions in this class are static, so there
// is no need to every instance this class.
//

#ifndef IMAGEIO_BMP_HEADER
#define IMAGEIO_BMP_HEADER

#include "portable\bfstream\bfstream.h"

#include "ImageData.h"

// BMP File Extension(s)
static const char *BMPExtensionList[] = {
  ".BMP",
  NULL
};

// BMP Bit Types
enum BMPBitTypes {
  BMP_INVALID_BIT_TYPE = 0,
  BMP_1_BIT  = 1,
  BMP_4_BIT  = 4,
  BMP_8_BIT  = 8,
  BMP_16_BIT = 16,
  BMP_24_BIT = 24,
  BMP_32_BIT = 32,

  NUM_BMP_BIT_TYPES = 6
};

// BMP Compression Types
enum BMPCompressionTypes {
  BMP_NONE      = 0,      // No Compression
  BMP_RGB       = 0,      // No Compression
  BMP_RLE8      = 1,      // 8-Bit Run-Length Encoding
  BMP_RLE4      = 2,      // 4-Bit Run-Length Encoding
  BMP_BITFIELDS = 3,      // No Compression; the "Palette" is an alpha channel?

  NUM_BMP_COMPRESSION_TYPES
};

class BMP;
class BMPHeader;

// BMP Class
class BMP : public ImageIO {
public:
  static ImageClass * Load( const char *filename, ImageData *data = NULL );
  static bool         Save( const char *filename, ImageData *data );
  static bool         Save( const char *filename,
                            ImageClass *image,
                            BMPBitTypes bit_type = BMP_INVALID_BIT_TYPE,
                            bool compress        = BMP_NONE,
                            BMPHeader  * _header = NULL );

  static int  IsRecognized(          const char * filename );
  static int  IsRecognizedExtension( const char * filename );
  static int  IsRecognizedHeader(    const char *header    );

  static const char **      GetExtensionList()      { return BMPExtensionList; }
  static const unsigned int GetFileFormatHeaderID() { return header_id; }
  static const char *       GetFileFormatHeader()   { return header_string; }
  static int                GetFileFormatID()       { return IMGFF_BMP; }

  static OutputHeaderAsText( const char *filename,
                             BMPHeader *header );

protected:
  // Protected Memeber Functions
  static ImageClass * Load1Bit(  bifstream &in, BMPHeader *header );
  static ImageClass * Load4Bit(  bifstream &in, BMPHeader *header );
  static ImageClass * Load8Bit(  bifstream &in, BMPHeader *header );
  static ImageClass * Load16Bit( bifstream &in, BMPHeader *header );
  static ImageClass * Load24Bit( bifstream &in, BMPHeader *header );
  static ImageClass * Load32Bit( bifstream &in, BMPHeader *header );

  static bool Save1Bit(  bofstream &out, ImageClass *image, BMPHeader *header );
  static bool Save4Bit(  bofstream &out, ImageClass *image, BMPHeader *header );
  static bool Save8Bit(  bofstream &out, ImageClass *image, BMPHeader *header );
  static bool Save16Bit( bofstream &out, ImageClass *image, BMPHeader *header );
  static bool Save24Bit( bofstream &out, ImageClass *image, BMPHeader *header );
  static bool Save32Bit( bofstream &out, ImageClass *image, BMPHeader *header );

  // Protected Variables
  static const unsigned short header_id;
  static const char *         header_string;
};

// BMPHeader Class
//  Contains all information found in a BMP's Header.
//  Used internally by the Load and Save functions.
//  Note that this header does not include the "BM"
//  type identifier.
class BMPHeader {
public:
  BMPHeader();

  bool      Load( bifstream &in  );
  streampos Save( bofstream &out );
  bool      OutputAsText( const char *filename );

  ImageClass * FillWithImageClass( ImageClass *image, BMPBitTypes bit_type = BMP_INVALID_BIT_TYPE );
  bool         SetCompression( bool compress );

  bool FillWithImageData( ImageData *data );
  bool FillInImageData(   ImageData *data );

public:
  unsigned long  filesize;             // Total size of the file
  unsigned short reserved_1;           // Reserved; must be 0
  unsigned short reserved_2;           // Reserved; must be 0
  unsigned long  offset;               // Offset in the file to the image data.  This includes the size of the palette!

  unsigned long  header_size;          // Size of the header (everything after the offset but before the image data)
  long           width;                // Width of the image in pixels
  long           height;               // Height of the image in pixels
  unsigned short planes;               // Number of bit-planes in the output device.  Must be 1
  unsigned short bits_per_pixel;       // Bits Per Pixel
  unsigned long  compression;          // Compression Mode
  unsigned long  bytes_in_image;       // Number of bytes in the image.  May be 0 if in uncompressed, true-color mode
  long           x_pels;               // Device-specific resolution
  long           y_pels;               // Device-specific resolution
  long           colors_used;          // Number of colors used in a <= 8-bit image (ie:  10 colors in a 4-bit (16-color) image)
  long           colors_important;     // Number of "important" colors in the palette
};

#endif