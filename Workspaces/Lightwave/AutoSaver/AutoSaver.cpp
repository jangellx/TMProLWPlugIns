/*  AutoSave
    
    Jim Hofman

    Auto save LW files

*/

#define   NULL 0
#define   MAXPATHLEN 256

#include	<stdio.h>
#include	<stdlib.h>
#include  <string.h>
#include  <time.h>
#include <fstream.h>

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>
#include  <lwmaster.h>
#include  <lwgeneric.h>
#include  <lwpanel.h>
#include  <lwdisplay.h>
#include  <direct.h>
#include  "AutoSaver.h"
#include  "ASLogo.h"
#include  "doh.h"
#include  <mmsystem.h>


extern "C"
{
  static int Activate(long version, GlobalFunc *g, void *_local, void *data);
  // "void *_local" replaces "LWMasterHandler *local"
  // in the fucntion
  // LWMasterHandler *local = (LWMasterHandler *)_local;  will be added as the first line
  static int Interface(long version, GlobalFunc *global, void *inst, void *serverData);
	// used with generic call
	static int Generic_Activate(long version, GlobalFunc *g, void *_local, void *serverData);

  ServerRecord ServerDesc[] = {
    {LWMASTER_HCLASS, "AutoSaver", Activate},
    {LWMASTER_ICLASS, "AutoSaver", Interface},
		{LWLAYOUTGENERIC_CLASS, "AutoSaver", Generic_Activate},
    {NULL}
  };

}


GlobalFunc *global;
LWMessageFuncs  *message;
HostDisplayInfo   *hdi;
LWPanelFuncs      *panf;
LWPanelFuncs      *panf1;
LWRasterFuncs     *rastf;
LWDirInfoFunc     *dirInfo;


LWPanelID         panel;
LWPanelID         about_panel;
LWPanelID					help_panel;
LWPanelID         notify_panel;

//LWPanControlDesc  desc;


typedef int CommandFunc(const char *);


const char    *describe(LWInstance);
LWInstance    create(void *, void *, LWError *);
void          destroy(LWInstance);
LWError       copy(LWInstance, LWInstance);
LWError       load(LWInstance, const LWLoadState *);
LWError       save(LWInstance, const LWSaveState *);
double        process(LWInstance, const LWMasterAccess *);
unsigned int  flags(LWInstance);
int           firstCommand = 1;  
//int           useHalfTime = 0;
time_t        startTime, currentTime, previousTime;
bool          already_done = false;
bool          instance_exists = false;


int Generic_Activate(long version, GlobalFunc *g, void *_local, void *data)
{

  if (version != LWLAYOUTGENERIC_VERSION)
    return(AFUNC_BADVERSION);


//	if (!instance_exists) {
		LWLayoutGeneric *generic  = (LWLayoutGeneric *)_local;
		LWItemInfo      *iteminfo = (LWItemInfo      *)g( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );

  const char *server_name;
  char buffer[ 256 ];
	 // Look for AutoSaver
  server_name = (const char *)-1;
  for( unsigned long i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( NULL, LWMASTER_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "AutoSaver" ) == 0 ) {
      break;
    }
  }

  // Apply AutoSaver if it isn't already there
  if( server_name == NULL ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)_local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWMASTER_HCLASS, "AutoSaver" );
    (*generic->evaluate)( generic->data, buffer );
  }


  // Open it's UI
  sprintf( buffer, "EditServer %s %d", LWMASTER_HCLASS, i );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}



int Activate(long version, GlobalFunc *g, void *_local, void *data)
{

	hdi = (HostDisplayInfo *) g( LWHOSTDISPLAYINFO_GLOBAL, GFUSE_TRANSIENT );

	if (!hdi) return 1;


  LWMasterHandler *local = (LWMasterHandler *)_local;

  if (version != LWMASTER_VERSION)
    return(AFUNC_BADVERSION);

  if (local->inst)  {   
    local->inst->create   = create;
    local->inst->destroy  = destroy;
    local->inst->load     = load;
    local->inst->save     = save;
    local->inst->copy     = copy;
    local->inst->descln   = describe;
  }

  if (local->item) {
    local->item->useItems = NULL;
    local->item->changeID = NULL;
  }

  local->event  = process;
  local->flags  = flags;

  global = g;

  message = (LWMessageFuncs *) (*global) (LWMESSAGEFUNCS_GLOBAL, GFUSE_ACQUIRE);

	instance_exists = true;

  LoadDefaults();

  return(AFUNC_OK);

}

unsigned int flags(LWInstance inst) {
  return(0);
}

const char * describe(LWInstance inst) {
  return("AutoSaver");
}

LWInstance create(void *priv, void *context, LWError *err) {
  MCData  *myData;

  XCALL_INIT;

	instance_count++;

  myData = (MCData *) malloc(sizeof(MCData));
  memset(myData,0,sizeof(MCData));   

  return(myData);

}


