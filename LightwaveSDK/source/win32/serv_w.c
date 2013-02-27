/*
 * LWSDK Startup File
 * Copyright 1995,1997  NewTek, Inc.
 */
#include <splug.h>

typedef struct st_ModuleDescriptor {
	unsigned long            sysSync;
	unsigned long            sysVersion;
	unsigned long            sysMachine;
	void                  *(*startup) (void);
	void                   (*shutdown) (void *);
	ServerRecord            *serverDefs;
} ModuleDescriptor;
#define MOD_SYSSYNC              0x04121994
#define MOD_SYSVER               2
#ifdef _XGL
 #define MOD_MACHINE     0x200
#endif
#ifdef _WIN32
 #ifdef _MIPS_
  #define MOD_MACHINE    0x301
 #elif  _ALPHA_
  #define MOD_MACHINE    0x302
 #else
  #define MOD_MACHINE    0x300
 #endif
#endif
#ifdef _MACOS
 #define MOD_MACHINE     0x400
#endif
extern ServerRecord     ServerDesc[];

__declspec( dllexport ) ModuleDescriptor _mod_descrip = {
//ModuleDescriptor _mod_descrip = {
	MOD_SYSSYNC,
	MOD_SYSVER,
	MOD_MACHINE,
	Startup,
	Shutdown,
	ServerDesc
};
