//
// IllumiSurf.cpp
//

#include "illumisurf-interface.h"
#include <lightwave/vecmath/vecmath_inline.h>
#include "illumisurf-imp.h"

extern "C" {
  // User Names
  ServerUserName IllumiSurf_Usernames[] = { {"IllumiSurf"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "IllumiSurf (Master)"   DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWSHADER_HCLASS,  SHADER_PLUGIN_NAME, IllumiSurf_Activate,           IllumiSurf_Usernames },
    { LWSHADER_ICLASS,  SHADER_PLUGIN_NAME, IllumiSurf_Interface_Activate, IllumiSurf_Usernames },
    { (const char *)NULL }
  };
}

// Globals
GlobalFunc *global;
DynArray< IllumiSurf_Instance * >   instances;
IllumiSurf_Instance               * copy_inst = NULL;   // Instance copy buffer

LWSurfaceFuncs  * surf_funcs;
LWEnvelopeFuncs * env_funcs;
LWVParmFuncs    * vparam_funcs;
LWItemInfo      * item_info;
LWLightInfo     * light_info;
LWObjectInfo    * object_info;
LWFileIOFuncs   * fileio;
IMPGlobal       * imp;

LWTime            curTime;          // Current time to be rendered

// IllumiSurf_Activate():
XCALL_ (int) IllumiSurf_Activate( long version, GlobalFunc *_global,
                                  void *_local, void *serverData ) {

  if( version != LWSHADER_VERSION )
    return(AFUNC_BADVERSION);

  LWShaderHandler *local = (LWShaderHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = IllumiSurf_Create;
    local->inst->destroy    = IllumiSurf_Destroy;
    local->inst->load       = IllumiSurf_Load;
    local->inst->save       = IllumiSurf_Save;
    local->inst->copy       = IllumiSurf_Copy;
    local->inst->descln     = IllumiSurf_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = IllumiSurf_UseItems;
    local->item->changeID   = IllumiSurf_ChangeID;
  }

  if( local->rend != NULL ) {
    local->rend->init       = IllumiSurf_Init;
    local->rend->cleanup    = IllumiSurf_CleanUp;
    local->rend->newTime    = IllumiSurf_NewTime;
  }

  local->evaluate           = IllumiSurf_Evaluate;
  local->flags              = IllumiSurf_Flags;

  global = _global;

  return AFUNC_OK;
}

// IllumiSurf_Create():
LWInstance IllumiSurf_Create( void *data, void *context, LWError *error ) {
  if( instances.NumElements() == 0 ) {
    surf_funcs   = (LWSurfaceFuncs  *)global( LWSURFACEFUNCS_GLOBAL,  GFUSE_ACQUIRE );
    env_funcs    = (LWEnvelopeFuncs *)global( LWENVELOPEFUNCS_GLOBAL, GFUSE_ACQUIRE );
    vparam_funcs = (LWVParmFuncs    *)global( LWVPARMFUNCS_GLOBAL,    GFUSE_ACQUIRE );
    item_info    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_ACQUIRE );
    light_info   = (LWLightInfo     *)global( LWLIGHTINFO_GLOBAL,     GFUSE_ACQUIRE );
    object_info  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_ACQUIRE );
    fileio       = (LWFileIOFuncs   *)global( LWFILEIOFUNCS_GLOBAL,   GFUSE_ACQUIRE );
    imp          = (IMPGlobal       *)global( IMP_GLOBAL,             GFUSE_ACQUIRE );

    imp->registerBank( &bank_IllumiSurfBasic );
    imp->registerBank( &bank_IllumiSurfShading );
    imp->registerBank( &bank_IllumiSurfReflection );
    imp->registerBank( &bank_IllumiSurfTransmission );
  }

  IllumiSurf_Instance *inst = new IllumiSurf_Instance( (LWSurfaceID)context );
  instances.Add( inst );
  return inst;
}

// IllumiSurf_Destroy():
void IllumiSurf_Destroy( LWInstance _inst ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;
  instances.RemoveItem( inst );

  if( copy_inst == inst ) {
    copy_inst = NULL;
    if( is_int != NULL ) {
      GHOST_CON( ((LWControl *)is_int->paste_button) );
      GHOST_CON( ((LWControl *)is_int->paste_popup)  );
    }
  }

  if( instances.NumElements() == 0 ) {
    imp->unregisterBank( bank_IllumiSurfBasic.id );
    imp->unregisterBank( bank_IllumiSurfShading.id );
    imp->unregisterBank( bank_IllumiSurfReflection.id );
    imp->unregisterBank( bank_IllumiSurfTransmission.id );

    global( LWSURFACEFUNCS_GLOBAL,  GFUSE_RELEASE );
    global( LWENVELOPEFUNCS_GLOBAL, GFUSE_RELEASE );
    global( LWVPARMFUNCS_GLOBAL,    GFUSE_RELEASE );
    global( LWITEMINFO_GLOBAL,      GFUSE_RELEASE );
    global( LWLIGHTINFO_GLOBAL,     GFUSE_RELEASE );
    global( LWOBJECTINFO_GLOBAL,    GFUSE_RELEASE );
    global( LWFILEIOFUNCS_GLOBAL,   GFUSE_RELEASE );
    global( IMP_GLOBAL,             GFUSE_RELEASE );

    if( is_int != NULL ) {
      delete is_int;
      is_int = NULL;
    }
  }

  delete inst;
}

