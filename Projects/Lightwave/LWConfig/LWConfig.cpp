//
// LWConfig.cpp
//

#include "LWConfig.h"

const char *LWConfigPluginClassStrings[] = {
  // Layout/Common Plug-ins
  "SceneConverter",
  "ImageLoader",
  "ImageSaver",
  "ItemMotionHandler",
  "ItemMotionInterface",
  "ObjReplacementHandler",
  "ObjReplacementInterface",
  "DisplacementHandler",
  "DisplacementInterface",
  "ShaderHandler",
  "ShaderInterface",
  "PixelFilterHandler",
  "PixelFilterInterface",
  "ImageFilterHandler",
  "ImageFilterInterface",
  "FrameBufferHandler",
  "FrameBufferInterface",
  "AnimSaverHandler",
  "AnimSaverInterface",
  "LayoutGeneric",
  "Global",

  // Layout L6
  "AnimLoaderHandler",
  "ChannelHandler",
  "ChannelInterface",
  "CustomObjHandler",
  "CustomObjInterface",
  "EnvironmentHandler",
  "EnvironmentInterface",
  "MasterHandler",
  "MasterInterface",
  "ObjectLoader",
  "TextureHandler",
  "TextureInterface",
  "VolumetricHandler",
  "VolumetricInterface",

  // Modeler Plug-ins
  "CommandSequence",
  "MeshDataEdit",

  // Modeler L6
  "MeshEditTool",
  
  // Unknown
  "<unknown>" };

extern const char *LWConfigErrorStrings[] = {
  "OK",
  "Config file not found",
  "File error occured opening config file for reading",
  "File I/O error occured reading from config file" };


// Constructor
LWConfig::LWConfig( const char *cfg, const char *ext, bool use_older ) {
  Reset();

  if( cfg != NULL )
    LoadConfig( cfg );

  if( ext != NULL )
    LoadConfig( ext, use_older );
}

// Destructor
LWConfig::~LWConfig() {
  for( int i=0; i < num_plugin_classes; i++ )
    plugins[i].Flush();
}

// Reset():
void LWConfig::Reset() {
  // Flush the plug-in lists
  for( int i=0; i < num_plugin_classes; i++ )
    plugins[i].Flush();
}


// LoadConfig():
//  Loads the config specified into the instance.  If use_older
//   is true, the then existing data will override incoming date;
//   otherwise, the new data will be used.  Returns a LWConfigError
//   if something went wrong.
LWConfigError LWConfig::LoadConfig( const char *cfg, bool use_older ) {
  // Open the config file
  if( !DirInfo::Exists( cfg ) )
    return LWCONFIG_ERROR_FILE_NOT_FOUND;

  pifstream in( cfg, ios::nocreate | ios::binary );
  if( !in )
    return LWCONFIG_ERROR_OPENING_FILE;

  in.SetAnyEOL( true );

  char buffer[ 8192 ];
  do {
    // Read in the next line
    in.getline( buffer, 8192 );

    // Error Test
    if( (in.bad() || in.fail()) && !in.eof() )
      return LWCONFIG_ERROR_READING_FROM_FILE;

    // EOF Test
    if( in.eof() )
      break;

    // Keyword Tests
    if( LoadPluginV1( in, buffer, 8192, use_older ) ) {
      ;
    } else if( LoadPluginV3( in, buffer, 8192, use_older ) ) {
      ;
    }
  } while( true );

  return LWCONFIG_ERROR_OK;
}

// LoadPluginV1():
//  Loads plug-in data from a LW 5.6 standard config.  Such plug-ins
//   start with the Plugin keyword.
bool LWConfig::LoadPluginV1( pifstream &in, char *buffer, int size, bool use_older ) {
  char * next_word = StringTools::SkipWhiteSpace( buffer );
  if( strncmp( next_word, "Plugin ", 7 ) != 0 )
    return false;

  LWPlugin *plugin = new LWPlugin;

  // Plugin Class
  char buffer2[1024];
  StringTools::IsolateWord( next_word, buffer2, 1, true, false );
  plugin->SetClass( StringToPluginClassID( buffer2 ) );

  // Plugin Name
  StringTools::IsolateWord( next_word, buffer2, 2, true, false );
  plugin->SetName( buffer2 );

  // Plugin Path
  StringTools::IsolateWord( next_word, buffer2, 3, true, false );
  plugin->SetPath( buffer2 );

  // Add the plug-in
  if( !AddPlugin( plugin, !use_older ) )
    delete plugin;

  return true;
}

