//
// FXLinkerPlus-Motion.cpp
//

#include "FXLinkerPlus.h"
#include "FXLinkerPlus-MotionInstance.h"

#include "lightwave/vecmath/vecmath.h"

#include <stdio.h>

DynArray< FXLinkerPlus_MotionInstance * > instances;
DynArray< FXLinkerPlus_ParticleFile   * > particle_files;
FXLinkerPlus_Interface *motion_ui = NULL;

extern FXLinkerPlus_MotionInstance *new_inst;

//#define PI 3.14159265358979323846
inline double DegreesToRadians( double degrees ) { return (degrees * (PI/180)); }
extern double RadiansToDegrees( double radians ) { return (radians * (180/PI)); }

// FXLinkerPlus_Motion_Activate():
XCALL_ (int) FXLinkerPlus_Motion_Activate( long version, GlobalFunc *global,
                                           void *_local, void *serverData ) {

  if( version != LWITEMMOTION_VERSION )
    return AFUNC_BADVERSION;

  // Fill in the handler functions
  LWItemMotionHandler *local = (LWItemMotionHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->priv    = NULL;
    local->inst->create  = FXLinkerPlus_Motion_Create;
    local->inst->destroy = FXLinkerPlus_Motion_Destroy;
    local->inst->copy    = FXLinkerPlus_Motion_Copy;
    local->inst->load    = FXLinkerPlus_Motion_Load;
    local->inst->save    = FXLinkerPlus_Motion_Save;
    local->inst->descln  = FXLinkerPlus_Motion_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems = FXLinkerPlus_Motion_UseItems;
    local->item->changeID = FXLinkerPlus_Motion_ChangeID;
  }

  local->evaluate = FXLinkerPlus_Motion_Evaluate;
  local->flags    = FXLinkerPlus_Motion_Flags;

  FXLinkerPlus_MotionInstance::global = global;

  return AFUNC_OK;
}

