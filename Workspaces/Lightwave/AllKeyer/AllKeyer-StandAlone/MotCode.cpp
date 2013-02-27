/*
 * MOTCODE.C -- Motion path implementation.  This provides a simple
 * implementation of motions paths as defined by LightWave 3D.
 *
 * originally by	Allen Hastings
 * modified by		Stuart Ferguson
 *
 *	1/23/96
 */

#include "MotCode.h"

/*
 * Compute Hermite spline coeficients for t, where 0 <= t <= 1.
 */
static void Hermite (double t, double *h1, double *h2, double *h3, double *h4)
	{
		double			 t2, t3, z;

		t2 = t * t;
		t3 = t * t2;
		z = 3.0 * t2 - t3 - t3;

		*h1 = 1.0 - z;
		*h2 = z;
		*h3 = t3 - t2 - t2 + t;
		*h4 = t3 - t2;
	}


/*
 * Compute the motion channel vector for the given step.  Step can be
 * fractional but values correspond to frames.
 */
void MotionCalcStep (Motion *mot, ChanVec resVec, double	step)
	// Motion		Pointer to a Motion structure
	// resVec		Result of the Spline computations, in a ChanVec array
	// step			Frame to compute resVec for
	{
		KeyFrame		*key0, *key1;
		double		 t, h1, h2, h3, h4, res, d10;
		double		 dd0a, dd0b, ds1a, ds1b;
		double		 adj0, adj1, dd0, ds1;
		int			 i, tlength;

		/*
		 * If there is but one key, the values are constant.
		 */
		if (mot->keys == 1)
			{
      	for (i = 0; i < mot->channels; i++)
				resVec[i] = mot->keylist[0].cv[i];

			return;
			}	// end of "if"

	/*
	 * Get keyframe pair to evaluate.  This should be within the range
	 * of the motion or this will raise an illegal access.
	 */
	key0 = mot->keylist;

	while (step > key0[1].step)
		key0 ++;

	key1 = key0 + 1;
	step -= key0->step;

	/*
	 * Get tween length and fractional tween position.
	 */
	tlength = key1->step - key0->step;
	t = step / tlength;

	/*
	 * Precompute spline coefficients.
	 */
	if (!key1->linear)
		{
		Hermite (t, &h1, &h2, &h3, &h4);
		dd0a = (1.0 - key0->tens) * (1.0 + key0->cont)
			 * (1.0 + key0->bias);
		dd0b = (1.0 - key0->tens) * (1.0 - key0->cont)
			 * (1.0 - key0->bias);
		ds1a = (1.0 - key1->tens) * (1.0 - key1->cont)
			 * (1.0 + key1->bias);
		ds1b = (1.0 - key1->tens) * (1.0 + key1->cont)
			 * (1.0 - key1->bias);

		if (key0->step != 0)
		    adj0 = tlength / (double)(key1->step - (key0-1)->step);
		if (key1->step != mot->steps)
		    adj1 = tlength / (double)((key1+1)->step - key0->step);
		} // end of "if"

	/*
	 * Compute the channel components.
	 */
	for (i = 0; i < mot->channels; i++)
		{
		d10 = key1->cv[i] - key0->cv[i];

		if (!key1->linear)
			{
			if (key0->step == 0)
				dd0 = .5 * (dd0a + dd0b) * d10;
			else
				dd0 = adj0 * (dd0a * (key0->cv[i] - (key0-1)->cv[i]) + dd0b * d10);

			if (key1->step == mot->steps)
				ds1 = .5 * (ds1a + ds1b) * d10;
			else
				ds1 = adj1 * (ds1a * d10 + ds1b * ((key1+1)->cv[i] - key1->cv[i]));

			res = key0->cv[i] * h1 + key1->cv[i] * h2 + dd0 * h3 + ds1 * h4;
			}
		else
			res = key0->cv[i] + t * d10;

		resVec[i] = res;
	} // end of function
}
