/*
======================================================================
vecmath_inline.h

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
   inline static void   initv      ( LWFVector a, float x, float y, float z );
   inline static void   initm3     ( LWFMatrix3 m, float, float, float, float,
                                     float, float, float, float, float );
   inline static void   initm4     ( LWFMatrix4 m, float, float, float, float,
                                     float, float, float, float, float, float,
                                     float, float, float, float, float, float );
   inline static void   identity3  ( LWFMatrix3 m );
   inline static void   identity4  ( LWFMatrix4 m );
   inline static void   copyv      ( LWFVector to, LWFVector from );
   inline static void   copym3     ( LWFMatrix3 to, LWFMatrix3 from );
   inline static void   copym4     ( LWFMatrix4 to, LWFMatrix4 from );
   inline static float  dot        ( LWFVector a, LWFVector b );
   inline static float  lenSquared ( LWFVector a );
   inline static float  len        ( LWFVector a );
   inline static void   scalev     ( LWFVector a, float s );
   inline static void   setlen     ( LWFVector a, float d );
   inline static void   normalize  ( LWFVector a );
   inline static void   neg        ( LWFVector a );
   inline static void   add        ( LWFVector a, LWFVector b, LWFVector c );
   inline static void   sub        ( LWFVector a, LWFVector b, LWFVector c );
   inline static void   mul        ( LWFVector a, LWFVector b, LWFVector c );
   inline static void   lerp       ( LWFVector a, LWFVector b, float t, LWFVector c );
   inline static void   combine    ( LWFVector a, LWFVector b, LWFVector c, float sa, float sb );
   inline static void   cross      ( LWFVector a, LWFVector b, LWFVector c );
   inline static void   polynorm   ( LWFVector v1, LWFVector v2, LWFVector vlast, LWFVector norm );
   inline static float  dist       ( LWFVector a, LWFVector b );
   inline static float  angle      ( LWFVector a, LWFVector b );
   inline static void   vec_hp     ( LWFVector a, float *h, float *p );
   inline static void   hp_vec     ( float h, float p, LWFVector a );
   inline static void   transform  ( LWFVector a, LWFMatrix3 m, LWFVector b );
   inline static void   transformp ( LWFVector a, LWFMatrix4 m, LWFVector b );
   inline static void   matmul3    ( LWFMatrix3 a, LWFMatrix3 b, LWFMatrix3 c );
   inline static void   matmul4    ( LWFMatrix4 a, LWFMatrix4 b, LWFMatrix4 c );
   inline static void   scalem4    ( LWFMatrix4 a, float s );
   inline static void   scalem3    ( LWFMatrix3 a, float s );
   inline static float  det2       ( float a, float b, float c, float d );
   inline static float  det3       ( LWFMatrix3 m );
   inline static float  det4       ( LWFMatrix4 m );
   inline static void   adjoint3   ( LWFMatrix3 m, LWFMatrix3 adj );
   inline static void   adjoint4   ( LWFMatrix4 m, LWFMatrix4 adj );
   inline static void   inverse3   ( LWFMatrix3 m, LWFMatrix3 inv );
   inline static void   inverse4   ( LWFMatrix4 m, LWFMatrix4 inv );
   inline static void   mat4_quat  ( LWFMatrix4 m, float q[ 4 ] );
   inline static void   quat_mat4  ( float q[ 4 ], LWFMatrix4 m );
   inline static void   slerp      ( float p[ 4 ], float q[ 4 ], float t, float qt[ 4 ] );

   // Double
   inline static void   initv      ( LWDVector a, double x, double y, double z );
   inline static void   initm3     ( LWDMatrix3 m, double, double, double, double,
                                     double, double, double, double, double );
   inline static void   initm4     ( LWDMatrix4 m, double, double, double, double,
                                     double, double, double, double, double, double,
                                     double, double, double, double, double, double );
   inline static void   identity3  ( LWDMatrix3 m );
   inline static void   identity4  ( LWDMatrix4 m );
   inline static void   copyv      ( LWDVector to, LWDVector from );
   inline static void   copym3     ( LWDMatrix3 to, LWDMatrix3 from );
   inline static void   copym4     ( LWDMatrix4 to, LWDMatrix4 from );
   inline static double dot        ( LWDVector a, LWDVector b );
   inline static double lenSquared ( LWDVector a );
   inline static double len        ( LWDVector a );
   inline static void   scalev     ( LWDVector a, double s );
   inline static void   setlen     ( LWDVector a, double d );
   inline static void   normalize  ( LWDVector a );
   inline static void   neg        ( LWDVector a );
   inline static void   add        ( LWDVector a, LWDVector b, LWDVector c );
   inline static void   sub        ( LWDVector a, LWDVector b, LWDVector c );
   inline static void   mul        ( LWDVector a, LWDVector b, LWDVector c );
   inline static void   lerp       ( LWDVector a, LWDVector b, double t, LWDVector c );
   inline static void   combine    ( LWDVector a, LWDVector b, LWDVector c, double sa, double sb );
   inline static void   cross      ( LWDVector a, LWDVector b, LWDVector c );
   inline static void   polynorm   ( LWDVector v1, LWDVector v2, LWDVector vlast, LWDVector norm );
   inline static double dist       ( LWDVector a, LWDVector b );
   inline static double angle      ( LWDVector a, LWDVector b );
   inline static void   vec_hp     ( LWDVector a, double *h, double *p );
   inline static void   hp_vec     ( double h, double p, LWDVector a );
   inline static void   transform  ( LWDVector a, LWDMatrix3 m, LWDVector b );
   inline static void   transformp ( LWDVector a, LWDMatrix4 m, LWDVector b );
   inline static void   matmul3    ( LWDMatrix3 a, LWDMatrix3 b, LWDMatrix3 c );
   inline static void   matmul4    ( LWDMatrix4 a, LWDMatrix4 b, LWDMatrix4 c );
   inline static void   scalem4    ( LWDMatrix4 a, double s );
   inline static void   scalem3    ( LWDMatrix3 a, double s );
   inline static double det2       ( double a, double b, double c, double d );
   inline static double det3       ( LWDMatrix3 m );
   inline static double det4       ( LWDMatrix4 m );
   inline static void   adjoint3   ( LWDMatrix3 m, LWDMatrix3 adj );
   inline static void   adjoint4   ( LWDMatrix4 m, LWDMatrix4 adj );
   inline static void   inverse3   ( LWDMatrix3 m, LWDMatrix3 inv );
   inline static void   inverse4   ( LWDMatrix4 m, LWDMatrix4 inv );
   inline static void   mat4_quat  ( LWDMatrix4 m, double q[ 4 ] );
   inline static void   quat_mat4  ( double q[ 4 ], LWDMatrix4 m );
   inline static void   slerp      ( double p[ 4 ], double q[ 4 ], double t, double qt[ 4 ] );
};


#ifndef PI
#define PI     3.14159265358979323846264338328
#define HALFPI 1.57079632679489661923132169164
#endif

#define RADIANS( deg ) ((deg) * PI / 180.0)
#define DEGREES( rad ) ((rad) * 180.0 / PI)

/*
======================================================================
vecmath.c

A Global class plug-in that supplies vector and matrix math functions.

Ernie Wright  19 Mar 00

I was looking at Ron Goldman's revised version of Andrew Glassner's
GRAPHICS GEMS vector library when I wrote this.  See GG II, James
Arvo ed., Academic Press, 1991, ISBN 0-12-064480-0.  I also looked at
Richard Carling's matrix inversion using adjoints and determinants in
GG I (A. Glassner ed., ISBN 0-12-286165-5) and my undergraduate linear
algebra textbook.

The quaternion routines are from Alan Watt and Mark Watt, ADVANCED
ANIMATION AND RENDERING TECHNIQUES, ACM Press, 1992, ISBN
0-201-54412-1.

These implementations favor simplicity over efficiency.  If you need
speed, find a good LU decomposition routine for matrix inversion and
inline the rest.

To call this from your code, first get the structure with the math
functions by calling the global() function.  You can get single or
double precision versions of these.

   #include "vecmath.h"

   LWFVecMathFuncs *vec;

   vec = global( LWFVECMATHFUNCS_GLOBAL, GFUSE_TRANSIENT );
   if ( !vec ) return AFUNC_BADGLOBAL;

Then just call the functions.

   LWFVector a, b, c;

   ...
   vec->add( a, b, c );     // c = a + b
   ...

- 04-25-01:  Modified into a linker library and a single C++ class
             of static functions.
====================================================================== */

