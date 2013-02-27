/*-----------------------------------------------------------------------------
 *
 *  crosprod.c -- Calculate vector cross product
 *
 *  $Id: crosprod.c,v 1.7 1998/03/05 13:18:22 jeffl Exp $
 *
 *  Module includes a function to calculate the cross product of two vectors
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: crosprod.c,v 1.7 1998/03/05 13:18:22 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

#define VCROSSi(v1, v2)  (v1[1] * v2[2] - v2[1] * v1[2])
#define VCROSSj(v1, v2)  (v1[2] * v2[0] - v2[2] * v1[0])
#define VCROSSk(v1, v2)  (v1[0] * v2[1] - v2[0] * v1[1])

/*-----------------------------------------------------------------------------
 *
 * void SPW_CrossProduct( float result[3], float vectorA[3], float vectorB[3] )
 *
 * Args: result  (w/o) -the resultant vector = vectorA x vectorB
 *       vectorA (r/o) -first vector
 *       vectorB (r/o) -second vector
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Calculates the cross product of two vectors.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_CrossProduct( float result[3], float vectorA[3], float vectorB[3] )
{
   float fTemp[3];  /* temporary vector */

   fTemp[0] = VCROSSi(vectorA,vectorB);
   fTemp[1] = VCROSSj(vectorA,vectorB);
   fTemp[2] = VCROSSk(vectorA,vectorB);

   result[0] = fTemp[0];            
   result[1] = fTemp[1];     
   result[2] = fTemp[2];     
} /* end of SPW_CrossProduct */
