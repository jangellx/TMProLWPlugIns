//
// LWScene.cpp
//

#include <portable/pfstream/pfstream.h>

#include "LWScene.h"

// Constructor
LWScene::LWScene( LWSceneMode open_mode, int load_mode,
                  const char *_source, const char *_output, const char * _content_dir ) :
                  base( "Scene Hierarchy Base", LWI_ITEM_SCENE ) {

  SetContentDir( _content_dir );
  lock_at_30  = false;

  // Version
  version             = 1;                                 // 1 == LW 5.6 or earlier; 2 == LW 5.7/6 beta; 3 == L6

  // Initialize the rest of the scene data
  Clear();

  // Paths
  SetSceneName( _source );
}

// Destructor
LWScene::~LWScene() {
  Clear();
}

// Clear():
//  Clears a scene.  Also called by the destructor
bool LWScene::Clear() {
  // Free the hierarcy
  base.FreeHierarchy();

  // Times/Frames
  start_time          =  0.0;
  end_time            =  2.0;
  frame_step          =  1;

  preview_start_time  =  0.0;
  preview_end_time    =  2.0;
  preview_frame_step  =  1;

  fps                  = 30.0;
  key_fps              = 30.0;

  // Base Member Initialization
  base.SetItemType( LWI_ITEM_SCENE );
  base.SetFullName( "Scene Base (No Scene Loaded)" );

  // Reset Base Pointers/Lists
  base.GetChildren().Reset();
  base.ClearSkeleton();

  return true;
}

// Load():
//  This function loads a Lightwave scene into memory.  load_mode determines
//   what parts of the scene will be loaded, and is determined by or'ing
//   together the LWS_ flags.  source and content_dir are optional source
//   and content directory paths; if not provided, those existing in the
//   LWScene instance will be used.
int LWScene::Load( int load_mode, const char *_source, const char *_content_dir ) {
  if( load_mode == LWS_LOAD_DO_NOTHING )
    return LWSERROR_OK;

  if( _source != NULL )       SetSceneName( _source );
  if( _content_dir != NULL )  SetContentDir( _content_dir );

  // Open the scene file
  if( !DirInfo::Exists( full_scene_name ) )
    return LWSERROR_SCENE_NOT_FOUND;

  pifstream in( full_scene_name, ios::nocreate );
//  in.GuessEOLType();
  in.SetAnyEOL( true );
  if( !in )
    return LWSERROR_ERROR_OPENING_SCENE;

  // Loop Variables
  char line[8192];
  DynArray< LWItem * > item_list;            // Item list used only at load time, before the hierarchy is built

  int      object_index = 0;
  int      light_index  = 0;
  int      camera_index = 0;

  LWItem * item         = NULL;
  int      error        = LWSERROR_OK;
  bool     load_item    = false;

  // Header and Version Check
  in.getline( line, 8192 );
  if( strncmp( line, "LWSC", 4 ) != 0 )
    return LWSERROR_NOT_A_LW_SCENE;

  in.getline( line, 8192 );
  version = atoi( line );
//  if( (version < 1) || (version > 3) )       // Versions 1 through 3 supported; fail otherwise
  if( (version < 1) || (version > 2 ) )
    return LWSERROR_UNSUPPORTED_VERSION;

  // Clear out any existing scene data
  Clear();

  // Data Loop
  do {
    // Error Test
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() )
      return false;

    // LWS Scene Data
    if( load_mode & LWS_LOAD_SCENE_DATA ) {
      // Render Frames
      if( strnicmp( line, "FirstFrame", 10 ) == 0 )
        SetFirstFrame( atoi( &(line[11]) ) );
      else if( strnicmp( line, "LastFrame", 9 ) == 0 )
        SetLastFrame( atoi( &(line[10]) ) );
      else if( strnicmp( line, "FrameStep", 9 ) == 0 )
        SetFrameStep( atoi( &(line[10]) ) );

      // Preview Frames
      if( strnicmp( line, "PreviewFirstFrame", 17 ) == 0 )
        SetPreviewFirstFrame( atoi( &(line[18]) ) );
      else if( strnicmp( line, "PreviewLastFrame", 16 ) == 0 )
        SetPreviewLastFrame( atoi( &(line[17]) ) );
      if( strnicmp( line, "PreviewFrameStep", 16 ) == 0 )
        SetPreviewFrameStep( atoi( &(line[17]) ) );

      // FPS
      if( strnicmp( line, "FramesPerSecond", 15 ) == 0 )
        SetFPS( atof( &(line[16]) ) );
    }

    // Item Data
    if( load_mode & LWS_LOAD_HIERARCHY ) {
      item = NULL;
      if( (strnicmp( line, "LoadObject",    10 ) == 0) ||
          (strnicmp( line, "AddNullObject", 13 ) == 0) ) {
        error = LWItem::LoadItemFromScene( in, line, ++object_index, load_mode, version, item );
      } else if( strnicmp( line, "AddLight", 9 ) == 0 ) {
        error = LWItem::LoadItemFromScene( in, line, ++light_index, load_mode, version, item );
      } else if( strnicmp( line, "ShowCamera", 10 ) == 0 ) {
        error = LWItem::LoadItemFromScene( in, line, ++camera_index, load_mode, version, item );
      }

      if( item != NULL ) {                // Add the item to the list if it was loaded
        item_list.Add( item );
      }
      else if( error != LWSERROR_OK )     // Note that item can be NULL if the mode bits say that the item shouldn't be loaded
        return error;
    }

    // Effects Data

    // Global Illumination Data

  } while( !in.eof() );

  // Clean-up
  in.close();

  if( load_mode & LWS_LOAD_HIERARCHY ) {
    // Build the hierarchy
    int parent_index;
    for( unsigned int i=0; i < item_list.NumElements(); i++ ) {
      if( item_list[i]->GetParentIndex() == 0 ) {  // No parent; add to the base list
        base.GetChildren().Add( item_list[i] );
      } else {                                     // Valid parent; add to the parent's list
        parent_index = item_list[i]->GetParentIndex() - 1;
        item_list[ parent_index ]->GetChildren().Add( item_list[i] );
        item_list[i]->SetParent( item_list[ parent_index ] );
      }
    }

    // Post Processing
    if( load_mode & LWS_LOAD_FULL_POST ) {
      FillInItemCloneNumber( item_list );
      FillInItemDepths();
    }
  }

  base.SetFullName( GetSceneName() );
  return LWSERROR_OK;
}

