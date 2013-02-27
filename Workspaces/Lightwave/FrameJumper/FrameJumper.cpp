//
// FrameJumper.cpp
//

#include "framejumper.h"
#include <stdio.h>

#include "FrameJumper-Interface.h"
#include "FrameJumper-Data.h"

extern FrameJumper_Data      fj_data;
extern FrameJumper_Interface *fj_int;

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

const char *framejumper_server_name = "TM-P_FrameJumper"DEBUG_PLUGIN_PLUGNAME;

extern "C" {
  ServerUserName JumpToRenderFirstFrame_Usernames[]       = { {"TM-P  Jump to First Frame (Render)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_USERNAME },  { "To Start (Render)"DEBUG_PLUGIN_USERNAME,             LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToRenderLastFrame_Usernames[]        = { {"TM-P  Jump to Last Frame (Render)"DEBUG_PLUGIN_USERNAME,             LANGID_USENGLISH | SRVTAG_USERNAME },  { "To End (Render)"DEBUG_PLUGIN_USERNAME,               LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToPreviewFirstFrame_Usernames[]      = { {"TM-P  Jump to First Frame (Preview)"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_USERNAME },  { "To Start (Preview)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToPreviewLastFrame_Usernames[]       = { {"TM-P  Jump to Last Frame (Preview)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_USERNAME },  { "To End (Preview)"DEBUG_PLUGIN_USERNAME,              LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName NextFrameByRenderStep_Usernames[]        = { {"TM-P  Next Frame At Step (Render)"DEBUG_PLUGIN_USERNAME,             LANGID_USENGLISH | SRVTAG_USERNAME },  { "Next Step (Render)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName PreviousFrameByRenderStep_Usernames[]    = { {"TM-P  Previous Frame At Step (Render)"DEBUG_PLUGIN_USERNAME,         LANGID_USENGLISH | SRVTAG_USERNAME },  { "Prev Step (Render)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName NextFrameByPreviewStep_Usernames[]       = { {"TM-P  Next Frame At Step (Preview)"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_USERNAME },  { "Next Step (Preview)"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName PreviousFrameByPreviewStep_Usernames[]   = { {"TM-P  Previous Frame At Step (Preview)"DEBUG_PLUGIN_USERNAME,        LANGID_USENGLISH | SRVTAG_USERNAME },  { "Prev Step (Preview)"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName SetRenderRangeToPreviewRange_Usernames[] = { {"TM-P  Set Render Range to Preview Range"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_USERNAME },  { "Set Render To Preview"DEBUG_PLUGIN_USERNAME,         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SetPreviewRangeToRenderRange_Usernames[] = { {"TM-P  Set Preview Range to Render Range"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_USERNAME },  { "Set Preview To Render"DEBUG_PLUGIN_USERNAME,         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName JumpToNextMarker_Usernames[]             = { {"TM-P  Jump To Next Marker"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Next Marker"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToPreviousMarker_Usernames[]         = { {"TM-P  Jump To Previous Marker"DEBUG_PLUGIN_USERNAME,                 LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Previous Marker"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName JumpToNextMarkerGroup_Usernames[]        = { {"TM-P  Jump To Next Marker Group"DEBUG_PLUGIN_USERNAME,               LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Next Marker Group"DEBUG_PLUGIN_USERNAME,     LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToPreviousMarkerGroup_Usernames[]    = { {"TM-P  Jump To Previous Marker Group"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Previous Marker Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName MarkFrame_Usernames[]                    = { {"TM-P  Mark Frame"DEBUG_PLUGIN_USERNAME,                              LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame"DEBUG_PLUGIN_USERNAME,                    LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName UnmarkFrame_Usernames[]                  = { {"TM-P  Unmark Frame"DEBUG_PLUGIN_USERNAME,                            LANGID_USENGLISH | SRVTAG_USERNAME },  { "Unmark Frame"DEBUG_PLUGIN_USERNAME,                  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName MarkFrameAs1_Usernames[]                 = { {"TM-P  Mark Frame As Key 1"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 1"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs2_Usernames[]                 = { {"TM-P  Mark Frame As Key 2"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 2"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs3_Usernames[]                 = { {"TM-P  Mark Frame As Key 3"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 3"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs4_Usernames[]                 = { {"TM-P  Mark Frame As Key 4"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 4"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs5_Usernames[]                 = { {"TM-P  Mark Frame As Key 5"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 5"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs6_Usernames[]                 = { {"TM-P  Mark Frame As Key 6"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 6"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs7_Usernames[]                 = { {"TM-P  Mark Frame As Key 7"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 7"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs8_Usernames[]                 = { {"TM-P  Mark Frame As Key 8"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 8"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs9_Usernames[]                 = { {"TM-P  Mark Frame As Key 9"DEBUG_PLUGIN_USERNAME,                     LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 9"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs10_Usernames[]                = { {"TM-P  Mark Frame As Key 10"DEBUG_PLUGIN_USERNAME,                    LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 10"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs11_Usernames[]                = { {"TM-P  Mark Frame As Key 11"DEBUG_PLUGIN_USERNAME,                    LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 11"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName MarkFrameAs12_Usernames[]                = { {"TM-P  Mark Frame As Key 12"DEBUG_PLUGIN_USERNAME,                    LANGID_USENGLISH | SRVTAG_USERNAME },  { "Mark Frame As Key 12"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName JumpToMarker1_Usernames[]                 = { {"TM-P  Jump To Marker At Key 1"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 1"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker2_Usernames[]                 = { {"TM-P  Jump To Marker At Key 2"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 2"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker3_Usernames[]                 = { {"TM-P  Jump To Marker At Key 3"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 3"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker4_Usernames[]                 = { {"TM-P  Jump To Marker At Key 4"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 4"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker5_Usernames[]                 = { {"TM-P  Jump To Marker At Key 5"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 5"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker6_Usernames[]                 = { {"TM-P  Jump To Marker At Key 6"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 6"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker7_Usernames[]                 = { {"TM-P  Jump To Marker At Key 7"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 7"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker8_Usernames[]                 = { {"TM-P  Jump To Marker At Key 8"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 8"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker9_Usernames[]                 = { {"TM-P  Jump To Marker At Key 9"DEBUG_PLUGIN_USERNAME,                LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 9"DEBUG_PLUGIN_USERNAME,       LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker10_Usernames[]                = { {"TM-P  Jump To Marker At Key 10"DEBUG_PLUGIN_USERNAME,               LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 10"DEBUG_PLUGIN_USERNAME,      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker11_Usernames[]                = { {"TM-P  Jump To Marker At Key 11"DEBUG_PLUGIN_USERNAME,               LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 11"DEBUG_PLUGIN_USERNAME,      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName JumpToMarker12_Usernames[]                = { {"TM-P  Jump To Marker At Key 12"DEBUG_PLUGIN_USERNAME,               LANGID_USENGLISH | SRVTAG_USERNAME },  { "Jump To Marker At Key 12"DEBUG_PLUGIN_USERNAME,      LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName FrameJumperMaster_Usernames[]            = { {"TM-P  Frame Jumper"DEBUG_PLUGIN_USERNAME,                            LANGID_USENGLISH | SRVTAG_USERNAME },  { "Frame Jumper"DEBUG_PLUGIN_USERNAME,                  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName FrameJumperInterface_Usernames[]         = { {"TM-P  Frame Jumper"DEBUG_PLUGIN_USERNAME,                            LANGID_USENGLISH | SRVTAG_USERNAME },  { "Frame Jumper"DEBUG_PLUGIN_USERNAME,                  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName FrameJumperInterfaceGeneric_Usernames[]  = { {"TM-P  Frame Jumper Panel"DEBUG_PLUGIN_USERNAME,                      LANGID_USENGLISH | SRVTAG_USERNAME },  { "Frame Jumper Panel"DEBUG_PLUGIN_USERNAME,            LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToRenderFirstFrame"DEBUG_PLUGIN_PLUGNAME,        JumpToRenderFirstFrame_Activate,        JumpToRenderFirstFrame_Usernames        },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToRenderLastFrame"DEBUG_PLUGIN_PLUGNAME,         JumpToRenderLastFrame_Activate,         JumpToRenderLastFrame_Usernames         },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToPreviewFirstFrame"DEBUG_PLUGIN_PLUGNAME,       JumpToPreviewFirstFrame_Activate,       JumpToPreviewFirstFrame_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToPreviewLastFrame"DEBUG_PLUGIN_PLUGNAME,        JumpToPreviewLastFrame_Activate,        JumpToPreviewLastFrame_Usernames        },

    { LWLAYOUTGENERIC_CLASS, "TM-P_NextFrameByRenderStep"DEBUG_PLUGIN_PLUGNAME,         NextFrameByRenderStep_Activate,         NextFrameByRenderStep_Usernames         },
    { LWLAYOUTGENERIC_CLASS, "TM-P_PreviousFrameByRenderStep"DEBUG_PLUGIN_PLUGNAME,     PreviousFrameByRenderStep_Activate,     PreviousFrameByRenderStep_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_NextFrameByPreviewStep"DEBUG_PLUGIN_PLUGNAME,        NextFrameByPreviewStep_Activate,        NextFrameByPreviewStep_Usernames        },
    { LWLAYOUTGENERIC_CLASS, "TM-P_PreviousFrameByPreviewStep"DEBUG_PLUGIN_PLUGNAME,    PreviousFrameByPreviewStep_Activate,    PreviousFrameByPreviewStep_Usernames    },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SetRenderRangeToPreviewRange"DEBUG_PLUGIN_PLUGNAME,  SetRenderRangeToPreviewRange_Activate,  SetRenderRangeToPreviewRange_Usernames  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SetPreviewRangeToRenderRange"DEBUG_PLUGIN_PLUGNAME,  SetPreviewRangeToRenderRange_Activate,  SetPreviewRangeToRenderRange_Usernames  },

    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToNextMarker"DEBUG_PLUGIN_PLUGNAME,              JumpToNextMarker_Activate,              JumpToNextMarker_Usernames              },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToPreviousMarker"DEBUG_PLUGIN_PLUGNAME,          JumpToPreviousMarker_Activate,          JumpToPreviousMarker_Usernames          },

    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToNextMarkerGroup"DEBUG_PLUGIN_PLUGNAME,         JumpToNextMarkerGroup_Activate,         JumpToNextMarkerGroup_Usernames         },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToPreviousMarkerGroup"DEBUG_PLUGIN_PLUGNAME,     JumpToPreviousMarkerGroup_Activate,     JumpToPreviousMarkerGroup_Usernames     },

    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrame"DEBUG_PLUGIN_PLUGNAME,                     MarkFrame_Activate,                     MarkFrame_Usernames                     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_UnmarkFrame"DEBUG_PLUGIN_PLUGNAME,                   UnmarkFrame_Activate,                   UnmarkFrame_Usernames                   },

    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs1"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs1_Activate,                  MarkFrameAs1_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs2"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs2_Activate,                  MarkFrameAs2_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs3"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs3_Activate,                  MarkFrameAs3_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs4"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs4_Activate,                  MarkFrameAs4_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs5"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs5_Activate,                  MarkFrameAs5_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs6"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs6_Activate,                  MarkFrameAs6_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs7"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs7_Activate,                  MarkFrameAs7_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs8"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs8_Activate,                  MarkFrameAs8_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs9"DEBUG_PLUGIN_PLUGNAME,                  MarkFrameAs9_Activate,                  MarkFrameAs9_Usernames                  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs10"DEBUG_PLUGIN_PLUGNAME,                 MarkFrameAs10_Activate,                 MarkFrameAs10_Usernames                 },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs11"DEBUG_PLUGIN_PLUGNAME,                 MarkFrameAs11_Activate,                 MarkFrameAs11_Usernames                 },
    { LWLAYOUTGENERIC_CLASS, "TM-P_MarkFrameAs12"DEBUG_PLUGIN_PLUGNAME,                 MarkFrameAs12_Activate,                 MarkFrameAs12_Usernames                 },

    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker1"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker1_Activate,                  JumpToMarker1_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker2"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker2_Activate,                  JumpToMarker2_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker3"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker3_Activate,                  JumpToMarker3_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker4"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker4_Activate,                  JumpToMarker4_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker5"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker5_Activate,                  JumpToMarker5_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker6"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker6_Activate,                  JumpToMarker6_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker7"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker7_Activate,                  JumpToMarker7_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker8"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker8_Activate,                  JumpToMarker8_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker9"DEBUG_PLUGIN_PLUGNAME,                 JumpToMarker9_Activate,                  JumpToMarker9_Usernames                },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker10"DEBUG_PLUGIN_PLUGNAME,                JumpToMarker10_Activate,                 JumpToMarker10_Usernames               },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker11"DEBUG_PLUGIN_PLUGNAME,                JumpToMarker11_Activate,                 JumpToMarker11_Usernames               },
    { LWLAYOUTGENERIC_CLASS, "TM-P_JumpToMarker12"DEBUG_PLUGIN_PLUGNAME,                JumpToMarker12_Activate,                 JumpToMarker12_Usernames               },

    { LWMASTER_HCLASS,       framejumper_server_name,                                   FrameJumperMaster_Activate,             FrameJumperMaster_Usernames             },
    { LWMASTER_ICLASS,       framejumper_server_name,                                   FrameJumperInterface_Activate,          FrameJumperMaster_Usernames             },

    { LWLAYOUTGENERIC_CLASS, "TM-P_FrameJumperProperties"DEBUG_PLUGIN_PLUGNAME,         FrameJumperInterfaceGeneric_Activate,   FrameJumperInterfaceGeneric_Usernames   },

    { (const char *)NULL }
  };
}

// JumpToRenderFirstFrame_Activate():
//  Sets the current frame number to the Render First Frame.
XCALL_ (int) JumpToRenderFirstFrame_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", sceneinfo->frameStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// JumpToRenderLastFrame_Activate():
//  Sets the current frame number to the Render Last Frame.
XCALL_ (int) JumpToRenderLastFrame_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", sceneinfo->frameEnd );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// JumpToPreviewFirstFrame_Activate():
//  Sets the current frame number to the Preview First Frame.
XCALL_ (int) JumpToPreviewFirstFrame_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  LWInterfaceInfo *ui      = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", ui->previewStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// JumpToPreviewLastFrame_Activate():
//  Sets the current frame number to the Preview Last Frame.
XCALL_ (int) JumpToPreviewLastFrame_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  LWInterfaceInfo *ui      = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", ui->previewEnd );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// NextFrameByRenderStep_Activate():
//  Sets forward to the next frame in the Frame Step.
XCALL_ (int) NextFrameByRenderStep_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  LWFrame now = (int)(ui->curTime * sceneinfo->framesPerSecond) - sceneinfo->frameStart;
  LWFrame next = now + sceneinfo->frameStep - (now % sceneinfo->frameStep);

  if( now == next )
    next += sceneinfo->frameStep;

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", next + sceneinfo->frameStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// PreviousFrameByRenderStep_Activate():
//  Sets backward to the next frame in the Frame Step.
XCALL_ (int) PreviousFrameByRenderStep_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  LWFrame now = (int)(ui->curTime * sceneinfo->framesPerSecond) - sceneinfo->frameStart;
  LWFrame next = now - (now % sceneinfo->frameStep);

  if( now == next )
    next -= sceneinfo->frameStep;

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", next + sceneinfo->frameStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// NextFrameByPreviewStep_Activate():
//  Sets forward to the next frame in the Frame Step.
XCALL_ (int) NextFrameByPreviewStep_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  LWFrame now = (int)(ui->curTime * sceneinfo->framesPerSecond) - ui->previewStart;
  LWFrame next = now + ui->previewStep - (now % ui->previewStep);

  if( now == next )
    next += ui->previewStep;

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", next + ui->previewStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// PreviousFrameByPreviewStep_Activate():
//  Sets backward to the next frame in the Frame Step.
XCALL_ (int) PreviousFrameByPreviewStep_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  LWFrame now = (int)(ui->curTime * sceneinfo->framesPerSecond) - ui->previewStart;
  LWFrame next = now - (now % ui->previewStep);

  if( now == next )
    next -= ui->previewStep;

  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %d", next + ui->previewStart );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// SetPreviewRangeToRenderRange_Activate():
//  Sets the Preview Range to the Render Range.
XCALL_ (int) SetPreviewRangeToRenderRange_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWMessageFuncs  *message   = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  if( (*message->yesNo)( "Set Preview Range To Render Range?",
                         "Set the Preview Start, End and Step to their Render counterparts?", NULL ) == 0 ) {
    return AFUNC_OK;
  }

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "PreviewFirstFrame %d", sceneinfo->frameStart );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "PreviewLastFrame %d", sceneinfo->frameEnd );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "PreviewFrameStep %d", sceneinfo->frameStep );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// SetRenderRangeToPreviewRange_Activate():
//  Sets the Preview Range to the Render Range.
XCALL_ (int) SetRenderRangeToPreviewRange_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWMessageFuncs  *message   = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  if( (*message->yesNo)( "Set Render Range To Preview Range?",
                         "Set the Render Start, End and Step to their Preview counterparts?", NULL ) == 0 ) {
    return AFUNC_OK;
  }

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  char buffer[ 100 ];
  sprintf( buffer, "FirstFrame %d", ui->previewStart );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "LastFrame %d", ui->previewEnd );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "FrameStep %d", ui->previewStep );
  (*generic->evaluate)( generic->data, buffer );

  return AFUNC_OK;
}

// MarkFrame():
XCALL_ (int) MarkFrame_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 0, version, global, local );
}

// UnmarkFrame():
XCALL_ (int) UnmarkFrame_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  LWInterfaceInfo *ui = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  FrameJumper_Group  *group  = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  FrameJumper_Marker *marker = group->FindMarkerWithTime( ui->curTime );

  if( marker == NULL )
    return AFUNC_OK;

  group->GetMarkers().RemoveItem( marker );
  delete marker;

  if( fj_int != NULL ) {
    if( fj_int->IsOpen() )
      HandleList( fj_int->list, (void *)-1 );
  }

  return AFUNC_OK;
}

// MarkFrameAsXX():
XCALL_ (int) MarkFrameAs1_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 1, version, global, local );
}

XCALL_ (int) MarkFrameAs2_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 2, version, global, local );
}

XCALL_ (int) MarkFrameAs3_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 3, version, global, local );
}

XCALL_ (int) MarkFrameAs4_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 4, version, global, local );
}

XCALL_ (int) MarkFrameAs5_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 5, version, global, local );
}

XCALL_ (int) MarkFrameAs6_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 6, version, global, local );
}

XCALL_ (int) MarkFrameAs7_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 7, version, global, local );
}

XCALL_ (int) MarkFrameAs8_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 8, version, global, local );
}

XCALL_ (int) MarkFrameAs9_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  return MarkFrameAs( 9, version, global, local );
}

XCALL_ (int) MarkFrameAs10_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return MarkFrameAs( 10, version, global, local );
}

XCALL_ (int) MarkFrameAs11_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return MarkFrameAs( 11, version, global, local );
}

XCALL_ (int) MarkFrameAs12_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return MarkFrameAs( 12, version, global, local );
}

