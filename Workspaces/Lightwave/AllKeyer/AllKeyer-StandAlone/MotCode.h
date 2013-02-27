
// MotCode.h

/*
  This file contains structures and typedefs for MotCode.c

    ChaVec:          Channel array, used by MotCode.c
    Motion:          Motion structure, used by MotCode.c
    KeyFrame:        Keyframe structure, used by MotCode.c
*/

#pragma once

// KeyFrame/Motion Structs and other MotCode.c definitions
typedef double	* ChanVec;		// was  typedef double ChanVec[NUM_CHAN]

/*
 * Each KeyFrame in a motion has the value of the channels at that key,
 * tension, contunity and bias spline controls, a linear flag and the
 * frame at which the key is located (step).
 */
typedef struct st_KeyFrame {
	ChanVec  cv;				    		// Channel Array (H P B   X Y Z   SX SY SZ)
	double   tens, cont, bias;	// Spline controls
	int			 linear;				  	// Linear (Boolean)
	int			 step;					    // Frame that the key is located at (current step in the animation)
	} KeyFrame;


/*
 * A motion is just an array of `keys' KeyFrames and the total number of
 * steps in the motion.
 */
typedef struct st_Motion {
	KeyFrame	*keylist;					// An array of KeyFrame Structure Pointers
	int		     keys, steps;			// Number of Keys and Total number of frames
	int			   channels;				// Number of channels, added by yours truly
	int			   TotalKeys;				// My addition:  Total keys in motion, for FreeMotion()
	} Motion;

