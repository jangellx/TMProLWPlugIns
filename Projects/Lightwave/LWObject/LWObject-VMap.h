//
// LWObject-VMap.h
//

#include <iomanip.h>

#include "lwbifstream.h"

#include "LWObject-Errors.h"
#include "LWObject-Chunks.h"

// LWObject_VMap
//  Contains basic information about a VMap, such as it's dimensions, name
//   and type.
class LWObject_VMap {
public:
  LWObject_VMap( const char *_name, unsigned long _type, unsigned short _dim );
  ~LWObject_VMap();

  // File I/O
  bool Read( LWbifstream &in, unsigned long chunk_size );

  // Accessors
  inline const char *   GetName()       { return name;       }
  inline unsigned long  GetType()       { return type;       }
  inline unsigned short GetDimensions() { return dimensions; }

protected:
  char *           name;             // Name
  unsigned long    type;             // Type of VMap (one of LWObject_VMapType_????)
  unsigned short   dimensions;       // Number of elements in a LWObject_PointVmap's values array
};


// LWObject_PointVMap
//  Contains point-specific VMap information, including a pointer
//   to the basic information and this point's VMAP values 
class LWObject_PointVMap {
public:
  LWObject_PointVMap( LWObject_VMap *_vmap );
  ~LWObject_PointVMap();

  // File I/O
  bool Read( LWbifstream &in );

  // Accessors
  inline LWObject_VMap  * GetVMap()       { return vmap; }
  inline unsigned short   GetDimensions() { return vmap->GetDimensions(); }
  inline float          * GetValues()     { return values; }

protected:
  LWObject_VMap  * vmap;             // Pointer to the basic vertex map information
  float          * values;           // Values array
};

