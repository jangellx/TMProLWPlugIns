//
// FreeKeyMotion.cpp
//

#include <assert.h>
#include <math.h>

#include "FreeKeyMotion.h"

//
// FreeKeyMotion Memeber Functions
//

// Destructor
FreeKeyMotion::~FreeKeyMotion() {
  keys.Flush();
}

// NewMotionFromFile():
//  Creates a new FreeKeyMotion and reads in the motion data
//   from the file stream specified.
FreeKeyMotion * FreeKeyMotion::NewMotionFromFile( ifstream &in ) {
  FreeKeyMotion * motion = new FreeKeyMotion();
  if( !motion )
    return NULL;

  // Read in the motion
  if( motion->ReadMotionFromFile( in ) ) {
    return motion;                        // Motion read OK
  } else {
    delete motion;                        // Motion read failed; abort
    return NULL;
  }
}

// ReadMotionFromFile():
//  Reads in a motion from the specified file stream.
//   If append is true, the new keys are added to the end
//   of the original keys.  If false, the old keys are
//   deleted before the new keys are loaded.
//  Note that the file stream should be pointing to the
//   begining of the line following the ObjectMotion/
//   BoneMotion/LightMotion/CameraMotion keyword.
bool FreeKeyMotion::ReadMotionFromFile( ifstream &in, bool append ) {
  if( !in )
    return false;

  // Append mode isn't supported yet.  Write it if you need it.
  //  Add an Insert mode if you want, too.
  assert( append == false );

  // Clear out the keys array (append mode off only)

  int num_keys;
  in >> num_channels;
  in >> num_keys;

  if( !in )
    return false;

  FreeKeyFrame *new_key;
  for( int i=0; i < num_keys; i++ ) {
    new_key = FreeKeyFrame::NewFrameFromFile( in, num_channels );  // Create and read a new keyframe
    if( !new_key )                                                 // Error Reading Key
      return false;

    keys.Add( new_key );                                          // Add the new key to the keyframe array
  }

  return true;
}

// WriteMotionToFile()
//  Writes a motion to a file.  Note that this does NOT write out the
//   ObjectMotion/BoneMotion/etc. line to the file.  This must be done
//   by the user.
bool FreeKeyMotion::WriteMotionToFile( ofstream &out ) {
  if( !out )
    return false;

  if( keys.NumElements() > 0 ) {
    // Find the first key >= 0;
    int first_key = FindKeyFrameIndex( 0, false );

    // Output the number of keys and channels
    int num_keys = keys.NumElements() - first_key;
    if( keys[ first_key ]->GetFrameNumber() > 0 )
      num_keys++;

    out << " " << num_channels << endl;
    out << " " << num_keys << endl;

    // Generate frame 0, if needed
    if( keys[ first_key ]->GetFrameNumber() != 0 ) {
      keys[ first_key ]->WriteFrameToFile( out, 0 );
    }

    // Increment first key if it's less than 0
    if( keys[ first_key ]->GetFrameNumber() < 0 )
      first_key++;

    // Output all the keys
    for( unsigned int i=first_key; i < keys.NumElements(); i++ ) {
      keys[i]->WriteFrameToFile( out );
    }

    if( !out )
      return false;
  } else {
    // Output the default motion
    out << " 9" << endl;
    out << " 1" << endl;
    out << " 0 0 0 0 0 0 0 0 0" << endl;
    out << " 0 0 0 0 0" << endl;
  }

  return true;
}

