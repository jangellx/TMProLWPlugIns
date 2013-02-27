/*-----------------------------------------------------------------------------
 *
 *  invmat.c -- Inverts the given transformation matrix.
 *
 *  $Id: invmat.c,v 1.7 1998/03/05 13:18:28 jeffl Exp $
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: invmat.c,v 1.7 1998/03/05 13:18:28 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_InvertMatrix( Matrix result, Matrix m )
 * 
 * Args:
 *   result   (w/o)  - inverse of matrix m
 *   m        (r/o)  - matrix to be inverted
 *
 * Description:
 *   Invert an orthogonal transformation matrix.  Tranposes the rotation
 *   part.  Invert the last row by negating the translation and dotting each
 *   element with its respective (untransposed) row.
 *
 *   THIS METHOD DOES NOT WORK WITH ANY OLD MATRIX--IT MUST BE ONE OF "OUR"
 *   MATRICES (ORTHONORMAL MATRIX).  That is, I am, obviously, 
 *   not doing Gaussian elimination here, I am just taking advantage of some 
 *   special properties of orthonormal transformation matrices.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_InvertMatrix( Matrix result, Matrix m )
{
   Matrix tmp; /* Temporary storage for the  result matrix */

   /*
    * Transpose the rotation
    */
   
   /*
    * First Row
    */
   tmp[0][0] = m[0][0]; 
   tmp[0][1] = m[1][0]; 
   tmp[0][2] = m[2][0];

   /*
    * Second Row
    */
   tmp[1][0] = m[0][1]; 
   tmp[1][1] = m[1][1]; 
   tmp[1][2] = m[2][1];

   /*
    * Third Row
    */
   tmp[2][0] = m[0][2]; 
   tmp[2][1] = m[1][2]; 
   tmp[2][2] = m[2][2];

   /*
    * Zero the last column
    */
   tmp[0][3] = 0.0f;
   tmp[1][3] = 0.0f;
   tmp[2][3] = 0.0f;
   tmp[3][3] = 1.0f;

   /*
    * Translation is minus the dot product of trans and rotation portions
    */
   tmp[3][0]= -(m[3][0] * m[0][0]) - (m[3][1] * m[0][1]) - (m[3][2] * m[0][2]);
   tmp[3][1]= -(m[3][0] * m[1][0]) - (m[3][1] * m[1][1]) - (m[3][2] * m[1][2]);
   tmp[3][2]= -(m[3][0] * m[2][0]) - (m[3][1] * m[2][1]) - (m[3][2] * m[2][2]);

   /*
    * Copy temporary matrix to result
    */
   SPW_CopyMatrix (result, tmp);
} /* end of SPW_InvertMatrix */


