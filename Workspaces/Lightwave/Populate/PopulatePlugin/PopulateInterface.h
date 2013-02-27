//
// PopulateInterface.h
//

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK

#include "../Populate/Populate.h"

// Misc. UI String Arrays
static char *about_text[] = {
  "Populate",
  "for Lightwave 3D",
  "",
  "Copyright 1999 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// PopulateInterface Class
class PopulateInterface {
public:
  PopulateInterface( GlobalFunc *_global,
                     void * local,
                     int &retval );
  ~PopulateInterface();

  bool OpenMainPanel();                          // Open the Main Panel
  void CloseMainPanel();                         // Close the Main Panel

  bool OpenAboutPanel();                         // Open the About Panel
  void CloseAboutPanel();                        // Close the About Panel

public:
  bool MakeMainPanel();                          // Create the main panel
  bool MakeAboutPanel();                         // Create the about panel

  GlobalFunc    &Global;                         // Global Function

  CLWPanelFuncs *panel_funcs;                    // Lightwave Panel Functions
  CLWSceneInfo  *scene_info;                     // Lightwave Scene Info
  CMessage      *message;                        // Lightwave User Message Functions

  LWPanelID      main_panel;                     // The Main Panel
  LWPanelID      about_panel;                    // The About Panel

  // Main Panel Controls
  LWControl     *populate;
  LWControl     *about;

  LWControl     *source_scene;
  LWControl     *output_scene;
  LWControl     *base_item_name;
  LWControl     *base_item_list;

  LWControl     *replace_toggle;
  LWControl     *replace_path;
  LWControl     *replace_item_name;
  LWControl     *replace_item_list;

  LWControl     *lwo_toggle;
  LWControl     *lwo_name;

  LWControl     *translation;
  LWControl     *translation_mode;
  LWControl     *rotation;
  LWControl     *rotation_mode;
  LWControl     *stretch;
  LWControl     *scale;
  LWControl     *scale_mode;

  LWControl     *scale_types;                    // Used to toggle Uniform Scale and Stetch modes

  LWControl     *scale_time;
  LWControl     *scale_time_mode;
  LWControl     *delta;
  LWControl     *delta_mode;

  // About Panel Controls
  LWControl     *about_area;                     // About Area

  // Misc. Variables
  char         main_title[256];                  // Window Title Bar
  const char * content_dir;

  Populate   * pop;                              // The Populate Class
};


// Draw Functions
void DrawMainPanel( LWPanelID panel, void *data, DrMode mdoe );

void DrawAbout( LWControl *ctl, PopulateInterface *ui, DrMode mode );

// Listener Functions
void OnSourceFile(       LWControl *ctl, PopulateInterface *panel );
void OnOutputFile(       LWControl *ctl, PopulateInterface *panel );
void OnAbout(            LWControl *ctl, PopulateInterface *panel );

