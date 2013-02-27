/*-----------------------------------------------------------------------------
 *
 *  gettranm.c -- gets translation part of matrix
 *
 *  $Id: gettranm.c,v 1.7 1998/03/05 13:18:25 jeffl Exp $
 *
 *  Module includes a function to get translation part of matrix
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: gettranm.c,v 1.7 1998/03/05 13:18:25 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_GetTranslationMatrix( Matrix destination, Matrix source )
 *
 * Args: 
 *   destination (w/o) -matrix that is to become a translation matrix
 *   source      (r/o) -matrix whose translation is to be extracted for
 *                      the destination matrix
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Creates a translation matrix by extracting the Translation (last row) 
 *   part of an existing transformation matrix.  The orientation part is
 *   left at identity.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_GetTranslationMatrix( Matrix destination, Matrix source )
{
   SPW_MakeIdentityMatrix (destination);
   destination[3][0] = source[3][0];
   destination[3][1] = source[3][1];
   destination[3][2] = source[3][2];
} /* end of SPW_GetTranslationMatrix */
