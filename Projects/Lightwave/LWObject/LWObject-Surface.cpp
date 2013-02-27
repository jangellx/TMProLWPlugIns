//
// LWobject-Surface.cpp
//

#include "lwobject-surface.h"

// Constructor
LWObject_Surface::LWObject_Surface() {
  name = NULL;
  parent_name = NULL;

  color.SetR(200.0f/255.0f);
  color.SetG(200.0f/255.0f);
  color.SetB(200.0f/255.0f);
  color_envelope_index                  = -1;

  diffusion                             = 0.0f;
  diffusion_envelope_index              = -1;

  diffuse_sharpness                     = 0.0f;
  diffuse_sharpness_envelope_index      = -1;

  luminosity                            = 0.0f;
  luminosity_envelope_index             = -1;

  specular                              = 0.0f;
  specular_envelope_index               = -1;

  glossiness                            = 0.4f;
  glossiness_envelope_index             = -1;

  color_highlights                      = 0.0f;
  color_highlights_envelope_index       = -1;

  reflectivity                          = 0.0f;
  reflectivity_envelope_index           = -1;

  transparency                          = 0.0f;
  transparency_envelope_index           = -1;

  color_filter                          = 0.0f;
  color_filter_envelope_index           = -1;

  translucency                          = 0.0f;
  translucency_envelope_index           = -1;

  refractive_index                      = 0.0f;
  refractive_index_envelope_index       = -1;

  additive_transparency                 = 0.0f;
  additive_transparency_envelope_index  = -1;

  translucency                          = 0.0f;
  translucency_envelope_index           = -1;

  bump                                  = 1.0f;
  bump_envelope_index                   = -1;

  is_double_sided                       = false;
  max_smoothing_angle                   = 0.0f;

  reflection_options                    = 0;
  transparency_options                  = 0;

  alpha_mode                            = 0;
  alpha_value                           = 1.0f;
}

// Destructor
LWObject_Surface::~LWObject_Surface() {
  SetName( NULL );
  SetParentName( NULL );
}

// Read():
bool LWObject_Surface::Read( LWbifstream &in, unsigned long chunk_size ) {
  if( chunk_size == 0 )
    return true;

  // Surface Name
  char buffer[256];
  in.GetString( buffer, 256 );
  SetName( buffer );

  // Parent Name
  in.GetString( buffer, 256 );
  SetParentName( buffer );

  // Sub-Chunk Loop
  streampos chunk_start = in.tellg();
  unsigned long subchunk_id, subchunk_size;

  while( !in.eof() ) {
    // Test for errors
    if( in.bad() || in.fail() )
      throw LWOERROR_ERROR_READING_FILE;

    // Get the basic chunk data
    chunk_start = in.tellg();
    in >> subchunk_id >> subchunk_size;

    // Figure out what kind of chunk this is
    switch( subchunk_id ) {
      case LWObject_SURFChunk_COLR:               // COLR
        color.Read( in, false );
        color_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_DIFF:               // DIFF
        in >> diffusion;
        diffusion_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_SHRP:               // SHRP
        in >> diffuse_sharpness;
        diffuse_sharpness_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_LUMI:               // LUMI
        in >> luminosity;
        luminosity_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_SPEC:               // SPEC
        in >> specular;
        specular_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_GLOS:               // GLOS
        in >> glossiness;
        glossiness_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_REFL:               // REFL
        in >> reflectivity;
        reflectivity_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_TRAN:               // TRAN
        in >> transparency;
        transparency_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_TRNL:               // TRNL
        in >> translucency;
        translucency_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_BUMP:               // BUMP
        in >> bump;
        bump_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_SIDE : {            // SIDE
        unsigned short temps;
        in >> temps;
        is_double_sided = (temps == 3) ? true : false;
      } break;

      case LWObject_SURFChunk_SMAN:               // SMAN
        in >> max_smoothing_angle;
        break;

      case LWObject_SURFChunk_RFOP:               // RFOP
        in >> reflection_options;
        break;

      case LWObject_SURFChunk_TROP:               // TROP
        in >> transparency_options;
        break;

      case LWObject_SURFChunk_CLRH:               // CLRH
        in >> color_highlights;
        color_highlights_envelope_index = in.ReadVX() - 1;
        break;

      case LWObject_SURFChunk_CLRF:               // CLRF
        in >> color_filter;
        color_filter_envelope_index = in.ReadVX() - 1;
        break;

      default:                                    // Unknown Chunk
        in.seekg( subchunk_size, ios::cur );
    }

    // Skip another byte if the chunk size is odd (chunks must be even sized)
    if( (chunk_size % 2) > 0 )
      in.seekg( 1, ios::cur );
  }

  return true;
}

