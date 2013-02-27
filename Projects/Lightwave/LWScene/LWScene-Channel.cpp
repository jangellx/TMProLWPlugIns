//
// LWScene-Channel.cpp
//

#include <portable/stringtools/stringtools.h>

#include "LWScene.h"

// Constructor
LWChannel::LWChannel() : pre_behavior(LWCHANB_STOP), post_behavior(LWCHANB_STOP){
  ;
}


// Destructor
LWChannel::~LWChannel() {
  ;
}

// CreateKey():
//  Creates a new keyframe with the data specified.  If sort is true, then the array
//   will be sorted by time, and if two key are at the same frame then the most
//   recent will overwrite the other; if false, neither of those events will occur,
//   and the key will just be added to the end of the keyframe array, even if a key
//   already exists at that time.  This makes it possible to do large batch
//   operations quickly (ie: loading key from a scene) and then sort the key later.
//  The return value is the index of the newly created keyframe in the key array.
//  Note that the time compare routine is exact; if the key time EXACTLY matches
//   an existing time, the key will be replaced.  This may result in rounding
//   errors, though, and may need to be changed if this occurs.
long LWChannel::CreateKey( LWKeyFrame *new_key, bool sort ) {
  if( sort ) {     // Sorted Mode
    int i = FindKeyFrameIndex( new_key->GetTime(), LWCHANFIND_ROUND_UP );
    if( i == LWCHANFIND_NO_MATCH ) {                   // No keys after this time; add to the end
      key.Add( new_key );
      return key.NumElements() - 1;
    } else {
      if( key[i]->GetTime() == new_key->GetTime() ) {  // Key exists; replace it
        delete key[i];
        key[i] = new_key;
      } else                                           // Key doesn't exist; insert it
        key.InsertAt( i, new_key );

      return i;
    }
  } else {         // Unsorted Mode
    key.Add( new_key );
    return key.NumElements() - 1;
  }
}

// DeleteKeyIndex():
//  Deletes an existing keyframe at the index provided.
//   Returns the number of keys remaining
//  This function automatically handles keyframe selections,
//   removing the key from the selection if nessesary.
long LWChannel::DeleteKeyIndex( unsigned long index ) {
  delete key[index];
  key.Remove(index);
  return key.NumElements();
}

// DeleteKeyTime():
//  Deletes an existing keyframe at the time provided.
//   Returns the number of keys remaining, which may be
//   unchanged if no key is at that time.
long LWChannel::DeleteKeyTime( double time ) {
  for( unsigned long i=0; i < key.NumElements(); i++ )
    if( key[i]->GetTime() == time )
      return DeleteKeyIndex( i );

  // This will occur only if no key matches the time exactly
  return key.NumElements();
}

// SetKeyTime():
//  Changes the time of the keyframe at the index provided.
//   This also updates the Keyframe array to ensure that
//   the keys remains sorted in time.  Returns the new
//   index of the keyframe.
//  Note that if a keyframe already exists at this time, 
//   the old keyframe will be destroyed and it's memory
//   freed, as there can only be one key at any given
//   time.
long LWChannel::SetKeyTime( unsigned long index, double time ) {
  assert( index >= key.NumElements() );

  // See if they key didn't go anywhere
  if( time == key[index]->GetTime() )
    return index;

  double old_time = key[index]->GetTime();
  key[index]->SetTime( time );

  if( (index == 0) && (time <= old_time) )
    return index;

  if( (index == key.NumElements() - 1) && (time >= old_time) )
    return index;

  if( (key[index-1]->GetTime() < time) && (key[index+1]->GetTime() > time) )
    return index;

  // Figure out where the keyframe should be inserted
  int new_index = FindKeyFrameIndex( time, LWCHANFIND_ROUND_UP );

  if( new_index == LWCHANFIND_NO_MATCH ) {           // No match found; must be first frame
    key.MoveTo( 0, index );
    return 0;
  } else if( key[ new_index ]->GetTime() == time ) { // Keys are at the same time; destroy the existing one
    key.Swap( new_index, index );                    //  Swap the two keys, so the changed one is in the right place
    DeleteKeyIndex( index );                         //  Delete the old key
    return new_index;
  } else {                                           // Move the key to the new location
    key.MoveTo( new_index, index );
    return new_index;
  }
}