// DeleteRange():
//  Deletes the keyframes between low and high inclusive.
//   Frame 0 can be deleted. If there are no keys inside
//   the range, this will do nothing.
bool FreeKeyMotion::DeleteRange( int low, int high ) {
  // Make sure low is more than high
  if( low >= high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  for( unsigned int i=0; i < keys.NumElements(); i++ ) {
    if( keys[i]->GetFrameNumber() > high )
      break;

    if( keys[i]->GetFrameNumber() >= low ) {
      // Key is in range; delete it.
      delete keys[i];
      keys.Remove( i-- );   // Since this index now points at a new key, run the index again
    }
  }

  return true;
}

// DeleteStep():
//  Deletes every Xth key in a motion.  If invert == true, then
//   every Xth frame in the motion will not be deleted, but the rest
//   will.  Frame 0 and the last frame are not affected.
bool FreeKeyMotion::DeleteStep( int low, int high, int step, bool invert ) {
  // Make sure the step is positive and non-zero
  if( step < 1 )
    return false;

  // Make sure low is more than high
  if( low >= high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  // Don't do anything if the step is not smaller than the number of keys
  if( (high - low) <= step )
    return true;

  int s = 0;
  // Inverted Step
  for( unsigned int i = 1; i < keys.NumElements()-1; i++ ) {
    if( keys[i]->GetFrameNumber() > high )
      break;

    if( keys[i]->GetFrameNumber() >= low ) {
      // We're now in range; process.
      if( invert ) {
        s++;
        if( s != step ) {
          delete keys[i];
          keys.Remove( i-- );      // i-- is so we do this index again, as this index now marks a different key
        } else {
          s = 0;
        }
      } else {
        // Normal Step
        s++;
        if( s == step ) {
          s = 0;

          delete keys[i];
          keys.Remove( i-- );      // i-- is so we do this index again, as this index now marks a different key
        }
      }
    }
  }

  return true;
}

// ShiftKeys():
//  Shifts a range of  keyframes in a motion.  Frames can be shifted into
//   the negatives, but only frame zero and above will be saved out.
bool FreeKeyMotion::ShiftKeys( int low, int high, int shift ) {
  // Make sure low is more than high
  if( low >= high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  // Figure out how far we can shift.  This handles keyframes
  //  already in the shifted location.
  int max_shift = 0;
  int key = 0;
  if( shift == 0 )
    // No Shift
    return true;
  else if( shift < 0 ) {
    // Shift Backward In Time
    key = FindKeyFrameIndex( low-1 );
    if( key == -1 )
      max_shift = 65535;
    else {
      max_shift = low - keys[ key ]->GetFrameNumber();
      if( max_shift == 1 )                    // See if there's nothing to do
        return true;
    }
  } else {
    // Shift Forward In Time
    key = FindKeyFrameIndex( high+1, false );
    if( key == -1 )
      max_shift = 65535;
    else {
      max_shift = keys[ key ]->GetFrameNumber() - high;
      if( max_shift == 1 )                    // See if there's nothing to do
        return true;
    }
  }

  if( max_shift > abs( shift ) )          // Use shift if max_shift is too high
    max_shift = shift;
  else if( shift < 0 )                    // Make sure max_shift is shifting the right way
    max_shift = -max_shift;

  // Do the shift operation
  for( unsigned int i=0; i < keys.NumElements(); i++ ) {
    if( keys[i]->GetFrameNumber() > high )                // Make sure we're under high
      break;

    if( keys[i]->GetFrameNumber() >= low ) {              // Make sure we're above low
      keys[i]->SetFrameNumber( keys[i]->GetFrameNumber() + max_shift ); // Shift the key
    }
  }

  return true;
}

// ScaleKeys():
//  Scales a range of keyframes in a motion.  This works exactly like
//   LW's Scale Keys features: if keys are too close to each other,
//   they are bunched up.  Keys beyond the high value are shifted
//   backward the appropriate amount.  Feel free to a dropped-key
//   support if you need it.
bool FreeKeyMotion::ScaleKeys( int low, int high, float scale ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  int high_shift = 0;                      // Amount to shift keys beyond the scale range backward
  int before_scale;                        // The keyframe of the highest key before being scaled
  int min_key = 0;                         // Minimum frame the key can be (handles fractional frame numbers)
  int range   = high - low;                // The range between the high and low keys, on which scaling is actually performed
  int new_frame;
  // Do the scale operation
  for( unsigned int i=0; i < keys.NumElements(); i++ ) {
    if( keys[i]->GetFrameNumber() > high )                                // Make sure we're under high
      keys[i]->SetFrameNumber( keys[i]->GetFrameNumber() + high_shift );  //   Shift the keys beyond the scale range backward

    if( keys[i]->GetFrameNumber() >= low ) {                              // Make sure we're above low
      before_scale = keys[i]->GetFrameNumber();                           //   Used later if this is the last shifted key

      new_frame = (int)((keys[i]->GetFrameNumber() - low) * scale);       //   Compute the scaled offset
      new_frame = low + new_frame;                                        //   Add the new offset to the start of the range to get the new key position
      if( new_frame < min_key )                                           //   Make sure the key isn't too low
        new_frame = min_key;

      keys[i]->SetFrameNumber( new_frame );                               //   Actually set the key (finally)
      high_shift = keys[i]->GetFrameNumber() - before_scale;
    }

    if( min_key == keys[i]->GetFrameNumber() )                            // Store this as the min. final shifted key position
      min_key++;
    else
      min_key = keys[i]->GetFrameNumber();
  }
  
  return true;
}


// ShiftValues():
//  Shifts all the values of a range of keyframes in a motion.
bool FreeKeyMotion::ShiftValues( int low, int high, FreeKeyFrame::chan_IDs chan, float offset ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  // Do the shift operation
  for( unsigned int i=0; i < keys.NumElements(); i++ ) {
    if( keys[i]->GetFrameNumber() > high )                // Make sure we're under high
      break;

    if( keys[i]->GetFrameNumber() >= low )                // Make sure we're above low
      keys[i]->SetChannel( chan, keys[i]->GetChannel( chan ) + offset );  // Shift the channel's value
  }

  return true;
}

// SetValues():
//  Sets all the values of a range of keyframes in a motion.
bool FreeKeyMotion::SetValues( int low, int high, FreeKeyFrame::chan_IDs chan, float offset ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetLastFrameNumber() < low )
    return true;

  // Do the shift operation
  for( unsigned int i=0; i < keys.NumElements(); i++ ) {
    if( keys[i]->GetFrameNumber() > high )                // Make sure we're under high
      break;

    if( keys[i]->GetFrameNumber() >= low )                // Make sure we're above low
      keys[i]->SetChannel( chan, offset );                // Set the channel's value
  }

  return true;
}

// JitterDampening():
//  Attempts to reduce the amount of jitter in a the specified motion channel.
//   within the given range.  The Threshold is the highest amount of difference
//   along a motion that will be considered jitter.
//  Note that this really only works well on all-keyed data, like MoCap, and
//   may cause unpredictable results on hand-animated motions.
//
//  THIS DOESN'T DO ANYTIHNG YET
//
bool FreeKeyMotion::JitterDampening( int low, int high, float threshold,
                                     FreeKeyFrame::chan_IDs chan ) {

  // Test to see if the threshold non-zero
  threshold = (float)fabs( threshold );
  if( threshold < 0.00001 )
    return true;

  unsigned int j;
  int high_key;
  float v1, v2, diff;

  for( unsigned int i=0; i < keys.NumElements()-1; i++ ) {
    high_key = -1;
    v1 = keys[i]->GetChannel( chan );                   // Get the base key's value

    // Find the last key within the threshold
    for( j=i+1; j < keys.NumElements(); j++ ) {
      v2 = keys[j]->GetChannel( chan );                 // Get the compare key's value

      // Get the difference between the two values
      if( v1 > v2 )
        diff = v1 - v2;
      else
        diff = v2 - v1;

      if( diff > threshold )
        high_key = j-1;
        break;
    }

    if( high_key != (int)i ) {
      // Average all the keys together to remove the jitter

    }
  }
}
  

// FindKeyFrame():
//  Finds the closest keyframe to the frame value specified.  By default,
//   the key equal to or less than will be returned.  If round_down is false,
//   the first key with an equal or greater value will be returned.  If no
//   match is found, -1 is returned.
//  This is a pretty simple search routine.  Write a faster one if needed.
int FreeKeyMotion::FindKeyFrameIndex( int frame, bool round_down ) {
  if( round_down ) {
    // Round Down

    // Handle first frame being greater than the frame we want
    if( round_down ) {
      if( keys[0]->GetFrameNumber() >= frame )
        return -1;
    } else {
      if( keys[ keys.NumElements() - 1 ]->GetFrameNumber() <= frame )
        return -1;
    }

    int best_match = keys[0]->GetFrameNumber();
    for( unsigned int i=0; i < keys.NumElements(); i++ ) {
      if( keys[i]->GetFrameNumber() > frame+1 )
        break;

      if( keys[i]->GetFrameNumber() < frame+1 )  // The +1 means that we only check for frames less than this.  If there is an equal key, this will be it.
        best_match = i;
    }
    return best_match;

    // Not found, so it must be the last key; return that instead.
    return i;
  } else {
    // Round Up
    for( unsigned int i=0; i < keys.NumElements(); i++ ) {
      if( keys[i]->GetFrameNumber() >= frame )
        return i;
    }

    // No match; return -1
    return -1;
  }

  // This should never be called
  return -1;
}


//
// FreeKeyFrame Memeber Functions
//

// Constructor
FreeKeyFrame::FreeKeyFrame( int _num_channels )
  : num_channels( _num_channels ), frame(0),
    tens(0.0f), cont(0.0f), bias(0.0f), linear(false) {
  
  channels = new float[ _num_channels ];
}

// Destructor
FreeKeyFrame::~FreeKeyFrame() {
  delete channels;
}

// NewFrameFromFile():
//  Creates a new FreeKeyFrame and reads in the motion data
//   from the file stream specified.
FreeKeyFrame * FreeKeyFrame::NewFrameFromFile( ifstream &in, int _num_channels ) {
  FreeKeyFrame * frame = new FreeKeyFrame();
  if( !frame )
    return NULL;

  // Read in the frame
  if( frame->ReadFrameFromFile( in ) ) {
    return frame;                        // Frame read OK
  } else {
    delete frame;                        // Frame read failed; abort
    return NULL;
  }
}

// ReadFrameFromFile():
//  Reads in a Frame from the specified file stream.
bool FreeKeyFrame::ReadFrameFromFile( ifstream &in ) {
  if( !in )
    return false;

  in >> channels[x]  >> channels[y]  >> channels[z]
     >> channels[h]  >> channels[p]  >> channels[b]
     >> channels[sx] >> channels[sy] >> channels[sz];
     
  in >> frame >> linear >> tens >> cont >> bias;

  if( !in )
    return false;

  return true;
}

// WriteFrameToFile():
//  Writes a frame to the specified file stream.  If rep_frame is >0, it
//   will be used as the frame number instead of the value stored in the
//   instance.
bool FreeKeyFrame::WriteFrameToFile( ofstream &out, int rep_frame ) {
  if( !out )
    return false;

  out << " " << channels[x]  << " " << channels[y]  << " " << channels[z]  << " "
             << channels[h]  << " " << channels[p]  << " " << channels[b]  << " "
             << channels[sx] << " " << channels[sy] << " " << channels[sz] << endl;

  if( rep_frame != -1 )
    out << " " << rep_frame;
  else
    out << " " << frame;

  out << " " << linear << " " << tens << " " << cont << " " << bias << endl;

  if( !out )
    return false;

  return true;
}