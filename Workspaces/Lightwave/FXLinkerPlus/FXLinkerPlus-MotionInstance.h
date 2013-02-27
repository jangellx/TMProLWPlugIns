//
// FXLinkerPlus-MotionInstance.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lwmotion.h>
#include "FXLinkerPlus-Interface.h"
#include "FXLinkerPlus-PFX.h"

class FXLinkerPlus_MotionInstance;
void EvaluateInstance( FXLinkerPlus_MotionInstance *inst, double current_time, int current_frame, double *position, double *rotation, double *scale );

/*
enum FXLP_RotationModes {
  FXLPROT_CONSTANT = 0,
  FXLPROT_ALIGN_TO_PATH };

enum FXLP_ScaleModes {
  FXLPSCALE_CONSTANT = 0,
  FXLPSCALE_SCALE_EFFECT };
*/

enum FXLP_AdditiveSlots {
  FXLPADD_X = 0,
  FXLPADD_Y,
  FXLPADD_Z,
  FXLPADD_H,
  FXLPADD_P,
  FXLPADD_B,
  FXLPADD_SX,
  FXLPADD_SY,
  FXLPADD_SZ,
  FXLPADD_NUM_SLOTS };

// FXLinkerPlus_MotionInstance
class FXLinkerPlus_MotionInstance {
public:
  FXLinkerPlus_MotionInstance( LWItemID _applied );
  ~FXLinkerPlus_MotionInstance();

  // Accessors
  inline LWItemID   GetAppliedItem()                      { return items[0]; }
  inline void       SetAppliedItem( LWItemID _applied )   { items[0] = _applied; }

  inline FXLP_FromModes GetFromMode()                       { return from_mode; }
  inline void           SetFromMode( FXLP_FromModes _mode ) { from_mode = _mode; }

  inline LWItemID   GetParticleItem()                     { return items[1]; }
  inline void       SetParticleItem( LWItemID _particle ) { items[1] = _particle; }

  inline const char * GetParticleFile()                     { return file; }
  inline void         SetParticleFile( const char *_file, LWMessageFuncs *message )  { ClearOldPFX(); StringTools::strdup2( file, _file ); UpdatePFX( message ); }

  inline LWItemID * GetItems()                            { return items; }

  inline long       GetParticleIndex()                    { return particle_index; }
  inline void       SetParticleIndex( long _index )       { particle_index = _index; }

  inline long       GetStartFrame()                       { return start_frame; }
  inline void       SetStartFrame( long _frame )          { start_frame = _frame; }

  inline bool       GetUseAlignToPath()                   { return use_align_to_path; }
  inline void       SetUseAlignToPath( bool _state )      { use_align_to_path = _state; }

  inline short      GetAlignLookAhead()                   { return align_look_ahead; }
  inline void       SetAlignLookAhead( short _value )     { align_look_ahead = _value; }

  inline short      GetJitterDampening()                  { return jitter_dampening; }
  inline void       SetJitterDampening( short _value )    { jitter_dampening = _value; }

  inline bool       GetLinkRotationAxes()                 { return link_rotation_axes; }
  inline void       SetLinkRotationAxes( bool _state )    { link_rotation_axes = _state; }

  inline double   * GetRotationVec()                      { return rotation; }
  inline double   * GetRotationVelocityVec()              { return rotation_velocity; }

  inline bool       GetUseSizeEffect()                    { return use_size_effect; }
  inline void       SetUseSizeEffect( bool _state )       { use_size_effect = _state; }

  inline double     GetSizeEffectMultiplier()               { return size_effect_multiplier; }
  inline void       SetSizeEffectMultiplier( double _size ) { size_effect_multiplier = _size; }

  inline bool       GetLinkScaleAxes()                    { return link_scale_axes; }
  inline void       SetLinkScaleAxes( bool _state )       { link_scale_axes = _state; }

  inline bool       GetKeepScaleAboveZero()               { return keep_scale_above_zero; }
  inline void       SetKeepScaleAboveZero( bool _state )  { keep_scale_above_zero = _state; }

  inline double   * GetScaleVec()                         { return scale; }
  inline double   * GetScaleVelocityVec()                 { return scale_velocity; }

  inline bool       GetAdditive( FXLP_AdditiveSlots _slot ) { return additive[_slot]; }
  inline void       SetAdditive( FXLP_AdditiveSlots _slot, bool state ) { additive[_slot] = state; }

  FXLinkerPlus_MotionInstance &operator=( FXLinkerPlus_MotionInstance *other );
  FXLinkerPlus_MotionInstance &operator=( FXLinkerPlus_MotionInstance &other );

  const char      * GenerateDescribeLine();

  inline FXLinkerPlus_ParticleFile * GetParticles() { return particles; }

  void              UpdatePFX( LWMessageFuncs * message = NULL );

public:
  static GlobalFunc *global;

protected:
  void ClearOldPFX();

  FXLP_FromModes     from_mode;
  char             * file;
  LWItemID           items[3];

  long               particle_index;
  int                start_frame;

  bool               use_align_to_path;
  short              align_look_ahead;
  short              jitter_dampening;
  bool               link_rotation_axes;
  double             rotation[3];
  double             rotation_velocity[3];

  bool               use_size_effect;
  double             size_effect_multiplier;
  bool               link_scale_axes;
  bool               keep_scale_above_zero;
  double             scale[3];
  double             scale_velocity[3];

  bool               additive[ FXLPADD_NUM_SLOTS ];

  char               describe_line[ 256 ];

  FXLinkerPlus_ParticleFile * particles;
};