// FindKeyFrameIndex():
//  Finds the index of the keyframe that most closely matches the time value
//   specified.  How a match is determined is based on match.  By default,
//   the key equal to or less than will be returned.  Other options are
//   round down, round up and nearest key.  If no key matches the criteria,
//   LWCHANFIND_NO_MATCH is returned (ie: -1).
//  This is a pretty simple search routine.  Write a faster one if needed.
int LWChannel::FindKeyFrameIndex( double time, LWChanFind match ) {
  if( key.NumElements() < 1 )
    return LWCHANFIND_NO_MATCH;

  unsigned int  a=0;                           //    Start Key
  unsigned long x=key.NumElements() - 1;       //    End Key
  unsigned long i=x;                           //    Current Compare

  // Pre-Process
  if( match == LWCHANFIND_EXACT ) {              // Exact Match.  May have floating point accuracy problems; be warned
    if( key[ key.NumElements() - 1 ]->GetTime() <= time )
      return LWCHANFIND_NO_MATCH;
    else if( key[0]->GetTime() >= time )
      return LWCHANFIND_NO_MATCH;

  } else if( match == LWCHANFIND_NEAREST ) {     // Nearest Match
    if( key[ key.NumElements() - 1 ]->GetTime() <= time )
      return key.NumElements() - 1;
    else if( key[0]->GetTime() >= time )
      return 0;

  } else if( match == LWCHANFIND_ROUND_DOWN ) {  // Round Down
    if( key[ key.NumElements() - 1]->GetTime() <= time )
      return key.NumElements() - 1;
    else if( key[0]->GetTime() >= time )
      return LWCHANFIND_NO_MATCH;

  } else if( match == LWCHANFIND_ROUND_UP ) {    // Round Up
    if( key[ key.NumElements() - 1]->GetTime() <= time )
      return LWCHANFIND_NO_MATCH;
    else if( key[0]->GetTime() >= time )
      return 0;
  }

  // Search
  do {
    i=a+( (x-a)/2 );
    if( key[i]->GetTime() == time )
      return i;

    if( (i-a == 0) || (x-i == 0) )
      break;

    if( key[i]->GetTime() > time )
      x = i;
    else
      a = i;
  } while( true );

  // Post Process
  if( match == LWCHANFIND_EXACT ) {              // Exact Match.  May have floating point accuracy problems; be warned
    return LWCHANFIND_NO_MATCH;

  } else if( match == LWCHANFIND_NEAREST ) {     // Nearest Match
    return i;

  } else if( match == LWCHANFIND_ROUND_DOWN ) {  // Round Down
    if( key[i]->GetTime() > time )
      return i-1;

  } else if( match == LWCHANFIND_ROUND_UP ) {    // Round Up
    if( key[i]->GetTime() < time )
      return i+1;
  }

  // This should never be called
  return LWCHANFIND_NO_MATCH;
}

// FindKeyFrameIndex_Linear():
//  Finds the index of the keyframe that most closely matches the time value
//   specified.  How a match is determined is based on match.  By default,
//   the key equal to or less than will be returned.  Other options are
//   round down, round up and nearest key.  If no key matches the criteria,
//   LWCHANFIND_NO_MATCH is returned (ie: -1).
//  This is a pretty simple search routine.  Write a faster one if needed.
int LWChannel::FindKeyFrameIndex_Linear( double time, LWChanFind match ) {
  if( key.NumElements() < 1 )
    return LWCHANFIND_NO_MATCH;

  if( match == LWCHANFIND_EXACT ) {              // Exact Match.  May have floating point accuracy problems; be warned
    for( unsigned long i=0; i < key.NumElements(); i++ ) {
      if( key[i]->GetTime() == time )
        return i;
    }
    return LWCHANFIND_NO_MATCH;

  } else if( match == LWCHANFIND_NEAREST ) {     // Nearest Match
    int    best_match = 0;
    double how_close, this_close;
    if( key[0]->GetTime() > time )
      return 0;  // Time can't be any less than this index
    else
      how_close = time - key[0]->GetTime();

    for( unsigned long i=1; i < key.NumElements(); i++ ) {
      if( key[i]->GetTime() > time )
        this_close = key[i]->GetTime() - time;
      else
        this_close = time - key[i]->GetTime();

      if( this_close < how_close ) {
        how_close = this_close;
        best_match = i;
      } else
        return best_match;
    }
    return key.NumElements() - 1;

  } else if( match == LWCHANFIND_ROUND_DOWN ) {  // Round Down
    if( time >= key[ key.NumElements() - 1 ]->GetTime() )
      return key.NumElements() - 1;

    for( unsigned long i=key.NumElements() - 1; i >= 0; i-- ) {
      if( key[i]->GetTime() <= time )
        return i;
    }

    return LWCHANFIND_NO_MATCH;
  } else if( match == LWCHANFIND_ROUND_UP ){     // Round Up
    if( time <= key[0]->GetTime() )
      return 0;

    for( unsigned long i=0; i < key.NumElements(); i++ ) {
      if( key[i]->GetTime() >= time )
        return i;
    }

    // No match; return -1
    return LWCHANFIND_NO_MATCH;
  }

  // This should never be called
  return LWCHANFIND_NO_MATCH;
}

