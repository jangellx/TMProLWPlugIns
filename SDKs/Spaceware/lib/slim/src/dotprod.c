/*----------------------------------------------------------------------------
 *
 *  dotprod.c -- calculate dot product of two vectors
 *
 * $Id: dotprod.c,v 1.7 1998/03/05 13:18:23 jeffl Exp $
 *
 *----------------------------------------------------------------------------
 *
 * (C) 1998 Spacetec IMC Corporation ("Spacetec").
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies. Permission to modify this software is granted
 * and Spacetec will support such modifications only if said modifications are
 * approved by Spacetec.
 *
 *---------------------------------------------------------------------------*/
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: dotprod.c,v 1.7 1998/03/05 13:18:23 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*----------------------------------------------------------------------------
 *
 * float SPW_DotProduct( float vectorA[3], float vectorB[3] )
 *
 * Args: vectorA (r/o) -first vector
 *       vectorB (r/o) -second vector
 *
 * Return Value:
 *    Dot product of A onto B
 *
 * Description:
 *   Calculates the dot product of two vectors.
 *
 * Notes:
 *
 *--------------------------------------------------------------------------*/
DLL_EXPORT float
SPW_DotProduct( float vectorA[3], float vectorB[3] )
{
   return( (vectorA[0] * vectorB[0]) + (vectorA[1] * vectorB[1]) +
           (vectorA[2] * vectorB[2]) );
} /* end of SPW_DotProduct */
