//
// MIDIChannel.h
//

#ifndef NULL
  #define NULL 0
#endif

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwchannel.h>

#include "MIDIChannel-Instance.h"

extern "C" {
  // Channel Handler
  XCALL_ (int) MIDIChannel_Activate( long version, GlobalFunc *global,
                                     void *_local, void *serverData );

  // Channel Interface
  XCALL_ (int) MIDIChannelInterface_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  // Interface Callbacks
  void *              MIDIChannel_Get( void *_inst, unsigned long vid );
  en_LWXPRefreshCodes MIDIChannel_Set( void *_inst, unsigned long vid, void *value );

  void                HandleAbout( LWXPanelID pan, int cid );

  XCALL_ (int)          TrackCount( void *data );
  XCALL_ (const char *) TrackName(  void *data, int idx );

  XCALL_ (int)          ChannelCount( void *data );
  XCALL_ (const char *) ChannelName(  void *data, int idx );

  XCALL_ (int)          ParamCount( void *data );
  XCALL_ (const char *) ParamName(  void *data, int idx );

  int (__cdecl MIDIEventBinarySearchByTime)(const void *elem1, const void *data );
}

MIDIChannel_Event * FindNote( MIDIEventID id, MIDIChannel_Instance *inst, unsigned long start_index, int dir );

// Interface Setup
enum MIDIChannel_UI_IDs {
  MUI_ABOUT = 0x8000,
  MUI_MIDI_FILE,
  MUI_TRACK,
  MUI_CHANNEL,
  MUI_PARAM,
  MUI_START_TIME,
  MUI_OFFSET,
  MUI_SCALE,
  MUI_LOW_NOTE,
  MUI_HIGH_NOTE,
  MUI_NOTE_SHIFT,
  MUI_DECAY,
  MUI_NOTE_OFF_LEVEL,
  MUI_EASE,
  MUI_ADDITIVE,

  MUI_GROUP_FILE,
  MUI_GROUP_BASE,
  MUI_GROUP_OPTIONS,
  MUI_GROUP_RANGE,
  MUI_GROUP_NOTE,
  MUI_GROUP_ABOUT,
};

static LWXPanelControl ctrl_list[] = {
    { MUI_MIDI_FILE,        "MIDI File",             "sFileName"  },
    { MUI_TRACK,            "Track",                 "iPopChoice" },
    { MUI_CHANNEL,          "Channel",               "iPopChoice" },
    { MUI_PARAM,            "Parameter",             "iPopChoice" },
    { MUI_START_TIME,       "Start Time",            "time"       },
    { MUI_OFFSET,           "Value Offset",          "float"      },
    { MUI_SCALE,            "Value Scale",           "float"      },
    { MUI_ADDITIVE,         "Additive",              "iBoolean"   },
    { MUI_LOW_NOTE,         "Low Note",              "integer"    },
    { MUI_HIGH_NOTE,        "High Note",             "integer"    },
    { MUI_NOTE_SHIFT,       "Parameter Shift",       "integer"    },
    { MUI_DECAY,            "Parameter Decay",       "time"       },
    { MUI_NOTE_OFF_LEVEL,   "Parameter Off Level",   "integer"    },
    { MUI_EASE,             "Parameter On Ease",     "time"       },
    { MUI_ABOUT,            "About",                 "vButton"    },
    {0} };

static LWXPanelDataDesc data_descrip[] = {
    { MUI_MIDI_FILE,        "MIDI File",             "string"  },
    { MUI_TRACK,            "Track",                 "integer" },
    { MUI_CHANNEL,          "Channel",               "integer" },
    { MUI_PARAM,            "Parameter",             "integer" },
    { MUI_START_TIME,       "Start Time",            "time"    },
    { MUI_OFFSET,           "Value Offset",          "float"   },
    { MUI_SCALE,            "Value Scale",           "float"   },
    { MUI_ADDITIVE,         "Additive",              "integer" },
    { MUI_LOW_NOTE,         "Low Note",              "integer" },
    { MUI_HIGH_NOTE,        "High Note",             "integer" },
    { MUI_NOTE_SHIFT,       "Parameter Shift",       "integer" },
    { MUI_DECAY,            "Parameter Decay",       "float"   },
    { MUI_NOTE_OFF_LEVEL,   "Parameter Off Level",   "integer" },
    { MUI_EASE,             "Parameter On Ease",     "time"    },
    {0} };

static LWXPanelHint hint[] = {
  XpLABEL(0,"MIDI Channel"),
  XpGROUP_( MUI_GROUP_FILE ),
    XpH( MUI_MIDI_FILE ),
    XpH( 0 ),

  XpGROUP_( MUI_GROUP_BASE ),
    XpH( MUI_TRACK ),
    XpH( MUI_CHANNEL ),
    XpH( MUI_PARAM ),
    XpH( 0 ),

  XpGROUP_( MUI_GROUP_OPTIONS ),
    XpH( MUI_START_TIME ),
    XpH( MUI_OFFSET ),
    XpH( MUI_SCALE ),
    XpH( MUI_ADDITIVE ),
    XpH( 0 ),

  XpGROUP_( MUI_GROUP_RANGE ),
    XpH( MUI_LOW_NOTE ),
    XpH( MUI_HIGH_NOTE ),
    XpH( MUI_NOTE_SHIFT ),
    XpH( 0 ),

  XpGROUP_( MUI_GROUP_NOTE ),
    XpH( MUI_NOTE_OFF_LEVEL ),
    XpH( MUI_DECAY ),
    XpH( MUI_EASE ),
    XpH( 0 ),

  XpGROUP_( MUI_GROUP_ABOUT ),
    XpH( MUI_ABOUT ),
    XpH( 0 ),

  XpPOPFUNCS( MUI_TRACK,   TrackCount,   TrackName ),
  XpPOPFUNCS( MUI_CHANNEL, ChannelCount, ChannelName ),
  XpPOPFUNCS( MUI_PARAM,   ParamCount,   ParamName ),

  XpBUTNOTIFY( MUI_ABOUT, HandleAbout ),

  XpEND };

