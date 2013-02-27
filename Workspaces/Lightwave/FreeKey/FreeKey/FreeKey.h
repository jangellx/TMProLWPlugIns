// FreeKey deletes keyframes from any hierarchy of objects and bones
//  in the scene.  This includes:
//
//  - Deleteing keys outside a user-defined range (ie: keep keys 80 to 150)
//  - Shift keys backward/forward, or shift to frame 0 after deleting.
//  - Delete every Xth frame within the range, or keep only every Xth
//     frame within the range.
//  - Anything else that affects keyframe data.
//
// This class also includes all variables that should persist between
//  invocations of the interface.
//

#ifndef HEADER_FREEKEY
#define HEADER_FREEKEY

#include <lightwave/lwscene/lwscene.h>
#include "freekey-group.h"

class FreeKey;

static const int FreeKey_Version  = 1;
static const int FreeKey_Revision = 9;

extern const char *freekey_error_strings[];

enum freekey_error_ids {
  FREEKEY_OK                          =   0,
  FREEKEY_ERROR_BASE                  = 101,
  FREEKEY_SOURCE_ISNT_LWS             = 101,
  FREEKEY_ERROR_READING_FROM_SOURCE,
  FREEKEY_LOW_GREATER_THAN_HIGH_INSIDE,         // When Remove Inside occurs
  FREEKEY_LOW_GREATER_THAN_HIGH_OUTSIDE,        // When Remove Outside occurs
  FREEKEY_USER_ABORT,
  FREEKEY_ITEM_NOT_IN_SCENE,
  FREEKEY_UNSUPPORTED_SCENE_VERSION,
  FREEKEY_ERROR_LOADING_MOTION,
  FREEKEY_LAST_ERROR };

enum fk_tool_option_tab_ids {
  TOOLTAB_ITEM_SELECTION = 0,
  TOOLTAB_KEY_SELECTION,
  TOOLTAB_GROUP,
  TOOLTAB_NUMERIC,
  TOOLTAB_OPTIONS,
  NUM_TOOL_TAB_IDS };

enum fk_drawstyle {
  DRAWSTYLE_56,
  DRAWSTYLE_L6 };

//
//
//

// FreeKey Class
class FreeKey {
public:
  // Constructor/Destructor
  FreeKey( const char * _source_name = NULL,
	         const char * _output_name = NULL );
	~FreeKey();

  // Filename Functions
  inline void        SetSource( const char *new_source );  // Set the Source Path
  inline const char *GetSource() { return source_name; }   // Get the Source Path

  inline void        SetOutput( const char *new_output );  // Get the Output Path
  inline const char *GetOutput() { return output_name; }   // Set the Output Path

  bool GenerateNames( const char *suffix );

  // Setup Functions
         void           Init( const char * _source_name = NULL,
	                            const char * _output_name = NULL );      // (Re)Initializes the instance
         int            OpenFiles();                                   // Make sure all the paths are valid, and open the source and output files
         int            OpenSource();                                  // Opens the source file only

  inline int            BuildHierarchy();                              // Build a hierarchy from the source scene
         void           FreeHierarchy();                               // Frees the hierarchy

         int            Process( bool(__cdecl Update)(char *name, int current, int total) = NULL );  // Process the scene.  Update is a callback function for updating UIs and aborting processing

  inline void           AddGroup();                                    // Adds a new group to the group list

  // Display Tree Accessors
  inline bool           GetDisplayTree() { return display_tree; }
  inline void           SetDisplayTree( bool state ) { display_tree = state; }

  // Count Accessors
  inline int            GetNumObjectsProcessed() { return items_processed;  }
  inline int            GetNumLightsProcessed()  { return lights_processed; }
  inline int            GetNumBonesProcessed()   { return bones_processed;  }

  // List Accessors
  inline LWScene &             GetScene() { return scene; }
  inline DynArray<LWItem *> &  GetItemList() { return item_list; }     // Returns the dynarray containing all the items in a one-dimensional list

  inline DynArray< FreeKeyGroup * >    &  GetGroupList() { return groups; }    // Returns the Group List

  // Static Functions
  static const char *   GetErrorString( int error );                           // Returns a string associated with an error enums

  // Interface-Level Accessors
  inline double         GetZoom() { return zoom; }
  inline void           SetZoom( double _zoom ) { zoom = _zoom; }

  inline double         GetCurrentTime() { return current_time; }
  inline void           SetCurrentTime( double _current_time ) { current_time = _current_time; }

  inline double         GetDisplayMin() { return display_min; }
  inline void           SetDisplayMin( double _display_min ) { display_min = _display_min; }

  inline double         GetDisplayMax() { return display_max; }
  inline void           SetDisplayMax( double _display_max ) { display_max = _display_max; }

  inline int            GetDivider() { return divider; }
  inline void           SetDivider( int _divider ) { divider = _divider; }