// SortLWKeyFrameByTime():
//  Sorts the channel by time
int (__cdecl SortLWKeyFrameByTime)( const void *arg1, const void *arg2 ) {
  LWKeyFrame * key1 = *((LWKeyFrame **) arg1);
  LWKeyFrame * key2 = *((LWKeyFrame **) arg2);

  if( key1->GetTime() == key2->GetTime() )
    return 0;
  else
    return( key1->GetTime() < key2->GetTime() ? -1 : 1 );
}

// SortLWKeyFrameByValue():
//  Sorts the channel by value
int (__cdecl SortLWKeyFrameByValue)( const void *arg1, const void *arg2 ) {
  LWKeyFrame * key1 = *((LWKeyFrame **) arg1);
  LWKeyFrame * key2 = *((LWKeyFrame **) arg2);

  if( key1->GetValue() == key2->GetValue() )
    return 0;
  else
    return( key1->GetValue() < key2->GetValue() ? -1 : 1 );
}

// DeleteRange():
//  Deletes the keyframes within the range inclusive.  Note that
//   deleting all keyframes in a channel is legal.
bool LWChannel::DeleteRange( double low, double high ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  ) return true;
  if( GetStartTime() > high ) return true;

  // Delete Loop
  for( unsigned long i=0; true; i++ ) {
    if( (key.NumElements() == 0) || (i >= key.NumElements() ) )
      break;

    if( key[i]->GetTime() > high )
      break;

    if( key[i]->GetTime() >= low ) {
      // Key is in range; delete it.
      DeleteKeyIndex( i-- );   // Since this index now points at a new key, process the index again
    }
  }

  return true;
}

// DeleteStep():
//  Deletes a key every Xth frame within low and high inclusive.
//   if invert is true, then all but every Xth frame within the
//   range will be deleted.
//  step_size is the how much time a step covers.  For example, if
//   the step_size is 1.0 and the step is 2.0, then after every 2
//   seconds one second of motion will be removed.
bool LWChannel::DeleteStep( double low, double high, double step, double step_size, bool invert ) {
  // Make sure the step is positive and non-zero
  if( step <= 0.0 )
    return false;

  if( step + step_size <= 0.0 )
    return false;

  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  )  return true;
  if( GetStartTime() > high )  return true;

  // Don't do anything if the step is not smaller than the number of keys
  if( (high - low) <= step )
    return true;

  // Step Loop
  double step_percent = step / (step + step_size);
  int    fraction_int;
  double fraction_float;
  double diff;
  for( unsigned long i=1; i < key.NumElements()-1; i++ ) {
    if( key[i]->GetTime() >= low ) {     // Process only the keys between low
      if( key[i]->GetTime() > high )     //  and high
        break;

      if( (key[i]->GetTime()-low) > 0.0 ) {
        fraction_float = (key[i]->GetTime()-low)/(step + step_size + 0.0000000001);
        fraction_int   = (int)fraction_float;
        diff           = fraction_float - (float)fraction_int;
        if( ( (fraction_float - fraction_int) <= step_percent ) &&
            ( (fraction_float - fraction_int) > 0.0000000001 ) ) {    // Fixes round-off problems
          if( invert )
            DeleteKeyIndex( i-- );
        } else {
          if( !invert )
            DeleteKeyIndex( i-- );
        }
      }
    }
  }
  return true;
}

