//
// LWObject.cpp
//

#include <portable/dirtools/dirtools.h>

#include "LWObject.h"

// Constructor
LWObject::LWObject()
  : filename(NULL) {
}

// Destructor
LWObject::~LWObject() {
  tags.Flush();
  layers.Flush();

  SetFilename( NULL );
}

// Read():
//  Reads an LWO from disk.  
LWObjectErrorCode LWObject::Read( const char *path ) {
  if( path == NULL )
    return LWOERROR_INVALID_PATH;

  // Test for path existance
  if( !DirInfo::Exists( path ) )
    return LWOERROR_FILE_NOT_FOUND;

  // Open the file
  LWbifstream in( path );
  if( !in )
    return LWOERROR_ERROR_OPENING_FILE;

  SetFilename( path );

  // Read in the LWO2 header
  in.SetEndian( ENDIAN_MOTOROLA );
  unsigned long chunk_id;
  in >> chunk_id;
  if( chunk_id != LWObjectChunk_FORM )
    return LWOERROR_NOT_A_LWO;

  // Read in the file size
  unsigned long file_size;                          // Actually the file size minus 8, since the header isn't included here
  in >> file_size;

  // Read in the base IFF chunk
  in >> chunk_id;
  if( chunk_id == LWObjectChunk_LWOB )
    return LWOERROR_BAD_VERSION;

  if( chunk_id != LWObjectChunk_LWO2 )
    return LWOERROR_NOT_A_LWO;

  // Start reading in chunks
  streampos chunk_start;
  unsigned long chunk_size;

  unsigned long current_layer = -1;

  bool          found_bbox;             // true if a BBOX chunk was found for this layer
  unsigned long point_start   = -1;     // first point from the most recent PNTS chunk
  unsigned long polygon_start = -1;     // first polygon from the most recent POLS chunk

  try {
    while( !in.eof() ) {
      // Test for errors
      if( in.bad() || in.fail() )
        return LWOERROR_ERROR_READING_FILE;

      // Get the basic chunk data
      chunk_start = in.tellg();
      in >> chunk_id >> chunk_size;

      // Figure out what kind of chunk this is
      switch( chunk_id ) {
        case LWObjectChunk_TAGS:                    // TAGS
          ReadTags( in, chunk_size );
          break;

        case LWObjectChunk_LAYR:                    // LAYR
          LayerPostLoad( current_layer, found_bbox );

          GetLayers().Add( new LWObject_Layer );
          current_layer = GetLayers().NumElements() - 1;
          GetLayers()[ current_layer ]->Read( in, chunk_size );
          found_bbox = false;
          break;

        case LWObjectChunk_PNTS:                    // PNTS
          if( current_layer == -1 ) {
            in.seekg( chunk_size, ios::cur );
          } else {
            if( chunk_size > 0 )
              point_start = GetLayers()[ current_layer ]->GetPoints().NumElements();

            LWObject_Point *new_point;
            for( unsigned long i=0; i < chunk_size/12; i++ ) {
              new_point = new LWObject_Point;
              new_point->Read( in );
              GetLayers()[ current_layer ]->GetPoints().Add( new_point );
            }
          }
          break;

        case LWObjectChunk_POLS:                    // POLS
          if( current_layer == -1 ) {
            in.seekg( chunk_size, ios::cur );
          } else {
            LWObject_Polygon *new_polygon;
            long chunk_start = in.tellg();
            unsigned long type;
            in >> type;

            if( chunk_size > 4 )
              polygon_start = GetLayers()[ current_layer ]->GetPolygons().NumElements();

            for( unsigned long chunk_pos = 0; chunk_pos < chunk_start + chunk_size; chunk_pos = in.tellg() ) {
              new_polygon = new LWObject_Polygon( type );
              new_polygon->Read( in, *GetLayers()[ current_layer ], point_start );
              GetLayers()[ current_layer ]->GetPolygons().Add( new_polygon );
            }
          }
          break;

        case LWObjectChunk_PTAG:                    // PTAG
          if( current_layer == -1 )
            in.seekg( chunk_size, ios::cur );
          else
            GetLayers()[ current_layer ]->ReadPolygonTagMap( in, chunk_size, polygon_start );
          break;

        case LWObjectChunk_SURF:                    // SURF
          if( current_layer == -1 ) {
            in.seekg( chunk_size, ios::cur );
          } else {
            LWObject_Surface *new_surface = new LWObject_Surface();
            new_surface->Read( in, chunk_size );
            GetLayers()[ current_layer ]->GetSurfaces().Add( new_surface );
          }
          break;

        case LWObjectChunk_VMAP:                    // VMAP
        case LWObjectChunk_VMAD:                    // VMAD
          if( current_layer == -1 ) {
            in.seekg( chunk_size, ios::cur );
          } else {
            GetLayers()[ current_layer ]->ReadVertexMap( in, chunk_size, point_start, polygon_start,
                                                         ((chunk_id == LWObjectChunk_VMAD) ? true : false) );
          }
          break;

        case LWObjectChunk_BBOX:                    // BBOX
          GetLayers()[ current_layer ]->GetBoundingBox()[0].Read( in );
          GetLayers()[ current_layer ]->GetBoundingBox()[1].Read( in );
          found_bbox = true;
          break;

        default:                                    // Unknown Chunk
          in.seekg( chunk_size, ios::cur );
      }

      // Skip another byte if the chunk size is odd (chunks must be even sized)
      if( (chunk_size % 2) > 0 )
        in.seekg( 1, ios::cur );
    }
  } catch( LWObjectErrorCode code ) {
    // Error Handling
    return code;
  }

  LayerPostLoad( current_layer, found_bbox );

  return LWOERROR_OK;
}

// ReadTags():
//  Reads the tags from an LWO.  
bool LWObject::ReadTags( LWbifstream &in, unsigned long chunk_size ) {
  if( chunk_size == 0 )
    return true;

  streampos chunk_start = in.tellg();
  char      buffer[256];

  while( in.tellg() < (long)(chunk_start + chunk_size) ) {
    // Read the string
    in.GetLWString( buffer, 256 );

    // Add the string to the buffer
    int len = strlen( buffer );
    tags.Add( StringTools::strdup( buffer ) );

    // Error Test
    if( in.bad() || in.fail() || (in.tellg() > (long)(chunk_start + chunk_size)) )
      throw LWOERROR_BAD_TAGS;
  }

  return true;
}

// LayerPostLoad():
//  Performs resolving operations on a layer after all it's data has 
//   been loaded.
void LWObject::LayerPostLoad( unsigned long current_layer, bool found_bbox ) {
  if( current_layer == -1 )
    return;

  if( !found_bbox )
    GetLayers()[ current_layer ]->ComputeBoundingBox();

  GetLayers()[ current_layer ]->ResolveDependancies( *this );
}

// OutputToStream():
//  Outputs some debugging-like data to the straem provided
void LWObject::OutputToStream( ostream &out ) {
  out << "LWO2 Object \"" << ((filename == NULL) ? "(unnamed)" : filename) << "\"" << endl;

  // Tag List
  out << "-- Tag List (" << GetTags().NumElements() << " tags)" << endl;
  for( unsigned long i=0; i < GetTags().NumElements(); i++ )
    out << "    \"" << GetTags()[i] << "\"" << endl;

  // Layers
  for( i=0; i < GetLayers().NumElements(); i++ )
    GetLayers()[i]->OutputToStream( out );
}

