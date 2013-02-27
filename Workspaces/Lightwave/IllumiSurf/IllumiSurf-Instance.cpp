//
// IllumiSurf-Instance.cpp
//

#include "illumisurf-instance.h"
#include <portable/dynarray/dynarray.h>

extern GlobalFunc *global;
extern DynArray< IllumiSurf_Instance * > instances;

extern LWSurfaceFuncs  * surf_funcs;
extern LWEnvelopeFuncs * env_funcs;
extern LWVParmFuncs    * vparam_funcs;

const char *IllumiSurf_BlendModeStrings[] = {
  "Replace",
  "Add",
  "Subtract",
  "Multiply",
  "Divide",
  NULL };

// Constructor():
IllumiSurf_Instance::IllumiSurf_Instance( LWSurfaceID _surface )
  : surface(_surface), blend_mode(ISBLEND_REPLACE),
    use_color(false), use_diffusion(false), use_luminosity(false),
    use_specular(false), use_glossiness(false), use_reflection(false),
    use_transparency(false), use_refraction(false), use_translucency(false) {

  // Create the new channel group in the surface's group
  LWChanGroupID surf_group = surf_funcs->chanGrp( surface );
  channel_group = env_funcs->createGroup( surf_group, "IllumiSurf" );

  // Create the base intensity vparam
  base_intensity = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( base_intensity, "BaseIntensity", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_BASE_INTENSITY );

  double value = 0.0;
  vparam_funcs->setVal( base_intensity, &value );

  // Create the alternate intensity vparam
  alt_intensity = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( alt_intensity, "AltIntensity", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_ALT_INTENSITY );

  value = 1.0;
  vparam_funcs->setVal( alt_intensity, &value );

  // Create the color vparam
  color = vparam_funcs->create( LWVP_COLOR, LWVPDT_NOTXTR );
  vparam_funcs->setup( color, "Color", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_COLOR );

  double values[3] = { 200.0/255.0, 200.0/255.0, 200.0/255.0 };
  vparam_funcs->setVal( color, values );

  // Create the luminosity vparam
  luminosity = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( luminosity, "Luminous", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_LUMINOSITY );

  value = 0.0;
  vparam_funcs->setVal( luminosity, &value );

  // Create the diffusion vparam
  diffusion = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( diffusion, "Diffuse", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_DIFFUSION );

  value = 1.0;
  vparam_funcs->setVal( diffusion, &value );

  // Create the specular vparam
  specular = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( specular, "Specular", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_SPECULAR );

  value = 0.0;
  vparam_funcs->setVal( specular, &value );

  // Create the glossiness vparam
  glossiness = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( glossiness, "Glossiness", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_GLOSSINESS );

  value = 0.4;
  vparam_funcs->setVal( glossiness, &value );

  // Create the reflection vparam
  reflection = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( reflection, "Reflection", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_REFLECTION );

  value = 0.0;
  vparam_funcs->setVal( reflection, &value );

  // Create the transparency vparam
  transparency = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( transparency, "Transparency", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_TRANSPARENCY );

  value = 0.0;
  vparam_funcs->setVal( transparency, &value );

  // Create the refraction vparam
  refraction = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( refraction, "Refraction", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_REFRACTION );

  value = 1.0;
  vparam_funcs->setVal( refraction, &value );

  // Create the translucency vparam
  translucency = vparam_funcs->create( LWVP_PERCENT, LWVPDT_NOTXTR );
  vparam_funcs->setup( translucency, "Translucency", channel_group, NULL, NULL, SHADER_PLUGIN_NAME, (void *)ISGIPUSERDATA_TRANSLUCENCY );

  value = 0.0;
  vparam_funcs->setVal( translucency, &value );
}

// Destructor():
IllumiSurf_Instance::~IllumiSurf_Instance() {
  vparam_funcs->destroy( color        );
  vparam_funcs->destroy( luminosity   );
  vparam_funcs->destroy( diffusion    );
  vparam_funcs->destroy( specular     );
  vparam_funcs->destroy( glossiness   );
  vparam_funcs->destroy( reflection   );
  vparam_funcs->destroy( transparency );
  vparam_funcs->destroy( refraction   );
  vparam_funcs->destroy( translucency );

  env_funcs->destroyGroup( channel_group );
}

