//
// ParticleCloud_ParticleData.h
//

#include <portable/stringtools/stringtools.h>
#include <portable/dynarray/dynarray.h>
#include <portable/bfstream/bfstream.h>

#include <lwhost.h>

class ParticleCloud_Particle;
class ParticleCloud_ParticleFrame;

// class ParticleCloud_ParticleData:
class ParticleCloud_ParticleData {
public:
  ParticleCloud_ParticleData( const char *_file = NULL );
  ~ParticleCloud_ParticleData();

  // Accessors
         bool         LoadPFX( LWMessageFuncs *message = NULL );

  inline void         SetFirstFrame( int _frame) { first_frame = _frame; }
  inline int          GetFirstFrame()            { return first_frame; }

  inline void         SetLastFrame( int _frame)  { last_frame = _frame; }
  inline int          GetLastFrame()             { return last_frame; }

  inline DynArray< ParticleCloud_Particle * > & GetParticles() { return particles; }

protected:
  int   first_frame;
  int   last_frame;

  DynArray< ParticleCloud_Particle * > particles;
};

// class ParticleCloud_Particle:
class ParticleCloud_Particle {
public:
  ParticleCloud_Particle();
  ~ParticleCloud_Particle();

  // Accessors
  inline long  GetLifeFrame()                    { return life_frame; }
  inline void  SetLifeFrame( long _frame )       { life_frame = _frame; }

  inline float GetMass()                         { return mass; }
  inline void  SetMass( float _mass )            { mass = _mass; }

  inline float GetResistance()                   { return resistance; }
  inline void  SetResistance( float _resistance) { resistance = _resistance; }

  inline float GetSize()                         { return size; }
  inline void  SetSize( float _size )            { size = _size; }

  inline DynArray< ParticleCloud_ParticleFrame * > & GetFrames() { return frames; }

protected:
  DynArray< ParticleCloud_ParticleFrame * > frames;

  long  life_frame;

  float mass;
  float resistance;
  float size;
};

// class ParticleCloud_ParticleFrame:
class ParticleCloud_ParticleFrame {
public:
  ParticleCloud_ParticleFrame();
  ~ParticleCloud_ParticleFrame();

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

