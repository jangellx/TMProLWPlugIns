//
// PanelToolsTest.cpp
//

#define NULL 0

#include "..\PanelToolsGlobal.h"

#include <stdio.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#include <lwgeneric.h>
#include <lwhost.h>

LWPanControlDesc desc;
LWValue ival    = {LWT_INTEGER};
LWValue ivecval = {LWT_VINT};
LWValue fval    = {LWT_FLOAT};
LWValue fvecval = {LWT_VFLOAT};
LWValue sval    = {LWT_STRING};

extern "C" {
  ServerUserName PanelToolsTest_Usernames[] = { {"TM-P  PanelTools Test" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "PanelTools Test)" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName PanelDrawTest_Usernames[]  = { {"TM-P  PanelDraw Test"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "PanelDraw Test)"  DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  XCALL_ (int) PanelToolsTest_Activate( long version, GlobalFunc *global, void *local, void *serverData );
  XCALL_ (int) PanelDrawTest_Activate(  long version, GlobalFunc *global, void *local, void *serverData );

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_PanelToolsTest" DEBUG_PLUGIN_PLUGNAME, PanelToolsTest_Activate, PanelToolsTest_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_PanelDrawTest"  DEBUG_PLUGIN_PLUGNAME, PanelDrawTest_Activate,  PanelDrawTest_Usernames  },
    { (const char *)NULL }
  };
}

// Prototypes
void DrawPanelToolsTest( LWPanelID panel, void *data, DrMode mode);

void HandleAlignLabels(               LWControl *con, void *data );
void HandleAlignLabelsArray(          LWControl *con, void *data );
void HandleAlignWith(                 LWControl *con, void *data );
void HandleAlignWithArray(            LWControl *con, void *data );
void HandleCenterUnder(               LWControl *con, void *data );
void HandleCenterUnderArray(          LWControl *con, void *data );
void HandlePutUnder(                  LWControl *con, void *data );
void HandlePutUnderArray(             LWControl *con, void *data );
void HandlePutOnLineWith(             LWControl *con, void *data );
void HandlePutOnLineWithArray(        LWControl *con, void *data );
void HandleRightAlign(                LWControl *con, void *data );
void HandleRightAlignArray(           LWControl *con, void *data );
void HandleRightAlignWithPanel(       LWControl *con, void *data );
void HandleRightAlignWithPanelArray(  LWControl *con, void *data );
void HandleBottomAlign(               LWControl *con, void *data );
void HandleBottomAlignArray(          LWControl *con, void *data );
void HandlePutOver(                   LWControl *con, void *data );
void HandlePutOverArray(              LWControl *con, void *data );

void ResetControls();

struct PanelToolsData {
  TMP_PanelTools *tools;
  LWPanelFuncs   *panel_funcs;
  LWMessageFuncs *message;

  LWPanelID       panel;

  LWControl      *first;
  LWControl      *second;
  LWControl      *third;

  LWControl      *array[4];
};

const char *second_strings[] = { "Just a string", NULL };

PanelToolsData ptdata;

// PanelToolsTest_Activate():
XCALL_ (int) PanelToolsTest_Activate( long version, GlobalFunc *global, void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  ptdata.panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,   GFUSE_TRANSIENT );
  ptdata.tools       = (TMP_PanelTools  *)global( TMP_PANELTOOLS_GLOBAL, GFUSE_TRANSIENT );
  ptdata.message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );

  // Create the panel
  char title_string[256];
  sprintf( title_string, "PanelTools Test -- %c2001 Joe Angell, TM Productions", 169 );
  ptdata.panel = (*ptdata.panel_funcs->create)( title_string, ptdata.panel_funcs );
  if( !ptdata.panel ) {                    // Make sure the panel was created
    (*ptdata.message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Main Controls
  LWControl *alignLabels                    = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "alignLabels()",               150 );
  LWControl *alignLabelsArray               = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "alignLabelsArray()",          150 );

  LWControl *alignWith                      = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "alignWith()",                 150 );
  LWControl *alignWithArray                 = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "alignWithArray()",            150 );

  LWControl *centerUnder                    = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "centerUnder()",               150 );
  LWControl *centerUnderArray               = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "centerUnderArray()",          150 );

  LWControl *putUnder                       = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putUnder()",                  150 );
  LWControl *putUnderArray                  = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putUnderArray()",             150 );

  LWControl *putOnLineWith                  = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putOnLineWith()",             150 );
  LWControl *putOnLineWithArray             = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putOnLineWithArray()",        150 );

  LWControl *rightAlign                     = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "rightAlign()",                150 );
  LWControl *rightAlignArray                = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "rightAlignArray()",           150 );

  LWControl *rightAlignWithPanel            = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "rightAlignWithPanel()",       150 );
  LWControl *rightAlignWithPanelArray       = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "rightAlignWithPanelArray()",  150 );

  LWControl *bottomAlign                    = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "bottomAlign()",               150 );
  LWControl *bottomAlignArray               = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "bottomAlignArray()",          150 );

  LWControl *putOver                        = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putOver()",                   150 );
  LWControl *putOverArray                   = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "putOverArray()",              150 );

  LWControl *aligner[] = { alignLabels, alignLabelsArray, alignWith, alignWithArray, centerUnder, centerUnderArray,
                           putUnder, putUnderArray, putOnLineWith, putOnLineWithArray, rightAlign, rightAlignArray,
                           rightAlignWithPanel, rightAlignWithPanelArray, bottomAlign, bottomAlignArray, putOver, putOverArray, NULL };
  ptdata.tools->putUnderArray( aligner, 0 );

  // Test Controls
  ptdata.first  = WBUTTON_CTL( ptdata.panel_funcs, ptdata.panel, "First", 100 );
  ptdata.second = WPOPUP_CTL(  ptdata.panel_funcs, ptdata.panel, "Second", second_strings, 100 );
  ptdata.third  = STR_CTL(     ptdata.panel_funcs, ptdata.panel, "Third", 40 );

  ptdata.array[0] = ptdata.first;
  ptdata.array[1] = ptdata.second;
  ptdata.array[2] = ptdata.third;
  ptdata.array[3] = NULL;

  ResetControls();

  // Event Handlers
  CON_SETEVENT( alignLabels,               HandleAlignLabels,               NULL );
  CON_SETEVENT( alignLabelsArray,          HandleAlignLabelsArray,          NULL );
  CON_SETEVENT( alignWith,                 HandleAlignWith,                 NULL );
  CON_SETEVENT( alignWithArray,            HandleAlignWithArray,            NULL );
  CON_SETEVENT( centerUnder,               HandleCenterUnder,               NULL );
  CON_SETEVENT( centerUnderArray,          HandleCenterUnderArray,          NULL );
  CON_SETEVENT( putUnder,                  HandlePutUnder,                  NULL );
  CON_SETEVENT( putUnderArray,             HandlePutUnderArray,             NULL );
  CON_SETEVENT( putOnLineWith,             HandlePutOnLineWith,             NULL );
  CON_SETEVENT( putOnLineWithArray,        HandlePutOnLineWithArray,        NULL );
  CON_SETEVENT( rightAlign,                HandleRightAlign,                NULL );
  CON_SETEVENT( rightAlignArray,           HandleRightAlignArray,           NULL );
  CON_SETEVENT( rightAlignWithPanel,       HandleRightAlignWithPanel,       NULL );
  CON_SETEVENT( rightAlignWithPanelArray,  HandleRightAlignWithPanelArray,  NULL );
  CON_SETEVENT( bottomAlign,               HandleBottomAlign,               NULL );
  CON_SETEVENT( bottomAlignArray,          HandleBottomAlignArray,          NULL );
  CON_SETEVENT( putOver,                   HandlePutOver,                   NULL );
  CON_SETEVENT( putOverArray,              HandlePutOverArray,              NULL );

  // Panel
  PAN_SETW( ptdata.panel_funcs, ptdata.panel, 500 );
  ptdata.panel_funcs->open( ptdata.panel, PANF_BLOCKING );

  return AFUNC_OK;
}