// IllumiSurf_Copy():
LWError IllumiSurf_Copy( LWInstance _to, LWInstance _from ) {
  IllumiSurf_Instance *to   = (IllumiSurf_Instance *)_to;
  IllumiSurf_Instance *from = (IllumiSurf_Instance *)_from;
  
  *to = *from;

  return NULL;
}

// IllumiSurf_Load():
LWError IllumiSurf_Load( LWInstance _inst, const LWLoadState *loader ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;
  return inst->Load( loader );
}

// IllumiSurf_Save():
LWError IllumiSurf_Save( LWInstance _inst, const LWSaveState *saver ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;
  return inst->Save( saver );
}

// IllumiSurf_Describe():
const char * IllumiSurf_Describe( LWInstance _inst ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;
  return "IllumiSurf"DEBUG_PLUGIN_USERNAME;
}

// IllumiSurf_Evaluate():
void IllumiSurf_Evaluate( LWInstance _inst, LWShaderAccess *access ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;
  LWMessageFuncs *message = (LWMessageFuncs *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  // If this is the sample sphere, just return
  if( ((int)access->objID & 0xffff) == 0x7fff )
    return;

  double total_intensity = 0.0;

  // Compute the illumination at this point
  for( LWItemID light = item_info->first( LWI_LIGHT, NULL ); light != NULL; light = item_info->next( light ) ) {
    // Skip excluded lights
    if( object_info->excluded( access->objID, light ) == 1 )
      continue;

    // Skip lights with their diff/spec flags off
    int flags = light_info->flags( light );
    if( (flags & LWLFL_NO_DIFFUSE) && (flags & LWLFL_NO_SPECULAR) )
      continue;

    // Skip zero intensity lights
    double intensity = light_info->intensity( light, curTime );
    if( (intensity < 0.000000001) && (intensity > -0.000000001) )
      continue;

    // Get some info about the light
    LWDVector light_wpos, light_forward;
    item_info->param( light, LWIP_W_POSITION, curTime, light_wpos    );
    item_info->param( light, LWIP_FORWARD,    curTime, light_forward );
    LWVecMath::normalize( light_forward );

    int    light_type   = light_info->type(      light );
    int    falloff_type = light_info->falloff(   light );
    double range        = light_info->range(     light, curTime );

    // Figure out how far away the light is from the surface
    LWDVector vector_to_light, vector_from_light;
    LWVecMath::sub( light_wpos, access->wPos, vector_to_light   );
    LWVecMath::sub( access->wPos, light_wpos, vector_from_light );

    double distance = LWVecMath::len( vector_to_light );

    // See if the distance is beyond the falloff range and compute the spot intensity
    double spot_intensity = 0.0;
    if( (light_type == LWLIGHT_DISTANT) || (falloff_type == LWLFALL_OFF) ) {
      spot_intensity = intensity;
    } else {
      switch( falloff_type ) {
        case LWLFALL_LINEAR:
          if( distance > range )
            continue;
          else
            spot_intensity = intensity * (1.0 - distance / range);
          break;

        case LWLFALL_INV_DIST:
          break;

        case LWLFALL_INV_DIST_2:
          break;
      }
    }

    // See if the light is hitting the surface based on light type
    LWVecMath::normalize( vector_to_light   );
    LWVecMath::normalize( vector_from_light );

    double incidence_angle = 0.0;

    switch( light_type ) {
      case LWLIGHT_DISTANT:
        // We just need to know if the light is pointing in the direction of the surface
        incidence_angle = -LWVecMath::dot( access->wNorm0, light_forward );
        break;

      case LWLIGHT_POINT:
        // We just need to know if the light is in front of the surface
        incidence_angle = LWVecMath::dot( access->wNorm0, vector_to_light );
        break;

      case LWLIGHT_SPOT: {
        // Find the angle of the light to the surface, as with point lights, and see if it's in front of the surface
        double light_incidence_angle = LWVecMath::dot( access->wNorm0, vector_to_light );
        if( light_incidence_angle < 0.0 )
          break;

        // Find the angle of the light to the surface
        double forward_incidence_angle = LWVecMath::dot( vector_from_light /*access->wNorm0*/, light_forward );
        double ray_angle = acos( forward_incidence_angle );

        // Check the ray angle against the cone sizes
        double cone_angle, soft_angle;
        light_info->coneAngles( light, curTime, &cone_angle, &soft_angle );

        if( ray_angle < cone_angle ) {
          if( ray_angle > cone_angle - soft_angle ) {
            // In the soft edge cone; modify the incidence angle accordingly
            double soft_amount = cone_angle - ray_angle;
            double ratio       = soft_amount / soft_angle;
            incidence_angle    = light_incidence_angle * ratio;
          } else {
            // In the main cone; use the light incidence angle
            incidence_angle = light_incidence_angle;
          }
        }
      } break;

      case LWLIGHT_LINEAR:
      case LWLIGHT_AREA:
      default:
        break;
    }

    if( incidence_angle <= 0.0 )
      continue;

    double ray_occlusion = incidence_angle;

    // Raytrace Shadows
    if( access->rayShade != NULL ) {
      LWShaderAccess shade;
      memset( &shade, 0, sizeof( shade ) );
      access->wPos[0] = light_wpos[0];
      access->wPos[1] = light_wpos[1];
      access->wPos[2] = light_wpos[2];

      LWDVector ray_vector;
      if( light_type == LWLIGHT_DISTANT ) {
        ray_vector[0] = light_forward[0];
        ray_vector[1] = light_forward[1];
        ray_vector[2] = light_forward[2];
      } else {
        ray_vector[0] = vector_from_light[0];
        ray_vector[1] = vector_from_light[1];
        ray_vector[2] = vector_from_light[2];
      }

      double total_ray_length = 0.0, ray_length;
//      while( true ) {
      for( int t=0; t < 1; t++ ) {
        // Stop if we hit infinity
        ray_length = access->rayShade( light_wpos, ray_vector, &shade );
        if( ray_length <= -1.0 )
          break;

        // Stop if the ray is beyond the surface
        total_ray_length += ray_length;
        if( total_ray_length >= distance )
          break;

        // See if the ray is blocked by another surface
        if( shade.transparency <= 0.0 )
          break;

        // Modify by the surface's transparency
        ray_occlusion *= access->transparency;
        if( ray_occlusion <= 0.0 )
          break;
      }

      if( ray_occlusion <= 0.0 )
        continue;
    }

    // Compute the final intensity
    double final_intensity = spot_intensity * ray_occlusion;

    total_intensity += final_intensity;
  }

  // Decide how to modify the surface values
  double base_intensity[3], alt_intensity[3];
  vparam_funcs->getVal( inst->GetBaseIntensity(), curTime, NULL, base_intensity );
  vparam_funcs->getVal( inst->GetAltIntensity(),  curTime, NULL, alt_intensity  );

  double range_start, range_end;
  bool   base_low;

  if( base_intensity < alt_intensity ) {
    range_start = base_intensity[0];
    range_end   = alt_intensity[0];
    base_low = true;
  } else {
    range_start = alt_intensity[0];
    range_end   = base_intensity[0];
    base_low = false;
  }

  // Handle Color
  if( inst->GetUseColor() ) {
    double color[3];
    vparam_funcs->getVal( inst->GetColor(), curTime, NULL, color );
    Intensify( access->color, 3, total_intensity, access->color, color, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Diffusion
  if( inst->GetUseDiffusion() ) {
    double diffusion[3];
    vparam_funcs->getVal( inst->GetDiffusion(), curTime, NULL, diffusion );
    Intensify( &access->diffuse, 1, total_intensity, &access->diffuse, diffusion, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Luminosity
  if( inst->GetUseLuminosity() ) {
    double luminosity[3];
    vparam_funcs->getVal( inst->GetLuminosity(), curTime, NULL, luminosity );
    Intensify( &access->luminous, 1, total_intensity, &access->luminous, luminosity, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Specular
  if( inst->GetUseSpecular() ) {
    double specular[3];
    vparam_funcs->getVal( inst->GetSpecular(), curTime, NULL, specular );
    Intensify( &access->specular, 1, total_intensity, &access->specular, specular, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Glossiness
  if( inst->GetUseGlossiness() ) {
    double glossiness[3];
    vparam_funcs->getVal( inst->GetGlossiness(), curTime, NULL, glossiness );
    Intensify( &access->roughness, 1, total_intensity, &access->roughness, glossiness, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Reflection
  if( inst->GetUseReflection() ) {
    double reflection[3];
    vparam_funcs->getVal( inst->GetReflection(), curTime, NULL, reflection );
    Intensify( &access->mirror, 1, total_intensity, &access->mirror, reflection, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Transparency
  if( inst->GetUseTransparency() ) {
    double transparency[3];
    vparam_funcs->getVal( inst->GetTransparency(), curTime, NULL, transparency );
    Intensify( &access->transparency, 1, total_intensity, &access->transparency, transparency, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Refraction
  if( inst->GetUseRefraction() ) {
    double refraction[3];
    vparam_funcs->getVal( inst->GetRefraction(), curTime, NULL, refraction );
    Intensify( &access->eta, 1, total_intensity, &access->eta, refraction, range_start, range_end, base_low, inst->GetBlendMode() );
  }

  // Handle Translucency
  if( inst->GetUseTranslucency() ) {
    double translucency[3];
    vparam_funcs->getVal( inst->GetTranslucency(), curTime, NULL, translucency );
    Intensify( &access->translucency, 1, total_intensity, &access->translucency, translucency, range_start, range_end, base_low, inst->GetBlendMode() );
  }
}

// Intensify()
void Intensify( double *outputs, int output_count, double total_intensity, double *base, double *alt, double low, double high, bool base_low, IllumiSurf_BlendModes blend_mode ) {
  double ratio;
  for( int i=0; i < output_count; i++ ) {
    if( total_intensity <= low ) {
      // Before the start of the range
      ratio = 0.0;

    } else if( total_intensity >= high ) {
      // Beyond the end of the range
      ratio = 1.0;
    } else {

      // Within range; blend
      double range = high - low;
      ratio = (total_intensity - low) / range;
    }

    // Perform Blending
    switch( blend_mode ) {
      case ISBLEND_REPLACE:
        if( base_low )
          outputs[i] = (alt[i] * ratio) + (base[i] * (1.0 - ratio));
        else
          outputs[i] = (base[i] * ratio) + (alt[i] * (1.0 - ratio));
        break;

      case ISBLEND_ADD:
        if( base_low )
          outputs[i] = base[i] + (alt[i] * ratio);
        else
          outputs[i] = alt[i] + (base[i] * ratio);
        break;

      case ISBLEND_SUBTRACT:
        if( base_low )
          outputs[i] = base[i] - (alt[i] * ratio);
        else
          outputs[i] = alt[i] - (base[i] * ratio);
        break;

      case ISBLEND_MULTIPLY:
        if( base_low )
          outputs[i] = base[i] * (alt[i] * ratio);
        else
          outputs[i] = alt[i] * (base[i] * ratio);
        break;

      case ISBLEND_DIVIDE:
        if( base_low ) {
          double mod = (alt[i] * ratio);
          if( mod > 0.000000001 )
            outputs[i] = base[i] / mod;
          else
            outputs[i] = 0.0;
        } else {
          double mod = (base[i] * ratio);
          if( mod > 0.000000001 )
            outputs[i] = alt[i] / mod;
          else
            outputs[i] = 0.0;
        }
        break;
    }
  }

  return;
}

// IllumiSurf_Flags():
unsigned int IllumiSurf_Flags( LWInstance _inst ) {
  return LWSHF_RAYTRACE | LWSHF_LUMINOUS | LWSHF_DIFFUSE;
}

// IllumiSurf_UseItems():
const LWItemID * IllumiSurf_UseItems( LWInstance _inst ) {
  return NULL;
}

// IllumiSurf_Init():
LWError IllumiSurf_Init( LWInstance _inst, int mode ) {
  return NULL;
}

// IllumiSurf_CleanUp():
void IllumiSurf_CleanUp( LWInstance _inst) {
  ;
}

// IllumiSurf_NewTime():
LWError IllumiSurf_NewTime( LWInstance _inst, LWFrame frame, LWTime time) {
  curTime = time;
  return NULL;
}

// IllumiSurf_ChangeID():
void IllumiSurf_ChangeID( LWInstance _inst, const LWItemID *ids ) {
  return;
}

// IllumiSurf_Interface_Activate():
XCALL_ (int) IllumiSurf_Interface_Activate( long version, GlobalFunc *_global,
                                            void *_local, void *serverData ) {
  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  local->panel   = NULL;
  local->options = IllumiSurf_OpenUI;
  local->command = NULL;

  global = _global;

  return AFUNC_OK;
}

// IllumiSurf_OpenUI():
LWError IllumiSurf_OpenUI( void *_inst ) {
  IllumiSurf_Instance *inst = (IllumiSurf_Instance *)_inst;

  // Open the UI if it isn't already open
  if( is_int == NULL ) {
    new IllumiSurf_Interface( inst );

    if( !is_int->IsCreationOK() ) {
      delete is_int;
      is_int = NULL;
      return NULL;
    }
  }

  if( is_int->IsOpen() ) {
    unsigned long index = instances.FindIndexOf( inst );
    SET_INT( ((LWControl *)is_int->instance_popup), index );
    HandleInstancePopup( (LWControl *)is_int->instance_popup, NULL );
  } else {
    is_int->OpenUI();
  }

  return NULL;
}

