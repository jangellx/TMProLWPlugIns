//
// AllKeyerPanel.h
//
// This file contains all the All-Keyer GUI class AllKeyerPanel,
// which handles all the LWPanels-based GUI elements for the
// All-Keyer.
//

#pragma once

// Defines
#define MPS_EXIT       0
#define MPS_PROCESS    1

// Arrays
static const char *key_mode_names[] = {
  "Key at Every Frame",
  "Linear Approximation",
   NULL };
static const char *compare_mode_names[] = {
  "(none)",
  "Keep If Matched",
  "Remove If Matched",
   NULL };
static const char *log_mode_names[] = {
  "(no log)",
  "Errors Only",
  "Simple",
  "Detailed",
   NULL };

static char *about_text[] = {
  "All-Keyer",
  "for Lightwave 3D",
  "",
  "Copyright 1999 Joe Angell, TM Productions",
  "All Rights Reserved",
  "",
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// Class Definitions
class AllKeyerPanel;

// ------------------------------------------------
// -- All-Keyer GUI Class
class AllKeyerPanel {
public:
  AllKeyerPanel( GlobalFunc *_global,
                 CMessage *message, int &retval );
  ~AllKeyerPanel();

  // Setup
  bool MakeMainPanel(void);                      // Construct the main panel and add controls to it
  bool MakeProcPanel(void);                      // Construct the process panel and add controls to it
  bool MakeAboutPanel(void);                     // Counstruct and fill in the About Box

  // Open Panels
  int  OpenMain(void);                           // Open the Main Window and close the Processing Window
  bool OpenProcess(void);                        // Open the Processing Window and close the Main Window
  bool OpenAbout(void);                          // Open the About Window

  GlobalFunc    &Global;                         // Global Function Pointer

public:
  CLWPanelFuncs *panel_funcs;                    // Lightwave Panel Functions
  CMessage      *message;                        // Lightwave User Message Functions

  LWPanelID      main_panel;                     // Main Panel
  LWPanelID      proc_panel;                     // Processing Panel
  LWPanelID      about_panel;                    // About Panel

  // Main Panel Controls
  LWControl      *compare_mode;                  // Compare Mode pop-up
  LWControl      *use_frame_0;                   // Use Frame 0?
  LWControl      *last_key;                      // Last Key is Last Frame?
  LWControl      *pause_on_errors;               // Pause On errors?
  LWControl      *option_border;                 // Border around the Options section

  LWControl      *compare_file;                  // Compare file
  LWControl      *source_pattern;                // Source Pattern
  LWControl      *destination_path;              // Destination Path

  LWControl      *key_mode;                      // Key Mode pop-up
  LWControl      *key_frame_step;                // Key Mode "Key Every X Frames" Frame Step value
  LWControl      *key_threshold;                 // Key Mode "Linear Approximation" Threshold value

  LWControl      *log_file;                      // Log Filename
  LWControl      *log_mode;                      // Log Mode
  LWControl      *log_append;                    // Log Append Toggle
  LWControl      *log_border;                    // Border around the Log section

  LWControl      *process_scenes;                // Process Scenes button
  
  LWControl      *status;                        // Status Bar
  LWControl      *about;                         // About Box

  int             main_panel_state;              // State of the main panel.  See defines above

  // Processing Panel Control
  
  // About Panel Controls  
  LWControl      *about_area;                    // About Text Area
};

// Event Handlers
//  MSVC won't let me put these in a class and cast them
//  to an int so I can pass them to the LWControls as event
//  handlers, so they have to be put out here instead. Each
//  function recieves a pointer to the panel class passed
//  into each of these functions is a pointer to an instance
//  of AllKeyerPanels
void OnKeyMode( LWControl *ctl, AllKeyerPanel *panel );  // Called when the Key Mode changes
void OnAbout(   LWControl *ctl, AllKeyerPanel *panel );  // Pop up the About Box
void OnProcess( LWControl *ctl, AllKeyerPanel *panel );  // Called when the Process Scenes button is clicked
void OnCompare( LWControl *ctl, AllKeyerPanel *panel );  // Called when the compare file changes
void OnSource(  LWControl *ctl, AllKeyerPanel *panel );  // Called when the source pattern changes
void OnDest(    LWControl *ctl, AllKeyerPanel *panel );  // Called when the destination path changes
void OnLog(     LWControl *ctl, AllKeyerPanel *panel );  // Called when the log file changes

// Draw Functions
void DrawAbout(LWControl *ctl, AllKeyerPanel *akp2, DrMode mode);
