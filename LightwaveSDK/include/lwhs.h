#ifndef LWHS_INCL
#define LWHS_INCL

/*
 * A motion tracks 9 channels (x, y, z position, h, p, b rotation angle,
 * and x, y, z scale), although this number can vary.  Envelopes work the
 * smae way but track only one channel.
 */

#define LWHS_NUM_CHAN    9
typedef double      LWHS_ChanVec[LWHS_NUM_CHAN];


/*
 * Each KeyFrame in a motion has the value of the channels at that key,
 * tension, contunity and bias spline controls, a linear flag and the
 * frame at which the key is located (step).
 */

typedef struct st_LWHS_KeyFrame
{
    LWHS_ChanVec    cv;
    double          tens, cont, bias;
    int             linear;
    int             step;
} LWHS_KeyFrame;


/*
 * A motion is just an array of `keys' KeyFrames and the total number of
 * steps in the motion.
 */

typedef struct st_LWHS_Motion
{
    LWHS_KeyFrame   *keylist;
    int             keys, steps;
} LWHS_Motion;

void    LWHS_CalcStep(LWHS_Motion *mot,LWHS_ChanVec resVec,double step);

#endif
