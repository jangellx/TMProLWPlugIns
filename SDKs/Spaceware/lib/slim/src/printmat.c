/*-----------------------------------------------------------------------------
 *
 *  printmat.c -- Prints the matrix.
 *
 *  $Id: printmat.c,v 1.8 1998/03/05 13:18:44 jeffl Exp $
 *
 *  Prints the given matrix to the standard output.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: printmat.c,v 1.8 1998/03/05 13:18:44 jeffl Exp $";

#include <stdio.h>
#include <stdlib.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_PrintMatrix( char *label, Matrix m )
 *
 * Args:
 *   label (r/o) -an identifying label to print first
 *   m     (r/o) -the matrix whose contents are to be printed
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Prints the contents of a matrix to stdout.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_PrintMatrix( char *label, Matrix m )
{
#ifndef WIN32
   printf (
    "\n%s:\n[[%f %f %f %f]\n [%f %f %f %f]\n [%f %f %f %f]\n [%f %f %f %f]]\n",
           label,
           m[0][0], m[0][1], m[0][2], m[0][3],
           m[1][0], m[1][1], m[1][2], m[1][3],
           m[2][0], m[2][1], m[2][2], m[2][3],
           m[3][0], m[3][1], m[3][2], m[3][3]);
#endif /*ifndef WIN32 */
} /* end of SPW_PrintMatrix */
