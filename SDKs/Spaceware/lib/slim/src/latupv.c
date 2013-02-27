/*-----------------------------------------------------------------------------
 *
 *  latupv.c -- define the view matrix
 *
 *  $Id: latupv.c,v 1.8 1998/03/05 13:18:29 jeffl Exp $
 *
 *  This function performs the function many graphics languages provide
 *  for camera orientation.  This is commonly called a "lookat" transformation.
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
  
static char cvsId[]="(C) 1998 Spacetec IMC Corporation: $Id: latupv.c,v 1.8 1998/03/05 13:18:29 jeffl Exp $";

#include "spwmacro.h"
#include "spwslim.h"

/*-----------------------------------------------------------------------------
 *
 *  void SPW_LookAtUpVectorToMatrix( Matrix m, float looking_from_pt[3],
 *                                  float looking_at_pt[3], float up_vector[3])
 *
 * Args: 
 *   m               (w/o) - resulting view matrix (world_to_eye)
 *   looking_from_pt (r/o) - the point in world coordinates that YOU are at
 *   looking_at_pt   (r/o) - the point in world coordinates that you are
 *                           looking at (sometimes called the reference pt)
 *   up_vector       (r/o) - the "up" direction in world coordinates.
 *                           This is a vector pointing in the direction
 *                           that is going to be directly up (the positive
 *                           y axis) in the eye coordinate system.  The
 *                           vector is specified in the world coordinate
 *                           system (we know it is going to be [0,1,0] in
 *                           the eye coordinate system).
 *
 * Return Value:
 *   None
 *
 * Description:
 *   This function performs the function many graphics languages provide
 *   for camera orientation.  This is commonly called a "lookat" 
 *   transformation. It merely defines a view matrix.  Most platforms make it 
 *   difficult to get at the actual matrix after the "lookat" function is
 *   called. This function creates a matrix from one of the common set of
 *   parameters.  There is another function (SPW_LookAtTwistToMatrix) that 
 *   specifies the up vector as a twist about the lookat direction instead
 *   of explicitly giving the up_vector.
 *
 *---------------------------------------------------------------------------*/
DLL_EXPORT void
SPW_LookAtUpVectorToMatrix( Matrix m, float looking_from_pt[3],
                            float looking_at_pt[3], float up_vector[3] )
{
   Matrix eye_to_world;  /* eyepoint to world coordinate matrix */

   /*
    * Work out eye position in world, then invert to give world_to_eye matrix.
    */
   eye_to_world[3][0] = looking_from_pt[0];
   eye_to_world[3][1] = looking_from_pt[1];
   eye_to_world[3][2] = looking_from_pt[2];

   /*
    * Eye Z orientation is right handed (pointing out the back of our head).
    */
   eye_to_world[2][0] = looking_from_pt[0] - looking_at_pt[0];
   eye_to_world[2][1] = looking_from_pt[1] - looking_at_pt[1];
   eye_to_world[2][2] = looking_from_pt[2] - looking_at_pt[2];
   SPW_NormalizeVector (eye_to_world[2], eye_to_world[2]);

   /*
    * The Eye up direction is given
    */
   eye_to_world[1][0] = up_vector[0];
   eye_to_world[1][1] = up_vector[1];
   eye_to_world[1][2] = up_vector[2];
   SPW_NormalizeVector (eye_to_world[1], eye_to_world[1]);

   /*
    * EYE X (side) vector is the cross product of the other two given vectors
    */
   SPW_CrossProduct (eye_to_world[0], eye_to_world[1], eye_to_world[2]);

   SPW_InvertMatrix (m, eye_to_world);
} /* end of SPW_LookAtUpVectorToMatrix */