void destroy(LWInstance inst) {
  MCData  *myData;
  Command *t;

  XCALL_INIT;

	instance_count --;
//  message = (LWMessageFuncs *) (*global) (LWMESSAGEFUNCS_GLOBAL, GFUSE_ACQUIRE);
  (*global)(LWMESSAGEFUNCS_GLOBAL, GFUSE_RELEASE);

//	(LWHOSTDISPLAYINFO_GLOBAL, GFUSE_RELEASE );


  myData = (MCData *) inst;

  while(myData->head) {
    t= myData->head->next;
    if(myData->head->cs)
      free(myData->head->cs);
    if(myData->head->ls)
      free(myData->head->ls);
    free(myData->head);
    myData->head = t;
  }

  free(myData);
}


LWError copy(LWInstance d1,LWInstance d2) {
  XCALL_INIT;
  return(NULL);
}


LWError load(LWInstance inst,const LWLoadState *lState) {
  XCALL_INIT;
  LWID id;

	if(instance_count > 1) 
		return(NULL);

  if(id = LWLOAD_FIND(lState, outerBlock)) {
    while(id = LWLOAD_FIND(lState,idVars )) {
      switch(id) {
        case ID_PATH:
          LWLOAD_STR(lState, (char *) &AutoSavePath, 256);
          break;
        case ID_INTV:
          LWLOAD_I4(lState, &interfaceSaveInterval, 1);
          break;
        case ID_SASF:
          LWLOAD_U2(lState, &AutoSave_state, 1);
          break;
        case ID_SCSF:
          LWLOAD_U2(lState, &save_curr_scn_state, 1);
          break;
        case ID_SUDF:
		      LWLOAD_U2(lState, &user_sel_scn_state, 1);
          break;
        case ID_DSJR:
          LWLOAD_U2(lState, &just_remind_state, 1);
          break;
        case ID_NOTO:
          LWLOAD_U2(lState, &notify_options, 1);
          break;
        case ID_WAVE:
          LWLOAD_STR(lState, (char *) &WaveFilePath, 256);
          break;
        case ID_CONF:
          LWLOAD_U2(lState, &confirm_flag, 1);
          break;
        case ID_IRPS:
          LWLOAD_U2(lState, &interrupt_sensitivity, 1);
          break;
      }
      LWLOAD_END(lState);
    }
    LWLOAD_END(lState);
  }
     
  return(NULL);
}



LWError save(LWInstance inst,const LWSaveState *sState) {
  XCALL_INIT;

	if(instance_count > 1) 
		return(NULL);

	LWSAVE_BEGIN(sState, &outerBlock[0],0);

		LWSAVE_BEGIN(sState, &idVars[0] ,1);
			LWSAVE_STR(sState, (const char *) &AutoSavePath);  // AutoSaved file path
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[1] ,1);
			LWSAVE_I4(sState,  &interfaceSaveInterval,1);   // save interval in minutes
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[2] ,1);
			LWSAVE_U2(sState,  &AutoSave_state, 1);  // AutoSave scene boolean
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[3] ,1);
			LWSAVE_U2(sState,  &save_curr_scn_state, 1);  // Save Current scene boolean
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[4] ,1);
			LWSAVE_U2(sState,  &user_sel_scn_state, 1);  // User defined scene boolean
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[5] ,1);
			LWSAVE_U2(sState,  &just_remind_state, 1);  // Don't save just remind
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[6] ,1);
			LWSAVE_U2(sState,  &notify_options, 1);  // Don't save just remind
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[7] ,1);
			LWSAVE_STR(sState,  (const char *) &WaveFilePath);  // Wave file path
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[8] ,1);
			LWSAVE_U2(sState,  &confirm_flag, 1);  // Confirm before saving/notifying
		LWSAVE_END(sState);

		LWSAVE_BEGIN(sState, &idVars[9] ,1);
			LWSAVE_U2(sState,  &interrupt_sensitivity, 1);  // User defined scene boolean
		LWSAVE_END(sState);


	LWSAVE_END(sState);




//  (*message->error)("wrote Save current scene.","should be one.");
//  char buffer[128];
//  _itoa( save_curr_scn_state, buffer, 10 );
// (*message->info)("Wrote saveInterval:", buffer);
// (*message->info)("Wrote AutoSavePath:", AutoSavePath);



  return(NULL);
}

