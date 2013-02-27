/*-----------------------------------------------------------------------------
 *
 *  m13x33.c -- 1x3 * 3x3 matrix multiply.
 *
 *  $Id: m13x33.c,v 1.7 1998/03/05 13:18:31 jeffl Exp $
 *
 *  Multiplies 1x3 by 3x3 matrices, yielding a 3x1 result.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: m13x33.c,v 1.7 1998/03/05 13:18:31 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 * 
 * void SPW_Mult13x33( float result[3], float vector[3], Matrix m )
 *
 *  args: result  (w/o)  -resultant vector = [vector][m]
 *        vector  (r/o)  -original vector
 *        m       (r/o)  -matrix to multiply vector by
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Multiply a vector by a 3x3 matrix (rotation part of a 4x4).
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_Mult13x33( float result[3], float vector[3], Matrix m )
{
   float t[3]; /* temporary vector */

   t[0]= (vector[0] * m[0][0]) + (vector[1] * m[1][0]) + (vector[2] * m[2][0]);
   t[1]= (vector[0] * m[0][1]) + (vector[1] * m[1][1]) + (vector[2] * m[2][1]);
   t[2]= (vector[0] * m[0][2]) + (vector[1] * m[1][2]) + (vector[2] * m[2][2]);

   /*
    * Copy results from temporary vector
    */
   result[0] = t[0];          
   result[1] = t[1];     
   result[2] = t[2];
} /* end of SPW_Mult13x33 */
