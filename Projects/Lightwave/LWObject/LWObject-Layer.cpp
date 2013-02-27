//
// LWObject-Layer.h
//

#include "LWObject-Layer.h"

// Constructor
LWObject_Layer::LWObject_Layer()
  : name(NULL), number(0), flags(0), parent(NULL), parent_index(-1) {

};

// Destructor
LWObject_Layer::~LWObject_Layer() {
  SetName(NULL);

  points.Flush();
  polygons.Flush();
  surfaces.Flush();
  vmaps.Flush();
};


// Read():
bool LWObject_Layer::Read( LWbifstream &in, unsigned long chunk_size ) {
  if( chunk_size == 0 )
    return true;

  streampos chunk_start = in.tellg();

  // Layer Number
  in >> number;

  // Layer Flags
  in >> flags;

  // Layer Pivot
  pivot.Read( in );

  // Layer Name
  char buffer[256];
  in.GetLWString( buffer, 256 );
  SetName( buffer );

  // Layer Parent (optional)
  if( in.tellg() == (long)(chunk_start + chunk_size) - 1 ) // Use the chunk size to determine if the parent flag exists
    in >> parent_index;

  if( in.bad() || in.fail() || (in.tellg() > (long)(chunk_start + chunk_size)) )
    throw LWOERROR_BAD_LAYR;

  return true;
}

// ReadPolygonTagMap():
//  Reads a PTAG chunk for the current layer and assigns 
bool LWObject_Layer::ReadPolygonTagMap( LWbifstream &in, unsigned long chunk_size,
                                        unsigned long polygon_start ) {
  if( chunk_size == 0 )
    return true;

  streampos chunk_start = in.tellg();
  unsigned long tag_type;
  in >> tag_type;

  LWObject_Polygon *polygon;
  unsigned short tag;

  while( in.tellg() < (long)(chunk_start + chunk_size) ) {
    polygon = polygons[ in.ReadVX() + polygon_start ];
    in >> tag;

    switch( tag_type ) {
      case LWObject_PTAGType_SURF:  polygon->SetSurfaceIndex(        tag );  break;
      case LWObject_PTAGType_PART:  polygon->SetPartIndex(           tag );  break;
      case LWObject_PTAGType_SMGP:  polygon->SetSmoothingGroupIndex( tag );  break;

      default:  // Unknown PTAG type; skip out
        in.seekg( chunk_start + chunk_size, ios::beg );
        return true;
    } 
  }

  return true;
}

// OutputToStream():
//  Dumps some debugging-like output to the stream provided
void LWObject_Layer::OutputToStream( ostream &out ) {
  out << "-- Layer " << number << ": \"" << ((name == NULL) ? "(unnamed)" : name) << "\"" << endl;
  out << "    Parent Index:  " << parent_index << endl;
  out << "    Flags:         " << flags << "  " << (GetIsHidden() ? "(hidden)" : "") << endl;
  out << "    Pivot:         " << pivot << endl;

  out << "   Bounding Box:   " << bounding_box[0] << endl;
  out << "                   " << bounding_box[1] << endl;

  out << "   Points:" << endl;
  for( unsigned long i=0; i < points.NumElements(); i++ )
    points[i]->OutputToStream( out );

  out << "   Polygons:" << endl;
  for( i=0; i < polygons.NumElements(); i++ )
    polygons[i]->OutputToStream( out );

  out << "   Surfaces:" << endl;
  for( i=0; i < surfaces.NumElements(); i++ )
    surfaces[i]->OutputToStream( out );
}

// ResolveDependancies():
//  Updates references into arrays by index to acutal pointers to
//   the element in the array.
void LWObject_Layer::ResolveDependancies( LWObject &object ) {
  ;
}

