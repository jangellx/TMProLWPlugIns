//
// FXLinkerPlus-Item.h
//

class FXLinkerPlus_Item {
public:
  FXLinkerPlus_Item( LWItemID _id, const char *_name ) : id(_id), name(NULL) { SetName( _name ); }
  ~FXLinkerPlus_Item() { SetName( NULL ); }

  inline void         SetName( const char *_name ) { StringTools::strdup2( name, _name ); }
  inline const char * GetName() { return name; }

  inline void         SetID( LWItemID _id ) { id = _id; }
  inline LWItemID     GetID() { return id; }

protected:
  LWItemID    id;
  char       *name;  
};
