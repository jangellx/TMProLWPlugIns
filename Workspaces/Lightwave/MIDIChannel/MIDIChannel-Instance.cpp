//
// MIDIChannel_Instance.cpp
//

#include <portable/dirtools/dirtools.h>

#include "MidiChannel.h"
#include "MidiChannel-Instance.h"

// Static Initializers
GlobalFunc * MIDIChannel_Instance::global = NULL;

// Constructor
MIDIChannel_Instance::MIDIChannel_Instance()
  : midi_file(NULL), track(0), channel(0), parameter(0),
    start_time(0.0), scale(1.0), offset(0.0),
    low_note(0), high_note(127), note_shift(0),
    note_off_level(0), decay(0.5), ease(0.1),
    additive(false) {
  ;
}

// Destructor
MIDIChannel_Instance::~MIDIChannel_Instance() {
  StringTools::strdup2( midi_file,  NULL );

  tracks.Flush();
}

// operator=():
MIDIChannel_Instance & MIDIChannel_Instance::operator=( MIDIChannel_Instance &other ) {
  SetMIDIFile(     other.GetMIDIFile(), false );

  SetTrack(        other.GetTrack()        );
  SetChannel(      other.GetChannel()      );
  SetParameter(    other.GetParameter()    );

  SetStartTime(    other.GetStartTime()    );
  SetOffset(       other.GetOffset()       );
  SetScale(        other.GetScale()        );

  SetLowNote(      other.GetLowNote()      );
  SetHighNote(     other.GetHighNote()     );
  SetNoteShift(    other.GetNoteShift()    );

  SetDecay(        other.GetDecay()        );
  SetNoteOffLevel( other.GetNoteOffLevel() );
  SetEase(         other.GetEase()         );

  SetAdditive(     other.GetAdditive()     );

  return *this;
}