double process(LWInstance inst, const LWMasterAccess *ma) {
  MCData    *myData;
  char      *command;
  double    elapsedTime, commandInterval;
	char *		waveFile;

  XCALL_INIT;

	if (!hdi) return(double(0.0));
  
	myData = (MCData *) inst;
	command = (char *) ma->eventData;


  // Turn off if you came via "LoadFromScene"
	if((!strnicmp("LoadFromScene",command,13)) && (instance_count == 1) && (already_done == false)) {
		removeAutoSaver(ma,1);
		return((double)0.0);
	}

	// Check for alternate instances of AutoSaver
// char buffer[128];
// _itoa( instance_count, buffer, 10 );
// (*message->info)("instance_count:", buffer);
	if (instance_count > 1) {
		removeAutoSaver(ma,2); 
		already_done = true;
		return((double)0.0);
	}



	// check for save scene command then restart counter
	if(!strnicmp("SaveScene",command,9)) {
		firstCommand = 1;
	}
  
  if (firstCommand == 1) {
    time(&startTime);
    time(&currentTime);
    firstCommand = 0;
//    (*message->error)("Initialize firstCommand.","Good.");
    return((double)0.0);
  }

  previousTime = currentTime;
  time(&currentTime);

  commandInterval = difftime(currentTime, previousTime);

  if (commandInterval <= interrupt_sensitivity)    // too busy do not interrupt
    return((double)0.0);

  if (commandInterval < saveInterval) { // no activity in save interval alert in 1/2 interval
    firstCommand = 0;
//    useHalfTime = 1;
  }

  elapsedTime = difftime(currentTime, startTime);
int conf;
  if (elapsedTime >= saveInterval) {
		
		firstCommand = 1;   //Set to true to reset time next time around
    if (!just_remind_state) {
      switch(notify_options) {
        case 0:       // do not notify
          writeFile(ma);
          break;
        case 1:      // ding!
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
					}
          break;
				case 2:     // wave file
          PlaySound(WaveFilePath,NULL,SND_SYNC);
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
          }
          break;
        case 3:     // message
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
            (*message->info) ("AutoSaver: Saving scene file to.",AutoSavePath);
						writeFile(ma);
          }
          break;
        case 4:     // do'h!
          NotifyUser();
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
          }
          break;
        case 5:      //   ding and message
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
          (*message->info) ("AutoSaver: Saving scene file to.",AutoSavePath);
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
					}
          break;
        case 6:      //   Wave and Message
          PlaySound(WaveFilePath,NULL,SND_SYNC);
          (*message->info) ("AutoSaver: Saving scene file to.",AutoSavePath);
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
					}
          break;
        case 7:     // Ding! and Do'h
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
          NotifyUser();
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
          }
          break;
        case 8:     //  Wave and Do'h
          PlaySound(WaveFilePath,NULL,SND_SYNC);
          NotifyUser();
					if(confirm_flag) {
						conf = (*message->yesNo) ("AutoSaver Confirm"," Confirm save to:",AutoSavePath);
					  if(conf) writeFile(ma);
					} 
					else  {
						writeFile(ma);
          }
          break;
        break;
      }

    }
    else {
      switch(notify_options) {
        case 0:
          (*message->info) ("AutoSaver is enabled, but doing nothing",
                            "You probably want to disable it.");
          break;
        case 1:       // fake out sndPlaySound to play the default sound
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
          break;
				case 2:      // play wave file
 //         (*message->info) ("Option:Wave no save.",buffer);
          PlaySound(WaveFilePath,NULL,SND_SYNC);
          break;
        case 3:
          (*message->info) ("AutoSaver Reminder:","Time to save scene file.");
          break;
        case 4:
          NotifyUser();
          break;
        case 5:       // ding and message
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
          (*message->info) ("AutoSaver Reminder:","Time to save scene file.");
          break;
        case 6:       // wave and message
          PlaySound(WaveFilePath,NULL,SND_SYNC);
          (*message->info) ("AutoSaver Reminder:","Time to save scene file.");
          break;
        case 7:       // ding and Do'h
          waveFile = "C:\\Winnt\\noName";
          sndPlaySound(waveFile,SND_SYNC);
          NotifyUser();
          break;
        case 8:       // wave and Do'h
          PlaySound(WaveFilePath,NULL,SND_SYNC);
          NotifyUser();
          break;
        break;
      }
    } 
  }
  else {

  }
 
  return((double)0.0);

}


void writeFile (const LWMasterAccess *ma) 
{
      ma->evaluate(ma->data,"AutoConfirm -1");
      strcpy(buffer, "SaveSceneCopy ");
      strcat(buffer,AutoSavePath);
      ma->evaluate(ma->data,buffer);
      time(&startTime);
      ma->evaluate(ma->data,"AutoConfirm 0");
}


void removeAutoSaver(const LWMasterAccess *ma, int type) 
{
	LWItemInfo * iteminfo = (LWItemInfo *) global(LWITEMINFO_GLOBAL,GFUSE_TRANSIENT);
	const char *server_name;
	bool first_found = false;
 //   char buffer[ 256 ];

      // Find duplicate instances of AutoSaver
	server_name = "-1";
//   (*message->info) ("Server_Name.",server_name);

  for( unsigned long i=1; server_name != NULL; i++ ) {
    server_name = (*iteminfo->server)( NULL, LWMASTER_HCLASS, i );
    if( server_name == NULL )
      break;

    if( stricmp( server_name, "AutoSaver" ) == 0 ) {
      if(( first_found == false ) && (type == 2)){
        first_found = true;
      } else {
//        (*message->info) ("Removing alternate.","AutoSaver");
				sprintf( buffer, "RemoveServer %s %i", LWMASTER_HCLASS, i );
				(*ma->evaluate)( ma->data, buffer );
				i--;
//				(*message->info) ("Multiple Remove","");
      }
    }
  }
}


