//
// ParticleCloud-Interface.cpp
//

#include <lightwave/lwaboutpanel_tmpro/lwaboutpanel_tmpro.h>

#include <lwcustobj.h>

#include "ParticleCloud-Interface.h"

//#include <lightwave/lwaboutpanel/lwaboutpanel.h>
//#include <lightwave/lwaboutpanel/logos/pietoolslogo.h>

// About Text
const char *about_text[] = {
  LWABOUT_YELLOW,
  "Particle Cloud",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Point to Particle System Reader for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright ©2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

// HandleAbout():
void HandleAbout( LWXPanelID pan, int cid ) {
  LWPanelFuncs *panel_funcs = (LWPanelFuncs *)ParticleCloud_Instance::global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  LWAboutPanel_TMPro( panel_funcs, "Particle Cloud", about_text, NULL, true );

//  PiEToolsLogo logo;
//  LWAboutPanel( imp_int->panel_funcs, "Particle Cloud", about_text, &logo, imp_int->panel, true, NULL /*keys_text*/ );
}

// ParticleCloud_Get():
void * ParticleCloud_Get( void *_inst, unsigned long vid ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;

  static double fvalue = 0;
  static int    ivalue = 0;

  if( inst != NULL ) {
    switch( vid ) {
      case PC_START_FRAME:
        fvalue = inst->GetStartTime();
        return &fvalue;
        break;

      case PC_BIRTH_RATE:
        ivalue = inst->GetBirthRate();
        return &ivalue;
        break;

      case PC_FROZEN:
        ivalue = inst->GetUseFrozen() ? 1 : 0;
        return &ivalue;
        break;

      case PC_INDICES:
        ivalue = inst->GetShowIndices() ? 1 : 0;
        return &ivalue;
        break;
    }
  }

  return NULL;
}

// ParticleCloud_Set():
en_LWXPRefreshCodes ParticleCloud_Set( void *_inst, unsigned long vid, void *value ) {
  ParticleCloud_Instance *inst = (ParticleCloud_Instance *)_inst;

  LWInstUpdate *instupdate = (LWInstUpdate *)inst->global( LWINSTUPDATE_GLOBAL, GFUSE_TRANSIENT );

  en_LWXPRefreshCodes retval;
  if( inst != NULL ) {
    switch ( vid ) {
      case PC_START_FRAME:
        inst->SetStartTime( *(double *)value );
        retval = LWXPRC_DFLT;
        break;

      case PC_BIRTH_RATE:
        inst->SetBirthRate( *(int *)value );
        retval = LWXPRC_DFLT;
        break;

      case PC_FROZEN:
        inst->SetUseFrozen( (*(int *)value) != 0 );
        retval = LWXPRC_DFLT;
        break;

      case PC_INDICES:
        inst->SetShowIndices( (*(int *)value) != 0 );
        retval = LWXPRC_DFLT;
        break;

      default:
        retval = LWXPRC_NONE;
        break;
    }
  }

  instupdate( LWCUSTOMOBJ_HCLASS, _inst );
  return retval;
}

