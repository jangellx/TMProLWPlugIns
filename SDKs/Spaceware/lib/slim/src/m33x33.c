/*-----------------------------------------------------------------------------
 *
 *  m33x33.c -- multiplies 3x3 matrices
 *
 *  $Id: m33x33.c,v 1.7 1998/03/05 13:18:34 jeffl Exp $
 *
 *  Mulitply 3x3 by 3x3 rotation portions of 4x4 matrices.  Leaves the rest (eg
 *  transformation part/homogeneous part) alone.
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
  
static char rcsid[] = "(C) 1998 Spacetec IMC Corporation: $Id: m33x33.c,v 1.7 1998/03/05 13:18:34 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_Mult33x33( Matrix result, Matrix m1, Matrix m2 )
 *
 * Args:
 *   result (w/o)  -result matrix = [m1(3x3)][m2(3x3)] +last row & column
 *   m1     (r/o)  -first matrix
 *   m2     (r/o)  -second matrix
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Multiply the rotation parts (upper 3x3) of two matrices.  Copy the 
 *   unaffected balance (last row and last column) of m1 to the result.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_Mult33x33( Matrix result, Matrix m1, Matrix m2 )
{
   Matrix t; /* temporary matrix */
   int i, j; /* counters         */

   /*
    * do the matrix multiply
    */
   for (i = 0; i < 3; i++)
      {
      for (j = 0; j < 3; j++)
         {
         t[i][j] = m1[i][0] * m2[0][j] +
                   m1[i][1] * m2[1][j] +
                   m1[i][2] * m2[2][j];
         }
      }

   /*
    * Copy temporary matrix into result matrix
    */
   for (i=0; i<3; i++)
      {
      for (j=0; j<3; j++)
         {
         result[i][j] = t[i][j];
         }
      }

   /*
    * Copy rest of m1 over to result w/o modification
    */
   result[0][3] = m1[0][3];
   result[1][3] = m1[1][3];
   result[2][3] = m1[2][3];
   result[3][0] = m1[3][0];
   result[3][1] = m1[3][1];
   result[3][2] = m1[3][2];
   result[3][3] = m1[3][3];
} /* end of SPW_Mult33x33 */

