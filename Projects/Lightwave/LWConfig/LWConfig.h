//
// LWConfig.h
//
// LWConfig opens a Lightwave config file and loads data from
//  the config to memory.  It will handle any version config,
//  and optionally a lwext3.cfg, if provided.
// 
// Multiple configs can be loaded into the same instance.  More
//  recent configs will overwrite similar data already loaded
//  This can be toggled by setting the use_older flag of
//  LoadConfig() to true.
//  
// The following configuration formats are currently supported:
//  LW.cfg     (LW 5.6 and earlier)
//  LW3.cfg    (L6 and later)
//  LWEXT3.cfg (L6.5 and later)
//
// Note that after 5.6, there is no way to tell if this really
//  is a lw.cfg or not, as there is no in-file header.  We just
//  assume there's valid data in here.
//
// Class Overview:
//   LWConfig:      Main config file class, used to load and manage
//                   config data
//   LWPlugin:      Plug-in class, containing data about a single
//                   plug-in type by class.
//   LWInfoTags:    Info Tags class, containing L6.5-specific plug-in
//                   data.
//
// Note also that plug-ins are added in the order they are found in
//  the config file.  This means that ImageSavers can be tested by
//  index, for example.
// 

//
// Note:  This class was originally written for reading only key
//        parts of the config for specific applications.  Expand
//        as needed.
//

#include <portable/dynarray/dynarray.h>
#include <portable/pfstream/pfstream.h>
#include <portable/stringtools/stringtools.h>
#include <portable/dirtools/dirtools.h>

// Enums
enum LWConfigPluginClassID {
  // Layout/Common
  LWPLUGIN_SCENE_CONVERTER = 0,
  LWPLUGIN_IMAGE_LOADER,
  LWPLUGIN_IMAGE_SAVER,
  LWPLUGIN_ITEM_MOTION_HANDLER,
  LWPLUGIN_ITEM_MOTION_INTERFACE,
  LWPLUGIN_OBJECT_REPLACEMENT_HANDLER,
  LWPLUGIN_OBJECT_REPLACEMENT_INTERFACE,
  LWPLUGIN_DISPLACEMENT_HANDLER,
  LWPLUGIN_DISPLACEMENT_INTERFACE,
  LWPLUGIN_SHADER_HANDLER,
  LWPLUGIN_SHADER_INTERFACE,
  LWPLUGIN_PIXEL_FILTER_HANDLER,
  LWPLUGIN_PIXEL_FILTER_INTERFACE,
  LWPLUGIN_IMAGE_FILTER_HANDLER,
  LWPLUGIN_IMAGE_FILTER_INTERFACE,
  LWPLUGIN_FRAME_BUFFER_HANDLER,
  LWPLUGIN_FRAME_BUFFER_INTERFACE,
  LWPLUGIN_ANIM_SAVER_HANDLER,
  LWPLUGIN_ANIM_SAVER_INTERFACE,
  LWPLUGIN_LAYOUT_GENERIC,
  LWPLUGIN_GLOBAL,

  // Layout L6
  LWPLUGIN_ANIM_LOADER_HANDLER,
  LWPLUGIN_CHANNEL_HANDLER,
  LWPLUGIN_CHANNEL_INTERFACE,
  LWPLUGIN_CUSTOM_OBJECT_HANDLER,
  LWPLUGIN_CUSTOM_OBJECT_INTERFACE,
  LWPLUGIN_ENVIRONMENT_HANDLER,
  LWPLUGIN_ENVIRONMENT_INTERFACE,
  LWPLUGIN_MASTER_HANDLER,
  LWPLUGIN_MASTER_INTERFACE,
  LWPLUGIN_OBJECT_LOADER,
  LWPLUGIN_TEXTURE_HANDLER,
  LWPLUGIN_TEXTURE_INTERFACE,
  LWPLUGIN_VOLUMETRIC_HANDLER,
  LWPLUGIN_VOLUMETRIC_INTERFACE,

  // Modeler Plug-ins
  LWPLUGIN_COMMAND_SEQUENCE,
  LWPLUGIN_MESH_EDIT_DATA,

