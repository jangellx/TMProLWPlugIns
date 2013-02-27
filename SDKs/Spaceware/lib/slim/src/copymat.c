/*-----------------------------------------------------------------------------
 *
 *  copymat.c -- copies a matrix
 *
 *  $Id: copymat.c,v 1.7 1998/03/05 13:18:21 jeffl Exp $
 *
 *  Copies a source matrix into a destination matrix with no changes
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: copymat.c,v 1.7 1998/03/05 13:18:21 jeffl Exp $";

#include <stdlib.h>
#include <string.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_CopyMatrix( Matrix destination, Matrix source )
 *
 * Args: destination  (w/o) -the copy of the original matrix
 *       source       (r/o) -the matrix to be copied
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Copies the source matrix into the destination matrix with no changes.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_CopyMatrix( Matrix destination, Matrix source )
{
   memcpy ((char *)destination, (char *)source, sizeof (Matrix));
} /* end of SPW_CopyMatrix */


