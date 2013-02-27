/*-----------------------------------------------------------------------------
 *
 *  orthnorm.c -- Orthonormalizes the upper 3x3 rotation part of a matrix.
 *
 *  $Id: orthnorm.c,v 1.9 1998/03/05 13:18:43 jeffl Exp $
 *
 *  Orthonormalizes the upper 3x3 rotation part of a matrix.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: orthnorm.c,v 1.9 1998/03/05 13:18:43 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * int SPW_OrthoNormMatrix( Matrix m )
 *
 * Args: 
 *   args: m (r/w) -matrix whose rotation part (upper 3x3) is to 
 *                  be orthonormalized
 *
 * Return Value:
 *   returns 0 if successful.
 *   returns -1 and converts the matrix to the identity matrix if unsuccessful.
 *
 * Description:
 *   Orthonormalizes the rotation part of the matrix. (Gram-Schmidt)
 *
 * Notes:
 *   This function could blow up if the length of u or f is zero.  This is
 *   unlikely to occur if an initialized and properly constructed rotation
 *   matrix is passed in; however no error checking is done to ensure that it
 *   doesn't divide by zero.  Caveat emptor!
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT int
SPW_OrthoNormMatrix( Matrix m )
{
   float dot;       /* dot product result */
   float x,y,z;     /* difference vector  */   
   float fx,fy,fz;  /* forward vector     */
   float ux,uy,uz;  /* up vector          */
   float sx,sy,sz;  /* side vector        */
   float len;       /* vector length      */
   float det;       /* matrix determinant */
   int tries;       /* loop counter       */


   for (tries = 0;
        (tries < 5) && (fabs (SPW_MatrixDeterminant (m) - 1.0) > 1e-6);
        tries++)
      {
      /*
       * Notation: f: forward vector (3rd row (z))
       *           u:   up    vector (2nd row (y))
       *           s:  side   vector (1st row (x))
       */
      fx = m[2][0]; 
      fy = m[2][1]; 
      fz = m[2][2];
      
      ux = m[1][0]; 
      uy = m[1][1]; 
      uz = m[1][2];

      /*
       * Unitize f (make it a unit vector)
       */
      len = (float)sqrt ((double)(fx * fx) + (fy * fy) + (fz * fz));
      fx /= len; 
      fy /= len; 
      fz /= len;

      /*
       * Subtract off the projection of up on forward
       */
      dot = (fx * ux) + (fy * uy) + (fz * uz); /* projection */
       
      /*
       * x, y, and z represent the difference vector
       */
      x = dot * fx; 
      y = dot * fy; 
      z = dot * fz;
       
      /*
       * Subtract difference vector from up vector
       */
      ux -= x; 
      uy -= y; 
      uz -= z; 

      /*
       * Unitize u
       */
      len = (float)sqrt ((double)(ux * ux) + (uy * uy) + (uz * uz));
      ux /= len; 
      uy /= len; 
      uz /= len;

      /*
       * s = u X f: calculate the cross product
       */
      sx = (uy * fz) - (fy * uz);
      sy = (uz * fx) - (fz * ux);
      sz = (ux * fy) - (fx * uy);
      
      /*
       * Unitize s
       */
      len = (float)sqrt ((double)(sx * sx) + (sy * sy) + (sz * sz));
      sx /= len; 
      sy /= len; 
      sz /= len;
      
      /*
       * Refill the matrix
       */
      m[2][0] = fx; 
      m[2][1] = fy; 
      m[2][2] = fz;
      
      m[1][0] = ux; 
      m[1][1] = uy; 
      m[1][2] = uz;

      m[0][0] = sx; 
      m[0][1] = sy; 
      m[0][2] = sz;
      } /* end for number of tries */

   if (fabs (det = SPW_MatrixDeterminant (m) - 1.0f) > 1e-6) 
      {
      SPW_MakeIdentityMatrix (m);
      return -1;
      }
   return 0;
} /* end of SPW_OrthoNormMatrix */