int Interface(long version, GlobalFunc *global, void *inst, void *serverData)
{
  MCData      *mydata;
  const char  *tmp;
  LWSceneInfo *scnInfo;

//  (*message->error)("AutoSaver: ok #1","ok");

  XCALL_INIT;

  mydata = (MCData *) inst;

  if ((tmp = getenv("TEMP")) == NULL)
  {
    if((tmp = getenv("TMP")) == NULL)
      tmp = "";
  }


  scnInfo = (LWSceneInfo *) global("LW Scene Info", GFUSE_TRANSIENT);
  strcpy(sceneName, scnInfo->filename); 
	strcpy(sceneFileName, scnInfo->name); 
  const char *NotifyOptions[][14]={"Do not notify ","    Ding!    ",
                                   "Play wave file","   Message    ","     Do'h!    ",
                                   "Message & Ding","Message & Wave","  Do'h & Ding ",
                                   "  Do'h & Wave "};

	Get_Scene_Dir();

  char edit[80]  = "Save Interval (min.)";

	// get the panel function ready
	panf = (LWPanelFuncs *) global(LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT);
	if (!panf) {
    (*message->error)("AutoSaver: Unable to activate global "LWPANELFUNCS_GLOBAL,"	you probably need to add the lwpanels.p plug-in.");
		return AFUNC_BADGLOBAL;
	}

  if (!(CreateAboutPanel()))
	  (*message->error)("AutoSaver: Unable to create about panel","Sorry.");

  if (!(CreateHelpPanel()))
	  (*message->error)("AutoSaver: Unable to create help panel","Sorry.");
 
  panel = PAN_CREATE(panf, "AutoSaver");
  if( !panel ) {  
		(*message->error)("AutoSaver: Unable to create the panel"," Sorry");
		return AFUNC_BADGLOBAL;
  }

  DrawFuncs *df = panf->drawFuncs;

  try	{

    if( !(About = WBUTTON_CTL(panf, panel," About ",70)))
			throw false;
    if( !(Help = WBUTTON_CTL(panf, panel," Help ",70)))
			throw false;
   if (!(Save_Settings_As_Default = WBUTTON_CTL(panf,panel,"Save Settings", 80)))
			throw false;
		if( !(Save_Interval = INT_CTL(panf,panel,"Save Interval (minutes)")))
			throw false;
    if( !(Save_AutoSaver_File = BOOL_CTL(panf,panel,"AutoSave backup file (AutoSaved.lws)")))
      throw false;
    if( !(Save_Scene_File = BOOL_CTL(panf,panel,"AutoSave current scene file")))
      throw false;
    if( !(Just_Remind_Me = BOOL_CTL(panf,panel,"Don't save, just remind me")))
      throw false;  
    if( !(Save_User_Defined_File = FILEBUTTON_CTL(panf,panel,"User defined file. ", 100)))
      throw false;
    if (!(Scene_File = STR_CTL(panf,panel,"Auto Saving to:  ",58)))
			throw false;
    if( !(Notify_Options = POPUP_CTL(panf, panel, "Notify Options ", (const char **) NotifyOptions)))
			throw false;
    if( !(Load_Wave_File = FILEBUTTON_CTL(panf, panel, "Load Wave File", 100)))
			throw false;
    if (!(Wave_File = STR_CTL(panf,panel,"Current wave file:",58)))
			throw false;
//  if (!(Bottom_Filler = BOOL_CTL(panf,panel,"")))
//			throw false;
    if (!(Confirm = BOOL_CTL(panf,panel,"Confirm save")))
			throw false;
    if (!(Test_Notify = WBUTTON_CTL(panf,panel,"Test Notify Selection", 110)))
			throw false;
    if (!(Interrupt_Sensitivity = SLIDER_CTL(panf,panel,"Min. command interval", 50,1,10)))
			throw false;
	  if( !( Logo_Area = CANVAS_CTL(panf, panel,"", ASLOGOWIDTH, ASLOGOHEIGHT)))
      throw false;

    SET_all();

//    (*message->info)("test",saveUserDefinedFile);

	} catch (bool a) {
		if (!a) {
			(*message->error)("AutoSaver: Error creating interface contols","sorry");
			return 0;
		}

	}

    // place controls

	MOVE_CON(Save_Interval,10,10);
	MOVE_CON(About, 330,10);
	MOVE_CON(Help, 250,10);
  MOVE_CON(Save_Settings_As_Default, 410,10);
	MOVE_CON(Save_AutoSaver_File,10,50);
	MOVE_CON(Save_Scene_File,10,80);
	MOVE_CON(Just_Remind_Me,10,110);
	MOVE_CON(Confirm, 10,140);
	MOVE_CON(Save_User_Defined_File,392,180);
	MOVE_CON(Scene_File,10,180);
	MOVE_CON(Notify_Options,10,220);
  MOVE_CON(Test_Notify,250,220);
	MOVE_CON(Load_Wave_File,392,250);
	MOVE_CON(Wave_File,10,250);	
	MOVE_CON(Logo_Area,251,45);
  MOVE_CON(Interrupt_Sensitivity,235,140);

  		// Resize Panel
	PAN_SETW(panf, panel, 400);
	PAN_SETH(panf, panel, 300);
  
  // Callbacks
  CON_SETEVENT(About, handle_about, NULL);
  CON_SETEVENT(Help, handle_help, NULL);
	CON_SETEVENT(Save_Interval, handle_save_interval, NULL);
  CON_SETEVENT(Save_User_Defined_File, handle_save_user_defined_file, NULL);
  CON_SETEVENT(Save_AutoSaver_File,handle_save_autosaver_file, NULL);
  CON_SETEVENT(Save_Scene_File,handle_save_scene_file, NULL);
  CON_SETEVENT(Just_Remind_Me,handle_just_remind_me, NULL);
	CON_SETEVENT(Notify_Options,handle_notify_options, NULL);
  CON_SETEVENT(Load_Wave_File,handle_load_wave_file, NULL);
	CON_SETEVENT(Confirm,handle_confirm, NULL);
  CON_SETEVENT(Test_Notify, handle_test_notify, NULL);
  CON_SETEVENT(Interrupt_Sensitivity,handle_interrupt_sensitivity,NULL);
  CON_SETEVENT(Save_Settings_As_Default,handle_save_settings_as_default,NULL);



//  ERASE_CON(Bottom_Filler);

  if((notify_options == 2) || (notify_options == 6) || (notify_options == 8)){
    RENDER_CON(Load_Wave_File);
    RENDER_CON(Wave_File); 
  } 
  else   {
    ERASE_CON(Load_Wave_File);
    ERASE_CON(Wave_File); 
  } 


  // Set the Draw Function
  (*panf->set)( panel, PAN_USERDRAW,DrawPanel);
int i;
  i = panf->open(panel, PANF_BLOCKING);
 

	GET_all();


//    GET_INT(Save_Interval,saveInterval);
  PAN_KILL(panf,panel);

    return(AFUNC_OK);
}


