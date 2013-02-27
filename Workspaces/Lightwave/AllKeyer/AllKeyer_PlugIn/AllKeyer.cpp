
// All-Keyer.cpp

// Used by the All_Keyer plug-in to actually open, all-key
//  and save the scene.  The only reason this isn't in the
//  .c file is because I can only use file streams in C++.

#include <fstream.h>
#include <string.h>

#include <Portable\DirTools\DirTools.h>                    // Directory Access and String Tools
#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK
#include "AllKeyerPanel.h"
#include "AllKeyer.h"

AllKeyer *ak;

// Define the C++ functions we'll be using in the .c file as extern so
//  they'll actually be found by the .c file
extern "C" {
  int DoAllKeyer( GlobalFunc *global, void *local, void *serverData );   // Called from the C part of the program to tell the C++ part
                                                                         //  to create the C++ class and do it's thing
}

// DoAllKeyer():  Creates the AllKeyer CPP class, initializes the UI, all-keys scenes, etc.
int DoAllKeyer( GlobalFunc *global, void *local, void *serverData ) {
  int retval = AFUNC_OK;

  AllKeyer all_keyer =  AllKeyer( global, local, serverData,             // Activation-Function-Provided data
                                  retval );                              // Return Value

  return retval;
}

// -----------------------------------------------------------------------
//                   AllKeyer Class Member Functions
// -----------------------------------------------------------------------

// Consrutor
AllKeyer::AllKeyer( GlobalFunc *_global, void *_local, void *_serverData, int &_retval )
  : Global(*_global), local(_local), server_data(_serverData), retval(_retval) {

  // Get some function pointers from LW
  item_info  = CLWItemInfo::New(  (LWItemInfo   *) Global("LW Item Info 2", GFUSE_TRANSIENT) );
  scene_info = CLWSceneInfo::New( (LWSceneInfo  *) Global("LW Scene Info",  GFUSE_TRANSIENT) );
  message    = CMessage::New(     (MessageFuncs *) Global("Info Messages",  GFUSE_TRANSIENT) );

  dir_info   = (DirInfoFunc  *) Global("Directory Info", GFUSE_TRANSIENT);

  // Make sure the function pointers are non-NULL
  if( !item_info || !scene_info || !message || !item_info ) {
    message->Error( "ERROR: Your version of LW is too old.  Upgrade!", NULL );    
    retval = AFUNC_BADGLOBAL;
    return;
  }

  ak = this;   // Set a global variable for the AllKeyerPanel

  // Create the AllKeyerPanel 
  int panel_retval;
  panel = new AllKeyerPanel( _global, message, panel_retval );
  if( panel_retval != AFUNC_OK ) {
    retval = panel_retval;
    return;
  }

  // Create the OS-specific Directory Class
  os_dir = new DirInfo();
  if( !os_dir ) {
    message->Error( "ERROR: Unable to initialize directory class!", NULL );    
    retval = AFUNC_OK;
    return;
  }

  // Open the Main Panel
  while( panel->OpenMain() == MPS_PROCESS ) {
    if( TestOptions() ) {
      // Process the Scenes
      if( os_dir->HasHeader( "LWSC" ) ) {     // Handle LW Scenes
        ProcessScene();
      } else {                                // Not a scene; assume it's a file list
        ParseAKB();
      }
      panel->message->Info("Everything's cool!");
    }
  }

//  retval = DoProcess();
}

// Destructor
AllKeyer::~AllKeyer() {
  delete item_info;
  delete scene_info;
  delete message;

  delete panel;
}

