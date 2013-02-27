//
// MIDIChannel-Interface.h
//

#include <lwxpanel.h>

// MIDIChannel_UI_IDs
enum MIDIChannel_UI_IDs {
  MUI_ABOUT = 0x8000,
  MUI_MIDI_FILE,
  MUI_START_TIME,
};

// class MIDIChannel_Interface
class MIDIChannel_Interface {
public:
  MIDIChannel_Interface( GlobalFunc *_global );
  ~MIDIChannel_Interface();

  bool OpenUI();

  LWXPanelFuncs   *xpanels;

};

