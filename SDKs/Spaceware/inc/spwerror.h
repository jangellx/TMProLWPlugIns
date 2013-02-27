/*----------------------------------------------------------------------
 *  spwerror.h -- Standard Spacetec IMC function return values
 *
 *  $Id: spwerror.h,v 1.10.4.1 1998/05/26 17:30:21 equerze Exp $
 *
 *  This file contains all the Spacetec IMC standard error return
 *  return values for functions
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

#ifndef _SPWERROR_H_
#define _SPWERROR_H_

#ifdef EXPORT_FUNCTIONS
#define DLL_EXPORT __declspec(dllexport)
#define CALL_TYPE __stdcall
#else
#define DLL_EXPORT
#define CALL_TYPE
#endif

static char spwerrorCvsId[]="(C) 1996 Spacetec IMC Corporation: $Id: spwerror.h,v 1.10.4.1 1998/05/26 17:30:21 equerze Exp $";

enum SpwRetVal             /* Error return values */
   {
   SPW_NO_ERROR,           /* No error */
   SPW_ERROR,              /* Error -- function failed */
   SI_BAD_HANDLE,          /* Invalid SpaceWare handle */
   SI_BAD_ID,              /* Invalid device ID */
   SI_BAD_VALUE,           /* Invalid argument value */
   SI_IS_EVENT,            /* Event is a SpaceWare event */
   SI_SKIP_EVENT,          /* Skip this SpaceWare event */
   SI_NOT_EVENT,           /* Event is not a SpaceWare event */
   SI_NO_DRIVER,           /* SpaceWare driver is not running */
   SI_NO_RESPONSE,         /* SpaceWare driver is not responding */
   SI_UNSUPPORTED,         /* The function is unsupported by this version */
   SI_UNINITIALIZED,       /* SpaceWare input library is uninitialized */
   SI_WRONG_DRIVER,        /* Driver is incorrect for this SpaceWare version */
   SI_INTERNAL_ERROR,      /* Internal SpaceWare error */
   SPW_DLL_LOAD_ERROR,     /* Could not load siapp dlls */ 
  };

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SPW_DEFINE_GLOBALS
DLL_EXPORT enum SpwRetVal SpwErrorVal = SPW_NO_ERROR;
#else
extern enum SpwRetVal SpwErrorVal;
#endif

extern DLL_EXPORT char * CALL_TYPE SpwErrorString (enum SpwRetVal val);

#ifdef __cplusplus
}
#endif

#endif   /* _SPWERROR_H_ */
