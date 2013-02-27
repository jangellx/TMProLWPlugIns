//
// Confy.cpp
//
// Confy is a smart Lightwave Config File Merger.  It allows you to use the user
//  settings from one config and merge in plug-in data from other configs.  It is
//  designed to be used in a batch file at LW's start-up: the batch first calls
//  Confy to generate a config, and then runs LW with the new config.
//
// The first config file specified will be the newly generated config file.  The
//  second config file's user settings will be will be put in the new config.
//  Any other config files specified will have their plugins added to the dest.
//  Note that the first plug-in config will also have certain properties copied,
//  such as the MaxObjects values and the Plugin Dir.
//
// On load, the entire config file is read into memory.  This keeps from having
//  duplicate plug-ins and other config settings being added more than once in the
//  same config file.
//
// If the dest.cfg and user_source.cfg are the same, user_source.cfg will be renamed
// to <name>-confy.cfg.
//
// This supports both Layout and Modeler configs.
//

// Test Arguments:
//  lw.cfg lw.cfg -e C:\NewTek\Lightwave\Programs_5.6\lightwav.exe -c lw.cfg -f -0 -s test.lws lw_plugin_1.cfg lw_plugin_2.cfg
//  confy\user.cfg confy\user.cfg -e C:\NewTek\Lightwave\Programs_5.6\lightwav.exe -c confy\user.cfg -s test.lws confy\showset.cfg  confy\baseplugs.cfg confy\showplugs.cfg

#include <portable\pfstream\pfstream.h>
#include <portable\dynarray\dynarray.h>
#include <portable\stringtools\stringtools.h>
#include <portable\dirtools\dirtools.h>

#include <process.h>

// Enums
enum settings_group_ids {
  // Layout/Common Plug-ins
  plugins_SceneConverter = 0,
  plugins_ImageLoader,
  plugins_ImageSaver,
  plugins_ItemMotionHandler,
  plugins_ItemMotionInterface,
  plugins_ObjReplacementHandler,
  plugins_ObjReplacementInterface,
  plugins_DisplacementHandler,
  plugins_DisplacementInterface,
  plugins_ShaderHandler,
  plugins_ShaderInterface,
  plugins_PixelFilterHandler,
  plugins_PixelFilterInterface,
  plugins_ImageFilterHandler,
  plugins_ImageFilterInterface,
  plugins_FrameBufferHandler,
  plugins_FrameBufferInterface,
  plugins_AnimSaverHandler,
  plugins_AnimSaverInterface,
  plugins_LayoutGeneric,
  plugins_Global,
  // Layout L6
  plugins_AnimLoaderHandler,
  plugins_ChannelHandler,
  plugins_ChannelInterface,
  plugins_CustomObjHandler,
  plugins_CustomObjInterface,
  plugins_EnvironmentHandler,
  plugins_EnvironmentInterface,
  plugins_MasterHandler,
  plugins_MasterInterface,
  plugins_ObjectLoader,
  plugins_TextureHandler,
  plugins_TextureInterface,
  plugins_VolumetricHandler,
  plugins_VolumetricInterface,

  // Modeler Plug-ins
  plugins_CommandSequence,
  plugins_MeshDataEdit,
  // Modeler L6
  plugins_MeshEditTool,

  num_plugin_ids,

  plugins_other = num_plugin_ids,
  user_settings_pre,
  user_settings_post,
  num_settings_group_ids };

// settings_group_strings
//  This array aligns with the settings_group_ids above
const char *settings_group_strings[] = {
  // Modeler Plug-ins
  "CommandSequence",
  "MeshDataEdit",
  "ObjectLoader",

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

  "<other plugins>",
  "<user settings pre>",
  "<user settings post>",
  "<invalid>" };

// Config Types IDs
//  These determine what kind of config file we are creating
enum config_type_ids {
  config_bad     = 0,
  config_layout,
  config_modeler,
  config_unknown,
  num_config_types };

