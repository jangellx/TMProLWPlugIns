//
// IMP-PiEAboutBox.h
//
// Borrowed from LWAboutBox, among other of my libs
//

#include "Motify-PiEToolsLogo.h"                  // PiE Tools

static const char *LWABOUT_HDIV   = "<HDIV>";     // Horizontal Divider
static const char *LWABOUT_VTAB   = "<VTAB>";     // Vertical Tab (skip a line)
static const char *LWABOUT_YELLOW = "<YELLOW>";   // Yellow Text Color
static const char *LWABOUT_BLACK  = "<BLACK>";    // Black text Color

class LWAboutPanel {
public:
  LWAboutPanel( LWPanelFuncs *_panel_funcs, 
                const char *title, const char **_lines,
                CSourceData *_logo,
                LWPanelID _parent, bool open = false,
                const char **_keys = NULL );
  ~LWAboutPanel();

  void Open();

public:
  LWPanelFuncs          *panel_funcs;

  LWPanelID              panel;
  LWPanelID              parent;

  LWControl             *tabs;
  LWControl             *key_lister;

  int                    text_top;
  const char **          lines;
  const char **          keys;

  int                    current_tab;
  int                    key_count;

  CSourceData           *logo;

  static LWAboutPanel   *about_panel;
};

// Panel Callbacks
void DrawAboutPanel( LWPanelID panel, LWAboutPanel *data, DrMode mode   );
void AboutPanelKeys( LWPanelID panel, LWAboutPanel *data, LWDualKey key );

// Event Callbacks
void HandleAboutPanelTabs(      LWControl *con, void *_data );

// Multilist Callbacks
int    AboutPanelKeyListCount(        void *_data );
char * AboutPanelKeyListNames(        void *_data, int item, int col );
int    AboutPanelKeyListColumnWidths( void *_data, int col );