  inline int            GetNameScroll() { return name_scroll; }
  inline void           SetNameScroll( int _name_scroll ) { name_scroll = _name_scroll; }

  inline double         GetTimeScroll() { return time_scroll; }
  inline void           SetTimeScroll( double _time_scroll ) { time_scroll = _time_scroll; }

  inline int            GetKeyDisplayMode() { return key_display_mode; }
  inline void           SetKeyDisplayMode( int _key_display_mode ) { key_display_mode = _key_display_mode; }

  inline int            GetItemDisplayMode() { return item_display_mode; }
  inline void           SetItemDisplayMode( int _item_display_mode ) { item_display_mode = _item_display_mode; }

  inline fk_tool_option_tab_ids  GetToolOptionsTab() { return tool_options_tab; }
  inline void                    SetToolOptionsTab( fk_tool_option_tab_ids _tool_options_tab ) { tool_options_tab = _tool_options_tab; }

  inline unsigned long  GetCurrentItem() { return current_item; }
  inline void           SetCurrentItem( unsigned long _current_item ) { current_item = _current_item; }
  inline bool           IsCurrentItemValid() { return (current_item == -1) ? false : true; }

  inline long           GetTopItem() { return item_top; }
  inline void           SetTopItem( long _item_top ) { item_top = _item_top; }

  inline unsigned long  GetCurrentGroup() { return current_group; }
  inline void           SetCurrentGroup( unsigned long _current_group ) { current_group = _current_group; }

  inline fk_drawstyle   GetDrawStyle() { return draw_style; }
  inline void           SetDrawStyle( fk_drawstyle _draw_style ) { draw_style = _draw_style; }

  inline bool           GetUseOpenGLItemList() { return use_opengl_item_list; }
  inline void           SetUseOpenGLItemList( bool state ) { use_opengl_item_list = state; }

  inline bool           GetUseOpenGLLogo() { return use_opengl_logo; }
  inline void           SetUseOpenGLLogo( bool state ) { use_opengl_logo = state; }

  inline DynArray< LWItem * > &  GetSelectedItems() { return selected_items; }

protected:
  // Independant Variables
  bool display_tree;                      // Toggles tree dirsplay on and off.

  LWScene scene;                          // LWScene instance; contains hierarchy, motions, etc.
  DynArray<LWItem *> item_list;           // Unsorted list of items in the hierarchy tree; can be sorted if needed.

  DynArray< FreeKeyGroup * > groups;      // Group List

  char source_name[ MAX_PATH_LENGTH ];
  char output_name[ MAX_PATH_LENGTH ];

  // Process Count Variables
  int items_processed;
  int lights_processed;
	int bones_processed;

  // Interface-level variables
  double zoom;                               // Keyframe List Zoom Factor
  double current_time;                       // Current display time
  double display_min;                        // Low Keyframe to display in key list
  double display_max;                        // High Keyframe to display in key list
  int    name_scroll;                        // Current position the name slider is scrolled to in pixels
  double time_scroll;                        // Current position the time slider is scrolled to in time

  unsigned long current_item;                // Currently selected item (-1 for no item)
  unsigned long current_group;               // Currently selected group
           long item_top;                    // Item visible at the top of the item list

  DynArray< LWItem * >    selected_items;    // Pointers to the selected items.


  // Scene Independant Options
  int  divider;                              // Divider position
  int  key_display_mode;                     // Current Key Mode
  int  item_display_mode;                    // Current Display Mode
  bool use_opengl_item_list;                 // true if using the OpenGL Item List
  bool use_opengl_logo;                      // true if using the OpenGL Logo

  fk_drawstyle            draw_style;        // Draw Style (5.6, L6 style, etc.)
  fk_tool_option_tab_ids  tool_options_tab;  // Current Tool Options Tab
};

bool ScanHandler( int key_index );

// AddGroup():
//  Adds a new group to the group list.
inline void FreeKey::AddGroup() {
  groups.Add( new FreeKeyGroup() );
}

// BuildHierarchy()
inline int FreeKey::BuildHierarchy() {
  return scene.Load( LWS_LOAD_SCENE_DATA | LWS_LOAD_HIERARCHY | LWS_LOAD_MOTIONS | LWS_LOAD_FULL_POST, GetSource() );
//  return LWItem::BuildHierarchyFromScene( *_in, *base, item_list );
}

// SetSource():
//  Set the source filename.  This can't be done if the source
//   or output files are open.
inline void FreeKey::SetSource( const char *new_source ) {
  if( new_source == NULL )
    source_name[0] = '\0';
  else
    strcpy( source_name, new_source );
}

// SetOutput():
//  Set the output filename.  This can't be done if the source
//   or output files are open.
inline void FreeKey::SetOutput( const char *new_output ) {
  if( new_output == NULL )
    output_name[0] = '\0';
  else
    strcpy( output_name, new_output );
}

#endif
