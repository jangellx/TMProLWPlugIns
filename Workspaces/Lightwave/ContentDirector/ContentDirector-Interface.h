//
// ContentDirector_Interface.h
//

#include <portable/dynarray/dynarray.h>

#include "ContentDirector.h"
#include "ContentDirector-Entry.h"

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>

// ContentDirector_Interface
class ContentDirector_Interface {
public:
  ContentDirector_Interface( GlobalFunc *_global, void *local );
  ~ContentDirector_Interface();

  bool CreateUI();
  int  OpenUI();

public:
  void SetStatus( const char *string, bool is_ok );
  bool SaveSettings( const char *local_path, char *remote_path );
  bool LoadSettings( const char *path, bool recursed = false );

  void CreateCreatePanel();
  void SetCreateStatus( const char *string, float progress );

  void InitEnvVarList();

  // Activation Data
  GlobalFunc      &global;
  LWLayoutGeneric *generic;

  // Layout Globals
  LWMessageFuncs  *message;
  LWSceneInfo     *sceneinfo;
  LWDirInfoFunc   *dirinfo;

  LWPanelFuncs    *panel_funcs;

  // Main UI
  LWPanelID        panel;

  LWControl       *current_dir_comment;
  LWControl       *current_dir_path;
  LWControl       *current_dir_select;

  LWControl       *dir_list;

  LWControl       *add_button;
  LWControl       *add_current_button;
  LWControl       *duplicate_button;
  LWControl       *remove_button;

  LWControl       *create_button;

  LWControl       *shift_up_button;
  LWControl       *shift_down_button;

  LWControl       *edit_button;
  LWControl       *about_button;

  LWControl       *path_string;
  LWControl       *default_dir_toggle;
  LWControl       *comment_string;
  LWControl       *test_path_button;

  LWControl       *path_from_popup;
  LWControl       *env_string;
  LWControl       *env_popup;

  LWControl       *config_from_popup;
  LWControl       *config_filebutton;
  LWControl       *config_reload;

  LWControl       *status_field;
  LWControl       *change_content_button;

  // Create Panel
  LWPanelID        create_panel;

  LWControl       *create_base_popup;
  LWControl       *create_template_freq;
  LWControl       *create_save_example;

  LWControl       *create_add_button;
  LWControl       *create_root_freq;

  LWControl       *create_status;
  LWControl       *create_now;

  const char      *checkmark_string;

  // Data
  DynArray< ContentDirector_Entry * >   entries;
  int                                   cur_entry;
  int                                   cur_content_entry;
  int                                   default_entry;

  char                                * status_string;
  bool                                  status_ok;

  char                                * create_status_string;
  float                                 create_progress;

  DynArray< char * >                    env_vars;
};

// Draw Callbacks
void DrawPanel(             LWPanelID   panel, void *data, DrMode mode );
void DrawStatusField(       LWControl * con,   void *data, DrMode mode );

void DrawCreatePanel(       LWPanelID   panel, void *data, DrMode mode );
void DrawCreateStatusField( LWControl * con,   void *data, DrMode mode );

// Event Handlers
void HandleEditButton(          LWControl *con, void *data );
void HandleAboutButton(         LWControl *con, void *data );
void HandleSelectCurrentButton( LWControl *con, void *data );

void HandleDirList(          LWControl *con, void *data );

void HandleAddButton(        LWControl *con, void *data );
void HandleAddCurrentButton( LWControl *con, void *data );
void HandleDuplicateButton(  LWControl *con, void *data );
void HandleRemoveButton(     LWControl *con, void *data );

void HandleCreateButton(     LWControl *con, void *data );

void HandleShiftUpButton(    LWControl *con, void *data );
void HandleShiftDownButton(  LWControl *con, void *data );

void HandleCommentString(    LWControl *con, void *data );
void HandleDefaultDirToggle( LWControl *con, void *data );
void HandlePathString(       LWControl *con, void *data );
void HandleTestPathButton(   LWControl *con, void *data );

void HandlePathFromPopup(    LWControl *con, void *data );
void HandleEnvString(        LWControl *con, void *data );
void HandleEnvPopup(         LWControl *con, void *data );

void HandleConfigFromPopup(  LWControl *con, void *data );
void HandleConfigFileButton( LWControl *con, void *data );
void HandleReloadButton(     LWControl *con, void *data );

void HandleChangeContentDirButton( LWControl *con, void *data );

void HandleKeys(             LWPanelID panel, void *data, LWDualKey key );
void HandleKeyUps(           LWPanelID panel, void *data, LWDualKey key );

// Create Panel Handlers
void HandleCreatePanelOpen( LWPanelID pan, void *data );

void HandleCreateBasePopup(  LWControl *con, void *data );
void HandleCreateNow(        LWControl *con, void *data );
void HandleSaveExample(      LWControl *con, void *data );

bool ReadTemplate( const char *template_path, DynArray< char * > &subdirs );

// List Callbacks
int    DirListColumnWidths( LWPanelID panel, int col );
int    DirListCount( LWPanelID panel );
char * DirListNames( LWPanelID panel, int item, int col );

// Popup Callbacks
int    EnvCount( void *userdata );
char * EnvName( void *userdata, int index );

