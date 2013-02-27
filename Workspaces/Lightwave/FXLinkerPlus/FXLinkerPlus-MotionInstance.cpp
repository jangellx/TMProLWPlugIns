//
// FXLinkerPlus-MotionInstance.cpp
//

#include "FXLinkerPlus-MotionInstance.h"

GlobalFunc *FXLinkerPlus_MotionInstance::global = NULL;

extern DynArray< FXLinkerPlus_ParticleFile * > particle_files;

// Constrcutor:
FXLinkerPlus_MotionInstance::FXLinkerPlus_MotionInstance( LWItemID _applied )
  : particle_index(0), use_align_to_path(false), align_look_ahead(1),
    jitter_dampening(0), use_size_effect(false), size_effect_multiplier(1.0),
    file(NULL), from_mode(FXFROM_ITEM), particles(NULL), keep_scale_above_zero(false),
    link_scale_axes(false), link_rotation_axes(false), start_frame(0) {

  items[0] = _applied;
  items[1] = LWITEM_NULL;
  items[2] = LWITEM_NULL;

  rotation[0] = 0.0;
  rotation[1] = 0.0;
  rotation[2] = 0.0;

  rotation_velocity[0] = 0.0;
  rotation_velocity[1] = 0.0;
  rotation_velocity[2] = 0.0;

  scale[0] = 1.0;
  scale[1] = 1.0;
  scale[2] = 1.0;

  scale_velocity[0] = 0.0;
  scale_velocity[1] = 0.0;
  scale_velocity[2] = 0.0;
}

// Destructor:
FXLinkerPlus_MotionInstance::~FXLinkerPlus_MotionInstance() {
  ClearOldPFX();
}

// operator=( Pointer )
FXLinkerPlus_MotionInstance & FXLinkerPlus_MotionInstance::operator=( FXLinkerPlus_MotionInstance *other ) {
  return operator=( *other );
}

// operator=( Reference )
FXLinkerPlus_MotionInstance & FXLinkerPlus_MotionInstance::operator=( FXLinkerPlus_MotionInstance &other ) {
  from_mode            = other.GetFromMode();
  SetParticleFile( other.GetParticleFile(), NULL );

  items[0]             = other.GetItems()[0];
  items[1]             = other.GetItems()[1];
  items[2]             = other.GetItems()[2];
  particle_index       = other.GetParticleIndex();

  start_frame          = other.GetStartFrame();

  use_align_to_path    = other.GetUseAlignToPath();
  align_look_ahead     = other.GetAlignLookAhead();
  jitter_dampening     = other.GetJitterDampening();

  rotation[0]          = other.GetRotationVec()[0];
  rotation[1]          = other.GetRotationVec()[1];
  rotation[2]          = other.GetRotationVec()[2];

  rotation_velocity[0] = other.GetRotationVelocityVec()[0];
  rotation_velocity[1] = other.GetRotationVelocityVec()[1];
  rotation_velocity[2] = other.GetRotationVelocityVec()[2];

  use_size_effect      = other.GetUseSizeEffect();
  size_effect_multiplier = other.GetSizeEffectMultiplier();

  scale[0]             = other.GetRotationVec()[0];
  scale[1]             = other.GetRotationVec()[1];
  scale[2]             = other.GetRotationVec()[2];

  scale_velocity[0]    = other.GetScaleVelocityVec()[0];
  scale_velocity[1]    = other.GetScaleVelocityVec()[1];
  scale_velocity[2]    = other.GetScaleVelocityVec()[2];

  return *this;
}

// UpdatePFX():
//  Update a particles file.  If this instance is set to use a particles file,
//   it will load it if needed.  If not using one, it will reset
//   clear out the particles if it is the only one using, or else it will
//   just decrememt it's counter.
void FXLinkerPlus_MotionInstance::UpdatePFX( LWMessageFuncs * message ) {
  // See if we're using a particles or not
  if( ((from_mode == FXFROM_PFX_FILE) || (from_mode == FXFROM_PSM_FILE)) && (file != NULL) ) {
    // See if it's already loaded
    for( unsigned long i=0; i < particle_files.NumElements(); i++ ) {
      if( stricmp( particle_files[i]->GetFile(), file ) == 0 ) {
        if( particles == particle_files[i] )
          return;

        if( particles != NULL )
          ClearOldPFX();

        particles = particle_files[i];
        particles->OneMoreUser();
        return;
      }
    }

    particles = new FXLinkerPlus_ParticleFile( file );
    particles->OneMoreUser();
    particles->Load( from_mode, message );
    particle_files.Add( particles );
  } else {
    // Decrement/Clear an old PFX
    ClearOldPFX();
  }
}

// ClearOldPFX():
void FXLinkerPlus_MotionInstance::ClearOldPFX() {
  if( particles == NULL )
    return;

  // Decrement the particles counter
  particles->OneLessUser();

  // If no one else is using it, delete it
  if( particles->GetUsers() == 0 ) {
    unsigned long index = particle_files.FindIndexOf( particles );
    assert( index != -1 );

    delete particle_files[ index ];
    particle_files.Remove( index );
  }

  particles = NULL;
}

