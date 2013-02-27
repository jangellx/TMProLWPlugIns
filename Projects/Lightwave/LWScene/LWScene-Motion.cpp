//
// LWScene-Motion.cpp
//

#include <portable/stringtools/stringtools.h>

#include "LWScene-Motion.h"
#include "LWScene.h"

// Constructor
LWMotion::LWMotion( int num_channels, bool link ) : link_channels( link ) {
  // Create the requested number of channels
  for( int i=0; i < num_channels; i++ )
    AddChannel();
}

// Destructor
LWMotion::~LWMotion() {
  // Free the motion channels
  channel.Flush();
}

// AddChannel():
//  Adds the channel passed in to the motion.  If NULL, then it creates
//   a new channel and adds it.  Returns the index of the new channel.
//   Channels are always added to the end of the channel list.
//  Note that once added, the channel belongs to the motion, and will
//   be freed when the motion is freed unless removed with RemoveChannel().
unsigned int LWMotion::AddChannel( LWChannel *new_chan ) {
  if( new_chan == NULL )
    channel.Add( new LWChannel );
  else 
    channel.Add( new_chan );

  return channel.NumElements() - 1;
}

// RemoveChannel():
//  Removes the a channel from the motion, but does not free the
//   memory it uses.  Returns false if the channel doesn't exist
bool LWMotion::RemoveChannel( unsigned int index ) {
  if( index < channel.NumElements() ) {
    channel.Remove( index );
    return true;
  } else
    return false;
}

// DeleteChannel():
//  Deletes the a channel from the motion, freeing the memory
//   it used.  Returns false if the channel doesn't exist
bool LWMotion::DeleteChannel( unsigned int index ) {
  if( index < channel.NumElements() ) {
    delete channel[index];
    channel.Remove( index );
    return true;
  } else
    return false;
}

// LoadMotion():
//  Loads a motion from a scene file.  If mode doesn't contain
//   LWS_LOAD_MOTION, nothing will be loaded, but any existing
//   motion data will be cleared.
//  version let's the loader know what to expect from the scene
//   file.  If the file argument is provided, then that file will
//   be tested for a valid filename, and will be loaded instead
//   of the motion (the rest of the motion in the scene will be
//   skipped), as LW 5.6 does.  fps is used to convert from
//   the frames stored in the scene file to the times used
//   by LWKeyFrame classes.  The default of 30 is consistant
//   with LW 5.6 scenes.
int LWMotion::LoadMotion( pifstream &in, int mode, int version, const char *file, double fps ) {
  if( fps == 0 )
    return LWSERROR_FPS_CANT_BE_ZERO;

  int error;

  // Test the file argument
  if( file != NULL ) {
    if( stricmp( file, "(unnamed)" ) != 0 ) {
      error = LoadMotionFile( file );
      if( error == LWSERROR_OK )                    // If the motion loaded from the file OK, return.  Otherwise, try loading from the .LWS
        return error;
    }
  }

  // Free all the motion channels
  unsigned int i;
  unsigned int num_chans = channel.NumElements();
  char line[8192];
  channel.Flush();

  // See if we should load anything
  if( !(mode & LWS_LOAD_MOTIONS ) ) {
    for( i=0; i < num_chans; i++ )   // Put back num_chans empty channels
      AddChannel();

    // Skip the rest of the motion data
    do {
      if( (in.bad() || in.fail()) && !in.eof() )
        return LWSERROR_ERROR_READING_MOTION;

      if( in.eof() )
        return LWSERROR_UNEXPECTED_EOF;
      
      in.getline( line, 8192 );
    } while( strncmp( line, "EndBehavior", 12 ) != 0 );
  }

  // Version-Specific Motion Loaders
  if( version == 1 ) {
    return ReadVersion1Motion( in, fps );
  } else if (version == 2) {                 // LW 5.7
    return ReadVersion2Motion( in );
  } else if (version == 3) {                 // L6
    return LWSERROR_UNSUPPORTED_VERSION;
  } else                                     // Unknown?!?
    return LWSERROR_UNSUPPORTED_VERSION;
}

