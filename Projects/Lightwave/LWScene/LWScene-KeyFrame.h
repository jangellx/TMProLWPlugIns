//
// LWScene-Keyframe.h
//
// A LWKeyframe contains all the values that exists at a particular
//  frame in a Lightwave scene.  It is the basis of a LWChannel.
//

#ifndef HEADER_LWSCENE_KEYFRAME
#define HEADER_LWSCENE_KEYFRAME

#include <portable/dynarray/dynarray.h>
#include <portable/pfstream/pfstream.h>

#include "LWScene-Spline.h"

class LWKeyFrame;

class LWKeyFrame {
public:
  LWKeyFrame( double _value = 0.0, LWSplineType _stype = LWSPLINE_TCB, double _time = 0.0 );
  ~LWKeyFrame();

  // File I/O Functions
  void LoadKeyFrame( pifstream &in, int mode, int version );  // Loads a keyframe from a scene/mot/env/etc. file
  
  // Accessors
  double GetValue() { return value; }
  void   SetValue( double new_value ) { value = new_value; }

  double GetTime() { return time; }
  void   SetTime( double new_time ) { time = new_time; }

  // Spline Control
  LWSpline & GetSpline() { return spline; }
  LWSpline & ChangeSplineType( LWSplineType type );

protected:
  double value;                   // Value at this frame
  double time;                    // Time that this key exists at

  LWSpline spline;                // Spline Settings
};

#endif