void DrawPanel( LWPanelID panel, void *data, DrMode mode) 
{
	// draw logo
  int i,j,x,y; 
	int index = 0;
  LWRasterID ras;
  rastf = (LWRasterFuncs *) global(LWRASTERFUNCS_GLOBAL, GFUSE_TRANSIENT); 
  ras = (LWRasterID *) rastf->create( ASLOGOWIDTH, ASLOGOHEIGHT, 0 );
//  if ( !ras ) return NULL;
	y = 0;
  for(j=0;j<ASLOGOHEIGHT;j++) {
    x = ASLOGOWIDTH;
    for(i=0;i<ASLOGOWIDTH;i++) {
    rastf->drawRGBPixel(ras,aslogo_data[index],aslogo_data[index+1],
												aslogo_data[index+2],x--,y);
    index += 3;
    }
	  y++;
  }		
    rastf->blitPanel(ras,0,0,panel,250,49,ASLOGOWIDTH,ASLOGOHEIGHT);


}

void LoadDefaults ()
{
 	char tBuff[256];
	sprintf(tBuff,"");  // initialize temp buffer

  dirInfo = (LWDirInfoFunc *) global (LWDIRINFOFUNC_GLOBAL, GFUSE_TRANSIENT);
  if (dirInfo) {
    defaultPath = (char *) dirInfo("Settings");
    if (defaultPath)
      if (strlen(defaultPath) <= MAXPATHLEN) {
        strncat(defaultPath,"\\AutoSaver.cfg",14);
        (*message->info)("Default directory:", defaultPath);
      }
  }

}



static void SET_all() 
{

  SET_STR(Scene_File,AutoSavePath,strlen(AutoSavePath));
  SET_INT(Save_Interval, interfaceSaveInterval);
  SET_INT(Save_AutoSaver_File, AutoSave_state);
  SET_INT(Save_Scene_File, save_curr_scn_state); 
	SET_INT(Just_Remind_Me, just_remind_state);
  SET_STR(Save_User_Defined_File,saveUserDefinedFile,strlen(saveUserDefinedFile));
  SET_STR(Load_Wave_File,WaveFilePath,strlen(WaveFilePath));
  SET_INT(Notify_Options, notify_options);
  SET_STR(Wave_File,WaveFilePath,strlen(WaveFilePath));
	SET_INT(Confirm,confirm_flag);
  SET_INT(Interrupt_Sensitivity,interrupt_sensitivity);

}

static void GET_all() {

  GET_STR(Scene_File,AutoSavePath,strlen(AutoSavePath));
  GET_INT(Save_Interval, interfaceSaveInterval);
  GET_INT(Save_AutoSaver_File, AutoSave_state);
  GET_INT(Save_Scene_File, save_curr_scn_state); 
  GET_INT(Just_Remind_Me, just_remind_state);
  GET_STR(Save_User_Defined_File,saveUserDefinedFile,strlen(saveUserDefinedFile));
  GET_STR(Load_Wave_File,WaveFilePath,strlen(WaveFilePath));
  GET_INT(Notify_Options, notify_options);
	GET_STR(Wave_File,WaveFilePath,strlen(WaveFilePath));
	GET_INT(Confirm,confirm_flag);
  SET_INT(Interrupt_Sensitivity,interrupt_sensitivity);
}


void Get_Scene_Dir() {	
	
	char tBuff[256];
	sprintf(tBuff,"");  // initialize temp buffer

    // Get the current working directory: 

	// Use the scene_name variable to get the rest of the scene path, 
	// then strip the filename
	strncat(tBuff,sceneName,(strlen(sceneName)-strlen(sceneFileName)));
	strcpy(scene_dir,tBuff);

//	(*message->info)("new scene dir", scene_dir);
}


