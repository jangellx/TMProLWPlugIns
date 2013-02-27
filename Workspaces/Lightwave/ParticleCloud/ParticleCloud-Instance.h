//
// ParticleCloud-Instance.h
//

#include <lightwave/lw_sdk_cpp_wrappers/LWWrappers_L6.h>
#include "particlecloud-ParticleData.h"

class ParticleCloud_Instance {
public:
  ParticleCloud_Instance( LWItemID _id );
  ~ParticleCloud_Instance();

  // Maintenance
  ParticleCloud_Instance &operator=( ParticleCloud_Instance &other_inst );

  // Accessors
  inline LWItemID                   * GetItems()                       { return items; }

  inline double                       GetStartTime()                   { return start_time; }
  inline void                         SetStartTime( double _time )     { start_time = _time; }

  inline int                          GetBirthRate()                   { return birth_rate; }
  inline void                         SetBirthRate( int _rate )         { birth_rate = _rate; }

  inline int                          GetMaxParticles()                { return max_particles; }
  inline void                         SetMaxParticles( int _max )      { max_particles = _max; }

  inline bool                         GetUseFrozen()                   { return use_frozen; }
  inline void                         SetUseFrozen( bool _state )      { use_frozen = _state; }

  inline bool                         GetFixedRandom()                 { return fixed_random; }
  inline void                         SetFixedRandom( bool _state )    { fixed_random = _state; }

  inline bool                         GetShowIndices()                 { return show_indices; }
  inline void                         SetShowIndices( bool _state )    { show_indices = _state; }

  inline ParticleCloud_ParticleData & GetParticleData()                { return particle_data; }

  inline void                         SetParticleFile( const char *_file )  { StringTools::strdup2( particle_file, _file ); }
  inline const char                 * GetParticleFile()                     { return particle_file; }

  inline bool                         GetUseParticlesFromFile()             { return use_particles_from_file; }
  inline void                         SetUseParticlesFromFile( bool _state) { use_particles_from_file = _state; }

  // Render Accessors
  inline void       SetNow( LWFrame _frame, LWTime _time ) { frame = _frame; time = _time; }
  inline LWFrame    GetFrame() { return frame; }
  inline LWTime     GetTime()  { return time;  }

protected:
  LWItemID                     items[2];                // Items we're using

  double                       start_time;              // Time to start emitting particles
  int                          birth_rate;              // How quickly to emit particles per second
  int                          max_particles;           // Maximum number of particles to emit

  bool                         fixed_random;            // True if particles should be emitted in point order.
  bool                         use_frozen;              // True if the frozen mesh should be used
  bool                         show_indices;            // True if the particle indices should be displayed in OpenGL

  bool                         reinitialize;            // True if the particle motions need to be completely reinitialized
  bool                         reevaluate;              // True if the particle motion needs to be recalculated for the current frame

  ParticleCloud_ParticleData   particle_data;           // Partilce Data
  bool                         use_particles_from_file; // True if getting particle data from a PFX instead of dynamically generating it
  char                       * particle_file;           // Particle Filename

  // Render Variables
  LWFrame                      frame;
  LWTime                       time;

public:
  static GlobalFunc          * global;                  // Global Function
};

