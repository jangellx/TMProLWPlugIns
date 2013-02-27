/*
 * LWSDK Header File
 * Copyright 1995,1997  NewTek, Inc.
 */
#ifndef TOOLS_SPLUG_H
#define TOOLS_SPLUG_H

#include <plug.h>

typedef struct st_ServerRecord {
	const char      *className;
	const char      *name;
	ActivateFunc    *activate;
} ServerRecord;

#ifdef HEADER_LW_SDK_CPP_WRAPPERS
  extern "C" { void * Startup (void); }
  extern "C" { void   Shutdown ( void *serverData); }
#else
  extern void * Startup (void);
  extern void   Shutdown ( void *serverData);
#endif

#endif
