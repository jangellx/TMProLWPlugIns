//
// LWScene-Channels.h
//
// It consists of a collection of LWKeyFrames stored in a DynArray.
//
// A collection of LWChannel 9 classes makes a LWMotion, and one LWChannel
//  makes an envelope.  While a LWChannel by itself isn't technically a
//  motion, there are both motion and envelope static functions available.
//
//
// As with LWScene, all time values are stored in doubles representing
//  seconds.  For this reason, all channel access functions take an
//  FPS function.  The default FPS is 30.  To convert from frames, use the
//  FrameToTime() static function in LWScene with something like this:
//
//     LWScene::FrameToTime( frame, scene.GetFPS() );
//
// It is strongly recommended that you use the CreateKey() and DeleteKey()
//  members to create a keyframe, as directly editing the key DynArray
//  could result in duplicate keyframes or out-of-order keys.
//
// It is also recommended that you use SetKeyTime() to change what time
//  a keyframe is at to ensure that the keyframe array remains sorted
//  by time.
//
//
// 4/21/00:  Keyframe Selection Functions
// 
// The LWChannel class includes functions and a DynArray for storing a
//  selection of keyframes.  The selections are stored as pointers to the
//  actual keyframes in the order they were selected.  Since the selection
//  tools are integrated into the channel class, normal keyframe
//  management (deleting keys, for example), is automatically reflected
//  in the keyframe list.
//
// Use GetSelection() to get the keyframes DynArray, and add or remove
//  keys directly.  Note that you should only create and delete keyframes
//  via the standard LWChannel keyframe functions.
// 

#ifndef HEADER_LWSCENE_CHANNEL
#define HEADER_LWSCENE_CHANNEL

#include "LWScene-KeyFrame.h"

enum LWChanBehavior {
  LWCHANB_RESET      = 0,
  LWCHANB_STOP       = 1,   // 5.6 Name
  LWCHANB_CONSTANT   = 1,   // L6 Name
  LWCHANB_REPEAT,
  LWCHANB_OSCILATE,
  LWCHANB_OFFSET_REPEAT,
  LWCHANB_LINEAR,
  NUM_LWCHANB };

enum LWChanFind {
  LWCHANFIND_NO_MATCH     = -1,  // Returned if no match is found
  LWCHANFIND_NEAREST      =  0,
  LWCHANFIND_ROUND_DOWN,         // ie: Previous Key
  LWCHANFIND_ROUND_UP,           // ie: Next Key
  LWCHANFIND_EXACT,
  NUM_LWCHANFIND };

enum LWChanKeyPost {
  LWCHANKEYPOST_DO_NOTHING,
  LWCHANKEYPOST_DROP_FRAMES,
  LWCHANKEYPOST_SCRUNCH_FRAMES };

class LWChannel;

class LWChannel {
public:
  friend class LWMotion;

  LWChannel();
  ~LWChannel();

  // File I/O Functions
  void LoadEnvelopeFile( const char *file );                                          // Loads a .ENV file
  void LoadChannel( pifstream &in, int mode, int version, const char *file = NULL );  // Loads a channel from a scene file

  // Keyframe Functions
  long CreateKey( LWKeyFrame *new_key, bool sort = true );
  long DeleteKeyIndex( unsigned long index );
  long DeleteKeyTime( double time );
  long SetKeyTime( unsigned long index, double time );

  // Keyframe Management
  bool Freeze( double low, double high, LWChannel *new_motion );                 // All-Keys a motion into the new_motion provided

  bool DeleteRange( double low, double high );                                   // Deletes the keys between low and high inclusive.
  bool DeleteStep(  double low, double high, double step, double step_size, bool invert = false ); // Delete the every Xth frame (or keep every Xth frame if invert == true );

  bool ShiftKeys(   double low, double high, double shift, LWChanKeyPost mode, double min_spacing = 1.0/30.0 );                 // Shifts a channel forward or backward in time.
  bool ScaleKeys(   double low, double high, double scale, double center, LWChanKeyPost mode, double min_spacing = 1.0/30.0  ); // Scales a channel in time.

  bool ShiftValues( double low, double high, double offset );                    // Shifts a channel's keyframe values (pos/rot/scale)
  bool ScaleValues( double low, double high, double factor, double center );     // Scales a channel's keyframe values (pos/rot/scale)

  bool SetValues( double low, double high, float offset );                       // Sets a channel's keyframe values (pos/rot/scale)

  bool KeyPost( unsigned long first_key, unsigned long last_key, LWChanKeyPost mode, double min_spacing = 1.0/30.0 );

  int  FindKeyFrameIndex( double time, LWChanFind match = LWCHANFIND_EXACT );    // Find the closest keyframe based on the match mode.
  int  FindKeyFrameIndex_Linear( double time, LWChanFind match = LWCHANFIND_EXACT );    // Find the closest keyframe based on the match mode.
  int  FindDuplicateKeys();                                                      // Returns the first index of a duplicate keyframe (ie: the first key that is a copy of an already existing key, which means if two keys have the same value, the second one will be returned).

  bool ReverseKeys(   double low, double high );                                 // Reverses the keyframes within the range specified
  bool ReverseValues( double low, double high );                                 // Reverses the values within the range specifed (not sure how this should work yet)

  inline void SortKeys( int (__cdecl sort_func) ( const void *arg1, const void *arg2 ) );  // Sorts the keyframe array in the mode specified

  // Accessors
  inline DynArray< LWKeyFrame * > & GetKeys() { return key; }

  inline LWChanBehavior GetPreBehavior() { return pre_behavior; }
  void                  SetPreBehavior( LWChanBehavior new_b ) { pre_behavior = new_b; }

  inline LWChanBehavior GetPostBehavior() { return post_behavior; }
  void                  SetPostBehavior( LWChanBehavior new_b ) { post_behavior = new_b; }

  inline double         GetStartTime() { return ((key.NumElements() > 0) ? key[0]->GetTime() : 0.0); }
  inline double         GetEndTime()   { return ((key.NumElements() > 0) ? key[ key.NumElements() - 1 ]->GetTime() : 0.0); }

  inline unsigned int   GetNumKeys() { return key.NumElements(); }

  // Operator Overloads
  inline LWKeyFrame   & operator[]( unsigned long index ) { return *(key[ index ]); }  // Returns the key at the given index in the keyframe array.  This is NOT a frame number -- it's an index

  // Keyframe Selection
  inline DynArray< LWKeyFrame * > &GetSelected() { return selected; }

protected:
  int ReadVersion2Channel( pifstream &in );
  int ReadVersion3Channel( pifstream &in );

  int WriteVersion2Channel( pofstream &out );
  int WriteVersion3Channel( pofstream &out );

  DynArray< LWKeyFrame * > key;                     // Keyframe Array
  DynArray< LWKeyFrame * > selected;                // Keyframe Selection Array

  LWChanBehavior pre_behavior;                      // What to do before first key is reached.  Version 3 (L6) only.
  LWChanBehavior post_behavior;                     // What to do after last key is passed
};

// Sort Functions
int (__cdecl SortLWKeyFrameByTime) ( const void *arg1, const void *arg2 );
int (__cdecl SortLWKeyFrameByValue)( const void *arg1, const void *arg2 );


// SortKeys():
//  Sorts the keyframes in the channel using the sort function provided.
inline void LWChannel::SortKeys( int (__cdecl sort_func) ( const void *arg1, const void *arg2 ) ) {
  key.QSort( sort_func );
}


#endif