// config_replace_strings[]
//  All strings that match here will cause the same line
//   in the user config to be replaced with that in the
//   plug-in condig 
const char *config_replace_strings[] = {
  "ContentDirectory",
  "ScenesDirectory",
  "ObjectsDirectory",
  "HierarchiesDirectory",
  "SurfacesDirectory",
  "ImagesDirectory",
  "OutputDirectory",
  "AnimationsDirectory",
  "MotionsDirectory",
  "EnvelopesDirectory",
  "PreviewsDirectory",
  "DefaultZoomFactor",
  "MaxObjects",
  "MaxSurfaces",
  "MaxImages",
  "MaxLights",
  "DefaultFPS",
  "DefaultPixelAspect",
  "DefaultSegmentMemory",
  "RGBImageFormat",
  "AlphaImageFormat",
  // L6 Keywords
  "FrameSize",
  "PixelAspect",
  "FramesPerFoot",
  "OutputFileFormat",
  "RGBImageFormat",
  "AlphaImageFormat",
  "DirectoryType",
  "FileType",

  // Modeler Keywords
  "FlatnessLimit",

  NULL };

// Prototypes
void            Help(void);
config_type_ids OpenConfig( const char * cfg, pifstream &in );
int             GetConfigVersion( pifstream &in, config_type_ids cfg_type );
void            UpdateUser( const char * buffer, DynArray< char *> & _settings, config_type_ids cfg_type );
void            LaunchProgram( void * _executable );

// Argument Template
//  Config <dest.cfg> <user_source.cfg> [-e <executable>] [-s <scene>] [-p] <plugin_source.cfg> [plugin_source.cfg] ... [plugin_source.cfg]
//   <dest.cfg>           Path to the .cfg file to create
//   <user_source.cfg>    Config file containing user-level configuration options
//   [-e <executable>]    Executable to launch when Confy finishes (usually Lightwave or Modeler)
//   [-c <config>]        Config file to pass to the executable with the -c option
//   [-f <flags>]         Optional arguments that will be passed to the executable.
//   [-s <scene>]         Scene file to have LW load on startup.  Requires -e.
//   [-p]                 Flag to toggle adding only plug-ins and not copying key settings from <plugin source>
//   <plugin_source.cfg>  Base plug-in config file.  All plug-ins and some user settings will be copied to <dest.cfg>
//   [plugin_source.cfg]  Other optional plug-in config files.  All plug-ins found will be copied to <dest.cfg>
//

const char *Confy_Version  = "1";
const char *Confy_Revision = "2";