bool NotifyUser() {

  panf1 = (LWPanelFuncs *) global(LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT);

  notify_panel = PAN_CREATE(panf1, "Do'h!");

	if( !notify_panel ) {  
		(*message->error)("AutoSaver: Unable to create the notify panel"," Sorry");
		return false;
	}

  	// About Box Area
	if( !( Notify_Area = CANVAS_CTL( panf1, notify_panel,"", DOHWIDTH+1, DOHHEIGHT+1 ) ) ) {
		(*message->error)("AutoSaver: Unable to create the interface controls"," Sorry");
		return false;
  }

  PAN_SETW(panf1, notify_panel, DOHWIDTH + 1 );
  PAN_SETH(panf1, notify_panel, DOHHEIGHT + 1 );

  ival.intv.value = (int)&Draw_Notify;
  (*Notify_Area->set)( Notify_Area, CTL_USERDRAW, &ival );

  MOVE_CON(Notify_Area, (CON_X(Notify_Area)),
                        (CON_Y(Notify_Area)));

  if(panf1->open(notify_panel, PANF_BLOCKING))


  PAN_KILL(panf1,notify_panel);

    return(AFUNC_OK);

  return true;

}


static void Draw_Notify(LWControl *ctl, void * myData, DrMode mode) {

  LWRasterID ras;

  rastf = (LWRasterFuncs *) global(LWRASTERFUNCS_GLOBAL, GFUSE_TRANSIENT);

  
  ras = (LWRasterID *) rastf->create( DOHWIDTH, DOHHEIGHT, 0 );
//  if ( !ras ) return NULL;


  DrawFuncs *df = panf1->drawFuncs;
  int x1 = CON_X(ctl);
  int y1 = CON_Y(ctl);
  int w = CON_W(ctl);
  int h = CON_H(ctl);


  int ph = PAN_GETH( panf1, notify_panel );
  int pw = PAN_GETW( panf1, notify_panel );

  int i,j,x,y,index;

  index = 0;
	y = 0;

  for(j=0;j<DOHHEIGHT;j++) {
    x = DOHWIDTH;
    for(i=0;i<DOHWIDTH;i++) {
      rastf->drawRGBPixel(ras,doh_data[index],doh_data[index+1],
      doh_data[index+2],x--,y);
      index += 3;
    }
	y++;
  }

  rastf->drawText(ras,"Time to Save.",6,50,185); 
  rastf->drawText(ras,"Don't Let this happen to you.",6,20,200); 
		
  rastf->blitPanel(ras,0,0,notify_panel,4,6,w,h);


}


bool CreateAboutPanel() {

  about_panel = PAN_CREATE(panf, "AutoSaver About");

	if( !about_panel ) {  
		(*message->error)("AutoSaver: Unable to create the about panel"," Sorry");
		return false;
	}

	// About Box Area
	if( !( About_Area = CANVAS_CTL( panf, about_panel,"", 320, 160 ) ) ) {
		(*message->error)("AutoSaver: Unable to create the interface controls"," Sorry");
		return false;
  }

  PAN_SETW(panf, about_panel, 300 );
  PAN_SETH(panf, about_panel, 200 );

  ival.intv.value = (int)&Draw_About;
  (*About_Area->set)( About_Area, CTL_USERDRAW, &ival );

  MOVE_CON(About_Area, (CON_X(About_Area)) + 0,
                       (CON_Y(About_Area)));

  return true;
}


static void Draw_About(LWControl *ctl, void * myData, DrMode mode) {

  DrawFuncs *df = panf->drawFuncs;

  int x = CON_X(ctl);
  int y = CON_Y(ctl);
  int w = CON_W(ctl);
  int h = CON_H(ctl);

  int ph = PAN_GETH( panf, about_panel );
  int pw = PAN_GETW( panf, about_panel );

//  df->drawRGBBox(about_panel,100,100,100,x+14,y+6,319,158);
  df->drawRGBBox(about_panel,100,130,150,x,y+4,319,160);


//  sprintf( buffer, "%s V%.2f", about_text[0] );
  df->drawText( about_panel, about_text[0], 6,
                 (pw/2)-(df->textWidth(about_panel, about_text[0]))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->drawText(about_panel, about_text[i], 6,
             (pw/2)-(df->textWidth(about_panel, about_text[i] ))/2,
             y );
    y += 15;
  }

}


bool CreateHelpPanel() {

  help_panel = PAN_CREATE(panf, "AutoSaver Help");

	if( !help_panel ) {  
		(*message->error)("AutoSaver: Unable to create the help panel"," Sorry");
		return false;
	}

	// About Box Area
	if( !(Help_Area = CANVAS_CTL( panf, help_panel,"", 385, 470 ) ) ) {
		(*message->error)("AutoSaver: Unable to create the interface controls"," Sorry");
		return false;
  }

  PAN_SETW(panf, help_panel, 375 );
  PAN_SETH(panf, help_panel, 475 );

  ival.intv.value = (int)&Draw_Help;
  (*Help_Area->set)( Help_Area, CTL_USERDRAW, &ival );

  MOVE_CON(Help_Area, (CON_X(Help_Area)) ,
                      (CON_Y(Help_Area)));

  return true;
}


