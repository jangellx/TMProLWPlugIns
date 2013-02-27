//
// PanelControls_WPopdown.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwpanel.h>
#include "PanelToolsGlobal.h"

extern TMP_PanelProgress panel_progress_functions;

extern "C" {
  /* ProgressControl  */
  ProgressControlID pp_progressControl             ( GlobalFunc *global, LWPanelID panel, int width );              /* Progress Control                         */

  void              pp_progressControlRedraw         ( ProgressControlID priv );                                    /* Handle input.  Returns 0 if the the user canceled the operation */

  int               pp_progressControlGetTotal     ( ProgressControlID priv );                                      /* Get the total number of steps */
  void              pp_progressControlSetTotal     ( ProgressControlID priv, int total );                           /* Set the total number of steps */

  int               pp_progressControlGetCurrent   ( ProgressControlID priv );                                      /* Get the current step */
  void              pp_progressControlSetCurrent   ( ProgressControlID priv, int current );                         /* Set the current step */

  const char *      pp_progressControlGetString    ( ProgressControlID priv );                                      /* Get the progress string */
  void              pp_progressControlSetString    ( ProgressControlID priv, const char *string );                  /* Set the progress string */

  void              pp_progressControlIncrementBar        ( ProgressControlID priv );                               /* Increment the progress bar                       */
  void              pp_progressControlIncrementBarString  ( ProgressControlID priv, const char *_string );          /* Increment the progress bar and change the string */

  void              pp_progressControlSetBar       ( ProgressPanelID priv, int current, const char *string );       /* Set the progress bar and the string */

  LWControlID       pp_progressControlGetControl   ( ProgressControlID priv );                                      /* Get the underlying control               */

  void              pp_progressControlDestroy      ( ProgressControlID priv );                                      /* Destroy the progress control             */

  /* ProgressPanel    */
  ProgressPanelID   pp_progressPanel               ( GlobalFunc *global );                                          /* Progress Panel                           */

  int               pp_progressPanelOpen           ( ProgressPanelID priv, const char *title );                     /* Open the progress panel                  */
  void              pp_progressPanelClose          ( ProgressPanelID priv);                                         /* Close the progress panel                 */
  int               pp_progressPanelIsOpen         ( ProgressPanelID priv );                                        /* Returns 1 if the panel is open           */

  int               pp_progressPanelHandle         ( ProgressPanelID priv );                                        /* Handle input.  Returns 0 if the the user canceled the operation                             */
  int               pp_progressPanelHandleProgress ( ProgressPanelID priv, int progress, const char *string );      /* Handle input and update the progress bar.  Returns 0 if the the user canceled the operation */
  void              pp_progressPanelAbortMessage   ( ProgressPanelID priv, const char *title, const char *string1, const char *string2 ); /* Message to present the user on abort                                  */

  int               pp_progressPanelGetTotal       ( ProgressPanelID priv );                                        /* Get the total number of steps */
  void              pp_progressPanelSetTotal       ( ProgressPanelID priv, int total );                             /* Set the total number of steps */

  int               pp_progressPanelGetCurrent     ( ProgressPanelID priv );                                        /* Get the current step */
  void              pp_progressPanelSetCurrent     ( ProgressPanelID priv, int current );                           /* Set the current step */

  const char *      pp_progressPanelGetString      ( ProgressPanelID priv );                                        /* Get the progress string */
  void              pp_progressPanelSetString      ( ProgressPanelID priv, const char *string );                    /* Set the progress string */

  void              pp_progressPanelIncrementBar        ( ProgressPanelID priv );                                   /* Increment the progress bar                       */
  void              pp_progressPanelIncrementBarString  ( ProgressPanelID priv, const char *_string );              /* Increment the progress bar and change the string */

  void              pp_progressPanelSetBar         ( ProgressPanelID priv, int current, const char *_string );      /* Set the progress bar and the string */

  LWPanelID         pp_progressPanelGetPanel       ( ProgressPanelID priv );                                        /* Get the panel                            */

  void              pp_progressPanelDestroy        ( ProgressPanelID priv );                                        /* Destroy the progress panel               */
}

