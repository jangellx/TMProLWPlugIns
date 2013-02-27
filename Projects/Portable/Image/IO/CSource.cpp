//
// CSource.cpp
//

#include <math.h>
#include <assert.h>
#include <iomanip.h>

#include <portable/DirTools/DirTools.h>

#include "CSource.h"

#define CSOURCE_PACKET_SIZE 8192

// Save()
//  This varient of the Save() function wraps around the other Save()
//   function, adding support for ImageData classes.  This function
//   is called by the ImageData::Save() function when saving CSources.
//  If a header is provided in the ImageData instance, then it will be
//   used for for saving, and the ImageData informatino will be discarded.
//   It is VERY IMPORTANT that a CSourceHeader be supplied; otherwise, the
//   saver will crash (it will think it is a CSourceHeader).  If you don't want
//   to supply a header, just leave it at NULL.
bool CSource::Save( const char * filename, ImageData *data ) {
  // Test for a non-existant ImageData class
  if( data == NULL ) {
    CSource::last_error = IMGSAVER_INVALID_USER_DATA;
    return false;
  }

  if( data->GetCompression() ) {
    return CSource::Save( filename, data->GetImage(), CSOURCE_RLE );
  } else {
    return CSource::Save( filename, data->GetImage(), CSOURCE_UNCOMPRESSED );
  }
}

// Save()
//  Saves a CSource image as the given filename with the given image data.
//   If a bit type is specified, then the image will be saved in that
//   CSource bits-per-pixel mode; otherwise, the saver will try to guess
//   which mode to save in based on the image data provided.  If a
//   header is provide, the bit type/compression is ignored and the
//   header's data will be used instead.
//  Note that the header will be modified to match the image width,
//   height and number of registers, if needed.
//
//  Here's how a CSource is saved:
//   The filename passed in will result in two files being generated,
//    a filename.h and a filename.cpp.  These will contain a
//    CSouce_filename class that inherits from CSourceData.  Note that
//    only the file part with no extension will be used for this name.
bool CSource::Save( const char *filename, ImageClass *image,
                    CSourceCompressionTypes compress ) {
  CSource::last_error = IMGSAVER_NO_ERROR;

  // Get the file part of the filename.
  char iname[ MAX_PATH_LENGTH ];  // Instance Name
  char hname[ MAX_PATH_LENGTH ];  // .h Filename
  char cname[ MAX_PATH_LENGTH ];  // .cpp Filename

  char * temp = DirStrings::HasFile( filename );
  if( !temp )
    return false;

  strcpy( iname, temp );
  DirStrings::ChangeExtension( iname, "" );

  strcpy( hname, filename );
  DirStrings::ChangeExtension( hname, ".h" );

  strcpy( cname, filename );
  DirStrings::ChangeExtension( cname, ".cpp" );

  // Open and test the output files
  ofstream outh( hname );
  if( !outh ) {
    CSource::last_error = IMGSAVER_ERROR_OPENING_FILE;
    return false;
  }

  ofstream outc( cname );
  if( !outc ) {
    CSource::last_error = IMGSAVER_ERROR_OPENING_FILE;
    return false;
  }

  // Write the output files
  bool retval = WriteDotH( outh, iname );
  if( retval )
    retval = WriteDotC( outc, iname, image, compress );
 
  // Close the output files and exit
  outh.close();
  outc.close();
  return retval;
}

// WriteDotH();
bool CSource::WriteDotH( ofstream &out, const char *iname ) {
  // Write some comments for the user
  out << "//" << endl;
  out << "//  " << iname << ".h" << endl;
  out << "//" << endl;
  out << "//  Auto-Generated CSource Image Data Header" << endl;
  out << "//" << endl;
  out << "//  This file should be included in the source file that will" << endl;
  out << "//   \"load\" the image." << endl;
  out << "//" << endl;
  
  // Write out the header files required
  out << endl;
  out << "#include <portable/image/image.h>" << endl;
  out << endl;

  // Write out the class declaration
  out << "// " << iname << " Class" << endl;
  out << "class " << iname << " : public CSourceData {" << endl;
  out << "public:" << endl;
  out << "  // Constructor/Destructor" << endl;
  out << "  " << iname << "();" << endl;
//  out << "  ~" << iname << "();" << endl;   // Not currently used
  out << endl;

  out << "};" << endl;
  out << endl;
  out << endl;

  if( !out )
    return false;
  else 
    return true;
}

