/****
 * lwvbshelf.h
 ****
 * COPYRIGHT (C) 1999 NewTek, Inc.
 ****
 */

#ifndef LWVBSHELF_H
#define LWVBSHELF_H

/* Standard */
#include <stdio.h>
#include <stdlib.h>

#include <lwdialog.h>
#include <lwglobsrv.h>


/****
 * Defines
 ****
 */
#define LWBRFUNCS_GLOBAL "LWBrowseRequest"

/****
 * TypeDefs
 ****
 */
typedef struct st_BrowseClient *LWBRCltID;
typedef char **LWBRStrList;

/* callbacks */
typedef int   LWBRPreMultiFunc ( void *userdata, char *fullname,
                                 char *datafile, char **parms );
typedef char *LWBRFileThumFunc ( void *userdata, char *filename,
                                 int *thum_w, int *thum_h );
typedef char *LWBRPreThumFunc  ( void *userdata, int *thum_w,
                                 int *thum_h );
typedef int   LWBRLoadSettings ( void *userdata, char *datafile,
                                 LWBRStrList parms );
typedef int   LWBRShelfClosed  ( void *userdata );

/* services API */
typedef struct st_LWBRFUNCS {
  LWBRCltID (*subscribe)      ( char *str, char *substr, void *userdata,
                                char *path, char *filetype,
                                LWBRFileThumFunc *filethum,
                                LWBRPreThumFunc  *prethum,
                                LWBRPreMultiFunc *premulti,
                                LWBRLoadSettings *loadsettings,
                                LWBRShelfClosed  *closedNotify );
  void    (*unsubscribe)      ( LWBRCltID );
  /* These allow the client to set the callbacks
   * or userdata pointer independently or modify
   * them at a later date
   */
  void    (*setFileThumFunc)  ( LWBRCltID clt, LWBRFileThumFunc *func );
  void    (*setPreThumFunc)   ( LWBRCltID clt, LWBRPreThumFunc  *func );
  void    (*setPreMultiFunc)  ( LWBRCltID clt, LWBRPreMultiFunc *func );
  void    (*setLoadSettings)  ( LWBRCltID clt, LWBRLoadSettings *func );
  void    (*setShelfClosed)   ( LWBRCltID clt, LWBRShelfClosed  *func );
  void    (*setUserData)      ( LWBRCltID clt, void *userdata );
  /* These are used to load/save a preset file */
  int     (*presetSave)       ( LWBRCltID clt, char *title,
                                char *filename, char *datafile,
                                LWBRStrList parms );
  int     (*presetLoad)       ( LWBRCltID clt, char *title,
                                char *filename, char *datafile,
                                LWBRStrList *parms );
  int     (*presetMulti)      ( LWBRCltID clt, char *title );
  /* The following are used to open and close the non-modal preset shelf.
   * shelfFocus sets focus to the specified client
   */
  void    (*shelfOpen)        ( LWBRCltID clt );
  void    (*shelfClose)       ( LWBRCltID clt );
  int     (*shelfIsOpen)      ( LWBRCltID clt );
  void    (*shelfFocus)       ( LWBRCltID clt );
  /* This adds a preset to the shelf */
  int     (*shelfInclude)     ( LWBRCltID clt, char *datafilename,
                                LWBRStrList parms );
} LWBRFuncs;

/* Close the LWVBSHELF_H header */
#endif
