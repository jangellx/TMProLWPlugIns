//
// Spaceball-Interface.h
//

#include "spaceball.h"

#include <portable/stringtools/stringtools.h>

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

const int    num_functions = 28;
extern char *function_map[ num_functions ];

class SpaceBall_Interface {
public:
  SpaceBall_Interface();
  ~SpaceBall_Interface();

  void CreateUI();
  void OpenUI();
  bool IsOpen() { return is_open; }

  bool CreationOK() { return creation_ok; }

public:
  LWInterfaceInfo   * ui;
  LWMessageFuncs    * message;
  LWPanelFuncs      * panel_funcs;

  bool                creation_ok;
  bool                is_open;

  LWPanelID           panel;

  LWControl         * list;
  LWControl         * about;
  LWControl         * command;
  LWControl         * capture;
  LWControl         * configure;
  LWControl         * open_config_on_startup_toggle;

  int                 current_function;
};

int    ListCount( LWPanelID panel );
char * ListNames( LWPanelID panel, int item, int col );
int    ListColumnWidths( LWPanelID panel, int col );

void HandleAbout(               LWControl *con, void *data );
void HandleList(                LWControl *con, void *data );
void HandleCommand(             LWControl *con, void *data );
void HandleCapture(             LWControl *con, void *data );
void HandleConfigure(           LWControl *con, void *data );
void HandleOpenConfigOnStartup( LWControl *con, void *data );