void main( int argc, char *argv[] ) {
  cout << " Confy V" << Confy_Version << "." << Confy_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  if( argc < 4 )
    Help();

  if( argv[1][0] == '?' )
    Help();

  // Argument Tests
  char * dest = argv[1];
  char * user = argv[2];
  bool   plugins_only = false;

  bool   use_executable = false;
  char   executable[ MAX_PATH_LENGTH ] = "";

  bool   use_dash_c = false;
  char   dash_c[ MAX_PATH_LENGTH ] = "-c";   // Oddly enough, this is correct (ie: "-clw.cfg").  Weird.

  bool   use_flags = false;
  char   flags[ 1024 ] = "";

  bool   use_scene = false;
  char   scene[ MAX_PATH_LENGTH ] = "";

  bool   stop = false;
  for( int first_plugin_cfg = 3; first_plugin_cfg < argc; first_plugin_cfg++ ) {
    switch( argv[first_plugin_cfg][1] ) {
    // Specify Config Path for Layout
    case 'c':
      if( ++first_plugin_cfg >= argc ) {
        cerr << " ERROR: No config file following arguemnt -c!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }

      use_dash_c = true;
      strcat( dash_c, argv[ first_plugin_cfg ] );

      if( !DirInfo::Exists( argv[ first_plugin_cfg ] ) ) {
        cerr << " ERROR: The -c config file \"" << dash_c << "\" doesn't exist!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }
      break;

    // Executable
    case 'e':
      if( ++first_plugin_cfg >= argc ) {
        cerr << " ERROR: No executable following argument -e!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }

      use_executable = true;
      strcpy( executable, argv[ first_plugin_cfg ] );

      if( !DirInfo::Exists( executable ) ) {
        cerr << " ERROR: The executable \"" << executable << "\" doesn't exist!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }
      break;

    // Flags
    case 'f':
      if( ++first_plugin_cfg >= argc ) {
        cerr << " ERROR: No flags following argument -f!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }

      use_flags = true;
      strcpy( flags, argv[ first_plugin_cfg ] );

    // Plug-ins Only
    case 'p':
      plugins_only = true;
      break;

    // Scene/Object to pass to Lightwave/Modeler on load
    case 's':
      if( ++first_plugin_cfg >= argc ) {
        cerr << " ERROR: No scene/object file following argument -s!" << endl;
        cerr << " Type Confy ? for help" << endl;
        exit(20);
      }

      use_scene = true;
      strcpy( scene, argv[ first_plugin_cfg ] );

      break;

    // End of options
    default:                   // This is probably the first plugin config file
      stop = true;
      break;
    }

    if( stop )
      break;
  }

  // Test Arguments For Validity
  if( first_plugin_cfg >= argc ) {
    cerr << " ERROR: No plug-in configs specified!" << endl;
    cerr << " Type Confy ? for help" << endl;
    exit( 20 );
  }

  if( use_scene && !use_executable ) {
    cerr << " ERROR:  using -s requires -e" << endl;
    cerr << " Type Confy ? for help" << endl;
    exit(20);
  }

  // Display Settings
  cout << "-Settings:" << endl;
  cout << "  Destination Config:             \"" << dest << "\"" << endl;
  cout << "  User Config:                    \"" << user << "\"" << endl;

  if( plugins_only )
    cout << "  Adding plug-ins only; no settings will be replaced" << endl;

  if( use_executable )
    cout << "  Executable to launch:           \"" << executable << "\"" << endl;

  if( use_scene )
    cout << "  Scene to load into Lightwave:   \"" << scene << "\"" << endl;

  cout << "  Settings and Plug-In Config 0:  \"" << argv[ first_plugin_cfg ] << "\"" << endl;
  for( int i = first_plugin_cfg+1; i < argc; i++ ) 
    cout << "  Plug-In Config " << i-first_plugin_cfg << ":               \"" << argv[ i ] << "\"" << endl;

  // Config Variable Setup
  DynArray< char * >   settings[ num_settings_group_ids ];
  char                 buffer[ 8192 ];
  char               * plugin_type;
  char               * new_name, * old_name;
  unsigned int         j;
  pifstream            in;
  bool                 error = false;
  
  config_type_ids      cfg_type = config_unknown, new_type;
  int                  cfg_version = -1, new_version;
  DynArray< char * > * user_settings_loc = &settings[ user_settings_pre ];

  // ---
  // Load the User Config Settings
  // ---
  try {
    cout << "-Loading User Config..." << endl;
    cfg_type = OpenConfig( user, in );
    if( cfg_type != config_unknown ) {
      cfg_version = GetConfigVersion( in, cfg_type );
      if( cfg_version == -1 )
        throw false;
    }

    if( cfg_type == config_layout ) {
      cout << " User config file is a Layout config, version " << cfg_version << endl;
    } else if( cfg_type == config_modeler ) {
      cout << " User config file is a Modeler config, version " << cfg_version << endl;
    } else {
      cout << " Unknown config file type; L6 config?" << endl;
    }

    while( true ) {
      // Read in a new line
      in.getline( buffer, 8192 );

      // Error Test
      if( (in.fail() || in.bad()) && !in.eof() ) {
        cerr << " ERROR:  File error reading from user config file; aborting" << endl;
        throw false;
      }

      // See if there are any plug-ins here and skip them
      if( strncmp( "Plugin ", buffer, 7 ) == 0 ) {
        user_settings_loc = &settings[ user_settings_post ];
      } else {
        // Add the prev. line to the appropriate array
        user_settings_loc->Add( strdup( buffer ) );
      }

      // EOF test
      if( in.eof() )
        break;
    };

    // ---
    // Load the Plug-In Configs
    // ---
    user_settings_loc = &settings[ user_settings_pre ];
    for( int k=first_plugin_cfg; k < argc; k++ ) {
      cout << "-Loading Plugin Config " << k - first_plugin_cfg << ":  \"" << argv[k] << "\"..." << endl;

      in.close();
      new_type = OpenConfig( argv[ k ], in );
      if( new_type == config_bad ) {
        cerr << " ERROR:  Error opening config file; aborting" << endl;
        throw false;
      } else if( new_type == config_unknown ) {
        if( cfg_type != new_type ) {
          cerr << " ERROR:  Attempting to merge unknown config file (not a .cfg?); aborting" << endl;
          throw false;
        }
      } else if( new_type != cfg_type ) {
        cerr << " ERROR:  Illegal attempt to merge Modeler and Layout configs file; aborting" << endl;
        throw false;
      }

      if( new_type != config_unknown ) {
        new_version = GetConfigVersion( in, cfg_type );
        if( cfg_version != new_version ) {
          cerr << " ERROR:  The configuration files are not the same version; aborting" << endl;
          throw false;
        }
      }

      while( true ) {
        // Read in a new line
        in.getline( buffer, 8192 );

        // Error Test
        if( (in.fail() || in.bad()) && !in.eof() ) {
          cerr << " ERROR:  File error reading from plug-in config file \""
               << argv[ k ] << "\"; aborting" << endl;
          throw false;
        }

        // Update the user config options
        if( !plugins_only && (k == first_plugin_cfg) )
          UpdateUser( buffer, *user_settings_loc, cfg_type );

        // See if there's a plug-in here and store it
        if( strncmp( "Plugin ", buffer, 7 ) == 0 ) {           // The space is so we don't confuse this with 
          user_settings_loc = &settings[ user_settings_post ];

          plugin_type = StringTools::FindWord( buffer, 1 );
          if( plugin_type == NULL ) {
            cerr << " WARNING:  Missing plug-in identifier after keyword \"" << buffer
                 << "\" (config " << k - first_plugin_cfg << ")" << endl;
          } else {
            for( i = 0; i < num_plugin_ids; i++ ) {
              // Figure out what group it's in
              if( strncmp( plugin_type, settings_group_strings[i], strlen( settings_group_strings[i] ) ) == 0 ) {
                break;
              }
            }

            // Test for unknown plug-in types
            if( i == num_plugin_ids ) {
              cerr << " WARNING:  Unknown plug-in type found in this line: \"" << buffer
                   << "\" (config " << k - first_plugin_cfg << ")" << endl;
              settings[ plugins_other ].Add( strdup( buffer ) );
            }

            // See if this plug-in is a duplicate
            new_name = StringTools::FindWord( buffer, 2 );         // This should be the name of the plug-in as found in the .p file
            if( new_name == NULL ) {
              cerr << " WARNING:  The following line contains invalid plug-in information: \"" << buffer
                    << "\" (config " << k - first_plugin_cfg << ")" << endl;
            } else {
              for( j=0; j < settings[i].NumElements(); j++ ) {
                old_name = StringTools::FindWord( settings[i][j], 2 );
                if( old_name == NULL ) {
                  cerr << " WARNING:  The following plug-in line is invalid \"" << buffer << "\"; skipping";
                } else {
                  if( strncmp( old_name, new_name, StringTools::FindWordLength( old_name ) ) == 0 ) {
                    if( StringTools::FindWordLength( old_name ) == StringTools::FindWordLength( new_name ) ) {  // make sure the words are equal length; gets around strncmp() failing on things like "Metaform_Shaper" and "Metaform_Shaper(FASTER)"
                      cerr << " WARNING:  The following plug-in is already in the config: \"" << buffer
                           << "\"; replacing existing with this (config " << k - first_plugin_cfg << ")" << endl;
                      delete settings[i][j];
                      settings[i][j] = strdup( buffer );
                      break;
                    }
                  }
                }
              }

              // Not a duplicate; add it to the end
              if( j == settings[i].NumElements() ) {
                settings[i].Add( strdup( buffer ) );
              }
            }

          }
        }

        // EOF test
        if( in.eof() )
          break;
      };
    }

    in.close();

    // ---
    // Backup the existing config, if needed
    // ---
    if( DirInfo::Exists( dest ) ) {
      char backup[ MAX_PATH_LENGTH ];
      char extension[ MAX_FILENAME_LENGTH ];

      // Generate the backup name
      strcpy( backup, dest );
      char * ext = DirStrings::HasExtension( backup ); // Find any extension
      if( ext ) {                                      // Strip the extension
        strcpy( extension, ext );
        *ext = '\0';
      }
      strcat( backup, "-backup" );                     // Append "-backup"
      strcat( backup, extension );                     // Put the extension back

      cout << "-Destination configs already exists; backing up original as \"" << backup << "\"" << endl;

      // Delete any existing backup file so we can rename this one
      if( DirInfo::Exists( backup ) ) {
        if( DirInfo::Delete( backup ) != DI_DELETE_OK ) {
          cerr << " ERROR:  Unable to remove previously existing backup; aborting" << endl;
          throw false;
        }
      }

      // Rename the existing config
      if( DirInfo::Rename( dest, backup ) != DI_RENAME_OK ) {
        cerr << " ERROR:  Unable to rename existing config as backup; aborting" << endl;
        throw false;
      }
    }


    // ---
    // Output the final config
    // ---
    cout << "-Writing Composite Config \"" << dest << "\"..." << endl;
    ofstream out( dest );
    if( !out ) {
      cerr << " ERROR:  Unable to open output config \"" << dest << "\"; aborting" << endl;
      throw false;
    }

    // Pre-Plugin Settings
    for( j = 0; j < settings[ user_settings_pre ].NumElements(); j++ ) {
      if( settings[ user_settings_pre ][j][0] != '\0' )   // Skip any possible blank lines
        out << settings[ user_settings_pre ][j] << endl;
    }

    // Plug-ins
    for( i = 0; i < num_plugin_ids; i++ ) {
      for( j = 0; j < settings[i].NumElements(); j++ )
        out << settings[i][j] << endl;
    }

    // Post-Plugin Settings
    for( j = 0; j < settings[ user_settings_post ].NumElements(); j++ ) {
      if( settings[ user_settings_post ][j][0] != '\0' )   // Skip any possible blank lines
        out << settings[ user_settings_post ][j] << endl;
    }

  } catch( bool ) {
    error = true;  // We're just here so the clean-up code gets called on errors
  }

  // Free the memory used by the strings
  if( !error )
    cout << "-Cleaning up..." << endl;
  for( i=0; i < num_settings_group_ids; i++ ) {
    for( j = 0; j < settings[i].NumElements(); j++ )
      free( settings[i][j] );
  }

  if( !error ) {
    if( use_executable ) {
      cout << "-Launching \"" << executable << "\"..." << endl;

      // Change the working dir
      char cwd[ MAX_PATH_LENGTH ];
      strcpy( cwd, executable );
      *DirStrings::HasFile( cwd ) = '\0';
      if( cwd[0] != '\0' )
        DirInfo::ChangeWorkingDir( cwd );

      // Build the argument list
      char *exec_args[128];
      int a1 = 0;
      exec_args[ a1++ ] = strdup( DirStrings::GetFilePart( executable ) );

      if( use_dash_c )           // -c option
        exec_args[ a1++ ] = strdup( dash_c );

      if( use_flags ) {          // User-defined flags
        int j = 0;
        strcpy( buffer, flags );

        do {
          if( StringTools::IsolateWord( flags, buffer, j++ )[0] != '\0' )
            exec_args[ a1++ ] = strdup( buffer );

          if( a1 > 126 ) {
            cerr << "  ERROR:  More than 127 arguments being paseed to executable; aborting" << endl;
            exec_args[ a1-1 ] = '\0';
            for( i = 0; exec_args[i] != NULL; i++ )
              free( exec_args[i] );
            return;
          }
        } while( buffer[0] != '\0' );
      }

      if( use_scene )
        exec_args[ a1++ ] = strdup( scene );

      exec_args[ a1++ ] = NULL;          // NULL-terminate the array

      // Display the argument string
      cout << "  Argument String For Executable:  " << endl;
      cout << "  ";
      for( unsigned int i = 0; exec_args[i] != NULL; i++ )
        cout << " " << exec_args[i];
      cout << endl;

      // Run the program asynchronously
      _spawnv( _P_NOWAIT, executable, exec_args );

      // Free the argument list
      for( i = 0; exec_args[i] != NULL; i++ )
        free( exec_args[i] );
    }

    cout << "-Complete!" << endl;
  }
}

// Help()
void Help(void) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Merges Lightwave config files together.\n" << endl;
  cout << endl;
  cout << "  Usage:   Config <dest.cfg> <user_source.cfg> [-e <executable>] [-s <scene>] [-p]" << endl;
  cout << "                  <plugin_source.cfg> [plugin_source.cfg] ... [plugin_source.cfg]" << endl;
  cout << "   <dest.cfg>           Path to the .cfg file to create" << endl;
  cout << "   <user_source.cfg>    Config file containing user-level configuration options" << endl;
  cout << "   [-c <config>]        Config file to pass to the executable with the -e option" << endl;
  cout << "   [-f <flags>]         Optional arguments that will be passed to the executable." << endl;
  cout << "   [-e <executable>]    Executable to launch when Confy finishes (usually Lightwave or Modeler)" << endl;
  cout << "   [-s <scene>]         Scene file to have LW load on startup.  Requires -e." << endl;
  cout << "   [-p]                 Flag to toggle adding only plug-ins and not copying key settings from <plugin source>" << endl;
  cout << "   <plugin_source.cfg>  Base plug-in config file.  All plug-ins and some user settings will be copied to <dest.cfg>" << endl;
  cout << "   [plugin_source.cfg]  Other optional plug-in config files.  All plug-ins found will be copied to <dest.cfg>" << endl;

  exit(0);
}

// OpenConfig():
//  Opens the config file and makes sure it really is a LW config.
config_type_ids OpenConfig( const char * cfg, pifstream &in ) {
  in.open( cfg, ios::nocreate );
  if( !in ) {
    cerr << " ERROR:  Unable to open config \"" << cfg << "\"; aborting" << endl;
    return config_bad;
  }
  
  in.GuessEOLType();

  // Make sure the config file is valid
  config_type_ids cfg_type = config_unknown;
  char id[4];
  in.read( id, 4 );
  if( strncmp( id, "LWCO", 4 ) == 0 ) {
    cfg_type = config_layout;
  } else if( strncmp( id, "MDOP", 4 ) == 0 ) {
    cfg_type = config_modeler;
  } else {  // L6 configs don't have headers for some reason
//    cerr << " ERROR:  The config file \"" << cfg << "\" isn't a recognized Lightwave config; aborting" << endl;
    cfg_type = config_unknown;
  }

  // Seek back to the begining and return the type
  in.seekg( 0, ios::beg );
  return cfg_type;
}

// GetConfigVersion():
//  Reads in the version of the configuration file passed in.
//   Call this after calling OpenConfig().  If the version
//   found is unsupported, this will return -1.  
int GetConfigVersion( pifstream &in, config_type_ids cfg_type ) {
  if( !in )
    return -1;

  char buffer[1024];
  int  version;

  if( cfg_type == config_modeler ) {           // Modeler Config
    in >> buffer >> version;
    if( version != 3 ) {
      cout << " ERROR:  Unsupported Modeler config version " << version << "; aborting" << endl;
      version = -1;
    }
  } else if( cfg_type == config_layout ) {    // Layout Config
    in >> buffer >> version;
    if( version != 0 ) {
      cout << " ERROR:  Unsupported Layout config version " << version << "; aborting" << endl;
      version = -1;
    }
  }

  // Seek back to the begining and return the version
  in.seekg( 0, ios::beg );
  return version;
}

// UpdateUser():
//  Updates the user config with the values in the buffer, but only if
//   we care about the buffer's keyword.
void UpdateUser( const char * buffer, DynArray< char *> & settings, config_type_ids cfg_type ) {
  int len = StringTools::FindWordLength( buffer );

  int          i = 0;
  unsigned int j;

  if( buffer[0] == '\0' )
    return;

  // Layout
  if( cfg_type == config_layout ) {
    while( config_replace_strings[i] != NULL ) {
      // See if this keyword matches the replace strings
      if( strncmp( buffer, config_replace_strings[i], len ) == 0 ) {
        // Find the keyword in the user settings already loaded
        for( j = 0; j < settings.NumElements(); j++ ) {
          if( strncmp( settings[j], config_replace_strings[i], len ) == 0 ) {
            // Replace the User string with the new one
            cout << " Replacing User Setting \"" << config_replace_strings[i] << "\" with Alternate Setting:  \""
                 << StringTools::FindWord( buffer, 1) << "\"" << endl;
            delete settings[j];
            settings[j] = strdup( buffer );
            break;
          }
        }

        // Not found; add the new key to the user settings
        if( j == settings.NumElements() ) {
          settings.Add( strdup( buffer ) );
          cout << " Adding Alternate Setting:  \"" << buffer << "\"" << endl;
        }
        break;
      }
      i++;
    }
  } else {
  // Modeler
    ; // We don't replace anything here yet...
  }
}
