//
// ImageLister.cpp
//
// ImageLister simply lists the images within an object, or all
//  objects in a scene.  Simple enough...
//

#include <portable/bfstream/bfstream.h>
#include <portable/dynarray/dynarray.h>
#include <portable/dirtools/dirtools.h>

enum IL_File_Types {
  ILFT_UNKNOWN = -1,
  ILFT_ERROR   =  0,
  ILFT_SCENE   =  1,
  ILFT_OBJECT  =  2
};

IL_File_Types GuessFileType( const char * source );
bool          ProcessScene( ofstream &out, const char *filename, const char *content );
bool          ProcessObject( ofstream &out, const char *filename );
void          ReadSurfaceList( bifstream &in, long chunk_size, DynArray< char *> &surface_names );
void          ReadSurfaceData( bifstream &in, long chunk_size, ofstream &out );
void          Help(void);

// Argument Template
//  ImageLister <source> [-c <content>] [-o <output>]
//   <source>        Source object/scene to process
//   [-c <content>]  Content Directory used for this scene/object
//   [-o <output>]   The name of the text file that will be written.

const char *ImageLister_Version  = "1";
const char *ImageLister_Revision = "0";

void main( int argc, char *argv[] ) {
  cout << " ImageLister V" << ImageLister_Version << "." << ImageLister_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Check for too few arguments
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Set up some variables
  char * source = argv[1];                    // Source Path
  char   output[ MAX_PATH_LENGTH ] = "";      // Output Path
  char   content[ MAX_PATH_LENGTH ] = "";     // Content Dir

  cerr << "-Settings..." << endl;
  cerr << "  Source File will be \"" << source << "\"" << endl;
  for( int i=2; i < argc; i++ ) {
    if( argv[i][0] == '-' ) {
      switch( argv[i][1] ) {
      case 'c':
        // Content Dir Test
        if( ++i >= argc ) {
          cerr << " ERROR: No content dir following argument -c!" << endl;
          cerr << " Type ImageLister ? for help" << endl;
          exit(20);
        }
        strcpy( content, argv[i] );
        cerr << "  Content Dir will be \"" << content << "\"" << endl;
        break;

      case 'o':
        // Output File Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          cerr << " Type ImageLister ? for help" << endl;
          exit(20);
        }
        strcpy( output, argv[i] );
        cerr << "  Output File will be \"" << output << "\"" << endl;
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type ImageLister ? for help" << endl;
        exit(20);
      }
    } else {
      cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
      cerr << " Type ImageLister ? for help" << endl;
      exit(20);
    }
  }

  // Create the output path, if needed
  if( output[0] == '\0' ) {
    strcpy( output, source );
    char * ext = DirStrings::HasExtension( output ); // Find any extension
    if( ext )                                        // Strip the extension
      *ext = '\0';
    strcat( output, "-ImageList.txt" );              // Append -ImageList.txt
  } 

  // Test the paths
  if( !DirInfo::Exists( source ) ) {
    cerr << " ERROR: The source file \"" << source << "\" doesn't exist!" << endl;
    cerr << " Type ImageLister ? for help" << endl;
    exit(20);
  }  

  if( stricmp( source, output ) == 0 ) {
    cerr << " ERROR: The source and output files cannot be the same!" << endl;
    cerr << " Type ImageLister ? for help" << endl;
    exit(20);
  }

  // Figure out if this is a scene, object or unknown.
  int file_type = GuessFileType( source );
  switch( file_type ) {
    // Error opening source file
    case ILFT_ERROR:
      cerr << " ERROR: Unable to open source file \"" << source << "\"; aborting" << endl;
      exit(20);

    // Unknown file type
    case ILFT_UNKNOWN:
      cerr << " ERROR: Source file \"" << source << "\" is not a know Lightwave Scene or Object file; aborting" << endl;
      cerr << " Type ImageLister ? for help" << endl;
      exit(20);
  }

  // Open the output file
  ofstream out( output );
  if( !out ) {
    cerr << " ERROR: Unable to open output scene \"" << output << "\"!" << endl;
    cerr << " File may be in use by another program or write-protected; aborting" << endl;
    exit(20);
  }

  // Process the file
  switch( file_type ) {
  case ILFT_OBJECT:    // Object File
    ProcessObject( out, source );
    break;

  case ILFT_SCENE:     // Scene File
    ProcessScene( out, source, content );
    break;
  }

  cout << "- Operation Complete!" << endl;
  cout << " Image list has been output to \"" << output << "\"" << endl;
}

// GuessFileType():
//  Guesses the type of file.  Returns one of the IL_FileType enums.
IL_File_Types GuessFileType( const char * source ) {
  bifstream in( source, ios::nocreate );
  if( !source )
    return ILFT_ERROR;

  // Read in the header ID
  char signature[4];
  in.read( signature, 4 );

  if( strncmp( signature, "LWSC", 4 ) == 0 )
    return ILFT_SCENE;

  if( strncmp( signature, "FORM", 4 ) == 0 ) {
    long file_size;
    char id[4];

    in >> file_size;
    in.read( id, 4 );
    if( strncmp( id, "LWOB", 4 ) == 0 )
      return ILFT_OBJECT;
  }

  return ILFT_UNKNOWN;
}

