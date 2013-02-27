//
// Populate.h
//
// This contains the Populate class, which inherits from
//  CLIFileManipBase.  It contains all the main functions
//  of Populate seperate from the CLI/GUI parts of the
//  program.
//

#include <math.h>

#include <portable/clibase/clibase.h>
#include <portable/bfstream/bfstream.h>

#include "../../batarang/bataranghierarchy.h"

static const float Populate_Version = 1.0;

class Populate;
class PopulateReplacement;

extern const char *populate_error_strings[];
extern const char *populate_rep_error_strings[];

enum populate_error_ids {
  POPULATE_OK                         = 0,
  POPULATE_ERROR_BASE                 = 101,
  POPULATE_CANT_OPEN_LWO              = 101,
  POPULATE_SOURCE_ISNT_LWS,
  POPULATE_OBJECT_ISNT_LWO,
  POPULATE_ERROR_READING_FROM_SOURCE,
  POPULATE_LAST_ERROR };

enum populate_rep_error_ids {
  POPULATEREP_OK                      = 0,
  POPULATEREP_PATH_DOESNT_EXIST,
  POPULATEREP_BAD_OBJECTS,
  POPULATEREP_ERROR_OPENING_PATH,
  POPULATEREP_NUM_ERRORS };



// Populate Class
class Populate : public CLIFileManipBase {
public:
  enum ModifyMode {
    random    = 0,
    absolute,
    replace,
    num_modes
  };

  Populate( const char *_source_name = NULL,
            const char *_output_name = NULL,
            const char *_item_name   = NULL );

  ~Populate();

  // Setup Functions
  int                    OpenFiles();                   // Make sure all the paths are valid, and open the source and output files
  void                   CloseFiles();                  // Close the source and output files

  inline bool            BuildHierarchy();              // Build a hierarchy from the source scene
  void                   FreeHierarchy();               // Frees the hierarchy
  bool                   FindPNTS();                    // Seeks to the begining of the points chunk in the LWO, if needed
  populate_rep_error_ids LoadReplacementPath();         // Tests the Replacement Path and fills the Replacemend Array with the objects within

  int                    Process();                     // Performs the actual processing

  // Value Accessors
  inline const float *  GetPosition() { return t; }
  inline void           SetPosition( float x, float y, float z) { t[0] = x;  t[1] = y;  t[2] = z; }
  inline int            GetPositionMode() { return position_mode; }
  inline void           SetPositionMode( ModifyMode state ) { position_mode = state; }

  inline const float *  GetRotation() { return y; }
  inline void           SetRotation( float h, float p, float b) { y[0] = h;  y[1] = p;  y[2] = b; }
  inline ModifyMode     GetRotationMode() { return rotation_mode; }
  inline void           SetRotationMode( ModifyMode state ) { rotation_mode = state; }

  inline const float *  GetScale() { return h; }
  inline void           SetScale( float sx, float sy, float sz) { h[0] = sx;  h[1] = sy;  h[2] = sz; }
  inline ModifyMode     GetScaleMode() { return scale_mode; }
  inline void           SetScaleMode( ModifyMode state ) { scale_mode = state; }

  inline bool           GetUseUniformScale() { return use_uniform_scale; }
  inline void           SetUseUniformScale( bool state ) { use_uniform_scale = state; }

  // Time Accessors
  inline float          GetScaleTime() { return scale; }
  inline void           SetScaleTime( float _scale ) { scale = _scale; }
  inline ModifyMode     GetScaleTimeMode() { return scale_time_mode; }
  inline void           SetScaleTimeMode( ModifyMode state ) { scale_time_mode = state; }

  inline int            GetDelta() { return delta; }
  inline void           SetDelta( int _delta ) { delta = _delta; }
  inline ModifyMode     GetDeltaMode() { return delta_mode; }
  inline void           SetDeltaMode( ModifyMode state ) { delta_mode = state; }

  // Random Accessors
  inline int            GetSeed() { return seed; }
  inline void           SetSeed( int _seed ) { seed = _seed; }

