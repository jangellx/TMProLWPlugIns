//
// LWObject_Point.h
//
// NOTE:  LW uses a +Y-up coordinate system, with +Z pointing into
//        the screen and +X pointing to the right (ie: left-handed).
//  Up:    +Y    Down:   -Y
//  Away:  +Z    Toward: -Z
//  Right: +X    Left:   -X
//

#ifndef HEADER_LWOBJECT_POINT
#define HEADER_LWOBJECT_POINT

#include <portable/dynarray/dynarray.h>

#include "LWObject-VMap.h"

class LWObject_Polygon;

// class LWObject_Point
class LWObject_Point {
public:
  LWObject_Point();
  ~LWObject_Point();

  // File I/O
  bool Read( LWbifstream &in );

  void OutputToStream( ostream &out );

  // Accessors (Point)
  inline float     GetX()           { return vector[0]; }
  inline void      SetX( float _x ) { vector[0] = _x;   }

  inline float     GetY()           { return vector[1]; }
  inline void      SetY( float _y ) { vector[1] = _y;   }

  inline float     GetZ()           { return vector[2]; }
  inline void      SetZ( float _z ) { vector[2] = _z;   }

  inline float   * GetVector()      { return vector;    }

  inline DynArray< LWObject_Polygon   * > & GetPolygons() { return polygons; }
  inline DynArray< LWObject_PointVMap * > & GetVMaps()    { return vmaps;    }

  // Operators
  inline LWObject_Point & operator=( LWObject_Point other_point );
  inline ostream & operator<<( ostream &out );

protected:
  float vector[3];                                  // Point position relative to Layer Pivot Point
  
  DynArray< LWObject_Polygon   * > polygons;        // Polygons that this point belongs to
  DynArray< LWObject_PointVMap * > vmaps;           // Vertex maps applied to this point (VMAP's)
};

// operator=
inline LWObject_Point & LWObject_Point::operator=( LWObject_Point other_point ) {
  memcpy( vector, other_point.GetVector(), (3 * sizeof( vector[0] )) );
  return *this;
}

// operator<<( ofstream &, LWObject_Point & )
inline ostream & operator<<( ostream &out, LWObject_Point &point ) {
  out << "[" << setw(8) << point.GetX() << ","
             << setw(8) << point.GetY() << ","
             << setw(8) << point.GetZ() << "]";
  return out;
}

#endif