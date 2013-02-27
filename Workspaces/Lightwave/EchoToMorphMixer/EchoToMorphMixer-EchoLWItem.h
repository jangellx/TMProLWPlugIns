//
// EchoToMorphMixer-EchoLWItem.h
//

// class EchoLWItem
class EchoLWItem {
public:
  EchoLWItem( LWItemID _id, const char * _name ) : id(_id), name(NULL) { StringTools::strdup2( name, _name ); }
  ~EchoLWItem() { StringTools::strdup2( name, NULL ) ; }

  // Accessors
  LWItemID     GetID()   { return id;   }
  const char * GetName() { return name; }

protected:
  LWItemID   id;
  char     * name;
};

// struct EchoVMapScanData
struct EchoVMapScanData {
  LWItemID             item;
  LWMeshInfo          *mesh;
  DynArray< char * >   vmap_names;
};

