//
// Imstance.cpp
//

#include "Imstance.h"
#include "lightwave/vecmath/vecmath_inline.h"

#include <stdio.h>

extern "C" {
	// User Names
	ServerUserName Imstance_Usernames[] = { {"Imstance" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Imstance" DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

	// Server Record
	ServerRecord ServerDesc[] = {
		{ LWVOLUMETRIC_HCLASS, "Imstance" DEBUG_PLUGIN_PLUGNAME, Imstance_Activate, Imstance_Usernames },
		{ (const char *)NULL }
	};
}

// Globals
LWMessageFuncs		*message       = NULL;
GlobalFunc		*global        = NULL;
LWItemInfo		*itemInfo      = NULL;
LWObjectInfo		*objectInfo    = NULL;
LWObjectFuncs		*objectFuncs   = NULL;
//IMPGlobal		*imp           = NULL;

int			 instanceCount = 0;
//ImstanceInstance	*inst          = NULL;

double			 now;

// Imstance_Activate():
XCALL_ (int) Imstance_Activate( long version, GlobalFunc *_global,
                                void *_local, void *serverData ) {

	if( version != LWVOLUMETRIC_VERSION )
		return(AFUNC_BADVERSION);

	LWVolumetricHandler *local = (LWVolumetricHandler *)_local;

	if( local->inst != NULL ) {
		local->inst->create     = Imstance_Create;
		local->inst->destroy    = Imstance_Destroy;
		local->inst->load       = Imstance_Load;
		local->inst->save       = Imstance_Save;
		local->inst->copy       = Imstance_Copy;
		local->inst->descln     = Imstance_Describe;
	}

	if( local->item != NULL ) {
		local->item->useItems   = NULL;
		local->item->changeID   = NULL;
	}

	if( local->rend != NULL ) {
		local->rend->init       = Imstance_Init;
		local->rend->cleanup    = Imstance_Cleanup;
		local->rend->newTime    = Imstance_NewTime;
	}

	local->evaluate                 = Imstance_Evaluate;
	local->flags                    = Imstance_Flags;

	if( global == NULL )
		global = _global;

	return AFUNC_OK;
}

// Imstance_Create():
XCALL_( static LWInstance ) Imstance_Create( void *priv, void *context, LWError *err ) {
	itemInfo = (LWItemInfo *)global( LWITEMINFO_GLOBAL, GFUSE_ACQUIRE );

	return &instanceCount;
}

// Imstance_Destroy():
XCALL_( static void ) Imstance_Destroy( void *inst ) {
	global( LWITEMINFO_GLOBAL, GFUSE_RELEASE );     itemInfo = NULL;

	return;
}

// Imstance_Copy():
XCALL_( static LWError ) Imstance_Copy( void *to, void *from) {
	return NULL;
}

// Imstance_Load():
XCALL_( static LWError ) Imstance_Load( void *dat, const LWLoadState *lState ) {
	return NULL;
}

// Imstance_Save():
XCALL_( LWError ) Imstance_Save( void *dat, const LWSaveState *sState ) {
	return NULL;
}

// Imstance_Describe():
XCALL_( static const char * ) Imstance_Describe( void *dat ) {
	return "Imstance "DEBUG_PLUGIN_USERNAME;
}

// Imstance_NewTime():
XCALL_( static LWError ) Imstance_NewTime( void *dat, LWFrame f, LWTime t ) {
	now = t;

	return NULL;
}

// Imstance_Evaluate():
XCALL_( static double ) Imstance_Evaluate( void *dat, LWVolumeAccess *access ) {
	LWDVector	wpos, diff, cross;
	LWVolumeSample	sample;
	double		opc;

//	for( LWItemID id = itemInfo->first( LWI_OBJECT, NULL ); id != NULL; id = itemInfo->next( id ) ) {
	LWItemID id = itemInfo->first( LWI_OBJECT, NULL );
		/* Loop through the items in the scene */
		itemInfo->param( id, LWIP_W_POSITION, now, wpos );

		/* Put wpos in the space of the ray origin */
		LWVecMath::sub( access->o, wpos, diff );

		/* Cross diff with dir */
		LWVecMath::cross( access->dir, diff, cross );

		/* Find the distance from the origin to the point */
		sample.dist = LWVecMath::len( cross );
		if( sample.dist < 0 )
			sample.dist = -sample.dist;

		if( sample.dist > 0.5 )
			return 1.0;
//			continue;

		/* Set the sample data */
		sample.stride     =  1.0;

//		sample.opacity[0] = 20.0;		// Opacity is the logarithmic; 20 is basically solid.

		/* Compute variable opacity using a logarithmic function */
		opc = 1.0 - (sample.dist * 2.0);
		opc = (opc > 0.99999999) ? 20.0 : -log( 1.0 - opc );

		sample.opacity[0] = opc;
		sample.opacity[1] = opc;
		sample.opacity[2] = opc;

		sample.color[0]   =  1.0 - (sample.dist * 2.0);
		sample.color[1]   =  0.0;
		sample.color[2]   =  1.0;

		/* Add the sample */
	        access->addSample( access->ray, &sample );
//	}

	return 1.0;
}

// Imstance_Flags():
XCALL_( static unsigned int ) Imstance_Flags( void *dat ) {
   return 0;
}

// Imstance_Init():
XCALL_( LWError ) Imstance_Init( void *_inst, int mode ) {
	return NULL;
}

XCALL_( void ) Imstance_Cleanup( void *_inst ) {
	return;
}
