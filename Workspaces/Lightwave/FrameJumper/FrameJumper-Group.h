//
// FrameJumper-Group.h
//

#include "FrameJumper-Marker.h"

// class FrameJumper_Group
class FrameJumper_Group {
public:
  FrameJumper_Group() : name(NULL), current_marker(-1) { ; }
  ~FrameJumper_Group() { StringTools::strdup2( name, NULL ); markers.Flush(); }

  // Mangement
  inline unsigned long CountEnabledMarkers();

  inline FrameJumper_Marker * FindMarkerWithKey( int key );
  inline FrameJumper_Marker * FindMarkerWithTime( double time );

  // Accessors
  inline const char * GetName()                    { return name; }
  inline void         SetName( const char * _name) { StringTools::strdup2( name, _name ); }

  inline int          GetCurrentMarker()           { return current_marker; }
  inline void         SetCurrentMarker( int _cur ) { current_marker = _cur; }

  inline DynArray< FrameJumper_Marker * > & GetMarkers() { return markers; }

protected:
  char                             * name;
  int                                current_marker;
  DynArray< FrameJumper_Marker * >   markers;
};

// CountEnabledMarkers():
inline unsigned long FrameJumper_Group::CountEnabledMarkers() {
  unsigned long count = 0;
  for( unsigned long i=0; i < markers.NumElements(); i++ ) {
    if( markers[i]->GetEnabled() )
      count++;
  }

  return count;
}

// FindMarkerWithKey():
inline FrameJumper_Marker * FrameJumper_Group::FindMarkerWithKey( int key ) {
  for( unsigned long i=0; i < GetMarkers().NumElements(); i++ ) {
    if( GetMarkers()[i]->GetKey() == key )
      return GetMarkers()[i];
  }

  return NULL;
}

// FindMarkerWithTime():
inline FrameJumper_Marker * FrameJumper_Group::FindMarkerWithTime( double time ) {
  for( unsigned long i=0; i < GetMarkers().NumElements(); i++ ) {
    if( GetMarkers()[i]->GetTime() == time )
      return GetMarkers()[i];
  }

  return NULL;
}

