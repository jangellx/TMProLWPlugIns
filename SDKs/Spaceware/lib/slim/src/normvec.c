/*-----------------------------------------------------------------------------
 *
 *  normvec.c -- Normalizes a vector.
 *
 *  $Id: normvec.c,v 1.10 1998/03/05 13:18:42 jeffl Exp $
 *
 *  Normalizes the passed in vector.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: normvec.c,v 1.10 1998/03/05 13:18:42 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_NormalizeVector( float result[3], float vector[3] )
 *
 * Args: 
 *   result (w/o) -vector after normalization
 *   vector (r/o) -original vector before normalization
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Normalizes the passed in vector.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_NormalizeVector( float result[3], float vector[3] )
{
   float lensq;  /* square of the vector components' lengths */ 
   float len;    /* vector magnitude                         */
   
   /*
    * Calculate the square of all the vector component lengths.
    */
   lensq = SPW_SQ (vector[0]) + SPW_SQ (vector[1]) + SPW_SQ (vector[2]); 

   /*
    * Don't do anything if it is already normalized
    */
   if (SPW_ABSDIFF (lensq, 1.0) <0.00001) 
      {
      result[0] = vector[0];
      result[1] = vector[1];
      result[2] = vector[2];
      }
   else if (SPW_ABSDIFF (lensq, 0.0) < 0.00001)    /* check for the zero case */
      {
      result[0] = 0.0f;
      result[1] = 0.0f;
      result[2] = 0.0f;
      }
   else  /* otherwise do the normalization */
      {
      len = (float) sqrt ((double)lensq);
      result[0] = vector[0] / len;
      result[1] = vector[1] / len;
      result[2] = vector[2] / len;
      }
} /* end of SPW_NormalizeVector */
