//
// Simplify_ObjRep_Instance.h
//
// This contains the SimplifyInstance class, which contains all of the
//  data used by a single Simplify instance.
// This also contains the SimplifyLOD class, which contains level-of-detail
//  replacement information.  A series of SimplifyLOD's are stored in the
//  lod array of a SimplifyInstance.
//

#ifndef HEADER_SIMPLIFY_OBJREP_INSTANCE
#define HEADER_SIMPLIFY_OBJREP_INSTANCE

#include <string.h>

#include <fstream.h>

#include <portable/dynarray/dynarray.h>
#include <portable/dirtools/dirtools.h>

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

class SimplifyInstance;
class SimplifyLOD;

// SimplifyInstance
//  If you add any new data members, BE SURE to add them to
//   the CopyFrom() function, if needed!
class SimplifyInstance {
public:
  SimplifyInstance();
  ~SimplifyInstance();

  // Accessors Functions
  const char * GetPreview() { return preview_filename; }
  bool         SetPreview( const char *preview_filename );

  const char * GetRender() { return render_filename; }
  bool         SetRender( const char *render_filename );

  const char * GetSDF() { return sdf_filename; }
  bool         SetSDF( const char *sdf_filename );

  bool         GetEnabled() { return enabled; }
  void         SetEnabled( bool state ) { enabled = state; }

  int          GetStoreAs() { return store_as_mode; }
  void         SetStoreAs( int sam ) { store_as_mode = sam; }

  bool         GetEnablePreview() { return use_preview; }
  void         SetEnablePreview( bool state ) { use_preview = state; }

  bool         GetEnableLOD() { return use_lod; }
  void         SetEnableLOD( bool state ) { use_lod = state; }

  LWItemID     GetItemID() { return item[1]; }
  void         SetItemID( LWItemID new_item ) { item[1] = new_item; }

  float        GetLODNominalCameraZoom() { return lod_nominal_camera_zoom; }
  void         SetLODNominalCameraZoom( float value ) { lod_nominal_camera_zoom = value; }

  bool         GetLODUseCameraZoom() { return lod_use_camera_zoom; }
  void         SetLODUseCameraZoom( bool state ) { lod_use_camera_zoom = state; }

  float        GetLODDistanceMultiplier() { return lod_distance_multiplier; }
  void         SetLODDistanceMultiplier( float value ) { lod_distance_multiplier = value; }

  // Lightwave Instanace Functions
  void         CopyFrom( SimplifyInstance &other_inst );

  LWItemID   * GetItemList() { return item; }
  void         ChangeItemIDs( const LWItemID *new_items );
  const char * EvaluateReplacement( ObjReplacementAccess *objrep );

  // File I/O Functions
  bool         LoadFromFile( const char *filename, char * &error );
  bool         SaveToFile(   const char *filename, char * &error );

  bool         LoadFromScene( char *error_buffer, CLWLoadState *loader );
  bool         SaveToScene(   char *error_buffer, CLWSaveState *saver  );

  // Status Functions
  bool         GetSavedState() { return saved_state; }
  void         SetSavedState( bool state ) { saved_state = state; }

  // Static Functions
  static LWItemID     GetCameraID() { return camera_id; }
  static void         SetCameraID( LWItemID new_camera_id ) { camera_id = new_camera_id; }

  static GlobalFunc * GetGlobal() { return global; }
  static void         SetGlobal( GlobalFunc *_global ) { global = _global; }

  // LOD Array
  DynArray< SimplifyLOD * > lod;

protected:
  void         CreateDefaultLOD();
  const char * PickLOD( LWTime lw_time );

  LWItemID item[3];             // The list of items we're operating on (this is an array so we can return it as an item list for UseItems/ChangeIDs)

  bool     enabled;             // True if this instance is endabled
  bool     use_preview;         // True if we are using the Preview features
  bool     use_lod;             // True if we are using the LOD features

  char     preview_filename[ MAX_PATH_LENGTH ];     // Interactive Preview Object Path
  char     render_filename[ MAX_PATH_LENGTH ];      // Final Render Object Path
  char     sdf_filename[ MAX_PATH_LENGTH ];         // .SDF Path

  char     replacement_filename[ MAX_PATH_LENGTH ]; // Replacement Filename passed back to Lightwave

  bool     lod_use_camera_zoom;                     // True if using the Camera Zoom Factor as a modifier for LOD distance
  float    lod_nominal_camera_zoom;                 // Nominal Camera Zoom Factor
  float    lod_distance_multiplier;                 // Distance Muiltiplier

  int      store_as_mode;       // Always load the default config from the file, overriding the values saved in the scene file

  bool     saved_state;         // False if the instance was changed but not yet saved

  // Static Data
  static LWItemID     camera_id;   // Camera ID
  static GlobalFunc * global;      // Global Function
};

// SimplifyLOD
class SimplifyLOD {
public:
  SimplifyLOD();
  ~SimplifyLOD();

  // Accessor Functions
  const char * GetFilename() { return filename; }
  bool         SetFilename( const char *preview );

  float        GetDistance() { return swap_distance; }
  void         SetDistance( float dist ) { swap_distance = dist; }

  void         SetEnabled( bool state ) { enabled = state; }
  bool         GetEnabled() { return enabled; }

  const char * GenerateListerName();
  const char * GetListerName() { return lister_name; }

  // Instance Management
  void         CopyFrom( SimplifyLOD &other_inst );

  //File I/O
  bool         LoadFromStream( ifstream &in );
  bool         SaveToStream( ofstream &out );

  bool         LoadFromScene( CLWLoadState *loader );
  bool         SaveToScene(   CLWSaveState *saver  );

protected:
  bool   enabled;                      // True if this instance is endabled
  float  swap_distance;                // Distance to perform the replacement at

  char   filename[ MAX_PATH_LENGTH ];  // Filename to replace with.  If NULL, then replace with a Null Object instead
  char   lister_name[ 128 ];           // Name of the item as it appears in the LOD Listbox
};

#endif
