
// LWWrappers.h
//
// This file contains a series of classes which encapsulate the structures
// used by the Lightwave SDK.  It's purpose is to make those wacky function
// pointers into function references, so they can be used just like any other
// function.  Otherwise, they're the same.
//
// Include this file to use the wrappers in your code.

#ifndef HEADER_LW_SDK_CPP_WRAPPERS
#define HEADER_LW_SDK_CPP_WRAPPERS

#ifdef LW_SDK_CPP_WRAPPERS_L6_HEADER
  #include <lwserver.h>
  #include <lwhost.h>
  #include <lwmonitor.h>
  #include <lwrender.h>
  #include <lwio.h>
  #include <lwdyna.h>
#else
  #include <splug.h>
  #include <stdlib.h>
  #include <moni.h>
  #include <lwran.h>
#endif

#include "Clwran.h"
#include "Clwpanel.h"
//#include <gui_help.h>
#include "PanelTools.h"
#include "PanelDraw.h"

#ifdef LW_SDK_CPP_WRAPPERS_L6_HEADER
  #include "RasterDraw.h"
#endif

// These values are required by the macros in lwpanels.h, and are provided
// for easy access in your main program.
extern LWPanControlDesc   desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

#endif

