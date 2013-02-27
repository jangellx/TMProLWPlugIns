//
// LWObject-VMap.cpp
//

#include <portable/stringtools/stringtools.h>

#include "LWObject-VMap.h"

// Constructor
LWObject_VMap::LWObject_VMap( const char *_name, unsigned long _type, unsigned short _dim )
  : name(NULL), type(_type), dimensions(_dim) {
  StringTools::strdup2( name, _name );
}

// Destructor
LWObject_VMap::~LWObject_VMap() {
  StringTools::strdup2( name, NULL );
}



// Constructor
LWObject_PointVMap::LWObject_PointVMap( LWObject_VMap *_vmap )
  : vmap(_vmap) {
  if( vmap->GetDimensions() == 0 )
    values = NULL;
  else
    values = new float[ vmap->GetDimensions() ];
}

// Destructor
LWObject_PointVMap::~LWObject_PointVMap() {
  if( values != NULL )
    delete[] values;
}

