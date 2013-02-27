//
// ParticleCloud-Instance.cpp
//

#include "particlecloud-instance.h"

GlobalFunc *ParticleCloud_Instance::global = NULL;

// Constructor:
ParticleCloud_Instance::ParticleCloud_Instance( LWItemID _id )
  : start_time(0.0), birth_rate(100), max_particles(1000),
    fixed_random(true), use_frozen(false), show_indices(false),
    reinitialize(true), reevaluate(true),
    use_particles_from_file(false), particle_file(NULL) {

  items[0] = _id;
  items[1] = LWITEM_NULL;
}

// Destructor:
ParticleCloud_Instance::~ParticleCloud_Instance() {
  ;
}

// operator=():
ParticleCloud_Instance & ParticleCloud_Instance::operator=( ParticleCloud_Instance &other_inst ) {
  items[0]        = other_inst.GetItems()[0];

  start_time      = other_inst.GetStartTime();
  birth_rate      = other_inst.GetBirthRate();
  max_particles   = other_inst.GetMaxParticles();

  fixed_random    = other_inst.GetFixedRandom();
  use_frozen      = other_inst.GetUseFrozen();
  show_indices    = other_inst.GetShowIndices();

  return *this;
}

