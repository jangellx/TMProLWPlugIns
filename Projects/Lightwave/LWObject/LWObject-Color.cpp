//
// LWObject_Color.cpp
//

#include "LWObject-Color.h"

// Constructor
LWObject_Color::LWObject_Color() {
  rgba[0] = rgba[1] = rgba[2] = 0.0f;
  rgba[3] = 1.0f;
}

// Destructor
LWObject_Color::~LWObject_Color() {
  ;
}

// Read():
bool LWObject_Color::Read( LWbifstream &in, bool read_alpha ) {
  in >> rgba[0] >> rgba[1] >> rgba[2];

  if( read_alpha )
    in >> rgba[3];
  else
    rgba[3] = 1.0f;

  return true;
}
