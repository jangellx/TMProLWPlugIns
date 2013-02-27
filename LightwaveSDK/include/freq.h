/*
 * LWSDK Header File
 * Copyright 1995,1997  NewTek, Inc.
 */
#ifndef TOOLS_FREQ_H
#define TOOLS_FREQ_H

#include <plug.h>

#define FREQ_LOAD        1
#define FREQ_SAVE        2
#define FREQ_DIRECTORY   3
#define FREQ_MULTILOAD   4
typedef struct st_FileReq_Local {
	int              reqType, result;
	const char      *title;
	const char      *fileType;
	char            *path;
	char            *baseName;
	char            *fullName;
	int              bufLen;
	int            (*pickName) (void);
} FileReq_Local;
typedef const char      *FileTypeFunc (const char *);
typedef struct st_ColorPick_Local {
	const char      *title;
	int              red, green, blue;
	int              result;
} ColorPick_Local;
extern         int
FileReq_SysLocal (
	long                     version,
	GlobalFunc              *global,
	FileReq_Local           *local,
	void                    *dummy);
#ifdef _WIN32
 #define HAS_COLORPICK
 extern         int
ColorPick_SysLocal (
	long                     version,
	GlobalFunc              *global,
	ColorPick_Local         *local,
	void                    *dummy);
#endif

#endif
