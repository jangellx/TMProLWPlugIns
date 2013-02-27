/*-----------------------------------------------------------------------------
 *
 *  initmat.c -- initializes a matrix
 *
 *  $Id: initmat.c,v 1.7 1998/03/05 13:18:27 jeffl Exp $
 *
 *  Initialize a matrix with the passed values
 *
 *-----------------------------------------------------------------------------
 *
 * (C) 1998 Spacetec IMC Corporation ("Spacetec").
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies. Permission to modify this software is granted
 * and Spacetec will support such modifications only if said modifications are
 * approved by Spacetec.
 *
 */
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: initmat.c,v 1.7 1998/03/05 13:18:27 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void
 * SPW_InitializeMatrix( Matrix m, float m00, float m01, float m02, float m03,
 *                                 float m10, float m11, float m12, float m13,
 *                                 float m20, float m21, float m22, float m23,
 *                                 float m30, float m31, float m32, float m33 )
 *
 * Args: m            (w/o) -matrix to be initialized
 *       m[0-3][0-3]  (r/o) -values in obvious order
 *
 * Return Value:
 *   None
 *
 * Description:
 *        Initialize a matrix with the passed values
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_InitializeMatrix( Matrix m, float m00, float m01, float m02, float m03,
                                float m10, float m11, float m12, float m13,
                                float m20, float m21, float m22, float m23,
                                float m30, float m31, float m32, float m33 )
{
   /*
    * first row
    */
   m[0][0] = m00; 
   m[0][1] = m01; 
   m[0][2] = m02; 
   m[0][3] = m03;

   /*
    * second row
    */
   m[1][0] = m10; 
   m[1][1] = m11; 
   m[1][2] = m12; 
   m[1][3] = m13;

   /*
    * third row
    */
   m[2][0] = m20; 
   m[2][1] = m21; 
   m[2][2] = m22; 
   m[2][3] = m23;

   /*
    * fourth row
    */
   m[3][0] = m30; 
   m[3][1] = m31; 
   m[3][2] = m32; 
   m[3][3] = m33;
} /* end of SPW_InitializeMatrix */
