//
// IllumiSurf-Instance.h
//

#ifndef ILLUMISURF_INSTANCE_HEADER
#define ILLUMISURF_INSTANCE_HEADER

#ifndef NULL
#define NULL 0
#endif

#include "illumisurf-base.h"

// GIP User Data
enum IllumiSurf_GIP_UserData {
  ISGIPUSERDATA_BASE_INTENSITY,
  ISGIPUSERDATA_ALT_INTENSITY,
  ISGIPUSERDATA_COLOR,
  ISGIPUSERDATA_LUMINOSITY,
  ISGIPUSERDATA_DIFFUSION,
  ISGIPUSERDATA_SPECULAR,
  ISGIPUSERDATA_GLOSSINESS,
  ISGIPUSERDATA_REFLECTION,
  ISGIPUSERDATA_TRANSPARENCY,
  ISGIPUSERDATA_REFRACTION,
  ISGIPUSERDATA_TRANSLUCENCY,
};

// Blending Modes
enum IllumiSurf_BlendModes {
  ISBLEND_REPLACE,
  ISBLEND_ADD,
  ISBLEND_SUBTRACT,
  ISBLEND_MULTIPLY,
  ISBLEND_DIVIDE,
};

extern const char *IllumiSurf_BlendModeStrings[];

// class IllumiSurf_Instance
class IllumiSurf_Instance {
public:
  IllumiSurf_Instance( LWSurfaceID surface );
  ~IllumiSurf_Instance();

  IllumiSurf_Instance & operator=( IllumiSurf_Instance &other_inst );

  const char * LoadFromFile( const char *filename );
  const char * SaveToFile(   const char *filename );

  const char * Load( const LWLoadState *loader );
  const char * Save( const LWSaveState *saver  );

  // Accessors
  LWSurfaceID   GetSurface()      { return surface; }
  LWChanGroupID GetChannelGroup() { return channel_group; }

  IllumiSurf_BlendModes GetBlendMode()                              { return blend_mode;  }
  void                  SetBlendMode( IllumiSurf_BlendModes _mode ) { blend_mode = _mode; }

  LWVParmID   GetBaseIntensity()              { return base_intensity; }
  LWVParmID   GetAltIntensity()               { return alt_intensity;  }

  LWVParmID   GetColor()                      { return color;        }
  LWVParmID   GetLuminosity()                 { return luminosity;   }
  LWVParmID   GetDiffusion()                  { return diffusion;    }
  LWVParmID   GetSpecular()                   { return specular;     }
  LWVParmID   GetGlossiness()                 { return glossiness;   }
  LWVParmID   GetReflection()                 { return reflection;   }
  LWVParmID   GetTransparency()               { return transparency; }
  LWVParmID   GetRefraction()                 { return refraction;   }
  LWVParmID   GetTranslucency()               { return translucency; }

  bool        GetUseColor()                   { return use_color; }
  void        SetUseColor( bool _state )      { use_color = _state; }

  bool        GetUseLuminosity()              { return use_luminosity; }
  void        SetUseLuminosity( bool _state ) { use_luminosity = _state; }

  bool        GetUseDiffusion()               { return use_diffusion; }
  void        SetUseDiffusion( bool _state )  { use_diffusion = _state; }

  bool        GetUseSpecular()                { return use_specular; }
  void        SetUseSpecular( bool _state )   { use_specular = _state; }

  bool        GetUseGlossiness()              { return use_glossiness; }
  void        SetUseGlossiness( bool _state ) { use_glossiness = _state; }

  bool        GetUseReflection()              { return use_reflection; }
  void        SetUseReflection( bool _state ) { use_reflection = _state; }

  bool        GetUseTransparency()              { return use_transparency; }
  void        SetUseTransparency( bool _state ) { use_transparency = _state; }

  bool        GetUseRefraction()                { return use_refraction; }
  void        SetUseRefraction( bool _state )   { use_refraction = _state; }

  bool        GetUseTranslucency()              { return use_translucency; }
  void        SetUseTranslucency( bool _state ) { use_translucency = _state; }

protected:
  LWSurfaceID    surface;
  LWChanGroupID  channel_group;

  IllumiSurf_BlendModes blend_mode;

  LWVParmID      base_intensity;
  LWVParmID      alt_intensity;

  LWVParmID      color;
  LWVParmID      luminosity;
  LWVParmID      diffusion;
  LWVParmID      specular;
  LWVParmID      glossiness;
  LWVParmID      reflection;
  LWVParmID      transparency;
  LWVParmID      refraction;
  LWVParmID      translucency;

  bool           use_color;
  bool           use_luminosity;
  bool           use_diffusion;
  bool           use_specular;
  bool           use_glossiness;
  bool           use_reflection;
  bool           use_transparency;
  bool           use_refraction;
  bool           use_translucency;
};

#endif
