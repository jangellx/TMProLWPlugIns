//
// LWScene.h
//
// LWScene contains all the nessesary functions to load and save a Lightwave
//  Scene File.  This is not yet a complete scene load/save system, but
//  it will become that as more functionality is added
//
// This also contains the LWScene class, which can be used for loading,
//  modifying and saving LWScenes.  It contains all the information about
//  a scene file, including a full hierarchy tree.
//
// There are two ways to load a file:  Load() and Scan().  Load()
//  will load a scene fully into memory, similar to loading a scene
//  into Lightwave.  What is loaded can be set by or'ing the
//  LWSceneLoad switches.  Note that some of these switches
//  require other switches (ie: LWS_LOAD_OBJECT requires
//  LWS_BUILD_HIERARCHY so that it has a place to put the hierarchy.)
//  If a required switch is missing, then neiter will be processed.
//  Limited loading is useful for building things like hierarchy
//  lists for UIs without having the RAM and time overhead of loading
//  the entire scene file.
//
// Scan() will scane a scene file, passing each keyword that the
//  calling function cares about to a callback function.  It can also
//  output data from the scene back into an output file, or it can just
//  ignore the output data. Scan() will not actually load any data at
//  all into the scene file; it is provided for the callback function
//  to gather the data it needs to modifty the source scene.
//
// IMPORTANT NOTE:  Times vs. Frames
//  All time values (ie: frames) are stored in seconds, NOT frames.  This
//   is how Lightwave internally stores time.  It also makes it easy to
//   support L6's Fractional Frames feature.
//
//  All floating point values are stored as doubles, which is consitant
//   with L6.
//
// IMPORTANT NOTE: Lock At 30
//  There are two "modes" for FPS.  The default behavior uses the FPS
//   of the scene to determine what frame a key is on.  This is consistant
//   with the way L6 handles things.  In 5.6, though, keyframes do not
//   shift in time relative to the FPS.  If LockAt30 is set to true,
//   LWScene will function in a 5.6-compatible mode, and keyframes will
//   always be at the current time in seconds divided by 30.
//

#ifndef HEADER_LWSCENE_SCENE
#define HEADER_LWSCENE_SCENE

#include <portable/dynarray/dynarray.h>
#include <portable/pfstream/pfstream.h>
#include <portable/dirtools/dirtools.h>

#include "LWScene-Error.h"
#include "LWScene-Item.h"
//#include "LWEffect.h"
//#include "../LWObject/LWObject.h"
//#include "LWGlobalIllum.h"
//#include "LWRender.h"

const int LWS_LOAD_DO_NOTHING      = 0x0000;                          // Default state; does nothing at all
const int LWS_LOAD_SCENE_DATA      = 0x0001;                          // Loads the scene-level data (first frame, FPS, etc.)
const int LWS_LOAD_HIERARCHY       = 0x0002;                          // Builds a hierarchy tree, including baic item information (parenting, index, etc)
const int LWS_LOAD_OBJECTS         = 0x0004;                          // Loads the object data.  Requires LWS_BUILD_HIERARCHY
const int LWS_LOAD_LWOS            = 0x0008;                          // Loads the .lwo files associated with each object.  Requires LWS_BUILD_HIERARCHY, but not nessesarily LOAD_OBJECTS
const int LWS_LOAD_LIGHTS          = 0x0010;                          // Loads the light data.  Requires LWS_BUILD_HIERARCHY
const int LWS_LOAD_BONES           = 0x0020;                          // Loads the bone data.  Requires LWS_BUILD_HIERARCHY, but not nessesarily LWS_LOAD_OBJECTS
const int LWS_LOAD_CAMERAS         = 0x0040;                          // Loads the camera data.  Requires LWS_BUILD_HIERARCHY
const int LWS_LOAD_MOTIONS         = 0x0080;                          // Loads the motions associated with each item.  Requires LWS_BUILD_HIERARCHY
const int LWS_LOAD_PLUGINS         = 0x0100;                          // Loads the plug-ins associated with each item.  Requires LWS_BUILD_HIERARCHY
const int LWS_LOAD_EFFECTS         = 0x0200;                          // Loads in the effects section of the scene file
const int LWS_LOAD_ENVELOPES       = 0x0400;                          // Loads in envelopes found in a scene.  If LWS_BUILD_HIERARCHY is not set, then only envelopes that don't rely on items will be read
const int LWS_LOAD_GLOBAL_ILLUM    = 0x0800;                          // Loads the Global Illumination settings
const int LWS_LOAD_RENDER          = 0x1000;                          // Loads the Render Settings, including the raytracing options and output paths
const int LWS_LOAD_GENERATE_LOG    = 0x2000;                          // Enable to generate a log file
const int LWS_LOAD_FULL_POST       = 0x4000;                          // Enable to perform full scene checking and indexing (index duplicates, depths, etc.); disable for a quick and simply hiererachy load

