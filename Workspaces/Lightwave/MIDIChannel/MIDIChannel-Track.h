//
// MIDIChannel-Track.h
//

#ifndef MIDICHANNEL_TRACK_HEADER
#define MIDICHANNEL_TRACK_HEADER

class MIDIChannel_Channel;
class MIDIChannel_Event;

// class MIDIChannel_Track
class MIDIChannel_Track {
public:
  MIDIChannel_Track() : name(NULL) { ; }
  ~MIDIChannel_Track() { channels.Flush(); StringTools::strdup2( name, NULL ); }

  // Accessors
  inline const char * GetName() { return name; }
  inline void         SetName( const char * _name ) { StringTools::strdup2( name, _name ); }

  inline DynArray< MIDIChannel_Channel * > & GetChannels() { return channels; }

protected:
  char * name;

  DynArray< MIDIChannel_Channel * > channels;
};

// class MIDIChannel_Channel
class MIDIChannel_Channel {
public:
  MIDIChannel_Channel( unsigned char _index ) : index( _index ) { ; }
  ~MIDIChannel_Channel() { events.Flush(); }

  // Accessors
  inline unsigned char GetIndex() { return index; }

  inline DynArray< MIDIChannel_Event * > & GetEvents() { return events; }

protected:
  unsigned char index;

  DynArray< MIDIChannel_Event * > events;
};

enum MIDIEventID {
  MIDIEVENT_NOTE_ON = 0,
  MIDIEVENT_NOTE_OFF };

// class MIDIChannel_Event
class MIDIChannel_Event {
public:
  MIDIChannel_Event( MIDIEventID _event, double _when ) : event(_event), when(_when), velocity(64), next_event(NULL) { ; }
  ~MIDIChannel_Event() { if( next_event != NULL ) { delete next_event; } }

  // Accessors
  inline          MIDIEventID GetEvent() { return event; }
  inline          void        SetEvent( MIDIEventID _event ) { event = _event; }

  inline unsigned char        GetNote() { return note; }
  inline          void        SetNote( unsigned char _note ) { note = _note; }

  inline unsigned char        GetVelocity() { return velocity; }
  inline          void        SetVelocity( unsigned char _vel ) { velocity = _vel; }

  inline          double      GetWhen() { return when; }
  inline          void        SetWhen( double _when ) { when = _when; }

  inline MIDIChannel_Event  * GetNextEvent() { return next_event; }
  inline          void        SetNextEvent( MIDIChannel_Event * _next ) { next_event = _next; }

protected:
  MIDIEventID   event;            // What the event is
  unsigned char note;             // What note to play (or some other data)
  unsigned char velocity;         // Speed that the note's key was pressed/released
  double        when;             // Time in seconds from start of track

  MIDIChannel_Event * next_event; // Next Event at the same time as this event
};

#endif