#include <lwserver.h>
#include <lwglobsrv.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "vecmath.h"

#define EPSILON_F  1e-6
#define EPSILON_D  1e-15

/* some matrix shorthand */
#define m00  m[ 0 ][ 0 ]
#define m01  m[ 0 ][ 1 ]
#define m02  m[ 0 ][ 2 ]
#define m03  m[ 0 ][ 3 ]
#define m10  m[ 1 ][ 0 ]
#define m11  m[ 1 ][ 1 ]
#define m12  m[ 1 ][ 2 ]
#define m13  m[ 1 ][ 3 ]
#define m20  m[ 2 ][ 0 ]
#define m21  m[ 2 ][ 1 ]
#define m22  m[ 2 ][ 2 ]
#define m23  m[ 2 ][ 3 ]
#define m30  m[ 3 ][ 0 ]
#define m31  m[ 3 ][ 1 ]
#define m32  m[ 3 ][ 2 ]
#define m33  m[ 3 ][ 3 ]


/*
======================================================================
Single precision functions.
====================================================================== */

inline void LWVecMath::initv( LWFVector a, float x, float y, float z )
{
   a[ 0 ] = x;
   a[ 1 ] = y;
   a[ 2 ] = z;
}


inline void LWVecMath::initm3( LWFMatrix3 m,
   float a1, float b1, float c1,
   float a2, float b2, float c2,
   float a3, float b3, float c3 )
{
   m00 = a1;  m01 = b1;  m02 = c1;
   m10 = a2;  m11 = b2;  m12 = c2;
   m20 = a3;  m21 = b3;  m22 = c3;
}


inline void LWVecMath::initm4( LWFMatrix4 m,
   float a1, float b1, float c1, float d1,
   float a2, float b2, float c2, float d2,
   float a3, float b3, float c3, float d3,
   float a4, float b4, float c4, float d4 )
{
   m00 = a1;  m01 = b1;  m02 = c1;  m03 = d1;
   m10 = a2;  m11 = b2;  m12 = c2;  m13 = d2;
   m20 = a3;  m21 = b3;  m22 = c3;  m23 = d3;
   m30 = a4;  m31 = b4;  m32 = c4;  m33 = d4;
}


inline void LWVecMath::identity3( LWFMatrix3 m )
{
   initm3( m,
      1.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 1.0f );
}


inline void LWVecMath::identity4( LWFMatrix4 m )
{
   initm4( m,
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f );
}


inline void LWVecMath::copyv( LWFVector to, LWFVector from )
{
   to[ 0 ] = from[ 0 ];
   to[ 1 ] = from[ 1 ];
   to[ 2 ] = from[ 2 ];
}


inline void LWVecMath::copym3( LWFMatrix3 to, LWFMatrix3 from )
{
   memcpy( to, from, sizeof( LWFMatrix3 ));
}


inline void LWVecMath::copym4( LWFMatrix4 to, LWFMatrix4 from )
{
   memcpy( to, from, sizeof( LWFMatrix4 ));
}


inline float LWVecMath::dot( LWFVector a, LWFVector b )
{
   return a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ 1 ] + a[ 2 ] * b[ 2 ];
}


