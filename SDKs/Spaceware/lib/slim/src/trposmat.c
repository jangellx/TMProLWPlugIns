/*-----------------------------------------------------------------------------
 *
 *  trposmat.c -- Return the transpose of a matrix.
 *
 *  $Id: trposmat.c,v 1.8 1998/03/05 13:18:50 jeffl Exp $
 *
 *  Returns the transpose of the input matrix.
 *
 *-----------------------------------------------------------------------------
 *
 * (C) 1998 Spacetec IMC Corporation ("Spacetec").  Permission to use, copy, 
 * modify and distribute this software for all purposes and without fees is
 * hereby granted provided that this copyright notice appears in all copies.
 * Permission to modify this software is granted and Spacetec will support such
 * modifications only if said modifications are approved by Spacetec.
 *
 */
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: trposmat.c,v 1.8 1998/03/05 13:18:50 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_TransposeMatrix( Matrix result, Matrix m )
 *
 * Args:
 *   result  (w/o) - matrix to be calculated 
 *   m       (r/o) - matrix to be transposed
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Transpose a matrix.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_TransposeMatrix( Matrix result, Matrix m )
{
   float tmp;  /* temporary value to hold matrix element move */

   /*
    * Diagonal elements are simply moved across
    */
   result[0][0] = m[0][0];
   result[1][1] = m[1][1]; 
   result[2][2] = m[2][2]; 
   result[3][3] = m[3][3]; 

   /*
    * Now move the rest
    */
   tmp=m[1][0]; 
   result[1][0]=m[0][1]; 
   result[0][1]=tmp;

   tmp=m[2][0]; 
   result[2][0]=m[0][2]; 
   result[0][2]=tmp;

   tmp=m[3][0]; 
   result[3][0]=m[0][3]; 
   result[0][3]=tmp;

   tmp=m[2][1]; 
   result[2][1]=m[1][2]; 
   result[1][2]=tmp;

   tmp=m[3][1]; 
   result[3][1]=m[1][3]; 
   result[1][3]=tmp;

   tmp=m[3][2]; 
   result[3][2]=m[2][3];
   result[2][3]=tmp;
} /* end of SPW_TransposeMatrix */

