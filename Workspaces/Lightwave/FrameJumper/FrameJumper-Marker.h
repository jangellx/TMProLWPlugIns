//
// FrameJumper-Marker.h
//

#include <portable/stringtools/stringtools.h>

class FrameJumper_Marker {
public:
  FrameJumper_Marker() : name(NULL), key(0), time(0.0), enabled(true) { ; }
  ~FrameJumper_Marker() { StringTools::strdup2( name, NULL ); }

  // Accessors
  inline const char * GetName()                    { return name; }
  inline void         SetName( const char * _name) { StringTools::strdup2( name, _name ); }

  inline int          GetKey()                    { return key; }
  inline void         SetKey( int _key )          { key = _key; }

  inline double       GetTime()                    { return time; }
  inline void         SetTime( double _time )      { time = _time; }

  inline bool         GetEnabled()                 { return enabled; }
  inline void         SetEnabled( bool state )     { enabled = state; }

  inline FrameJumper_Marker & operator=( FrameJumper_Marker *other_group );
  inline FrameJumper_Marker & operator=( FrameJumper_Marker &other_group );

protected:
  char   * name;
  int      key;
  double   time;
  bool     enabled;
};

// operator=( pointer )
inline FrameJumper_Marker & FrameJumper_Marker::operator=( FrameJumper_Marker *other_group ) {
  SetTime(    other_group->GetTime()    );
  SetKey(     other_group->GetKey()     );
  SetName(    other_group->GetName()    );
  SetEnabled( other_group->GetEnabled() );

  return *this;
}

// operator=( reference )
inline FrameJumper_Marker & FrameJumper_Marker::operator=( FrameJumper_Marker &other_group ) {
  return *this = &other_group;
}