// ReadVersion1Motion():
//  Reads a Version 1 Motion from the stream provided
int LWMotion::ReadVersion1Motion( pifstream &in, double fps ) {
  unsigned int i, j, num_chans;
  char line[8192];

  // Get the number of channels
  in.getline( line, 8192 );
  num_chans = atoi( line );
  if( num_chans < 1 )                        // Test for motion channels
    return LWSERROR_OK;

  // Create our channels
  for( i=0; i < num_chans; i++ )
    AddChannel();

  // Get the number of keys
  in.getline( line, 8192 );
  unsigned int num_keys = atoi( line );

  char   * temp;
  double   value;
  int      frame, linear, tens, cont, bias;

  // Keyframe Loop
  for( i=0; i < num_keys; i++ ) {          // Read in each key...
    in.getline( line, 8192 );
    temp = line;

    // Channel Data
    for( j=0; j < num_chans; j++ ){     // ...on each channel...
      temp = StringTools::SkipWhiteSpace( temp );
      value = atof( temp );
      GetChannel(j).CreateKey( new LWKeyFrame( value, LWSPLINE_TCB ), false );
      
      temp = StringTools::FindWhiteSpace( temp );
    }

    // Load the rest of the data
    in.getline( line, 8192 );
    temp = line;
    frame = atoi( temp );
    temp = StringTools::FindWord( temp, 1 );
    linear = atoi( temp );
    temp = StringTools::FindWord( temp, 1 );
    tens = atoi( temp );
    temp = StringTools::FindWord( temp, 1 );
    cont = atoi( temp );
    temp = StringTools::FindWord( temp, 1 );
    bias = atoi( temp );

    // Frame Number and Spline Data
    for( j=0; j < num_chans; j++ ){     // ... and get the frame number and spline data
      GetChannel(j)[i].SetTime( frame/fps );
      GetChannel(j)[i].GetSpline().tcb->SetLinear( linear == 1 ? true : false );
      GetChannel(j)[i].GetSpline().tcb->SetTension( tens );
      GetChannel(j)[i].GetSpline().tcb->SetContinuity( cont );
      GetChannel(j)[i].GetSpline().tcb->SetBias( bias );
    }
  }

  // Sort the key arrays
  for( i=0; i < num_chans; i++ )
    GetChannel(i).SortKeys( SortLWKeyFrameByTime );

  // Read the EndBehavior
  in.getline( line, 8192 );
  if( strncmp( line, "EndBehavior", 11 ) != 0 )
    return LWSERROR_MISSING_END_BEHAVIOR;

  int eb = atoi( &(line[12]) );
  for( i=0; i < channel.NumElements(); i++ ) {
    channel[i]->SetPreBehavior( LWCHANB_CONSTANT );
    channel[i]->SetPostBehavior( (LWChanBehavior)eb );
  }

  return LWSERROR_OK;
}

// ReadVersion2Motion():
//  Reads a Version 2 Motion from the stream provided
int LWMotion::ReadVersion2Motion( pifstream &in ) {
  unsigned int i, num_chans;
  int error;
  char line[8192];

  // Get the number of channels
  in.getline( line, 8192 );
  num_chans = atoi( line );
  if( num_chans < 1 )                        // Test for motion channels
    return LWSERROR_OK;

  // Channel Loop
  for( i=0; i < num_chans; i++ ) {
    AddChannel();

    // Read the channel from the file
    error = channel[i]->ReadVersion2Channel( in );
    if( error != LWSERROR_OK )
      return error;
  }

  // Read the EndBehavior
  in.getline( line, 8192 );
  if( strncmp( line, "EndBehavior", 11 ) != 0 )
    return LWSERROR_MISSING_END_BEHAVIOR;

  int eb;
  char * temp = line;
  for( i=0; i < channel.NumElements(); i++ ) {
    temp = StringTools::FindWord( temp, 1 );
    eb = atoi( temp );
    channel[i]->SetPreBehavior( LWCHANB_CONSTANT );
    channel[i]->SetPostBehavior( (LWChanBehavior)eb );
  }

  return LWSERROR_OK;
}

// LoadMotionFile():
//  Loads a .MOT from disk.  Returns a LWSERROR enum
int LWMotion::LoadMotionFile( const char *file ) {
  return LWSERROR_UNSUPPORTED_MOT_VERSION;
}

