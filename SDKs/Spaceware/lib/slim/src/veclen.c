/*-----------------------------------------------------------------------------
 *
 *  veclen.c -- Calculates a vector's length.
 *
 *  $Id: veclen.c,v 1.7 1998/03/05 13:18:51 jeffl Exp $
 *
 *  Module includes a function to calculate a vector's length.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: veclen.c,v 1.7 1998/03/05 13:18:51 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * float SPW_VectorLength( float vector[3] )
 *
 * Args: 
 *   vector (r/o) -vector whose length is to be calculated
 *
 * Return Value:
 *   length of passed in vector
 *
 * Description:
 *   Calculates length of passed in vector.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT float
SPW_VectorLength( float vector[3] )
{
   return (float)(sqrt (SPW_SQ (vector[0]) + SPW_SQ (vector[1]) +
                        SPW_SQ (vector[2])));
} /* end of SPW_VectorLength */

