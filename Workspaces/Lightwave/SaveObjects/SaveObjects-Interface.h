//
// SaveObects-Interface.h
//

#include <portable/dynarray/dynarray.h>

#include "SaveObjects.h"
#include "SaveObjects-Object.h"

// class SaveObjects_ProcessData
class SaveObjects_ProcessData {
public:
  SaveObjects_ProcessData( LWLayoutGeneric *_generic, bool _overwrite_read_only = false,
                           int _post_save_mode = PSFM_PREVIOUS_STATE )
    : generic(_generic), overwrite_read_only(_overwrite_read_only),
      post_save_mode(_post_save_mode), versioning_mode(SOVM_DISABLED),
      min_digits(2), seperator(0) {

    path[0] = prefix[0] = suffix[0] = '\0';
  }

public:
  LWLayoutGeneric *generic;
  LWMessageFuncs  *message;

  bool             overwrite_read_only;
  int              post_save_mode;
  int              versioning_mode;
  int              min_digits;                 // Only for Revision modes
  int              seperator;                  // Only for Revision modes
  char             path[   MAX_PATH_LENGTH ];  // Only for Revision Backup/Duplicate Modes
  char             prefix[ MAX_PATH_LENGTH ];  // Only for Duplicate mode
  char             suffix[ MAX_PATH_LENGTH ];  // Only for Duplicate mode
};

// SaveObjects_Interface
class SaveObjects_Interface {
public:
  SaveObjects_Interface( GlobalFunc *_global, void *local );
  ~SaveObjects_Interface();

  bool BuildItemList();
  void ReselectItems();

  void FillObjectList();
  
  bool OpenUI();
  bool ObtainUIStates();

  void PostError( const char *progress_error = NULL,
                  const char *buffer1 = NULL, const char *buffer2 = NULL );

public:
  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWInterfaceInfo *ui;
  LWItemInfo      *iteminfo;
  LWObjectInfo    *objectinfo;

//  LWXPanelFuncs   *xpanels;
  LWXPanelFuncs   *xpanels;
  LWPanelFuncs    *panel_funcs;

  LWPanelID        panel;

  LWControl       *limit_to_popup;
  LWControl       *alpha_toggle;
  LWControl       *refresh_button;
  LWControl       *pick_popdown;
  LWControl       *about;

  LWControl       *list;

  LWControl       *post_save_popup;
  LWControl       *save_read_only_toggle;

  LWControl       *versioning_popup;
  LWControl       *min_digits_int;
  LWControl       *seperator_popup;

  LWControl       *path_string;
  LWControl       *prefix_string;
  LWControl       *suffix_string;

  LWControl       *progress_field;
  LWControl       *save_marked_objects_button;

  char            *checkmark_string;

  // Static Control States
  static int  limit_to_state;
  static int  alpha_state;
  static int  post_save_state;
  static int  save_read_only_state;
  static int  versioning_state;
  static int  min_digits_state;
  static char path_string_state[   MAX_PATH_LENGTH ];
  static char prefix_string_state[ MAX_PATH_LENGTH ];
  static char suffix_string_state[ MAX_PATH_LENGTH ];
  static int  seperator_state;

  // Processing Variables
  DynArray< LWItemID > items;
  LWItemType           sel_type;

  DynArray< SaveObjects_Object * > objects;

  float            progress;
  char             progress_string[512];
};

// Callbacks
int    ListCount( LWPanelID panel );
char * ListNames( LWPanelID panel, int item, int col );
int    ListColumnWidths( LWPanelID panel, int col );

// Event Handlers
void HandleList(              LWControl *con, void *data );
void HandleLimitToPopup(      LWControl *con, void *data );
void HandleAlphaToggle(       LWControl *con, void *data );
void HandlePickPopdown(       LWControl *con, void *data );
void HandleAbout(             LWControl *con, void *data );
void HandleRefreshButton(     LWControl *con, void *data );
void HandleVersioningPopup(   LWControl *con, void *data );
void HandleMinDigitsInt(      LWControl *con, void *data );
void HandlePathString(        LWControl *con, void *data );
void HandleSaveMarkedObjects( LWControl *con, void *data );

// Draw Functions
void DrawProgressField( LWControl *con, void *data, DrMode mode );

// Sort Functions
int __cdecl SortObjectsAlpha(   const void *elem1, const void *elem2 );
int __cdecl SortObjectsNatural( const void *elem1, const void *elem2 );
