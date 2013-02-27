//
// LWObject_Point.cpp
//

#include "LWObject-Point.h"

// Constructor
LWObject_Point::LWObject_Point() {
  vector[0] = vector[1] = vector[2] = 0.0f;
}

// Destructor
LWObject_Point::~LWObject_Point() {
  vmaps.Flush();
}

// Read():
bool LWObject_Point::Read( LWbifstream &in ) {
  in >> vector[0] >> vector[1] >> vector[2];
  return true;
}

// OutputToStream():
void LWObject_Point::OutputToStream( ostream &out ) {
  out << "          Point:      " << *this << endl;
  out << "            Polygons: " << polygons.NumElements() << endl;

  if( polygons.NumElements() > 0 )
    out << "            ";

  for( unsigned long i=0; i < polygons.NumElements(); i++ )
    out << "  " << polygons[i];

  out << endl;
  out << "            VMAP's:   " << vmaps.NumElements() << endl;

  char buffer[5] = "????";
  unsigned long type, j;
  for( i=0; i < vmaps.NumElements(); i++ ) {
    type = vmaps[i]->GetVMap()->GetType();
    memcpy( buffer, &type, 4 );
    StringTools::Reverse( buffer );
    out << "                " << buffer << "  \""
        << ((vmaps[i]->GetVMap()->GetName() == NULL) ? "(unnamed)" : vmaps[i]->GetVMap()->GetName() )
        << "\"  [";

    for( j=0; j < vmaps[i]->GetVMap()->GetDimensions(); j++ )
      out << ((j == 0) ? " " : ", ") << vmaps[i]->GetValues()[j];

    out << " ]" << endl;
  }
}

