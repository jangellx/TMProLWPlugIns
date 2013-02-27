//
// IllumiSurf-Interface.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwpanel.h>
#include <lwxpanel.h>
#include <lwhandler.h>
#include <lwrender.h>
#include <lwhost.h>

#include "../paneltools/paneltoolsglobal.h"

#include "illumisurf-xpanel.h"

// LWPanels Macro Stuff
extern LWPanControlDesc desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

class IllumiSurf_Interface {
public:
  IllumiSurf_Interface( IllumiSurf_Instance *_inst );
  ~IllumiSurf_Interface();

  // Main Panel
         void CreateUI();
  inline bool IsCreationOK()  { return creation_ok; }

         void UpdateGhosts();

         void OpenUI();
  inline bool IsOpen()        { return is_open;     }

public:
  LWMessageFuncs      * message;
  LWPanelFuncs        * panel_funcs;
  LWXPanelFuncs       * xpanels;

  TMP_PanelTools      * panel_tools;
  TMP_PanelControls   * panel_controls;
  TMP_PanelDraw       * panel_draw;

  // Controls/XPanels
  LWPanelID             panel;

  LWControlID           instance_popup;

  LWControlID           main_control;
  LWXPanelID            main_xpanel;

  LWControlID           copy_button;
  LWControlID           paste_button;
  LWControlID           paste_popup;

  LWControlID           load_button;
  LWControlID           save_button;

  LWControlID           about_button;

  bool                  is_open;
  bool                  creation_ok;

  IllumiSurf_Instance * inst;        // Current instance
};

// HandlePanelClose():
void HandlePanelClose( LWPanelID panel, void *data );
void HandleDrawPanel(  LWPanelID panel, void *data, DrMode mode);

// Control Callbacks
void HandleInstancePopup( LWControl *control, void *user_data );
void HandleCopy(          LWControl *control, void *user_data );
void HandlePaste(         LWControl *control, void *user_data );
void HandlePastePopup(    LWControl *control, void *user_data );
void HandleLoad(          LWControl *control, void *user_data );
void HandleSave(          LWControl *control, void *user_data );
void HandleAbout(         LWControl *control, void *user_data );

// Xpanel Callbacks
void *              ISXPanel_Get( void *inst, unsigned long vid );
en_LWXPRefreshCodes ISXPanel_Set( void *_inst, unsigned long vid, void *value );

int    InstanceCount( void *userdata );
char * InstanceName(  void *userdata, int index );

