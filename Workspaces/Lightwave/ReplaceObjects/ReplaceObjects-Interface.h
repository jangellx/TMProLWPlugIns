//
// ReplaceObjects-Interface.h
//

#include <portable/dynarray/dynarray.h>

#include "ReplaceObjects.h"
#include "ReplaceObjects-SwapObject.h"

// class ReplaceObjects
class ReplaceObjects_Interface {
public:
  ReplaceObjects_Interface( GlobalFunc *_global, void *local );
  ~ReplaceObjects_Interface();

  void ReselectItems();

  void FillObjectList();
  
  bool OpenUI();
  int  OpenProgressPanel();
  
  void PostError( const char *progress_error = NULL,
                  const char *buffer1 = NULL, const char *buffer2 = NULL );

  void Process();
  bool ProcessItem( LWItemID item, double total_weight, bool remember_name, const char * match_data );

public:
  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWInterfaceInfo *ui;
  LWItemInfo      *iteminfo;
  LWObjectInfo    *objectinfo;

  LWPanelFuncs    *panel_funcs;

  LWPanelID        panel;

  LWControl       *limit_to_popup;
  LWControl       *sort_by_popup;

  LWControl       *match_toggle;
  LWControl       *match_string;

  LWControl       *list;

  LWControl       *add;
  LWControl       *clone;
  LWControl       *remove;

  LWControl       *clear_all;

  LWControl       *save;
  LWControl       *load;

  LWControl       *about;

  LWControl       *path;
  LWControl       *weight;
  LWControl       *enable;

  LWControl       *store_original_name;

  // Variables
  LWItemType                              sel_type;
  DynArray< ReplaceObjects_SwapObject * > swaps;

  static DynArray< ReplaceObjects_SwapObject * > old_swaps;
  static int                                     store_original_name_value;

  LWPanelID        progress_panel;
  LWControl      * progress_field;

  bool             do_process;

  bool             progress_open;

  int              progress_total;
  int              progress_current;
  char             progress_string[ MAX_PATH_LENGTH ];

  char           * checkmark_string;
};

// Panel Callbacks
void DrawPanel( LWPanelID panel, void *data, DrMode mode );

void DrawProgressField( LWControl *con, void *data, DrMode mode );

// Handlers
void HandleList(        LWControl *con, void *data );
void HandleAbout(       LWControl *con, void *data );

void HandleSortBy(      LWControl *con, void *data );

void HandleMatchToggle( LWControl *con, void *data );

void HandleAdd(         LWControl *con, void *data );
void HandleClone(       LWControl *con, void *data );
void HandleRemove(      LWControl *con, void *data );
void HandleClearAll(    LWControl *con, void *data );

void HandleLoad(        LWControl *con, void *data );
void HandleSave(        LWControl *con, void *data );

void HandlePath(        LWControl *con, void *data );
void HandleWeight(      LWControl *con, void *data );
void HandleEnable(      LWControl *con, void *data );

// Callbacks
int    ListCount( LWPanelID panel );
char * ListNames( LWPanelID panel, int item, int col );
int    ListColumnWidths( LWPanelID panel, int col );

// Sort Functions
extern "C" {
  int SortSwapObjects_Alpha(const void *elem1, const void *elem2 );
  int SortSwapObjects_Weight(const void *elem1, const void *elem2 );
}
