//
// Targa.cpp
//

#include <math.h>
#include <assert.h>

#include "Targa.h"
//#include "Targa_RLE8.h"

#include <portable/DirTools/DirTools.h>

// Load()
//  Load a Targa image.  Returns NULL if the file couldn't be
//   loaded.   If an ImageData class is passed in, it will be
//   filled with information from the loaded file's header.
ImageClass *Targa::Load( const char * filename, ImageData *data ) {
  Targa::last_error = IMGLOADER_NO_ERROR;

  // Make sure the ImageData is there
  if( data == NULL ) {
    Targa::last_error = IMGLOADER_INVALID_USER_DATA;
    return NULL;
  }

  // See if the Targa exists
  if( !DirInfo::Exists( filename ) ) {
    Targa::last_error = IMGLOADER_FILE_NOT_FOUND;
    return NULL;
  }

  // Check the header/footer ID and/or extension (older Targa formats)
  if( !IsRecognized( filename ) ) {
    Targa::last_error = IMGLOADER_UNRECOGNIZED_FILE_FORMAT;
    return NULL;
  }

  // Open and test the input file
  bifstream in( filename, ios::nocreate | ios::binary | ios::in );
  if( !in ) {
    Targa::last_error = IMGLOADER_ERROR_OPENING_FILE;
    return NULL;
  }

  in.SetEndian( ENDIAN_INTEL );
  
  // Read in the Targa header information.  Do byte-order-swapping
  //  on non-Intel machines here.
  TargaHeader * header = new TargaHeader;
  if( !header->Load( in ) ) {
    Targa::last_error = IMGLOADER_ERROR_READING_HEADER;
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

  // Load the image data
  switch( header->image_type ) {
  case TARGATYPE_COLOR_MAPPED:
    image = LoadColorMapped( in, header );
    break;
  case TARGATYPE_COLOR_MAPPED_RLE:
    image = LoadColorMappedRLE( in, header );
    break;

  case TARGATYPE_TRUE_COLOR:
    image = LoadTrueColor( in, header );
    break;
  case TARGATYPE_TRUE_COLOR_RLE:
    image = LoadTrueColorRLE( in, header );
    break;

  case TARGATYPE_BLACK_AND_WHITE:
    image = LoadGreyscale( in, header );
    break;
  case TARGATYPE_BLACK_AND_WHITE_RLE:
    image = LoadGreyscaleRLE( in, header );
    break;
  }

  if( image == NULL ) {
    return NULL;
  }

  // Fill in the ImageData class
  if( data != NULL ) {
    header->FillInImageData( data );
    data->SetFilename( filename );
    data->SetImage( image );
  }

  return image;
}

// LoadColorMapped()
//  Loads the palette and image data of an indexed Targa.  Returns
//  a pointer to the image class created.  This function assumes
//  that the file pointer is at the begining of the palette data.
ImageClass * Targa::LoadColorMapped( bifstream &in, TargaHeader *header ) {
  Targa::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// LoadColorMappedRLE()
//  Loads the palette and image data of an RLE-compressed indexed
//   Targa.  Returns a pointer to the image class created.  This
//   function assumes that the file pointer is at the begining of
//   the palette data.
ImageClass * Targa::LoadColorMappedRLE( bifstream &in, TargaHeader *header ) {
  Targa::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// LoadTrueColor()
//  Loads the image data of a true-color Targa.  Returns
//   a pointer to the image class created.  This function assumes
//   that the file pointer is at the begining of the image data.
//  Note that currently only 8-bit Alphas are used.  1-7 bit
//   Alphas will be loaded as a full 8 bits, and may thus be
//   ususable or mangled.
//  Contrary to the Targa File Format Spec, the data seems
//   to be stored backward (ie: ABGR).
//  BIG NOTE:  This currently ONLY supports 24-bit RGB and
//   32-bit RGBA Targas. 16 bit RGB and 24-bit RGBA is not
//   yet supported.
ImageClass * Targa::LoadTrueColor( bifstream &in, TargaHeader *header ) {
  // Test for an Alpha and allocate the new ImageClass
  ImageClass *image = NULL;
  IMG_BYTE RGB[4];

  in.seekg( header->image_offset, ios::beg );

  int old_zerozero;

  if( header->descriptor.attrib_bits_per_pixel > 0 ) {
    // RGBA
    image = new ImageClass( header->width, header->height,
                            IMAGE_RGBA );
    if( !image )
      return NULL;

    old_zerozero = image->GetZeroZero();
    if( !header->descriptor.ordering_top_to_bottom )
      image->SetZeroZero( IMAGE_BOTTOMLEFT );

    // Read in the RGBA data
    for( int y=0; y < header->height; y++ ) {
      for( int x=0; x < header->width; x++ ) {
        in >> RGB[2] >> RGB[1] >> RGB[0] >> RGB[3];
        image->SetRGBAAt( x, y, RGB );
      }
    }
  } else {
    // RGB
    image = new ImageClass( header->width, header->height,
                            IMAGE_RGB );
    if( !image )
      return NULL;

    old_zerozero = image->GetZeroZero();
    if( !header->descriptor.ordering_top_to_bottom )
      image->SetZeroZero( IMAGE_BOTTOMLEFT );


    // Read in the RGB data
    for( int y=0; y < header->height; y++ ) {
      for( int x=0; x < header->width; x++ ) {
        in >> RGB[2] >> RGB[1] >> RGB[0];
        image->SetRGBAt( x, y, RGB );
      }
    }
  }

  // Check for errors
  if( !in )
    return NULL;

  // Restore the Zero-Zero mode
  image->SetZeroZero( old_zerozero );

  return image;
}

// LoadTrueColorRLE()
//  Loads the image data of an RLE-compressed true-color Targa.
//   Returns a pointer to the image class created.  This function
//   assumes that the file pointer is at the begining of the
//   image data.
//  Note that currently only 8-bit Alphas are used.  1-7 bit
//   Alphas will be loaded as a full 8 bits, and may thus be
//   ususable or mangled.
//  BIG NOTE:  This currently ONLY supports 24-bit RGB and
//   32-bit RGBA Targas. 16 bit RGB and 24-bit RGBA is not
//   yet supported.
ImageClass * Targa::LoadTrueColorRLE( bifstream &in, TargaHeader *header ) {
  // Test for an Alpha and allocate the new ImageClass
  ImageClass *image = NULL;
  IMG_BYTE RGB[4];
  Targa_RLE_Packet packet;

  // Seek to the image data
  in.seekg( header->image_offset, ios::beg );

  int old_zerozero;

  if( header->descriptor.attrib_bits_per_pixel > 0 ) {
    // --------------------
    // RGBA
    image = new ImageClass( header->width, header->height,
                            IMAGE_RGBA );
    if( !image )
      return NULL;

    old_zerozero = image->GetZeroZero();
    if( !header->descriptor.ordering_top_to_bottom )
      image->SetZeroZero( IMAGE_BOTTOMLEFT );

    // Read in the RGBA data
    int x=0, y=0, i;
    do {
      in.read( (char *)(&packet), 1 );          // Read in a packet
      if( packet.type == TARGA_PACKET_RLE ) {   // RLE Packet
        in >> RGB[2] >> RGB[1] >> RGB[0] >> RGB[3];
        for( i=0; i < packet.count+1; i++ )
          image->SetRGBAAt( x++, y, RGB );
      } else {                                  // Raw Packet
        for( i=0; i < packet.count+1; i++ ) {
          in >> RGB[2] >> RGB[1] >> RGB[0] >> RGB[3];
          image->SetRGBAAt( x++, y, RGB );
        }
      }
      
      // Update line number
      if( x >= header->width ) {
        x = 0;
        y++;
      }
    } while( y < header->height );
  } else {
    // --------------------
    // RGB
    image = new ImageClass( header->width, header->height,
                            IMAGE_RGB );
    if( !image )
      return NULL;

    old_zerozero = image->GetZeroZero();
    if( !header->descriptor.ordering_top_to_bottom )
      image->SetZeroZero( IMAGE_BOTTOMLEFT );

    // Read in the RGB data
    int x=0, y=0, i;
    do {
      in.read( (char *)(&packet), 1 );          // Read in a packet
      if( packet.type == TARGA_PACKET_RLE ) {   // RLE Packet
        in >> RGB[2] >> RGB[1] >> RGB[0];
        for( i=0; i < packet.count+1; i++ ) {
          image->SetRGBAt( x++, y, RGB );

          if( (in.bad() || in.fail()) && !in.eof() ) 
            return NULL;
        }
      } else {                                  // Raw Packet
        for( i=0; i < packet.count+1; i++ ) {
          in >> RGB[2] >> RGB[1] >> RGB[0];
          image->SetRGBAt( x++, y, RGB );

          if( (in.bad() || in.fail()) && !in.eof() ) 
            return NULL;
        }
      }

      // Update line number
      if( x >= header->width ) {
        x = 0;
        y++;
      }

      if( y > header->height )
        break;

      if( (in.bad() || in.fail()) && !in.eof() ) 
        return NULL;

    } while( (y < header->height) || in.eof() );

    assert( (y - 1) * header->width + x <= header->width * header->height );
  }

  // Check for errors
  if( !in )
    return NULL;

  // Restore the Zero-Zero mode
  image->SetZeroZero( old_zerozero );

  return image;
}

// LoadGreyscale()
//  Loads the a greyscale Targa image.  Returns a pointer to the
//   image class created.  This function assumes that the file
//   pointer is at the begining of the palette data.
ImageClass * Targa::LoadGreyscale( bifstream &in, TargaHeader *header ) {
  Targa::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// LoadGreyscaleRLE()
//  Loads the an RLE-compressed greyscale Targa image.  Returns
//   a pointer to the image class created.  This function assumes
//   that the file pointer is at the begining of the palette data.
ImageClass * Targa::LoadGreyscaleRLE( bifstream &in, TargaHeader *header ) {
  Targa::last_error = IMGLOADER_MODE_NOT_SUPPORTED;
  return NULL;
}

// IsReocognized()
//  Performs a check on the contents of the file pointed to by the filename 
//   passed into to see if it's recognized.  This is the most accurate test,
//   but also the slowest, since it involves opening the file to test
//   it's contents.  Returns IMGIOREC_RECOGNIZED_DATA if identified.
int Targa::IsRecognized( const char *filename ) {
  // Open and test the input file
  bifstream in( filename, ios::nocreate | ios::binary | ios::in );
  if( !in ) {
    Targa::last_error = IMGLOADER_UNKNOWN_ERROR;
    return IMGIOREC_ERROR;
  }

  in.SetEndian( ENDIAN_INTEL );
  
  // Read in and test the Footer ID (should be "TRUEVISION-XFILE")
  char signature[18];
  in.seekg( -18, ios::end );                       // Skip to the signature
  in.read( signature, 18 );                        // Read in the signature
  in.close();

  if( strcmp( signature, TargaSignature ) == 0 )   // Test the signature
    return IMGIOREC_RECOGNIZED_DATA;

  // Signature test failed, fall back to extension test
  return IsRecognizedExtension( filename );
}

// IsReocognizedExtension()
//  Performs a quick check on the filename passed in to see if it's 
//   extension is known by the file format.  Note that this is an inaccurate
//   test, as files are not required to have extensions. Returns
//   IMGIOREC_RECOGNIZED_EXTENSION if identified.
int Targa::IsRecognizedExtension( const char *filename ) {
  int i = 0;
  char *extension = &((DirStrings::HasExtension(filename))[1]);  // Skip the '.' in the extension

  if( extension == NULL )                     // Make sure there's an extension in the filename
    return IMGIOREC_NOT_RECOGNIZED;

  while( TargaExtensionList[ i ] != NULL ) {  // Rest each extension in the list
    if( stricmp( TargaExtensionList[i], extension ) == 0 )
      return IMGIOREC_RECOGNIZED_HEADER;
    i++;
  };

  return IMGIOREC_NOT_RECOGNIZED;
}

// IsRecognizedHeader()
//  This function is designed to check against a header in the file, usually
//   the first few bytes.  Targas do not contain a header ID, but rather a
//   footer ID.  Thus, this function will not return meaningful data.
int Targa::IsRecognizedHeader( const char *header ) {
  return IMGIOREC_NOT_AVAIlABLE;
}



// ---
// ------------------------------------- TargaHeader Member Functions ---
// ---

// Constructor
TargaHeader::TargaHeader()
  : filesize(0), extension_offset(0), developer_offset(0),
    color_map_offset(0), extension_fixed_size(0),
    image_offset(0), has_footer(false), id_length(0),
    color_map_type(COLORMAP_NONE), first_index(0), num_indices(0),
    image_type(TARGATYPE_TRUE_COLOR),
    x_offset(0), y_offset(0), width(0), height(0),
    bits_per_pixel(TARGATYPE_TRUE_COLOR), compression(false),
    palette_bits_per_pixel(0),
    software_version(0), software_subrevision(' '),
    pixel_aspect_num(1), pixel_aspect_denom(1),
    gamma_num(1), gamma_denom(0),                      // NOTE:  The default value is 1/0, which means no gamma correction
    color_correction_offset(0), postage_stamp_offset(0),
    scanline_offset(0), attribute_type(0),
    postage_stamp_width(0), postage_stamp_height(0),
    scanlines(NULL), postage_stamp(NULL),
    color_correction(NULL) {

  descriptor.attrib_bits_per_pixel  = 0;
  descriptor.ordering_left_to_right = 0;
  descriptor.ordering_top_to_bottom = 0;
  descriptor.reserved               = 0;

  author_name[0]    = '\0';
  author_comment[0] = '\0';
  job_name[0]       = '\0';
  software_id[0]    = '\0';

  date_stamp[0] = date_stamp[1] = date_stamp[2] = 0;
  date_stamp[3] = date_stamp[4] = date_stamp[5] = 0;

  job_time[0] = job_time[1] = job_time[2] = 0;

  key_color[0] = key_color[1] = key_color[2] = key_color[3] = 0;
}

// Destructor
TargaHeader::~TargaHeader() {
  developer.Flush();

  if( scanlines != NULL )
    delete scanlines;

  if( postage_stamp != NULL )
    delete postage_stamp;

  if( color_correction != NULL )
    delete color_correction;
}

// Load()
//  Loads the Targa header and footer from the given
//   input stream.
bool TargaHeader::Load( bifstream &in ) {
  // First scan and read in the footer
  in.seekg( -18, ios::end );
  filesize = in.tellg() + 18;                     // Get the size of the file

  char signature[18];                             // See if it's a Version 1 or Version 2+ Targa
  in.read( signature, 18 );
  if( strcmp( signature, TargaSignature) != 0 )   // Version 1 Targa
    has_footer = false;
  else {                                          // Version 2+ Targa
    has_footer = true;
    in.seekg( -26, ios::end );
    in >> extension_offset >> developer_offset;     // Read in the offsets
  }

  if( in.eof() && !in.fail() && !in.bad() )       // Clear an EOF, if one is set, so we can start again from the begining of the file
    in.clear();

  // Read in the file header
  in.seekg( 0, ios::beg );
  in >> id_length;
  in >> color_map_type >> image_type;
  in >> first_index >> num_indices >> palette_bits_per_pixel;
  
  in >> x_offset >> y_offset >> width >> height >> bits_per_pixel;
  in.read( (char *)(&descriptor), 1 );

  // Read in the ID
  in.read( id, id_length );

  // Get the Color Map/Image Offset
  if( color_map_type )
    color_map_offset = in.tellg();
  else
    image_offset = in.tellg();

  //
  // ----------------------------------------------------
  //

  // Read in the Targa Extension Area
  if( extension_offset != 0 ) {
    in.seekg( extension_offset, ios::beg );
    in >> extension_fixed_size;
    
    in.read( author_name,     41 );
    in.read( author_comment, 324 );

    // These have to be read in individually to handle endian reversal
    in >> date_stamp[0] >> date_stamp[1] >> date_stamp[2]
       >> date_stamp[3] >> date_stamp[4] >> date_stamp[5];

    in.read( job_name, 41 );
    in >> job_time[0] >> job_time[1] >> job_time[2];

    in.read( software_id, 41 );
    in >> software_version >> software_subrevision;

    in.read( key_color, 4 );

    in >> pixel_aspect_num >> pixel_aspect_denom;
    in >> gamma_num >> gamma_denom;

    in >> color_correction_offset;
    in >> postage_stamp_offset;
    in >> scanline_offset;
    
    // Read in the Scanline Table
    if( scanline_offset != 0 ) {
      in.seekg( scanline_offset, ios::beg );
      scanlines = new long[ height ];

      for( int i=0; i < height; i++ )
        in >> scanlines[i];
    }

    // Read in the Thumbnail Image
    if( postage_stamp_offset != 0 ) {
      in.seekg( postage_stamp_offset, ios::beg );
      in >> postage_stamp_width >> postage_stamp_height;

      // Insert Thumbnail Image Reader Here
    }

    // Read in the Color Correction Table
    if( color_correction_offset != 0 ) {
      in.seekg( color_correction_offset, ios::beg );
      color_correction = new short[ 2048 ];

      for( int i=0; i < 2048; i++ )
        in >> color_correction[i];
    }
  }

  //
  // ----------------------------------------------------
  //

  // Read in the Developer Area
  if( developer_offset != 0 ) {
    in.seekg( developer_offset, ios::beg );

    short num_tags;
    in >> num_tags;

    // Read in the Developer Directory
    TargaDeveloper * dev;
    for( short i=0; i < num_tags; i++ ) {
      dev = new TargaDeveloper;
      in >> dev->tag >> dev->offset >> dev->size;
      developer.Add( dev );
    }

    // Read in each Developer's Data
    for( unsigned int j = 0; j < developer.NumElements(); j++ ) {
      in.seekg( developer[j]->offset, ios::beg );
      in.read( developer[j]->data, developer[j]->size );
    }
  }

  if( image_type > 8 )
    compression = true;
  else
    compression = false;

  if( !in && !in.eof())
    return false;

  return true;
}

// OutputAsText():
//  Outputs the header as ASCII text to the filename specified
bool TargaHeader::OutputAsText( const char *filename ) {
  ofstream out( filename );
  if( !out )
    return false;

  out << " Targa Header/Footer Information:" << endl;
  out << "  File Size:             " << filesize << endl;
  if( has_footer ) {
    out << "  Targa Version:         2.0+" << endl;
    out << "  Extension Offset:      " << extension_offset << endl;
    out << "   Extension Fixed Size: " << extension_fixed_size << endl;
    out << "  Developer Offset:      " << developer_offset << endl;
  } else {
    out << "  Targa Version:         1.0" << endl;
  }

  if( color_map_type )
    out << "  Color Map Offset:      " << color_map_offset << endl;

  out << "  Image Offset:          " << image_offset << endl;
  out << "  ID Length:             " << (int)id_length << endl;
  if( id_length > 0 ) {
    out << "    ID:                ";
    out.write( id, id_length );
  }
  out << "  Image Type:            " << (int)image_type << endl;

  out << endl;
  out << " Color Map Type          " << (int)color_map_type << endl;
  out << "  First Index:           " << first_index << endl;
  out << "  Num Indices:           " << num_indices << endl;
  out << "  Bits Per Pixel:        " << (int)palette_bits_per_pixel << endl;

  out << endl;
  out << " Image Information:      " << endl;
  out << "  X Offset:              " << x_offset << endl;
  out << "  Y Offset:              " << y_offset << endl;
  out << "  Width:                 " << width << endl;
  out << "  Height:                " << height << endl;
  out << "  Bits Per Pixel:        " << (int)bits_per_pixel << endl;

  out << endl;
  out << " Descriptor              " << endl;
  out << "  Attrib Bits/Pixel:     " << descriptor.attrib_bits_per_pixel  << endl;
  out << "  Order Left to Right:   " << descriptor.ordering_left_to_right << endl;
  out << "  Order Top to Bottom:   " << descriptor.ordering_top_to_bottom << endl;
  out << "  Reserved:              " << descriptor.reserved << endl;

  if( extension_offset > 0  ) {
    out << endl;
    out << " Extension Area          " << endl;
    out << "  Author Name:           " << author_name << endl;
    out << "   Author Comment:       " << author_comment << endl;
    out << "   Date Stamp:           " << date_stamp[0] << "/" << date_stamp[1] << "/" << date_stamp[2] << "  "
                                       << date_stamp[3] << ":" << date_stamp[4] << ":" << date_stamp[5] << endl;
    out << "  Job Name:              " << job_name << endl;
    out << "   Job Time:             " << job_time[0] << ":" << job_time[1] << ":" << job_time[2] << endl;

    out << "  Software ID:           " << software_id << endl;
    out << "   Software Version/Rev: " << software_version << software_subrevision << endl;

    out << "  Key Color:             " << key_color[0] << " " << key_color[1] << " " << key_color[2] << " "<< key_color[3] << endl;
    out << "  Pixel Aspect Ratio:    " << pixel_aspect_num << "/" << pixel_aspect_denom << endl;
    out << "  Gamma Correction:      " << gamma_num << "/" << gamma_denom << endl;

    out << endl;
    out << "  Color Correction Offset: " << color_correction_offset << endl;
    if( color_correction_offset != 0 ) {
      for( int i=0; i < 2048; i+=4 ) {
        out << "     " << color_correction[i] << "  " << color_correction[i + 1]
            << "  " << color_correction[i + 2] << "  " << color_correction[i + 3] << endl;
      }
    }

    out << endl;
    out << "  Postage Stamp Offset:  " << postage_stamp_offset << endl;
    if( postage_stamp_offset != 0 ) {
      out << "  Width:                 " << postage_stamp_width <<  endl;
      out << "  Height:                " << postage_stamp_height << endl;
    }

    out << endl;
    out << "  Scanline Table Offset: " << scanline_offset << endl;
    if( scanline_offset != 0 ) {
      for( int i = 0; i < height; i++ )
        out << "    " << scanlines[i] << endl;
    }
  }

  return true;
}

// SetCompression()
//  Sets the compression flag to the mode specified and
//   changes the image type to match.  Returns the current
//   compression level.
bool TargaHeader::SetCompression( bool compress ) {
  compression = compress;

  if( compression ) {
    if( image_type < 9 )
      image_type += 8;
  } else {
    if( image_type > 8 )
      image_type -= 8;
  }

  return compression;
}

// FillInImageData()
//  Fills in an ImageData class with values from a header instance.
//   This is called directly by the Load() function.
bool TargaHeader::FillInImageData( ImageData *data ) {
  // See if an ImageData class was supplied
  if( data == NULL )
    return false;

  data->SetFileFormat( IMGFF_TARGA );
  data->SetFileSize( filesize );
  data->SetBitsPerPixel( bits_per_pixel );
  data->SetPixelAspect( pixel_aspect_num, pixel_aspect_denom );
  data->SetGamma( gamma_num, gamma_denom );

  if( image_type > 8 )
    data->SetCompression( IMGCOMPRESSION_RLE );
  else
    data->SetCompression( IMGCOMPRESSION_NONE );

  data->SetAuthor(  author_name );
  data->SetComment( author_comment );
  data->SetCreator( software_id );
  data->SetJobName( job_name );
  data->SetJobTime( job_time );

  data->SetDateStamp( date_stamp );

  data->SetThumbnail( postage_stamp );
  
  data->SetFileData( this );

  return true;
}

// ---
// ------------------------------------- TargaDeveloper Member Functions ---
// ---

// Constructor
TargaDeveloper::TargaDeveloper() 
  : tag(0), offset(0), size(0), data(NULL) {
  ;
}

// Destructor
TargaDeveloper::~TargaDeveloper() {
  if( data != NULL )
    delete data;
}
