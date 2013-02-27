//
// CSource.h
//
// This generates a .cpp file containing image data from an ImageClass
//  instance.  This data can later be loaded back into an ImageClass
//  and displayed.
//

#ifndef HEADER_IMAGE_CSOURCE
#define HEADER_IMAGE_CSOURCE

#include "portable\dynarray\dynarray.h"

#include "ImageData.h"

#include "../Process/Process.h"  // This must be before the LW C++ Wrappers or you'll get link errors in PanelDraw::Checkerboard()
#include <lightwave\lw_sdk_cpp_wrappers\lwwrappers_L6.h>

// CSource File Extensions
static const char *CSourceExtensionList[] = {
  ".cpp",
  ".h",
  NULL };

// CSource Compression Types
enum CSourceCompressionTypes {
  CSOURCE_UNCOMPRESSED,
  CSOURCE_RLE,
  CSOURCE_NUM_COMPRESSION_TYPES };

class CSource;
class CSourceData;

// CSource Class
class CSource : public ImageIO {
public:
  static ImageClass * Load( const char *filename, ImageData *data = NULL );     // Loads a .cpp/.h file
  static ImageClass * Load( CSourceData *cdata, ImageData *data = NULL );        // Loads from a CSourceData instance
  static bool         Save( const char *filename, ImageData *data );
  static bool         Save( const char *filename,
                            ImageClass *image,
                            CSourceCompressionTypes compress = CSOURCE_UNCOMPRESSED );

  static int  IsRecognized(          const char * filename );
  static int  IsRecognizedExtension( const char * filename );
  static int  IsRecognizedHeader(    const char * header   );

  static const char **      GetExtensionList()      { return CSourceExtensionList; }
  static const unsigned int GetFileFormatHeaderID() { return NULL; }
  static const char *       GetFileFormatHeader()   { return NULL; }
  static int                GetFileFormatID()       { return IMGFF_CSOURCE; }

  static void OutputHeaderAsText( const char *filename, void *header ) { ; }

protected:
  static bool WriteDotH( ofstream &out, const char *iname );
  static bool WriteDotC( ofstream &out, const char *iname, ImageClass *image,
                         CSourceCompressionTypes compress );
};

// CSourceData Class
//  This contains the actual image data that would be written out to a CSource file.
//   The CSource Save() function writes out a new class that inherits CSourceData, and
//   who's constructor contains sets all the apropriate information about the image.
//  Note that the data stored in this class is not the same as the same
//   data used in an ImageClass.  Be sure to use the Decode() function
//   to extract the data into an image, or one of the Draw functions to
//   display it directly.
class CSourceData {
public:
  // Constructor/Destructor
  CSourceData() : data(NULL), palette(NULL) { ; }
  ~CSourceData() { delete data; }
  
  // Accessors
  char    ** GetData()         { return data; }           // Returns a pointer to the encoded image data
  char     * GetPalette()      { return palette; }        // Returns a pointer to the image palette (if in IMAGE_INDEXED mode)

  int        GetPacketSize()   { return packet_size; }    // Returns the size of each packet
  int        GetPacketCount()  { return packet_count; }   // Retuns the number of packets

  int        GetVersion()      { return version; }        // Version of this class
  int        GetWidth()        { return width; }          // Image Width
  int        GetHeight()       { return height; }         // Image Height
  int        GetImageType()    { return image_type; }     // Image Type
  int        GetNumRegisters() { return num_registers; }  // Number of registers in the pallete, if applicable
  int        GetPixelSize()    { return pixel_size; }     // Size of a pixel in IMG_BYTES
  CSourceCompressionTypes GetCompression() { return compression; };  // Image Compression

  // Decode/Display Functions
  ImageClass * Decode();                                                           // Decodes the image and returns a pointer to a new ImageClass instance
  bool         DrawInLWPanels(  LWPanelID panel, CDrawFuncs *df, int x, int y  );  // Draws the image directly into a LWPanels GUI

protected:
  char                     ** data;                               // Array of packet_count IMG_BYTES of packet_size length each
  char                      * palette;

  int                         packet_size;                        // Size in bytes of each packet
  int                         packet_count;                       // Number of pakcets

  int                         version;
  int                         width, height;
  int                         image_type;
  int                         num_registers;
  int                         pixel_size;
  CSourceCompressionTypes     compression;
};

#endif