// WriteDotC();
bool CSource::WriteDotC( ofstream &out, const char *iname, ImageClass *image,
                         CSourceCompressionTypes compress ) {

  // Figure out how many 8K chunks we need
  long image_size = image->GetWidth() * image->GetHeight() * image->GetPixelSize() * sizeof(IMG_BYTE);
  int  packet_count = image_size/CSOURCE_PACKET_SIZE;
  if( image_size % CSOURCE_PACKET_SIZE != 0 )
    packet_count++;

  // Write some comments for the user
  out << "//" << endl;
  out << "//  " << iname << ".cpp" << endl;
  out << "//" << endl;
  out << "//  Auto-Generated CSource Image Data Source" << endl;
  out << "//" << endl;
  out << "//  This file should be linked with the program that will" << endl;
  out << "//   use the image." << endl;
  out << "//" << endl;

  // Write out the header files required
  out << endl;
  out << "#include \"" << iname << ".h\"" << endl;
  out << endl;

  out << "// Constructor" << endl;
  out << iname << "::" << iname << "() {" << endl;
  out << "  version       = 1;" << endl;
  out << "  width         = " << image->GetWidth()        << ";" << endl;
  out << "  height        = " << image->GetHeight()       << ";" << endl;
  out << "  image_type    = " << image->GetType()         << ";" << endl;
  out << "  num_registers = " << image->GetNumRegisters() << ";" << endl;
  out << "  pixel_size    = " << image->GetPixelSize()    << ";" << endl;
  out << "  packet_size   = " << CSOURCE_PACKET_SIZE      << ";" << endl;
  out << "  packet_count  = " << packet_count             << ";" << endl;

  if( compress == CSOURCE_RLE )
    out << "  compression = CSOURCE_RLE;" << endl;
  else
    out << "  compression = CSOURCE_UNCOMPRESSED;" << endl;

  // Write out the palette, if applicable
  out << setfill('0');
  if( image->GetType() == IMAGE_INDEXED ) {
    out << "  // Palette Data (This could be long and messy...)" << endl;
    out << "  palette = \"";

    for( unsigned int i = 0; i < image->GetNumRegisters() * 3; i++ ) {
      if( (i % 99) == 0 ) {
        out << "\"     // Index " << dec << i/3 << endl;
        out << "         \"";
      }
      out << hex << "\\x" << hex << (int)(image->GetPalette()[i]);
    }

    if( (i-1) % 99 != 0 )
      out << "\"";
    out << ";         // Index " << dec << i/3 << endl;
  }

  // Allocate the data array
  out << endl;
  out << "  // Image Data (This could be long and messy...)" << endl;
  out << "  data = new char *[" << dec << packet_count << "];" << endl;

  // Write out the image data.  This won't look pretty in the source file...
  int j = 0;
  for( long i = 0; i < image->GetWidth() * image->GetHeight() * image->GetPixelSize(); i++ ) {
    if( i % CSOURCE_PACKET_SIZE == 0 ) {
      if( i != 0 ) {
        out << "\"";
        out << ";         // Index " << dec << i/image->GetPixelSize() << endl;
      }

      out << "  data[" << dec << j++ << "] = \"";
    }

    if( ((i+1) % 127) == 0 ) {
      out << "\"     // Index " << dec << (i/image->GetPixelSize()) << endl;
      out << "            \"";
    }

    out << "\\x" << hex << (int)(image->GetBuffer()[i]);

  }

  out << "\"";
  out << ";         // Index " << dec << i/image->GetPixelSize() << endl;

  out << "}" << endl;
  out << endl;

  if( !out )
    return false;
  else 
    return true;
}

