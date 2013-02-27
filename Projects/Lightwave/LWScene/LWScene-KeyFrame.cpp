//
// LWKeyFrame.cpp
//

#include "LWScene-KeyFrame.h"

// Constructor
LWKeyFrame::LWKeyFrame( double _value, LWSplineType _stype, double _time)
  : value(_value), time(_time) {

  spline.base = NULL;
  ChangeSplineType( _stype );
}

// Destructor
LWKeyFrame::~LWKeyFrame() {
 ChangeSplineType( LWSPLINE_NONE );
}

// ChangeSplineType():
//  Changes the spline type.  This results in any existing spline type
//   being deleted.  If LWSPLINE_NONE is passed in, then the spline will be cleared
//   and no new spline created (used during destruction).
LWSpline & LWKeyFrame::ChangeSplineType( LWSplineType type ) {
  // Free the old spline data
  if( spline.base != NULL ) {
    switch( spline.base->GetType() ) {
    case LWSPLINE_TCB:
      delete spline.tcb;
      break;

    case LWSPLINE_HERMITE:
      delete spline.hermite;
      break;

    case LWSPLINE_BEZIER:
      delete spline.bezier;
      break;

    case LWSPLINE_LINEAR:
      delete spline.linear;
      break;

    case LWSPLINE_STEPPED:
      delete spline.stepped;
      break;
    };

    spline.base = NULL;
  }

  // Allocate the new spline data, if applicable
  if( type == LWSPLINE_NONE )
    return spline;

  switch( type ) {
  case LWSPLINE_TCB:
    spline.tcb = new LWSplineTCB;
    break;

  case LWSPLINE_HERMITE:
    spline.hermite = new LWSplineHermite;
    break;

  case LWSPLINE_BEZIER:
    spline.bezier = new LWSplineBezier;
    break;

  case LWSPLINE_LINEAR:
    spline.linear = new LWSplineLinear;
    break;

  case LWSPLINE_STEPPED:
    spline.stepped = new LWSplineStepped;
    break;
  };

  return spline;
}
