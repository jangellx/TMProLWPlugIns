/*----------------------------------------------------------------------
 *  sbtest32.h  -- Basic Program to init spaceball, read its data and 
 *                  print it out.
 *
 *  $Id: sbtest32.h,v 1.4 1998/03/24 14:28:41 jeffl Exp $
 *
 *  Written By Elio Querze
 *  
 *
 *----------------------------------------------------------------------
 *
 *  (C) 1997 Spacetec IMC Corporation.  All rights reserved.
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

static char SbTestCvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: sbtest32.h,v 1.4 1998/03/24 14:28:41 jeffl Exp $";
/* Global Variables */
 
#ifndef SBTEST32_H
#define SBTEST32_H

HDC         hdc;          /* Handle to Device Context used to draw on screen */
HWND        hWndMain;     /* Handle to Main Window */
SiHdl       devHdl;       /* Handle to Spaceball Device */

/* Functions --See C File for Additional Details */

long  WINAPI HandleNTEvent (HWND hWnd, unsigned msg, WPARAM wParam, LPARAM lParam);
int   DispatchLoopNT(); 
void  CreateSPWindow(int, int, int, int, char *);
int   SbInit();
void  SbMotionEvent(SiSpwEvent *pEvent);
void  SbZeroEvent();
void  SbButtonPressEvent(int buttonnumber);
void  SbButtonReleaseEvent(int buttonnumber);
#endif