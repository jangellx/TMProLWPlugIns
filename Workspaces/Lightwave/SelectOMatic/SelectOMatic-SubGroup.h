//
// SelectOMatic-SubGroup.h
//

// class SelectOMatic_SubGroup
class SelectOMatic_SubGroup {
public:
  SelectOMatic_SubGroup() { name = NULL; }
  ~SelectOMatic_SubGroup() { SetName( NULL ); }

  // Accessors
  const char *GetName()                    { return name; }
  void        SetName( const char *_name ) { StringTools::strdup2( name, _name ); }

  bool        SetIsFolded( bool _state )   { is_folded = _state; }
  bool        GetIsFolded()                { return is_folded; }

  DynArray< SelectOMatic_SubGroup * >  GetSubGroups() { return sub_groups; }
  DynArray< SelectOMatic_Item     * >  GetItems()     { return children; }

protected:
  char *name;

  bool is_folded;
};