inline float LWVecMath::lenSquared( LWFVector a )
{
   return dot( a, a );
}


inline float LWVecMath::len( LWFVector a )
{
   return ( float ) sqrt( lenSquared( a ));
}


inline void LWVecMath::neg( LWFVector a )
{
   a[ 0 ] = -a[ 0 ];
   a[ 1 ] = -a[ 1 ];
   a[ 2 ] = -a[ 2 ];
}


inline void LWVecMath::scalev( LWFVector a, float s )
{
   a[ 0 ] *= s;
   a[ 1 ] *= s;
   a[ 2 ] *= s;
}


inline void LWVecMath::setlen( LWFVector a, float d )
{
   float d0 = len( a );

   if ( d0 != 0.0f ) scalev( a, d / d0 );
}


inline void LWVecMath::normalize( LWFVector a )
{
   setlen( a, 1.0f );
}


inline void LWVecMath::add( LWFVector a, LWFVector b, LWFVector c )
{
   c[ 0 ] = a[ 0 ] + b[ 0 ];
   c[ 1 ] = a[ 1 ] + b[ 1 ];
   c[ 2 ] = a[ 2 ] + b[ 2 ];
}


inline void LWVecMath::sub( LWFVector a, LWFVector b, LWFVector c )
{
   c[ 0 ] = a[ 0 ] - b[ 0 ];
   c[ 1 ] = a[ 1 ] - b[ 1 ];
   c[ 2 ] = a[ 2 ] - b[ 2 ];
}


inline void LWVecMath::mul( LWFVector a, LWFVector b, LWFVector c )
{
   c[ 0 ] = a[ 0 ] * b[ 0 ];
   c[ 1 ] = a[ 1 ] * b[ 1 ];
   c[ 2 ] = a[ 2 ] * b[ 2 ];
}


inline void LWVecMath::lerp( LWFVector a, LWFVector b, float t, LWFVector c )
{
   c[ 0 ] = a[ 0 ] + ( b[ 0 ] - a[ 0 ] ) * t;
   c[ 1 ] = a[ 1 ] + ( b[ 1 ] - a[ 1 ] ) * t;
   c[ 2 ] = a[ 2 ] + ( b[ 2 ] - a[ 2 ] ) * t;
}


inline void LWVecMath::combine( LWFVector a, LWFVector b, LWFVector c, float sa, float sb )
{
   c[ 0 ] = sa * a[ 0 ] + sb * b[ 0 ];
   c[ 1 ] = sa * a[ 1 ] + sb * b[ 1 ];
   c[ 2 ] = sa * a[ 2 ] + sb * b[ 2 ];
}


inline void LWVecMath::cross( LWFVector a, LWFVector b, LWFVector c )
{
   c[ 0 ] = a[ 1 ] * b[ 2 ] - a[ 2 ] * b[ 1 ];
   c[ 1 ] = a[ 2 ] * b[ 0 ] - a[ 0 ] * b[ 2 ];
   c[ 2 ] = a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ];
}


inline void LWVecMath::polynorm( LWFVector v1, LWFVector v2, LWFVector vlast, LWFVector norm )
{
   LWFVector a, b;

   sub( v2, v1, a );
   sub( vlast, v1, b );
   cross( a, b, norm );
   normalize( norm );
}


inline float LWVecMath::dist( LWFVector a, LWFVector b )
{
   LWFVector c;

   sub( a, b, c );
   return len( c );
}


inline float LWVecMath::angle( LWFVector a, LWFVector b )
{
   LWFVector na, nb;

   copyv( na, a );
   normalize( na );
   copyv( nb, b );
   normalize( nb );
   return ( float ) acos( dot( na, nb ));
}


inline void LWVecMath::vec_hp( LWFVector a, float *h, float *p )
{
   LWFVector n;

   copyv( n, a );
   normalize( n );

   *p = ( float ) asin( -n[ 1 ] );
   if ( 1.0f - fabs( n[ 1 ] ) > EPSILON_F ) {
      *h = ( float )( acos( n[ 2 ] / cos( *p )));
      if ( n[ 0 ] < 0.0f )
         *h = ( float )( 2 * PI - *h );
   }
   else *h = 0.0f;
}


inline void LWVecMath::hp_vec( float h, float p, LWFVector a )
{
   float
      ch = ( float ) cos( h ),
      sh = ( float ) sin( h ),
      cp = ( float ) cos( p ),
      sp = ( float ) sin( p );

   a[ 0 ] = sh * cp;
   a[ 1 ] = -sp;
   a[ 2 ] = ch * cp;
}


inline void LWVecMath::transform( LWFVector a, LWFMatrix3 m, LWFVector b )
{
   int i;

   for ( i = 0; i < 3; i++ )
      b[ i ] = a[ 0 ] * m[ 0 ][ i ]
             + a[ 1 ] * m[ 1 ][ i ]
             + a[ 2 ] * m[ 2 ][ i ];
}


inline void LWVecMath::transformp( LWFVector a, LWFMatrix4 m, LWFVector b )
{
   float w;
   int i;

   for ( i = 0; i < 3; i++ )
      b[ i ] = a[ 0 ] * m[ 0 ][ i ]
             + a[ 1 ] * m[ 1 ][ i ]
             + a[ 2 ] * m[ 2 ][ i ] + m[ 3 ][ i ];

   w = a[ 0 ] * m[ 0 ][ 3 ]
     + a[ 1 ] * m[ 1 ][ 3 ]
     + a[ 2 ] * m[ 2 ][ 3 ] + m[ 3 ][ 3 ];

   if ( w != 0.0f ) {
      b[ 0 ] /= w;
      b[ 1 ] /= w;
      b[ 2 ] /= w;
   }
}


