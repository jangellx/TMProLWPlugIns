/*-----------------------------------------------------------------------------
 *
 *  mkidentm.c -- Make the identity matrix.
 *
 *  $Id: mkidentm.c,v 1.8 1998/03/05 13:18:40 jeffl Exp $
 *
 *  Includes function to build the identity matrix.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: mkidentm.c,v 1.8 1998/03/05 13:18:40 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_MakeIdentityMatrix( Matrix m )
 *
 * Args:
 *   m (w/o) -matrix that is to become an identity matrix
 *
 * Return Value:
 *   None
 *
 * Description:
 *   Turns the passed Matrix into an identity matrix.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_MakeIdentityMatrix( Matrix m )
{
   static Matrix I= {
      { 1.0f, 0.0f, 0.0f, 0.0f},
      { 0.0f, 1.0f, 0.0f, 0.0f},
      { 0.0f, 0.0f, 1.0f, 0.0f},
      { 0.0f, 0.0f, 0.0f, 1.0f}};

   SPW_CopyMatrix (m, I);
} /* end of SPW_MakeIdentityMatrix */


