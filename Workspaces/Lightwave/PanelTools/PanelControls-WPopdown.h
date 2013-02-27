//
// PanelControls_WPopdown.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwpanel.h>
#include "PanelToolsGlobal.h"

#define CONTEXT_MENU( cmenu, name, num, userdata ) (desc.type=LWT_POPUP, desc.popup.nameFn=name, desc.popup.countFn=num,\
                                                   (*cmenu->cmenuCreate)( &desc.popup, userdata ))

// class PanelControls_WPopdown
class PanelControls_WPopdown {
public:
  PanelControls_WPopdown( GlobalFunc *_global, LWPanelID _panel, int width,
                          const char *_title, const char **_strings );

  PanelControls_WPopdown( GlobalFunc *_global, LWPanelID _panel, int width, 
                          const char *_title, WPopdown_Name *_name, WPopdown_Count *_count );

  ~PanelControls_WPopdown();

  // Management
         int           OpenMenu( bool force );

  // Accessors
  inline LWControl   * GetControl() { return control; }

  inline const char ** GetStrings() { return strings; }

  inline void          SetEvent( WPopdown_Event *_event, void *_user_data ) { event = _event; user_data = _user_data; }

public:
  LWPanelFuncs      * panel_funcs;
  LWRasterFuncs     * raster_funcs;
  ContextMenuFuncs  * context_funcs;

  LWPanelID           panel;
  LWControl         * control;

  LWContextMenuID     menu;

  const char        * title;

  WPopdown_Event    * event;
  void              * user_data;

  // Used by callback version
  WPopdown_Count    * count;
  WPopdown_Name     * name;

  // Used by the string version
  int                 string_count;
  const char       ** strings;

  int                 prev_index;

  GlobalFunc        * global;
};

int    WPopdown_StringCount(  void * data );
char * WPopdown_StringName(   void * data, int index );
void   WPopdown_ControlEvent( LWControlID con, void *data );
void   WPopdown_ControlDraw(  LWControl *con, void *data, DrMode mode );

