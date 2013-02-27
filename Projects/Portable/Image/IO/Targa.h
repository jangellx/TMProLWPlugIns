//
// Targa.h
//
// This includes the Truvision Targa Image Loader/Saver class.
//

#ifndef IMAGEIO_TARGA_HEADER
#define IMAGEIO_TARGA_HEADER

#include "portable\bfstream\bfstream.h"
#include "portable\dynarray\dynarray.h"

#include "ImageData.h"

// Targa File Extensions
static const char *TargaExtensionList[] = {
  ".TGA",
  ".VDA",
  ".ICB",
  ".VST",
  NULL
};

// Targa File Footer Signature
static const char *TargaSignature = "TRUEVISION-XFILE.";

// Targa Image Types
enum TargaImageTypes {
  TARGATYPE_UNKNOWN             = -1,
  TARGATYPE_INVALID             = -1,
  TARGATYPE_COLOR_MAPPED        =  1,
  TARGATYPE_TRUE_COLOR          =  2,
  TARGATYPE_BLACK_AND_WHITE     =  3,
  TARGATYPE_COLOR_MAPPED_RLE    =  9,
  TARGATYPE_TRUE_COLOR_RLE      = 10,
  TARGATYPE_BLACK_AND_WHITE_RLE = 11
};

// Targa RLE Packet Bit-Field
struct Targa_RLE_Packet {
  unsigned short count : 7;
  unsigned short type  : 1;
};

enum Targa_RLE_Packet_types {
  TARGA_PACKET_RAW = 0,
  TARGA_PACKET_RLE = 1
};


// Class Declarations
class Targa;
class TargaHeader;
class TargaDeveloper;

// Targa Class
class Targa : public ImageIO {
public:
  static ImageClass * Load( const char *filename, ImageData *data = NULL );
  static bool         Save( const char *filename, ImageData *data );
  static bool         Save( const char *filename,
                            ImageClass *image,
                            TargaImageTypes image_type  = TARGATYPE_INVALID,
                            bool compress         = false,
                            TargaHeader * _header = NULL );

  static int  IsRecognized(          const char * filename );
  static int  IsRecognizedExtension( const char * filename );
  static int  IsRecognizedHeader(    const char * header   );

  static const char **      GetExtensionList()      { return TargaExtensionList; }
  static const unsigned int GetFileFormatHeaderID() { return header_id; }
  static const char *       GetFileFormatHeader()   { return header_string; }
  static int                GetFileFormatID()       { return IMGFF_TARGA; }

  static OutputHeaderAsText( const char *filename,
                             TargaHeader *header );

protected:
  // Protected Memeber Functions
  static ImageClass * LoadColorMapped(    bifstream &in, TargaHeader *header );
  static ImageClass * LoadColorMappedRLE( bifstream &in, TargaHeader *header );
  static ImageClass * LoadPalette(        bifstream &in, TargaHeader *header );
  static ImageClass * LoadTrueColor(      bifstream &in, TargaHeader *header );
  static ImageClass * LoadTrueColorRLE(   bifstream &in, TargaHeader *header );
  static ImageClass * LoadGreyscale(      bifstream &in, TargaHeader *header );
  static ImageClass * LoadGreyscaleRLE(   bifstream &in, TargaHeader *header );