inline void LWVecMath::matmul3( LWFMatrix3 a, LWFMatrix3 b, LWFMatrix3 c )
{
   int i, j, k;
   LWFMatrix3 m = { 0.0f };

   for ( i = 0; i < 3; i++ )
      for ( j = 0; j < 3; j++ )
         for ( k = 0; k < 3; k++ )
            m[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];

   memcpy( &c[ 0 ][ 0 ], &m[ 0 ][ 0 ], sizeof( LWFMatrix3 ));
}


inline void LWVecMath::matmul4( LWFMatrix4 a, LWFMatrix4 b, LWFMatrix4 c )
{
   int i, j, k;
   LWFMatrix4 m = { 0.0f };

   for ( i = 0; i < 4; i++ )
      for ( j = 0; j < 4; j++ )
         for ( k = 0; k < 4; k++ )
            m[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];

   memcpy( &c[ 0 ][ 0 ], &m[ 0 ][ 0 ], sizeof( LWFMatrix4 ));
}


inline void LWVecMath::scalem4( LWFMatrix4 a, float s )
{
   int i, j;

   for ( i = 0; i < 4; i++ )
      for ( j = 0; j < 4; j++ )
         a[ i ][ j ] *= s;
}


inline void LWVecMath::scalem3( LWFMatrix3 a, float s )
{
   int i, j;

   for ( i = 0; i < 3; i++ )
      for ( j = 0; j < 3; j++ )
         a[ i ][ j ] *= s;
}


inline float LWVecMath::det2( float a, float b, float c, float d )
{
    return a * d - b * c;
}


inline float LWVecMath::det3( LWFMatrix3 m )
{
   return   m00 * det2( m11, m21, m12, m22 )
          - m01 * det2( m10, m20, m12, m22 )
          + m02 * det2( m10, m20, m11, m21 );
}


inline float LWVecMath::det4( LWFMatrix4 m )
{
   LWFMatrix3 p;
   float a, b, c, d;

   initm3( p, m11, m21, m31, m12, m22, m32, m13, m23, m33 );  a = det3( p );
   initm3( p, m10, m20, m30, m12, m22, m32, m13, m23, m33 );  b = det3( p );
   initm3( p, m10, m20, m30, m11, m21, m31, m13, m23, m33 );  c = det3( p );
   initm3( p, m10, m20, m30, m11, m21, m31, m12, m22, m32 );  d = det3( p );

   return m00 * a - m01 * b + m02 * c - m03 * d;
}


inline void LWVecMath::adjoint3( LWFMatrix3 m, LWFMatrix3 adj )
{
   adj[ 0 ][ 0 ] =  det2( m11, m12, m21, m22 );
   adj[ 1 ][ 0 ] = -det2( m10, m12, m20, m22 );
   adj[ 2 ][ 0 ] =  det2( m10, m11, m20, m21 );
   adj[ 0 ][ 1 ] = -det2( m01, m02, m21, m22 );
   adj[ 1 ][ 1 ] =  det2( m00, m02, m20, m22 );
   adj[ 2 ][ 1 ] = -det2( m00, m01, m20, m21 );
   adj[ 0 ][ 2 ] =  det2( m01, m02, m11, m12 );
   adj[ 1 ][ 2 ] = -det2( m00, m02, m10, m12 );
   adj[ 2 ][ 2 ] =  det2( m00, m01, m10, m11 );
}


inline void LWVecMath::adjoint4( LWFMatrix4 m, LWFMatrix4 adj )
{
   LWFMatrix3 a;

   initm3( a, m11, m21, m31, m12, m22, m32, m13, m23, m33 );  adj[ 0 ][ 0 ] =  det3( a );
   initm3( a, m10, m20, m30, m12, m22, m32, m13, m23, m33 );  adj[ 1 ][ 0 ] = -det3( a );
   initm3( a, m10, m20, m30, m11, m21, m31, m13, m23, m33 );  adj[ 2 ][ 0 ] =  det3( a );
   initm3( a, m10, m20, m30, m11, m21, m31, m12, m22, m32 );  adj[ 3 ][ 0 ] = -det3( a );
   initm3( a, m01, m21, m31, m02, m22, m32, m03, m23, m33 );  adj[ 0 ][ 1 ] = -det3( a );
   initm3( a, m00, m20, m30, m02, m22, m32, m03, m23, m33 );  adj[ 1 ][ 1 ] =  det3( a );
   initm3( a, m00, m20, m30, m01, m21, m31, m03, m23, m33 );  adj[ 2 ][ 1 ] = -det3( a );
   initm3( a, m00, m20, m30, m01, m21, m31, m02, m22, m32 );  adj[ 3 ][ 1 ] =  det3( a );
   initm3( a, m01, m11, m31, m02, m12, m32, m03, m13, m33 );  adj[ 0 ][ 2 ] =  det3( a );
   initm3( a, m00, m10, m30, m02, m12, m32, m03, m13, m33 );  adj[ 1 ][ 2 ] = -det3( a );
   initm3( a, m00, m10, m30, m01, m11, m31, m03, m13, m33 );  adj[ 2 ][ 2 ] =  det3( a );
   initm3( a, m00, m10, m30, m01, m11, m31, m02, m12, m32 );  adj[ 3 ][ 2 ] = -det3( a );
   initm3( a, m01, m11, m21, m02, m12, m22, m03, m13, m23 );  adj[ 0 ][ 3 ] = -det3( a );
   initm3( a, m00, m10, m20, m02, m12, m22, m03, m13, m23 );  adj[ 1 ][ 3 ] =  det3( a );
   initm3( a, m00, m10, m20, m01, m11, m21, m03, m13, m23 );  adj[ 2 ][ 3 ] = -det3( a );
   initm3( a, m00, m10, m20, m01, m11, m21, m02, m12, m22 );  adj[ 3 ][ 3 ] =  det3( a );
}


