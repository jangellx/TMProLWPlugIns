//
// Simplify.c
//
// This file contains the C portion of Simplify, as required by the
// LW 5.x plug-in API.
//

#include <stdlib.h>
#include <splug.h>
#include <moni.h>
#include <lwran.h>

#include "Simplify.h"

// Plug-In Information for Lightwave's Use
ServerRecord ServerDesc[] = {
#ifdef _DEBUG
    { "ObjReplacementHandler",   "TM-P_Simplify--Debug",                   Simplify_Activate          },
    { "ObjReplacementInterface", "TM-P_Simplify--Debug",                   Simplify_Interface         },
    { "LayoutGeneric",           "TM-P_Simplify_TogglePreview--Debug",     Simplify_TogglePreview     },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleRender--Debug",      Simplify_ToggleRender      },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleLOD--Debug",         Simplify_ToggleLOD         },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleAutoLoad--Debug",    Simplify_ToggleAutoLoad    },
    { "LayoutGeneric",           "TM-P_Simplify_GlobalControl--Debug",     Simplify_GlobalControl     },
#else
    { "ObjReplacementHandler",   "TM-P_Simplify",                          Simplify_Activate          },
    { "ObjReplacementInterface", "TM-P_Simplify",                          Simplify_Interface         },
    { "LayoutGeneric",           "TM-P_Simplify_TogglePreview",            Simplify_TogglePreview     },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleRender",             Simplify_ToggleRender      },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleLOD",                Simplify_ToggleLOD         },
    { "LayoutGeneric",           "TM-P_Simplify_ToggleAutoLoad",           Simplify_ToggleAutoLoad    },
    { "LayoutGeneric",           "TM-P_Simplify_GlobalControl",            Simplify_GlobalControl     },
#endif
    { NULL }
};


