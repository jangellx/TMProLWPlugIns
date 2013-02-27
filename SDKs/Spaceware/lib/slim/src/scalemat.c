/*-----------------------------------------------------------------------------
 *
 *  scalemat.c -- Scales a matrix by scale_factor.
 *
 *  $Id: scalemat.c,v 1.8 1998/03/05 13:18:48 jeffl Exp $
 *
 *  Scales a matrix by scale_factor.
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
  
static char cvsId[]="(C) 1997 Spacetec IMC Corporation: $Id: scalemat.c,v 1.8 1998/03/05 13:18:48 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_ScaleMatrix( Matrix result, Matrix m, float  scale_factor )
 *
 * Args: 
 *   result       (w/o) -resultant matrix after scaling m by scalefactor
 *   m            (r/o) -original matrix
 *   scale_factor (r/o) -factor to scale m by
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Scales a matrix by scale_factor
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_ScaleMatrix( Matrix result, Matrix m, float  scale_factor )
{
   Matrix s;  /* straight scale matrix */

   SPW_MakeIdentityMatrix (s);
   
   /*
    * Multiply the diagonal elements
    */
   s[0][0] = s[1][1] = s[2][2] = scale_factor;
   
   /*
    * Perform the matrix multiplication
    */
   SPW_Mult44x44 (result, m, s); 
} /* end of SPW_ScaleMatrix */