  // LWO Accessors
  inline int            GetFirstPoint() { return first_point; }
  inline void           SetFirstPoint( int _first_point ) { first_point = _first_point; }

  inline bool           GetUseLWO() { return use_lwo; }
  inline void           SetUseLWO( bool state ) { use_lwo = state; }

  inline const char *   GetLWO() { return lwo_name; }
  inline void           SetLWO( const char *new_lwo );

  // Replacement Accessors
  inline bool           GetUseRepDir() { return use_repdir; }
  inline void           SetUseRepDir( bool state ) { use_repdir = state; }

  inline void           SetRepDir( const char *new_repdir );
  inline const char *   GetRepDir() { return repdir_name; }

  inline void           SetRepItem( const char *new_repitem );
  inline const char *   GetRepItem() { return repitem_name; }

  // Content Dir Accessors
  inline void           SetContentDir( const char *new_content );
  inline const char *   GetContentDir() { return content_dir; }

  // Item Accessors
  inline bool           SetItem( const char *new_item );
  inline const char *   GetItem() { return item_name; }

  // Display Tree Accessors
  inline bool           GetDisplayTree() { return display_tree; }
  inline void           SetDisplayTree( bool state ) { display_tree = state; }

  // Count Accessors
  inline int            GetNumObjectsProcessed() { return items_processed;  }
  inline int            GetNumLightsProcessed()  { return lights_processed; }
  inline int            GetNumItemsReplaced()    { return items_replaced;   }

  // List Accessors
  inline BatarangHierarchy *               GetBase() { return base; }        // Treat this as READ ONLY!!!!
  inline int                               GetLWOPointCount();               // Returns the number of points in the LWO
  inline DynArray<PopulateReplacement *> & GetRepList() { return rep_list; } // Returns the Replacement List as loaded by LoadReplacementPath().  Should be considered Read-Only!

  // Static Functions
  static const char *   GetErrorString( int error );                         // Returns a string associated with an error enums

protected:
  inline bool           AreFilesOpen();           // Overloaded; returns true if any of the files are open
  void                  NormalizeReplacements();  // Normalizes (puts between 0.0 and 1.0) the probabilities for replacement objects

  float t[3];                             // Max. amount of translation to perform
  float y[3];                             // Max. amount of rotation to perform
  float h[3];                             // Max. amount of scale to perform

  int   delta;                            // Max. amount of shift to apply to item keys
  float scale;                            // Max. amount of scale to apply to item keys

  ModifyMode position_mode;
  ModifyMode rotation_mode;
  ModifyMode scale_mode;
  ModifyMode delta_mode;
  ModifyMode scale_time_mode;

  int   seed;                             // Random seed
  int   first_point;                      // First point to use from the object
  int   point_count;                      // Number of points in the LWO Point Reference Object
  bool  display_tree;                     // Toggles tree dirsplay on and off.
  bool  use_uniform_scale;                // Use uniform scaling instead of stretching

  bool  use_lwo;                          // True if we use the points of a .lwo for item placement
  char  lwo_name[  MAX_PATH_LENGTH ];     // The full path of the Lightwave Object who's points will be uses for placement

  char  item_name[ MAX_PATH_LENGTH ];     // The name of the item we are shifting

  bool  use_repdir;                       // True if we're using the replacement object dir
  char  content_dir[  MAX_PATH_LENGTH ];  // Content Directory
  char  repdir_name[  MAX_PATH_LENGTH ];  // The name of the replacement object directory
  char  repitem_name[ MAX_PATH_LENGTH ];  // The name of item to replace
  DynArray< PopulateReplacement * > rep_list;  // List of replacement objects

  BatarangHierarchy *base;                // Base of the hierarchy tree
  DynArray<BatarangHierarchy *> item_list;// Unsorted list of items in the hierarchy tree

  bifstream * _lwo;                       // Input Lightwave Object who's points are being used for placement
  streampos   pnts_start;                 // Where the PNTS Chunk Data is in the LWO

  int items_processed;
  int lights_processed;
  int items_replaced;
};

