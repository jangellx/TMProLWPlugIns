//
//  PiEToolsLogo.h
//
//  Auto-Generated CSource Image Data Header
//
//  This file should be included in the source file that will
//   "load" the image.
//

#ifndef MOTIFY_PIE_TOOLS_LOGO_HEADER
#define MOTIFY_PIE_TOOLS_LOGO_HEADER

#include "motify-fromdynarray.h"

#include <fstream.h>

#include <lwpanel.h>

typedef unsigned char IMG_BYTE;         // Used to represent a single color channel

class ImageClass {
public:
  ImageClass( int _w, int _h, int _type, unsigned int _num_registers = 0 );
  ~ImageClass();

  // General Accessors
  inline int  GetType()       { return type;   }     // Get the image type
  inline int  GetWidth()      { return width;  }     // Get the image width
  inline int  GetHeight()     { return height; }     // Get the image height

  // Image Buffer Accessors
  inline IMG_BYTE * GetBuffer() { return image; } // Return a pointer to the image buffer.  Use GetIndexAt() to get a pixel at a coordinate in the buffer.

  // Buffer-Level Access
  inline int GetPixelSize() { return pixel_size; } // Returns the size of a pixel in IMG_BYTEs.  Use to index into the image array or a line in the image

  // Clear Functions
  void ClearRGB(     IMG_BYTE r, IMG_BYTE g, IMG_BYTE b );              // Clear the buffer to the given color.  Won't affect the alpha

protected:
  int type;                   // Image Type

  // Image Size
  int width;
  int height;

  int pixel_size;             // Number of elements in a pixel array (RGBA == 4; RGB == 3; Indexed == 1)

  // Palette Colors
  unsigned int num_registers; // Number of palette registers
  IMG_BYTE *   palette;       // Palette Colors Array
  
  // Image Data
  IMG_BYTE * image;           // A 2-dimensional array (x and y) of palette indices, grey values or RGB/RGBA arrays

};

// ImageData IO Types, to pass to the constructor
enum ImageDataIOTypes {
  IMGDATA_INIT = 0,
  IMGDATA_LOAD,
  IMGDATA_SAVE,

  IMGDATA_NUM_IO_TYPES
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

// ---
// ------------------------------------- Inline Functions ---
// ---

// CSource Compression Types
enum CSourceCompressionTypes {
  CSOURCE_UNCOMPRESSED,
  CSOURCE_RLE,
  CSOURCE_NUM_COMPRESSION_TYPES };

class CSourceData;

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
  bool         DrawInLWPanels(  LWPanelID panel, DrawFuncs *df, int x, int y  );  // Draws the image directly into a LWPanels GUI

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

// PiEToolsLogo Class
class PiEToolsLogo : public CSourceData {
public:
  // Constructor/Destructor
  PiEToolsLogo();

};

#endif