// SaveMotion():
//  Saves a motion from a scene file. version let's the saver
//   know what format to store that data as. fps is used to
//   convert from the times stored internally to frames for
//   saving into the scene file.  The default of 30 is
//   consistant with LW 5.6 scenes.
//  Note that if a version 1 scene file is being saved, all
//   channels MUST have keys on the same frame for each
//   channel.  The system does NOT currently convert between
//   independantly keyed and uniformly keyed data.  This
//   restriction doesn't exist for version 2 and 3 scenes.
int LWMotion::SaveMotion( pofstream &out, int version, double fps ) {
  if( fps == 0 )
    return LWSERROR_FPS_CANT_BE_ZERO;

  // Version-Specific Motion Savers
  if( version == 1 ) {
    return WriteVersion1Motion( out, fps );
  } else if (version == 2) {                 // LW 5.7
    return WriteVersion2Motion( out );
  } else if (version == 3) {                 // L6
    return LWSERROR_UNSUPPORTED_VERSION;
  } else                                     // Unknown?!?
    return LWSERROR_UNSUPPORTED_VERSION;
}

// WriteVersion1Motion():
//  Outputes a Version 1 motion to the stream probided.
int LWMotion::WriteVersion1Motion( pofstream &out, double fps ) {
  if( channel.NumElements() < 1 )                      // Test for motion channels
    return LWSERROR_NO_MOTION_CHANNELS;

  unsigned int i;
  // Find the first non-negative keyframe
  for( i=0; i < channel[0]->GetKeys().NumElements(); i++ ) {
    if( channel[0]->GetKeys()[i]->GetTime() >= 0.0 )
      break;
  }

  // Output Basic Motion Data
  out << "  " << channel.NumElements() << out.endl();                 // Write the number of channels

  // Write the Number of Keys and Frame Zero, if one doesn't exist in the motion
  if( channel[0]->GetKeys().NumElements() == 0 ) {                    // Handle no keys in motion (write a default key)
    out << "  1" << out.endl();
    out << "  0 0 0 0 0 0 1 1 1" << out.endl();
    out << "  0 0 0 0 0" << out.endl();
  } else if( channel[0]->GetKeys().NumElements() == i ) {             // Handle no keys at or above Frame 0 in motion (write last negative key)
    out << "  1" << out.endl();
    WriteVersion1Keyframe( out, i-1, fps, true );
  } else if( channel[0]->GetKeys()[i]->GetTime() > 0.0 ) {            // Handle no keys at Frame 0 (write first positive keyframe)
    out << "  " << channel[0]->GetKeys().NumElements()-i + 1 << out.endl();   // Write the number of keys, accounting for the extra key needed for Frame 0
    WriteVersion1Keyframe( out, i, fps, true );
  } else 
    out << "  " << channel[0]->GetKeys().NumElements()-i << out.endl();       // Write the number of keys

  // Write each keyframe
  for( /*NULL*/; i < channel[0]->GetKeys().NumElements(); i++ ) {          // Write out each key's data
    WriteVersion1Keyframe( out, i, fps );
    if( out.bad() || out.fail() )
      return LWSERROR_ERROR_WRITING_MOTION;
  }

  // End Behavior
  out << "EndBehavior " << channel[0]->GetPostBehavior() << out.endl();

  if( out.bad() || out.fail() )
    return LWSERROR_ERROR_WRITING_MOTION;

  return LWSERROR_OK;
}