// SetMIDIFile():
//  Sets the MIDI filename and loads MIDI data from the current
//   track and channel.
void MIDIChannel_Instance::SetMIDIFile( const char *_file, bool askForFile ) {
  if( _file == NULL ) 
    return;

  LWMessageFuncs *message = (LWMessageFuncs *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  unsigned long   sysid   = (unsigned long)   global( LWSYSTEMID_GLOBAL,     GFUSE_TRANSIENT );
  LWFileReqFunc *filereq  = (LWFileReqFunc  *)global( LWFILEREQFUNC_GLOBAL,  GFUSE_TRANSIENT );

  // See if the file exists
  char fbuffer[ 256 ];
  if( askForFile && (sysid != LWSYS_SCREAMERNET) ) {
    while( true ) {
      if( !DirInfo::Exists( _file ) ) {
        if( (*message->yesNo)( "MIDI File Error", "MIDI Channel Error:  This MIDI File doesn't exist, load another?", _file ) == 1 ) {
          if( !filereq( "Select MIDI File", "*.mid", "", fbuffer, 256 ) )
            return;

          _file = fbuffer;
          continue;
        }
      }
      break;
    }
  } else {
    if( !DirInfo::Exists( _file ) ) {
      (*message->error)( "MIDI Channel Error:  The MIDI File doesn't exist:", _file );
      return;
    }
  }

  // See if the file exists
  bifstream in( _file );
  if( !in ) {
    (*message->error)( "MIDI Channel Error:  File I/O error occurred opening MIDI file", _file );
    return;
  }

  in.SetEndian( ENDIAN_MOTOROLA );

  const unsigned long MIDI_MThd = ('M'<<24) | ('T'<<16) | ('h'<<8) | 'd';
  const unsigned long MIDI_MTrk = ('M'<<24) | ('T'<<16) | ('r'<<8) | 'k';

  // Test the header
  unsigned long chunk_id, chunk_size;
  short temps;
  short ppqn;
  unsigned long temp_track = track;
  unsigned long time_delta = 0;

  in >> chunk_id;
  if( chunk_id != MIDI_MThd ) {
    (*message->error)( "MIDI Channel Error:  File is not a recognized MIDI file", _file );
    return;
  }

  // Check the chunk size
  in >> chunk_size;
  if( chunk_size != 6 ) {
    (*message->error)( "MIDI Channel Error:  File is not a recognized MIDI file (MThd too long)", _file );
    return;
  }

  in >> temps;       // Skip the Format Type
  in >> temps;       // Read the Track Count

  if( (int)temp_track > temps )
    temp_track = 0;
  
  bool need_tempo = true;
  in >> ppqn;        // Read in the Pulses Per Quarter Note
  if( ppqn < 0 ) {   // - It's really SMPTE, so read that in instead
    char rate, div;
    in.seekg( -2, ios::cur );
    in >> rate >> div;
    time_delta = div * rate;
    need_tempo = false;
  }

  DynArray< MIDIChannel_Track * > temp_tracks;
  unsigned long      when, i, len;
  unsigned char      status, byte;
  char               buffer[ 8192 ];
  bool               end_of_track;
  streampos          chunk_start;
  unsigned long      tempo = 500000;

  // Scan through the rest of the chunks
  while( !in.eof() ) {
    if( in.bad() || in.fail() ) {
      (*message->error)( "MIDI Channel Error:  File I/O error reading MIDI file", _file );
      temp_tracks.Flush();
      return;
    }

    chunk_start = in.tellg();
    in >> chunk_id >> chunk_size;

    switch( chunk_id ) {
      case MIDI_MTrk: {                           // New Track
        temp_tracks.Add( new MIDIChannel_Track );
        end_of_track = false;

        // Start reading in events
        while( !end_of_track ) {
          when = ReadVarLen( in );
          in >> byte;

          if( in.bad() || in.fail() ) {
            (*message->error)( "MIDI Channel Error:  File I/O error reading MIDI file", _file );
            temp_tracks.Flush();
            return;
          }

          if( byte & 0x80 ) {
            status = byte;
            in >> byte;
          }

          assert( !in.eof() );

          // Scan the status bytes we care about
          switch( 0xF0 & status ) {               // Figure out what kind of status message this is
            case 0x80:                            // - Note Off
            case 0x90: {                          // - Note On
              MIDIChannel_Track   * current_track = temp_tracks[ temp_tracks.NumElements() - 1 ];
              MIDIChannel_Channel * current_channel = NULL;
              for( i=0; i < current_track->GetChannels().NumElements(); i++ ) {
                if( current_track->GetChannels()[i]->GetIndex() == (0x0F & status) )
                  break;
              }

              if( i == current_track->GetChannels().NumElements() )
                current_track->GetChannels().Add( new MIDIChannel_Channel( 0x0F & status ) );

              // Create the new event
              MIDIChannel_Event *event = new MIDIChannel_Event( MIDIEVENT_NOTE_ON, when );
              event->SetNote( byte );

              // Get the velocity
              in >> byte;
              event->SetVelocity( byte );

              // See if the previous event is at the same time
              bool add_event = true;
              unsigned long event_count = current_track->GetChannels()[i]->GetEvents().NumElements();
              if( event_count > 0 ) {
                MIDIChannel_Event *prev_event = current_track->GetChannels()[i]->GetEvents()[ event_count - 1 ];
                if( event->GetWhen() == 0.0 ) {
                  add_event = false;
                  prev_event->SetNextEvent( event );
                }
              }

              // Handle Note Off Events
              if( add_event ) {
                if( (byte == 0) || ((0xF0 &status) == 0x80) ) {               // Handle Note Off Events
                  event->SetEvent( MIDIEVENT_NOTE_OFF );
                  event->SetVelocity( 64 );
                }
              }

              // If add_event is false, then it's already been added to the linked list of the previous event
              if( add_event )
                current_track->GetChannels()[i]->GetEvents().Add( event );

            } break;

            case 0xA0:                            // - After-touch
              in >> byte;                         // -- Velocity
              break;

            case 0xB0:                            // - Control Change
              in >> byte;                         // -- New Value
              break;

            case 0xC0:                            // - Program Patch Change
              break;

            case 0xD0:                            // - Pressure
              break;

            case 0xE0:                            // - Pitch Wheel Change
              in >> byte;                         // -- Top 7 Bits
              break;

            case 0xF0:
              switch( 0x0F & status ) {
                case 0x0F:
                  len = ReadVarLen( in );         // Length
                  switch( byte ) {
                    case 0x03:                    // - Track Name
                      in.read( buffer, len );
                      buffer[ len ] = '\0';
                      temp_tracks[ temp_tracks.NumElements() - 1 ]->SetName( buffer );
                      break;

                    case 0x2F:                    // - End Of Track
                      end_of_track = true;
                      break;

                    case 0x51:                    // - Set Tempo
                      tempo = 0;
                      in.read( ((char *)&tempo) + 1, 3 );
                      bifstream::ReverseEndian( tempo );

                      if( need_tempo )
                        time_delta = (unsigned long)(((double)tempo)/((double)ppqn));
                      break;

                    default:
                      in.seekg( len, ios::cur );
                  }
                  break;
              }
              break;
          }
        }

        break;
      }

      default:                                    // Unknown Chunk
        in.seekg( chunk_size, ios::cur );
    }
  }

  // Set the new MIDI file
  StringTools::strdup2( midi_file, _file );
  time_delta;

  tracks.Flush();
  tracks = temp_tracks;

  // Make sure the Track and Channel pop-ups are in range
  if( track >= tracks.NumElements() )
    track = 0;

  if( channel >= tracks[ track ]->GetChannels().NumElements() )
    channel = 0;

  // Convert times from deltas into seconds
  unsigned long j, k;
  double new_time;
  for( i=0; i < tracks.NumElements(); i++ ) {
    MIDIChannel_Track *current_track = tracks[i];

    for( j=0; j < current_track->GetChannels().NumElements(); j++ ) {
      MIDIChannel_Channel *current_channel = current_track->GetChannels()[ j ];

      for( k=0; k < current_channel->GetEvents().NumElements(); k++ ) {          // Start at 1; we don't adjust 0's time
        MIDIChannel_Event *current_event = current_channel->GetEvents()[ k ];
        new_time = (current_event->GetWhen() * (double)time_delta)/1000000.0;
        if( k == 0 ) {
          current_event->SetWhen( new_time );
        } else {
          MIDIChannel_Event *prev_event = current_channel->GetEvents()[ k-1 ];
          current_event->SetWhen( prev_event->GetWhen() + new_time );
        }

        // Set the child events in the linked list to the same time.
        for( MIDIChannel_Event *next_event = current_event; next_event != NULL; next_event = next_event->GetNextEvent() )
          next_event->SetWhen( current_event->GetWhen() );
      }
    }
  }

  #ifdef _DEBUG
    ofstream out( "i:\\MIDIDebug.txt" );
    out << "Tracks:  " << tracks.NumElements() << endl;
    for( i=0; i < tracks.NumElements(); i++ ) {
      MIDIChannel_Track *current_track = tracks[i];
      out << "  Track " << i << ":  \"" << ((current_track->GetName() == NULL) ? "(unnamed)": current_track->GetName()) << "\"" << endl;
      out << "  Channels:  " << current_track->GetChannels().NumElements() << endl;

      for( j=0; j < current_track->GetChannels().NumElements(); j++ ) {
        MIDIChannel_Channel *current_channel = current_track->GetChannels()[ j ];
        out << "    Channel " << j << ", index " << (int)current_channel->GetIndex() << endl;
        out << "    Events: " << current_channel->GetEvents().NumElements() << endl;

        for( k=0; k < current_channel->GetEvents().NumElements(); k++ ) {
          MIDIChannel_Event *current_event = current_channel->GetEvents()[ k ];
          out << "      Event: " << ((current_event->GetEvent() == MIDIEVENT_NOTE_ON) ? "Note On " : "Note Off")
              << ";  Note: " << (int)current_event->GetNote()
              << ";  Vel:  " << (int)current_event->GetVelocity()
              << ";  When: " << current_event->GetWhen() << endl;

          // Set the child events in the linked list to the same time.
          for( MIDIChannel_Event *next_event = current_event->GetNextEvent(); next_event != NULL; next_event = next_event->GetNextEvent() ) {
            out << "        Child: " << ((next_event->GetEvent() == MIDIEVENT_NOTE_ON) ? "Note On " : "Note Off")
                << ";  Note: " << (int)next_event->GetNote()
                << ";  Vel:  " << (int)next_event->GetVelocity()
                << ";  When: " << next_event->GetWhen() << endl;
          }
        }
      }
    }
  #endif
}

// ReadVarLen():
//  Reads a variable length value from a MIDI file
unsigned long MIDIChannel_Instance::ReadVarLen( bifstream &in ) {
  unsigned long value;
  unsigned char c = 0x80;

  if ( (value = getc(in)) & 0x80 ) {
    value &= 0x7F;
    for( int i=0; (c & 0x80); i++ ) {
      assert( i < 4 );
      value = (value << 7) + ((c = getc(in)) & 0x7F);
    }
  }

  return value;
}

unsigned char MIDIChannel_Instance::getc( bifstream &in ) {
  char c;
  in >> c;
  return c;
}