static void Draw_Help(LWControl *ctl, void * myData, DrMode mode) {

	LWRasterID raster;

//  rastf = (LWRasterFuncs *) global(LWRASTERFUNCS_GLOBAL, GFUSE_TRANSIENT);

  int x = CON_X(ctl);
  int y = CON_Y(ctl);
  int w = CON_W(ctl);
  int h = CON_H(ctl);

// int ph = PAN_GETH( panf, help_panel );
// int pw = PAN_GETW( panf, help_panel );

	raster = (LWRasterID *) rastf->create( w, h, 0 );

	rastf->drawRGBBox(raster,100,130,145,x,y,w,h);

	rastf->drawText(raster,"AutoSaver Help",6,150,5); 
	rastf->drawText(raster,"Save Interval - This is the approximate interval that AutoSave will",6,10,25);  
	rastf->drawText(raster,"save or notify.  AutoSaver is triggered by commands and there are 4 ",6,10,40);
	rastf->drawText(raster,"cases that will alter the interval: ",6,10,55);
	rastf->drawText(raster,"1.  High activity - AutoSaver waits for the 'Min. Command Interval'",6,30,70);
	rastf->drawText(raster,"    between commands before saving/notifying (see below).",6,30,85);
	rastf->drawText(raster,"2.  Low activity - AutoSaver will not save/notify if the time between",6,30,100);
	rastf->drawText(raster,"    commands is greater than the save interval (e.g you went to lunch.)",6,30,115);
	rastf->drawText(raster,"3.  User changes save interval - This resets the clock.",6,30,130);
	rastf->drawText(raster,"4.  User saves the scene - This also resets the clock.",6,30,145);
	rastf->drawText(raster,"AutoSave Options",6,10,160);
	rastf->drawText(raster,"1.  AutoSave backup file - This saves your file into AutoSaved.lws in",6,30,175);
	rastf->drawText(raster,"    the current scene directory.",6,30,190);
	rastf->drawText(raster,"2.  AutoSave current scene file - This saves (thus overwriting) your",6,30,205);
	rastf->drawText(raster,"    current scene file.",6,30,220);
	rastf->drawText(raster,"3.  Don't save, just notify",6,30,235);
	rastf->drawText(raster,"4.  User defined file - This pops up a file selection window from which",6,30,250);
	rastf->drawText(raster,"    you can chose the file that AutoSaver will save to. (e.g. ..tk1, tk2)",6,30,265);
  rastf->drawText(raster,"Confirm Save - A Yes/No message will pop up so you can save or not save.",6,10,280);
	rastf->drawText(raster,"Notify Options - allow you to chose what you want to see/hear.",6,10,295);
	rastf->drawText(raster,"1.   Do not notify - save without a cue.",6,30,310);
	rastf->drawText(raster,"2.   Ding! - The default system sound will ding.",6,30,325);
	rastf->drawText(raster,"3.   Play Wave File - Allow you to load a .wav file for your cue.",6,30,340);
	rastf->drawText(raster,"4.   Do'h! - Displays a corny Homer joke.",6,30,355);
  rastf->drawText(raster,"Test Notify Selection - previews notify selections.",6,10,370);
  rastf->drawText(raster,"Min. Command Interval - Allows you to adjust the sensitivity to commands.",6,10,385);
  rastf->drawText(raster,"AutoSaver will not save/notify until this number of seconds has elapsed",6,10,400);
  rastf->drawText(raster,"between commands.  The default of 2 seconds shoud be acceptable in most",6,10,415);
  rastf->drawText(raster,"cases. I have had to turn it up for such things as motion designer captures,",6,10,430);
  rastf->drawText(raster,"where many computations may occur between commands.",6,10,445);
	rastf->drawText(raster,"",6,30,470);

  rastf->blitPanel(raster,0,0,help_panel,0,0,w,h);

}

//  Callback functions
static void handle_about( LWControl *ctl, void *mydata) {
	// open about panel
	if (!panf->open(about_panel, PANF_BLOCKING))
		(*message->error)("AutoSaver: Could not open about panel", "");
	panf->close(about_panel);
}


static void handle_help( LWControl *ctl, void *mydata) {
	// open about panel
	if (!panf->open(help_panel, PANF_BLOCKING))
		(*message->error)("AutoSaver: Could not open help panel", "");
	panf->close(help_panel);
}


static void handle_save_interval(LWControl *ctl, void *mydata) {

	GET_INT(ctl, interfaceSaveInterval);
	saveInterval = interfaceSaveInterval * 60; 
//  saveInterval = interfaceSaveInterval; 
  firstCommand = 1;  // restart counter with the new time



}


static void handle_save_settings_as_default(LWControl *ctl, void *mydata) {

  (*message->info) ("Your current settings will be","saved as your default settings.");

   // Open AutoSaver.cfg
  ifstream in(defaultPath, ios::nocreate);
  if( !in ) {
    (*message->info) ("Could not find default file"," ");
    return;
  }
  do {
    in.getline(buffer, 256);             // Get a line from the file
    if( (in.bad() || in.fail()) && !in.eof() ) {
      (*message->error) ("Could not open default file", defaultPath);
    }
  } while( !in.eof() );

  (*message->info) ("Opened default file"," ");
  in.close();;

}