// MarkFrameAs():
int MarkFrameAs( int key, long version, GlobalFunc *global, void *local ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  LWInterfaceInfo *ui        = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );

  // Master plug-in not found; add it
  if( fj_data.GetUsers() == 0 ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWMASTER_HCLASS, framejumper_server_name );
    (*generic->evaluate)( generic->data, buffer );
  }

  FrameJumper_Group * group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  // Make sure no one else is using this key
  if( key > 0 ) {
    for( unsigned long i=0; i < group->GetMarkers().NumElements(); i++ ) {
      if( group->GetMarkers()[i]->GetKey() == key )
        group->GetMarkers()[i]->SetKey( 0 );
    }
  }

  // See if any markers are already at this time
  FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithTime( ui->curTime );
  if( marker == NULL ) {
    // None exist; create a new one
    marker = new FrameJumper_Marker;
    marker->SetKey( key );
    marker->SetEnabled( true );
    marker->SetTime( ui->curTime );

    group->GetMarkers().Add( marker );
    group->GetMarkers().QSort( SortMarkersByTime );
  } else {
    if( key != 0 )
      marker->SetKey( key );
  }

  int index = group->GetMarkers().FindIndexOf( marker );
  group->SetCurrentMarker( index );

  // Update the UI
  if( fj_int != NULL ) {
    if( fj_int->IsOpen() ) {
      SET_INT( fj_int->list, group->GetCurrentMarker() );
      HandleList( fj_int->list, (void *)-1 );
    }
  }
    
  return AFUNC_OK;
};

