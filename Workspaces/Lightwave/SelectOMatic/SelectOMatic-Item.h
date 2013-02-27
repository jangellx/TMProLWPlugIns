//
// SelectOMatic-Item
//

#include <portable/stringtools/stringtools.h>
#include <portable/dynarray/dynarray.h>

#include <lwrender.h>

extern void * SOM_GROUP;
extern int next_group_index;

class SelectOMatic_Item {
public:
  SelectOMatic_Item( const char *_name, SelectOMatic_Item *_parent );           // For creating groups
  SelectOMatic_Item( LWItemID _id, int _index, SelectOMatic_Item *_parent );    // For creating items
  SelectOMatic_Item( SelectOMatic_Item *other_item ) : name(NULL) { operator=(other_item); }
  SelectOMatic_Item( SelectOMatic_Item &other_item ) : name(NULL) { operator=(other_item); }
  ~SelectOMatic_Item();

  // File I/O
  void Load( const LWLoadState *loader );
  void Save( const LWSaveState *saver  );

  // Search
  void FindSelectedItems(  DynArray< SelectOMatic_Item * > &array );
  void FindSelectedGroups( DynArray< SelectOMatic_Item * > &array );
  void FindSelected(       DynArray< SelectOMatic_Item * > &array );

  void FindAllItems(       DynArray< SelectOMatic_Item * > &array );

  bool IsDragDropOK(       SelectOMatic_Item *target );
  bool BuildDragDropList(  DynArray< SelectOMatic_Item * > &array, bool root );

  void QSortChildren( int (__cdecl *compare )(const void *elem1, const void *elem2 ) );

  // Accessors
  inline LWItemID     GetID()              { return id; }
  inline void         SetID( LWItemID _id) { id = _id; }

         SelectOMatic_Item & operator=( SelectOMatic_Item *other_item );
  inline SelectOMatic_Item & operator=( SelectOMatic_Item &other_item );

  DynArray< SelectOMatic_Item * > & GetChildren()    { return children; }

  inline bool         GetIsFolded()                  { return is_folded; }
  inline void         SetIsFolded( bool _state )     { is_folded = _state; }

  inline bool         GetIsSelected()                { return is_selected; }
  inline void         SetIsSelected( bool _state )   { is_selected = _state; }

  inline int          GetOriginalIndex()             { return original_index; }
  inline void         SetOriginalIndex( int _index ) { original_index = _index; }

  inline const char * GetName()                      { return name; }
  inline void         SetName( const char * _name )  { StringTools::strdup2( name, _name ); }

  inline SelectOMatic_Item * GetParent()                             { return parent; }
  void                       SetParent( SelectOMatic_Item *_parent ) { parent = _parent; }

protected:
  LWItemID   id;                             // If SOM_GROUP, this is a foldable group.  If not, this is an actual item

  DynArray< SelectOMatic_Item * > children;  // Contains items only if id is SOM_GROUP

  bool       is_selected;
  bool       is_folded;                      // Can only be true if id is SOM_GROUP

  int        original_index;                 // Original index of this item in the list

  char     * name;                           // Group name

  SelectOMatic_Item *parent;                 // Parent group.  Must be non-NULL except for the root group
};

// operator=( reference )
inline SelectOMatic_Item & SelectOMatic_Item::operator=( SelectOMatic_Item &other_item ) {
  return *this = &other_item;
}