// operator=():
IllumiSurf_Instance & IllumiSurf_Instance::operator=( IllumiSurf_Instance &other_inst ) {
  //surface = other_inst.GetSurface();  // - Do NOT copy the surface; that's unique to the instance

  blend_mode       = other_inst.GetBlendMode();

  use_color        = other_inst.GetUseColor();
  use_luminosity   = other_inst.GetUseLuminosity();
  use_diffusion    = other_inst.GetUseDiffusion();
  use_specular     = other_inst.GetUseSpecular();
  use_glossiness   = other_inst.GetUseGlossiness();
  use_reflection   = other_inst.GetUseReflection();
  use_transparency = other_inst.GetUseTransparency();
  use_refraction   = other_inst.GetUseRefraction();
  use_translucency = other_inst.GetUseTranslucency();

  vparam_funcs->copy( base_intensity, other_inst.GetBaseIntensity() );
  vparam_funcs->copy( alt_intensity,  other_inst.GetAltIntensity()  );

  vparam_funcs->copy( color,        other_inst.GetColor()        );
  vparam_funcs->copy( luminosity,   other_inst.GetLuminosity()   );
  vparam_funcs->copy( diffusion,    other_inst.GetDiffusion()    );
  vparam_funcs->copy( specular,     other_inst.GetSpecular()     );
  vparam_funcs->copy( glossiness,   other_inst.GetGlossiness()   );
  vparam_funcs->copy( reflection,   other_inst.GetReflection()   );
  vparam_funcs->copy( transparency, other_inst.GetTransparency() );
  vparam_funcs->copy( refraction,   other_inst.GetRefraction()   );
  vparam_funcs->copy( translucency, other_inst.GetTranslucency() );

  return *this;
}

// File I/O Setup
enum ISIOIDs_Base {
  ISIOID_BLND,
  ISIOID_BINT,
  ISIOID_AINT,
  ISIOID_COLR,
  ISIOID_LUMI,
  ISIOID_DIFF,
  ISIOID_SPEC,
  ISIOID_GLOS,
  ISIOID_REFL,
  ISIOID_TRNP,
  ISIOID_REFR,
  ISIOID_TRNC,
};

#define ISIOPRE_ISFV  LWID_( 'I','S','F','V' )

#define ISIOPRE_BLND  LWID_( 'B','L','N','D' )
#define ISIOPRE_BINT  LWID_( 'B','I','N','T' )
#define ISIOPRE_AINT  LWID_( 'A','I','N','T' )
#define ISIOPRE_COLR  LWID_( 'C','O','L','R' )
#define ISIOPRE_LUMI  LWID_( 'L','U','M','I' )
#define ISIOPRE_DIFF  LWID_( 'D','I','F','F' )
#define ISIOPRE_SPEC  LWID_( 'S','P','E','C' )
#define ISIOPRE_GLOS  LWID_( 'G','L','O','S' )
#define ISIOPRE_REFL  LWID_( 'R','E','F','L' )
#define ISIOPRE_TRNP  LWID_( 'T','R','B','P' )
#define ISIOPRE_REFR  LWID_( 'R','E','F','R' )
#define ISIOPRE_TRNC  LWID_( 'T','R','N','C' )

static LWBlockIdent illumisurf_io_isfv[] = {
  { ISIOPRE_ISFV, "IllumiSurf" },
  0 };

static LWBlockIdent illumisurf_io_root[] = {
  { ISIOPRE_BLND, "BlendingMode"     },
  { ISIOPRE_BINT, "BaseIntensity"    },
  { ISIOPRE_AINT, "AltIntensity"     },
  { ISIOPRE_COLR, "Color"            },
  { ISIOPRE_LUMI, "Luminosity"       },
  { ISIOPRE_DIFF, "Diffusion"        },
  { ISIOPRE_SPEC, "Specular"         },
  { ISIOPRE_GLOS, "Glossiness"       },
  { ISIOPRE_REFL, "Reflection"       },
  { ISIOPRE_TRNP, "Transparency"     },
  { ISIOPRE_REFR, "Refraction"       },
  { ISIOPRE_TRNC, "Translucency"     },
  0 };

// LoadFromFile():
const char * IllumiSurf_Instance::LoadFromFile( const char *filename ) {
  LWLoadState *loader = fileio->openLoad( filename, LWIO_BINARY );
  if( loader == NULL )
    return false;

  const char *retval = Load( loader );
  fileio->closeLoad( loader );

  return retval;
}