inline void LWVecMath::inverse3( LWFMatrix3 m, LWFMatrix3 inv )
{
   float det;

   det = det3( m );
   if ( fabs( det ) < EPSILON_F ) return;

   adjoint3( m, inv );
   scalem3( inv, 1.0f / det );
}


inline void LWVecMath::inverse4( LWFMatrix4 m, LWFMatrix4 inv )
{
   float det;

   det = det4( m );
   if ( fabs( det ) < EPSILON_F ) return;

   adjoint4( m, inv );
   scalem4( inv, 1.0f / det );
}


int nxt[] = { 1, 2, 0 };
inline void LWVecMath::mat4_quat( LWFMatrix4 m, float q[ 4 ] )
{
   float tr, s;
   int i, j, k;

   tr = m00 + m11 + m22;
   if ( tr > 0.0f ) {
      s = ( float ) sqrt( tr + 1.0 );
      q[ 3 ] = s * 0.5f;
      s = 0.5f / s;
      q[ 0 ] = ( m12 - m21 ) * s;
      q[ 1 ] = ( m20 - m02 ) * s;
      q[ 2 ] = ( m01 - m10 ) * s;
   }
   else {
      i = 0;
      if ( m11 > m00 ) i = 1;
      if ( m22 > m[ i ][ i ] ) i = 2;
      j = nxt[ i ];
      k = nxt[ j ];
      s = ( float ) sqrt(( m[ i ][ i ] - ( m[ j ][ j ] + m[ k ][ k ] )) + 1.0 );
      q[ i ] = s * 0.5f;
      s = 0.5f / s;
      q[ 3 ] = ( m[ j ][ k ] - m[ k ][ j ] ) * s;
      q[ j ] = ( m[ i ][ j ] + m[ j ][ i ] ) * s;
      q[ k ] = ( m[ i ][ k ] + m[ k ][ i ] ) * s;
   }
}


inline void LWVecMath::quat_mat4( float q[ 4 ], LWFMatrix4 m )
{
   float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

   s = 2.0f / ( q[ 0 ] * q[ 0 ]
              + q[ 1 ] * q[ 1 ]
              + q[ 2 ] * q[ 2 ]
              + q[ 3 ] * q[ 3 ] );

   xs = q[ 0 ] * s;
   ys = q[ 1 ] * s;
   zs = q[ 2 ] * s;
   wx = q[ 3 ] * xs;
   wy = q[ 3 ] * ys;
   wz = q[ 3 ] * zs;
   xx = q[ 0 ] * xs;
   xy = q[ 0 ] * ys;
   xz = q[ 0 ] * zs;
   yy = q[ 1 ] * ys;
   yz = q[ 1 ] * zs;
   zz = q[ 2 ] * zs;

   m00 = 1.0f - ( yy + zz );
   m01 = xy + wz;
   m02 = xz - wy;

   m10 = xy - wz;
   m11 = 1.0f - ( xx + zz );
   m12 = yz + wx;

   m20 = xz + wy;
   m21 = yz - wx;
   m22 = 1.0f - ( xx + yy );

   m03 = m13 = m23 = m30 = m31 = m32 = 0.0f;
   m33 = 1.0f;
}


/* spherical linear interpolation between 2 quaternions */

inline void LWVecMath::slerp( float p[ 4 ], float q[ 4 ], float t, float qt[ 4 ] )
{
   float omega, cosom, sinom, sclp, sclq;
   int i;

   cosom = p[ 0 ] * q[ 0 ]
         + p[ 1 ] * q[ 1 ]
         + p[ 2 ] * q[ 2 ]
         + p[ 3 ] * q[ 3 ];

   if (( 1.0f + cosom ) > EPSILON_F ) {
      if (( 1.0f - cosom ) > EPSILON_F ) {
         omega = ( float ) acos( cosom );
         sinom = ( float ) sin( omega );
         sclp  = ( float )( sin(( 1.0 - t ) * omega ) / sinom );
         sclq  = ( float )( sin( t * omega ) / sinom );
      }
      else {
         sclp = 1.0f - t;
         sclq = t;
      }
      for ( i = 0; i < 4; i++ )
         qt[ i ] = sclp * p[ i ] + sclq * q[ i ];
   }
   else {
      qt[ 0 ] = -p[ 1 ];
      qt[ 1 ] =  p[ 0 ];
      qt[ 2 ] = -p[ 3 ];
      qt[ 3 ] =  p[ 2 ];
      sclp = ( float ) sin(( 1.0 - t ) * HALFPI );
      sclq = ( float ) sin( t * HALFPI );
      for ( i = 0; i < 3; i++ )
         qt[ i ] = sclp * p[ i ] + sclq * qt[ i ];
   }
}


/*
======================================================================
Double precision functions.
====================================================================== */

inline void LWVecMath::initv( LWDVector a, double x, double y, double z )
{
   a[ 0 ] = x;
   a[ 1 ] = y;
   a[ 2 ] = z;
}


inline void LWVecMath::initm3( LWDMatrix3 m,
   double a1, double b1, double c1,
   double a2, double b2, double c2,
   double a3, double b3, double c3 )
{
   m00 = a1;  m01 = b1;  m02 = c1;
   m10 = a2;  m11 = b2;  m12 = c2;
   m20 = a3;  m21 = b3;  m22 = c3;
}


inline void LWVecMath::initm4( LWDMatrix4 m,
   double a1, double b1, double c1, double d1,
   double a2, double b2, double c2, double d2,
   double a3, double b3, double c3, double d3,
   double a4, double b4, double c4, double d4 )
{
   m00 = a1;  m01 = b1;  m02 = c1;  m03 = d1;
   m10 = a2;  m11 = b2;  m12 = c2;  m13 = d2;
   m20 = a3;  m21 = b3;  m22 = c3;  m23 = d3;
   m30 = a4;  m31 = b4;  m32 = c4;  m33 = d4;
}