// ---
// ---------------- CSource Data Member Functions
// ---

// Decode()
//  Decodes the image data into an ImageClass.  It is
//   up to the calling function to delete the new ImageClass.
//   Returns NULL on an error
//  Note that right now this code relies on an IMG_BYTE being
//   and unsigned char.  Be sure to change this in the future
//   if the type changes.
ImageClass * CSourceData::Decode() {
  if( version != 1 )
    return false;

  ImageClass *image = new ImageClass( width, height, image_type, num_registers );
  if( image == NULL )
    return false;

  // Fill in the palette
  if( image_type == IMAGE_INDEXED )
    image->CopyPaletteFrom( (IMG_BYTE *)palette );

  // Fill in the image
  int pixels_in_packet;
  int image_size = width * height * image->GetPixelSize() * sizeof(IMG_BYTE);

  for( int i=0; i < packet_count; i++ ) {
    pixels_in_packet = image_size - (i * packet_size);
    if( pixels_in_packet > packet_size )
      pixels_in_packet = packet_size;

    memcpy( image->GetBuffer() + (i * packet_size), data[i], pixels_in_packet );
  }

  return image;
}

// DrawInLWPanels():
//  Draws the image data directly into a LWPanels GUI.
//  Note that RGBA images currently use the Alpha as a clip mask.
//   Only pixels that do not have an Alpha of 0 are drawn.  No
//   special transparency effects are currently performed.
bool CSourceData::DrawInLWPanels( LWPanelID panel, CDrawFuncs *df, int x, int y ) {
  long pixel_count = width * height;
  long image_size  = pixel_count * pixel_size;

  int dx, dy, i;
  int packet = 0, index = 0;
  IMG_BYTE rgba[4], * pal_color;

  switch( image_type ) {
  case IMAGE_INDEXED:
    for( dy = 0; dy < height; dy++ ) {
      for( dx = 0; dx < width; dx++ ) {
        // Get a pixel's worth of data
        rgba[0] = data[ packet ][ index++ ];

        if( index == packet_size ) {
          index = 0;
          packet++;
        }

        // Draw the pixel
        pal_color = (unsigned char *)&(palette[ rgba[0] ]);
        df->DrawRGBPixel( panel, pal_color[0], pal_color[1], pal_color[2], x+dx, y+dy );
      }
    }
    return false;
    break;

  case IMAGE_RGB:
    for( dy = 0; dy < height; dy++ ) {
      for( dx = 0; dx < width; dx++ ) {
        // Get a pixel's worth of data
        for( i=0; i < 3; i++ ) {
          rgba[i] = data[ packet ][ index++ ];

          if( index == packet_size ) {
            index = 0;
            packet++;
          }
        }
        // Draw the pixel
        df->DrawRGBPixel( panel, rgba[0], rgba[1], rgba[2], x+dx, y+dy );
      }
    }
    return true;
    break;

  case IMAGE_RGBA:
    for( dy = 0; dy < height; dy++ ) {
      for( dx = 0; dx < width; dx++ ) {
        // Get a pixel's worth of data
        for( i=0; i < 4; i++ ) {
          rgba[i] = data[ packet ][ index++ ];

          if( index == packet_size ) {
            index = 0;
            packet++;
          }
        }
        // Draw the pixel if the Alpha is non-zero
        if( rgba[3] > 0 )
          df->DrawRGBPixel( panel, rgba[0], rgba[1], rgba[2], x+dx, y+dy );
      }
    }
    return false;
    break;

  case IMAGE_GREY:
    for( dy = 0; dy < height; dy++ ) {
      for( dx = 0; dx < width; dx++ ) {
        // Get a pixel's worth of data
        rgba[0] = data[ packet ][ index++ ];

        if( index == packet_size ) {
          index = 0;
          packet++;
        }
        // Draw the pixel as greyscale
        df->DrawRGBPixel( panel, rgba[0], rgba[0], rgba[0], x+dx, y+dy );
      }
    }
    return false;
    break;

  default:
    return false;
    break;
  }
  
  // This line should never be reached
  return false;
}
