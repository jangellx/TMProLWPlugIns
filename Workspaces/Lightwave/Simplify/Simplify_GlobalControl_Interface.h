//
// Simplifly_GlobalControl_Interface.h
//
// This contains the Simplify Global Control Interface, which provides
//  a unified front end to the Simplify Object Replacement Plugin.
//

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "simplify_objrep_interface.h"

// Class SimplifyGlobalInterface
class SimplifyGlobalInterface {
public:
  SimplifyGlobalInterface( GlobalFunc  *_global,
                           void        *local,
                           int         &retval );
  ~SimplifyGlobalInterface();

  // Panel Open/Close Functions
  bool OpenMainPanel();                          // Open the Main Panel
  void CloseMainPanel();                         // Close the Main Panel

  bool OpenAboutPanel();                         // Open the About Panel
  void CloseAboutPanel();                        // Close the About Panel

public:
  bool MakeMainPanel();                          // Create the main panel
  bool MakeAboutPanel();                         // Create the about panel

  bool InitMainPanel( bool initialize = true );

  GlobalFunc            &Global;                 // Global Function

  CLWPanelFuncs         *panel_funcs;            // Lightwave Panel Functions
  CMessage              *message;                // Lightwave User Message Functions
  CLWSceneInfo          *scene_info;             // Lightwave Scene Info

  LWPanelID              main_panel;             // The Main Panel
  LWPanelID              about_panel;            // The About Panel

  const char           * content_dir;                         // Content Directory
  char                   source_filename[ MAX_PATH_LENGTH ];  // Source Scene name
  char                   output_filename[ MAX_PATH_LENGTH ];  // Output Scene name
  char                   main_title_bar[  MAX_PATH_LENGTH ];  // Main Title Bar Name

  // Main Panel Controls
  LWControl     *about;                          // About Button

  LWControl     *source_scene;                   // Source scene to modifiy
  LWControl     *output_scene;                   // Output scene to created by Apply Changes

  LWControl     *instance_list_mode;             // Instance Lister Mode (all items, items with Simplify, etc.)
  LWControl     *instance_listbox;               // Instance Lister

  LWControl     *instance_add;                   // Apply a new Simplify Instance to the selected object
  LWControl     *instance_remove;                // Remove Simplify from the selected object  
  LWControl     *open_objrep_interface;          // Open the Object Repalcement Interface to edit the SDF

  LWControl     *generate_scene;                 // Generates a scene file containing all objects in all the SDFs in the currently loaded scene
  LWControl     *generate_scene_from_sdf;        // Generates a scene file containing all obejcts in an SDF

  LWControl     *apply_changes;                  // Applies changes from the source scene into the destination scene
};

// Draw Functions
void SG_DrawMainPanel(         LWPanelID panel, void *user, DrMode draw_mode );
//extern void DrawAboutPanel( LWPanelID panel, void *user, DrMode draw_mode );
void SG_DrawInstanceListbox(   LWControl *ctl, SimplifyInterface *si, DrMode mode );

// Event Handler Functions
void SG_OnAbout(               LWControl *ctl, SimplifyInterface *panel );

// Lister Functions
char * InstanceListerNames( void *dat, int index );
int    InstanceListerCount( void *dat );
