/*-----------------------------------------------------------------------------
 *  matdeter.c -- Finds matrix determinant.
 *
 *  $Id: matdeter.c,v 1.7 1998/03/05 13:18:36 jeffl Exp $
 *
 *  Finds the determinant of the upper 3x3 portion of the given matrix.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: matdeter.c,v 1.7 1998/03/05 13:18:36 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 *  float SPW_MatrixDeterminant( Matrix m )
 *
 *  Args: 
 *    m (r/o) -matrix whose determinant (of upper 3x3 only) is to
 *             be calculated
 * 
 *  Return Value:
 *    Determinant of the upper 3x3 part of m
 *
 *  Description:
 *    returns determinant of the upper 3x3 part of a Matrix (rotations).
 *
 *  Notes:
 *    (From Strang) The determinant of A is a combination of row i and 
 *    the cofactors of row i:  
 *         det A = ai1Ai1 + ai2Ai2 + ... + ainAin.
 *    The cofactor Aij is the determinant of Mij with the correct sign:
 *         Aij = (-1)**i+j det Mij.
 *    Mij is formed by deleting row i and column j of A.
 * 
 *---------------------------------------------------------------------------*/
DLL_EXPORT float
SPW_MatrixDeterminant( Matrix m )
{

   return   m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1])
          + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2])
          + m[0][2] * (m[1][0] * m[2][1] - m[2][0] * m[1][1]);
} /* end of SPW_MatrixDeterminant */