inline void LWVecMath::identity3( LWDMatrix3 m )
{
   initm3( m,
      1.0, 0.0, 0.0,
      0.0, 1.0, 0.0,
      0.0, 0.0, 1.0 );
}


inline void LWVecMath::identity4( LWDMatrix4 m )
{
   initm4( m,
      1.0, 0.0, 0.0, 0.0,
      0.0, 1.0, 0.0, 0.0,
      0.0, 0.0, 1.0, 0.0,
      0.0, 0.0, 0.0, 1.0 );
}


inline void LWVecMath::copyv( LWDVector to, LWDVector from )
{
   to[ 0 ] = from[ 0 ];
   to[ 1 ] = from[ 1 ];
   to[ 2 ] = from[ 2 ];
}


inline void LWVecMath::copym3( LWDMatrix3 to, LWDMatrix3 from )
{
   memcpy( to, from, sizeof( LWDMatrix3 ));
}


inline void LWVecMath::copym4( LWDMatrix4 to, LWDMatrix4 from )
{
   memcpy( to, from, sizeof( LWDMatrix4 ));
}


inline double LWVecMath::dot( LWDVector a, LWDVector b )
{
   return a[ 0 ] * b[ 0 ] + a[ 1 ] * b[ 1 ] + a[ 2 ] * b[ 2 ];
}


inline double LWVecMath::lenSquared( LWDVector a )
{
   return dot( a, a );
}


inline double LWVecMath::len( LWDVector a )
{
   return ( double ) sqrt( lenSquared( a ));
}


inline void LWVecMath::neg( LWDVector a )
{
   a[ 0 ] = -a[ 0 ];
   a[ 1 ] = -a[ 1 ];
   a[ 2 ] = -a[ 2 ];
}


inline void LWVecMath::scalev( LWDVector a, double s )
{
   a[ 0 ] *= s;
   a[ 1 ] *= s;
   a[ 2 ] *= s;
}


inline void LWVecMath::setlen( LWDVector a, double d )
{
   double d0 = len( a );

   if ( d0 != 0.0 ) scalev( a, d / d0 );
}


inline void LWVecMath::normalize( LWDVector a )
{
   setlen( a, 1.0 );
}


inline void LWVecMath::add( LWDVector a, LWDVector b, LWDVector c )
{
   c[ 0 ] = a[ 0 ] + b[ 0 ];
   c[ 1 ] = a[ 1 ] + b[ 1 ];
   c[ 2 ] = a[ 2 ] + b[ 2 ];
}


inline void LWVecMath::sub( LWDVector a, LWDVector b, LWDVector c )
{
   c[ 0 ] = a[ 0 ] - b[ 0 ];
   c[ 1 ] = a[ 1 ] - b[ 1 ];
   c[ 2 ] = a[ 2 ] - b[ 2 ];
}


inline void LWVecMath::mul( LWDVector a, LWDVector b, LWDVector c )
{
   c[ 0 ] = a[ 0 ] * b[ 0 ];
   c[ 1 ] = a[ 1 ] * b[ 1 ];
   c[ 2 ] = a[ 2 ] * b[ 2 ];
}


inline void LWVecMath::lerp( LWDVector a, LWDVector b, double t, LWDVector c )
{
   c[ 0 ] = a[ 0 ] + ( b[ 0 ] - a[ 0 ] ) * t;
   c[ 1 ] = a[ 1 ] + ( b[ 1 ] - a[ 1 ] ) * t;
   c[ 2 ] = a[ 2 ] + ( b[ 2 ] - a[ 2 ] ) * t;
}


inline void LWVecMath::combine( LWDVector a, LWDVector b, LWDVector c, double sa, double sb )
{
   c[ 0 ] = sa * a[ 0 ] + sb * b[ 0 ];
   c[ 1 ] = sa * a[ 1 ] + sb * b[ 1 ];
   c[ 2 ] = sa * a[ 2 ] + sb * b[ 2 ];
}


inline void LWVecMath::cross( LWDVector a, LWDVector b, LWDVector c )
{
   c[ 0 ] = a[ 1 ] * b[ 2 ] - a[ 2 ] * b[ 1 ];
   c[ 1 ] = a[ 2 ] * b[ 0 ] - a[ 0 ] * b[ 2 ];
   c[ 2 ] = a[ 0 ] * b[ 1 ] - a[ 1 ] * b[ 0 ];
}


inline void LWVecMath::polynorm( LWDVector v1, LWDVector v2, LWDVector vlast, LWDVector norm )
{
   LWDVector a, b;

   sub( v2, v1, a );
   sub( vlast, v1, b );
   cross( a, b, norm );
   normalize( norm );
}


inline double LWVecMath::dist( LWDVector a, LWDVector b )
{
   LWDVector c;

   sub( a, b, c );
   return len( c );
}


inline double LWVecMath::angle( LWDVector a, LWDVector b )
{
   LWDVector na, nb;

   copyv( na, a );
   normalize( na );
   copyv( nb, b );
   normalize( nb );
   return acos( dot( na, nb ));
}


inline void LWVecMath::vec_hp( LWDVector a, double *h, double *p )
{
   LWDVector n;

   copyv( n, a );
   normalize( n );

   *p = ( double ) asin( -n[ 1 ] );
   if ( 1.0 - fabs( n[ 1 ] ) > EPSILON_D ) {
      *h = acos( n[ 2 ] / cos( *p ));
      if ( n[ 0 ] < 0.0 )
         *h = 2 * PI - *h;
   }
   else *h = 0.0;
}


inline void LWVecMath::hp_vec( double h, double p, LWDVector a )
{
   double
      ch = cos( h ),
      sh = sin( h ),
      cp = cos( p ),
      sp = sin( p );

   a[ 0 ] = sh * cp;
   a[ 1 ] = -sp;
   a[ 2 ] = ch * cp;
}