// WriteVersion1Keyframe():
//  Outputs a Version 1 (LW 5.6 and earlier) keyframe to the stream provided.
//   The keyframe index is output to the stream.  If frame_zero is true, then
//   the keyframe index will be output as Frame 0.
bool LWMotion::WriteVersion1Keyframe( pofstream &out, unsigned int index, double fps, bool frame_zero ) {
  out << " ";
  for( unsigned int j=0; j < channel.NumElements(); j++ ) {
    if( channel[j]->GetKeys().NumElements() > index )           // Handle channels with too few keys
      out << " " << channel[j]->GetKeys()[index]->GetValue();
    else
      out << " " << (j < 6) ? "0" : "1";                        // Handle scale and pos/rot differently
  }
  out << out.endl();

  if( frame_zero )
    out << "  0";                                                       // Write out as frame zero
  else {
    if( (channel[0]->GetKeys()[index]->GetTime() * 30) - ((float)((int)(channel[0]->GetKeys()[index]->GetTime() * 30))) > .9 )
      out << "  " << (int)(channel[0]->GetKeys()[index]->GetTime() * fps) + 1; // Hack; eliminates round-off error, which sometimes caused values very near a whole number to be written out as one under that number (ie: .99999999999999999999 being written as 0 instead of 1)
    else
      out << "  " << (int)(channel[0]->GetKeys()[index]->GetTime() * fps);     // Write out the key's frame
  }

  LWSpline *spline = &channel[0]->GetKeys()[index]->GetSpline();
  if( spline->base->GetType() == LWSPLINE_TCB ) {
    out << " " << (spline->tcb->GetLinear() ? 1 : 0)                           // Write out the spline data
        << " " << spline->tcb->GetTension()
        << " " << spline->tcb->GetContinuity()
        << " " << spline->tcb->GetBias() << out.endl();
  } else                                                        // Handle channels with the wrong spline type
    out << " 0 0 0 0" << out.endl();

  return true;
}

// WriteVersion2Motion():
//  Outputes a Version 2 motion to the stream probided.
int LWMotion::WriteVersion2Motion( pofstream &out ) {
  if( channel.NumElements() < 1 )                      // Test for motion channels
    return LWSERROR_NO_MOTION_CHANNELS;

  // Output Basic Motion Data
  out << "  " << channel.NumElements() << out.endl();

  // Write each channel's data
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    channel[i]->WriteVersion2Channel( out );
    if( out.bad() || out.fail() )
      return LWSERROR_ERROR_WRITING_MOTION;
  }

  // End Behavior
  out << "EndBehavior";
  for( i = 0; i < channel.NumElements(); i++ )
    out << " " << channel[i]->GetPostBehavior();
  out << out.endl();

  if( out.bad() || out.fail() )
    return LWSERROR_ERROR_WRITING_MOTION;

  return LWSERROR_OK;
}

// DeleteRange():
//  Deletes the keys between low and high inclusive.
bool LWMotion::DeleteRange( double low, double high ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->DeleteRange( low, high ) )
      return false;
  }

  return true;
}

// DeleteStep():
//  Deletes a key every Xth frame within low and high inclusive.
//   if( invert is true, then all but every Xth frame within the
//   range will be deleted.
//  step_size is the how much time a step covers.  For example, if
//   the step_size is 1.0 and the step is 2.0, then after every 2
//   seconds one second of motion will be removed.
bool LWMotion::DeleteStep( double low, double high, double step, double step_size, bool invert ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->DeleteStep( low, high, step, step_size, invert ) )
      return false;
  }

  return true;
}

// ShiftKeys():
//  Shifts the keys within the range inclusive by the time
//   delta provided.
bool LWMotion::ShiftKeys( double low, double high, double shift,
                          LWChanKeyPost mode, double min_spacing ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->ShiftKeys( low, high, shift, mode, min_spacing ) )
      return false;
  }

  return true;
}

// ScaleKeys():
//  Scales the keys within the range inclusive by the factor
//   provided around the center provided.
bool LWMotion::ScaleKeys( double low, double high, double scale, double center,
                          LWChanKeyPost mode, double min_spacing ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->ScaleKeys( low, high, scale, center, mode, min_spacing ) )
      return false;
  }

  return true;
}

// ShiftValues():
//  Shifts the keys within the range inclusive by the time
//   delta provided.
bool LWMotion::ShiftValues( double low, double high, double offset ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->ShiftValues( low, high, offset ) )
      return false;
  }

  return true;
}

// ReverseKeys():
//  Reverses the keys within the range inclusive
bool LWMotion::ReverseKeys( double low, double high ) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->ReverseKeys( low, high ) )
      return false;
  }

  return true;
}

// ScaleValues():
//  Shifts the keys within the range inclusive by the time
//   delta provided.
bool LWMotion::ScaleValues( double low, double high, double factor, double center) {
  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    if( !channel[i]->ScaleValues( low, high, factor, center ) )
      return false;
  }

  return true;
}

