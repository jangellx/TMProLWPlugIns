//
// SelectOMatic-Group.h
//

#include "SelectOMatic-Item.h"

// class SelectOMatic_Group
class SelectOMatic_Group {
public:
  SelectOMatic_Group() : name(NULL), current_item(-1) { ; }
  ~SelectOMatic_Group() { StringTools::strdup2( name, NULL ); items.Flush(); }

  // Accessors
  inline const char * GetName()                    { return name; }
  inline void         SetName( const char * _name) { StringTools::strdup2( name, _name ); }

  inline int          GetCurrentItem()           { return current_item; }
  inline void         SetCurrentItem( int _cur ) { current_item = _cur; }

  inline DynArray< SelectOMatic_Item * > & GetItems() { return items; }

protected:
  char                             * name;
  int                                current_item;
  DynArray< SelectOMatic_Item * >    items;
};