  // Modeler L6
  LWPLUGIN_MESH_EDIT_TOOL,
  
  // Unknown
  LWPLUGIN_UNKNOWN };

const long num_plugin_classes = LWPLUGIN_UNKNOWN + 1;

extern const char *LWConfigPluginClassStrings[];

enum LWConfigError {
  LWCONFIG_ERROR_OK = 0,
  LWCONFIG_ERROR_FILE_NOT_FOUND,
  LWCONFIG_ERROR_OPENING_FILE,
  LWCONFIG_ERROR_READING_FROM_FILE };

extern const char *LWConfigErrorStrings[];

// Classes
class LWConfig;
class LWPlugin;
class LWInfoTag;

// class LWConfig
class LWConfig {
public:
  LWConfig( const char *cfg = NULL, const char *ext = NULL, bool use_older = false );
  ~LWConfig();

  void Reset();

  LWConfigError LoadConfig( const char *cfg, bool use_older = false );

  // Accessors
  inline DynArray< LWPlugin * > & GetPluginClassList( unsigned long _class ) { return plugins[ _class ]; }

  inline bool                     AddPlugin( LWPlugin *plugin, bool overwrite = true );

  // Static Functions
  static inline LWConfigPluginClassID   StringToPluginClassID( const char *match );
  static inline const char            * PluginClassIDToString( LWConfigPluginClassID _id ) { return LWConfigPluginClassStrings[ _id ]; }

  static inline const char            * GetErrorString( LWConfigError _error ) { return LWConfigErrorStrings[ _error ]; }

protected:
  bool LoadPluginV1( pifstream &in, char *buffer, int size, bool use_older = false );
  bool LoadPluginV3( pifstream &in, char *buffer, int size, bool use_older = false );

  DynArray< LWPlugin *> plugins[ num_plugin_classes ];
};

// StringToPluginClassID():
//  Finds the plug-in class ID that matches the string passed in,
//   returning the class type or LWPLUGIN_UNKNOWN if no match is
//   found.
inline LWConfigPluginClassID LWConfig::StringToPluginClassID( const char *match ) {
  for( unsigned long i=0; i < num_plugin_classes; i++ ) {
    if( strcmp( LWConfigPluginClassStrings[i], match ) == 0 )
      return (LWConfigPluginClassID)i;
  }

  return LWPLUGIN_UNKNOWN;
}


// class LWPlugIn
class LWPlugin {
public:
  LWPlugin() : plugin_class(LWPLUGIN_UNKNOWN), plugin_name(NULL), plugin_path(NULL) { ; }
  ~LWPlugin() { SetPath(NULL); SetName(NULL); info_tags.Flush(); }

  // Accessors
  inline LWConfigPluginClassID   GetClass() { return plugin_class; }
  inline void                 SetClass( LWConfigPluginClassID _class ) { plugin_class = _class; }

  inline const char         * GetName() { return plugin_name; }
  inline void                 SetName( const char *_name ) { StringTools::strdup2( plugin_name, _name ); }

  inline const char         * GetPath() { return plugin_path; }
  inline void                 SetPath( const char *_path ) { StringTools::strdup2( plugin_path, _path ); }

  inline DynArray< LWInfoTag * > & GetInfoTags() { return info_tags; }

protected:
  LWConfigPluginClassID    plugin_class;         // Plug-In Class
  char                   * plugin_name;          // Plug-In Name
  char                   * plugin_path;          // Plug-In Path (Module)

  DynArray< LWInfoTag * >  info_tags;            // Info Tags Array
};



// class LWInfoTag
class LWInfoTag {
public:
  LWInfoTag() : id(0), data(NULL) { ; }
  ~LWInfoTag() { SetData(NULL); }

  // Accessors
  inline long                 GetID() { return id; }
  inline void                 SetID( long _id ) { id = _id; }

  inline const char         * GetData() { return data; }
  inline void                 SetData( const char *_data ) { StringTools::strdup2( data, _data ); }

protected:
  long   id;
  char * data;
};