// ProcessScene():
//  Scans all the objects in a scene (and the scene itself) and outputs a list
//   of the image maps it uses
bool ProcessScene( ofstream &out, const char *filename, const char *content ) {
  // Make sure this is really a Lightwave Scene File
  ifstream in( filename, ios::nocreate );
  if( !in ) {
    cerr << "  ERROR: Unable to open scene \"" << filename << "\" for processing; aborting" << endl;
    return false;
  }

  char signature[4];
  in.read( signature, 4 );
  if( strncmp( signature, "LWSC", 4 ) != 0 ) {
    out << "    ERROR:  Not a supported Lightwave Scene Format; aborting." << endl;
    return false;
  }

  // Search Loop
  int  num_tex;
  char line[8192];
  char texture[512];
  char used_by[512];
  char object[512];

  do {
    in.getline( line, 8192 );

    // Check for errors
    if( (in.bad() || in.fail()) && !in.eof() ) {
      out << "    ERROR:  Failed to read from file; skipping object" << endl;
      cerr << "  ERROR:  Failed to read from file; skipping object." << endl;
      return false;
    }

    // Test for an object to process
    if( strncmp( line, "LoadObject", 10 ) == 0 ) {
      strcpy( object, content );
      DirStrings::AddPathPart( object, &(line[11]) );
      ProcessObject( out, object );
    }

    // Check for Scene-Level Object Maps
    if( strncmp( line, "DisplacementMap", 15 ) == 0 ) {
      num_tex = 0;
      strcpy( used_by, "Displacement Map" );
      strcpy( texture, &(line[15]) );
    }

    if( strncmp( line, "ClipMap", 8 ) == 0 ) {
      num_tex = 0;
      strcpy( used_by, "Clip Map" );
      strcpy( texture, &(line[8]) );
    }

    if( strncmp( line, "TextureImage", 12 ) == 0 ) {
      if( num_tex++ == 0 )
        out << "  " << used_by << ", Layer " << num_tex << ":" << endl;

      out << "    " << texture << ":  " << &(line[12]) << endl;
    }    

    // Test for a Light to process
    if( strncmp( line, "LightName", 9 ) == 0 ) {
      num_tex = 0;
      strcpy( object, &(line[10]) );
    }

    // Check for Projection Light Maps
    if( strncmp( line, "LightProjImage", 14 ) == 0 ) {
      if( num_tex++ == 0 )
        out << "  Light:  \"" << object << "\"" << endl;
      out << "      Light Projection Image:  \"" << &(line[15]) << "\"" << endl;
    }

    // Check for Effects Panel Maps
    if( strncmp( line, "BGImage", 7 ) == 0 )
      out << "  Background Image:  \"" << &(line[8]) << "\"" << endl;

    if( strncmp( line, "FGImage", 7 ) == 0 )
      out << "  Foreground Image:  \"" << &(line[8]) << "\"" << endl;

    if( strncmp( line, "FGAlphaImage", 12 ) == 0 )
      out << "  Forgrounf Alpha Image:  \"" << &(line[13]) << "\"" << endl;

  } while( !in.eof() );

  return true;
}


// ProcessObject():
//  Scans a single object and outputs a list of the image maps it
//   uses
bool ProcessObject( ofstream &out, const char *filename ) {
  bifstream in( filename, ios::nocreate );
  if( !in ) {
    cerr << "  ERROR: Unable to open object \"" << filename << "\" for processing; skipping" << endl;
    return false;
  }
  in.SetEndian( ENDIAN_MOTOROLA );

  out << "  Object:  \"" << filename << "\"" << endl;
  cout << "  Object:  \"" << filename << "\"" << endl;

  // Make sure it's an IFF file
  char signature[4];
  in.read( signature, 4 );
  if( strncmp( signature, "FORM", 4 ) != 0 ) {
    out << "    ERROR:  Not a supported Lightwave Object Format; skipped." << endl;
    cerr << "  ERROR: \"" << filename << "\" is not a supported Lightwave Object; skipping" << endl;
    return false;
  }

  // Read in the object's file size
  unsigned long file_size = 0;
  in >> file_size;

  // Make sure it's a Lightwave Object
  in.read( signature, 4 );
  if( strncmp( signature, "LWOB", 4 ) != 0 ) {
    out << "    ERROR:  Not a supported Lightwave Object Format; skipped." << endl;
    cerr << "  ERROR: \"" << filename << "\" is not a supported Lightwave Object; skipping" << endl;
    return false;
  }

  // Set up some variables
  char chunk_id[4];
  long chunk_size;
  DynArray< char * > surface_names;

  // Scan the object's chunks
  do {
    // Check for errors
    if( (in.bad() || in.fail()) && !in.eof() ) {
      out << "    ERROR:  Failed to read from file; skipping object" << endl;
      cerr << "  ERROR:  Failed to read from file; skipping object." << endl;
      return false;
    }

    in.read( chunk_id, 4 );
    in >> chunk_size;

    if( in.eof() )
      break;

    if( strncmp( chunk_id, "SRFS", 4 ) == 0 ) {      // Surface Name Chunk
      ReadSurfaceList( in, chunk_size, surface_names );
    } else if( strncmp( chunk_id, "SURF", 4 ) == 0 ) {
      ReadSurfaceData( in, chunk_size, out );
    } else {                                         // Unknown Chunk; skip it
      in.seekg( chunk_size, ios::cur );
   }
  } while( !in.eof() );

  // Clean-Up

  // Free the surface list
  for( unsigned int i = 0; i < surface_names.NumElements(); i++ )
    free( surface_names[i] );

  return true;
}

