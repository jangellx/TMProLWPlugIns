//
// LWAboutPanel_TMPro
//

#ifndef LWABOUTPANEL_TMPRO_HEADER
#define LWABOUTPANEL_TMPRO_HEADER

#define NULL 0

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>
#include "TMPro_MiniLogo.h"

static const char *LWABOUT_HDIV   = "<HDIV>";     // Horizontal Divider
static const char *LWABOUT_VTAB   = "<VTAB>";     // Vertical Tab (skip a line)
static const char *LWABOUT_YELLOW = "<YELLOW>";   // Yellow Text Color
static const char *LWABOUT_BLACK  = "<BLACK>";    // Black text Color

class LWAboutPanel_TMPro {
public:
  friend int AboutPanelKeyListCount( void *_data );

  LWAboutPanel_TMPro( LWPanelFuncs *_panel_funcs, 
                      const char *title, const char **_lines,
                      LWPanelID _parent, bool open = false,
                      const char **_keys = NULL, bool keyed = false );
  ~LWAboutPanel_TMPro();

          void Open( bool request_key = false );

          bool LoadKey( const char *filename, int dongle_id, char obfuscator[20] );
          bool SaveKey( const char *filename, int dongle_id, char obfuscator[20] );

  virtual bool TestKey( const char *key, int dongle_id ) { return false; }

public:
  LWPanelFuncs          *panel_funcs;

  LWPanelID              panel;
  LWPanelID              parent;

  LWControl             *tabs;
  LWControl             *key_lister;
  LWControl             *license;

  int                    text_top;
  const char **          lines;
  const char **          keys;

  int                    current_tab;
  int                    key_count;

  TMPro_MiniLogo        *tmpro_logo;

  static LWAboutPanel_TMPro *tmpro_about_panel;

  bool                   return_true_count;

  int                    TABID_ABOUT;
  int                    TABID_KEYS;
  int                    TABID_DONGLE;
};

// Panel Callbacks
void DrawAboutPanel( LWPanelID panel, LWAboutPanel_TMPro *data, DrMode mode   );
void AboutPanelKeys( LWPanelID panel, LWAboutPanel_TMPro *data, LWDualKey key );

// Event Callbacks
void HandleAboutPanelTabs(      LWControl *con, void *_data );

// Multilist Callbacks
int    AboutPanelKeyListCount(        void *_data );
char * AboutPanelKeyListNames(        void *_data, int item, int col );
int    AboutPanelKeyListColumnWidths( void *_data, int col );

#endif

