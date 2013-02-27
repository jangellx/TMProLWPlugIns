//
// Cuttlefish.h
//

#ifndef NULL
  #define NULL 0
#endif

#include <lwserver.h>
#include <lwhost.h>
#include <lwmonitor.h>
#include <lwrender.h>
#include <lwio.h>
#include <lwdyna.h>

#include <lwgeneric.h>
#include <lwvolume.h>
#include <lwmaster.h>
#include <lwvparm.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

//#include "Imstance-Interface.h"

extern "C" {
	XCALL_ (int) Imstance_Activate( long version, GlobalFunc *global,
				        void *_local, void *serverData );

//	void * Startup( void );
//	void   Shutdown( void *serverData );

	// Pixel Filter functions
	LWInstance       Imstance_Create(   void *data, void *context, LWError *error );
	void             Imstance_Destroy(  LWInstance _inst );
	LWError          Imstance_Copy(     LWInstance _to, LWInstance _from );
	LWError          Imstance_Load(     LWInstance _inst, const LWLoadState *loader );
	LWError          Imstance_Save(     LWInstance _inst, const LWSaveState *saver );
	const char     * Imstance_Describe( LWInstance _inst );

	double           Imstance_Evaluate( LWInstance _inst, LWVolumeAccess *access );
	unsigned int     Imstance_Flags(    LWInstance _inst );

	LWError          Imstance_Init(     LWInstance _inst, int mode );
	void             Imstance_Cleanup(  LWInstance _inst );
	LWError          Imstance_NewTime(  LWInstance _inst, LWFrame frame , LWTime time );

	const LWItemID * Imstance_UseItems( LWInstance _inst );
	void             Imstance_ChangeID( LWInstance _inst, const LWItemID *ids );

	LWError          Imstance_OpenUI( void *data );
}

// Instance
//extern ImstanceInstance *inst;

// IMP Banks
/*
extern IMPBank bank_CuttlefishFlags;
extern IMPBank bank_CuttlefishBasic;
extern IMPBank bank_CuttlefishOverrideSilhouette;
extern IMPBank bank_CuttlefishOverrideUnshared;
extern IMPBank bank_CuttlefishOverrideSurface;
extern IMPBank bank_CuttlefishOverridePart;
extern IMPBank bank_CuttlefishOverrideNormal;
extern IMPBank bank_CuttlefishOverridePolyline;
*/
