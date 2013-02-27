/*----------------------------------------------------------------------
 *  si.h -- SpaceWare input library header
 *
 *  $Id: si.h,v 1.22 1998/03/12 11:07:03 mfarr Exp $
 *
 *  SpaceWare input library
 *
 *----------------------------------------------------------------------
 *
 *  (C) 1996 Spacetec IMC Corporation.  All rights reserved.
 *
 *  The computer codes included in this file, including source code and
 *  object code, constitutes the proprietary and confidential information of
 *  Spacetec IMC Corporation, and are provided pursuant to a license
 *  agreement.  These computer codes are protected by international, federal
 *  and state law, including United States Copyright Law and international
 *  treaty provisions.  Except as expressly authorized by the license
 *  agreement, or as expressly permitted under applicable laws of member
 *  states of the European Union and then only to the extent so permitted,
 *  no part of these computer codes may be reproduced or transmitted in any
 *  form or by any means, electronic or mechanical, modified, decompiled,
 *  disassembled, reverse engineered, sold, transferred, rented or utilized
 *  for any unauthorized purpose without the express written permission of
 *  Spacetec IMC Corporation.
 *
 */


#ifndef _SI_H_
#define _SI_H_

static char incFileNameCvsId[]="(C) 1996 Spacetec IMC Corporation: $Id: si.h,v 1.22 1998/03/12 11:07:03 mfarr Exp $";

#include "spwmacro.h"
#include "spwdata.h"

#ifdef OS_DEFINED
#undef OS_DEFINED
#endif

#ifdef OS_X
#define OS_DEFINED
typedef long Display;
typedef long Window;
typedef long XEvent;
#endif

#ifdef OS_WIN32
#define OS_DEFINED
#include <windows.h>
#endif

#ifndef OS_DEFINED
#ifdef WIN32
#pragma message ("ERROR: an OS type must be defined -- e.g. OS_WIN32")
#endif
ERROR: an OS type must be defined -- e.g. OS_WIN32
#endif

#include <stdio.h>
#include "spwerror.h"

/* UI modes */

#define SI_UI_ALL_CONTROLS    0xffffffffL
#define SI_UI_NO_CONTROLS     0x00000000L
#define SI_UI_FILTERS         0x00000001L
#define SI_UI_FUNC_BUTTONS    0x00000002L
#define SI_UI_RESET_BUTTONS   0x00000004L
#define SI_UI_SENSITIVITY     0x00000008L
#define SI_UI_TUNING          0x00000010L
#define SI_UI_DIALOG_POPUP    0x00000020L

/* Device types and classes */

#define SI_SPACEBALL_2003     1
#define SI_SPACEBALL_3003     2
#define SI_SPACE_CONTROLLER   3
#define SI_AVENGER            4
#define SI_SPACEORB_360       5
#define SI_NAVIGATOR          6
#define SI_ALL_TYPES          -1

#define SI_HIGH_END           63
#define SI_MED_END            62
#define SI_LOW_END            61

/* Data retrieval mode */

#define SI_EVENT              0x0001
#define SI_POLL               0x0002

/* Get event flags */

#define SI_AVERAGE_EVENTS     0x0001

/* SpaceWare event types */

#define SI_BUTTON_EVENT       1
#define SI_MOTION_EVENT       2
#define SI_COMBO_EVENT        3
#define SI_ZERO_EVENT         4
#define SI_EXCEPTION_EVENT    5

/* Data modes */

#define SI_MODE_NORMALIZE     0x0001
#define SI_MODE_COMPRESS      0x0002
#define SI_MODE_SENSITIVITY   0x0004
#define SI_MODE_TUNING        0x0008

/* Motion data offsets */

#define SI_TX                 0              /* Translation X value */
#define SI_TY                 1              /* Translation Y value */
#define SI_TZ                 2              /* Translation Z value */
#define SI_RX                 3              /* Rotation X value */
#define SI_RY                 4              /* Rotation Y value */
#define SI_RZ                 5              /* Rotation Z value */

/* Reserved buttons */

#define SI_RESET_BIT          0x00000001L
#define SI_PICK_BIT           0x80000000L
#define SI_DIALOG_BIT         0x40000000L

#define SI_RESET_BUTTON       0
#define SI_PICK_BUTTON        31
#define SI_DIALOG_BUTTON      30