// JumpToMarkerXX():
XCALL_ (int) JumpToMarker1_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 1, version, global, local );
}

XCALL_ (int) JumpToMarker2_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 2, version, global, local );
}

XCALL_ (int) JumpToMarker3_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 3, version, global, local );
}

XCALL_ (int) JumpToMarker4_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 4, version, global, local );
}

XCALL_ (int) JumpToMarker5_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 5, version, global, local );
}

XCALL_ (int) JumpToMarker6_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 6, version, global, local );
}

XCALL_ (int) JumpToMarker7_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 7, version, global, local );
}

XCALL_ (int) JumpToMarker8_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 8, version, global, local );
}

XCALL_ (int) JumpToMarker9_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData ) {
  return JumpToMarker( 9, version, global, local );
}

XCALL_ (int) JumpToMarker10_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  return JumpToMarker( 10, version, global, local );
}

XCALL_ (int) JumpToMarker11_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  return JumpToMarker( 11, version, global, local );
}

XCALL_ (int) JumpToMarker12_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  return JumpToMarker( 12, version, global, local );
}

// JumpToMarker():
int JumpToMarker( int key, long version, GlobalFunc *global, void *local ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );

  FrameJumper_Marker *marker = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ]->FindMarkerWithKey( key );
  if( marker == NULL )
    return AFUNC_OK;
    
  char buffer[ 100 ];
  sprintf( buffer, "GoToFrame %g", marker->GetTime() * sceneinfo->framesPerSecond );
  (*generic->evaluate)( generic->data, buffer );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];
  int index = group->GetMarkers().FindIndexOf( marker );
  group->SetCurrentMarker( index );

  // Update the UI
  if( fj_int != NULL ) {
    if( fj_int->IsOpen() ) {
      SET_INT( fj_int->list, group->GetCurrentMarker() );
      HandleList( fj_int->list, (void *)-1 );
    }
  }

  return AFUNC_OK;
};

