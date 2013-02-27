//
// LWObject-Layer.h
//
// Objects consists of multiple layers.  Each LWO2 layer is loaded
//  as an individually animatable item within Layout.  Layers can
//  be through of sub-objects contained in a single file, including
//  inter-layer parenting, layer naming, and pivot point centering.
// 

#include <portable/stringtools/stringtools.h>

#include "LWObject-Polygon.h"
#include "LWObject-Surface.h"

const unsigned long LWObject_PTAGType_SURF = ('S'<<24) | ('U'<<16) | ('R'<<8) | 'F';
const unsigned long LWObject_PTAGType_PART = ('P'<<24) | ('A'<<16) | ('R'<<8) | 'T';
const unsigned long LWObject_PTAGType_SMGP = ('S'<<24) | ('M'<<16) | ('G'<<8) | 'P';

class LWObject;

// class LWObject_Layer
class LWObject_Layer {
public:
  LWObject_Layer();
  ~LWObject_Layer();

  // File I/O
  bool Read( LWbifstream &in, unsigned long chunk_size );
  bool ReadPolygonTagMap( LWbifstream &in, unsigned long chunk_size, unsigned long polygon_start );
  bool ReadVertexMap( LWbifstream &in, unsigned long chunk_size,
                      unsigned long point_start, unsigned long polygon_start, bool is_vmad );

  void OutputToStream( ostream &out );

  void ResolveDependancies( LWObject &object );

  // Accessors
  inline const char     * GetName() { return name; }
  inline void             SetName( const char *_name ) { StringTools::strdup2( name, _name ); }

  inline LWObject_Point   GetPivot()                         { return pivot; }
  inline LWObject_Point   SetPivot( LWObject_Point &_pivot ) { pivot = _pivot; }

  inline unsigned short   GetLayerNumber() { return number; }

  inline LWObject_Layer * GetParent()      { return parent; }
  inline unsigned short   GetParentIndex() { return parent_index; }

  inline unsigned short   GetFlags()                        { return flags; }
  inline void             SetFlags( unsigned short _flags ) { flags = _flags; }

  inline bool             GetIsHidden()             { return ((flags & 0x01) ? true : false); }
  inline void             SetIsHidden( bool state ) { flags = (state ? (flags | 0x01) : (flags & ~0x01)); }

  inline LWObject_Point * GetBoundingBox() { return bounding_box; }
         void             ComputeBoundingBox();

  inline DynArray< LWObject_Point   * > & GetPoints()   { return points;   }
  inline DynArray< LWObject_Polygon * > & GetPolygons() { return polygons; }
  inline DynArray< LWObject_Surface * > & GetSurfaces() { return surfaces; }
  inline DynArray< LWObject_VMap    * > & GetVMaps()    { return vmaps;    }

protected:
  char             * name;                  // Layer name, or NULL if the layer is "(unnamed)"

  unsigned short     number;                // Layer Number.  May NOT be the same as the index into the array, and there may be gaps
  unsigned short     flags;                 // Layer Flags.  Currently only "hidden"

  LWObject_Layer   * parent;                // Parent layer, or NULL if this layer is unparented
  unsigned short     parent_index;          // Parent Layer Index, or -1 if this layer is unparented

  LWObject_Point     pivot;                 // Pivot Point (rotational center) of the layer

  LWObject_Point     bounding_box[2];       // Points representing the bounding box.  The first point is the top near left, and the second is the bottom far right

  DynArray< LWObject_Point   * > points;    // Point Array
  DynArray< LWObject_Polygon * > polygons;  // Polygon Array
  DynArray< LWObject_Surface * > surfaces;  // Surface Array
  DynArray< LWObject_VMap    * > vmaps;     // VMap Array
};

