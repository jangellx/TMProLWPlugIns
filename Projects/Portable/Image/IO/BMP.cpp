//
// BMP.cpp
//

#include <math.h>
#include <assert.h>

#include "BMP.h"
#include "BMP_RLE8.h"

#include <portable/DirTools/DirTools.h>

//#define DEBUG_BMP_RLE8_SAVER         // Add this for BMP 8-bit debugging stuff

// Initialize Static Members
const unsigned short BMP::header_id     = *((unsigned short *)"BM");
const char *         BMP::header_string = "BM";

// Load()
//  Load a BMP image.  Returns NULL if the file couldn't be
//   loaded.   If an ImageData class is passed in, it will be
//   filled with information from the loaded file's header.
ImageClass *BMP::Load( const char * filename, ImageData *data ) {
  BMP::last_error = IMGLOADER_NO_ERROR;

  // See if the BMP exists
  if( !DirInfo::Exists( filename ) ) {
    BMP::last_error = IMGLOADER_FILE_NOT_FOUND;
    return NULL;
  }
  
  // Open and test the input file
  bifstream in( filename, ios::nocreate | ios::binary | ios::in );
  if( !in ) {
    BMP::last_error = IMGLOADER_ERROR_OPENING_FILE;
    return NULL;
  }

  in.SetEndian( ENDIAN_INTEL );
  
  // Read in and test the Header ID (should be "BM")
  unsigned short test_header_id = 0;
  in >> test_header_id;
  if( test_header_id != header_id ) {
    BMP::last_error = IMGLOADER_UNRECOGNIZED_FILE_FORMAT;
    return NULL;
  }
  
  // Read in the BMP header information.  Do byte-order-swapping
  //  on non-Intel machines here.
  BMPHeader * header = new BMPHeader;
  if( !header->Load( in ) ) {
    BMP::last_error = IMGLOADER_ERROR_READING_HEADER;
    return NULL;
  }
  

#ifdef DEBUG
  // Output the header as ASCII, for testing.
  char out_name[256] = "";
  strcpy( out_name, filename );
  DirStrings::ChangeExtension( out_name, ".txt" );
  DirStrings::ChangeDirectory( out_name, "f:\\test\\" );

  header->OutputAsText( out_name );
#endif

  // Load the Palette and Image Data
  ImageClass *image = NULL;

  switch( header->bits_per_pixel ) {
  case BMP_1_BIT:
    image = Load1Bit( in, header );
    break;
  case BMP_4_BIT:
    image = Load4Bit( in, header );
    break;
  case BMP_8_BIT:
    image = Load8Bit( in, header );
    break;
  case BMP_16_BIT:
    image = Load16Bit( in, header );
    break;
  case BMP_24_BIT:
    image = Load24Bit( in, header );
    break;
  case BMP_32_BIT:
    image = Load32Bit( in, header );
    break;
  }

  if( image == NULL )
    return NULL;

  // Fill in the ImageData class
  if( data != NULL ) {
    header->FillInImageData( data );
    data->SetFilename( filename );
    data->SetImage( image );
  }

  return image;
}