// Load():
const char * IllumiSurf_Instance::Load( const LWLoadState *loader ) {
  char cvalue;

  if( ISIOPRE_ISFV == LWLOAD_FIND( loader, illumisurf_io_isfv ) ) {
    LWLOAD_I1( loader, &cvalue, 1 );
    if( cvalue != 1 )
      return "IllumiSurf Error:  Unsupported version found, aborting load";

    while( int id = LWLOAD_FIND( loader, illumisurf_io_root ) ) {
      switch( id ) {
        case ISIOPRE_BLND:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetBlendMode( (IllumiSurf_BlendModes)cvalue );
          break;

        case ISIOPRE_BINT:
          vparam_funcs->load( GetBaseIntensity(), loader );
          break;

        case ISIOPRE_AINT:
          vparam_funcs->load( GetAltIntensity(), loader );
          break;

        case ISIOPRE_COLR:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseColor( cvalue != 0 );
          vparam_funcs->load( GetColor(), loader );
          break;

        case ISIOPRE_LUMI:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseLuminosity( cvalue != 0 );
          vparam_funcs->load( GetLuminosity(), loader );
          break;

        case ISIOPRE_DIFF:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseDiffusion( cvalue != 0 );
          vparam_funcs->load( GetDiffusion(), loader );
          break;

        case ISIOPRE_SPEC:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseSpecular( cvalue != 0 );
          vparam_funcs->load( GetSpecular(), loader );
          break;

        case ISIOPRE_GLOS:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseGlossiness( cvalue != 0 );
          vparam_funcs->load( GetGlossiness(), loader );
          break;

        case ISIOPRE_REFL:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseReflection( cvalue != 0 );
          vparam_funcs->load( GetReflection(), loader );
          break;

        case ISIOPRE_TRNP:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseTransparency( cvalue != 0 );
          vparam_funcs->load( GetTransparency(), loader );
          break;

        case ISIOPRE_REFR:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseRefraction( cvalue != 0 );
          vparam_funcs->load( GetRefraction(), loader );
          break;

        case ISIOPRE_TRNC:
          LWLOAD_I1( loader, &cvalue, 1 );
          SetUseTranslucency( cvalue != 0 );
          vparam_funcs->load( GetTranslucency(), loader );
          break;
      }

      LWLOAD_END( loader );            // End Keyword
    }

    LWLOAD_END( loader );            // End Keyword
  }

  return NULL;
}

// SaveToFile():
const char * IllumiSurf_Instance::SaveToFile( const char *filename ) {
  LWSaveState *saver = fileio->openSave( filename, LWIO_BINARY );
  if( saver == NULL )
    return false;

  const char *retval = Save( saver );
  fileio->closeSave( saver );

  return retval;
}

// Save():
const char * IllumiSurf_Instance::Save( const LWSaveState *saver ) {
  char cvalue;

  // Version
  LWSAVE_BEGIN( saver, &illumisurf_io_isfv[0], 0 );
  cvalue = 1;
  LWSAVE_I1( saver, &cvalue, 1 );

  // Blending Mode
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_BLND ], 1 );
  cvalue = GetBlendMode();
  LWSAVE_I1( saver, &cvalue, 1 );
  LWSAVE_END( saver );

  // Base Intensity
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_BINT ], 1 );
  vparam_funcs->save( GetBaseIntensity(), saver );
  LWSAVE_END( saver );

  // Alternate Intensity
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_AINT ], 1 );
  vparam_funcs->save( GetAltIntensity(), saver );
  LWSAVE_END( saver );

  // Color
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_COLR ], 1 );
  cvalue = GetUseColor() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetColor(), saver );
  LWSAVE_END( saver );

  // Luminosity
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_LUMI ], 1 );
  cvalue = GetUseLuminosity() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetLuminosity(), saver );
  LWSAVE_END( saver );

  // Diffusion
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_DIFF ], 1 );
  cvalue = GetUseDiffusion() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetDiffusion(), saver );
  LWSAVE_END( saver );

  // Specular
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_SPEC ], 1 );
  cvalue = GetUseSpecular() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetSpecular(), saver );
  LWSAVE_END( saver );

  // Glossiness
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_GLOS ], 1 );
  cvalue = GetUseGlossiness() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetGlossiness(), saver );
  LWSAVE_END( saver );

  // Reflection
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_REFL ], 1 );
  cvalue = GetUseReflection() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetReflection(), saver );
  LWSAVE_END( saver );

  // Transparency
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_TRNP ], 1 );
  cvalue = GetUseTransparency() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetTransparency(), saver );
  LWSAVE_END( saver );

  // Refraction
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_REFR ], 1 );
  cvalue = GetUseRefraction() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetRefraction(), saver );
  LWSAVE_END( saver );

  // Translucency
  LWSAVE_BEGIN( saver, &illumisurf_io_root[ ISIOID_TRNC ], 1 );
  cvalue = GetUseTranslucency() ? 1 : 0;
  LWSAVE_I1( saver, &cvalue, 1 );
  vparam_funcs->save( GetTranslucency(), saver );
  LWSAVE_END( saver );

  LWSAVE_END( saver );

  return NULL;
}

