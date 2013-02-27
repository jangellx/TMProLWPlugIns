//
// MotionReferencerInterface.h
//

#include <lwserver.h>
#include <lwmotion.h>

#include <lightwave/l6_sdk_cpp_wrappers/l6wrappers.h>

#include "MotionReferencerInstance.h"

// Misc. UI String Arrays
static char *about_text[] = {
  "MotionReferencer",
  "for Lightwave 3D",
  "",
  "Copyright 2000 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

class MotionReferencerInterface;

class MotionReferencerInterface {
public:
  MotionReferencerInterface( GlobalFunc *_global, MotionReferencerInstance *_instance, int &retval );
  ~MotionReferencerInterface();

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
  void TestInstance( MotionReferencerInstance *output_inst );

  void StripContent( MotionReferencerInstance *inst );
  void AddContent(   MotionReferencerInstance *inst );

  GlobalFunc            &Global;                 // Global Function
  LWItemMotionHandler   *local;                  // Item Motion Handler, so we can change the Describe Line

  CLWPanelFuncs         *panel_funcs;            // Lightwave Panel Functions
  CMessage              *message;                // Lightwave User Message Functions

  LWPanelID              main_panel;             // The Main Panel
  LWPanelID              about_panel;            // The About Panel
  LWPanelID              keys_panel;             // The Keyboard Help Panel

  char main_title_bar[256];                      // Main Title Bar Name

  // Main Panel Controls
  LWControl     *about;                          // About Button
  LWControl     *f1;                             // Keyboard Help
  LWControl     *enable_mr;                      // Enable Motion Referencer
  LWControl     *load_mrf;                       // Load MRF
  LWControl     *save_mrf;                       // Save MRF
  LWControl     *instance_popup;                 // Instance Popup

  // Instance Data
  MotionReferencerInstance            instance;          // Instance we are currently editing
  MotionReferencerInstance          * orig_instance;     // Original Instance from LW (filled in when the interface exits with OK)

  char                   content_dir[ MAX_PATH_LENGTH ];

  // Keyboard Control
  bool shift_down;         // true if either shift key is down
  bool ctrl_down;          // true if either control key is down
  bool alt_down;           // true if either alt key is down
  int  last_key_down;      // ID of the last key pressed
};

// Draw Functions
void DrawMainPanel(     LWPanelID panel, void *user, DrMode draw_mode );
void DrawAboutPanel(    LWPanelID panel, void *user, DrMode draw_mode );
void DrawKeysPanel(     LWPanelID panel, void *data, DrMode mode );

// Event Handler Functions
void OnAbout(           LWControl *ctl, MotionReferencerInterface *panel );
void OnEnableMR(        LWControl *ctl, MotionReferencerInterface *panel );
void OnLoadMRF(         LWControl *ctl, MotionReferencerInterface *panel );
void OnSaveMRF(         LWControl *ctl, MotionReferencerInterface *panel );
void OnInstancePopup(   LWControl *ctl, MotionReferencerInterface *panel );
void OnF1(              LWControl *ctl, MotionReferencerInterface *panel );

void OnKeyDown(         LWPanelID panel, void *data, LWDualKey key );
void OnKeyUp(           LWPanelID panel, void *data, LWDualKey key );

// Popup Functions
char * InstanceNames( void *dat, int index );
int    InstanceCount( void *dat );