// Load1Bit()
//  Loads the palette and image data of a 1-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the palette data.
ImageClass * BMP::Load1Bit( bifstream &in, BMPHeader *header ) {
  BMP::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Load4Bit()
//  Loads the palette and image data of a 4-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the palette data.
ImageClass * BMP::Load4Bit( bifstream &in, BMPHeader *header ) {
  BMP::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Load8Bit()
//  Loads the palette and image data of an 8-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the palette data.
ImageClass * BMP::Load8Bit( bifstream &in, BMPHeader *header ) {
  // Create the new image class
  ImageClass *image = NULL;
  if( header->colors_used == 0 ) {
    image = new ImageClass( header->width, header->height,
                            IMAGE_INDEXED, 256 );
  } else  {
    image = new ImageClass( header->width, header->height,
                            IMAGE_INDEXED, header->colors_used );
  }

  // See if the image was allocated correctly
  if( image == NULL )
    return NULL;

  if( !(*image) )
    return NULL;

  // Load the palette
  IMG_BYTE color[4];                                // Index 4 is stored in the file, but isn't used for anything
  for( unsigned int i=0; i < image->GetNumRegisters(); i++ ) {
    in.read( color, 4 );
    StringTools::Reverse( (char *)&color, 3 );      // Swap R and B, since BMPs store these backward (both Intel and Motorola)
    image->SetPaletteColor( i, color );
  }

  // See if we do top-to-bottom (neg. height) or bottom-to-top.
  int height = header->height;
  if( height < 0 )
    height = abs( height );
  else
    image->SetZeroZero( IMAGE_BOTTOMLEFT );

  // Load the image data (Uncompressed)
  if( header->compression == BMP_NONE ) {
    IMG_BYTE *line;
    int y;
    int pad = header->width % 2;
    char padding;
    for( y=0; y < height; y++ ) {
      line = image->GetLine( y );
      in.read( line, header->width );  // Read in the whole line

      if( pad > 0 )                    // Read in any pad bytes
        in >> padding;

      if( !in && !(in.eof()) ) {
        delete image;
        return NULL;
      }
    }
  } else {
  // Load the image data (RLE)
    streampos image_start = in.tellg();
    unsigned char b0, b1, ox, oy;
    IMG_BYTE index;
    int i;
    long pixel_count;
    bool just_got_eol = true;

    do {
      in >> b0;                     // Read in the mode

      // Encoded Mode
      if( b0 > 0 ) {
        just_got_eol = false;
        in >> index;                    // Read in the index
        for( i=0; i < b0; i++ )
          image->SetPaletteIndexAt( pixel_count++, index );
      } else {

        // Absolute Mode or Escape Sequence
        if( b0 == 0 ) {
          in >> b1;
          if( b1 == 1 )  // End of Data test
            break;

          switch( b1 ) {
          case 0:        // End Of Line
            if( just_got_eol )
              pixel_count += header->width;
            else {
              if( pixel_count % header->width > 0 )
                pixel_count += header->width - ( pixel_count % header->width );   // Simple skip to end of line
            }
             
            just_got_eol = true;
            break;
          case 1:        // End Of Data (checked above)
            just_got_eol = false;
            break;
          case 2:        // Skip to X, Y
            // --- NOT TESTED YET!!! ---
            in >> ox >> oy;
            pixel_count += (oy * header->height) + ox;

            just_got_eol = false;
            break;
          default:       // Absolute Mode
            just_got_eol = false;
            for( i=0; i < b1; i++ ) {
              in >> index;     // Read in a new index
              image->SetPaletteIndexAt( pixel_count++, index );
            }
            
            // Read in the pad byte, if applicable (absoulte mode is word-aligned)
            if( b1 % 2 > 0 )
              in >> index;
          }
        }
      }

      if( !in && !(in.eof()) ) {
        delete image;
        return NULL;
      }

    // See if we should stop
    if( header->bytes_in_image > 0 ) {
      if( image_start - in.tellg() < (long)header->bytes_in_image )
        break;
    }

    // Make sure we didn't skip past the end of the file
//    assert( pixel_count <= header->width * header->height );

    } while( !in.eof() );  // Stop on an EOF, too
    cerr << " BMP: Pixel Count: " << pixel_count << endl;
  }

  image->SetZeroZero( IMAGE_TOPLEFT );
  return image;
}

// Load16Bit()
//  Loads the image data of a 24-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the image data.
ImageClass * BMP::Load16Bit( bifstream &in, BMPHeader *header ) {
  BMP::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Load24Bit()
//  Loads the image data of a 24-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the image data.
ImageClass * BMP::Load24Bit( bifstream &in, BMPHeader *header ) {
  // Create the new image class
  ImageClass *image = NULL;
  image = new ImageClass( header->width, header->height,
                          IMAGE_RGB );

  // See if the image was allocated correctly
  if( image == NULL )
    return NULL;

  if( !(*image) )
    return NULL;

  // Zero-Zero Setup
  int old_zero = image->GetZeroZero();

  int height = header->height;
  if( height < 0 )
    height = abs( height );
  else
    image->SetZeroZero( IMAGE_BOTTOMLEFT );

  // Pre-Read Variable Setup
  int  pad = (header->width * 3) % 2;
  char padding;

  IMG_BYTE rgb[3];
  int x, y;

  // Read in the RGB data
  for( y = 0; y < header->height; y++ ) {
    for( x = 0; x < header->width; x++ )  {
      in >> rgb[2] >> rgb[1] >> rgb[0];
      image->SetRGBAt( x, y, rgb );
    }

    if( pad > 0 )                      // Read in the pad bytes, if needed
      in >> padding;
  }

  // Restore previous settings
  image->SetZeroZero( old_zero );

  return image;
}

// Load32Bit()
//  Loads the image data of a 24-bit BMP.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the image data.
ImageClass * BMP::Load32Bit( bifstream &in, BMPHeader *header ) {
  BMP::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Save()
//  This varient of the Save() function wraps around the other Save()
//   function, adding support for ImageData classes.  This function
//   is called by the ImageData::Save() function when saving BMPs.
//  If a header is provided in the ImageData instance, then it will be
//   used for for saving, and the ImageData informatino will be discarded.
//   It is VERY IMPORTANT that a BMPHeader be supplied; otherwise, the
//   saver will crash (it will think it is a BMPHeader).  If you don't want
//   to supply a header, just leave it at NULL.
bool BMP::Save( const char * filename, ImageData *data ) {
  // Test for a non-existant ImageData class
  if( data == NULL ) {
    BMP::last_error = IMGSAVER_INVALID_USER_DATA;
    return false;
  }

  // See if the user provided a header
  BMPHeader * header = (BMPHeader *)data->GetFileData();

  int header_existed = true;
  if( header == NULL ) { // no header exists; generate one.
    header_existed = false;
    header = new BMPHeader;
    header->FillWithImageData( data );
  }
    
  bool ret_val = BMP::Save( filename, data->GetImage(),
                            BMP_INVALID_BIT_TYPE,           // These two really don't matter, as they'll just
                            false,                          //  get overridden by the header data, anyway...
                            header );
  
  // Delete the header if we created it
  if( header_existed == false )
    delete header;

  return ret_val;
}

// Save()
//  Saves a BMP file as the given filename with the given image data.
//   If a bit type is specified, then the image will be saved in that
//   BMP bits-per-pixel mode; otherwise, the saver will try to guess
//   which mode to save in based on the image data provided.  If a
//   header is provide, the bit type/compression is ignored and the
//   header's data will be used instead.
//  Note that the header will be modified to match the image width,
//   height and number of registers, if needed.
bool BMP::Save( const char *filename, ImageClass *image,
                BMPBitTypes bit_type, bool compress,
                BMPHeader * _header ) {
  BMP::last_error = IMGSAVER_NO_ERROR;

  // Open and test the output file
  bofstream out( filename, ios::binary | ios::out );
  if( !out ) {
    BMP::last_error = IMGSAVER_ERROR_OPENING_FILE;
    return false;
  }

  out.SetEndian( ENDIAN_INTEL );
  
  // Write out the Header ID
  out << 'B' << 'M';
  
  // Create and fill in the  BMP header information.  This may
  //  include creating a new image class, if needed.
  // Note that certain aspects of a provided header are
  //  overwritten with data from the image, including the image
  //  resolution and number of registers, as needed.
  BMPHeader *header = _header;
  if( header == NULL ) {                      // No header provided; generate one
    header = new BMPHeader;
    header->SetCompression( compress );
  } else {                                    // Header provided; override bit_type
    bit_type = (BMPBitTypes)header->bits_per_pixel;
  }

  ImageClass * out_image = header->FillWithImageClass( image, bit_type );
  if( out_image == NULL )
    return false;

  //if( image == out_image )  // Figure out which image we're using
  //  out_image = image;      //  Obsolete?

  bool retval = false;
  
  // Write the BMP header information.  Do byte-order-swapping
  //  on non-Intel machines here.
  streampos image_size_pos = header->Save( out );
  if( image_size_pos != 0 ) {                      // Header Saving returns 0 in the event of an error
    streampos image_start    = out.tellp();

  #ifdef DEUBG
    // Output the header as ASCII, for testing.
    char out_name[256] = "";
    strcpy( out_name, filename );
    DirStrings::ChangeExtension( out_name, ".txt" );
    DirStrings::ChangeDirectory( out_name, "f:\\test\\" );

    header->OutputAsText( out_name );
  #endif

    // Save the Palette and Image Data
    switch( header->bits_per_pixel ) {
    case BMP_1_BIT:
      retval = Save1Bit(  out, out_image, header );
      break;
    case BMP_4_BIT:
      retval = Save4Bit(  out, out_image, header );
      break;
    case BMP_8_BIT:
      retval = Save8Bit(  out, out_image, header );
      break;
    case BMP_16_BIT:
      retval = Save16Bit( out, out_image, header );
      break;
    case BMP_24_BIT:
      retval = Save24Bit( out, out_image, header );
      break;
    case BMP_32_BIT:
      retval = Save32Bit( out, out_image, header );
      break;
    }

    // Set the Header's Image Size
    streampos file_size  = out.tellp();
    streampos image_size = file_size - image_start - ( sizeof( IMG_BYTE ) * 4 * header->colors_used ); // Subtract off the palette
    out.seekp( image_size_pos );
    out << image_size;

    // Set the Header's File Size
    out.seekp( 2 );
    out << file_size;
  } else {
    BMP::last_error = IMGSAVER_ERROR_WRITING_HEADER;
  }

  // Free the image allocated by Header::FillWithImageClass(), if needed
  if( image != out_image )
    delete out_image;

  out.close();
  return retval;
}


// Save1Bit()
//  Saves the palette and image data of a 1-bit BMP.  Returns
//  true if successful, and false if not.  This function assumes
//  that the header has already been written out.
bool BMP::Save1Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  BMP::last_error = IMGSAVER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Save4Bit()
//  Saves the palette and image data of a 1-bit BMP.  Returns
//  true if successful, and false if not.  This function assumes
//  that the header has already been written out.
bool BMP::Save4Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  BMP::last_error = IMGSAVER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Save8Bit()
//  Saves the palette and image data of a 1-bit BMP.  Returns
//   true if successful, and false if not.  This function assumes
//   that the header has already been written out.
//  Note that this saver assumes that the image is in indexed mode.
bool BMP::Save8Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  IMG_BYTE *rgb;
  IMG_BYTE color[4] = { 0, 0, 0, 0 };

  // Output the palette
  unsigned int i;
  for( i=0; i < image->GetNumRegisters(); i++ ) {
    rgb = image->GetPaletteColor( i );
    // Swap R and B, since BMPs store these backward (both Intel and other)
    StringTools::Reverse( color, rgb, 3 );
    out.write( color, 4 );
  }

  int old_zero = image->GetZeroZero();

  int height = header->height;
  if( height < 0 )
    height = abs( height );
  else
    image->SetZeroZero( IMAGE_BOTTOMLEFT );

  // Uncompressed 8-Bit BMP
  if( header->compression == BMP_NONE ) {
    // Save the image data.
    IMG_BYTE *line;
    int pad = header->width % 2;

    for( int y=0; y < height; y++ ) {
      line = image->GetLine( y );
      out.write( line, header->width );  // Write out the whole line

      if( pad > 0 )                      // Write out pad bytes, if needed
        out << '\0';

      if( !out ) {
        return false;
      }
    }

  } else {
  // RLE-Compressed 8-Bit BMP
    int line_count = 0;
    int code_pixel_count = 0;
    int line_pixel_count = 0;
    IMG_BYTE *line;

    enum bmp_rle8_modes { RLE8_UNKNOWN, RLE8_DETERMINE, RLE8_ABSOLUTE, RLE8_ENCODED };
    bmp_rle8_modes mode = RLE8_UNKNOWN;

    BMP_RLE8_Line   data;
    BMP_RLE8_Code * current;

    #ifdef DEBUG_BMP_RLE8_SAVER
      ofstream aout( "e:\\Test_RLE8_ASCII.txt" );
    #endif

    for( line_count = 0; line_count < header->height; line_count++ ) {        // Iterate through each line
      #ifdef DEBUG_BMP_RLE8_SAVER
        cerr << "    Comrpessing Line " << line_count << "..." << endl;
      #endif
      line = image->GetLine( line_count );
      line_pixel_count = 0;

      while( line_pixel_count < header->width ) {                            // Stop at the end of a line
        code_pixel_count = 0;

        // Figure out what mode we're in
        if( line[ line_pixel_count ] == line[ line_pixel_count + 1 ] ) {
          #ifdef DEBUG_BMP_RLE8_SAVER
            cerr << "      Encoded Mode"<< endl;
          #endif
          data.codes.Add( new BMP_RLE8_Code( BMP_RLE8_Code::RLE8_ENCODED ) );    // Create a new Encoded code
        } else {
          data.codes.Add( new BMP_RLE8_Code( BMP_RLE8_Code::RLE8_ABSOLUTE ) );   // Create a new Absolute code
          #ifdef DEBUG_BMP_RLE8_SAVER
            cerr << "      Absolute Mode"<< endl;
          #endif
        }
        current = data.codes[ data.codes.NumElements()-1 ];

        // Count the number of pixels in this code
        for( code_pixel_count = 1; line_pixel_count + code_pixel_count < header->width; code_pixel_count++ ) {
          if( current->GetMode() == BMP_RLE8_Code::RLE8_ENCODED ) {
            if( line[ line_pixel_count + code_pixel_count ] != line[ line_pixel_count + code_pixel_count + 1 ] ) {
              code_pixel_count++;
              break;
            }
          } else {
            if( line[ line_pixel_count + code_pixel_count ] == line[ line_pixel_count + code_pixel_count + 1 ] ) {
              break;
            }
          }
        }

        current->SetNumPixels( code_pixel_count );
        line_pixel_count += code_pixel_count;
      }


      #ifdef DEBUG_BMP_RLE8_SAVER
        data.OutputAsAscii( aout, line, line_count );
      #endif

      // Optimize and output the line
      data.Optimize();
      data.Write( out, line );
      data.codes.Flush();
    }

    // Output an End Of Data
    out << (char)0x00 << (char)0x01;
  }

  // Restore previous settings
  image->SetZeroZero( old_zero );

  return true;
}

// Save16Bit()
//  Saves the image data of a 16-bit BMP.  Returns true
//  if successful, and false if not.  This function assumes
//  that the header has already been written out.
bool BMP::Save16Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  BMP::last_error = IMGSAVER_MODE_NOT_SUPPORTED;
  return NULL;
}

// Save24Bit()
//  Saves the image data of a 24-bit BMP.  Returns true
//  if successful, and false if not.  This function assumes
//  that the header has already been written out.
bool BMP::Save24Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  const IMG_BYTE *rgb;

  int old_zero = image->GetZeroZero();

  int height = header->height;
  if( height < 0 )
    height = abs( height );
  else
    image->SetZeroZero( IMAGE_BOTTOMLEFT );

  int  pad = (header->width * 3) % 2;

  int x, y;
  for( y = 0; y < header->height; y++ ) {
    for( x = 0; x < header->width; x++ )  {
      rgb = image->GetRGBAt( x, y );
      out << rgb[2] << rgb[1] << rgb[0];
    }

    if( pad > 0 )                      // Write out pad bytes, if needed
      out << '\0';
  }


  // Restore previous settings
  image->SetZeroZero( old_zero );

  return true;
}

// Save32Bit()
//  Saves the image data of a 1-bit BMP.  Returns true if
//  successful, and false if not.  This function assumes
//  that the header has already been written out.
bool BMP::Save32Bit( bofstream &out, ImageClass *image, BMPHeader *header ) {
  BMP::last_error = IMGSAVER_MODE_NOT_SUPPORTED;
  return NULL;
}

// IsReocognized()
//  Performs a check on the contents of the file pointed to by the filename 
//   passed into to see if it's recognized.  This is the most accurate test,
//   but also the slowest, since it involves opening the file to test
//   it's contents.  Returns IMGIOREC_RECOGNIZED_DATA if identified.
int BMP::IsRecognized( const char *filename ) {
  // Open and test the input file
  bifstream in( filename, ios::nocreate | ios::binary | ios::in );
  if( !in ) {
    BMP::last_error = IMGLOADER_UNKNOWN_ERROR;
    return IMGIOREC_ERROR;
  }

  in.SetEndian( ENDIAN_INTEL );
  
  // Read in and test the Header ID (should be "BM")
  unsigned short test_header_id = 0;
  in >> test_header_id;
  in.close();

  if( test_header_id == header_id )
    return IMGIOREC_RECOGNIZED_DATA;

  return IMGIOREC_NOT_RECOGNIZED;
}

// IsReocognizedExtension()
//  Performs a quick check on the filename passed in to see if it's 
//   extension is known by the file format.  Note that this is an inaccurate
//   test, as files are not required to have extensions. Returns
//   IMGIOREC_RECOGNIZED_EXTENSION if identified.
int BMP::IsRecognizedExtension( const char *filename ) {
  int i = 0;
  char *extension = &((DirStrings::HasExtension(filename))[1]);  // Skip the '.' in the extension

  if( extension == NULL )                     // Make sure there's an extension in the filename
    return IMGIOREC_NOT_RECOGNIZED;

  while( BMPExtensionList[ i ] != NULL ) {
    if( stricmp( BMPExtensionList[i], extension ) == 0 )
      return IMGIOREC_RECOGNIZED_HEADER;
    i++;
  };

  return IMGIOREC_NOT_RECOGNIZED;
}

// IsRecognizedHeader()
//  Performs a quick check against the header string passed in.  This is the
//   best compromize of speed vs. accuracy test.  Unfortunately, not all
//   file formats contain headers, and those that don't will return
//   IMGIOREC_NOT_AVAILABLE.  Returns IMGIOREC_RECOGNIZED_HEADER if
//   identified.
int BMP::IsRecognizedHeader( const char *header ) {
  if( strlen( header ) < strlen( header_string ) )
    return IMGIOREC_HEADER_TOO_SHORT;

  if( stricmp( header, GetFileFormatHeader() ) == 0 )
    return IMGIOREC_RECOGNIZED_HEADER;

  return IMGIOREC_NOT_RECOGNIZED;
}


// ---
// ------------------------------------- BMPHeader Member Functions ---
// ---

// Constructor
BMPHeader::BMPHeader()
  : filesize(0), reserved_1(0), reserved_2(0), offset(0),
    header_size(40), width(0), height(0), planes(1),
    bits_per_pixel(BMP_8_BIT), compression(BMP_NONE),
    bytes_in_image(0), x_pels(0), y_pels(0),
    colors_used(0), colors_important(0) {
}

// Load()
//  Loads the BMP header from the given input stream
bool BMPHeader::Load( bifstream &in ) {
  in >> filesize >> reserved_1 >> reserved_2 >> offset;

  in >> header_size;
  if( header_size != 40 )  // Fail if the header isn't 40 (required)
    return false;

  in >> width >> height >> planes >> bits_per_pixel >> compression;
  in >> bytes_in_image >> x_pels >> y_pels >> colors_used >> colors_important;

  if( !in )
    return false;

  return true;
}

// Save()
//  Saves the BMP header into the given output stream.  Returns
//  the streampos pointing to the bytes_in_image part of the header,
//  which should be set by the saver function.  
streampos BMPHeader::Save( bofstream &out ) {
  out << ((unsigned long)0);                                       // File size is unknown right now
  out << reserved_1 << reserved_2;

  streampos offset_pos = out.tellp();
  out << offset;                                                   // This will get overwritten later on

  streampos header_size_pos = out.tellp();
  out << header_size;                                              // This will get overwritten later on, too

  out << width << height << planes << bits_per_pixel << compression;

  streampos bim_pos = out.tellp();
  out << bytes_in_image << x_pels << y_pels << colors_used << colors_important;

  if( !out )
    return false;

  // Set the offset to the image data.
  streampos end_pos = out.tellp();
  unsigned long true_offset = end_pos + ( sizeof( IMG_BYTE ) * 4 * colors_used );

  out.seekp( offset_pos );
  out << true_offset;

  // Set the header size.  This should be 40, so it'll assert if it
  //  isn't.  If it isn't 40, it's probably because the size of the
  //  datatypes are different on the platform you are compiling on. 
  unsigned long true_header_size = end_pos - header_size_pos;
  assert( true_header_size == 40 );  // FATAL ERROR HERE!!! This should be 40!

  out.seekp( header_size_pos );
  out << true_header_size;
  
  out.seekp( end_pos );

  if( !out )
    return 0;

  return bim_pos;
}

// OutputAsText()
//  Output the header as human-readable ASCII text to the
//  given filename.  This is meant for debugging purposes.
bool BMPHeader::OutputAsText( const char *filename ) {
  ofstream out( filename );
  if( !out )
    return false;

  out << " BMP Header Information:" << endl;
  out << "  File Size:        " << filesize << endl;
  out << "  Reserved 1:       " << reserved_1 << endl;
  out << "  Reserved 2:       " << reserved_2 << endl;
  out << "  Offset To Image:  " << offset << endl;
  out << endl;
  out << "  Header Size:      " << header_size << endl;
  out << "  Width:            " << width << endl;
  out << "  Height:           " << height << endl;
  out << "  Planes:           " << planes << endl;
  out << "  Bits Per Pixel:   " << bits_per_pixel << endl;

  out << "  Compression:      "; 
  switch( compression) {
  case BMP_NONE:
    out << "None" << endl;
    break;
  case BMP_RLE8:
    out << "8-Bit RLE" << endl;
    break;
  case BMP_RLE4:
    out << "4-Bit RLE" << endl;
    break;
  default:
    out << "Unknown" << endl;
  }

  out << "  Bytes In Image:   " << bytes_in_image << endl;
  out << "  X Pels:           " << x_pels << endl;
  out << "  Y Pels:           " << y_pels << endl;
  out << "  Colors Used:      " << colors_used << endl;
  out << "  Important Colors: " << colors_important << endl;

  return true;
}


// FillWithImageClass()
//  Fills in the header with the given image's values.  Anything
//   that it can't know about from the image is left at it's
//   default.  If BMP_INVALID_BIT_TYPE is passed in, a guess will
//   be made based on the image type; otherwise, that type will be
//   the output type.  If the image isn't in the correct mode, a
//   new image in the required mode will be created and returned.
//  The function returns a pointer to the ImageClass needed to comply
//   with the bit_type passed in.  If it is BMP_INVALID_BIT_TYPE, then
//   the returned ImageClass is always equal to the image passed in.
//   If the bit_type is not BMP_INVALID_BIT_TYPE, then the ImageClass
//   returned may have been created by this function.  The returned
//   image must be freed by the user.  BE SURE TO CHECK THIS!
//  Note that the value that bytes_in_image is set to by this function
//   only applies to uncompressed image.  The compressed image size
//   isn't known until the file has been saved to disk.
ImageClass * BMPHeader::FillWithImageClass( ImageClass *image,
                                            BMPBitTypes bit_type ) {
  if( image == NULL )
    return NULL;

  width  = image->GetWidth();            // True image width.  Does not include 4-byte alignment stuff
  height = image->GetHeight();           // Image height.

  ImageClass * ret_image = image;

  if( (bit_type == BMP_INVALID_BIT_TYPE) &&
      (bits_per_pixel != BMP_INVALID_BIT_TYPE ) )
    bit_type = (BMPBitTypes)bits_per_pixel;

  // Number of Bits Per Pixel and Colors Used.
  //  Only supports 1, 4, 8 and 24-bit right now.
  if( bit_type == BMP_INVALID_BIT_TYPE ) {  // We figure out what the type is
    switch( image->GetType() ) {
    case IMAGE_INDEXED:
      colors_used = image->GetNumRegisters();
      colors_important = colors_used;       // I don't know quite what this means, so it's equal to colors_used for now.

      if( image->GetNumRegisters() < 3 ) {
        bytes_in_image = width * height/8;  // 8 pixels per byte
        bits_per_pixel = 1;
      } else if( image->GetNumRegisters() < 17 ) {
        bytes_in_image = width * height/2;  // 2 pixels per byte
        bits_per_pixel = 4;
      } else {
        bytes_in_image = width * height;    // 1 pixel per byte
        bits_per_pixel = 8;
      }
      break;

    case IMAGE_RGB:
    case IMAGE_RGBA:
      colors_used = 0;
      colors_important = 0;
      bytes_in_image = width * height * 3;  // 1/3 pixel per byte
      bits_per_pixel = 24;
      break;

    case IMAGE_GREY:
      colors_used = 256;
      colors_important = 0;
      bytes_in_image = width * height;      // 1 pixel per byte
      bits_per_pixel = 8;
      break;

    default:                                // Unknown image type; set to 0 and fail
      colors_used = 0;
      colors_important = 0;
      bytes_in_image = width * height;
      bits_per_pixel = 0;
      SetCompression( false );
      ret_image = NULL;
    }
  } else {                                  // User supplies the image type
    // -- Most of this is currently not implemented.  --
    switch( bit_type ) {
    case BMP_1_BIT:
      if( !( ( image->GetType() == IMAGE_INDEXED ) &&
           ( image->GetNumRegisters() > 2 ) ) )
        ret_image = image->ConvertTypeTo( IMAGE_INDEXED, 8 );

      colors_used = ret_image->GetNumRegisters();
      colors_important = 0;
      bytes_in_image = width * height/8;    // 8 pixels per byte
      bits_per_pixel = 1;
      break;

    case BMP_4_BIT:
      if( !( ( image->GetType() == IMAGE_INDEXED ) &&
           ( image->GetNumRegisters() > 16 ) ) )
        ret_image = image->ConvertTypeTo( IMAGE_INDEXED, 16 );

      colors_used = ret_image->GetNumRegisters();
      colors_important = 0;
      bytes_in_image = width * height/2;    // 2 pixels per byte
      bits_per_pixel = 4;
      break;

    case BMP_8_BIT:
      if( image->GetType() != IMAGE_INDEXED )
        ret_image = image->ConvertTypeTo( IMAGE_INDEXED, 256 );

      colors_used = ret_image->GetNumRegisters();
      colors_important = 0;
      bytes_in_image = width * height;      // 1 pixel per byte
      bits_per_pixel = 8;
      break;

    // True-Color modes.  No need to convert to true color, since
    //  we can always get the RGBs from the image.
    case BMP_16_BIT:
      colors_used = 0;
      colors_important = 0;
      bits_per_pixel = 16;
      bytes_in_image = width * height/2;    // 1/2 pixel per byte
      break;

    case BMP_24_BIT:
      colors_used = 0;
      colors_important = 0;
      bytes_in_image = width * height/3;    // 1/3 pixel per byte
      bits_per_pixel = 24;
      break;

    case BMP_32_BIT:
      colors_used = 0;
      colors_important = 0;
      bytes_in_image = width * height/4;    // 1/4 pixel per byte
      bits_per_pixel = 32;
      break;

    default:                                // Uknown BMP BPP Type
      ret_image = NULL;
    }
  }

  if( compression != BMP_NONE )
    SetCompression( true );

  return ret_image;
}

// SetCompression():
//  Toggle image compression.  The header determines which type
//  of compression should be used baed on the number of bits per
//  pixel.  Returns the previous compression type.  Note that if
//  the BPP mode doesn't support compression, compression will
//  still be false.
bool BMPHeader::SetCompression( bool compress ) {
  bool old_comp = compress;
  if( !compress ) {
    compression = BMP_NONE;
    return old_comp;
  }

  switch( bits_per_pixel ) {
  case BMP_1_BIT:
    compression = BMP_NONE;
    break;

  case BMP_4_BIT:
    compression = BMP_RLE4;
    break;

  case BMP_8_BIT:
    compression = BMP_RLE8;
    break;

  case BMP_16_BIT:
  case BMP_32_BIT:
    compression = BMP_BITFIELDS;
    break;

  case BMP_24_BIT:
    compression = BMP_RGB;
    break;
  }

  return old_comp;
}

// FillWithImageData()
//  Fills in a header instance with the the values from the provided
//   ImageData class.  This is generally called just before a Save()
//   function is called.
//  Note that this does NOT call FillWithImageClass(); this is up to
//   the user (or to the Save() function).
bool BMPHeader::FillWithImageData( ImageData *data ) {
  // See if an ImageData class was supplied
  if( data == NULL )
    return false;

  // Size
  width  = data->GetImage()->GetWidth();
  height = data->GetImage()->GetHeight();

  // Compression
  if( data->GetCompression() != IMGCOMPRESSION_NONE )
    SetCompression( true );
  else
    SetCompression( false );

  // DPI
  if( data->GetDPIX() > 0 )
    x_pels = data->GetDPIX();

  if( data->GetDPIY() > 0 )
    y_pels = data->GetDPIY();

  //  Bit Depth
  switch( data->GetBitsPerPixel() ) {
  case 1:
    bits_per_pixel = BMP_1_BIT;
    break;
  case 4:
    bits_per_pixel = BMP_4_BIT;
    break;
  case 8:
    bits_per_pixel = BMP_8_BIT;
    break;
  case 16:
    bits_per_pixel = BMP_16_BIT;
    break;
  case 24:
    bits_per_pixel = BMP_24_BIT;
    break;
  case 32:
    bits_per_pixel = BMP_32_BIT;
    break;
  default:  // Unknown
    bits_per_pixel = BMP_INVALID_BIT_TYPE;
    break;
  }

  return true;
}

// FillInImageData()
//  Fills in an ImageData class with values from a header instance.
//   This is called directly by the Load() function.
bool BMPHeader::FillInImageData( ImageData *data ) {
  // See if an ImageData class was supplied
  if( data == NULL )
    return false;

  data->SetFileFormat( IMGFF_BMP );
  data->SetFileSize( filesize );
  data->SetBitsPerPixel( bits_per_pixel );
  data->SetDPI( x_pels, y_pels );

  switch( compression ) {
  case BMP_NONE:      // No Compression
  case BMP_BITFIELDS:
    data->SetCompression( IMGCOMPRESSION_NONE );
    break;
  case BMP_RLE8:      // Run-Length Encoding
  case BMP_RLE4:
    data->SetCompression( IMGCOMPRESSION_RLE );
    break;
  }

  data->SetFileData( this );

  return true;
}
