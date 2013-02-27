//
// LWObject_Polygon.cpp
//

#include "LWObject-Layer.h"

// Constructor
LWObject_Polygon::LWObject_Polygon( unsigned long _type )
  : flags(0), type(_type) {
  ;
}

// Destructor
LWObject_Polygon::~LWObject_Polygon() {
  points.Flush();
}

// Read():
bool LWObject_Polygon::Read( LWbifstream &in, LWObject_Layer &layer, unsigned long point_start ) {
  unsigned short count;
  in >> count;

  flags = count & 0xFC00;  // High 6 bits are flags
  count &= 0x03FF;         // Remove the flag bits from the count

  points.SetSize( count );
  LWObject_Point * current_point;
  for( unsigned short i=0; i < count; i++ ) {
    current_point = layer.GetPoints()[ point_start + in.ReadVX() ];
    current_point->GetPolygons().Add( this );
    points.Add( new LWObject_PolygonPoint( current_point ) );
  }

  return true;
}

// OutputToStream():
void LWObject_Polygon::OutputToStream( ostream &out ) {
  long old_flags = out.flags();

  out << "     ";
  switch( GetType() ) {
    case LWObject_PolygonType_FACE:   out << setw(8) << "Face";     break;
    case LWObject_PolygonType_CURV:   out << setw(8) << "Curve";    break;
    case LWObject_PolygonType_PTCH:   out << setw(8) << "Patch";    break;
    case LWObject_PolygonType_MBAL:   out << setw(8) << "Metaball"; break;
    case LWObject_PolygonType_BONE:   out << setw(8) << "Bone";     break;
    default:                          out << setw(8) << "Unknown";  break;
  }

  out << "  Flags: "          << setw(8) << GetFlags() << "  ";
  out << "Surface: "          << setw(3) << GetSurfaceIndex() << "  ";
  out << "Part: "             << setw(3) << GetPartIndex()<< "  ";
  out << "Smoothing Group: "  << setw(3) << GetSmoothingGroupIndex() << endl;
  out << "          Points: " << setw(6) << GetPoints().NumElements() << endl;

  for( unsigned long i=0; i < GetPoints().NumElements(); i++ )
    GetPoints()[i]->OutputToStream( out );

  out << setiosflags( old_flags );
}

// OutputToStream():
void LWObject_PolygonPoint::OutputToStream( ostream &out ) {
  out << "            Point: " << *point << "  " << point << endl;
  out << "              VMAD\'s: " << vmaps.NumElements() << endl;;

  char buffer[5] = "????";
  unsigned long type;
  for( unsigned long i=0; i < vmaps.NumElements(); i++ ) {
    type = vmaps[i]->GetVMap()->GetType();
    memcpy( buffer, &type, 4 );
    StringTools::Reverse( buffer );
    out << "                " << vmaps[i] << "  " << buffer << "  \""
        << ((vmaps[i]->GetVMap()->GetName() == NULL) ? "(unnamed)" : vmaps[i]->GetVMap()->GetName() )
        << "\"" << endl;
  }
}