const int LWS_LOAD_ALL_BUT_OBJECTS =  LWS_LOAD_OBJECTS | LWS_LOAD_LIGHTS | LWS_LOAD_BONES | LWS_LOAD_CAMERAS | // Load everything except the LWOs
                                      LWS_LOAD_MOTIONS | LWS_LOAD_PLUGINS | LWS_LOAD_EFFECTS |
                                      LWS_LOAD_ENVELOPES | LWS_LOAD_GLOBAL_ILLUM |
                                      LWS_LOAD_RENDER | LWS_LOAD_SCENE_DATA | LWS_LOAD_FULL_POST;
const int LWS_LOAD_ALL             =  LWS_LOAD_ALL_BUT_OBJECTS | LWS_LOAD_LWOS;                                // Load everything, including the LWOs

class LWScene;

// Class LWScene
class LWScene {
public:
  // Enums
  enum LWSceneMode {
    init, load, scan };

  // Constructor/Destructor
  LWScene( LWSceneMode open_mode = init, int load_mode = LWS_LOAD_DO_NOTHING,
           const char * source = NULL, const char *output = NULL, const char * content_dir = NULL );
  ~LWScene();

  bool Clear();

  // Load/Scan Functions
  int  Load( int load_mode = LWS_LOAD_ALL_BUT_OBJECTS,
             const char *_source = NULL, const char *_content_dir = NULL );
  int  Load( pifstream *in, int load_mode = LWS_LOAD_ALL_BUT_OBJECTS,
             const char *_content_dir = NULL );

  int  Scan( const char **keywords, bool (__cdecl Handler)( int key_index ), 
             char *_source = NULL, char *_output = NULL );
  int  Scan( const char **keywords, bool (__cdecl Handler)( int key_index ), 
             pifstream *_in, ofstream *_out = NULL );

  // Post-Load Functions
  inline void FillInItemCloneNumber( DynArray< LWItem * > &item_list );               // Sets the item's copy count (-1 for only 1 instance of the item)
  inline void FillInItemDepths();                                                     // Fills in the item depths

  // Save Functions
  int Save( char * output = NULL );

  // List Accessors
  LWItem & GetBaseItem() { return base; }

  // Path Accessors
  inline const char *GetFullSceneName() { return scene_name; }                        // Returns scene name with path
  inline const char *GetSceneName() { return DirStrings::GetFilePart( scene_name ); } // Returns scene name sans path
               void  SetSceneName( const char *new_name );                            // Set scene name, with path.  Content dir is automatically added if needed

  inline const char *GetContentDir() { return content_dir; }                          // Content Directory
               void  SetContentDir( const char * _content_dir );

  inline int         GetVersion() { return version; }                                 // LWS Version
  inline void        SetVersion( int _version ) { version = _version; }

  // Frame/Time Accessors
  inline int         GetFirstFrame() { return (int)(start_time * fps); }
  inline int         GetLastFrame()  { return (int)(end_time   * fps); }
  inline double      GetStartTime()  { return start_time;              }
  inline double      GetEndTime()    { return end_time;                }

  inline void        SetFirstFrame( int frame ) { start_time = frame/fps; }
  inline void        SetLastFrame(  int frame ) { end_time   = frame/fps; }
  inline void        SetStartTime(  int time  ) { start_time = time;      }
  inline void        SetEndTime(    int time  ) { end_time   = time;      }

  inline int         GetFrameStep() { return frame_step; }
  inline bool        SetFrameStep( int new_step );

  // Preview Frame/Time Accessors
  inline int         GetPreviewFirstFrame() { return (int)(preview_start_time * key_fps); }
  inline int         GetPreviewLastFrame()  { return (int)(preview_end_time   * key_fps); }
  inline double      GetPreviewStartTime()  { return preview_start_time;                  }
  inline double      GetPreviewEndTime()    { return preview_end_time;                    }

