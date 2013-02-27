//
// MIDIChannel-Instance.h
//

#ifndef MIDICHANNEL_INSTANCE_HEADER
#define MIDICHANNEL_INSTANCE_HEADER

#include <portable/stringtools/stringtools.h>
#include <portable/dynarray/dynarray.h>
#include <portable/bfstream/bfstream.h>

#include "midichannel-track.h"

class MIDIChannel_Instance {
public:
  MIDIChannel_Instance();
  ~MIDIChannel_Instance();

  // Operator Overloads
  MIDIChannel_Instance & operator=( MIDIChannel_Instance &other );

  // Accessors
         void          SetMIDIFile( const char *_file, bool askForFile );
  inline const char *  GetMIDIFile()                    { return midi_file; }

  inline void          SetTrack( unsigned long _track ) { track = (_track >= tracks.NumElements()) ? 0 : _track; }
  inline unsigned long GetTrack() { return track; }

  inline void          SetChannel( unsigned long _channel ) { if( tracks.NumElements() > 0 ) {
                                                                if( channel >= tracks[ track ]->GetChannels().NumElements() )
                                                                  channel = 0; else channel = _channel;
                                                              } else { channel = 0; } }
  inline unsigned long GetChannel() { return channel; }

  inline void          SetParameter( short _param ) { parameter = _param; }
  inline short         GetParameter() { return parameter; }

  inline void          SetStartTime( double _time ) { start_time = _time; }
  inline double        GetStartTime() { return start_time; }

  inline void          SetOffset( double _offset ) { offset = _offset; }
  inline double        GetOffset() { return offset; }

  inline void          SetScale( double _scale ) { scale = _scale; }
  inline double        GetScale() { return scale; }

  inline void          SetLowNote( unsigned char _note ) { low_note = ((_note > 127) ? 127 : _note);
                                                           if( high_note < low_note ) high_note = low_note; }
  inline unsigned char GetLowNote() { return low_note; }

  inline void          SetHighNote( unsigned char _note ) { high_note = ((_note > 127) ? 127 : _note);
                                                            if( low_note > high_note ) low_note = high_note; }
  inline unsigned char GetHighNote() { return high_note; }

  inline void          SetNoteShift( char _note ) { note_shift = _note; }
  inline char          GetNoteShift() { return note_shift; }

  inline void          SetDecay( double _decay ) { decay = _decay; }
  inline double        GetDecay() { return decay; }

  inline void          SetNoteOffLevel( unsigned char _note_off_level ) { note_off_level = ((_note_off_level > 127) ? 127 : _note_off_level); }
  inline unsigned char GetNoteOffLevel() { return note_off_level; }

  inline void          SetEase( double _ease ) { ease = _ease; }
  inline double        GetEase() { return ease; }

  inline void          SetAdditive( bool _additive ) { additive = _additive; }
  inline bool          GetAdditive() { return additive; }

  // Non-Persitant Accessors
  inline DynArray< MIDIChannel_Track * > & GetTracks() { return tracks; }

  // Static Members
  static GlobalFunc *global;

protected:
  unsigned long ReadVarLen( bifstream &in );
  unsigned char getc( bifstream &in );

  char         * midi_file;

  unsigned long  track;
  unsigned long  channel;
           short parameter;     // Note (0) or Velocity (1)

  double         start_time;
  double         offset;
  double         scale;

  unsigned char  low_note;
  unsigned char  high_note;
           char  note_shift;

  double         decay;
  unsigned char  note_off_level;
  double         ease;

  bool           additive;

  // Non-Persistant Data
  DynArray< MIDIChannel_Track * > tracks;
};

#endif
