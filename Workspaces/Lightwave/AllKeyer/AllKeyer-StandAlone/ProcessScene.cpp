
#include <string.h>
#include "ProcessScene.h"

// GUIData Contructor
//  Eventually, this should also handle loading the .config file
//  in as the default settings
GUIData::GUIData() {
  strcpy( compare_name,     "(none)" );
  strcpy( source_name,      "(none)" );
  strcpy( destination_name, "(none)" );

  compare_mode = 0;       // Default Compare Mode
  key_mode     = 0;       // Default Key Mode

  strcpy( log_name,         "(none)" );
  log_append = false;     // Default Log Append State
  log_mode   = 0;         // Default Log Mode

  use_frame_0    = false; // Default Use Frame 0 State
  last_key       = true;  // Default Last Key is Last Frame State
  pause_on_error = true;  // Default Pause On Error State
}

