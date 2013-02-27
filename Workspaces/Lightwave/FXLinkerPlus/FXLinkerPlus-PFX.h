//
// FXLinkerPlus-ParticleFile.h
//

#include <portable/stringtools/stringtools.h>
#include <portable/dynarray/dynarray.h>
#include <portable/bfstream/bfstream.h>

#include <lwhost.h>

enum FXLP_FromModes {
  FXFROM_ITEM = 0,
  FXFROM_PFX_FILE,
  FXFROM_PSM_FILE };

class FXLinkerPlus_Particle;
class FXLinkerPlus_ParticleFrame;

// class FXLinkerPlus_ParticleFile:
class FXLinkerPlus_ParticleFile {
public:
  FXLinkerPlus_ParticleFile( const char *_file );
  ~FXLinkerPlus_ParticleFile();

  // Accessors
  inline void         SetFile( const char *_file ) { StringTools::strdup2( file, _file ); }
  inline const char * GetFile() { return file; }

  inline int          GetUsers() { return users; }
  inline int          OneMoreUser() { return ++users; }
  inline int          OneLessUser() { return --users; }

  inline bool         Load( FXLP_FromModes from, LWMessageFuncs *message = NULL );
         bool         LoadPFX( LWMessageFuncs *message = NULL );
         bool         LoadPSM( LWMessageFuncs *message = NULL );

  inline void         SetFirstFrame( int _frame) { first_frame = _frame; }
  inline int          GetFirstFrame()            { return first_frame; }

  inline void         SetLastFrame( int _frame)  { last_frame = _frame; }
  inline int          GetLastFrame()             { return last_frame; }

  inline DynArray< FXLinkerPlus_Particle * > & GetParticles() { return particles; }

protected:
  char *file;

  int   users;

  int   first_frame;
  int   last_frame;

  DynArray< FXLinkerPlus_Particle * > particles;
};

inline bool FXLinkerPlus_ParticleFile::Load( FXLP_FromModes from, LWMessageFuncs *message ) {
  switch( from ) {
    case FXFROM_PFX_FILE:      return LoadPFX( message );
    case FXFROM_PSM_FILE:      return LoadPSM( message );
    default:                   return false;
  }
}

// class FXLinkerPlus_Particle:
class FXLinkerPlus_Particle {
public:
  FXLinkerPlus_Particle();
  ~FXLinkerPlus_Particle();

  // Accessors
  inline long  GetLifeFrame()                    { return life_frame; }
  inline void  SetLifeFrame( long _frame )       { life_frame = _frame; }

  inline float GetMass()                         { return mass; }
  inline void  SetMass( float _mass )            { mass = _mass; }

  inline float GetResistance()                   { return resistance; }
  inline void  SetResistance( float _resistance) { resistance = _resistance; }

  inline float GetSize()                         { return size; }
  inline void  SetSize( float _size )            { size = _size; }

  inline DynArray< FXLinkerPlus_ParticleFrame * > & GetFrames() { return frames; }

protected:
  DynArray< FXLinkerPlus_ParticleFrame * > frames;

  long  life_frame;

  float mass;
  float resistance;
  float size;
};

// class FXLinkerPlus_ParticleFrame:
class FXLinkerPlus_ParticleFrame {
public:
  FXLinkerPlus_ParticleFrame();
  ~FXLinkerPlus_ParticleFrame();

  // Accessors
  inline long    GetFrame()               { return frame;   }
  inline void    SetFrame( long _frame )  { frame = _frame; }

  inline float * GetPosition() { return position; }

  inline float   GetScale()               { return scale;   }
  inline void    SetScale( float _scale ) { scale = _scale; }

protected:
  long  frame;

  float position[3];
  float scale;
};