void ResetControls() {
  ERASE_CON( ptdata.first  );
  ERASE_CON( ptdata.second );
  ERASE_CON( ptdata.third  );
  
  MOVE_CON( ptdata.first,  200, 200 );
  MOVE_CON( ptdata.second, 200, 230 );
  MOVE_CON( ptdata.third,  200, 260 );

  UNGHOST_CON( ptdata.first  );
  UNGHOST_CON( ptdata.second );
  UNGHOST_CON( ptdata.third  );

  ptdata.panel_funcs->draw( ptdata.panel, DR_REFRESH );
}

void HandleAlignLabels( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->alignLabels( ptdata.first,  ptdata.second, 0 );
  ptdata.tools->alignLabels( ptdata.second, ptdata.third,  0 );
}

void HandleAlignLabelsArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->alignLabelsArray( ptdata.array, 0 );
}

void HandleAlignWith( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->alignWith( ptdata.first,  ptdata.second );
  ptdata.tools->alignWith( ptdata.second, ptdata.third );
}

void HandleAlignWithArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->alignWithArray( ptdata.array );
}

void HandleCenterUnder( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->centerUnder( ptdata.first,  ptdata.second, 0, 1 );
  ptdata.tools->centerUnder( ptdata.second, ptdata.third,  0, 1 );
}

void HandleCenterUnderArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->centerUnderArray( ptdata.array, 0 );
}

void HandlePutUnder( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putUnder( ptdata.first,  ptdata.second, 0 );
  ptdata.tools->putUnder( ptdata.second, ptdata.third,  0 );
}

void HandlePutUnderArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putUnderArray( ptdata.array, 0 );
}

void HandlePutOnLineWith( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putOnLineWith( ptdata.first,  ptdata.second, 0 );
  ptdata.tools->putOnLineWith( ptdata.second, ptdata.third,  0 );
}

void HandlePutOnLineWithArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putOnLineWithArray( ptdata.array, 0 );
}

void HandleRightAlign( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->rightAlign( ptdata.first,  ptdata.second, 0, 1, 0 );
  ptdata.tools->rightAlign( ptdata.second, ptdata.third,  0, 1, 0 );
}

void HandleRightAlignArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->rightAlignArray( ptdata.array, 0, 1, 0 );
}

void HandleRightAlignWithPanel( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->rightAlignWithPanel( ptdata.panel_funcs, ptdata.panel, ptdata.first,  0 );
  ptdata.tools->rightAlignWithPanel( ptdata.panel_funcs, ptdata.panel, ptdata.second, 0 );
  ptdata.tools->rightAlignWithPanel( ptdata.panel_funcs, ptdata.panel, ptdata.third,  0 );
}

void HandleRightAlignWithPanelArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->rightAlignWithPanelArray( ptdata.panel_funcs, ptdata.panel, ptdata.array, 0, 1, 0 );
}

void HandleBottomAlign( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->bottomAlign( ptdata.first,  ptdata.second, 0 );
  ptdata.tools->bottomAlign( ptdata.second, ptdata.third,  0 );
}

void HandleBottomAlignArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->bottomAlignArray( ptdata.array, 0 );
}

void HandlePutOver( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putOver( ptdata.first,  ptdata.second, 0 );
  ptdata.tools->putOver( ptdata.second, ptdata.third,  0 );
}

void HandlePutOverArray( LWControl *con, void *data ) {
  ResetControls();
  ptdata.tools->putOverArray( ptdata.array, 0 );
}

