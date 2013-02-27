//
// UberNullInterface.h
//
// This contains the UberNull Interface Class, repsonsible
// for creating and maintaining the UberNull UI from within
// Lightwave.
//

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK
#include "..\UberNull\UberNull.h"

// Misc. UI String Arrays
static char *about_text[] = {
  "UberNull",
  "for Lightwave 3D",
  "",
  "Copyright 1999 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

static const char *UberNull_MatchModes[] = {
  "Starting With",
  "End With",
  "Containing",
  "Exact Match",
  NULL };

static const char *UberNull_NullLocations[] = {
  "Scene Start",
  "Scene End",
  "Hierarchy Start",
  NULL };

// Globals
extern UberNull *un;

// Name Listbox Data
static const int item_height        =    14;         // Height of each line
static const int item_text_offset   =    20;         // Horizontal position of the null name
static const int item_toggle_offset =     1;         // Modifier to item_text_x for enable/disable

// UberNullInterface Class
class UberNullInterface {
public:
  UberNullInterface( GlobalFunc *_global,
                     void * local,
                     int &retval );
  ~UberNullInterface();

  bool OpenMainPanel();                          // Open the Main Panel
  void CloseMainPanel();                         // Close the Main Panel

  bool OpenAboutPanel();                         // Open the About Panel
  void CloseAboutPanel();                        // Close the About Panel

public:
  bool MakeMainPanel();                          // Create the main panel
  bool MakeAboutPanel();                         // Create the about panel

  void UpdateName( int index );                  // Update UberNull interface Name data

  GlobalFunc    &Global;                         // Global Function

  CLWPanelFuncs *panel_funcs;                    // Lightwave Panel Functions
  CLWSceneInfo  *scene_info;                     // Lightwave Scene Info
  CMessage      *message;                        // Lightwave User Message Functions

  LWPanelID      main_panel;                     // The Main Panel
  LWPanelID      about_panel;                    // The About Panel

  // Main Panel Controls
  LWControl     *source_scene;                   // Source Scene File, to which UberNull will be applied
  LWControl     *output_scene;                   // Output Scene File, which contains the UberNull
  LWControl     *ubernull_master_name;           // The name of the UberNull in the scene
  LWControl     *ubernull_master_enable;         // The name of the UberNull in the scene
  LWControl     *ubernull_master_location;       // The location of the UberNull in the scene
  LWControl     *ubernullify;                    // Click to begin processing the scene
  LWControl     *about;                          // About UberNull
  LWControl     *save_settings;                  // Save Settings
  LWControl     *load_settings;                  // Load Settings

  // Hierarchy Null Name Controls
  LWControl     *name_listbox;                   // Name Lister
  LWControl     *name_scroll;                    // Name List Sroll Bar

  LWControl     *name_use;                       // Use Names
  LWControl     *name_add;                       // Add a new Name
  LWControl     *name_remove;                    // Remove an existing Name
  LWControl     *name_clone;                     // Clone an existing Name
  LWControl     *name_enable;                    // Enable an existing Name
  LWControl     *name_null;                      // Filename of an existing Name
  LWControl     *name_match;                     // Filename pattern to match
  LWControl     *name_match_mode;                // Matching method
  LWControl     *name_location;                  // Location of the NULL in the scene

  // Misc. Variables
  char         main_title[256];                  // Window Title Bar
  const char * content_dir;

  int          selected_name;
  int          name_top;
};

// Draw Functions
void DrawMainPanel(  LWPanelID panel, void *data, DrMode mdoe );
void DrawAboutPanel( LWPanelID panel, void *data, DrMode mdoe );
void DrawNameListbox(    LWControl *ctl, UberNullInterface *si, DrMode mode );

// Listener Functions
void OnSourceFile(       LWControl *ctl, UberNullInterface *panel );
void OnOutputFile(       LWControl *ctl, UberNullInterface *panel );
void OnUberNullName(     LWControl *ctl, UberNullInterface *panel );
void OnUberNullLocation( LWControl *ctl, UberNullInterface *panel );
void OnUberNullify(      LWControl *ctl, UberNullInterface *panel );
void OnAbout(            LWControl *ctl, UberNullInterface *panel );
void OnNameScroll(       LWControl *ctl, UberNullInterface *panel );
void OnNameListbox(      LWControl *ctl, UberNullInterface *panel );
void OnNameAdd(          LWControl *ctl, UberNullInterface *panel );
void OnNameRemove(       LWControl *ctl, UberNullInterface *panel );
void OnNameClone(        LWControl *ctl, UberNullInterface *panel );
void OnNameEnable(       LWControl *ctl, UberNullInterface *panel );
void OnNameNull(         LWControl *ctl, UberNullInterface *panel );
void OnNameMatch(        LWControl *ctl, UberNullInterface *panel );
void OnNameMatchMode(    LWControl *ctl, UberNullInterface *panel );
void OnNameLocation(     LWControl *ctl, UberNullInterface *panel );
void OnNameEnable(       LWControl *ctl, UberNullInterface *panel );
void OnNameUse(          LWControl *ctl, UberNullInterface *panel );
void OnEnableMaster(     LWControl *ctl, UberNullInterface *panel );
void OnSaveSettings(     LWControl *ctl, UberNullInterface *panel );
void OnLoadSettings(     LWControl *ctl, UberNullInterface *panel );