// ShiftKeys():
//  Shifts the keys withing the range inclusive by the amount
//   specified in time.
//  mode is used to determine what to do with keyframes that get shifted/scaled
//   beyond keys that are outside the range. Options include Scrunch (LW5.6-style)
//   and Drop (throw out the keys).
//  min_spacing is the minimum amount of spacing each key must have between itself
//   and it's neighbor.  When used with a value of 1/30th in Scrunch mode,
//   ShiftKeys() will emulate LW5.6's Scale Keys function. This variable is only used
//   with Scrunch Mode, and is ignored by Drop Mode.  Note that the min_spacing may
//   not exceed the original spacing of the keys; if it does, it will be adjusted to
//   a valid value.
bool LWChannel::ShiftKeys( double low, double high, double shift,
                           LWChanKeyPost mode, double min_spacing ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  )  return true;
  if( GetStartTime() > high )  return true;

  // Shift the keys
  int first_key = -1, last_key = -1;
  for( unsigned long i=0; i < key.NumElements(); i++ ) {
    if( key[i]->GetTime() >= low ) {     // Process only the keys between low and high
      if( first_key == -1 )
        first_key = i;

      if( key[i]->GetTime() > high ) {
        last_key = i;
        break;
      }

      key[i]->SetTime( key[i]->GetTime() + shift );
    }
  }    

  if( last_key == -1 )
    last_key = (int)key.NumElements()-1;

  // Test to see if the Apply mode needs to be used
  if( (first_key == 0)                       && (shift <= 0) ) return true;
  if( (last_key == (int)key.NumElements()-1) && (shift >= 0) ) return true;

  if( first_key != -1 )
    KeyPost( first_key, last_key, mode, min_spacing );

  return true;
}

// ScaleKeys():
//  Scales the keyframes within the range of low to high inclusive
//   by the scale factor provided.  center is the point in time
//   that the keyframes will be scaled around.
//  mode is used to determine what to do with keyframes that get shifted/scaled
//   beyond keys that are outside the range. Options include Scrunch (LW5.6-style)
//   and Drop (throw out the keys).
//  min_spacing is the minimum amount of spacing each key must have between itself
//   and it's neighbor.  When used with a value of 1/30th in Scrunch mode,
//   ShiftKeys() will emulate LW5.6's Scale Keys function. This variable is only used
//   with Scrunch Mode, and is ignored by Drop Mode.  Note that the min_spacing may
//   not exceed the original spacing of the keys; if it does, it will be adjusted to
//   a valid value.
bool LWChannel::ScaleKeys( double low, double high, double scale, double center,
                LWChanKeyPost mode, double min_spacing ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  )  return true;
  if( GetStartTime() > high )  return true;

  // Scale the keys
  unsigned long first_key = -1, last_key = -1;
  for( unsigned long i=0; i < key.NumElements(); i++ ) {
    if( key[i]->GetTime() >= low ) {     // Process only the keys between low and high
      if( first_key == -1 )
        first_key = i;

      if( key[i]->GetTime() > high ) {
        last_key = i;
        break;
      }

      key[i]->SetTime( center + ((key[i]->GetTime() - center) * scale) );
    }
  }

  if( last_key == -1 )
    last_key = key.NumElements()-1;

  if( first_key != -1 )
    KeyPost( first_key, last_key, mode, min_spacing );

  return true;
}

