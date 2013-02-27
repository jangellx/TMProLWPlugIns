//
// LWWrappers.h
//
// This file contains a series of classes which encapsulate the structures
// used by the Lightwave 6 SDK.  It's purpose is to make those wacky function
// pointers into function references, so they can be used just like any other
// function.  Otherwise, they're the same.
//
// Include this file to use the wrappers in your code.
//

#ifndef HEADER_L6_SDK_CPP_WRAPPERS
#define HEADER_L6_SDK_CPP_WRAPPERS

#include <stdlib.h>
#include <lwserver.h>

//#include "Clwran.h"
#include "Cl6panel.h"
#include "Cl6Host.h"
#include "PanelToolsL6.h"
#include "PanelDrawL6.h"

// These values are required by the macros in lwpanel.h, and are provided
// for easy access in your main program.
extern LWPanControlDesc   desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

#endif