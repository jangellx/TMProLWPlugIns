/*-----------------------------------------------------------------------------
 *
 *  printvec.c -- Prints out a vector.
 *
 *  $Id: printvec.c,v 1.8 1998/03/05 13:18:46 jeffl Exp $
 *
 *  Prints out a vector to the standard output.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: printvec.c,v 1.8 1998/03/05 13:18:46 jeffl Exp $";

#include <stdio.h>
#include <stdlib.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_PrintVector( char *label, float v[3] )
 *
 * Args:
 *   label (r/o) -an identifying label to print first
 *   v     (r/o) -the vector whose contents are to be printed (3 floats)
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Prints the contents of a vector to stdout.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_PrintVector( char *label, float v[3] )
{
#ifndef WIN32
   printf ("\n%s:\n[%f %f %f]\n", label, v[0], v[1], v[2]);
#endif /* #ifndef WIN32 */
} /* end of SPW_PrintVector */

