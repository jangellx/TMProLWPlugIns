//
// UberNull.c
//
// This file contains the C portion of UberNull, as required by the
// LW 5.x plug-in API.
//

#include <stdlib.h>
#include <splug.h>
#include <moni.h>
#include <lwran.h>

// Plug-In Information for Lightwave's Use
// Function Prototypes
extern XCALL_ (int) UberNull_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

// Plug-In Information for Lightwave's Use
ServerRecord ServerDesc[] = {
#ifdef _DEBUG
    { "LayoutGeneric", "TM-P_UberNull--Debug", UberNull_Activate },
#else
    { "LayoutGeneric", "TM-P_UberNull",        UberNull_Activate },
#endif
    { NULL }
};