// EvaluateInstance():
void EvaluateInstance( FXLinkerPlus_MotionInstance *inst, double current_time, int current_frame, double *position, double *rotation, double *scale ) {
  float particle_position[3], particle_size, particle_age;
  double h=0.0, p=0.0;

  LWPSysFuncs *particle_funcs = NULL;
  LWPSysID *particles = NULL;

  // Particles from item (Particle Services)
  if( inst->GetFromMode() == FXFROM_ITEM ) {
    if( inst->GetParticleItem() == LWITEM_NULL )
      return;

    // Fetch the particle system
    particle_funcs = (LWPSysFuncs *)(*inst->global)( LWPSYSFUNCS_GLOBAL, GFUSE_TRANSIENT );
    particles = (*particle_funcs->getPSys)( inst->GetParticleItem() );
    if( particles[0] == NULL )
      return;

    // See if the particle index is in range
    int count = (*particle_funcs->getPCount)( particles[0] );
    if( inst->GetParticleIndex() >= count )
      return;

    // Get the particles
    LWPSBufID ps_pos = (*particle_funcs->getBufID)( particles[0], LWPSB_POS );
    LWPSBufID ps_siz = (*particle_funcs->getBufID)( particles[0], LWPSB_SIZ );
    LWPSBufID ps_age = (*particle_funcs->getBufID)( particles[0], LWPSB_AGE );

    (*particle_funcs->getParticle)( ps_pos, inst->GetParticleIndex(),  particle_position );
    (*particle_funcs->getParticle)( ps_siz, inst->GetParticleIndex(), &particle_size     );
    (*particle_funcs->getParticle)( ps_age, inst->GetParticleIndex(), &particle_age      );

    // Align To Path:  One frame look-ahead, no dampening
    float particle_velocity[3];
    LWPSBufID ps_vel = (*particle_funcs->getBufID)( particles[0], LWPSB_VEL );
    (*particle_funcs->getParticle)( ps_vel, inst->GetParticleIndex(), particle_velocity );
    
    if( inst->GetUseAlignToPath() ) {
      LWDVector vector;
      vector[0] = particle_velocity[0];
      vector[1] = particle_velocity[1];
      vector[2] = particle_velocity[2];
      LWVecMath::vec_hp( vector, &h, &p );
    }
  } else {
    // Particles from PFX or PSM file
    if( inst->GetParticles() == NULL )
      return;

    if( inst->GetParticleIndex() >= (int)inst->GetParticles()->GetParticles().NumElements() )
      return;

    FXLinkerPlus_Particle*particle = inst->GetParticles()->GetParticles()[ inst->GetParticleIndex() ];
    if( particle->GetFrames().NumElements() == 0 )
      return;

    long first_frame = inst->GetStartFrame() + particle->GetFrames()[0]->GetFrame();
    long last_frame  = inst->GetStartFrame() + particle->GetFrames()[ (particle->GetFrames().NumElements() - 1) ]->GetFrame();
    FXLinkerPlus_ParticleFrame *frame;
    FXLinkerPlus_ParticleFrame *prev_frame;;

    // Figure out which particle frame we want
    if( first_frame >= current_frame ) {
      frame = particle->GetFrames()[ 0 ];
      prev_frame = frame;
    } else if( current_frame > last_frame ) {
      frame = particle->GetFrames()[ particle->GetFrames().NumElements() - 1 ];
      prev_frame = frame;
    } else {
      frame = particle->GetFrames()[ current_frame - first_frame ];
      prev_frame = particle->GetFrames()[ (current_frame - first_frame) - 1 ];
    }

    // Set up the initial data
    particle_position[0] = frame->GetPosition()[0];
    particle_position[1] = frame->GetPosition()[1];
    particle_position[2] = frame->GetPosition()[2];

    particle_size = frame->GetScale();

    // Modify the particle for motion blur
    LWSceneInfo *scene_info = (LWSceneInfo*)(*inst->global)( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
    if( current_time != 0.0 ) {
      double now = ((double)(inst->GetStartFrame() + frame->GetFrame())) / scene_info->framesPerSecond;
      double diff = (now - current_time);

      particle_position[0] -= (particle_position[0] - prev_frame->GetPosition()[0]) * (float)(diff * scene_info->framesPerSecond);
      particle_position[1] -= (particle_position[1] - prev_frame->GetPosition()[1]) * (float)(diff * scene_info->framesPerSecond);
      particle_position[2] -= (particle_position[2] - prev_frame->GetPosition()[2]) * (float)(diff * scene_info->framesPerSecond);

      particle_size -= (particle_size - prev_frame->GetScale()) * (float)(diff * scene_info->framesPerSecond);
    }

    // User-defined look-ahead with dampening
    LWDVector look_vector = { 0.0, 0.0, 0.0 };
    if( inst->GetUseAlignToPath() ) {
      long look_frame_number = current_frame + inst->GetAlignLookAhead();
      FXLinkerPlus_ParticleFrame *look_frame = NULL;

      if( look_frame_number >= last_frame ) {
        look_frame = particle->GetFrames()[ particle->GetFrames().NumElements() - 1 ];
      } else if( look_frame_number <= first_frame ) {
        look_frame = particle->GetFrames()[ 0 ];
      } else {
        look_frame = particle->GetFrames()[ look_frame_number - first_frame ];
      }

      // Jitter Dampening
      if( inst->GetJitterDampening() == 0 ) {
        look_vector[0] = look_frame->GetPosition()[0];
        look_vector[1] = look_frame->GetPosition()[1];
        look_vector[2] = look_frame->GetPosition()[2];
      } else {
        long count = 0;
        for( long i = look_frame_number - inst->GetJitterDampening(); i <= (look_frame_number + inst->GetJitterDampening()); i++ ) {
          if( (i <= first_frame) || (i >= last_frame) )
            continue;

          look_vector[0] += particle->GetFrames()[ i - first_frame ]->GetPosition()[0];
          look_vector[1] += particle->GetFrames()[ i - first_frame ]->GetPosition()[1];
          look_vector[2] += particle->GetFrames()[ i - first_frame ]->GetPosition()[2];
          count++;
        }

        look_vector[0] /= (double)count;
        look_vector[1] /= (double)count;
        look_vector[2] /= (double)count;
      }

      LWDVector vector;
      vector[0] = look_vector[0] - frame->GetPosition()[0];
      vector[1] = look_vector[1] - frame->GetPosition()[1];
      vector[2] = look_vector[2] - frame->GetPosition()[2];
      LWVecMath::vec_hp( vector, &h, &p );
    }

    particle_size *= particle->GetSize();
    particle_age = (float) (current_frame - first_frame);
    if( particle_age < 0 )
      particle_age = 0;
  }

  // Add the particle position to the base position
  position[0] = inst->GetAdditive( FXLPADD_X ) ? position[0] + particle_position[0] : particle_position[0];
  position[1] = inst->GetAdditive( FXLPADD_Y ) ? position[1] + particle_position[1] : particle_position[1];
  position[2] = inst->GetAdditive( FXLPADD_Z ) ? position[2] + particle_position[2] : particle_position[2];

  // Apply the particle size
  scale[0] = inst->GetScaleVec()[0] + (inst->GetAdditive( FXLPADD_SX ) ? scale[0] : 0 );
  scale[1] = inst->GetScaleVec()[1] + (inst->GetAdditive( FXLPADD_SY ) ? scale[1] : 0 );
  scale[2] = inst->GetScaleVec()[2] + (inst->GetAdditive( FXLPADD_SZ ) ? scale[2] : 0 );

  if( inst->GetUseSizeEffect() ) {
    scale[0] = scale[0] * particle_size;
    scale[1] = scale[1] * particle_size;
    scale[2] = scale[2] * particle_size;
  }

  // Apply the scale velocity
  scale[0] += (inst->GetScaleVelocityVec()[0] * particle_age);
  scale[1] += (inst->GetScaleVelocityVec()[1] * particle_age);
  scale[2] += (inst->GetScaleVelocityVec()[2] * particle_age);

  // Handle Keep Scale Above Zero
  if( inst->GetKeepScaleAboveZero() ) {
    scale[0] < 0.0 ? 0.0 : scale[0];
    scale[1] < 0.0 ? 0.0 : scale[1];
    scale[2] < 0.0 ? 0.0 : scale[2];
  }

  // Handle Link Scale Axes
  if( inst->GetLinkScaleAxes() ) {
    scale[1] = scale[0];
    scale[2] = scale[0];
  }

  // Apply Align To Path
  double particle_rotation[3] = { RadiansToDegrees( h ), RadiansToDegrees( p ), 0.0 };

  // Add the particle rotation to the base position
  rotation[0] = inst->GetAdditive( FXLPADD_H ) ? RadiansToDegrees( rotation[0] ) + particle_rotation[0] : particle_rotation[0];
  rotation[1] = inst->GetAdditive( FXLPADD_P ) ? RadiansToDegrees( rotation[1] ) + particle_rotation[1] : particle_rotation[1];
  rotation[2] = inst->GetAdditive( FXLPADD_B ) ? RadiansToDegrees( rotation[2] ) + particle_rotation[2] : particle_rotation[2];

  // Apply the rotation value and velocity
  rotation[0] += (inst->GetRotationVelocityVec()[0] * particle_age);
  rotation[1] += (inst->GetRotationVelocityVec()[1] * particle_age);
  rotation[2] += (inst->GetRotationVelocityVec()[2] * particle_age);

  rotation[0] = DegreesToRadians( rotation[0] );
  rotation[1] = DegreesToRadians( rotation[1] );
  rotation[2] = DegreesToRadians( rotation[2] );

  // Handle Link Rotation Axes
  if( inst->GetLinkRotationAxes() ) {
    rotation[1] = rotation[0];
    rotation[2] = rotation[0];
  }
}

// FXLinkerPlus_Motion_Evaluate():
void FXLinkerPlus_Motion_Evaluate( LWInstance _inst, const LWItemMotionAccess *access ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;

  if( inst->GetAppliedItem() != access->item )
    return;

  double position[3], rotation[3], scale[3];

  // Get the item's current position/rotation/scale
  (*access->getParam)( LWIP_POSITION, access->time, position );
  (*access->getParam)( LWIP_ROTATION, access->time, rotation );
  (*access->getParam)( LWIP_SCALING,  access->time, scale    );

  EvaluateInstance( inst, access->time, access->frame, position, rotation, scale );

  // Fill back in the item's position/rotation/scale
  (*access->setParam)( LWIP_POSITION, position );
  (*access->setParam)( LWIP_ROTATION, rotation );
  (*access->setParam)( LWIP_SCALING,  scale    );
}

// FXLinkerPlus_Motion_Flags():
unsigned int FXLinkerPlus_Motion_Flags( LWInstance _inst ) {
  return 0;
}

// FXLinkerPlus_Motion_Create():
//  If new_inst is non-NULL, it will be assumed that we should use this as our
//   new instance, and we will claim ownership, return it as our instance data,
//   and change new_inst to NULL
LWInstance FXLinkerPlus_Motion_Create ( void *priv, void *context, LWError *error ) {
  FXLinkerPlus_MotionInstance *inst = new_inst;
  if( inst == NULL )
    inst = new FXLinkerPlus_MotionInstance( (LWItemID)context );

  inst->SetAppliedItem( (LWItemID)context );
  instances.Add( inst );

  new_inst = NULL;
  return inst;
}

// FXLinkerPlus_Motion_Destroy():
void FXLinkerPlus_Motion_Destroy( LWInstance _inst ){
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;

  int index = instances.FindIndexOf( inst );
  assert( index != DA_NO_MATCH );
  instances.Remove( index );

  if( instances.NumElements() == 0 ) {
    if( motion_ui != NULL ) {
      delete motion_ui;
      motion_ui = NULL;
    }
  } else {
    if( motion_ui != NULL )
      HandleInstancePopup( motion_ui->instance_popup, (void *)motion_ui );
  }

  delete inst;
}

// FXLinkerPlus_Motion_Copy():
LWError FXLinkerPlus_Motion_Copy( LWInstance _to, LWInstance _from) {
  FXLinkerPlus_MotionInstance *to   = (FXLinkerPlus_MotionInstance *)_to;
  FXLinkerPlus_MotionInstance *from = (FXLinkerPlus_MotionInstance *)_from;
  to = from;
  return NULL;
}

// FXLinkerPlus_Motion_Load():
LWError FXLinkerPlus_Motion_Load( LWInstance _inst, const LWLoadState *loader ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;
  assert( loader->ioMode == LWIO_ASCII );

  char buffer[ 2048 ];

  // Check the Header
  for( int i = 0; i < 2; i++ )  {  // For some reason there was a blank line.  This fixed it.
    if( (*loader->read)( loader->readData, buffer, 2048 ) > 0 )
      break;
  }

  StringTools::RemoveHeadWhiteSpace( buffer );

  if( strnicmp( buffer, "FXLinkerPlus", 12 ) != 0 )
    return "FXLinkerPlus Error:  Unrecognized FXLinkerPlus data found; aborting load.";

  char *word = StringTools::FindWord( buffer, 1 );
  if( word == NULL )
    return "FXLinkerPlus Load Error:  Version Number Missing; aborting load.";

  if( atoi( word ) != 1 )
    return "FXLinkerPlus Load Error:  Unsupported Version; aborting load.";

  // Look for keywords
  while( true ) { 
    (*loader->read)( loader->readData, buffer, 2048 );
    StringTools::RemoveHeadWhiteSpace( buffer );

    char *word = StringTools::FindWord( buffer, 1 );
    if( buffer[0] == '}' ) {
      break;

    } else if( strnicmp( buffer, "FromMode", 8 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after FromMode keyword; aborting load.";
      inst->SetFromMode( (FXLP_FromModes) atoi( word ) );

    } else if( strnicmp( buffer, "ParticleItem", 12 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing ID after ParticleItem keyword; aborting load.";

      LWItemID id;
      sscanf( word, "%x", &id );
      inst->SetParticleItem( id );

    } else if( strnicmp( buffer, "ParticleFile", 12 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      inst->SetParticleFile( word, NULL );

    } else if( strnicmp( buffer, "ParticleIndex", 13 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after ParticleIndex keyword; aborting load.";
      inst->SetParticleIndex( atoi( word ) );

    } else if( strnicmp( buffer, "StartFrame", 13 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after StartFrame keyword; aborting load.";
      inst->SetStartFrame( atoi( word ) );

    } else if( strnicmp( buffer, "UseAlignToPath", 14 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after UseAlignToPath keyword; aborting load.";
      inst->SetUseAlignToPath( (atoi( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "AlignLookAhead", 14 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after AlignLookAhead keyword; aborting load.";
      inst->SetAlignLookAhead( atoi( word ) );

    } else if( strnicmp( buffer, "LinkRotationAxes", 16 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after LinkRotationAxes keyword; aborting load.";
      inst->SetLinkRotationAxes( (atoi( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "JitterDampening", 15 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after JitterDampening keyword; aborting load.";
      inst->SetJitterDampening( atoi( word ) );

    } else if( strnicmp( buffer, "RotationVec", 11 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      for( int i=0; i < 3; i++ ) {
        if( word == NULL )
          return "FXLinkerPlus Load Error:  Missing value(s) after RotationVec keyword; aborting load.";
        inst->GetRotationVec()[i] = atof( word );
        word = StringTools::FindWord( word, 1 );
      }

    } else if( strnicmp( buffer, "RotationVelocityVec", 19 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      for( int i=0; i < 3; i++ ) {
        if( word == NULL )
          return "FXLinkerPlus Load Error:  Missing value(s) after RotationVelocityVec keyword; aborting load.";
        inst->GetRotationVelocityVec()[i] = atof( word );
        word = StringTools::FindWord( word, 1 );
      }

    } else if( strnicmp( buffer, "UseSizeEffect", 13 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after UseSizeEffect keyword; aborting load.";
      inst->SetUseSizeEffect( (atoi( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "SizeEffectMultiplier", 20 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after SizeEffectMultiplier keyword; aborting load.";
      inst->SetSizeEffectMultiplier( (atof( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "KeepScaleAboveZero", 16 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after KeepScaleAboveZero keyword; aborting load.";
      inst->SetKeepScaleAboveZero( (atoi( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "LinkScaleAxes", 13 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      if( word == NULL )
        return "FXLinkerPlus Load Error:  Missing value after LinkScaleAxes keyword; aborting load.";
      inst->SetLinkScaleAxes( (atoi( word ) == 0) ? false : true );

    } else if( strnicmp( buffer, "ScaleVec", 8 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      for( int i=0; i < 3; i++ ) {
        if( word == NULL )
          return "FXLinkerPlus Load Error:  Missing value(s) after ScaleVec keyword; aborting load.";
        inst->GetScaleVec()[i] = atof( word );
        word = StringTools::FindWord( word, 1 );
      }

    } else if( strnicmp( buffer, "ScaleVelocityVec", 16 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      for( int i=0; i < 3; i++ ) {
        if( word == NULL )
          return "FXLinkerPlus Load Error:  Missing value(s) after GetScaleVelocityVec keyword; aborting load.";
        inst->GetScaleVelocityVec()[i] = atof( word );
        word = StringTools::FindWord( word, 1 );
      }

    } else if( strnicmp( buffer, "Additive", 8 ) == 0 ) {
      word = StringTools::FindWord( buffer, 1 );
      for( int i=0; i < 9; i++ ) {
        if( word == NULL )
          return "FXLinkerPlus Load Error:  Missing value(s) after Additive keyword; aborting load.";
        inst->SetAdditive( (FXLP_AdditiveSlots) i, ((atoi( word ) == 0) ? false : true) );
        word = StringTools::FindWord( word, 1 );
      }
    }
  }

  inst->UpdatePFX();

  return NULL;
}

// FXLinkerPlus_Motion_Save():
LWError FXLinkerPlus_Motion_Save( LWInstance _inst, const LWSaveState *saver ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;
  assert( saver->ioMode == LWIO_ASCII );

  char buffer[ 2048 ] = "FXLinkerPlus 1 {";
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  FromMode %d", inst->GetFromMode() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ParticleItem %x", inst->GetParticleItem() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ParticleFile %s", ((inst->GetParticleFile() == NULL) ? "" : inst->GetParticleFile()) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ParticleIndex %d", inst->GetParticleIndex() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  StartFrame %d", inst->GetStartFrame() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  UseAlignToPath %d", (inst->GetUseAlignToPath() ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  AlignLookAhead %d", inst->GetAlignLookAhead() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  JitterDampening %d", inst->GetJitterDampening() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  LinkRotationAxes %d", (inst->GetLinkRotationAxes() ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  RotationVec %g %g %g", inst->GetRotationVec()[0], inst->GetRotationVec()[1], inst->GetRotationVec()[2] );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  RotationVelocityVec %g %g %g", inst->GetRotationVelocityVec()[0], inst->GetRotationVelocityVec()[1], inst->GetRotationVelocityVec()[2] );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  UseSizeEffect %d", (inst->GetUseSizeEffect() ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  SizeEffectMultiplier %g", inst->GetSizeEffectMultiplier() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  LinkScaleAxes %d", (inst->GetLinkScaleAxes() ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  KeepScaleAboveZero %d", (inst->GetKeepScaleAboveZero() ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ScaleVec %g %g %g", inst->GetScaleVec()[0], inst->GetScaleVec()[1], inst->GetScaleVec()[2] );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ScaleVelocityVec %g %g %g", inst->GetScaleVelocityVec()[0], inst->GetScaleVelocityVec()[1], inst->GetScaleVelocityVec()[2] );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  Additive %d %d %d %d %d %d %d %d %d", (inst->GetAdditive(FXLPADD_X)  ? 1 : 0), (inst->GetAdditive(FXLPADD_Y)  ? 1 : 0), (inst->GetAdditive(FXLPADD_Z)  ? 1 : 0),
                                                            (inst->GetAdditive(FXLPADD_H)  ? 1 : 0), (inst->GetAdditive(FXLPADD_P)  ? 1 : 0), (inst->GetAdditive(FXLPADD_B)  ? 1 : 0),
                                                            (inst->GetAdditive(FXLPADD_SX) ? 1 : 0), (inst->GetAdditive(FXLPADD_SY) ? 1 : 0), (inst->GetAdditive(FXLPADD_SZ) ? 1 : 0) );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  (*saver->write)( saver->writeData, "}", 1 );

  return NULL;
}

// FXLinkerPlus_Motion_Describe():
const char * FXLinkerPlus_Motion_Describe ( LWInstance _inst ) {
  return "FX Linker Plus (Motion)";
}

// FXLinkerPlus_Motion_UseItems():
const LWItemID * FXLinkerPlus_Motion_UseItems( LWInstance _inst ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;
  return inst->GetItems();
}

// FXLinkerPlus_Motion_ChangeID():
void FXLinkerPlus_Motion_ChangeID( LWInstance _inst, const LWItemID *id ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;

  int i, j;
  for( j=0; j != (int)LWITEM_NULL; j++ ) {
    for( i=0; i != (int)LWITEM_NULL; i+=2 ) {
      if( id[i] == inst->GetItems()[i] ) {
        inst->GetItems()[i] = id[i];
        break;
      }
    }
  }
}

// FXLinkerPlus_MotionUI_Activate():
XCALL_ (int) FXLinkerPlus_MotionUI_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  FXLinkerPlus_Interface::global = global;
  LWInterface *ui = (LWInterface *)local;

  ui->panel   = NULL;
  ui->options = FXLinkerPlus_MotionUI_Open;
  ui->command = NULL;

  return AFUNC_OK;
}

// FXLinkerPlus_MotionUI_Open()
LWError FXLinkerPlus_MotionUI_Open( LWInstance _inst ) {
  FXLinkerPlus_MotionInstance *inst = (FXLinkerPlus_MotionInstance *)_inst;
  if( motion_ui == NULL ) {
    motion_ui = new FXLinkerPlus_Interface( inst );
    motion_ui->CreateUI( inst );
  }

  unsigned long index = instances.FindIndexOf( inst );
  assert( index != -1 );
  SET_INT( motion_ui->instance_popup, index );
  HandleInstancePopup( motion_ui->instance_popup, (void *)motion_ui );

  motion_ui->OpenUI();
  return NULL;
}