  static bool SaveColorMapped(     bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SaveColorMappedRLE(  bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SavePalette(         bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SaveTureColor(       bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SaveTrueColorRLE(    bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SaveGreyscale(       bofstream &out, ImageClass *image, TargaHeader *header );
  static bool SaveGreyscaleRLE(    bofstream &out, ImageClass *image, TargaHeader *header );

  // Protected Variables
  static const unsigned short header_id;
  static const char *         header_string;
};

// TargaHeader Class
//  Contains all information found in a Targa's Header
//   (well, footer; same data, anyway). This also contains
//   all non-image data, including the Developer Area and
//   Extension Area.  Used internally by the Load and Save
//   functions.
//  The destructor will delete everything in the developer
//   array; nothing should persist.
class TargaHeader {
public:
  // Enums
  enum TargaColorMapTypes {
    COLORMAP_NONE   = 0,
    COLORMAP_EXISTS = 1
  };

  enum TargaAttribTypes {
    IMGATTRIB_NO_ALPHA            = 0,   // No alpha channel present
    IMGATTRIB_IGNORE_ALPHA        = 1,   // Ignore the Alpha data; no need to read in
    IMGATTRIB_RETAIN_ALPHA        = 2,   // Retain the Alpha data, but it isn't useful
    IMGATTRIB_USEFUL_ALPHA        = 3,   // The Alpha is actually useful; keep it
    IMGATTRIB_PREMULTIPLIED_ALPHA = 4    // The Alpha is pre-multiplied into the image data
  };

public:
  TargaHeader();
  ~TargaHeader();

  bool      Load( bifstream &in  );
  streampos Save( bofstream &out ) { return 0; }
  bool      OutputAsText( const char *filename );

  ImageClass * FillWithImageClass( ImageClass *image, TargaImageTypes image_type = TARGATYPE_INVALID );
  bool         SetCompression( bool compress );
  bool         GetCompression() { return compression; }

  bool FillWithImageData( ImageData *data ) { return true; }
  bool FillInImageData(   ImageData *data );

public:
  DynArray< TargaDeveloper * > developer;  // Optional Targa Developer Array

  // File Offsets and Sizes
  unsigned long  filesize;                 // Total size of the file
  long           extension_offset;         // Offset of the Extension Data in the file
  short          extension_fixed_size;     // The size of the fixed-size area of the extension.  495 bytes is the size of the V2.0 Extension Area
  long           developer_offset;         // Offset of the Developer Directory
  streampos      color_map_offset;         // Offset to the Color Map (palette)
  streampos      image_offset;             // Offset to the Image Data

  bool           has_footer;               // True if the file has a footer (V2 or better); if there isn't one, then it's a V1 file
  bool           compression;              // True if the image_type is >8
  
  // Image ID
  char           id[255];                  // Identifier.  Up to 255 characters.
  char           id_length;                // Number of characters in ID

  // Palette (ie: Color Map) data (Field 6 in the Targa File Format Specification)
  char           color_map_type;           // Color Map Type.  See TargaColorMapTypes for more info.
  short          first_index;              // Actual index of the first palette index (ie: initial index in the file is actually the first_index in the image).  Currently only 256 registers are supported due to limitations of the image class
  short          num_indices;              // Number of entries in the color map.  Non-zero if in Indexed (ie: Psuedo-Color) mode
  char           palette_bits_per_pixel;   // Bits per pixel in the palette

  // Image Information
  char           image_type;               // Image Type
  short          x_offset;                 // X Offset of the image from the lower-left corner of the screen
  short          y_offset;                 // Y Offset of the image fromt he lower-left corner of the screen
  short          width;                    // Width of the image in pixels
  short          height;                   // Height of the image in pixels
  char           bits_per_pixel;           // Bits Per Pixel

  // Image Descriptor
  struct {  // Bit-Field.  All the items listed MUST be in this order so they can be read/written correctly!
    unsigned short attrib_bits_per_pixel  : 4; // Bits Per Pixel in the Attribute/Alpha part of the image data
    unsigned short ordering_left_to_right : 1; // Left To Right byte ordering
    unsigned short ordering_top_to_bottom : 1; // Top To Bottom ordering
    unsigned short reserved               : 2; // Unused bits; must be 0
  } descriptor;

  // Targa Extension Area Data
  //  The following data is found in the Extension Area
  //   of a Targa file.  This data is only read in if the
  //   extension_offset is 0.
  char  author_name[41];                       // Author Name
  char  author_comment[324];                   // Author Comment
  short date_stamp[6];                         // Date Stamp.  Indices 0-5 represent Month, Day, Year, Hours, Minutes and Seconds, respectively

  char  job_name[41];                          // Job Name
  short job_time[3];                           // Job Time

  char  software_id[41];                       // Software ID
  short software_version;                      // Software Version
  char  software_subrevision;                  // Software Subrevision (single letter)

  char  key_color[4];                          // Key Color, represented as ARGB
  short pixel_aspect_num;                      // Pixel Aspect Ration Numerator (width)
  short pixel_aspect_denom;                    // Pixel Aspect Ration Demonenator (height)

  short gamma_num;                              // Gamma Numerator
  short gamma_denom;                            // Gamma Denometor

  long  color_correction_offset;               // Color Correction Offset
  long  postage_stamp_offset;                  // Postage Stamp (thumbnail) Image Offset
  long  scanline_offset;                       // Scan Line Table Offset

  char  attribute_type;                        // Attribute (alpha) type

  long * scanlines;                            // Scanline Table

  char  postage_stamp_width;                   // Thumbnail Width
  char  postage_stamp_height;                  // Thumbnail Height
  ImageClass * postage_stamp;                  // Thumbnail Image

  short * color_correction;                    // Color Correction Table
};

// TargaDeveloper Class
//  Contains the data associated with a Targa's Developer Area.
//   I would recommend inheriting from this class if you want to
//   support a specific Developer Area and modifying the header
//   (a misnomer, I know) loader appropriately.  Alternately, you
//   could use the offset and size provided here, open the file
//   and read the data yourself, or you could simply parse the
//   data member directly.
//  The data member is automatically freed when the class is
//   destroyed.
class TargaDeveloper {
public:
  TargaDeveloper();                         // Constructor
  ~TargaDeveloper();                        // Destructor

public:
  short tag;                                // Tag ID
  long  offset;                             // Offset of this Developer Area in the file
  long  size;                               // Size of this Developer Area in the file

  char *data;                               // The actual data of this Developar Area.
};

#endif IMAGEIO_TARGA_HEADER