// JumpToNextMarker():
XCALL_ (int) JumpToNextMarker_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  for( unsigned long i=group->GetCurrentMarker() + 1; i < group->GetMarkers().NumElements(); i++ ) {
    // Skip Disabled Markers
    if( group->GetMarkers()[ i ]->GetEnabled() ) {
      group->SetCurrentMarker( i );

      // Update the UI
      if( fj_int != NULL ) {
        if( fj_int->IsOpen() ) {
          SET_INT( fj_int->list, group->GetCurrentMarker() );
          HandleList( fj_int->list, (void *)-1 );
        }
      }

      // Jump to the Marker
      char buffer[ 100 ];
      sprintf( buffer, "GoToFrame %g", group->GetMarkers()[i]->GetTime() * sceneinfo->framesPerSecond );
      (*generic->evaluate)( generic->data, buffer );

      break;
    }
  }

  return AFUNC_OK;
}

// JumpToPreviousMarker():
XCALL_ (int) JumpToPreviousMarker_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  LWLayoutGeneric *generic   = (LWLayoutGeneric *)local;
  LWSceneInfo     *sceneinfo = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  for( unsigned long i=group->GetCurrentMarker() - 1; i != -1; i-- ) {
    // Skip Disabled Markers
    if( group->GetMarkers()[ i ]->GetEnabled() ) {
      group->SetCurrentMarker( i );

      // Update the UI
      if( fj_int != NULL ) {
        if( fj_int->IsOpen() ) {
          SET_INT( fj_int->list, group->GetCurrentMarker() );
          HandleList( fj_int->list, (void *)-1 );
        }
      }

      // Jump to the Marker
      char buffer[ 100 ];
      sprintf( buffer, "GoToFrame %g", group->GetMarkers()[i]->GetTime() * sceneinfo->framesPerSecond );
      (*generic->evaluate)( generic->data, buffer );

      break;
    }
  }

  return AFUNC_OK;
}

// JumpToNextMarkerGroup():
XCALL_ (int) JumpToNextMarkerGroup_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  if( fj_data.GetCurrentGroup() + 1 >= fj_data.GetGroups().NumElements() )
    return AFUNC_OK;

  fj_data.SetCurrentGroup( fj_data.GetCurrentGroup() + 1 );

  if( fj_int != NULL ) {
    if( fj_int->IsOpen() ) {
      SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );
      HandleGroupPopup( fj_int->group_popup, NULL );
    }
  }

  return AFUNC_OK;
}

// JumpToPreviousMarkerGroup():
XCALL_ (int) JumpToPreviousMarkerGroup_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  if( fj_data.GetUsers() == 0 )
    return AFUNC_OK;

  if( fj_data.GetCurrentGroup() < 1 )
    return AFUNC_OK;

  fj_data.SetCurrentGroup( fj_data.GetCurrentGroup() - 1 );

  if( fj_int != NULL ) {
    if( fj_int->IsOpen() ) {
      SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );
      HandleGroupPopup( fj_int->group_popup, NULL );
    }
  }

  return AFUNC_OK;
}