// ComputeBoundingBox():
void LWObject_Layer::ComputeBoundingBox() {
  bounding_box[0].GetVector()[0] = bounding_box[0].GetVector()[1] = bounding_box[0].GetVector()[2] = 1e20f;
  bounding_box[1].GetVector()[0] = bounding_box[1].GetVector()[1] = bounding_box[1].GetVector()[2] = 1e20f;

  unsigned long i, j;
  for( i = 0; i < points.NumElements(); i++ ) {
    for( j = 0; j < 3; j++ ) {
      if( bounding_box[0].GetVector()[j] > points[i]->GetVector()[j] )
        bounding_box[0].GetVector()[j] = points[i]->GetVector()[j];
      if( bounding_box[1].GetVector()[j] < points[i]->GetVector()[j] )
        bounding_box[1].GetVector()[j] = points[i]->GetVector()[j];
    }
  }
}

// ReadVertexMap():
//  Reads both straight and discontinuous vertex maps (VMAP and VMAD).
//   Values are assigned directly to the points that belong to the map,
//   but common VMap data (name, type, etc) is stored in the layer's
//   VMap array.
bool LWObject_Layer::ReadVertexMap( LWbifstream &in, unsigned long chunk_size,
                                    unsigned long point_start, unsigned long polygon_start,
                                    bool is_vmad ) {
  streampos chunk_start = in.tellg();

  // Read the basic VMap data
  unsigned long vmap_type;
  in >> vmap_type;

  unsigned short vmap_dimensions;
  in >> vmap_dimensions;

  char vmap_name[256];
  in.GetLWString( vmap_name, 256 );

  LWObject_VMap *vmap = NULL;

  // See if this VMap already exists
  for( unsigned long i=0; i < vmaps.NumElements(); i++ ) {
    if( vmap_type == vmaps[i]->GetType() ) {
      if( stricmp( vmap_name, vmaps[i]->GetName() ) == 0 )
        vmap = vmaps[i];
    }
  }

  if( vmap == NULL ) {
    vmap = new LWObject_VMap( vmap_name, vmap_type, vmap_dimensions );
    vmaps.Add( vmap );
  }

  // Read in each set of VMap data and apply it to the appropriate point
  LWObject_Point        *point;
  LWObject_Polygon      *polygon;
  LWObject_PolygonPoint *polypoint;
  LWObject_PointVMap    *vmap_point;

  while( in.tellg() < (long)(chunk_start + chunk_size) ) {
    // See if the VMap already exists for this point; if so, overwrite it
    vmap_point = NULL;
    if( is_vmad ) {  // VMAD
      polygon   = polygons[ in.ReadVX() + polygon_start ];
      point     = points[ in.ReadVX() ];

      // Find the point in this polygon
      polypoint = NULL;
      for( i=0; i < polygon->GetPoints().NumElements(); i++ ) {
        if( polygon->GetPoints()[i]->GetPoint() == point ) {
          polypoint = polygon->GetPoints()[i];
          break;
        }
      }
      assert( polypoint != NULL );

      // Look for or create a new VMap Point
      for( i=0; i < polypoint->GetVMaps().NumElements(); i++ ) {
        if( vmap == polypoint->GetVMaps()[i]->GetVMap() ) {
          vmap_point = polypoint->GetVMaps()[i];
          break;
        }
      }

      if( vmap_point == NULL ) {
        vmap_point = new LWObject_PointVMap( vmap );
        polypoint->GetVMaps().Add( vmap_point );
      }
    } else {         // VMAP
      point = points[ in.ReadVX() + point_start ];

      // Look for or create a new VMap Point
      for( i=0; i < point->GetVMaps().NumElements(); i++ ) {
        if( vmap == point->GetVMaps()[i]->GetVMap() ) {
          vmap_point = point->GetVMaps()[i];
          break;
        }
      }

      if( vmap_point == NULL ) {
        vmap_point = new LWObject_PointVMap( vmap );
        point->GetVMaps().Add( vmap_point );
      }
    }

    // Read in the map data
    for( i=0; i < vmap->GetDimensions(); i++ )
      in >> vmap_point->GetValues()[i];
  }

  return true;
}

