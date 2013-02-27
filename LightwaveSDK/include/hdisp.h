/*
 * LWSDK Header File
 * Copyright 1995,1997  NewTek, Inc.
 */
#ifndef TOOLS_HDISP_H
#define TOOLS_HDISP_H

#ifdef _XGL
 #include <X11/Xlib.h>
#endif
#ifdef _WIN32
 #include <windows.h>
#endif
#ifdef _MACOS
 #include <Quickdraw.h>
#endif

typedef struct st_HostDisplayInfo {
    #ifdef _XGL
	Display         *xsys;
	Window           window;
    #endif

    #ifdef _MACOS
	WindowPtr        window;
    #endif

    #ifdef _WIN32
	HANDLE           instance;
	HWND             window;
    #endif
} HostDisplayInfo;

#endif