// SetSceneName():
//  Changes the scene name.  The content dir is automatically prefixed as
//   the full_scene_name.  If the new name is NULL, the scene name will
//   be cleared.
void LWScene::SetSceneName( const char *new_name ) {
  if( new_name == NULL ) {
    scene_name[0]= '\0';
    strcpy( full_scene_name, content_dir );
  } else {
    strcpy( scene_name, new_name );
    strcpy( full_scene_name, content_dir );
    DirStrings::AddPathPart( full_scene_name, scene_name );
  }
}

// SetContentDir():
//  Changes the content dir.  The full scene name is automatically updated
//   with the new content dir path. If the new name is NULL, the scene name
//   will be cleared.
void LWScene::SetContentDir( const char *_content_dir ) {
  if( _content_dir == NULL ) {
    content_dir[0] = '\0';
    strcpy( full_scene_name, scene_name );
  } else {
    strcpy( full_scene_name, content_dir );
    DirStrings::AddPathPart( full_scene_name, scene_name );
  }
}

// IsLWS():
//  Tests the pifstream provided for a LWSC header.
bool LWScene::IsLWS( pifstream &in ) {
  streampos orig = in.tellg();
  in.seekg( 0, ios::beg );

  char buffer[5];
  in.getline( buffer, 5 );
  in.seekg( orig, ios::beg );
  if( strncmp( buffer, "LWSC", 4 ) != 0 )
    return false;

  return true;
}

// RAMUsageToFile():
//  Dumps the RAM used by FreeKey to the file provided.
bool LWScene::RAMUsageToFile( const char * file ) {
  ofstream out( file );
  if( !out )
    return false;

  out << "FreeKey RAM Usage Information" << endl;
  out << endl;

  out << "Basic Scene Information" << endl;
  out << " Currently Loaded Scene:  " << scene_name      << endl;
  out << " Content Dir:             " << content_dir     << endl;
  out << " Full Scene Name:         " << full_scene_name << endl;
  out << " Scene Version:           " << version         << endl;
  out << endl;

  out << " Start Time:              " << start_time      << endl;
  out << " End Time:                " << end_time        << endl;
  out << " Frame Step:              " << frame_step      << endl;
  out << endl;

  out << " Preview Start Time:      " << preview_start_time << endl;
  out << " Preview End Time:        " << preview_end_time   << endl;
  out << " Preview Frame Step:      " << preview_frame_step << endl;
  out << endl;

  out << "Basic RAM Usage:  LWScene" << endl;
  out << " sizeof( *this ):         " << sizeof( *this ) << endl;
  out << endl;

  unsigned long size = sizeof( *this );

  size += base.RAMUsageToStream( out );

  out << "Total size of entire scene:  " << size << " bytes" << endl;
  out << "                             " << size/1024 << " KB" << endl;
  out << "                             " << size/( 1024 * 1024 ) << " MB" <<endl;

  return true;
}

