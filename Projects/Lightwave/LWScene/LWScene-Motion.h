//
// LWScene-Motion.h
//
// This contains what used to be FreeKeyMotion.  It has the ability
//  to load, save and manipulate Lightwave motions.  Portions of
//  FreeKeyMotion now also exist in LWKeyFrame and LWChannel.
//
// A LWMotion is simply a collection of LWChannels.  Although
//  this grouping commonly represents the 9 motion channels of
//  an item, it does not need to be used for that, nor does it
//  need to have 9 channels.
//
// LWMotions support independantly keyed channels, as is
//  consitant with Version 2 and 3 (5.7 and 6) scene files.
//  However, LWSMotion does not include anything more elaborate
//  than TCB-type freezing, and as such cannot reproduce the
//  alternate spline types present in L6.  If independantly
//  keyed channels are used and a Version 1 (<=5.6) scene is
//  is to be saved, the save function will return an error.
//
// To help with this problem, there is a Link Channels function.
//  when enabled, any operation performed on a motion will be
//  performed on all it's channels at the same time.
//

#ifndef HEADER_LWSCENE_MOTION
#define HEADER_LWSCENE_MOTION

#include "LWScene-Channel.h"

// Common Channel IDs
enum LWChan_IDs {
  LWCHAN_ALL_CHANNELS = -1,  // Forces all channels to be processed; do NOT use to access the channel arrat
  LWCHAN_ENV = 0,            // Use for envelopes
  LWCHAN_X   = 0,            // Use for position values
  LWCHAN_Y,
  LWCHAN_Z,
  LWCHAN_H,                  // Use for rotational values
  LWCHAN_P,
  LWCHAN_B,
  LWCHAN_SX,                 // Use for scale values
  LWCHAN_SY,
  LWCHAN_SZ,
  NUM_LWCHAN_IDS };

class LWMotion;

class LWMotion {
public:
  LWMotion( int num_channels = 0, bool link = false );
  ~LWMotion();

  // File I/O Functions
  int LoadMotionFile( const char *file );                                        // Loads a .MOT file
  int LoadMotion( pifstream &in, int mode, int version,
                  const char *file = NULL, double fps = 30.0 );                  // Loads a motion from a scene file

  int SaveMotionFile( const char *file );                                        // Saves a .MOT file
  int SaveMotion( pofstream &out, int version,
                  double fps = 30.0 );                                           // Saves a motion to a scene file

  // Channel-Level Functions
  unsigned int  AddChannel( LWChannel *new_chan = NULL );                        // Adds a new channel
  bool          RemoveChannel( unsigned int index );                             // Removes an existing channel from the motion.  This does NOT free the memory for the channel
  bool          DeleteChannel( unsigned int index );                             // Deletes an existing channel from the motion

  bool GetKeysAt( double time, DynArray< LWKeyFrame * > *key_list );             // Gets all the keys at the given time and stores them in the DynArray provided

  void CreateKeyAllChannels( double time, DynArray< LWKeyFrame *> *key_list );   // Creates a keyframe on all the channels at the given time
  void DeleteKeyAllChannels( double time, DynArray< LWKeyFrame *> *key_list );   // Deletes a keyframe on all the channels at the given time

  inline LWChannel & GetChannel( int chan ) { return *(channel[ chan ]); }

  inline DynArray< LWChannel * > &  GetChannelList() { return channel; }

  double FindEarliestKey();                                                      // Returns the time of the earliest keyframe in the motion (taking into account all channels)
  double FindLatestKey();                                                        // Returns the time of the latest keyframe in the motion (taking into account all channels)

  unsigned int FindMinKeys();                                                    // Returns the fewest number in any one channels
  unsigned int FindMaxKeys();                                                    // Returns the most number of keys in any one channel

  LWKeyFrame * FindNextKey( double start_time, int *chan = NULL );               // Returns a pointer to the next keyframe in any channel of the motion
  LWKeyFrame * FindPrevKey( double start_time, int *chan = NULL );               // Returns a pointer to the previous keyframe in any channel of the motion

  // Channel Linkage Functions
  bool GetLinkChannels() { return link_channels; }
  void SetLinkChannels( bool state ) { link_channels = state; }
  bool TestLinkedChannels();

  // Keyframe Management
  //  These functions act on all the channels in the motion.  For independant channel
  //   modifications, use the associated function on the channel[x] instance.
  bool Freeze( int channel, double low, double high, LWMotion *new_motion );                       // All-Keys a channel into the new_motion provided

  bool DeleteRange( double low, double high );                                                     // Deletes the keys between low and high inclusive.
  bool DeleteStep(  double low, double high, double step, double step_size, bool invert = false ); // Delete the every Xth frame (or keep every Xth frame if invert == true );

  bool ShiftKeys(   double low, double high, double shift, LWChanKeyPost mode, double min_spacing = 1.0/30.0 );                 // Shifts a motion forward or backward in time.
  bool ScaleKeys(   double low, double high, double scale, double center, LWChanKeyPost mode, double min_spacing = 1.0/30.0 );  // Scales a motion in time.
  bool ShiftValues( double low, double high, double offset );                                      // Shifts a motion's keyframe values (pos/rot/scale)
  bool ScaleValues( double low, double high, double factor, double center );                       // Scales a motion's keyframe values (pos/rot/scale)

  bool SetValues(   double low, double high, float offset );                                       // Sets a motion's keyframe values (pos/rot/scale)

  bool ReverseKeys( double low, double high );
  
  // Public Data Members
  DynArray< LWChannel * > channel;                  // Array of channels in the motion

protected:
  int  ReadVersion1Motion( pifstream &in, double fps );                       // Reads a Version 1 motion (LW 5.6 or earlier)
  int  ReadVersion2Motion( pifstream &in );                                   // Reads a Version 2 motion (LW 5.7)
  int  ReadVersion3Motion( pifstream &in );                                   // Reads a Version 3 motion (L[6])

  int  WriteVersion1Motion( pofstream &out, double fps );                     // Writes a Version 1 motion (LW 5.6 or earlier)
  bool WriteVersion1Keyframe( pofstream &out, unsigned int index, double fps, bool frame_zero = false ); // Outputs a Version 1 keyframe (LW 5.6 or earlier)
  int  WriteVersion2Motion( pofstream &out );                                 // Writes a Version 2 motion (LW 5.7)
  int  WriteVersion3Motion( pofstream &out );                                 // Writes a Version 3 motion (L[6])

  bool link_channels;                               // true if all channels should have keyframes at the same frame
};

#endif