// OutputToStream():
//  Outputs some debugging-like data to the straem provided
void LWObject_Surface::OutputToStream( ostream &out ) {
  out << "        Surface Name:                                 " << ((name        == NULL) ? "(unknown)" : name)        << endl;
  out << "        Parent Surface Name:                          " << ((parent_name == NULL) ? "(unknown)" : parent_name) << endl;
  out << "        - Surface Channels"                            << endl;
  out << "            Color:                                    " << color << endl;
  out << "              Color Envelope Index:                   " << color_envelope_index << endl;
  out << "            Diffusion:                                " << diffusion << endl;
  out << "              Diffusion Envelope Index:               " << diffusion_envelope_index << endl;
  out << "            Diffuse Sharpness:                        " << diffuse_sharpness << endl;
  out << "              Diffusion Sharpness Envelope Index:     " << diffuse_sharpness_envelope_index << endl;
  out << "            Luminosity:                               " << luminosity << endl;
  out << "              Luminosity Envelope Index:              " << luminosity_envelope_index << endl;
  out << "            Specular:                                 " << specular << endl;
  out << "              Specular Envelope Index:                " << specular_envelope_index << endl;
  out << "            Glossiness:                               " << glossiness << endl;
  out << "              Glossiness Envelope Index:              " << glossiness_envelope_index << endl;
  out << "            Color Highlights:                         " << color_highlights << endl;
  out << "              Color Highlights Envelope Index:        " << color_highlights_envelope_index << endl;
  out << "            Reflectivity:                             " << reflectivity << endl;
  out << "              Reflectivity Envelope Index:            " << reflectivity_envelope_index << endl;
  out << "            Transparency:                             " << transparency << endl;
  out << "              Transparency Envelope Index:            " << transparency_envelope_index << endl;
  out << "              Refractive Index:                       " << refractive_index << endl;
  out << "                Refractive Index Envelope Index:      " << refractive_index_envelope_index << endl;
  out << "              Additive Transparency:                  " << additive_transparency << endl;
  out << "                Additive Transparency Envelope Index: " << additive_transparency_envelope_index << endl;
  out << "              Color Filter:                           " << color_filter << endl;
  out << "                Color Filter Envelope Index:          " << color_filter_envelope_index << endl;
  out << "            Translucency Index:                       " << translucency << endl;
  out << "              Translucency Envelope Index:            " << translucency_envelope_index << endl;
  out << "            Bump Index:                               " << bump << endl;
  out << "              Bump Envelope Index:                    " << bump_envelope_index << endl;
  out << "        - Surface Options"                           << endl;
  out << "            Double Sided?                             " << (is_double_sided ? "Yes" : "No") << endl;
  out << "            Smoothed?                                 " << (IsSmoothed() ? "Yes" : "No") << endl;
  out << "              Max Smoothing Angle (radians)           " << max_smoothing_angle << endl;
  out << "            Alpha Mode                                " << alpha_mode << endl;
  out << "              Alpha Value                             " << alpha_value << endl;
  out << "            Reflection Options                        " << reflection_options << endl;
  out << "            Transparecny Options                      " << transparency_options << endl;
}

