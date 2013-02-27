//
// ReplaceObjects-SwapObject.h
//

#include <portable/dirtools/dirtools.h>

// class ReplaceObjects_SwapObject
class ReplaceObjects_SwapObject {
public:
  ReplaceObjects_SwapObject() : use(true), full_path(NULL), filename(NULL), weight(1.0) { ; }
  ~ReplaceObjects_SwapObject() { SetFullPath(NULL); }

  // Accessors
  inline bool         GetUse() { return use; }
  inline void         SetUse( bool _state ) { use = _state; }

  inline const char * GetFullPath() { return full_path; }
  inline void         SetFullPath( const char *_path ) { StringTools::strdup2( full_path, _path );
                                                         filename = (full_path == NULL) ? NULL : DirStrings::HasFile( full_path ); }

  inline const char * GetFilename() { return filename; }

  inline double       GetWeight() { return weight; }
  inline void         SetWeight( double _weight ) { weight = _weight; }

  inline ReplaceObjects_SwapObject & operator=( ReplaceObjects_SwapObject &other );
  inline ReplaceObjects_SwapObject & operator=( ReplaceObjects_SwapObject *other );

protected:
  bool     use;

  char   * full_path;
  char   * filename;

  double   weight;
};

// operator=( reference ):
inline ReplaceObjects_SwapObject & ReplaceObjects_SwapObject::operator=( ReplaceObjects_SwapObject &other ) {
  SetFullPath( other.GetFullPath() );
  SetWeight(   other.GetWeight()   );
  SetUse(      other.GetUse()      );

  return *this;
}

// operator=( pointer ):
inline ReplaceObjects_SwapObject & ReplaceObjects_SwapObject::operator=( ReplaceObjects_SwapObject *other ) {
  return operator=( *other );
}

