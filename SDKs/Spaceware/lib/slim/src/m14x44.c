/*-----------------------------------------------------------------------------
 *
 *  m14x44.c -- 1x4 * 4x4 matrix multiply.
 *
 *  $Id: m14x44.c,v 1.7 1998/03/05 13:18:32 jeffl Exp $
 *
 *  Multiplies a 1x4 vector by a 4x4 matrix, to give a 4x1 resultant vector.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: m14x44.c,v 1.7 1998/03/05 13:18:32 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_Mult14x44( float result[3], float vector[3], Matrix m )
 *
 * Args:
 *   result (w/o)   -the resultant vector of the multiplication
 *   vector (r/o)   -the original vector (assumes 3 elements + fourth =1.0)
 *   m      (r/o)   -the matrix to multiply vector by
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Multiply a vector by a Matrix
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_Mult14x44( float result[3], float vector[3], Matrix m )
{
   float t[4];    /* temporary vector */

   /*
    * Multiply rows by columns to give resultant column
    */
   t[0] = (vector[0] * m[0][0]) + (vector[1] * m[1][0]) + (vector[2] * m[2][0])
          + m[3][0];
   t[1] = (vector[0] * m[0][1]) + (vector[1] * m[1][1]) + (vector[2] * m[2][1])
          + m[3][1];
   t[2] = (vector[0] * m[0][2]) + (vector[1] * m[1][2]) + (vector[2] * m[2][2])
          + m[3][2];

   result[0] = t[0];            
   result[1] = t[1];     
   result[2] = t[2];     
} /* end of SPW_Mult14x44 */