// DoProcess( void ):  
int AllKeyer::DoProcess() {
  // Make sure the scene name is valid
  if( stricmp( scene_info->name, "(unnamed)" ) == 0 ) {
    message->Error( "ERROR:  The scene hasn't been saved yet.",
                    "Save first so we can get an output filename.");
    return( AFUNC_OK );  // Abort
  }

  // Create the input scene's filename
  char out_name[256];
  char in_name[256];
	strcpy( in_name, (*dir_info)("Content") );  // Get the Content Dir
  if( out_name[ strlen(in_name)-1] != '\\' );
    strcat( in_name, "\\" );                  // Add a trailing '/' if there isn't one already
  strcat( in_name, scene_info->filename );    // Add on the scene's relative path

  // Craete the output scene's filename
  strcpy( out_name, in_name );

  // Add -AK just before the extension
  char *mark;
  if( strlen( out_name ) > 250 )               // Check for possible buffer overflow
    mark = &out_name[250];
  else
    mark = &out_name[ strlen(out_name)-4 ];

  if( stricmp( mark, ".lws" ) != 0 )           // Insert before the extension, if there is one
    mark = &out_name[ strlen(out_name)-1 ];
  strcpy( mark, "-AK.lws" );

  // Get ready to go
  message->Error( "All-Keyer, ready to begin.", "This could take a while..." );    

  // Open the input file
  ifstream in( in_name );

  // Open the output fule
  ofstream out( out_name );
  if( !out ) {
    message->Error( "Error opening output scene", out_name );    
    return( AFUNC_OK );
  }

  // Find each object in the input .lws and save it's motion from Lightwave
  char buffer[2048];
  char name[256];
  int item_count[4] = {0, 0, 0, 0};
  LWItemType type = -1;
  LWItemID id = LWITEM_NULL;

  do {
    in.getline( buffer, 2048 );           // Read in a line
    out << buffer << endl;                // Output the line

    // Get the next item's name
    if( strstr( buffer, "LoadObject" ) != 0 ) {
      type = LWI_OBJECT;
      strcpy( name, &buffer[ strlen("LoadObject ")-1 ] );
      item_count[type]++;
    } else if( strstr( buffer, "AddNullObject" ) != 0 ) {
      type = LWI_OBJECT;
      strcpy( name, &buffer[ strlen("AddNullObject ")-1 ] );
      item_count[type]++;
    } else if( strstr( buffer, "AddLight" ) != 0 ) {
      type = LWI_LIGHT;
      strcpy( name, &buffer[ strlen("AddLight ")-1 ] );
      item_count[type]++;
    } else if( strstr( buffer, "CameraMotion" ) != 0 ) {
      type = LWI_CAMERA;
      strcpy( name, "(none)" );
      item_count[type]++;
    } else if( strstr( buffer, "AddBone" ) != 0 ) {
      type = LWI_BONE;
      strcpy( name, &buffer[ strlen("AddBone ")-1 ] );
      item_count[type]++;
    }

    // Find the item's motion
    if( ( strstr( buffer, "ObjectMotion" ) != 0 ) ||
        ( strstr( buffer, "LightMotion"  ) != 0 ) ||
        ( strstr( buffer, "BonetMotion"  ) != 0 ) ||
        ( strstr( buffer, "CameraMotion" ) != 0 ) ) {
      // Find the object in LW's linked list
      id = item_info->First( type, LWITEM_NULL );              // Get the first object's ID
      for( int i=1; i < item_count[ type ]; i++ )
        id = item_info->Next( id );                            // Get the Next Object's ID

      // Output the item's new motion
      WriteMotion( out, item_info, id, scene_info);           // Output the object's data

      // Skip the motion in the .LWS
      do {
        in.getline( buffer, 2048 );           // Read in a line
      } while( !in.eof() && ( strstr( buffer, "EndBehavior" ) != 0 ) );
      out << buffer << endl;
    }
  } while( !in.eof() );

  // Finish up
  message->Info( "All-Keying complete; created", out_name);
  return( AFUNC_OK );
}

bool AllKeyer::WriteMotion( ofstream &out, CLWItemInfo *item_info,
                            LWItemID id, CLWSceneInfo *scene_info ) {
  out << "  9" << endl;                         // Number of Channels
  out << "  " << scene_info->frameEnd << endl;  // Number of keys

  double channels[9];
  int j;
  for( int i=0; i < scene_info->frameEnd; i++ ) {
    // Get the time step
    LWTime time = (double) i/scene_info->framesPerSecond;  
    // Get all the channels
    item_info->Param( id, LWIP_POSITION, time, &channels[0] );
    item_info->Param( id, LWIP_ROTATION, time, &channels[3] );
    item_info->Param( id, LWIP_SCALING,  time, &channels[6] );

    // Convert Position from Radians to Degrees
    for( j=3; j < 6; j++ )
      channels[j] *= 180.0 / 3.14159265;

    // Output the channel data
    out << " "; 
    for( j=0; j < 9; j++ )
      out << " " << channels[j];
    out << endl;

    // Output the frame number and spline data
    out << "  " << i << " 1 0 0 0" << endl;
  }

  return true;
}

// TestOptions():
//  Test the options on the Main Panel before processing,
//  such as the Source Pattern, Destination Path, etc.
bool AllKeyer::TestOptions() {
  // Destination Path Tests
  char dest[MAX_PATH_LENGTH];
  GET_STR( panel->destination_path, dest, MAX_PATH_LENGTH );
  if( (stricmp( dest, "(none)" ) == 0) ||
      (dest[0] == '\0' ) ) {
    message->Error("You must enter a destination path!");
    return false;
  }

  if( !DirInfo::Exists( dest, DI_EXIST_ONLY ) ) {
    panel->message->Error("Destination path doesn't exist:", dest);
    return false;
  }

  // Comapre Test
  char comp[MAX_PATH_LENGTH];
  GET_STR( panel->compare_file, comp, MAX_PATH_LENGTH );
  if( stricmp( comp, "(none)" ) != 0 ){
    if( DirInfo::Exists( comp ) )
      use_compare = true;
    else
      use_compare = false;
  }

  // Source Pattern Tests
  char source[MAX_PATH_LENGTH];
  GET_STR( panel->source_pattern, source, MAX_PATH_LENGTH );
  if( (stricmp( source, "(none)"    ) == 0) ||
      (stricmp( source, "(unnamed)" ) == 0) ) {
    panel->message->Error("You must enter a source pattern",
                          "or load a scene into layout" );
    return false;
  }

  os_dir->ChangeFullPath( source );
  if( !os_dir->FindFirstWildcardMatch() ) {
    message->Error("No files found that match the source pattern!");
    os_dir->CloseSearch();
    return false;
  }

  return true;
}

// DoNextScene():
//  
bool DoNextScene() {
}
