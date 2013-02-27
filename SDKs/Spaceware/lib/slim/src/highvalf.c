/*-----------------------------------------------------------------------------
 *
 *  highvalf.c -- finds largest absolute value in array subset
 *
 *  $Id: highvalf.c,v 1.7 1998/03/05 13:18:26 jeffl Exp $
 *
 *  This function finds the largest absolute value in the first n elements of
 *  an array, and zeros out the others.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: highvalf.c,v 1.7 1998/03/05 13:18:26 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_HighValuePassFilter( float *array, int array_length )
 *
 *  Args: 
 *    array         (r/w) - the array to filter 
 *    array_length  (r/o) - the number of elements of "array" to look at
 *
 *  Return Value:
 *    None
 *
 *  Description:
 *    Zeros all but the largest (in absolute value) element in "array".
 *    Only looks at the first "array_length" elements.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_HighValuePassFilter( float *array, int array_length )
{
   float *largest = array;  /* keep track of largest element;          */
                            /* initially point at first array element  */
   float newabs;            /* temporary floating point absolute value */
   float large;             /* comparison value                        */

   large = (float) fabs( *array );  /* set large to first array element */

   /*
    * Look at array_length elements.
    */
   while (--array_length > 0)       
      {
      /*
       * Get new value from next array element.
       */
      newabs = (float) fabs(*++array);
      if (newabs > large)           /* if newabs is bigger... */ 
         {
         *largest = 0.0f;           /* zero out previous winner */
         large = newabs;            /* move large pointer */
         largest = array;           /* make largest point to new winner */
         }
      else
         {
         *array = 0.0f;             /* otherwise, just zero out the element */
         }
      }
} /* end of SPW_HighValuePassFilter */


