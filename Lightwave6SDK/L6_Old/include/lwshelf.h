/****
 * lwshelf.h
 ****
 * COPYRIGHT (C) 1999 NewTek, Inc.
 ****
 */

#ifndef LWSHELF_H
#define LWSHELF_H

/* Standard */
#include <stdio.h>
#include <stdlib.h>

#include <lwdialog.h>
#include <lwglobsrv.h>
#include <lwtypes.h>
#include <lwimage.h>
#include <lwio.h>

/****
 * Callback Prototypes:
 * --------------------
 * LWShelfLoadOkFunc - Asks the subscribed client if it is ready to accept
 *                     loading a preset.
 * LWShelfLoadFunc - Called to have the client load settings
 * LWShelfSaveFunc - Called during save to obtain client settings
 *
 *     These load and save callbacks provide for loading and
 *     saving client settings using the LW Load and Save State (LWIO.H)
 *     OR as a separate data file.
 *
 *     Use of the LWIO states is recommended.
 *
 *     Which format is used depends on the subscription flags
 *     where SHLF_BIN or SHLF_ASC uses the corresponding LWIO
 *     load/save states and SHLF_SEP indicates the client will
 *     save data to a separate file in whatever manner best suits
 *     its needs. The filename argument to the Load/SaveFunc
 *     callbacks is the filename the client should use for this
 *     activity.
 *
 * LWShelfClosed   - Callback to notify subscribed clients of the following:
 *                   1) The client was the currently active client (had focus)
 *                   2) The user closed the Preset Shelf window directly
 *                   3) (Typical) The client should not reactivate the
 *                      shelf automatically since the user closed it for
 *                      a reason.
 *
 * API Description:
 * ----------------
 * subscribe      - Adds a client to the Preset Shelf and returns a client ID.
 * unsubscribe    - Unsubscribes the client. Clients should, at a minimum,
 *                  do this when its instance is destroyed.
 * open           - Opens the Preset Shelf window and sets focus to the client.
 * close          - Closes the Shelf.
 * isOpen         - Returns true if the Preset Shelf window is currently open.
 * getFocus       - Grabs the focus of the Shelf. This differs from the Open
 *                  method by giving the client focus but does not open the
 *                  Shelf window.
 * addPreset      - Called by the client to add a preset settings
 *                  file to the Presets Shelf. The 'img' is an
 *                  LWImageID to use as the preset's thumbnail. The
 *                  'parms' is a NULL terminated list of string
 *                  parameter tags which can be used to conditionally
 *                  load settings from the preset.
 * (Independent Operations)
 * load/save      - These load/save preset settings files to an
 *                  arbitrary location specified by 'filename'. As part
 *                  of the shelf API, the load and save methods reuse
 *                  the client subscription settings (like userdata,
 *                  callbacks, etc.). If 'prompt_user' is true and the
 *                  preset contains a parameter list, this will prompt
 *                  user for input.
 ****
 */

#define LWSHELFFUNCS_GLOBAL "LWShelfFuncs"

typedef struct st_ShelfClient *LWShelfCltID;
typedef char **LWShelfParmList;

typedef int  LWShelfLoadOkFunc ( void *userdata );
typedef void LWShelfLoadFunc   ( void *userdata, const LWLoadState *load,
                                 const char *filename, LWShelfParmList parms );
typedef void LWShelfSaveFunc   ( void *userdata, const LWSaveState *save,
                                 const char *filename );
typedef void LWShelfClosed     ( void *userdata );

/* Load and Save Options */
#define SHLF_BIN    1   /* Binary LWIO State */
#define SHLF_ASC    2   /* Ascii LWIO State */
#define SHLF_SEP    3   /* Client uses separate file */

/* LoadOk Codes */
enum en_SHELF_LOADOK {
  SHLC_NOWAY = 0,  /* Do not load. Client told user reason */
  SHLC_DFLT,       /* Use default confirmation dialog. */
  SHLC_FORCE       /* Load. Do Not confirm with user. */
};

typedef struct st_LWSHELFFUNCS {
  /* Client subscriptions */
  LWShelfCltID (*subscribe)    ( char *name, char *subName,
                                 void *userData, int flags,
                                 LWShelfLoadOkFunc *loadOk_callback,
                                 LWShelfLoadFunc *load_callback,
                                 LWShelfSaveFunc *save_callback,
                                 LWShelfClosed *closedNotify );
  void         (*unsubscribe)  ( LWShelfCltID clt );
  /* Shelf Window */
  void         (*open)         ( LWShelfCltID clt );
  void         (*close)        ( LWShelfCltID clt );
  int          (*isOpen)       ( LWShelfCltID clt );
  void         (*getFocus)     ( LWShelfCltID clt );
  /* Adding Preset */
  int          (*addPreset)    ( LWShelfCltID clt,
                                 LWImageID img, LWShelfParmList parms );
  /* Independent Operations */
  void         (*load)         ( LWShelfCltID clt,
                                 char *filename, int prompt_user );
  void         (*save)         ( LWShelfCltID clt, char *filename,
                                 LWImageID thumimg, LWShelfParmList parms );
} LWShelfFuncs;

/* Close the header */
#endif