// AreFilesOpen():
//  Returns true if the source or output files are open,
//   and if the lwo is open, if it's being used
inline bool Populate::AreFilesOpen() {
  if( use_lwo && (_lwo == NULL ) )
    return false;

  return CLIFileManipBase::AreFilesOpen();
}

// SetLWO():
//  Set the LWO filename.  This can't be done if the files are
//   open.
inline void Populate::SetLWO( const char *new_lwo ) {
  if( !AreFilesOpen() ) {
    if( new_lwo == NULL )
      lwo_name[0] = '\0';
    else
      strcpy( lwo_name, new_lwo );
  }
}

// SetRepDir():
//  Set the RepDir filename.  This can't be done if the files are
//   open.
inline void Populate::SetRepDir( const char *new_repdir ) {
  if( !AreFilesOpen() ) {
    if( new_repdir == NULL )
      repdir_name[0] = '\0';
    else
      strcpy( repdir_name, new_repdir );
  }
}

// SetRepItem():
//  Set the RepItem filename.  This can't be done if the files are
//   open.
inline void Populate::SetRepItem( const char *new_repitem ) {
  if( !AreFilesOpen() ) {
    if( new_repitem == NULL )
      repitem_name[0] = '\0';
    else
      strcpy( repitem_name, new_repitem );
  }
}


// SetContentDir():
//  Set the Content Directory.  This can't be done if the files are
//   open.
inline void Populate::SetContentDir( const char *new_content ) {
  if( !AreFilesOpen() ) {
    if( new_content == NULL )
      content_dir[0] = '\0';
    else {
      strcpy( content_dir, new_content );
      DirStrings::AddDirSeperator( content_dir );
    }
  }
}

// SetItem():
//  Set the Item Name.  Return false if the item name is invalid
inline bool Populate::SetItem( const char *new_item ) {
  if( new_item == NULL )
    item_name[0] = '\0';
  else {
    // Store only the file part
    char * temp;
    if( (temp = DirStrings::HasFile( new_item )) != NULL ) {
      strcpy( item_name, temp );
      
      // Remove a *, if the user put one in
      if( (temp = strchr( item_name, '*' )) != NULL ) {
        *temp = '\0';
      }
    } else
      return false;
  }

  return true;
}

// BuildHierarchy()
inline bool Populate::BuildHierarchy() {
  return BatarangHierarchy::BuildHierarchyFromScene( *_in, *base, item_list );
}

// GetLWOPointCount():
//  Returns the number of points in the LWO.  This requires that
//   FindPNTS() has been successfully called already.  Otherwise,
//   this will return -1
inline int Populate::GetLWOPointCount() {
  return point_count;
}


//
//  --- Class PopulateReplacement
//
// Replacement Paths work like this:  The path can either point to a:
//
//  Replacement Object Dir:  This is a directory full of objects.  Rach
//    object has an even chance of replacing the existing object.
//
//  Replacement Text File:  This is a text file containing probability/object
//    pairs in the following format:
//
//    #  Comment
//    #  Prob      Filename
//    1.0          tree_1.lwo
//    2.0          tree_2.lwo
//    0.5          tree_3.lwo
//
//    Setting all probabilities to 1.0 means that each item has an even chance
//    of being picked.  In the above example, tree_2 is twice as likely to be
//    picked as tree_1, and four times a likely as tree_3.
//

class PopulateReplacement {
public:
  PopulateReplacement() : valid(false), prob(0.0f) { filename[0] = '\0'; }

  // Accessors
  void         SetFilename( const char *_filename ) {
                 if( _filename == NULL ) filename[0] = '\0';
                 else strcpy( filename, _filename ); }
  const char * GetFilename() { return filename; }

  void         SetIsValid( bool state ) { valid = state; }
  float        IsValid() { return prob; }

  void         SetProb( float _prob ) { prob = _prob; }
  float        GetProb() { return prob; }

protected:
  char  filename[ MAX_PATH_LENGTH ];      // Filename of the object
  bool  valid;                         // True if the file exists/is valid; false if it doesn't
  float prob;                             // Probablity that this item will be used
};

