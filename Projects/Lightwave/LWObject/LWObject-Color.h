//
// LWObject_Color.h
//
// Colors are RGBA quads stored as floats, usually with values
//  between 0.0 and 1.0.  For the alpha value, 1.0 means solid
//  and 0.0 means transparent.
//

#ifndef HEADER_LWOBJECT_COLOR
#define HEADER_LWOBJECT_COLOR

#include <iomanip.h>

#include "lwbifstream.h"

// class LWObject_Color
class LWObject_Color {
public:
  LWObject_Color();
  ~LWObject_Color();

  // File I/O
  bool Read( LWbifstream &in, bool read_alpha );

  // Accessors
  inline float     GetR()           { return rgba[0]; }
  inline void      SetR( float _r ) { rgba[0] = _r;   }

  inline float     GetG()           { return rgba[1]; }
  inline void      SetG( float _g ) { rgba[1] = _g;   }

  inline float     GetB()           { return rgba[2]; }
  inline void      SetB( float _b ) { rgba[2] = _b;   }

  inline float     GetA()           { return rgba[3]; }
  inline void      SetA( float _a ) { rgba[3] = _a;   }

  inline float   * GetRGBA()        { return rgba;    }

  // Operators
  inline LWObject_Color & operator=( LWObject_Color other_point );
  inline ostream & operator<<( ostream &out );

protected:
  float rgba[4];
};

// operator=
inline LWObject_Color & LWObject_Color::operator=( LWObject_Color other_color ) {
  memcpy( rgba, other_color.GetRGBA(), (4 * sizeof( rgba[0] )) );
  return *this;
}

// operator<<( ofstream &, LWObject_Color & )
inline ostream & operator<<( ostream &out, LWObject_Color &color ) {
  out << "[" << setw(8) << color.GetR() << ","
             << setw(8) << color.GetG() << ","
             << setw(8) << color.GetB() << ","
             << setw(8) << color.GetA() << "]";
  return out;
}

#endif