inline void LWVecMath::transform( LWDVector a, LWDMatrix3 m, LWDVector b )
{
   int i;

   for ( i = 0; i < 3; i++ )
      b[ i ] = a[ 0 ] * m[ 0 ][ i ]
             + a[ 1 ] * m[ 1 ][ i ]
             + a[ 2 ] * m[ 2 ][ i ];
}


inline void LWVecMath::transformp( LWDVector a, LWDMatrix4 m, LWDVector b )
{
   double w;
   int i;

   for ( i = 0; i < 3; i++ )
      b[ i ] = a[ 0 ] * m[ 0 ][ i ]
             + a[ 1 ] * m[ 1 ][ i ]
             + a[ 2 ] * m[ 2 ][ i ] + m[ 3 ][ i ];

   w = a[ 0 ] * m[ 0 ][ 3 ]
     + a[ 1 ] * m[ 1 ][ 3 ]
     + a[ 2 ] * m[ 2 ][ 3 ] + m[ 3 ][ 3 ];

   if ( w != 0.0 ) {
      b[ 0 ] /= w;
      b[ 1 ] /= w;
      b[ 2 ] /= w;
   }
}


inline void LWVecMath::matmul3( LWDMatrix3 a, LWDMatrix3 b, LWDMatrix3 c )
{
   int i, j, k;
   LWDMatrix3 m = { 0.0 };

   for ( i = 0; i < 3; i++ )
      for ( j = 0; j < 3; j++ )
         for ( k = 0; k < 3; k++ )
            m[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];

   memcpy( &c[ 0 ][ 0 ], &m[ 0 ][ 0 ], sizeof( LWDMatrix3 ));
}


inline void LWVecMath::matmul4( LWDMatrix4 a, LWDMatrix4 b, LWDMatrix4 c )
{
   int i, j, k;
   LWDMatrix4 m = { 0.0 };

   for ( i = 0; i < 4; i++ )
      for ( j = 0; j < 4; j++ )
         for ( k = 0; k < 4; k++ )
            m[ i ][ j ] += a[ i ][ k ] * b[ k ][ j ];

   memcpy( &c[ 0 ][ 0 ], &m[ 0 ][ 0 ], sizeof( LWDMatrix4 ));
}


inline void LWVecMath::scalem4( LWDMatrix4 a, double s )
{
   int i, j;

   for ( i = 0; i < 4; i++ )
      for ( j = 0; j < 4; j++ )
         a[ i ][ j ] *= s;
}


inline void LWVecMath::scalem3( LWDMatrix3 a, double s )
{
   int i, j;

   for ( i = 0; i < 3; i++ )
      for ( j = 0; j < 3; j++ )
         a[ i ][ j ] *= s;
}


inline double LWVecMath::det2( double a, double b, double c, double d )
{
    return a * d - b * c;
}


inline double LWVecMath::det3( LWDMatrix3 m )
{
   return   m00 * det2( m11, m21, m12, m22 )
          - m01 * det2( m10, m20, m12, m22 )
          + m02 * det2( m10, m20, m11, m21 );
}


inline double LWVecMath::det4( LWDMatrix4 m )
{
   LWDMatrix3 p;
   double a, b, c, d;

   initm3( p, m11, m21, m31, m12, m22, m32, m13, m23, m33 );  a = det3( p );
   initm3( p, m10, m20, m30, m12, m22, m32, m13, m23, m33 );  b = det3( p );
   initm3( p, m10, m20, m30, m11, m21, m31, m13, m23, m33 );  c = det3( p );
   initm3( p, m10, m20, m30, m11, m21, m31, m12, m22, m32 );  d = det3( p );

   return m00 * a - m01 * b + m02 * c - m03 * d;
}


inline void LWVecMath::adjoint3( LWDMatrix3 m, LWDMatrix3 adj )
{
   adj[ 0 ][ 0 ] =  det2( m11, m12, m21, m22 );
   adj[ 1 ][ 0 ] = -det2( m10, m12, m20, m22 );
   adj[ 2 ][ 0 ] =  det2( m10, m11, m20, m21 );
   adj[ 0 ][ 1 ] = -det2( m01, m02, m21, m22 );
   adj[ 1 ][ 1 ] =  det2( m00, m02, m20, m22 );
   adj[ 2 ][ 1 ] = -det2( m00, m01, m20, m21 );
   adj[ 0 ][ 2 ] =  det2( m01, m02, m11, m12 );
   adj[ 1 ][ 2 ] = -det2( m00, m02, m10, m12 );
   adj[ 2 ][ 2 ] =  det2( m00, m01, m10, m11 );
}


