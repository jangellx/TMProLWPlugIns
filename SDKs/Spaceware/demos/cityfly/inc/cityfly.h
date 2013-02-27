/* sbXcityfly.h - General defines and typedefs for sbXcityfly
 *  
 *  Written:     March 1996 (moved from sbXcityfly.c)
 *  Author:      Jeff LeBlanc
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
 *
-----------------------------------------------------------------------------*/

/* 
 *  Constants and macros
 */

#define NUM_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

#define FOV    0.1  
#define XTRAN  0.0f
#define YTRAN -100.0f
#define ZTRAN -800.0f

/*
 * Typedefs
 */

typedef struct 
{
  int *porigxyz;                 /* pointers to original points in array */
  float  xyz[4];                                 /* transformed points   */
} uniq_st;

typedef struct
{
  int draw;
  int xyz[4];
  uniq_st *puniq;           /* the unique entry in uniq */
}
data3d;

extern void UpdateScreen ( void );
extern void GetUniqPoints ( data3d *local_data, uniq_st *local_uniq, int cnt );

