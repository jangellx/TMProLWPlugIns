/*-----------------------------------------------------------------------------
 *
 *  compmat.c -- compares two matrices
 *
 *  $Id: compmat.c,v 1.8 1998/03/05 13:18:20 jeffl Exp $
 *
 *  Module includes a function to compare two matrices.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: compmat.c,v 1.8 1998/03/05 13:18:20 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * int SPW_CompareMatrices( Matrix m1, Matrix m2 )
 *
 * Args: 
 *   m1 (r/o) -first matrix
 *   m2 (r/o) -second matrix 
 *
 * Return Value:
 *   0 if matrices are identical, 1 if not
 *
 * Description:
 *   Compares two matrices.
 *---------------------------------------------------------------------------*/
DLL_EXPORT int
SPW_CompareMatrices( Matrix m1, Matrix m2 )
{
   int i, j;   /* loop counters */

   for (i = 0; i < 4; i++)
      {
      for (j = 0; j < 4; j++)
         {
         if (fabs ((double)(m1[i][j] - m2[i][j])) > 0.0001 ) 
            {
            return 1;
            }
         }
      }

   return 0;
} /* end of SPW_CompareMatrices */
