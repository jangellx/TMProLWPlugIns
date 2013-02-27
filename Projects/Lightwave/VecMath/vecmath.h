/*
======================================================================
vecmath.h

Header file for the vector math Global class sample plug-in.

Ernie Wright  19 Mar 00

Include this header in code that uses the vector math global.  See the
comments in vecmath.c for details.

- 04-25-01:  Modified into a linker library and a single C++ class
             of static functions.
====================================================================== */

#ifndef VECMATH_H
#define VECMATH_H

#include <lwtypes.h>

typedef float  LWFMatrix3[ 3 ][ 3 ];
typedef float  LWFMatrix4[ 4 ][ 4 ];

typedef double LWDMatrix3[ 3 ][ 3 ];
typedef double LWDMatrix4[ 4 ][ 4 ];

class LWVecMath {
public:
   // Float
   static void   initv      ( LWFVector a, float x, float y, float z );
   static void   initm3     ( LWFMatrix3 m, float, float, float, float,
                             float, float, float, float, float );
   static void   initm4     ( LWFMatrix4 m, float, float, float, float,
                             float, float, float, float, float, float,
                             float, float, float, float, float, float );
   static void   identity3  ( LWFMatrix3 m );
   static void   identity4  ( LWFMatrix4 m );
   static void   copyv      ( LWFVector to, LWFVector from );
   static void   copym3     ( LWFMatrix3 to, LWFMatrix3 from );
   static void   copym4     ( LWFMatrix4 to, LWFMatrix4 from );
   static float  dot        ( LWFVector a, LWFVector b );
   static float  lenSquared ( LWFVector a );
   static float  len        ( LWFVector a );
   static void   scalev     ( LWFVector a, float s );
   static void   setlen     ( LWFVector a, float d );
   static void   normalize  ( LWFVector a );
   static void   neg        ( LWFVector a );
   static void   add        ( LWFVector a, LWFVector b, LWFVector c );
   static void   sub        ( LWFVector a, LWFVector b, LWFVector c );
   static void   mul        ( LWFVector a, LWFVector b, LWFVector c );
   static void   lerp       ( LWFVector a, LWFVector b, float t, LWFVector c );
   static void   combine    ( LWFVector a, LWFVector b, LWFVector c, float sa, float sb );
   static void   cross      ( LWFVector a, LWFVector b, LWFVector c );
   static void   polynorm   ( LWFVector v1, LWFVector v2, LWFVector vlast, LWFVector norm );
   static float  dist       ( LWFVector a, LWFVector b );
   static float  angle      ( LWFVector a, LWFVector b );
   static void   vec_hp     ( LWFVector a, float *h, float *p );
   static void   hp_vec     ( float h, float p, LWFVector a );
   static void   transform  ( LWFVector a, LWFMatrix3 m, LWFVector b );
   static void   transformp ( LWFVector a, LWFMatrix4 m, LWFVector b );
   static void   matmul3    ( LWFMatrix3 a, LWFMatrix3 b, LWFMatrix3 c );
   static void   matmul4    ( LWFMatrix4 a, LWFMatrix4 b, LWFMatrix4 c );
   static void   scalem4    ( LWFMatrix4 a, float s );
   static void   scalem3    ( LWFMatrix3 a, float s );
   static float  det2       ( float a, float b, float c, float d );
   static float  det3       ( LWFMatrix3 m );
   static float  det4       ( LWFMatrix4 m );
   static void   adjoint3   ( LWFMatrix3 m, LWFMatrix3 adj );
   static void   adjoint4   ( LWFMatrix4 m, LWFMatrix4 adj );
   static void   inverse3   ( LWFMatrix3 m, LWFMatrix3 inv );
   static void   inverse4   ( LWFMatrix4 m, LWFMatrix4 inv );
   static void   mat4_quat  ( LWFMatrix4 m, float q[ 4 ] );
   static void   quat_mat4  ( float q[ 4 ], LWFMatrix4 m );
   static void   slerp      ( float p[ 4 ], float q[ 4 ], float t, float qt[ 4 ] );

   // Double
   static void   initv      ( LWDVector a, double x, double y, double z );
   static void   initm3     ( LWDMatrix3 m, double, double, double, double,
                                 double, double, double, double, double );
   static void   initm4     ( LWDMatrix4 m, double, double, double, double,
                                 double, double, double, double, double, double,
                                 double, double, double, double, double, double );
   static void   identity3  ( LWDMatrix3 m );
   static void   identity4  ( LWDMatrix4 m );
   static void   copyv      ( LWDVector to, LWDVector from );
   static void   copym3     ( LWDMatrix3 to, LWDMatrix3 from );
   static void   copym4     ( LWDMatrix4 to, LWDMatrix4 from );
   static double dot        ( LWDVector a, LWDVector b );
   static double lenSquared ( LWDVector a );
   static double len        ( LWDVector a );
   static void   scalev     ( LWDVector a, double s );
   static void   setlen     ( LWDVector a, double d );
   static void   normalize  ( LWDVector a );
   static void   neg        ( LWDVector a );
   static void   add        ( LWDVector a, LWDVector b, LWDVector c );
   static void   sub        ( LWDVector a, LWDVector b, LWDVector c );
   static void   mul        ( LWDVector a, LWDVector b, LWDVector c );
   static void   lerp       ( LWDVector a, LWDVector b, double t, LWDVector c );
   static void   combine    ( LWDVector a, LWDVector b, LWDVector c, double sa, double sb );
   static void   cross      ( LWDVector a, LWDVector b, LWDVector c );
   static void   polynorm   ( LWDVector v1, LWDVector v2, LWDVector vlast, LWDVector norm );
   static double dist       ( LWDVector a, LWDVector b );
   static double angle      ( LWDVector a, LWDVector b );
   static void   vec_hp     ( LWDVector a, double *h, double *p );
   static void   hp_vec     ( double h, double p, LWDVector a );
   static void   transform  ( LWDVector a, LWDMatrix3 m, LWDVector b );
   static void   transformp ( LWDVector a, LWDMatrix4 m, LWDVector b );
   static void   matmul3    ( LWDMatrix3 a, LWDMatrix3 b, LWDMatrix3 c );
   static void   matmul4    ( LWDMatrix4 a, LWDMatrix4 b, LWDMatrix4 c );
   static void   scalem4    ( LWDMatrix4 a, double s );
   static void   scalem3    ( LWDMatrix3 a, double s );
   static double det2       ( double a, double b, double c, double d );
   static double det3       ( LWDMatrix3 m );
   static double det4       ( LWDMatrix4 m );
   static void   adjoint3   ( LWDMatrix3 m, LWDMatrix3 adj );
   static void   adjoint4   ( LWDMatrix4 m, LWDMatrix4 adj );
   static void   inverse3   ( LWDMatrix3 m, LWDMatrix3 inv );
   static void   inverse4   ( LWDMatrix4 m, LWDMatrix4 inv );
   static void   mat4_quat  ( LWDMatrix4 m, double q[ 4 ] );
   static void   quat_mat4  ( double q[ 4 ], LWDMatrix4 m );
   static void   slerp      ( double p[ 4 ], double q[ 4 ], double t, double qt[ 4 ] );
};


#ifndef PI
#define PI     3.14159265358979323846264338328
#define HALFPI 1.57079632679489661923132169164
#endif

#define RADIANS( deg ) ((deg) * PI / 180.0)
#define DEGREES( rad ) ((rad) * 180.0 / PI)

#endif
