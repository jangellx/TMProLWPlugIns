/*-----------------------------------------------------------------------------
 *
 *  printsbd.c -- Print Spaceball data.
 *
 *  $Id: printsbd.c,v 1.9 1998/03/05 13:18:45 jeffl Exp $
 *
 *  Print the Spaceball data in a useful labeled manner.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: printsbd.c,v 1.9 1998/03/05 13:18:45 jeffl Exp $";

#include <stdio.h>
#include <stdlib.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_PrintSpaceballData( char *label, float sbdata[7] )
 *
 * Args: 
 *   label  (r/o) -an identifying label to print first
 *   sbdata (r/o) -an array of sb data in the "standard" order
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Print the Spaceball data in a useful labeled manner.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_PrintSpaceballData( char *label, float sbdata[7] )
{
#ifndef WIN32
   if (label && *label)
      {
      printf (
      "\n%s:\nPeriod: %.3f TX %.3f TY %.3f TZ %.3f RX %.3f RY %.3f RZ %.3f\n",
      label, sbdata[6], sbdata[0], sbdata[1], sbdata[2], sbdata[3], sbdata[4], sbdata[5]);
      }
   else
      {
      printf("\nPeriod: %.3f TX %.3f TY %.3f TZ %.3f RX %.3f RY %.3f RZ %.3f\n",
             sbdata[6], sbdata[0], sbdata[1], sbdata[2], sbdata[3], sbdata[4], sbdata[5]);
      }
#endif /* WIN32 */
} /* end of SPW_PrintSpaceballData */