// ReadSurfaceList():
//  Read the surface list from the object file.  Assumes
//   that the streampos is at the start of the SRFS chunk.
void ReadSurfaceList( bifstream &in, long chunk_size, DynArray< char *> &surface_names ) {
  char   buffer[1024];
  int    loc = 0;

  for( long i=0; i < chunk_size; i++ ) {
    in >> buffer[ loc ];                        // Read in a character
    if( buffer[ loc ] == '\0' ) {               // See if it's the end of the string
      surface_names.Add( strdup( buffer ) );    // Copy the surface name and add it to the array
      if( loc % 2 == 0 ) {                      // Handle the pad byte, if needed
        in >> buffer[ 0 ];
        i++;
      }

      loc = 0;                                  // Reset the loc
    } else {
      loc++;                                    // Increment the location
    }
  }

//#define DEBUG_SURFACE_LIST
#ifdef  DEBUG_SURFACE_LIST
  // Output the surface list to the screen for debugging
  cout << "    Surface List:" << endl;
  for( unsigned int j=0; j < surface_names.NumElements(); j++ )
    cout << "      " << surface_names[j] << endl;
#endif
}

// ReadSurfaceData():
//  Read the surface data from the object file.  Assumes
//   that the streampos is at the start of the SURF chunk.
void ReadSurfaceData( bifstream &in, long chunk_size, ofstream &out ) {
  char  subchunk_id[4];
  short subchunk_size;

  char channel[ 256 ];
  char texture[ 256 ];
  char image[ 1024 ];

  int num_tex = 0;
  streampos chunk_start = in.tellg();

  // Read in the surface name
  char surface_name[1024];
  for( long i=0; i < 1024; i++ ) {
    in >> surface_name[ i ];                    // Read in a character
    if( surface_name[ i ] == '\0' ) {           // See if it's the end of the string
      if( i % 2 == 0 )                          // Handle the pad byte, if needed
        in >>surface_name[ i ];
      break;
    }
  }

  out << "    Surface Name:  \"" << surface_name << "\"" << endl;

  // Sub-Chunk Loop
  do {
    in.read( subchunk_id, 4 );
    in >> subchunk_size;

    if( strncmp( subchunk_id, "CTEX", 4 ) == 0 ) {         // Color Map
      strcpy( channel, "Color Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "DTEX", 4 ) == 0 ) {  // Diffuse Map
      strcpy( channel, "Diffuse Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "STEX", 4 ) == 0 ) {  // Specular Map
      strcpy( channel, "Specular Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "RTEX", 4 ) == 0 ) {  // Reflection Map
      strcpy( channel, "Reflection Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "TTEX", 4 ) == 0 ) {  // Transparency Map
      strcpy( channel, "Transparency Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "LTEX", 4 ) == 0 ) {  // Luminosity Map
      strcpy( channel, "Luminosity Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "BTEX", 4 ) == 0 ) {  // Bump Map
      strcpy( channel, "Bump Map" );
      in.read( texture, subchunk_size );
      num_tex = 0;
    } else if( strncmp( subchunk_id, "TIMG", 4 ) == 0 ) {  // Texture Map Image Filename
      in.read( image, subchunk_size );
      if( num_tex++ == 0 )
        out << "      " << channel << ", Layer " << num_tex << ":" << endl;
      out << "        " << texture << ":  \"" << image << "\"" << endl;
    } else if( strncmp( subchunk_id, "TALP", 4 ) == 0 ) {  // Texture Map Alpha Image Filename
      in.read( image, subchunk_size );
      if( num_tex == 0 )
        out << "      " << channel << ", Layer " << num_tex << ":" << endl;
      out << "        Alpha:  \"" << image << "\"" << endl;
    } else if( strncmp( subchunk_id, "RIMG", 4 ) == 0 ) {  // Reflection Image Filename
      in.read( image, subchunk_size );
      out << "      Reflection Image:  \"" << image << "\"" << endl;
    } else {
      in.seekg( subchunk_size, ios::cur );
    }

  } while( (in.tellg() - chunk_start) < chunk_size );
}

// Help():
void Help(void) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Lists all of the images found in an object/scene.\n" << endl;
  cout << endl;
  cout << "  Usage:   ImageLister <source> [-c[ontent] <content>] [-o[utput] <output>]" << endl;
  cout << "    <source>:               source object/scene filename and path." << endl; 
  cout << "   [-c[ontent] <content>]:  content directory used for source scene/object" << endl;
  cout << "   [-o[utput] <output>]:    optional output name." << endl;

  exit(0);
}
