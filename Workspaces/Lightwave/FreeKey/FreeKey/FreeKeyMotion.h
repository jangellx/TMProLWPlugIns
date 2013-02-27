//
// FreeKeyMotion.h
//
// This contains the FreeKeyFrame and FreeKeyMotion classes.
//  FreeKeyFrame contains all the data for a single keyframe.
//  FreeKeyMotion contains all the kyes for a single motion.
//
// Note that negative keyframes are allowed.  When saving, the
//  first positive keyframe (or frame 0) will be saved, and the
//  negative keys discarded.
//
// Note that is is possible to delete keyframe 0. When saving the
//  motion and keyframe 0 doesn't exists, the first key will be
//  copied as keyframe 0.  If there are no positive keyframes and
//  no frame 0, the last negative key will be copied as frame 0.
//  If there aren't any keyframes, the default 0 keyframe (all
//  values 0) will be written.
//

#ifndef HEADER_FREEKEYMOTION
#define HEADER_FREEKEYMOTION

#include <fstream.h>

#include <portable/dynarray/dynarray.h>

class FreeKeyFrame;
class FreeKeyMotion;

// class FreeKeyFrame
class FreeKeyFrame {
public:
  // Enums
  enum chan_IDs {
    env = 0,          // Use for envelopes
    x   = 0, y, z,    // Use for position values
    h, p, b,          // Use for rotation values
    sx, sy, sz,       // Use for size values
  };

  // Constructors/Destructors
  FreeKeyFrame( int _num_channels = 9 );
  ~FreeKeyFrame();

  // File I/O
  static FreeKeyFrame * NewFrameFromFile(  ifstream &in, int _num_channels );   // Returns a pointer to a new key frame read in from a file
         bool           ReadFrameFromFile( ifstream &in );                      // Reads a keyframe from a file into this instance
         bool           WriteFrameToFile(  ofstream &out, int rep_frame = -1 ); // Writes a keyframe out to a file

  // Accessors
  int  GetFrameNumber() { return frame; }
  void SetFrameNumber( int _new_frame ) { frame = _new_frame; }

  void SetPosition( float _x,  float _y,  float _z  ) { channels[x]  = _x;  channels[y]  = _y;  channels[z]  = _z;  }
  void SetRotation( float _h,  float _p,  float _b  ) { channels[h]  = _h;  channels[h]  = _p;  channels[b]  = _b;  }
  void SetScale(    float _sx, float _sy, float _sz ) { channels[sx] = _sx; channels[sy] = _sy; channels[sz] = _sz; }

  float GetChannel( chan_IDs id ) { return channels[ id ]; }
  void  SetChannel( chan_IDs id, float value ) { channels[ id ] = value; }

  float GetTension()     { return tens; }
  float GetContinuity()  { return cont; }
  float GetBias()        { return bias; }
  int   GetLinear()      { return linear; }

  void SetTension(    float t   ) { tens   = t;   }
  void SetContinuity( float c   ) { cont   = c;   }
  void SetBias(       float b   ) { bias   = b;   }
  void SetLinear(     int   lin ) { linear = lin; }

protected:
  int    frame;

  int    num_channels;
  float *channels;

  float  tens, cont, bias;
  int    linear;
};


// class FreeKeyMotion
class FreeKeyMotion {
public:
  FreeKeyMotion() : num_channels(9) {;}
  ~FreeKeyMotion();

  // File I/O
  static FreeKeyMotion * NewMotionFromFile(  ifstream &in );                          // Returns a pointer to a new motion read in from a file
         bool            ReadMotionFromFile( ifstream &in, bool append = false );     // Reads a motion from a file into this instance
         bool            WriteMotionToFile(  ofstream &out );                         // Writes a motion out to a file

  // Accessors
  inline int GetNumChannels() { return num_channels; }                                // Returns the number of channels each key has
  inline int GetFirstFrameNumber();                                                   // Returns the first keyframe's frame number
  inline int GetLastFrameNumber();                                                    // Returns the last keyframe's frame number

  // Management
  bool DeleteRange( int low, int high );                                              // Deletes the keys between low and high inclusive.
  bool DeleteStep(  int low, int high, int step, bool invert = false );               // Delete the every Xth frame (or keep every Xth frame if invert == true );

  bool ShiftKeys( int low, int high, int   shift );                                   // Shifts a motion forward or backward in time.
  bool ScaleKeys( int low, int high, float scale );                                   // Scales a motion in time.
  bool ShiftValues( int low, int high, FreeKeyFrame::chan_IDs chan, float offset );   // Shifts a motion's keyframe values (pos/rot/scale)

  bool SetValues( int low, int high, FreeKeyFrame::chan_IDs chan, float offset );     // Sets a motion's keyframe values (pos/rot/scale)

  int  FindKeyFrameIndex( int frame, bool round_down = true );                        // Find the closest keyframe.

  // Manipulation
  bool JitterDampening( int low, int high, float threshold, FreeKeyFrame::chan_IDs chan );  // Attempts to reduce jitter on a motion channel

  // Keyframe array
  DynArray< FreeKeyFrame * > keys;

protected:
  int num_channels;
};

// GetFirstFrameNumber()
inline int FreeKeyMotion::GetFirstFrameNumber() {
  if( keys.NumElements() > 0 )
    return keys[0]->GetFrameNumber();
  else
    return 0;
}

// GetLastFrameNumber()
inline int FreeKeyMotion::GetLastFrameNumber() {
  if( keys.NumElements() > 0 )
    return keys[keys.NumElements()-1]->GetFrameNumber();
  else
    return 0;
}

#endif