/*
  ProcessScene.h for the All-Keyer

  This contains the classes that deal with loading and maintaining the
  compare file data and loading, saving and all-keying scenes.  This is
  accomplished through the ObjectComparer and SceneAllKey classes,
  respectively. This also includes the KeyFrame and Motion structures
  used by MotCode.c

  When the Process button is clicked, the Comparer class is instantiated
  and the the compare data is loaded in.  Then an instance of AllKeyer
  processes a file and outputs the all-keyed version.  One instance of
  AllKeyer is used for each source file.

  When processing is complete, the remaining AllKeyer instance and the
  Comparer instance is destroyed.

  The following classes/typedefs are in this file:
    GUIData:         Stores the data fromt he Main Window controls when it
                     isn't open
    ObjectComparer:  Handles the compare file and testing objects against it
    SceneAllKey:     Loads, processes and all-keys a scene
*/

#pragma once

#include <fstream.h>
#include "MotCode.h"

// Defines
#define FILENAME_LENGTH 512
// Compare Modes
#define CMODE_NONE        0
#define CMODE_KEEP        1
#define CMODE_REMOVE      2
// Log Modes
#define LMODE_NONE        0
#define LMODE_ERRORS      1
#define LMODE_SIMPLE      2
#define LMODE_DETAILED    3

// Processing Step Enumeration
//  These enums are used by DoStep() in CProcessThread() and various
//  in SceneAllKey and ObjectComparer functions.
enum StepEnum {
  STEP_UNKNOWN     = -3,
  STEP_SCENE_ERROR = -2,  // Scene error (error loading data from scene, opening file, etc).
  STEP_FATAL_ERROR = -1,  // Fatal Error (no scenes match pattern, dest. not found, etc).
  STEP_ALL_DONE    =  0,  // Returned when finished processing
  // Setup
  STEP_INITIAL     =  1,  // First time DoStep is called
  STEP_LOAD_COMPARE,
  STEP_GET_SOURCE_LIST,
  // Processing
  STEP_OPEN_SOURCE,
  STEP_OPEN_DESTINATION,
  STEP_FIND_HIGH_FRAME,
  STEP_FIND_NEXT_OBJECT,
  STEP_LOAD_MOTION,
  STEP_ALL_KEY_MOTION,
  STEP_LINEAR_APROXIMATE_MOTION,
  STEP_SAVE_MOTION
};

// Class Definitions
class GUIData;
class Processing;
class ObjectComparer;
class SceneAllKey;

// GUIData:  Stores various UI settings.  This is used to fill the UI back in when
//           it's window is reopened, and used by SceneAllKey so it can access the
//           GUI data.  When the Main Window is closed, the data from the UI elements
//           is copied into a GUIData instance.  When the window is opened, the data
//           in GUIData is used to fill in the window's controls.

class GUIData {
public:
  GUIData();

  char compare_name[     FILENAME_LENGTH ];
  char source_name[      FILENAME_LENGTH ];
  char destination_name[ FILENAME_LENGTH ];

  char log_name[512];
  int  log_append;
  int  log_mode;

  int  compare_mode;
  int  key_mode;

  int  use_frame_0;
  int  last_key;
  int  pause_on_error;
};

// ObjectComparer:  Handles loading, saving and managing of the Compare File, which can
//                   be either a Lightwave Scene or an ASCII text file.
//                   The functions of this class return StepEnum's used by
//                   Processing::DoStep(), and thus should only be called from within
//                   that function.
class ObjectComparer {
public:
  ObjectComparer( char *in_name );
  ~ObjectComparer();

  bool FindMatch( char * keyword );

  // Accessor Functions
  inline const char * CompareName() { return in_name; }

protected:
  char *in_name;
};

// SceneAllKey:  Handles the loading, saving and all-keying of a Lightwave Scene.
//                All functions in this class return StepEnum's use by
//                Processing::DoStep(), and thus should only be called from within
//                that function.
class SceneAllKey {
public:
  SceneAllKey( GUIData* data, ObjectComparer *comparer );
  ~SceneAllKey();

  bool OpenSource();              // Open the Source LWS 
  bool OpenDestination();         // Open the Destination LWS

  bool FindHighFrame();           // Figures out the highest keyframe in the scene, and uses it as
                                  //  LastFrame if it is greater than LastFrame

  bool FindNextObject();          // Finds the next item in the scene (AddNullObject, LoadObject, AddLight or AddCamera)
  bool LoadMotion();              // Finds the motion of the object found by GotoNextItem()
  bool AllKeyMotion();            // All-keys the protected Motion
  bool LinearApproximateMotion(); // Linearly Aproximate the motion
  bool SaveMotion();              // Outputs the all-keyed motion

protected:
  // GUI Data
  GUIData *data;                  // Paths, filenames, flags, etc.

  // Compare Class
  ObjectComparer * comparer;      // Compare data

  // File IO streams
  ifstream in;                    // Input/Source stream
  ofstream out;                   // Output/Destination stream

  // Motions being input/output. Used by ReadMotion, AllKeyMotion and OutputMotion
  Motion *original_motion;           // Motion being processed.
  Motion *linear_aproximated_motion; // Linearly Aproximated Motion to save
  Motion *allkeyed_motion;           // All-keyed version of original_motion

  // Misc. Stuff
  int HighFrame;                  // Highest keyframe in scene, or LastKeyFrame
};

