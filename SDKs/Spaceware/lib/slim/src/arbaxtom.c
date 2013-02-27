/*-----------------------------------------------------------------------------
 *
 *  arbaxtom.c -- Calculate the arbitrary axis rotation matrix
 *
 *  $Id: arbaxtom.c,v 1.8 1998/03/05 13:18:18 jeffl Exp $
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
  
static char cvsId[]="(C) 1997 Spacetec IMC Corporation: $Id: arbaxtom.c,v 1.8 1998/03/05 13:18:18 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

#define MIN_RADIANS .0001

/*---------------------------------------------------------------------------
 * 
 *  void SPW_ArbitraryAxisToMatrix( Matrix m, float arbaxis[3],
 *                                  float rot_scale )
 *
 *   Args: 
 *      m         (w/o)  -matrix to be calculated
 *      arbaxis   (r/o)  -arbitrary axis, the length is amount of rotation
 *      rot_scale (r/o)  -an arbitrary scale factor to be applied to the
 *                        amount of rotation
 *
 *   Return Value:
 *      None
 *
 *   Description:
 *     Calculate the rotation matrix that corresponds to the rotation specified
 *     by an arbitrary axis whose components determine the direction of the 
 *     axis and whose length indicates the amount of rotation (this is what 
 *     physicists call the infinitesimal angular displacement and what we call 
 *     the Spaceball rotation vector).  
 *
 *     Source: Robot Manipulators: Mathematics, Programming and Control
 *             by Richard Paul, pg. 28
 *
 *   Notes:
 *
 *   This is the matrix m:
 *
 *  --                                                                      --
 * | kx*kx*vers0 + cos0     ky*kx*vers0 - kz*sin0  kz*kx*vers0 + ky*sin0   0  |
 * | kx*ky*vers0 + kz*sin0  ky*ky*vers0 + kz*cos0  kz*ky*vers0 - kx*sin0   0  |
 * | kx*kz*vers0 - ky*sin0  ky*kz*vers0 + kx*sin0  kz*kz*vers0 + cos0      0  |
 * |           0                         0                    0            1  |
 *  --                                                                      --
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_ArbitraryAxisToMatrix( Matrix m, float arbaxis[3], float rot_scale )
{
   float radians;       /* length of the arbitrary axis vector            */
   float invlength;     /* 1/length                                       */
   float x_rotvec;      /* x component of rotation vector (k-sub-x or kx) */
   float y_rotvec;      /* y component of rotation vector (k-sub-y or ky) */
   float z_rotvec;      /* z component of rotation vector (k-sub-z or kz) */
   float sin0;          /* sine of rotation angle theta                   */
   float cos0;          /* cosine of rotation angle theta                 */
   float vers0;         /* versine of rotation angle theta;               */
                        /* vers(theta) = 1 - sin(theta)                   */
   float kxvers0;       /* kx * vers(theta)                               */
   float kyvers0;       /* ky * vers(theta)                               */
   float kzvers0;       /* kz * vers(theta)                               */
   float kxsin0;        /* kx * sin(theta)                                */
   float kysin0;        /* ky * sin(theta)                                */
   float kzsin0;        /* kz * sin(theta)                                */

   /*
    * Find the length of the vector.
    */
   radians = (float)sqrt (arbaxis[0] * arbaxis[0]
                          + arbaxis[1] * arbaxis[1] 
                          + arbaxis[2] * arbaxis[2]);

   /*
    * If the vector has zero length - return the identity matrix.
    */
   if (SPW_ABS (radians) < MIN_RADIANS) 
      {
      SPW_MakeIdentityMatrix (m);
      return;
      }

   /*
    * Normalize the rotation vector.
    */
   invlength = 1 / radians;
   x_rotvec = arbaxis[0] * invlength;
   y_rotvec = arbaxis[1] * invlength;
   z_rotvec = arbaxis[2] * invlength;
   radians *= rot_scale;   /* calculate the number of radians */

   /*
    * Calculate angles
    */
   sin0 = (float)sin (radians);
   cos0 = (float)cos (radians);
   vers0 = (float)(1.0 - cos0);

   /*
    * Calculate commonly used values.
    */
   kxvers0 = x_rotvec * vers0;  /* kx * vers(theta) */
   kyvers0 = y_rotvec * vers0;  /* ky * vers(theta) */
   kzvers0 = z_rotvec * vers0;  /* kz * vers(theta) */
   kxsin0  = x_rotvec * sin0;   /* kx * sin(theta)  */
   kysin0  = y_rotvec * sin0;   /* ky * sin(theta)  */
   kzsin0  = z_rotvec * sin0;   /* kz * sin(theta)  */

   /*
    * Calculate matrix elements (see above descriptions)
    */
   m[0][0] = x_rotvec * kxvers0 + cos0;
   m[0][1] = y_rotvec * kxvers0 - kzsin0;
   m[0][2] = z_rotvec * kxvers0 + kysin0;
   m[0][3] = (float)0.0;
   m[1][0] = x_rotvec * kyvers0 + kzsin0;
   m[1][1] = y_rotvec * kyvers0 + cos0;
   m[1][2] = z_rotvec * kyvers0 - kxsin0;
   m[1][3] = (float)0.0;
   m[2][0] = x_rotvec * kzvers0 - kysin0;
   m[2][1] = y_rotvec * kzvers0 + kxsin0;
   m[2][2] = z_rotvec * kzvers0 + cos0;
   m[2][3] = (float)0.0;
   m[3][0] = (float)0.0;
   m[3][1] = (float)0.0;
   m[3][2] = (float)0.0;
   m[3][3] = (float)1.0;

} /* end of SPW_ArbitraryAxisToMatrix */


