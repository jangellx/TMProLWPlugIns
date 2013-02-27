//
// EchoToMorphMixer-Interface.h
//

#include "EchoToMorphMixer.h"

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>

#include "EchoToMorphMixer-EchoLWItem.h"
#include "EchoToMorphMixer-EchoMappings.h"

enum EchoListIDs {
  ECHOLIST_ENDO = 0,
  ECHOLIST_SYNONYM,
  ECHOLIST_TARGET,
  ECHOLIST_VISEME,
  ECHOLIST_ASSIGNED };

// EchoToMM_Interface
class EchoToMM_Interface {
public:
  EchoToMM_Interface( GlobalFunc *_global, void *local );
  ~EchoToMM_Interface();

  bool BuildItemList();
  void ReselectItems();

  bool OpenUI();
  int  OpenProgressPanel();

  int  BuildEndomorphList();

  void UpdateWithCurrentMapping();

  bool AddSynonym( const char *path );
  
  int FindLinkedEndomorphAtIndex( int _index, bool linked );
  int FindLinkedVisemeAtIndex( int _index, bool linked );

  bool Process();

public:
  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs     *message;
  LWInterfaceInfo    *ui;
  LWItemInfo         *iteminfo;
  LWObjectInfo       *objectinfo;
  LWObjectFuncs      *objectfuncs;
  LWSceneInfo        *sceneinfo;
  LWChannelInfo      *chaninfo;
  LWEnvelopeFuncs    *env_funcs;
  LWFileReqFunc      *freq_func;

  LWPanelFuncs    *panel_funcs;

  // Main UI
  LWPanelID        panel;

  LWControl       *project_mapping_freq;
  LWControl       *viseme_freq;
  LWControl       *object_popup;
  LWControl       *shift_keys;
  LWControl       *scale_values;
  LWControl       *clip_values;

  LWControl       *setup;
  LWControl       *about;

  LWControl       *mapping_name;
  LWControl       *mapping_popup;

  LWControl       *synonym_list;
  LWControl       *synonym_add;
  LWControl       *synonym_remove;

  LWControl       *endomorph_list;
  LWControl       *endomorph_reload;

  LWControl       *link;

  LWControl       *hide_linked;
  LWControl       *find_link;

  LWControl       *unlink;
  LWControl       *clear_links;

  LWControl       *test_links;

  LWControl       *viseme_list;
  LWControl       *viseme_reload;

  LWControl       *assigned_list;

  LWControl       *save_project_mapping;

  int              current_mapping;
  int              current_synonym;
  int              current_target;
  int              current_viseme;
  int              current_assigned;

  // Endomorph Object List
  DynArray< EchoLWItem * > objects;

  // Progress Panel
  LWPanelID        progress_panel;
  LWControl       *progress_field;

  // Keyboard States
  bool             ctrl_down;
  bool             shift_down;

  // Processing Variables
  int              progress_total;
  int              progress_current;
  char             progress_string[ MAX_PATH_LENGTH ];

  EchoListIDs      last_clicked_linkable_list;
  EchoListIDs      last_clicked_list;

  // Static State Variables
  static int    object_state;
  static char   viseme_freq_state[  MAX_PATH_LENGTH ];
  static char   project_freq_state[ MAX_PATH_LENGTH ];
  static double shift_state;
  static double scale_state;
  static int    clip_values_state;

  char         *checkmark_string;
};

// Multilist Callbacks
int    SynCount(        LWPanelID panel                    );
char * SynNames(        LWPanelID panel, int item, int col );
int    SynColumnWidths( LWPanelID panel, int col           );

int    EndoCount(        LWPanelID panel                    );
char * EndoNames(        LWPanelID panel, int item, int col );
int    EndoColumnWidths( LWPanelID panel, int col           );

int    VisCount(        LWPanelID panel                    );
char * VisNames(        LWPanelID panel, int item, int col );
int    VisColumnWidths( LWPanelID panel, int col           );

int    AssCount(        LWPanelID panel                    );
char * AssNames(        LWPanelID panel, int item, int col );
int    AssColumnWidths( LWPanelID panel, int col           );

// Popup Callbacks
char * ObjectPopup_Names( void * data, int index );
int    ObjectPopup_Count( void * data );

char * MappingPopup_Names( void * data, int index );
int    MappingPopup_Count( void * data );

// Draw Callbacks
void DrawPanel( LWPanelID panel, void *data, DrMode mode);
void DrawProgressField( LWControl *con, void *data, DrMode mode );

// Key Callbacks
void HandleKeys(   LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key );

// Event Handlers
void HandleProjectMappingFReq( LWControl *con, void *data );
void HandleSetup(              LWControl *con, void *data );
void HandleMappingName(        LWControl *con, void *data );
void HandleMappingPopup(       LWControl *con, void *data );

void HandleSynonymList(        LWControl *con, void *data );
void HandleSynonymAdd(         LWControl *con, void *data );
void HandleSynonymRemove(      LWControl *con, void *data );

void HandleEndomorphReload(    LWControl *con, void *data );
void HandleVisemeReload(       LWControl *con, void *data );

void HandleLink(               LWControl *con, void *data );
void HandleUnlink(             LWControl *con, void *data );
void HandleFindLink(           LWControl *con, void *data );
void HandleHideLinked(         LWControl *con, void *data );
void HandleClearLinks(         LWControl *con, void *data );
void HandleTestLinks(          LWControl *con, void *data );

void HandleEndomorphList(      LWControl *con, void *data );
void HandleVisemeList(         LWControl *con, void *data );
void HandleAssignedList(       LWControl *con, void *data );

void HandleSaveProjectMapping( LWControl *con, void *data );

void HandleKeys(               LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(             LWPanelID panel, void *data, LWDualKey key );

void HandleAbout( LWControl *con, void *data );

// Misc. Callbacks
int ScanPointsForEndomorphs( void *_data, LWPntID point );

int (__cdecl SortVisemesAlpha)( const void *arg1, const void *arg2 );
int (__cdecl SortTargetsAlpha)( const void *arg1, const void *arg2 );