inline void LWVecMath::adjoint4( LWDMatrix4 m, LWDMatrix4 adj )
{
   LWDMatrix3 a;

   initm3( a, m11, m21, m31, m12, m22, m32, m13, m23, m33 );  adj[ 0 ][ 0 ] =  det3( a );
   initm3( a, m10, m20, m30, m12, m22, m32, m13, m23, m33 );  adj[ 1 ][ 0 ] = -det3( a );
   initm3( a, m10, m20, m30, m11, m21, m31, m13, m23, m33 );  adj[ 2 ][ 0 ] =  det3( a );
   initm3( a, m10, m20, m30, m11, m21, m31, m12, m22, m32 );  adj[ 3 ][ 0 ] = -det3( a );
   initm3( a, m01, m21, m31, m02, m22, m32, m03, m23, m33 );  adj[ 0 ][ 1 ] = -det3( a );
   initm3( a, m00, m20, m30, m02, m22, m32, m03, m23, m33 );  adj[ 1 ][ 1 ] =  det3( a );
   initm3( a, m00, m20, m30, m01, m21, m31, m03, m23, m33 );  adj[ 2 ][ 1 ] = -det3( a );
   initm3( a, m00, m20, m30, m01, m21, m31, m02, m22, m32 );  adj[ 3 ][ 1 ] =  det3( a );
   initm3( a, m01, m11, m31, m02, m12, m32, m03, m13, m33 );  adj[ 0 ][ 2 ] =  det3( a );
   initm3( a, m00, m10, m30, m02, m12, m32, m03, m13, m33 );  adj[ 1 ][ 2 ] = -det3( a );
   initm3( a, m00, m10, m30, m01, m11, m31, m03, m13, m33 );  adj[ 2 ][ 2 ] =  det3( a );
   initm3( a, m00, m10, m30, m01, m11, m31, m02, m12, m32 );  adj[ 3 ][ 2 ] = -det3( a );
   initm3( a, m01, m11, m21, m02, m12, m22, m03, m13, m23 );  adj[ 0 ][ 3 ] = -det3( a );
   initm3( a, m00, m10, m20, m02, m12, m22, m03, m13, m23 );  adj[ 1 ][ 3 ] =  det3( a );
   initm3( a, m00, m10, m20, m01, m11, m21, m03, m13, m23 );  adj[ 2 ][ 3 ] = -det3( a );
   initm3( a, m00, m10, m20, m01, m11, m21, m02, m12, m22 );  adj[ 3 ][ 3 ] =  det3( a );
}


inline void LWVecMath::inverse3( LWDMatrix3 m, LWDMatrix3 inv )
{
   double det;

   det = det3( m );
   if ( fabs( det ) < EPSILON_D ) return;

   adjoint3( m, inv );
   scalem3( inv, 1.0 / det );
}


inline void LWVecMath::inverse4( LWDMatrix4 m, LWDMatrix4 inv )
{
   double det;

   det = det4( m );
   if ( fabs( det ) < EPSILON_D ) return;

   adjoint4( m, inv );
   scalem4( inv, 1.0 / det );
}


inline void LWVecMath::mat4_quat( LWDMatrix4 m, double q[ 4 ] )
{
   double tr, s;
   int i, j, k;

   tr = m00 + m11 + m22;
   if ( tr > 0.0 ) {
      s = sqrt( tr + 1.0 );
      q[ 3 ] = s * 0.5;
      s = 0.5 / s;
      q[ 0 ] = ( m12 - m21 ) * s;
      q[ 1 ] = ( m20 - m02 ) * s;
      q[ 2 ] = ( m01 - m10 ) * s;
   }
   else {
      i = 0;
      if ( m11 > m00 ) i = 1;
      if ( m22 > m[ i ][ i ] ) i = 2;
      j = nxt[ i ];
      k = nxt[ j ];
      s = sqrt(( m[ i ][ i ] - ( m[ j ][ j ] + m[ k ][ k ] )) + 1.0 );
      q[ i ] = s * 0.5;
      s = 0.5 / s;
      q[ 3 ] = ( m[ j ][ k ] - m[ k ][ j ] ) * s;
      q[ j ] = ( m[ i ][ j ] + m[ j ][ i ] ) * s;
      q[ k ] = ( m[ i ][ k ] + m[ k ][ i ] ) * s;
   }
}


inline void LWVecMath::quat_mat4( double q[ 4 ], LWDMatrix4 m )
{
   double s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

   s = 2.0 / ( q[ 0 ] * q[ 0 ]
             + q[ 1 ] * q[ 1 ]
             + q[ 2 ] * q[ 2 ]
             + q[ 3 ] * q[ 3 ] );

   xs = q[ 0 ] * s;
   ys = q[ 1 ] * s;
   zs = q[ 2 ] * s;
   wx = q[ 3 ] * xs;
   wy = q[ 3 ] * ys;
   wz = q[ 3 ] * zs;
   xx = q[ 0 ] * xs;
   xy = q[ 0 ] * ys;
   xz = q[ 0 ] * zs;
   yy = q[ 1 ] * ys;
   yz = q[ 1 ] * zs;
   zz = q[ 2 ] * zs;

   m00 = 1.0 - ( yy + zz );
   m01 = xy + wz;
   m02 = xz - wy;

   m10 = xy - wz;
   m11 = 1.0 - ( xx + zz );
   m12 = yz + wx;

   m20 = xz + wy;
   m21 = yz - wx;
   m22 = 1.0 - ( xx + yy );

   m03 = m13 = m23 = m30 = m31 = m32 = 0.0;
   m33 = 1.0;
}


/* spherical linear interpolation between 2 quaternions */

inline void LWVecMath::slerp( double p[ 4 ], double q[ 4 ], double t, double qt[ 4 ] )
{
   double omega, cosom, sinom, sclp, sclq;
   int i;

   cosom = p[ 0 ] * q[ 0 ]
         + p[ 1 ] * q[ 1 ]
         + p[ 2 ] * q[ 2 ]
         + p[ 3 ] * q[ 3 ];

   if (( 1.0 + cosom ) > EPSILON_D ) {
      if (( 1.0 - cosom ) > EPSILON_D ) {
         omega = acos( cosom );
         sinom = sin( omega );
         sclp  = sin(( 1.0 - t ) * omega ) / sinom;
         sclq  = sin( t * omega ) / sinom;
      }
      else {
         sclp = 1.0 - t;
         sclq = t;
      }
      for ( i = 0; i < 4; i++ )
         qt[ i ] = sclp * p[ i ] + sclq * q[ i ];
   }
   else {
      qt[ 0 ] = -p[ 1 ];
      qt[ 1 ] =  p[ 0 ];
      qt[ 2 ] = -p[ 3 ];
      qt[ 3 ] =  p[ 2 ];
      sclp = sin(( 1.0 - t ) * HALFPI );
      sclq = sin( t * HALFPI );
      for ( i = 0; i < 3; i++ )
         qt[ i ] = sclp * p[ i ] + sclq * qt[ i ];
   }
}

#endif