  inline void        SetPreviewFirstFrame( int frame ) { preview_start_time = frame/key_fps; }
  inline void        SetPreviewLastFrame(  int frame ) { preview_end_time   = frame/key_fps; }
  inline void        SetPreviewStartTime(  int time  ) { preview_start_time = time;          }
  inline void        SetPreviewEndTime(    int time  ) { preview_end_time   = time;          }

  inline int         GetPreviewFrameStep() { return preview_frame_step; }
  inline bool        SetPreviewFrameStep( int new_step );

  // FPS Accessors
  inline double      GetFPS() { return fps; }
  inline bool        SetFPS( double _fps );

  bool     GetLockAt30() { return lock_at_30; }
  void     SetLockAt30( bool state ) { lock_at_30 = state;   key_fps = lock_at_30 ? 30 : fps; }

  // Time/Frame Conversion Functions
  inline static int     TimeToFrame( double time, double _fps = 30.0 );
  inline static double  FrameToTime( int frame,   double _fps = 30.0 );

  // Debug Functions
  bool RAMUsageToFile( const char *file );

protected:
  // Protected Functions
  bool IsLWS( pifstream &in );               // Makes sure a scene really is a LWS

  // Paths
  char scene_name[ MAX_PATH_LENGTH ];        // Scene name as provided by the user
  char content_dir[ MAX_PATH_LENGTH ];       // Content Dir
  char full_scene_name[ MAX_PATH_LENGTH ];   // Scene name with the content dir added

  // Version
  int version;                               // 1 == LW 5.6 or earlier; 2 == LW 5.7/6 beta; 3 == L6

  // Frames
  double start_time;                         // Start Time (ie: First Frame in Seconds)
  double end_time;                           // End Time (ie: Last Frame in Seconds)
  int    frame_step;                         // Frame Step

  double preview_start_time;                 // Preview Start Time (ie: Preview First Frame in Seconds)
  double preview_end_time;                   // Preview End Time (ie: Preview Last Frame in Seconds)
  int    preview_frame_step;                 // Preview Frame Step

  bool   lock_at_30;                         // True to be in 5.6-compatible keyframe mode; false for L6-compatible mode
  double fps;                                // Frames Per Second in the scene
  double key_fps;                            // The FPS used 

  // Hierarchy Base
  LWItem base;                               // Base of the item tree (ie: the scene itself)
};

// FillInItemDepths()
//  Fills in the item depths of the hierarchy
inline void LWScene::FillInItemDepths() {
  base.FillInItemDepths(-1);
}

// FillInItemCloneNumber():
//  Fills in the Item's Clone Number.
inline void LWScene::FillInItemCloneNumber( DynArray< LWItem * > &item_list ) {
  LWItem::FillInItemCloneNumber( item_list );
}

// SetFrameStep():
//  Sets the Frame Step of the scene.  A step of 0 is illegal,
//   as divide-by-zero errors could occur in future operations.
inline bool LWScene::SetFrameStep( int new_step ) {
  if( new_step < 1 )
    return false;

  frame_step = new_step;
  return true;
}

// SetPreviewFrameStep():
//  Sets the Preview Frame Step of the scene.  A step of 0 is
//   illegal, as divide-by-zero errors could occur in future
//   operations.
inline bool LWScene::SetPreviewFrameStep( int new_step ) {
  if( new_step < 1 )
    return false;

  preview_frame_step = new_step;
  return true;
}

// SetFPS():
//  Sets the FPS of the scene.  FPS of 0 is illegal, as divide-by-zero
//   errors could occur in future operations.
inline bool LWScene::SetFPS( double _fps ) {
  if( _fps < 0.00001 )
    return false;         // Do NOT allow the FPS to become 0.  This could cause divide-by-zero errors if allowed

  fps = _fps;
  return true;
}

// TimeToFrame():
//  Converts the given time into a keyframe value.
inline int LWScene::TimeToFrame( double time, double _fps ) {
  if( _fps < 0.00001 )
    return 0;

  return (int)(time * _fps);
}

// FrameToTime():
//  Converts the given time into a keyframe value.  By default,
//   the scene's current FPS is used for the conversion.
inline double LWScene::FrameToTime( int frame, double _fps ) {
  if( _fps < 0.00001 )
    return 0;

  return frame/_fps;
}

#endif