// KeyPost():
//  Adjusts the keyframes within the range of indices provided based on
//   the mode provided.  This function is called by ShiftKeys() and ScaleKeys()
//   to handle keyframes that exist before or after other keyframes beyond
//   their operation's effected range.  The currently supported modes are:
//    DO_NOTHING:
//     Does nothing.  It is strongly suggested you do your own post-proccessing
//     if this option is used.
//    DROP_FRAMES:
//     Frames that pass other keys outside the range are dropped (ie: deleted)
//    SCRUNCH_FRAMES:
//     Any frames that pass other keys outside the range are bunched together.
//     This is similar to how LW5.6 handles scale/shift operations.
//  min_spacing is used y SCRUNCH_FRAMES to determine that min. amount of distance
//   between each keyframe.  This value must be small enough that all the keys can
//   fit in the allocated time span with the spacing provided; if it isn't small
//   enough, it will be adjusted to the maximum spacing possible to still fit all
//   the keys in the range.
bool LWChannel::KeyPost( unsigned long first_key, unsigned long last_key,
                         LWChanKeyPost mode, double min_spacing ) {
  double max_spacing;
  unsigned long i;

  switch( mode ) {
  case LWCHANKEYPOST_DO_NOTHING:
    break;
  case LWCHANKEYPOST_DROP_FRAMES:
    for( i=first_key; i < last_key; i++ ) {
      if( key[i]->GetTime() < key[ first_key-1 ]->GetTime() ) {  // Test for shifted key before first key
        DeleteKeyIndex( i-- );
        last_key--;
      } else {                                                   // Needed since we may be processing this index again, so we want to do BOTH tests, not just the next test
        if( key[i]->GetTime() > key[ last_key+1 ]->GetTime() ) { // Test for shifted key after last key
          DeleteKeyIndex( i-- );
          last_key--;
        }
      }
    }
    break;

  case LWCHANKEYPOST_SCRUNCH_FRAMES:
    // Theory of Operation:
    //  First we figure out how spacing the keys would need to be evenly spaced along the
    //   length of the range.  If this value is less than the user's min_spacing, we use
    //   the new value instead.
    //  Second, we go through each key from start to end, making sure that none are a) before
    //   the previous keyframe, and b) between the previous keyframe and the previous key
    //   plus the new min_spacing.  If it is too close, it gets adjusted to the time of
    //   the previous key plus the min_spacing.
    //  Third, we go through each key again, but from end to start, making sure that no
    //   keys go beyond their following keyframe, or existin within their following key's
    //   time plus the min_spacing; if so, they are adjusted.
    //  Note that if the first_key or last_key indices are at the begining or end of the
    //   array, they are ignored (not need to process them since their prev/next key doesn't
    //   exist to be tested against).

    // Check to see if the key indices are the same (ie: don't do anything)
    if( first_key == last_key )
      return true;

    // Figure out the max. allowed spacing
    bool infinite = false;
    if( last_key+1 >= key.NumElements() )
      infinite = true;
    if( first_key-1 == -1 )
      infinite = true;

    if( !infinite ) {
      max_spacing = key[ last_key+1 ]->GetTime() - key[ first_key-1 ]->GetTime();
      max_spacing = (max_spacing == 0.0) ? 0.0 : ( max_spacing / ((double)(last_key - first_key + 2)) );  // Also handles divide by zero

      if( min_spacing > max_spacing ) 
        min_spacing = max_spacing;
    }

    // Adjust the keys
    for( i=first_key; i <= last_key; i++ ) {
      if( i != 0 ) {                                                       // Handle reads before start of array
        if( key[i]->GetTime() < key[ i-1 ]->GetTime() )                    // Test for key before first key
          key[i]->SetTime( key[ i-1 ]->GetTime() + min_spacing );
        else if( key[i]->GetTime() < key[ i-1 ]->GetTime() + min_spacing )
          key[i]->SetTime( key[ i-1 ]->GetTime() + min_spacing );
      }
    }

    for( i=last_key; i >= first_key; i-- ) {
      if( i != key.NumElements() - 1 ) {                              // Handle reads past end of array
        if( key[i]->GetTime() > key[ i+1 ]->GetTime() )                    // Test for key before first key
          key[i]->SetTime( key[ i+1 ]->GetTime() - min_spacing );
        else if( key[i]->GetTime() > key[ i+1 ]->GetTime() - min_spacing )
          key[i]->SetTime( key[ i+1 ]->GetTime() - min_spacing );
      }
    }
    break;
  }

  return true;
}

// ShiftValues():
//  Shifts the values of the keys within the range inclusive by the
//   amount specified.
bool LWChannel::ShiftValues( double low, double high, double offset ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  )  return true;
  if( GetStartTime() > high )  return true;

  for( unsigned long i=0; i < key.NumElements(); i++ ) {
    if( key[i]->GetTime() >= low ) {     // Process only the keys between low and high
      if( key[i]->GetTime() > high )
        break;

      key[i]->SetValue( key[i]->GetValue() + offset );
    }
  }

  return true;
}