/* Miscellaneous */

#define SI_END_ARGS           0
#define SI_NO_HANDLE          ((SiHdl) NULL)
#define SI_ALL_HANDLES        ((SiHdl) NULL)
#define SI_ANY_HANDLE         ((SiHdl) NULL)
#define SI_NO_TRANSCTL        ((SiTransCtl) NULL)
#define SI_NO_MASK            ((SiTypeMask *) NULL)
#define SI_ANY_DEVICE         -1
#define SI_NO_DEVICE          -1
#define SI_NO_TYPE            -1
#define SI_NO_LIST            -1
#define SI_NO_BUTTON          -1
#define SI_STRSIZE            128
#define SI_MAXBUF             128

typedef int SiDevID;          /* Device ID */
typedef void *SiHdl;          /* SpaceWare handle */
typedef void *SiTransCtl;     /* SpaceWare transport control handle */

#ifdef OS_X
typedef struct                /* Open data */
   {
   Display *display;
   Window window;
   } SiOpenData;

typedef struct                /* Get event Data */
   {
   XEvent *event;
   } SiGetEventData;
#endif

#ifdef OS_WIN32
typedef struct                /* Open data */
   {
   HWND hWnd;                 /* Window handle for SpaceWare messages */
   SiTransCtl transCtl;       /* SpaceWare transport control handle */
   } SiOpenData;

typedef struct                /* Get event data */
   {
   UINT msg;
   WPARAM wParam;
   LPARAM lParam;
   } SiGetEventData;
#endif

typedef struct                /* Device type mask */
   {
   unsigned char mask[8];
   } SiTypeMask;

typedef struct                /* Device port information */
   {
   SiDevID devID;             /* Device ID */
   int devType;               /* Device type */
   int devClass;              /* Device class */
   char devName[SI_STRSIZE];  /* Device name */
   char portName[SI_STRSIZE]; /* Port name */
   } SiDevPort;

typedef struct                /* Device information */
   {
   int devType;               /* Device type */
   int numButtons;            /* Number of buttons */
   int numDegrees;            /* Number of degrees of freedom */
   SPWbool canBeep;           /* Device beeps */
   char firmware[SI_STRSIZE]; /* Firmware version */
   } SiDevInfo;

typedef struct                /* Version information */
   {
   int major;                 /* Major version number */
   int minor;                 /* Minor version number */
   int build;                 /* Build number */
   char version[SI_STRSIZE];  /* Version string */
   char date[SI_STRSIZE];     /* Date string */
   } SiVerInfo;

typedef struct                /* Sensitivity parameters */
   {
   char dummy;
   } SiSensitivity;

typedef struct                /* Tuning parameters */
   {
   char dummy;
   } SiTuning;

typedef struct                /* Bitmasks of button states */
   {
   SPWuint32 last;            /* Buttons pressed as of last event */
   SPWuint32 current;         /* Buttons pressed as of this event */
   SPWuint32 pressed;         /* Buttons pressed this event */
   SPWuint32 released;        /* Buttons released this event */
   } SiButtonData;

typedef struct                /* SpaceWare data */
   {
   SiButtonData bData;        /* Button data */
   long mData[6];             /* Motion data (index via SI_TX, etc) */
   long period;               /* Period (milliseconds) */
   } SiSpwData;

typedef struct                /* SpaceWare event */
   {
   int type;                  /* Event type */
   union
      {
      SiSpwData spwData;      /* Button, motion, or combo data */
      char exData[SI_MAXBUF]; /* Exception data */
      } u;
   } SiSpwEvent;

typedef struct                /* Event handler */
   {
   int (*func) (SiOpenData *, SiGetEventData *, SiSpwEvent *, void *);
   void *data;
   } SiEventHandler;

typedef struct                /* SpaceWare event handlers */
   {
   SiEventHandler button;     /* Button event handler */
   SiEventHandler motion;     /* Motion event handler */
   SiEventHandler combo;      /* Combo event handler */
   SiEventHandler zero;       /* Zero event handler */
   SiEventHandler exception;  /* Exception event handler */
   } SiSpwHandlers;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef EXPORT_FUNCTIONS
#define DLL_EXPORT __declspec(dllexport)
#define CALL_TYPE __stdcall
#else
#define DLL_EXPORT
#define CALL_TYPE
#endif

extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiInitialize (void);
extern DLL_EXPORT void CALL_TYPE SiTerminate (void);
extern DLL_EXPORT int CALL_TYPE SiGetNumDevices (void);
extern DLL_EXPORT SiDevID CALL_TYPE SiDeviceIndex (int idx);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiGetDevicePort (SiDevID devID, SiDevPort *pPort);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiGetDriverInfo (SiVerInfo *pInfo);
extern DLL_EXPORT void CALL_TYPE SiGetLibraryInfo (SiVerInfo *pInfo);
extern enum SpwRetVal __SiGetDataRate (SiDevID devID, unsigned int *pRate);
extern enum SpwRetVal __SiGetSensitivity (SiDevID devID, SiSensitivity *pSParm);
extern enum SpwRetVal __SiGetTuning (SiDevID devID, SiTuning *pTParm);
extern enum SpwRetVal __SiGetDataMode (SiDevID devID, int *pMode);
extern enum SpwRetVal __SiSetDataRate (SiDevID devID, unsigned int rate);
extern enum SpwRetVal __SiSetSensitivity (SiDevID devID, SiSensitivity *pSParm);
extern enum SpwRetVal __SiSetTuning (SiDevID devID, SiTuning *pTParm);
extern enum SpwRetVal __SiSetDataMode (SiDevID devID, int mode);
extern DLL_EXPORT SiHdl CALL_TYPE SiOpen (char *pAppName, SiDevID devID,
   SiTypeMask *pTMask, int mode, SiOpenData *pData);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiClose (SiHdl hdl);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiGetEvent (SiHdl hdl, int flags,
   SiGetEventData *pData, SiSpwEvent *pEvent);
extern DLL_EXPORT int CALL_TYPE SiDispatch (SiHdl hdl, SiGetEventData *pData,
   SiSpwEvent *pEvent, SiSpwHandlers *pDHandlers);
extern enum SpwRetVal __SiWrite (SiHdl hdl, unsigned char *pData, int size);
extern enum SpwRetVal __SiConverse (SiHdl hdl,
   unsigned char *pWData, int wSize, unsigned char *pRData, int *pRSize);
extern DLL_EXPORT SiDevID CALL_TYPE SiGetDeviceID (SiHdl hdl);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiBeep (SiHdl hdl, char *string);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiRezero (SiHdl hdl);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiGrabDevice (SiHdl hdl, SPWbool exclusive);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiReleaseDevice (SiHdl hdl);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiGetDeviceInfo (SiHdl hdl, SiDevInfo *pInfo);
extern enum SpwRetVal SiGetUiMode (SiHdl hdl, SPWuint32 *pMode);
extern DLL_EXPORT enum SpwRetVal CALL_TYPE SiSetUiMode (SiHdl hdl, SPWuint32 mode);
extern enum SpwRetVal SiSetTypeMask (SiTypeMask *pTMask, int type1, ...);
extern enum SpwRetVal SiAndTypeMask (SiTypeMask *pTMaskA, SiTypeMask *pTMaskB);
extern int SiGetPortList (SiDevPort **ppPort);
extern void SiFreePortList (SiDevPort *pPort);
extern DLL_EXPORT SPWbool CALL_TYPE SiIsSpaceWareEvent (SiGetEventData *pData, SiHdl *pHdl);
extern DLL_EXPORT int CALL_TYPE SiButtonPressed (SiSpwEvent *pEvent);
extern DLL_EXPORT int CALL_TYPE SiButtonReleased (SiSpwEvent *pEvent);

extern void SiTune2003 (SiSpwEvent *pEvent);
extern void SiTuneSC (SiSpwEvent *pEvent);


#ifdef OS_X
extern void SiOpenXInit (SiOpenData *pData, Display *display, Window window);
extern void SiGetEventXInit (SiGetEventData *pData, XEvent *event);
#endif

#ifdef OS_WIN32
extern DLL_EXPORT void CALL_TYPE SiOpenWinInit (SiOpenData *pData, HWND hWnd);
extern DLL_EXPORT void CALL_TYPE SiGetEventWinInit (SiGetEventData *pData,
   UINT msg, WPARAM wParam, LPARAM lParam);
#endif

#ifdef __cplusplus
}
#endif

#endif   /* _SI_H_ */
