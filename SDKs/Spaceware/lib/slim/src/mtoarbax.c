/*-----------------------------------------------------------------------------
 *
 *  SPW_MatrixToArbitraryAxis.c -- Create a matrix for rotation.
 *
 *  $Id: mtoarbax.c,v 1.7 1998/03/05 13:18:41 jeffl Exp $
 *
 *  Calculate the arbitrary axis and rotation angle represented in a matrix.
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
  
static char cvsId[] = "(C) 1998 Spacetec IMC Corporation: $Id: mtoarbax.c,v 1.7 1998/03/05 13:18:41 jeffl Exp $";

#include <math.h>

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 *  void SPW_MatrixToArbitraryAxis( float arbaxis[3], float *pangle, Matrix m )
 *
 *  Args: 
 *    arbaxis (w/o)  -arbitrary axis, the length is the amount of rotation
 *    pangle  (w/o)  -ptr to angle of rotation (in radians)
 *    m       (r/o)  -matrix to calculate arbitrary axis from
 *
 *  Return Value:
 *    None
 *
 *  Description:
 *    Calculate the arbitrary axis and rotation angle represented in a matrix.
 *
 *     Source: Robot Manipulators: Mathematics, Programming and Control
 *             by Richard Paul, pp. 
 *
 *  Notes:
 *     the matrix is of the form:
 *       _            _
 *      |  nx ox ax 0  |
 *      |  ny oy ay 0  |
 *      |  nz oz az 0  |
 *      |_ 0  0  0  1 _|
 *
 *    This is the same as the matrix presented in SPW_ArbitraryAxixToMatrix,
 *    with the elements renamed:
 *  --                                                                      --
 * | kx*kx*vers0 + cos0     ky*kx*vers0 - kz*sin0  kz*kx*vers0 + ky*sin0   0  |
 * | kx*ky*vers0 + kz*sin0  ky*ky*vers0 + kz*cos0  kz*ky*vers0 - kx*sin0   0  |
 * | kx*kz*vers0 - ky*sin0  ky*kz*vers0 + kx*sin0  kz*kz*vers0 + cos0      0  |
 * |           0                         0                    0            1  |
 *  --                                                                      --
 *    
 *  You can compute the cosine by summing the diagonal terms on both sides and 
 *  doing some algebraic manipulation you get cos of theta = cos0:
 * 
 *      [0] cos0 = 1/2(nx + oy + az - 1)
 *
 *  Subtracting pairs of off diagonal terms gives:
 *     
 *     [1] oz - ay = 2 * kx * sin0
 *     [2] ax - nz = 2 * ky * sin0
 *     [3] ny - ox = 2 * kz * sin0
 *
 *  Sum, square, and manipulate these equations and you get sin0:
 *
 *      sin0 = +/- 1/2 * sqrt((oz - ay)^2 + (ax - nz)^2 + (ny - ox)^2)
 *
 *    We can define the rotation to be positive about vector k such that
 *    0 <= theta <= 180, by which using the  + case of sin0, then theta is 
 *    defined computing tan0:
 *
 *             sin0         sqrt((oz - ay)^2 + (ax - nz)^2 + (ny - ox)^2)
 *    tan0 =  ------  =   -------------------------------------------------
 *             cos0                    nx + oy + az - 1
 *
 *    Theta, or the angle that we are after, is thus determined by computing
 *    the arctangent of the above equation.
 *
 *    The components of the arbitrary axis,which is also called vector k, are 
 *    derived from [1], [2], and [3], for theta < 90 degrees:
 *
 *         oz - ay          ax - nz          ny - ox
 *    kx = --------    ky = --------    kz = --------
 *         2 * sin0         2 * sin0         2 * sin0
 *  
 *    If theta > 90 degrees, another approach must be taken du to the sign 
 *    change in sin0.  Equate the diagonal elements of the matrix:
 *
 *     kx^2*vers0 + cos0 = nx   ky^2*vers0 + cos0 = oy   kz^2*vers0 = az
 *
 *    Substitute for cos0 and vers0 in [0] and solving for the elements of 
 *    vector k, we get:
 *    
 *    [4] kx = +/- sqrt((nx - cos0) / (1 - cos0))
 *    [5] ky = +/- sqrt((oy - cos0) / (1 - cos0))
 *    [6] kz = +/- sqrt((az - cos0) / (1 - cos0))
 *
 *    The largest component of k defined by these equations corresponds to the
 *    most positive component of nx, oy, and az.  For this largest element, 
 *    the  sign of the radical can be obtained from [1], [2], and [3].  Since
 *    the sine of the angle must be positive (we defined it that way), the 
 *    sign of [4], [5], and [6] must correspond to the lhs of those equations.
 *    We can therefore combine these equations and obtain:
 *
 *    kx = sgn(oz - ay) * sqrt ((nx - cos0)/(1 - cos0))
 *    ky = sgn(ax - nz) * sqrt ((oy - cos0)/(1 - cos0))
 *    kz = sgn(ny - ox) * sqrt ((az - cos0)/(1 - cos0))
 *
 *    Where sgn(x) = +1 if x>0 and sgn(x) = -1 if x<=0.
 *
 *    Only the largest element of vector k is determined in this manner.  The 
 *    remaining elements are computed from:
 *
 *    [7] ny + ox = 2 * kx * ky * vers0
 *    [8] ox + ay = 2 * ky * kz * vers0
 *    [9] nz + ax = 2 * kz * kx * vers0
 *
 *    If kx is largest, ky is computed from [7] and kz is computed from [9].
 *    If ky is largest, kx is computed from [7] and kz is computed from [8].
 *    If kz is largest, kz is computed from [9] and ky is computed from [8].
 *
 *    Note that this is an abbreviated discussion and it would behoove the 
 *    reader to check out the referenced text.  This math is definitely 
 *    nontrivial.  This discussion should give you enough info to at least 
 *    understand the function.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_MatrixToArbitraryAxis( float arbaxis[3], float *pangle, Matrix m )
{
   double oz_ay;  /* oz - ay */
   double ax_nz;  /* ax - nz */
   double ny_ox;  /* ny - ox */

   oz_ay = m[1][2] - m[2][1];
   ax_nz = m[2][0] - m[0][2];
   ny_ox = m[0][1] - m[1][0];

   /*
    * Calculate theta as described above
    */
   *pangle = (float) atan2 (sqrt (SPW_SQ (oz_ay) + SPW_SQ (ax_nz) +
                                  SPW_SQ (ny_ox)),
                            m[0][0] + m[1][1] + m[2][2] - 1);

   /*
    * If the angle is <= 90 degrees then getting k is fairly easy
    */
   if (*pangle <= (SPW_PI/2))
      {
      arbaxis[0] = (float)(oz_ay / (2 * sin (*pangle)));
      arbaxis[1] = (float)(ax_nz / (2 * sin (*pangle)));
      arbaxis[2] = (float)(ny_ox / (2 * sin (*pangle)));
      SPW_NormalizeVector (arbaxis, arbaxis);
      }
   else     /* Do this if the angle is > 90 degrees. */
      {
      double cosa;      /* cosine of pangle                 */
      double versa;     /* versine of pangle ( = 1 - sin0 ) */
      double kx;        /* x component of vector k          */
      double ky;        /* y component of vector k          */
      double kz;        /* z component of vector k          */
      double max;       /* max of kx, ky, and kz            */
      double tmp;       /* temporary variable               */

      cosa = cos(*pangle);
      versa = 1 - cosa;       /* get the versine of theta */
      
      /*
       * Determine the largest component of vector k
       */

      /*
       * First get kx
       */
      tmp = (m[0][0] - cosa)/versa;     /* get value inside radical          */
      tmp = (tmp > 0) ? sqrt(tmp) : 0.0;/* ensure we don't get an imaginary #*/
      kx = SPW_SIGN(oz_ay) * tmp;       /* apply sign                        */

      /*
       * Now get ky
       */
      tmp = (m[1][1] - cosa) / versa; 
      tmp = (tmp > 0) ? sqrt (tmp) : 0.0;
      ky = SPW_SIGN(ax_nz) * tmp;

      /*
       * Now get kz
       */
      tmp = (m[2][2] - cosa) / versa; 
      tmp = (tmp > 0) ? sqrt (tmp) : 0.0;
      kz = SPW_SIGN(ny_ox) * tmp;

      /*
       * Find the largest component of vector k
       */
      max = SPW_MAX(SPW_ABS(kx), SPW_MAX(SPW_ABS(ky), SPW_ABS(kz)));

      /*
       * Depending on which component is largest, recompute the other two 
       * using more precise math
       */
      if (max == kx)    /* kx is largest */
         {
         ky = (m[0][1] + m[1][0]) / (2 * kx * versa);
         kz = (m[2][0] + m[0][2]) / (2 * kx * versa);
         }
      else if (max == ky)    /* ky is largest */
         {
         kx = (m[0][1] + m[1][0]) / (2 * ky * versa);
         kz = (m[1][2] + m[2][1]) / (2 * ky * versa);
         }
      else /* (max==kz) */
         {
         kx = (m[2][0] + m[0][2]) / (2 * kz * versa);
         ky = (m[1][2] + m[2][1]) / (2 * kz * versa);
         }

      arbaxis[0] = (float)kx;
      arbaxis[1] = (float)ky;
      arbaxis[2] = (float)kz;
      } /* end of else (angle > 90 ) */

} /* end of SPW_MatrixToArbitraryAxis */


