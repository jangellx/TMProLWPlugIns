/*----------------------------------------------------------------------
 *  spwmacro.h -- cpp macros we ALWAYS use.
 *
 *  $Id: spwmacro.h,v 1.9 1998/02/13 14:45:28 prichard Exp $
 *
 *   We always seem to use the same macros.
 *   This is the place we define them.
 *
 *----------------------------------------------------------------------
 *
 * (C) 1997 Spacetec IMC Corporation ("Spacetec").
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies. Permission to modify this software is granted
 * and Spacetec will support such modifications only if said modifications are
 * approved by Spacetec.
 *
 */

#ifndef SPWMACRO_H
#define SPWMACRO_H

#ifndef NOCVSSTRINGS
static char spwmacroCvsId[]="(C) 1996 Spacetec IMC Corporation: $Id: spwmacro.h,v 1.9 1998/02/13 14:45:28 prichard Exp $";
#endif
 
#define SPW_FALSE   (0)
#define SPW_TRUE    (!SPW_FALSE)

#define SPW_MAX(a,b)   (((a)>(b))?(a):(b))
#define SPW_MIN(a,b)   (((a)<(b))?(a):(b))

#define SPW_ABS(a)   (((a)<0)?(-(a)):(a))

#define SPW_SIGN(a)  ((a)>=0?1:-1)

#define SPW_BIND(min,n,max)   (SPW_MIN((max),SPW_MAX((min),(n))))

#define SPW_NUM_ELEMENTS_IN(a)   (sizeof(a)/sizeof((a)[0]))

#define SPW_PI   3.14159265358979324f

#define SPW_DEG_TO_RAD(d)   ((d)*SPW_PI/180.0f)
#define SPW_RAD_TO_DEG(r)   ((r)*180.0f/SPW_PI)

#define SPW_LENGTH_OF(a)   (sizeof(a)/sizeof((a)[0]))

#define SPW_END_OF(a)   (&(a)[SPW_LENGTH_OF(a)-1])

#define SPW_SQ(a)   ((a)*(a))

#define SPW_ABSDIFF(a, b) (fabs((double) (a) - (b)))


#endif
