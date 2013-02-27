//
// LWScene-Spline.h
//
// This contains all of the spline types available in Lightwave.
//  This is mainly used by the LWKeyframe class for spline data
//  storage and for solving for in-between frames.
//

#ifndef HEADER_LWSCENE_SPLINE
#define HEADER_LWSCENE_SPLINE

// Spline Types
enum LWSplineType {
  LWSPLINE_NONE,
  LWSPLINE_TCB,
  LWSPLINE_HERMITE,
  LWSPLINE_BEZIER,
  LWSPLINE_LINEAR,
  LWSPLINE_STEPPED,
  NUM_LWSPLINE_TYPES };


// Spline Classes
class LWSplineBase;

class LWSplineTCB;
class LWSplineHermite;
class LWSplineBezier;
class LWSplineLinear;
class LWSplineStepped;

// Class LWSplineBase
class LWSplineBase {
public:
  LWSplineBase( LWSplineType _type ) : type(_type) { ; }

  LWSplineType GetType() { return type; }

protected:
  LWSplineType type;
};

// Class LWSplineTCB
class LWSplineTCB : public LWSplineBase {
public:
  LWSplineTCB() : LWSplineBase( LWSPLINE_TCB ),
                  tension(0.0), continuity(0.0), bias(0.0), linear(false) { ; }

   // Accessors
   void   SetTension( double value ) { tension = value; }
   double GetTension() { return tension; }

   void   SetContinuity( double value ) { continuity = value; }
   double GetContinuity() { return continuity; }

   void   SetBias( double value ) { bias = value; }
   double GetBias() { return bias; }

   void   SetLinear( bool state ) { linear = state; }
   bool   GetLinear() { return linear; }


protected:
  double tension;
  double continuity;
  double bias;
  
  bool   linear;          // Not used in L6-type TCB splines, but still present for backward compatability
};

// Class LWSplineHermite
class LWSplineHermite : public LWSplineBase {
public:
  LWSplineHermite() : LWSplineBase( LWSPLINE_HERMITE ) { ; }

};

// Class LWSplineBezier
class LWSplineBezier : public LWSplineBase {
public:
  LWSplineBezier() : LWSplineBase( LWSPLINE_BEZIER ) { ; }

};

// Class LWSplineLinear
class LWSplineLinear : public LWSplineBase {
public:
  LWSplineLinear() : LWSplineBase( LWSPLINE_LINEAR ) { ; }

};

// Class LWSplineStepped
class LWSplineStepped : public LWSplineBase {
public:
  LWSplineStepped() : LWSplineBase( LWSPLINE_STEPPED ) { ; }

};

// Spline Union
union LWSpline {
  LWSplineBase     *base;
  LWSplineTCB      *tcb;
  LWSplineHermite  *hermite;
  LWSplineBezier   *bezier;
  LWSplineLinear   *linear;
  LWSplineStepped  *stepped;
};

#endif