//
// LWObject_Polygon.h
//

#include "LWObject-Point.h"

#include <portable/dynarray/dynarray.h>

class LWObject_Layer;
class LWObject_PolygonPoint;

// class LWObject_Polygon
class LWObject_Polygon {
public:
  LWObject_Polygon( unsigned long _type );
  ~LWObject_Polygon();

  // File I/O
  bool Read( LWbifstream &in, LWObject_Layer &layer, unsigned long point_start );

  // Accessors
  inline unsigned long                         GetType() { return type; }
  inline void                                  SetType( unsigned long _type ) { type = _type; }

  inline unsigned short                        GetFlags() { return flags; }
  inline void                                  SetFlags( unsigned short _flags ) { flags = _flags; }

  inline DynArray< LWObject_PolygonPoint * > & GetPoints() { return points; }

  inline unsigned long                         GetSurfaceIndex() { return surface_index; }
  inline void                                  SetSurfaceIndex( unsigned long _index) { surface_index = _index; }

  inline unsigned long                         GetPartIndex() { return part_index; }
  inline void                                  SetPartIndex( unsigned long _index) { part_index = _index; }

  inline unsigned long                         GetSmoothingGroupIndex() { return smoothing_group_index; }
  inline void                                  SetSmoothingGroupIndex( unsigned long _index) { smoothing_group_index = _index; }

  void                                         OutputToStream( ostream &out );

protected:
  unsigned long                       type;                   // Polygon Type (one of LWObject_PolygonType_????)
  unsigned short                      flags;                  // high 6 bits are flags, currently only defined for CURVs

  DynArray< LWObject_PolygonPoint * > points;                 // Array of points and polygon-specific vertex maps (VMAD's)

  unsigned long                       surface_index;          // Surface Index
  unsigned long                       part_index;             // Part Index
  unsigned long                       smoothing_group_index;  // Smoothing Group Index
};

// class LWObject_PolygonPoint
class LWObject_PolygonPoint {
public:
  LWObject_PolygonPoint( LWObject_Point *_point = NULL ) : point(_point) {;}
  ~LWObject_PolygonPoint() { vmaps.Flush(); }

  inline LWObject_Point                   * GetPoint() { return point; }
  inline DynArray< LWObject_PointVMap * > & GetVMaps() { return vmaps; }

  void OutputToStream( ostream &out );

protected:
  LWObject_Point *point;

  DynArray< LWObject_PointVMap * > vmaps;   // VMAD vertex maps (discontinuous/polygon-level VMAPs)
};


