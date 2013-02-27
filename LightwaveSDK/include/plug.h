/*
 * LWSDK Header File
 * Copyright 1995,1997  NewTek, Inc.
 */
#ifndef TOOLS_PLUG_H
#define TOOLS_PLUG_H

typedef void *          GlobalFunc (const char *, int);

#define GFUSE_TRANSIENT         0
#define GFUSE_ACQUIRE           1
#define GFUSE_RELEASE           2

typedef struct st_GlobalService {
	const char      *id;
	void            *data;
} GlobalService;

typedef int ActivateFunc (long        version,
                          GlobalFunc *global,
                          void       *local,
                          void       *serverData );

#define AFUNC_OK            0
#define AFUNC_BADVERSION    1
#define AFUNC_BADGLOBAL     2
#define AFUNC_BADLOCAL      3
#define AFUNC_BADAPP        4

#ifdef _WIN32
 #define XCALL_(t)      t
 #define XCALL_INIT
#endif
#ifdef _XGL
 #define XCALL_(t)      t
 #define XCALL_INIT
#endif
#ifdef _MACOS
 #define XCALL_(t)      t
 #define XCALL_INIT
#endif

#endif
