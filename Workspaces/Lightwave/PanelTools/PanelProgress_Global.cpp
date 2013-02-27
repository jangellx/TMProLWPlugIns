//
// PanelProgress_Global.cpp
//

#define NULL 0

#include "PanelProgress_Global.h"

#include <lightwave/lwProgressPanel/lwprogresspanel.h>

TMP_PanelProgress panel_progress_functions = {
  pp_progressControl, pp_progressControlRedraw, pp_progressControlGetTotal, pp_progressControlSetTotal,
  pp_progressControlGetCurrent, pp_progressControlSetCurrent, pp_progressControlGetString, pp_progressControlSetString,
  pp_progressControlIncrementBar, pp_progressControlIncrementBarString, pp_progressControlSetBar,
  pp_progressControlGetControl, pp_progressControlDestroy,

  pp_progressPanel, pp_progressPanelOpen, pp_progressPanelClose, pp_progressPanelIsOpen,
  pp_progressPanelHandle, pp_progressPanelHandleProgress, pp_progressPanelAbortMessage,
  pp_progressPanelGetTotal, pp_progressPanelSetTotal, pp_progressPanelGetCurrent, pp_progressPanelSetCurrent,
  pp_progressPanelGetString, pp_progressPanelSetString, pp_progressPanelIncrementBar,
  pp_progressPanelIncrementBarString, pp_progressPanelSetBar, pp_progressPanelGetPanel, pp_progressPanelDestroy
};

// pp_progressControl():
ProgressControlID pp_progressControl( GlobalFunc *global, LWPanelID panel, int width ) {
  return NULL;
}

// pp_progressControlRedraw():
void pp_progressControlRedraw( ProgressControlID priv ) {
  ;
}

// pp_progressControlGetTotal():
int pp_progressControlGetTotal( ProgressControlID priv ){
  return 0;
}

// pp_progressControlSetTotal():
void pp_progressControlSetTotal( ProgressControlID priv, int total ) {
  ;
}

// pp_progressControlGetCurrent():
int pp_progressControlGetCurrent( ProgressControlID priv ){
  return 0;
}

// pp_progressControlSetCurrent():
void pp_progressControlSetCurrent( ProgressControlID priv, int current ) {
  ;
}

// pp_progressControlGetString():
const char * pp_progressControlGetString( ProgressControlID priv ) {
  return "";
}

// pp_progressControlSetString():
void pp_progressControlSetString( ProgressControlID priv, const char *string ) {
  ;
}

// pp_progressControlIncrementBar():
void pp_progressControlIncrementBar( ProgressControlID priv ) {
  ;
}

// pp_progressControlIncrementBarString():
void pp_progressControlIncrementBarString( ProgressControlID priv, const char *_string ) {
  ;
}

// pp_progressControlSetBar():
void pp_progressControlSetBar( ProgressControlID priv, int current, const char *string ) {
  ;
}

// pp_progressControlGetControl():
LWControlID pp_progressControlGetControl( ProgressControlID priv ) {
  return NULL;
}

// pp_progressControlDestroy():
void pp_progressControlDestroy( ProgressControlID priv ) {
  ;
}


//
// Progress Panel
//

// pp_progressPanel():
ProgressPanelID pp_progressPanel( GlobalFunc *global ) {
  return new LWProgressPanel( global, 100 );
}

// pp_progressPanelOpen():
int pp_progressPanelOpen( ProgressPanelID priv, const char *title ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return progress->Open( title );
}

// pp_progressPanelClose():
void pp_progressPanelClose( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->Close();
}

// pp_progressPanelIsOpen();
int pp_progressPanelIsOpen( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return (progress->IsOpen() ? 1 : 0);
}

// pp_progressPanelHandle():
int pp_progressPanelHandle( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return (progress->Handle() ? 1 : 0);
}

// pp_progressPanelHandleProgress():
int pp_progressPanelHandleProgress( ProgressPanelID priv, int _progress, const char *string ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return (progress->Handle( _progress, string) ? 1 : 0);
}

// pp_progressPanelAbortMessage():
void pp_progressPanelAbortMessage( ProgressPanelID priv, const char *title, const char *string1, const char *string2 ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->SetAbortMessage( title, string1, string2 );
}

// pp_progressPanelGetTotal();
int pp_progressPanelGetTotal( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return progress->GetTotal();
}

// pp_progressPanelSetTotal():
void pp_progressPanelSetTotal( ProgressPanelID priv, int total ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->SetTotal( total );
}

// pp_progressPanelGetCurrent():
int pp_progressPanelGetCurrent( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return progress->GetCurrent();
}

// pp_progressPanelSetCurrent():
void pp_progressPanelSetCurrent( ProgressPanelID priv, int current ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->SetCurrent( current );
}

// pp_progressPanelGetString():
const char * pp_progressPanelGetString( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return progress->GetString();
}

// pp_progressPanelSetString():
void pp_progressPanelSetString( ProgressPanelID priv, const char *string ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->SetString( string );
}

// pp_progressPanelIncrementBar():
void pp_progressPanelIncrementBar( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->IncrementBar();
}

// pp_progressPanelIncrementBarString():
void pp_progressPanelIncrementBarString( ProgressPanelID priv, const char *_string ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
}

// pp_progressPanelSetBar():
void pp_progressPanelSetBar( ProgressPanelID priv, int current, const char *string ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->SetBar( current, string );
}

// pp_progressPanelGetPanel():
LWPanelID pp_progressPanelGetPanel( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  return progress->panel;
}

// pp_progressPanelDestroy():
void pp_progressPanelDestroy( ProgressPanelID priv ) {
  LWProgressPanel *progress = (LWProgressPanel *)priv;
  progress->Close();
  delete progress;
}

