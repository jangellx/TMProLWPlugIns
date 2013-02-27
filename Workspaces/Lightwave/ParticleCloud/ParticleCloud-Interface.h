//
// ParticleCloud-Interface.h
//

#include <lightwave/lw_sdk_cpp_wrappers/LWWrappers_L6.h>

#include "particlecloud-instance.h"

extern "C" {
  void                * ParticleCloud_Get( void *_inst, unsigned long vid );
  en_LWXPRefreshCodes   ParticleCloud_Set( void *_inst, unsigned long vid, void *value );
  void                  HandleAbout( LWXPanelID pan, int cid );

}

// ParticleCloud_UI_IDs
enum ParticleCloud_UI_IDs {
  PC_ABOUT = 0x8000,

  PC_START_FRAME,
  PC_BIRTH_RATE,
  PC_FROZEN,
  PC_INDICES,

  PC_CLEAR_PFX,
  PC_LOAD_PFX,
  PC_SAVE_PFX,

  PC_GROUP_ABOUT,
  PC_GROUP_BASE,
  PC_GROUP_FILE,
};

// pc_xpanel_controls
static LWXPanelControl pc_xpanel_controls[] = {
  { PC_START_FRAME,      "Start Frame",             "time"     },
  { PC_BIRTH_RATE,       "Birth Rate (per second)", "integer"  },
  { PC_FROZEN,           "Use Frozen Mesh",         "iBoolean" },
  { PC_INDICES,          "Show Particle Indices",   "iBoolean" },

  { PC_CLEAR_PFX,        "Clear PFX",               "vButton"  },
  { PC_LOAD_PFX,         "Load PFX",                "vButton"  },
  { PC_SAVE_PFX,         "Save PFX",                "vButton"  },

  { PC_ABOUT,            "About",                   "vButton"  },
  {0} };

// pc_xpanel_data
static LWXPanelDataDesc pc_xpanel_data[] = {
  { PC_START_FRAME,      "Start Frame",             "time"     },
  { PC_BIRTH_RATE,       "Birth Rate",              "integre"  },
  { PC_FROZEN,           "Use Frozen Mesh",         "integer"  },
  { PC_INDICES,          "Show Particle Indices",   "integer"  },
  {0} };


// pc_xpanel_hint
static LWXPanelHint pc_xpanel_hint[] = {
  XpLABEL( 0, "Particle Cloud" ),

  XpBUTNOTIFY( PC_ABOUT, HandleAbout ),

  XpGROUP_( PC_GROUP_BASE ),
    XpH( PC_START_FRAME ),
    XpH( PC_BIRTH_RATE ),
    XpH( PC_FROZEN ),
    XpH( PC_INDICES ),
    XpH( 0 ),

  XpGROUP_( PC_GROUP_FILE ),
    XpH( PC_CLEAR_PFX ),
    XpH( PC_LOAD_PFX ),
    XpH( PC_SAVE_PFX ),
    XpH( 0 ),
  XpLEFT_(), XpH( PC_LOAD_PFX ), XpH( PC_SAVE_PFX ), XpH( 0 ),

  XpGROUP_( PC_GROUP_ABOUT ),
    XpH( PC_ABOUT ),
    XpH( 0 ),

  XpBUTNOTIFY( PC_ABOUT, HandleAbout ),

  XpEND };

