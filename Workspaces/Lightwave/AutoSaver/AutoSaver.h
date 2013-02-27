//* AutoSave.h

// Define labels for each of the blocks to be saved in the
// Lightwave file.


#define ID_PATH LWID_('P','A','T','H')
#define ID_INTV LWID_('I','N','T','V')
#define ID_SASF LWID_('S','A','S','F')
#define ID_SCSF LWID_('S','C','S','F')
#define ID_SUDF LWID_('S','U','D','F')
#define ID_DSJR LWID_('D','S','J','R')
#define ID_NOTO LWID_('N','O','T','O')
#define ID_NULL LWID_('N','U','L','L')
#define ID_WAVE LWID_('W','A','V','E')
#define ID_CONF LWID_('C','O','N','F')
#define ID_IRPS LWID_('I','R','P','S')

// LW Block Identifiers for each variable to be saved and loaded

static LWBlockIdent idVars [] = {ID_PATH, "AutoSaveScenePath",
                                 ID_INTV, "SaveInterval",
                                 ID_SASF, "SaveAutoSaveFile",
                                 ID_SCSF, "SaveCurrentScene",
                                 ID_SUDF, "SaveUserDefinedFile",
                                 ID_DSJR, "DontSaveJustRemind",
                                 ID_NOTO, "NotifyOptions",
                                 ID_WAVE, "WaveFile",
                                 ID_CONF, "ConfirmFlag",
                                 ID_IRPS, "InterruptSensitivity",
                                 0
                                };

static LWBlockIdent outerBlock []  = {ID_NULL,"AutoSave",0 };

class  Command {
public:
  char  *cs;
  char  *ls;
  time_t  tick;
  Command  *next;   // linked list
}; 

class  MCData {
public:
  Command *head;
  Command *curr;
  time_t    lasttick;
}; 


char  sceneName[256];
char  sceneFileName[128];
char  scene_dir[256];
char  buffer[256];
char  *defaultPath;
char  saveUserDefinedFile[256];
char  waveFile[256];
char  loadWaveFile[256];
char  AutoSavePath[256];
char  WaveFilePath[256];
long  interfaceSaveInterval = 15;
long  saveInterval = 60 * interfaceSaveInterval;
unsigned short  save_curr_scn_state = 0;    // Initialize false for first instance
unsigned short  AutoSave_state = 0;        // Initialize false for first instance
unsigned short  user_sel_scn_state = 0;    // Initialize false for first instance
unsigned short  just_remind_state = 1;          // Initialize false for first instance
unsigned short  notify_options = 0;
unsigned short  confirm_flag = 1;
unsigned short  interrupt_sensitivity = 2;  // Minimum time between commands before save
int  loadedVars; 

int instance_count = 0;  // instance count of plugin, used to detect multiple instances


// Globals needed for controls macros 
static LWPanControlDesc desc;

static LWValue ival={LWT_INTEGER},ivecval={LWT_VINT},
			   fval={LWT_FLOAT},fvecval={LWT_VFLOAT},
			   sval={LWT_STRING};


LWControl * Scene_File;
LWControl * Scene_File_Name;
//LWControl * Destination_File_Name;
LWControl * Save_Scene_File;
LWControl * Save_AutoSaver_File;
LWControl * Just_Remind_Me;
LWControl * Notify_Options;
LWControl * Save_User_Defined_File;
LWControl * Save_Interval;
LWControl * About;
LWControl * About_Area;
LWControl * Notify_Area;
LWControl * Help;
LWControl * Help_Area;
LWControl * Help_Draw_Area;
LWControl * Load_Wave_File;
LWControl * Wave_File;
//LWControl * Bottom_Filler;
LWControl * Logo_Area;
LWControl * Confirm; 
LWControl * Test_Notify;
LWControl * Save_Settings_As_Default;
LWControl * Interrupt_Sensitivity;

static char *about_text[] = {
  ""
	"",
	"AutoSaver V1.1",
	"",
	""
  "written by Jim Hofman",
	"",
	"",
	"",
	"hofman@earthlink.net",
	NULL };


void	Get_Scene_Dir();
bool CreateAboutPanel();
bool CreateHelpPanel();
void LoadDefaults ();
static void Draw_About(LWControl *ctl, void * myData, DrMode mode);
static void Draw_Help(LWControl *ctl, void * myData, DrMode mode);
static void handle_save_interval(LWControl *ctl, void *mydata);
static void handle_about( LWControl *ctl, void *mydata);
static void handle_help( LWControl *ctl, void *mydata);
static void handle_save_user_defined_file(LWControl *ctl, void *mydata);
static void handle_save_scene_file(LWControl *ctl, void *mydata);
static void handle_save_autosaver_file(LWControl *ctl, void *mydata);
static void handle_just_remind_me(LWControl *ctl, void *mydata);
static void handle_save_user_defined_file(LWControl *ctl, void *mydata);
static void handle_notify_options(LWControl *ctl, void *mydata);
static void handle_load_wave_file(LWControl *ctl, void *mydata);
static void handle_test_notify(LWControl *ctl, void *mydata);
static void handle_confirm(LWControl *ctl, void *mydata);
static void handle_interrupt_sensitivity(LWControl *ctl, void *mydata);
static void handle_save_settings_as_default(LWControl *ctl, void *mydata);
static void SET_all();
static void GET_all();
bool NotifyUser();
static void Draw_Notify(LWControl *ctl, void * myData, DrMode mode);
void writeFile (const LWMasterAccess *ma);
void DrawPanel( LWPanelID panel, void *data, DrMode mode);
void removeAutoSaver(const LWMasterAccess *ma, int type); 