// ScaleValues():
//  Scales the values of the keys within the range inclusive by the
//   specified factor around the center specified.
bool LWChannel::ScaleValues( double low, double high, double factor, double center ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if the range exists in this motion
  if( GetEndTime()   < low  )  return true;
  if( GetStartTime() > high )  return true;

  for( unsigned long i=0; i < key.NumElements(); i++ ) {
    if( key[i]->GetTime() >= low ) {     // Process only the keys between low and high
      if( key[i]->GetTime() > high )
        break;

      key[i]->SetValue( key[i]->GetValue() + ((key[i]->GetValue() - center) * factor) );
    }
  }

  return true;
}

// ReverseKeys():
//  Reverses the keyframes within the range specified inclusive.
bool LWChannel::ReverseKeys( double low, double high ) {
  // Make sure low is more than high
  if( low > high )
    return false;

  // See if there are any keys in the channel
  if( key.NumElements() == 0 )
    return true;

  unsigned long start = FindKeyFrameIndex( low,  LWCHANFIND_ROUND_UP );
  unsigned long end   = FindKeyFrameIndex( high, LWCHANFIND_ROUND_DOWN );

  // Make sure there are keyframes within the range
  if( (start == LWCHANFIND_NO_MATCH) || (end == LWCHANFIND_NO_MATCH) )
    return true;

  // Adjust the keyframe times
  for( unsigned long i = start; i <= end; i++ )
    key[i]->SetTime( high - (key[i]->GetTime() - low) );

  // Fix the ordering of the keyframe array
  for( i = 0; i < (end - start)/2; i++ )
    key.Swap( start + i, end - i );

  return true;
}

// ReadVersion2Channel():
//  Reads in a Version 2 Channel (ie: motion channel, envelope) from
//   the stream provided
int LWChannel::ReadVersion2Channel( pifstream &in ) {
  char line[8192];
  double   value;
  char   * temp;

  // Get the number of keys
  in.getline( line, 8192 );
  unsigned long num_keys = atoi( line );

  // Keyframe Loop
  for( unsigned long i=0; i < num_keys; i++ ) {          // Read in each key...
    in.getline( line, 8192 );
    value = atof( line );
    CreateKey( new LWKeyFrame( value, LWSPLINE_TCB ), false );

    // Load the rest of the data
    temp = StringTools::FindWord( line, 1 );
    key[i]->SetTime( atof( temp ) );

    temp = StringTools::FindWord( temp, 1 );
    key[i]->GetSpline().tcb->SetLinear( atoi(temp) == 1 ? true : false );

    temp = StringTools::FindWord( temp, 1 );
    key[i]->GetSpline().tcb->SetTension( atof( temp ) );

    temp = StringTools::FindWord( temp, 1 );
    key[i]->GetSpline().tcb->SetContinuity( atof( temp ) );

    temp = StringTools::FindWord( temp, 1 );
    key[i]->GetSpline().tcb->SetBias( atof( temp ) );
  }

  // Sort the key array
  SortKeys( SortLWKeyFrameByTime );

  return LWSERROR_OK;
}

// WriteVersion2Channel():
//  Writes out a Version 2 Channel (ie: motion channel, envelope) to
//   the stream provided
int LWChannel::WriteVersion2Channel( pofstream &out ) {
  // Handle no keys in channel
  if( key.NumElements() == 0 ) {
    out << "  1" << out.endl();
    out << "  0 0 2 0 0" << out.endl();
    return LWSERROR_OK;
  }

  // Write out number of keys
  out << "  " << key.NumElements() << out.endl();

  // Write out each key's data
  LWSpline *spline;
  for( unsigned long i=0; i < key.NumElements(); i++ ) {
    out << "  " << key[i]->GetValue()
        << " "  << key[i]->GetTime();                             // Write out the basic keyframe data

    spline = &key[i]->GetSpline();
    if( spline->base->GetType() == LWSPLINE_TCB ) {
      out << " " << (spline->tcb->GetLinear() ? 1 : 2)            // Write out the spline data
          << " " << spline->tcb->GetTension()
          << " " << spline->tcb->GetContinuity()
          << " " << spline->tcb->GetBias() << out.endl();
    } else                                                        // Handle channels with the wrong spline type
      out << " 2 0 0 0" << out.endl();
  }

  return LWSERROR_OK;
}
