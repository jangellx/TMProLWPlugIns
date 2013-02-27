//
// Simpilfy_ObjRep_interface.h
//
// This file contains the class definition for the Object Replacement
//  plug-in's interface.
//

#ifndef SIMPLIFY_OBJREP_INTERFACE_HEADER
#define SIMPLIFY_OBJREP_INTERFACE_HEADER

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "simplify_objrep_instance.h"
#include "Simplify_Dongle.h"

class SimplifyInterface;

// Misc. UI String Arrays
static char *about_text[] = {
  "Simplify",
  "for Lightwave 3D",
  "",
  "Copyright 1999-2000 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// Store-As Strings
static const char *store_as_strings[] = {
  "filename of .SDF in .LWS",
  "all data in .LWS",
  NULL };

// LOD Listbox Data
static const int item_height        =    12;         // Height of each line
static const int item_text_offset   =    20;         // Horizontal position of the item name
static const int item_toggle_offset =     1;         // Modifier to item_text_x for enable/disable
static const int item_dist_offset   =   300;         // Modifier to item_text_x for distance display

// SimplifyInterface
class SimplifyInterface {
public:
  SimplifyInterface( GlobalFunc       *_global,
                     SimplifyInstance *_instance,
                     int              &retval );
  ~SimplifyInterface();

  // Panel Open/Close Functions
  bool OpenMainPanel();                          // Open the Main Panel
  void CloseMainPanel();                         // Close the Main Panel

  bool OpenAboutPanel();                         // Open the About Panel
  void CloseAboutPanel();                        // Close the About Panel

public:
  bool MakeMainPanel();                          // Create the main panel
  bool MakeAboutPanel();                         // Create the about panel
  bool MakeKeysPanel();                          // Create the keyboard help panel

  bool OpenKeysPanel();                          // Open the Keyboard Help Panel
  void CloseKeysPanel();                         // Close the Keyboard Help Panel

  bool InitMainPanel( bool initialize = true );
  void TestInstance( SimplifyInstance *output_inst );

  void StripContent( SimplifyInstance *inst );
  void AddContent(   SimplifyInstance *inst );

  void UpdateLOD( int index );                   // Updates the LOD controls with the given index into the instance->lod array

  GlobalFunc            &Global;                 // Global Function
  ObjReplacementHandler *local;                  // Object Replacement Handler, so we can change the Describe Line

  CLWPanelFuncs         *panel_funcs;            // Lightwave Panel Functions
  CMessage              *message;                // Lightwave User Message Functions

  LWPanelID              main_panel;             // The Main Panel
  LWPanelID              about_panel;            // The About Panel
  LWPanelID              keys_panel;             // The Keyboard Help Panel

  char main_title_bar[256];                      // Main Title Bar Name

  // Main Panel Controls
  LWControl     *about;                          // About Button
  LWControl     *f1;                             // Keyboard Help
  LWControl     *enable_simplify;                // Enable Simplify
  LWControl     *load_sdf;                       // Load SDF
  LWControl     *save_sdf;                       // Save SDF
  LWControl     *instance_popup;                 // Instance Popup

  // -- Preview/Render Controls
  LWControl     *enable_preview;                 // Enable Preview Models
  LWControl     *preview_filename;               // Preview Object Filename
  LWControl     *render_filename;                // Render Object Filename

  // -- LOD Controls
  LWControl     *enable_lod;                     // Toggle LOD usage
  LWControl     *lod_distance_multiplier;        // LOD Distance Multiplier
  LWControl     *lod_nominal_camera_zoom;        // LOD Nominal Camera Zoom
  LWControl     *lod_use_camera_zoom;            // Toggle Nominal Camera Zoom
  LWControl     *lod_listbox;                    // LOD Lister
  LWControl     *lod_scroll;                     // LOD List Sroll Bar

  LWControl     *lod_add;                        // Add a new LOD Model
  LWControl     *lod_remove;                     // Remove an existing LOD Model
  LWControl     *lod_clone;                      // Clone an existing LOD Model
  LWControl     *lod_enable;                     // Enable an existing LOD Model
  LWControl     *lod_filename;                   // Filename of an existing LOD Model
  LWControl     *lod_distance;                   // Swap Distance of an existing LOD Model

  LWControl     *store_as;                       // Store In Scene As mode

  // Dongle Class
  SimplifyDongle            * simpd;             // Simplify Dongle Class

  // Instance Data
  SimplifyInstance            instance;          // Instance we are currently editing
  SimplifyInstance          * orig_instance;     // Original Instance from LW (filled in when the interface exits with OK)

  char                        content_dir[ MAX_PATH_LENGTH ];
  unsigned int                selected_lod;      // Currently selected LOD model
  int                         lod_top;           // Current top of the list

  bool                        shift_down;
  bool                        ctrl_down;
  bool                        alt_down;
  int                         last_key_down;
};

// Sort Functions
int (__cdecl SortLOD)( const void *arg1, const void *arg2 );

// Draw Functions
void DrawMainPanel(     LWPanelID panel, void *user, DrMode draw_mode );
void DrawAboutPanel(    LWPanelID panel, void *user, DrMode draw_mode );
void DrawKeysPanel(      LWPanelID panel, void *data, DrMode mode );
void DrawLODListbox(    LWControl *ctl, SimplifyInterface *si, DrMode mode );

// Event Handler Functions
void OnAbout(           LWControl *ctl, SimplifyInterface *panel );
void OnEnableSimplify(  LWControl *ctl, SimplifyInterface *panel );
void OnEnablePreview(   LWControl *ctl, SimplifyInterface *panel );
void OnEnableLOD(       LWControl *ctl, SimplifyInterface *panel );
void OnPreviewFilename( LWControl *ctl, SimplifyInterface *panel );
void OnRenderFilename(  LWControl *ctl, SimplifyInterface *panel );
void OnLoadSDF(         LWControl *ctl, SimplifyInterface *panel );
void OnSaveSDF(         LWControl *ctl, SimplifyInterface *panel );
void OnLODListbox(      LWControl *ctl, SimplifyInterface *panel );
void OnLODScroll(       LWControl *ctl, SimplifyInterface *panel );
void OnLODAdd(          LWControl *ctl, SimplifyInterface *panel );
void OnLODEnable(       LWControl *ctl, SimplifyInterface *panel );
void OnLODRemove(       LWControl *ctl, SimplifyInterface *panel );
void OnLODClone(        LWControl *ctl, SimplifyInterface *panel );
void OnLODFilename(     LWControl *ctl, SimplifyInterface *panel );
void OnLODDistance(     LWControl *ctl, SimplifyInterface *panel );
void OnStoreAs(         LWControl *ctl, SimplifyInterface *panel );
void OnInstancePopup(   LWControl *ctl, SimplifyInterface *panel );
void OnLODDistanceMultiplier( LWControl *ctl, SimplifyInterface *panel );
void OnLODUseCameraZoom(      LWControl *ctl, SimplifyInterface *panel );
void OnLODNominalCameraZoom(  LWControl *ctl, SimplifyInterface *panel );
void OnF1(              LWControl *ctl, SimplifyInterface *panel );

void OnKeyDown(         LWPanelID panel, void *data, DualKey key);
void OnKeyUp(           LWPanelID panel, void *data, DualKey key);

// Lister Functions
char * LODListerNames( void *dat, int index );
int    LODListerCount( void *dat );

// Popup Functions
char * InstanceNames( void *dat, int index );
int    InstanceCount( void *dat );

void UpdateGhosts();

#endif