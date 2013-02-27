
// All-Keyer.h
// This contains the class definitions for the AllKeyer class.

#pragma once

#include <Portable\DirTools\DirTools.h>                    // Directory Access and String Tools
#include <fstream.h>

class AllKeyer;

// ---------------------------
// -- All-Keyer Class
class AllKeyer {
public:
  AllKeyer( GlobalFunc *_global, void *_local, void *_serverData, int &_retval );
  ~AllKeyer();

  // Accessors
  inline const CLWItemInfo  *ItemInfo()    { return item_info;  }
  inline const CLWSceneInfo *SceneInfo()   { return scene_info; }
  inline const CMessage     *Message()     { return message;    }

protected:
  // Processing
  void OpenLog();                                // Opens the log file

  int ParseAKB();                                // Parse an AKB, processing all scenes referenced within
  int ProcessScene();                            // Process the Last Match that DirInfo found

  bool WriteMotion( ofstream &out, CLWItemInfo *item_info,
                    LWItemID id, CLWSceneInfo *scene_info );

  // Configuration
  void LoadConfig( void );                       // Load the All-Keyer Config File
  void SaveConfig( void );                       // Save the All-Keyer Config File

protected:
  bool TestOptions();                            // Test the Main Panel options before processing (paths, flags, etc.)

  // Lightwave Pointers
  GlobalFunc   &Global;                          // Lightwave Global Function reference
  void         *local;                           // Implementation-dependant data (we're a Generic Plug-in, so we don't really even use this)
  void         *server_data;                     // Server data, fromt the Activate function (will probably contain config settings in the future)

  int           &retval;                         // Return value passed back to the Activate function

  CLWItemInfo   *item_info;                      // Lightwave Item Information Functions
  CLWSceneInfo  *scene_info;                     // Lightwave Scene Information Functions
  CMessage      *message;                        // Lightwave User Message Functions
	DirInfoFunc   *dir_info;                       // Lightwave Directory Information Function

  // UI Pointers
  AllKeyerPanel *panel;                          // Pointer to the panel itself

  // Directory Tools
  DirInfo       *os_dir;                         // Operating System-based Directory Tools

  // Misc. Variables
  bool use_compare;                              // if we're using the compare file or not
};

