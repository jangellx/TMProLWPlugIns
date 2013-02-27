/*-----------------------------------------------------------------------------
 *
 *  getmatsc.c  -- calculate the matrix scale factor
 *
 *  $Id: getmatsc.c,v 1.8 1998/03/05 13:18:24 jeffl Exp $
 *
 *  Module includes a function to calculate scale factor of a matrix
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: getmatsc.c,v 1.8 1998/03/05 13:18:24 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * float SPW_GetMatrixScale( Matrix m )
 *
 * Args: 
 *   m (r/o) -matrix to get scale factor from
 *
 * Return Value:
 *   factor the rotation part of the matrix is scaled by
 *
 * Description:
 *   Look at the scaling of each row and take the average.  Should be the
 *   same in every direction, but you never know!
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT float
SPW_GetMatrixScale( Matrix m )
{
   return ( (float)( sqrt (SPW_SQ (m[0][0]) + SPW_SQ (m[0][1]) + 
                           SPW_SQ (m[0][2])) +
                     sqrt (SPW_SQ (m[1][0]) + SPW_SQ (m[1][1]) + 
                           SPW_SQ (m[1][2])) +
                     sqrt (SPW_SQ (m[2][0]) + SPW_SQ (m[2][1]) + 
                           SPW_SQ (m[2][2])) ) ) / 3.0f;

} /* end of SPW_GetMatrixScale */

