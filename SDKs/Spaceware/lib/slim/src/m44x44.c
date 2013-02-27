/*-----------------------------------------------------------------------------
 *
 *  m44x44.c -- 4x4 * 4x4 matrix multiply.
 *
 *  $Id: m44x44.c,v 1.7 1998/03/05 13:18:35 jeffl Exp $
 *
 *  Multiplies 4x4 by a 4x4 matrix, yielding a 4x4 matrix.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: m44x44.c,v 1.7 1998/03/05 13:18:35 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_Mult44x44( Matrix result, Matrix m1, Matrix m2 )
 * 
 * Args: 
 *   result (w/o)  -resultant matrix = [m1][m2]
 *   m1     (r/o)  -first matrix
 *   m2     (r/o)  -second matrix
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Multiply two 4x4 matrices.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_Mult44x44( Matrix result, Matrix m1, Matrix m2 )
{
   Matrix t;  /* temporary 4x4 matrix */
   int i, j;  /* counters             */

   /*
    * Do the matrix multiply
    */
   for (i = 0; i < 4; i++)
      {
      for (j = 0; j < 4; j++)
         {
         t[i][j] = m1[i][0] * m2[0][j] +
                   m1[i][1] * m2[1][j] +
                   m1[i][2] * m2[2][j] +
                   m1[i][3] * m2[3][j];
         }
      }

   /*
    * Copy the temporary matrix to the result matrix
    */
   SPW_CopyMatrix (result, t);  
} /* end of SPW_Mult44x44 */

