//
// EchnoToMorphMixer-EchnoMappings.h
//

class EchoSynonym;
class EchoTarget;
class EchoViseme;

// class EchoSynonym
class EchoSynonym {
public:
  EchoSynonym() : full_path(NULL), filename(NULL), enable(false) { ; }
  ~EchoSynonym() { StringTools::strdup2( full_path, NULL ); }

  inline void         SetFullPath( const char *_path ) { assert( _path != NULL );
                                                         StringTools::strdup2( full_path, _path );
                                                         filename = DirStrings::HasFile( full_path ); }
  inline const char * GetFullPath() { return full_path; }

  inline const char * GetFilename() { return filename; }

  inline void         SetIsEnabled( bool state ) { enable = state; }
  inline bool         GetIsEnabled() { return enable; }

  inline EchoSynonym & operator=( EchoSynonym &other );

protected:
  char * full_path;
  char * filename;
  bool   enable;
};

// operator=():
inline EchoSynonym & EchoSynonym::operator=( EchoSynonym &other ) {
  SetFullPath(  other.GetFullPath()  );
  SetIsEnabled( other.GetIsEnabled() );
  return *this;
}

// class EchoTarget
class EchoTarget {
public:
  EchoTarget( const char *_name = NULL ) : name(NULL), linked_to(-1) { SetName( _name ); }
  ~EchoTarget() { SetName( NULL ); }

  // Accessors
  inline void          SetName( const char *_name ) { StringTools::strdup2( name, _name ); }
  inline const char  * GetName()                    { return name; }

  inline void          SetLinkedTo( unsigned long _to ) { linked_to = _to; }
  inline unsigned long GetLinkedTo()                    { return linked_to; }

  inline bool          IsLinked() { return ((linked_to == -1) ? false : true); }

  inline EchoTarget & operator=( EchoTarget &other );

protected:
  char          *name;
  unsigned long  linked_to;
};

// operator=():
inline EchoTarget & EchoTarget::operator=( EchoTarget &other ) {
  SetName(     other.GetName()     );
  SetLinkedTo( other.GetLinkedTo() );
  return *this;
}

// class EchoViseme
class EchoViseme {
public:
  EchoViseme( const char *_name = NULL ) : name(NULL), linked_to(-1) { SetName( _name ); }
  ~EchoViseme() { SetName( NULL ); }

  // Accessors
  inline void          SetName( const char *_name ) { StringTools::strdup2( name, _name ); }
  inline const char  * GetName()                    { return name; }

  inline void          SetLinkedTo( unsigned long _to ) { linked_to = _to; }
  inline unsigned long GetLinkedTo()                    { return linked_to; }

  inline bool          IsLinked() { return ((linked_to == -1) ? false : true); }

  inline EchoViseme & operator=( EchoViseme &other );

protected:
  char          *name;
  unsigned long  linked_to;
};

// operator=():
inline EchoViseme & EchoViseme::operator=( EchoViseme &other ) {
  SetName(     other.GetName()     );
  SetLinkedTo( other.GetLinkedTo() );
  return *this;
}

// class EchoMapping
class EchoMapping {
public:
  EchoMapping() : name(NULL) { ; }
  ~EchoMapping() { StringTools::strdup2( name, NULL );
                   synonyms.Flush(); targets.Flush(); visemes.Flush(); }

  // Accessors
  inline void         SetName( const char *_name ) { StringTools::strdup2( name, _name ); }
  inline const char * GetName()                    { return name; }

  inline DynArray< EchoSynonym * > & GetSynonyms() { return synonyms; }
  inline DynArray< EchoTarget  * > & GetTargets()  { return targets; }
  inline DynArray< EchoViseme  * > & GetVisemes()  { return visemes; }

  inline EchoMapping & operator=( EchoMapping &other );

protected:
  char *name;

  DynArray< EchoSynonym * > synonyms;
  DynArray< EchoTarget  * > targets;
  DynArray< EchoViseme  * > visemes;
};

// operator=():
inline EchoMapping & EchoMapping::operator=( EchoMapping &other ) {
  SetName( other.GetName() );

  unsigned long i;

  synonyms.Flush();
  EchoSynonym *syn;
  for( i=0; i < other.GetSynonyms().NumElements(); i++ ) {
    syn = new EchoSynonym;
    *syn = *other.GetSynonyms()[i];
    synonyms.Add( syn );
  }

  targets.Flush();
  EchoTarget *tar;
  for( i=0; i < other.GetTargets().NumElements(); i++ ) {
    tar = new EchoTarget;
    *tar = *other.GetTargets()[i];
    targets.Add( tar );
  }

  visemes.Flush();
  EchoViseme *vis;
  for( i=0; i < other.GetVisemes().NumElements(); i++ ) {
    vis = new EchoViseme;
    *vis = *other.GetVisemes()[i];
    visemes.Add( vis );
  }

  return *this;
}

