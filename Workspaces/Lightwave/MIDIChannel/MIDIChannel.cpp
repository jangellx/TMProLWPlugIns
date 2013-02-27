//
// MIDIChannel.cpp
//

#include "midichannel.h"
#include <stdio.h>

//#include "MIDIChannel-Interface.h"

#include <portable/dynarray/dynarray.h>

#include <lightwave/lwaboutpanel_tmpro/lwaboutpanel_tmpro.h>

// About Text
const char *about_text[] = {
  LWABOUT_YELLOW,
  "MIDI Channel",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "MIDI file reader for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "Inspired by Teddy René Hansen",
  "http://www.trhlogos.dk",
  NULL };

// Instances Array
DynArray< MIDIChannel_Instance * > instances;

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
//  #define DEBUG_PLUGIN_USERNAME " (Debug)"
//  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

const char *midichannel_server_name = "TM-P_MIDIChannel"DEBUG_PLUGIN_PLUGNAME;

extern "C" {
  ServerUserName MIDIChannel_Usernames[]            = { {"TM-P  MIDI Channel"DEBUG_PLUGIN_USERNAME,                            LANGID_USENGLISH | SRVTAG_USERNAME },  { "MIDI Channel"DEBUG_PLUGIN_USERNAME,                  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MIDIChannelInterface_Usernames[]         = { {"TM-P  MIDI Channel"DEBUG_PLUGIN_USERNAME,                            LANGID_USENGLISH | SRVTAG_USERNAME },  { "MIDI Channel"DEBUG_PLUGIN_USERNAME,                  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
//  ServerUserName MIDIChannelInterfaceGeneric_Usernames[]  = { {"TM-P  Frame Jumper Panel"DEBUG_PLUGIN_USERNAME,                      LANGID_USENGLISH | SRVTAG_USERNAME },  { "Frame Jumper Panel"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWCHANNEL_HCLASS,       midichannel_server_name,                                   MIDIChannel_Activate,                   MIDIChannel_Usernames             },
    { LWCHANNEL_ICLASS,       midichannel_server_name,                                   MIDIChannelInterface_Activate,          MIDIChannel_Usernames             },

//    { LWLAYOUTGENERIC_CLASS, "TM-P_MIDIChannelProperties"DEBUG_PLUGIN_PLUGNAME,         MIDIChannelInterfaceGeneric_Activate,   MIDIChannelInterfaceGeneric_Usernames   },

    { (const char *)NULL }
  };

  LWInstance      MIDIChannel_Create(   void *data, void *context, LWError *error );
  void            MIDIChannel_Destroy(  LWInstance _inst );
  LWError         MIDIChannel_Copy(     LWInstance _to, LWInstance _from );
  LWError         MIDIChannel_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError         MIDIChannel_Save(     LWInstance _inst, const LWSaveState *saver );
  const char    * MIDIChannel_Describe( LWInstance _inst );

  void            MIDIChannel_Evaluate( LWInstance _inst, const LWChannelAccess *access );
  unsigned int    MIDIChannel_Flags(    LWInstance _inst );

  LWError         MIDIChannel_OpenUI( void *data );
}

// MIDIChannel_Activate
XCALL_ (int) MIDIChannel_Activate( long version, GlobalFunc *global,
                                   void *_local, void *serverData ) {

  if(version != LWCHANNEL_VERSION)
    return(AFUNC_BADVERSION);

  LWChannelHandler *local = (LWChannelHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = MIDIChannel_Create;
    local->inst->destroy    = MIDIChannel_Destroy;
    local->inst->load       = MIDIChannel_Load;
    local->inst->save       = MIDIChannel_Save;
    local->inst->copy       = MIDIChannel_Copy;
    local->inst->descln     = MIDIChannel_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = NULL;
    local->item->changeID   = NULL;
  }

  local->evaluate = MIDIChannel_Evaluate;
  local->flags    = MIDIChannel_Flags;

  MIDIChannel_Instance::global = global;

  return AFUNC_OK;
}

// MIDIChannel_Create():
//  All instances share one MIDIChannel_Data instance, referenced as a
//   global variable.
LWInstance MIDIChannel_Create( void *data, void *context, LWError *error ) {
  MIDIChannel_Instance *inst = new MIDIChannel_Instance;
  instances.Add( inst );

  return inst;
}

// MIDIChannel_Destroy():
//  All instances share one MIDIChannel_Data instance, referenced as a
//   global variable.  If no instances are still using this data, the
//   marker list is flushed.
void MIDIChannel_Destroy( LWInstance _inst ) {
  unsigned long index = instances.FindIndexOf( (MIDIChannel_Instance *)_inst );
  delete instances[ index ];
  instances.Remove( index );
}

// MIDIChannel_Copy():
//  Since all instances use the same data, this doesn't actually do anything.
LWError MIDIChannel_Copy( LWInstance _to, LWInstance _from ) {
  MIDIChannel_Instance &to   = *((MIDIChannel_Instance *)_to);
  MIDIChannel_Instance &from = *((MIDIChannel_Instance *)_from);

  to = from;
  return NULL;
}

// MIDIChannel_Load():
LWError MIDIChannel_Load( LWInstance _inst, const LWLoadState *loader ) {
  MIDIChannel_Instance *inst   = (MIDIChannel_Instance *)_inst;

  float         tempf;
  short         temps;
  unsigned long templ;
  unsigned char tempc;
  unsigned char version;
  char          buffer[ 512 ];

  // Format Version
  (*loader->readU1)( loader->readData, &version, 1 );
  if( version > 2 )
    return "MIDI Channel Error:  Unsupported MIDI Channel version; aborting load";

  // MIDI File
  (*loader->readStr)( loader->readData, buffer, 512 );
  inst->SetMIDIFile( buffer, true );

  // Track
  (*loader->readU4)( loader->readData, &templ, 1 );
  inst->SetTrack( templ );

  // Channel
  (*loader->readU4)( loader->readData, &templ, 1 );
  inst->SetChannel( templ );

  // Start Time
  (*loader->readFP)( loader->readData, &tempf, 1 );
  inst->SetStartTime( tempf );

  // Offset
  (*loader->readFP)( loader->readData, &tempf, 1 );
  inst->SetOffset( tempf );

  // Scale
  (*loader->readFP)( loader->readData, &tempf, 1 );
  inst->SetScale( tempf );

  // Low Note
  (*loader->readU1)( loader->readData, &tempc, 1 );
  inst->SetLowNote( tempc );

  // High Note
  (*loader->readU1)( loader->readData, &tempc, 1 );
  inst->SetHighNote( tempc );

  // Note Shift
  (*loader->readU1)( loader->readData, &tempc, 1 );
  inst->SetNoteShift( tempc );

  // Decay
  (*loader->readFP)( loader->readData, &tempf, 1 );
  inst->SetDecay( tempf );

  // Note Off Level
  (*loader->readU1)( loader->readData, &tempc, 1 );
  inst->SetNoteOffLevel( tempc );

  // Ease
  (*loader->readFP)( loader->readData, &tempf, 1 );
  inst->SetEase( tempf );

  // Additive
  (*loader->readU1)( loader->readData, &tempc, 1 );
  inst->SetAdditive( (tempc == 0) ? false : true );

  // Parameter
  if( version > 2 ) {
    (*loader->readI2)( loader->readData, &temps, 1 );
    inst->SetParameter( temps );
  }

  return NULL;
}

// MIDIChannel_Save();
LWError MIDIChannel_Save( LWInstance _inst, const LWSaveState *saver ) {
  MIDIChannel_Instance *inst   = (MIDIChannel_Instance *)_inst;

  float         tempf;
  short         temps;
  unsigned long templ;

  // Format Version
  unsigned char tempc = 2;
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // MIDI File
  char buffer[ 512 ];
  strcpy( buffer, ((inst->GetMIDIFile() == NULL) ? "" : inst->GetMIDIFile() ) );

  // - Strip the content dir
  LWDirInfoFunc *dirinfo = (LWDirInfoFunc *)inst->global( LWDIRINFOFUNC_GLOBAL, GFUSE_TRANSIENT );
  const char * content_dir = dirinfo( "Content" );
  char * start = buffer;
  if( content_dir != NULL ) {
    int len = strlen( content_dir );
    if( strnicmp( buffer, content_dir, len ) == 0 ) {
      if( (buffer[ len ] == '\\') ||
          (buffer[ len ] == '/') ||
          (buffer[ len ] == ':') ) {
        start = &(buffer[ len ]);
      } else {
        start = &(buffer[ len - 1 ]);
      }
    }
  }

  (*saver->writeStr)( saver->writeData, start );

  // Track
  templ = inst->GetTrack();
  (*saver->writeU4)( saver->writeData, &templ, 1 );

  // Channel
  templ = inst->GetChannel();
  (*saver->writeU4)( saver->writeData, &templ, 1 );

  // Start Time
  tempf = (float)inst->GetStartTime();
  (*saver->writeFP)( saver->writeData, &tempf, 1 );

  // Offset
  tempf = (float)inst->GetOffset();
  (*saver->writeFP)( saver->writeData, &tempf, 1 );

  // Scale
  tempf = (float)inst->GetScale();
  (*saver->writeFP)( saver->writeData, &tempf, 1 );

  // Low Note
  tempc = inst->GetLowNote();
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // High Note
  tempc = inst->GetHighNote();
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // Note Shift
  tempc = inst->GetNoteShift();
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // Decay
  tempf = (float)inst->GetDecay();
  (*saver->writeFP)( saver->writeData, &tempf, 1 );

  // Note Off Level
  tempc = inst->GetNoteOffLevel();
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // Ease
  tempf = (float)inst->GetEase();
  (*saver->writeFP)( saver->writeData, &tempf, 1 );

  // Additive
  tempc = inst->GetAdditive() ? 1 : 0;
  (*saver->writeU1)( saver->writeData, &tempc, 1 );

  // Parameter
  temps = inst->GetParameter();
  (*saver->writeI2)( saver->writeData, &temps, 1 );

  return NULL;
}

// MIDIChannel_Describe():
const char * MIDIChannel_Describe( LWInstance _inst ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)_inst;
  static char desc[ 512 ];

  sprintf( desc, "MIDI Channel from \"%s\"", ((inst->GetMIDIFile() == NULL) ? "(none)" : inst->GetMIDIFile() ) );
  return desc;
}

// MIDIChannel_Evaluate():
void MIDIChannel_Evaluate( LWInstance _inst, const LWChannelAccess *access ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)_inst;

  double value = (double)inst->GetNoteOffLevel();
  double time = access->time + -inst->GetStartTime();

  // Figure out which event is closest to this time
  if( inst->GetTrack() >= inst->GetTracks().NumElements() )
    return;

  if( inst->GetChannel() >= inst->GetTracks()[ inst->GetTrack() ]->GetChannels().NumElements() )
    return;

  DynArray< MIDIChannel_Event * > &events = inst->GetTracks()[ inst->GetTrack() ]->GetChannels()[ inst->GetChannel() ]->GetEvents();

  unsigned long match = events.BinarySearch( MIDIEventBinarySearchByTime, &time, DA_BSEARCH_ROUND_DOWN );
  if( match != -1 ) {
    // Find the On Note
    MIDIChannel_Event *on_note_event = FindNote( MIDIEVENT_NOTE_ON, inst, match, -1 );

    if( on_note_event != NULL ) {
      // Find the Off Note
      MIDIChannel_Event *off_note_event = FindNote( MIDIEVENT_NOTE_OFF, inst, match, -1 );

      bool on_only = false;
      if( off_note_event == NULL )
        on_only = true;

      if( off_note_event == on_note_event )
        on_only = true;

      if( off_note_event->GetWhen() == on_note_event->GetWhen() )
        on_only = true;

      if( on_only ) {
        // On Note only; set value
        value = (inst->GetParameter() == 0) ? on_note_event->GetNote() : on_note_event->GetVelocity();
      } else {
        if( off_note_event->GetWhen() < on_note_event->GetWhen() ) {
          // On Note after nearest Off Note; set the value
          MIDIChannel_Event *off_note_event = NULL;
          value = (inst->GetParameter() == 0) ? on_note_event->GetNote() : on_note_event->GetVelocity();
        } else {
          // Handle Off Notes (Decay)
          double decay = time - off_note_event->GetWhen();
          if( decay < inst->GetDecay() ) {
            unsigned char param = (inst->GetParameter() == 0) ? on_note_event->GetNote() : on_note_event->GetVelocity();
            double fraction     = (decay/inst->GetDecay());
            double mod          = (param - (double)inst->GetNoteOffLevel()) * fraction;
            value = param - mod;
          }
        }
      }
    }
  }

  // See if we should ease in the next note
  MIDIChannel_Event * next_event = FindNote( MIDIEVENT_NOTE_ON, inst, ((match == -1) ? 0 : match + 1), 1 );
  if( next_event != NULL ) {
    double ease = next_event->GetWhen() - time;
    if( ease < inst->GetEase() ) {
      unsigned char param = (inst->GetParameter() == 0) ? next_event->GetNote() : next_event->GetVelocity();
      double fraction     = (ease/inst->GetEase());
      double mod          = (param - (double)inst->GetNoteOffLevel()) * fraction;
      double final        = param - mod;

      // Keep which ever is greater.
      if( value < final )
        value = final;
    }
  }

  // Shift the notes
  value += inst->GetNoteShift();

  // Modify by scale
  value *= inst->GetScale();
  
  // Add the offset
  value += inst->GetOffset();

  // Handle additive mode
  if( inst->GetAdditive() )
    value += access->value;

  // Set the channel's value
  (*access->setChannel)( access->chan, value );
}

// FindNote():
MIDIChannel_Event * FindNote( MIDIEventID id, MIDIChannel_Instance *inst, unsigned long start_index, int dir ) {
  MIDIChannel_Event               *event  = NULL;
  DynArray< MIDIChannel_Event * > &events = inst->GetTracks()[ inst->GetTrack() ]->GetChannels()[ inst->GetChannel() ]->GetEvents();

  if( events.NumElements() < 1 )
    return NULL;

  if( start_index >= events.NumElements() )
    return NULL;

  int           increment = (dir < 1) ? -1 : 1;
  unsigned long test      = (dir < 1) ? -1 : events.NumElements() - 1;

  for( unsigned long note = start_index; note != test; note += increment ) {
    // Find the note in the low/high range
    if( (events[ note ]->GetNote() >= inst->GetLowNote()) &&
        (events[ note ]->GetNote() <= inst->GetHighNote()) ) {

      if( events[ note ]->GetEvent() <= id )
        event = events[ note ];

      if( event != NULL )
        break;
    }

    // Try other events at this time
    for( MIDIChannel_Event *temp_next_event = events[ note ]->GetNextEvent(); temp_next_event != NULL; temp_next_event = temp_next_event->GetNextEvent() ) {
      if( (temp_next_event->GetNote() >= inst->GetLowNote()) &&
          (temp_next_event->GetNote() <= inst->GetHighNote()) ) {

        if( temp_next_event->GetEvent() <= id ) {
          event = temp_next_event;
          break;
        }
      }
    }

    if( event != NULL )
      break;
  }

  return event;
}

// MIDIChannel_Flags():
unsigned int MIDIChannel_Flags( LWInstance _inst ) {
  return 0;
}

// MIDIChannelInterface_Activate():
XCALL_ (int) MIDIChannelInterface_Activate( long version, GlobalFunc *global,
                                            void *_local, void *serverData ) {
  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  // Get XPanels
  LWXPanelFuncs *xpanels = (LWXPanelFuncs *)global( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  if( xpanels == NULL )
    return AFUNC_BADGLOBAL;

  // Create the Panel
  LWXPanelID panel = (*xpanels->create)( LWXP_VIEW, ctrl_list );
  if( panel != NULL ) {
    (*xpanels->hint)(     panel, 0, hint );
    (*xpanels->describe)( panel, data_descrip, MIDIChannel_Get, MIDIChannel_Set );
    (*xpanels->viewInst)( panel, local->inst );
    (*xpanels->setData)(  panel, 0, local->inst );
    (*xpanels->setData)(  panel, MUI_TRACK, local->inst );
    (*xpanels->setData)(  panel, MUI_CHANNEL, local->inst );
  }

  // Fill in the local data
  local->panel   = panel;
  local->options = NULL;
  local->command = NULL;

  return AFUNC_OK;
}

// MIDIChannel_Get():
void * MIDIChannel_Get( void *_inst, unsigned long vid ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)_inst;

  static double retvald = 0;
  static int    retvali = 0;

  if( vid != MUI_MIDI_FILE ) {
    if( inst->GetMIDIFile() == NULL )
      return NULL;
  }

  if( inst != NULL ) {
    switch( vid ) {
      case MUI_MIDI_FILE:
        return (void *)((inst->GetMIDIFile() == NULL) ? "" : inst->GetMIDIFile());

      case MUI_START_TIME:
        retvald = inst->GetStartTime();
        return &retvald;

      case MUI_TRACK:
        retvali = inst->GetTrack();
        return &retvali;

      case MUI_CHANNEL:
        retvali = inst->GetChannel();
        return &retvali;

      case MUI_PARAM:
        retvali = inst->GetParameter();
        return &retvali;

      case MUI_OFFSET:
        retvald = inst->GetOffset();
        return &retvald;

      case MUI_SCALE:
        retvald = inst->GetScale();
        return &retvald;

      case MUI_LOW_NOTE:
        retvali = inst->GetLowNote();
        return &retvali;

      case MUI_HIGH_NOTE:
        retvali = inst->GetHighNote();
        return &retvali;

      case MUI_NOTE_SHIFT:
        retvali = inst->GetNoteShift();
        return &retvali;

      case MUI_NOTE_OFF_LEVEL:
        retvali = inst->GetNoteOffLevel();
        return &retvali;

      case MUI_DECAY:
        retvald = inst->GetDecay();
        return &retvald;

      case MUI_EASE:
        retvald = inst->GetEase();
        return &retvald;

      case MUI_ADDITIVE:
        retvali = (int)inst->GetAdditive();
        return &retvali;
    }
  }

  return NULL;
}

// MIDIChannel_Set():
en_LWXPRefreshCodes MIDIChannel_Set( void *_inst, unsigned long vid, void *value ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)_inst;

  LWInstUpdate *instupdate = (LWInstUpdate *)inst->global( LWINSTUPDATE_GLOBAL, GFUSE_TRANSIENT );

  en_LWXPRefreshCodes retval;
  if( inst != NULL ) {
    switch ( vid ) {
      case MUI_MIDI_FILE:
        inst->SetMIDIFile( (const char *)value, false );
        retval = LWXPRC_FULL;
        break;

      case MUI_TRACK:
        inst->SetTrack( *((int*)value) );
        retval = LWXPRC_FULL;
        break;

      case MUI_CHANNEL:
        inst->SetChannel( *((int*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_PARAM:
        inst->SetParameter( *((int*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_START_TIME:
        inst->SetStartTime( *((double*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_OFFSET:
        inst->SetOffset( *((double*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_SCALE:
        inst->SetScale( *((double*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_LOW_NOTE:
        inst->SetLowNote( *((int*)value) );
        retval = LWXPRC_FULL;
        break;

      case MUI_HIGH_NOTE:
        inst->SetHighNote( *((int*)value) );
        retval = LWXPRC_FULL;
        break;

      case MUI_NOTE_SHIFT:
        inst->SetNoteShift( *((int*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_NOTE_OFF_LEVEL:
        inst->SetNoteOffLevel( *((int*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_DECAY:
        inst->SetDecay( *((double*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_EASE:
        inst->SetEase( *((double*)value) );
        retval = LWXPRC_DFLT;
        break;

      case MUI_ADDITIVE:
        inst->SetAdditive( *((int*)value) != 0 );
        retval = LWXPRC_DFLT;
        break;

      default:
        retval = LWXPRC_NONE;
        break;
    }
  }

  // Update the instance
  instupdate( LWCHANNEL_HCLASS, _inst );

  return retval;
}
     
// HandleAbout():
void HandleAbout( LWXPanelID pan, int cid ) {
  LWPanelFuncs *panel_funcs = (LWPanelFuncs *)MIDIChannel_Instance::global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  LWAboutPanel_TMPro( panel_funcs, "MIDI Channel", about_text, NULL, true );
}

// Track Popup Callbacks
XCALL_ (int) TrackCount( void *data ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)data;
  return inst->GetTracks().NumElements();
}

XCALL_ (const char *) TrackName( void *data, int idx ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)data;

  if( idx >= (int)inst->GetTracks().NumElements() )
    return NULL;

  static char buffer[ 256 ];
  sprintf( buffer, "%d: %s", idx + 1, ((inst->GetTracks()[ idx ]->GetName() == NULL) ? "(unnamed)" : inst->GetTracks()[ idx ]->GetName()) );
  return buffer;
}

// Channel Popup Callbacks
XCALL_ (int) ChannelCount( void *data ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)data;
  return inst->GetTracks()[ inst->GetTrack() ]->GetChannels().NumElements();
}

XCALL_ (const char *) ChannelName(  void *data, int idx ) {
  MIDIChannel_Instance *inst = (MIDIChannel_Instance *)data;

  if( inst->GetTracks().NumElements() == 0 )
    return NULL;

  if( idx >= (int)inst->GetTracks()[ inst->GetTrack() ]->GetChannels().NumElements() )
    return NULL;

  static char buffer[ 256 ];
  sprintf( buffer, "%d", inst->GetTracks()[ inst->GetTrack() ]->GetChannels()[ idx ]->GetIndex() + 1 );
  return buffer;
}

// Parameter Popup Callbacks
XCALL_ (int) ParamCount( void *data ) {
  return 2;
}

XCALL_ (const char *) ParamName(  void *data, int idx ) {
  const char *param_names[] = { "Note On/Off", "Note Velocity" };

  if( idx > 1 )
    return "";

  return param_names[ idx ];
}

// MIDIEventBinarySearchByTime():
//  Used to find the event nearest to the current time via the DynArray BinarySearch()
int (__cdecl MIDIEventBinarySearchByTime)(const void *elem1, const void *data ) {
  MIDIChannel_Event *event = (MIDIChannel_Event *)elem1;
  double             time  = *((double *)data);

  if( event->GetWhen() == time )
    return 0;

  return ((time < event->GetWhen()) ? -1 : 1);
}

