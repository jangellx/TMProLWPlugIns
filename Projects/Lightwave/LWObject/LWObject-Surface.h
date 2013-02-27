//
// LWObject-Surface.h
//

#include <portable/stringtools/stringtools.h>

#include "lwbifstream.h"

#include "LWObject-Errors.h"
#include "LWObject-Chunks.h"

#include "LWObject-Color.h"

// LWObject_Surface
class LWObject_Surface {
public:
  LWObject_Surface();
  ~LWObject_Surface();

  // File I/O
  bool Read( LWbifstream &in, unsigned long chunk_size );
  void OutputToStream( ostream &out );

  // Basic Accessors
  inline const char *     GetName() { return name; }
  inline void             SetName( const char *_name ) { StringTools::strdup2( name, _name ); }

  inline const char *     GetParentName() { return parent_name; }
  inline void             SetParentName( const char *_name ) { StringTools::strdup2( parent_name, _name ); }

  // Channel Accessors
  inline LWObject_Color & GetColor() { return color; }
  inline unsigned long    GetColorEnvelopeIndex() { return color_envelope_index; }

  inline float            GetDiffusion() { return diffusion; }
  inline void             SetDiffusion( float _value ) { diffusion = _value; }
  inline unsigned long    GetDiffusionEnvelopeIndex() { return diffusion_envelope_index; }

  inline float            GetDiffuseSharpness() { return diffuse_sharpness; }
  inline void             SetDiffuseSharpness( float _value ) { diffuse_sharpness = _value; }
  inline unsigned long    GetDiffuseSharpnessEnvelopeIndex() { return diffuse_sharpness_envelope_index; }

  inline float            GetLuminosity() { return luminosity; }
  inline void             SetLuminosity( float _value ) { luminosity = _value; }
  inline unsigned long    GetLuminosityEnvelopeIndex() { return luminosity_envelope_index; }

  inline float            GetSpecular() { return specular; }
  inline void             SetSpecular( float _value ) { specular = _value; }
  inline unsigned long    GetSpecularEnvelopeIndex() { return specular_envelope_index; }

  inline float            GetGlossiness() { return glossiness; }
  inline void             SetGlossiness( float _value ) { glossiness = _value; }
  inline unsigned long    GetGlossinessEnvelopeIndex() { return glossiness_envelope_index; }

  inline float            GetColorHighlights() { return color_highlights; }
  inline void             SetColorHighlights( float _value ) { color_highlights = _value; }
  inline unsigned long    GetColorHighlightsEnvelopeIndex() { return color_highlights_envelope_index; }

  inline float            GetReflectivity() { return reflectivity; }
  inline void             SetReflectivity( float _value ) { reflectivity = _value; }
  inline unsigned long    GetReflectivityEnvelopeIndex() { return reflectivity_envelope_index; }

  inline float            GetTransparency() { return transparency; }
  inline void             SetTransparency( float _value ) { transparency = _value; }
  inline unsigned long    GetTransparencyEnvelopeIndex() { return transparency_envelope_index; }

  inline float            GetRefractiveIndex() { return refractive_index; }
  inline void             SetRefractiveIndex( float _value ) { refractive_index = _value; }
  inline unsigned long    GetRefractiveIndexEnvelopeIndex() { return refractive_index_envelope_index; }

  inline float            GetAdditiveTransparency() { return additive_transparency; }
  inline void             SetAdditiveTransparency( float _value ) { additive_transparency = _value; }
  inline unsigned long    GetAdditiveTransparencyEnvelopeIndex() { return additive_transparency_envelope_index; }

  inline float            GetColorFilter() { return color_filter; }
  inline void             SetColorFilter( float _value ) { color_filter = _value; }
  inline unsigned long    GetColorFilterEnvelopeIndex() { return color_filter_envelope_index; }

  inline float            GetTranslucency() { return translucency; }
  inline void             SetTranslucency( float _value ) { translucency = _value; }
  inline unsigned long    GetTranslucencyEnvelopeIndex() { return translucency_envelope_index; }

  inline float            GetBump() { return bump; }
  inline void             SetBump( float _value ) { bump = _value; }
  inline unsigned long    GetBumpEnvelopeIndex() { return bump_envelope_index; }

  // Surface Option Accessors
  inline bool             GetIsDoubleSided() { return is_double_sided; }
  inline void             SetIsDoubleSided( bool _state ) { is_double_sided = _state; }

  inline float            GetMaxSmoothingAngle() { return max_smoothing_angle; }
  inline void             SetMaxSmoothingAngle( float _value ) { max_smoothing_angle = _value; }
  inline bool             IsSmoothed() { return ((max_smoothing_angle <= 0) ? false : true); }

  inline unsigned short   GetAlphaMode() { return alpha_mode; }
  inline void             SetAlphaMode( unsigned short _mode ) { alpha_mode = _mode; }

  inline float            GetAlphaValue() { return alpha_value; }
  inline void             SetAlphaValue( float _value ) { alpha_value = _value; }

  inline unsigned short   GetReflectionOptions() { return reflection_options; }
  inline void             SetReflectionOptions( unsigned short _options ) { reflection_options = _options; }

  inline unsigned short   GetTransparencyOptions() { return transparency_options; }
  inline void             SetTransparencyOptions( unsigned short _options ) { transparency_options = _options; }

protected:
  char         * name;                              // Surface Name
  char         * parent_name;                       // Parent Surface's Name

  LWObject_Color color;                             // Color
  unsigned long  color_envelope_index;              // Color Envelope Index

  float          diffusion;                         // Diffusion
  unsigned long  diffusion_envelope_index;          // Diffusion Envelope Index

  float          diffuse_sharpness;                 // Glossiness
  unsigned long  diffuse_sharpness_envelope_index;  // Glossiness Envelope Index

  float          luminosity;                        // Luminosity
  unsigned long  luminosity_envelope_index;         // Luminosity Envelope Index

  float          specular;                          // Specular
  unsigned long  specular_envelope_index;           // Specular Envelope Index

  float          glossiness;                        // Glossiness
  unsigned long  glossiness_envelope_index;         // Glossiness Envelope Index

  float          color_highlights;                  // Color Highlights
  unsigned long  color_highlights_envelope_index;   // Color Highlights Envelope Index

  float          reflectivity;                      // Reflectivity
  unsigned long  reflectivity_envelope_index;       // Reflectivity Envelope Index

  float          transparency;                      // Transparency
  unsigned long  transparency_envelope_index;       // Transparency Envelope Index

  float          refractive_index;                  // Refractive Index
  unsigned long  refractive_index_envelope_index;   // Refractive Index Envelope Index

  float          additive_transparency;                 // Additive Transparency
  unsigned long  additive_transparency_envelope_index;  // Additive Transparency Envelope Index

  float          color_filter;                      // Color Filter
  unsigned long  color_filter_envelope_index;       // Color Filter Envelope Index

  float          translucency;                      // Translucency
  unsigned long  translucency_envelope_index;       // Translucency Envelope Index

  float          bump;                              // Translucency
  unsigned long  bump_envelope_index;               // Translucency Envelope Index

  bool           is_double_sided;                   // true if polygons are to be consided double sided
  float          max_smoothing_angle;               // Any two adjacent polygons will only be smoothed if their angle is less than this value.  Note this values is radians, not degrees

  unsigned short reflection_options;                // Reflection Option Flags
  unsigned short transparency_options;              // Transparency Option Flags

  unsigned short alpha_mode;                        // Alpha Mode Flats
  float          alpha_value;                       // Constant Alpha Value
};