// LoadPluginV3():
//  Loads plug-in data from an L6.5 standard config or ext config.
//   Such plug-ins start with a '{' followed by the Entry keyword.
bool LWConfig::LoadPluginV3( pifstream &in, char *buffer, int size, bool use_older ) {
  char * next_word = StringTools::SkipWhiteSpace( &(buffer[1]) );
  if( buffer[0] != '{' )
    return false;
  
  next_word = StringTools::SkipWhiteSpace( &(next_word[1]) );
  if( strncmp( next_word, "Entry", 5 ) != 0 )
    return false;

  LWPlugin *plugin = new LWPlugin;

  // Read in the plug-in data
  char   buffer2[ MAX_PATH_LENGTH ];
  do {
    in.getline( buffer, size );

    if( !in ) {
      delete plugin;
      return true;
    }

    next_word = StringTools::SkipWhiteSpace( buffer );
    if( next_word != NULL ) {
      if( strncmp( next_word, "Class", 5 ) == 0 ) {               // Class
        StringTools::IsolateWord( buffer, buffer2, 1, true, false );
        plugin->SetClass( StringToPluginClassID( buffer2 ) );

      } else if( strncmp( next_word, "Name", 4 ) == 0 ) {         // Name
        StringTools::IsolateWord( buffer, buffer2, 1, true, false );
        plugin->SetName( buffer2 );

      } else if( strncmp( next_word, "Module", 6 ) == 0 ) {       // Path
        // Reduce the double \'s to just one \ 
        int len = strlen( buffer2 );
        int j=0;

        for( int i=0; i < len; i++ ) {
          buffer2[j] = buffer[i];

          if( i == 0 )
            j++;
          else if( (buffer[i] == '\\') && (buffer[i-1] != '\\') )
            j++;
        }

        buffer2[ j+1 ] = '\0';
        plugin->SetPath( buffer2 );

      } else if( strncmp( next_word, "InfoTag", 7 ) == 0 ) {      // InfoTag
        LWInfoTag *info_tag = new LWInfoTag;
        info_tag->SetID( atoi( &(next_word[8]) ) );

        StringTools::IsolateWord( buffer, buffer2, 2, true, false );
        info_tag->SetData( buffer2 );

        plugin->GetInfoTags().Add( info_tag );
      } else if( buffer[0] == '}' ) {                            // End
        break;
      }
    }
  } while( true );

  // Add the plug-in
  if( !AddPlugin( plugin, !use_older ) )
    delete plugin;

  return true;
}

// AddPlugin():
//  Adds a plug-in to the plug-in list based on type.  If overwrite
//   is true, any existing plug-in of the same class with the same
//   name will be overwritten.  If false, the new plug-in will be
//   discarded.
//  Returns true if the plug-in was added to the list, and false if
//   not (ie: already exists and overwrite is false).  If it fails,
//   it's up to the caller to delete the plug-in instance.
inline bool LWConfig::AddPlugin( LWPlugin *plugin, bool overwrite ) {
  // See if this plug-in is already in the list
  for( unsigned long i=0; i < plugins[ plugin->GetClass() ].NumElements(); i++ ) {
    if( stricmp( plugin->GetName(), plugins[ plugin->GetClass() ][i]->GetName() ) == 0 )
      break;
  }

  if( i == plugins[ plugin->GetClass() ].NumElements() ) {   // No existing match; add it to the list
    plugins[ plugin->GetClass() ].Add( plugin );
  } else {
    if( overwrite ) {                                        // Handle Overwrite
      delete plugins[ plugin->GetClass() ][ i ];
      plugins[ plugin->GetClass() ][ i ] = plugin;
    } else {
      return false;
    }
  }

  return true;
}

