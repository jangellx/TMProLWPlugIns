/*-----------------------------------------------------------------------------
 *
 *  mlauvec.c -- Converts a matrix into lookat and up vector form.
 *
 *  $Id: m2lauvec.c,v 1.7 1998/03/05 13:18:33 jeffl Exp $
 *
 *  This routine takes a matrix and computes the looking from, looking at,
 *  and up vector notation.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: m2lauvec.c,v 1.7 1998/03/05 13:18:33 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 * void SPW_MatrixToLookAtUpVector( float looking_from_pt[3],
 *                                  float looking_at_pt[3],
 *                                  float up_vector[3], Matrix m )
 *
 * Args: looking_from_pt (w/o) - the point in world coordinates that YOU are at
 *       looking_at_pt   (w/o) - the point in world coordinates that you are
 *                               looking at (sometimes called the reference pt)
 *                               See caveat below.
 *       up_vector       (w/o) - the "up" direction in world coordinates.
 *                               This is a vector pointing in the direction
 *                               that is directly up (the positive y axis) 
 *                               in the eye coordinate system.
 *       m               (r/o) - view matrix (world_to_eye) to extract
 *                               parameters from.
 *
 * Return Value:
 *   None
 *
 * Description:
 *   This function returns "lookat" parameters from an existing world_to_eye
 *   matrix.  The looking_at_pt may be different than the looking_at_pt 
 *   passed to SPW_LookAtUpVectorToMatrix because only the direction from
 *   the looking_at_pt to the looking_from_pt are used to calculate the matrix,
 *   the actual distance between these points is not in the matrix (so it
 *   can not be retrieved).  As a result the looking_at_pt that is returned 
 *   from this function is one unit length away from the looking_from_pt.
 *   It is in the right direction, just, probably, not far enough along.
 * 
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_MatrixToLookAtUpVector( float looking_from_pt[3], float looking_at_pt[3],
                                float up_vector[3], Matrix m )
{
   Matrix eye_to_world;

   SPW_InvertMatrix (eye_to_world, m);

   looking_from_pt[0] = eye_to_world[3][0];
   looking_from_pt[1] = eye_to_world[3][1];
   looking_from_pt[2] = eye_to_world[3][2];

   looking_at_pt[0] = looking_from_pt[0] - eye_to_world[2][0];
   looking_at_pt[1] = looking_from_pt[1] - eye_to_world[2][1];
   looking_at_pt[2] = looking_from_pt[2] - eye_to_world[2][2];

   /*
    * The Eye up direction is given
    */
   up_vector[0] = eye_to_world[1][0];
   up_vector[1] = eye_to_world[1][1];
   up_vector[2] = eye_to_world[1][2];
} /* end of SPW_MatrixToLookAtUpVector */


