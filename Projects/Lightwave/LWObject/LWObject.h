//
// LWObject.h
//
// LWO2 (LW 6.0+) Lightwave Object Loader
//
// Based on the LWO2 specification from NewTek, and source code
//  by Ernie Write from http://members.home.net/erniew2/lwsdk/sample/lwobject/
//

#include "LWObject-Layer.h"

// LWObject
class LWObject {
public:
  LWObject();
  ~LWObject();

  // File I/O
  LWObjectErrorCode Read( const char *path );

  void OutputToStream( ostream &out );

  void LayerPostLoad( unsigned long current_layer, bool found_bbox );

  // Accessors
  const char * GetFilename() { return filename; }
  void         SetFilename( const char * _filename ) { StringTools::strdup2( filename, _filename ); }

  DynArray< char * >           & GetTags()   { return tags;   }
  DynArray< LWObject_Layer * > & GetLayers() { return layers; }

  // Static Functions
  inline const char * GetErrorString( LWObjectErrorCode code ) { return LWObjectErrorString[ code ]; }

protected:
  // Protected File I/O
  bool ReadTags( LWbifstream &in, unsigned long chunk_size );
  bool ReadPolygonTagMap( LWbifstream &in, unsigned long chunk_size,
                          LWObject_Layer &current_layer, unsigned long polygon_start );

  // Protected Members
  char                         * filename; // Object Filename, or NULL if none is specified

  DynArray< char * >             tags;     // Tags (strings stored in an LWO)
  DynArray< LWObject_Layer * >   layers;   // Layer List
};