static void handle_save_autosaver_file(LWControl *ctl, void *mydata) {

	GET_INT(ctl, AutoSave_state);
 
  if(AutoSave_state == true) {

		char tBuff[256];
	  sprintf(tBuff,"");  // initialize temp buffer
    strcpy(AutoSavePath, scene_dir);
	  strcat(tBuff,"AutoSaved.lws"); 
	  strcat(AutoSavePath,tBuff);

    GHOST_CON(Just_Remind_Me);
    GHOST_CON(Save_Scene_File);
    GHOST_CON(Save_User_Defined_File);
		UNGHOST_CON(Confirm);

		save_curr_scn_state = false;
		user_sel_scn_state = false;
		just_remind_state = false;
  }
  else  {

    UNGHOST_CON(Just_Remind_Me);
    UNGHOST_CON(Save_Scene_File);
    UNGHOST_CON(Save_User_Defined_File);

  }
	SET_all();

}

static void handle_save_scene_file(LWControl *ctl, void *mydata) {

	GET_INT(ctl, save_curr_scn_state);

  if(save_curr_scn_state == true) {

		strcpy(AutoSavePath, sceneName);

    GHOST_CON(Just_Remind_Me);
    GHOST_CON(Save_AutoSaver_File);
    GHOST_CON(Save_User_Defined_File);
		UNGHOST_CON(Confirm);
   
		AutoSave_state = false;
		just_remind_state = false;
		user_sel_scn_state = false;
  }
  else  {
		UNGHOST_CON(Just_Remind_Me);
    UNGHOST_CON(Save_AutoSaver_File);
    UNGHOST_CON(Save_User_Defined_File);
  }
	SET_all();

}


static void handle_just_remind_me(LWControl *ctl, void *mydata) {
	
	GET_INT(ctl, just_remind_state);

// char buffer[128];
// _itoa( just_remind_state, buffer, 10 );
// (*message->info)("just remind state is", buffer);
 
  if(just_remind_state == true) {

		char tBuff[256];
	  sprintf(tBuff,"");  // initialize temp buffer
	  strcat(tBuff,"       Just Reminding. "); 
    strcpy(AutoSavePath, tBuff);

    GHOST_CON(Save_Scene_File);
    GHOST_CON(Save_AutoSaver_File);
    GHOST_CON(Save_User_Defined_File);
		GHOST_CON(Confirm);

		AutoSave_state = false;
		save_curr_scn_state = false;
		user_sel_scn_state = false;

  }
  else  { 
		UNGHOST_CON(Save_Scene_File);
    UNGHOST_CON(Save_AutoSaver_File);
    UNGHOST_CON(Save_User_Defined_File);
		UNGHOST_CON(Confirm);
  }
	SET_all();
}


static void handle_save_user_defined_file(LWControl *ctl, void *mydata) 
{
	char filename[256] = "";
	GET_STR( ctl, filename, 256 );
  if (filename[0] != '\0') {
    strcpy(AutoSavePath, filename); 
	  strcpy(saveUserDefinedFile, filename);
	  UNGHOST_CON(Confirm);
  }
  SET_all();
}


static void handle_notify_options(LWControl *ctl, void *mydata) 
{
  GET_INT( ctl, notify_options);
  if(notify_options == 2 || (notify_options == 6) || (notify_options == 8)) {
    RENDER_CON(Load_Wave_File);
    RENDER_CON(Wave_File); 
  }  
  else   {
    ERASE_CON(Load_Wave_File);
    ERASE_CON(Wave_File);  
  }


	SET_all();
}


static void handle_load_wave_file(LWControl *ctl, void *mydata) 
{
	char filename[256] = "";
	GET_STR( ctl, filename, 256 );
  if (filename[0] != '\0') {
    strcpy(WaveFilePath, filename);
    
  }
  SET_all();
}

static void handle_confirm(LWControl *ctl, void *mydata) 
{
	GET_INT( ctl, confirm_flag );
	SET_all();
}

static void handle_interrupt_sensitivity(LWControl *ctl, void *mydata)
{
  GET_INT(ctl, interrupt_sensitivity);
  SET_all();
}

static void handle_test_notify(LWControl *ctl, void *mydata) 
{
  char *		waveFile;
  switch(notify_options) {
    case 0:
      break;
    case 1:       // fake out sndPlaySound to play the default sound
      waveFile = "C:\\Winnt\\noName";
      sndPlaySound(waveFile,SND_SYNC);
      break;
	  case 2:      // play wave file
      PlaySound(WaveFilePath,NULL,SND_SYNC);
      break;
    case 3:
      (*message->info) ("AutoSaver Message","Message Only");
      break;
    case 4:
      NotifyUser();
      break;
    case 5:       //  ding and message
      waveFile = "C:\\Winnt\\noName";
      sndPlaySound(waveFile,SND_SYNC);
      (*message->info) ("AutoSaver Message","With Ding");
      break;
    case 6:       // wave and message
      PlaySound(WaveFilePath,NULL,SND_SYNC);
      (*message->info) ("AutoSaver Message","With Wave File");
      break;
    case 7:       //  ding and Do'h
      waveFile = "C:\\Winnt\\noName";
      sndPlaySound(waveFile,SND_SYNC);
      NotifyUser();
      break;
    case 8:       // wave and Do'h
      PlaySound(WaveFilePath,NULL,SND_SYNC);
      NotifyUser();
      break;
    break;
  }	
}