// FindEarliestKey():
//  Returns the time of the earliest keyframe in the motion
//   (taking into account all channels).  Returns a time of
//   0.0 if there are no keys in the motion.
double LWMotion::FindEarliestKey() {
  if( channel.NumElements() == 0 )
    return 0.0;

  double earliest = channel[0]->GetStartTime();
  for( unsigned int i=1; i < channel.NumElements(); i++ ) {
    if( channel[i]->GetStartTime() < earliest )
      earliest = channel[i]->GetStartTime();
  }

  return earliest;
}

// FindLatestKey():
//  Returns the time of the latest keyframe in the motion
//   (taking into account all channels).  Returns a time of
//   0.0 if there are no keys in the motion.
double LWMotion::FindLatestKey() {
  if( channel.NumElements() == 0 )
    return 0.0;

  double latest = channel[0]->GetEndTime();
  for( unsigned int i=1; i < channel.NumElements(); i++ ) {
    if( channel[i]->GetEndTime() < latest )
      latest = channel[i]->GetEndTime();
  }

  return latest;
}

// FindMinKeys():
//  Returns the fewest number of keys in any one channel.
//   Returns 0 if there are no channels or keys
unsigned int LWMotion::FindMinKeys() {
  if( channel.NumElements() == 0 )
    return 0;

  unsigned int min = channel[0]->GetKeys().NumElements();
  for( unsigned int i=1; i < channel.NumElements(); i++ ) {
    if( channel[i]->GetKeys().NumElements() < min )
      min = channel[i]->GetKeys().NumElements();
  }

  return min;
}

// FindMaxKeys():
//  Returns the most number of keys in any one channel
//   Returns 0 if there are no channels or keys.
unsigned int LWMotion::FindMaxKeys() {
  if( channel.NumElements() == 0 )
    return 0;

  unsigned int max = channel[0]->GetKeys().NumElements();
  for( unsigned int i=1; i < channel.NumElements(); i++ ) {
    if( channel[i]->GetKeys().NumElements() > max )
      max = channel[i]->GetKeys().NumElements();
  }

  return max;
}

// FindNextKey():
//  Finds the next keyframe at or after the start_time provided in
//   any channel of the motion.  Returns NULL if ther is no next
//   frame. The optional channel argument will be filled with the
//   index of the channel that this key belongs to.
LWKeyFrame * LWMotion::FindNextKey( double start_time, int *chan ) {
  if( chan != NULL )
    *chan = -1;

  if( channel.NumElements() == 0 )
    return NULL;

  unsigned int  index;
  LWKeyFrame  * key = NULL;

  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    index = channel[i]->FindKeyFrameIndex( start_time, LWCHANFIND_ROUND_UP );
    if( index != LWCHANFIND_NO_MATCH ) {
      if( key == NULL ) {
        key = channel[i]->GetKeys()[index];
        if( chan != NULL )
          *chan = i;
      } else if( key->GetTime() > channel[i]->GetKeys()[index]->GetTime() ) {
        key = channel[i]->GetKeys()[index];
        if( chan != NULL )
          *chan = i;
      }
    }
  }

  return key;
}

// FindPrevKey():
//  Finds the next keyframe at or after the start_time provided in
//   any channel of the motion.  Returns NULL if there is no next
//   frame. The optional channel argument will be filled with the
//   index of the channel that this key belongs to.
LWKeyFrame * LWMotion::FindPrevKey( double start_time, int *chan ) {
  if( chan != NULL )
    *chan = -1;

  if( channel.NumElements() == 0 )
    return NULL;

  unsigned int  index;
  LWKeyFrame  * key      = NULL;

  for( unsigned int i=0; i < channel.NumElements(); i++ ) {
    index = channel[i]->FindKeyFrameIndex( start_time, LWCHANFIND_ROUND_DOWN );
    if( index != LWCHANFIND_NO_MATCH ) {
      if( key == NULL ) {
        key = channel[i]->GetKeys()[index];
        if( chan != NULL )
          *chan = i;
      } else if( key->GetTime() < channel[i]->GetKeys()[index]->GetTime() ) {
        key = channel[i]->GetKeys()[index];
        if( chan != NULL )
          *chan = i;
      }
    }
  }

  return key;
}


