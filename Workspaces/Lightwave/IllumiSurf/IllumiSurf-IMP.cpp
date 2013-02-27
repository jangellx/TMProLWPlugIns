//
// IllumiSurf-IMP.cpp
//

#include "IllumiSurf-IMP.h"
#include "IllumiSurf-Interface.h"

#include <string.h>

int         value_int;
double      value_double[3];
LWChannelID value_channels[4];

/*
 * IllumiSurf_Rows()
 */
int IllumiSurf_Rows( long bank_id, void * id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return 0;

  const char * item_name = object_info->filename( id );

  int instance_count = 0;
  for( unsigned long i=0; i < instances.NumElements(); i++ ) {
    const char * object_name = surf_funcs->sceneObject( instances[i]->GetSurface() );
    if( strcmp( item_name, object_name ) == 0 )
      instance_count++;
  }

  return instance_count;
}

/*
 *  IllumiSurf_EndProcess()
 */
void IllumiSurf_EndProcess( long bank_id ) {
  if( is_int != NULL ) {
    if( is_int->IsOpen() )
      is_int->xpanels->viewRefresh( is_int->main_xpanel );
  }
  global( LWINTERFACEINFO_GLOBAL, GFUSE_RELEASE );
}

// IllumiSurf_JumpTo():
void IllumiSurf_JumpTo( int column, int row, void * id ) {
  IllumiSurf_OpenUI( FindInstance( id, row ) );
}

/*
 * IllumiSurf_Ghost()
 *  Ghosts the cell if the item isn't a bone.
 */
IMPGhostModes IllumiSurf_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  return ((FindInstance( id, row ) == NULL) ? IMPGHOST_BLANK : IMPGHOST_ENABLED);
}

/*
 * Surface Name Label
 */
IMPGhostModes IllumiSurf_SurfaceName_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  return ((FindInstance( id, row ) == NULL) ? IMPGHOST_BLANK : IMPGHOST_LABEL);
}

void * IllumiSurf_SurfaceName_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  return (void *)(surf_funcs->name( inst->GetSurface() ));
}

IMPColumn col_IllumiSurf_SurfaceName = {
  "Surface Name",                            /* title                   */
  COLWIDTH_STRING,                           /* default width in pixels */
  IMPCOLTYPE_STRING,                         /* column type             */
  "IllumiSurf, Surface Name Label",          /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_SurfaceName_Ghost,              /* Ghosted function        */
  IllumiSurf_SurfaceName_Query,              /* Query function          */
  NULL,                                      /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};


/*
 * Blending Mode
 */
void * IllumiSurf_BlendMode_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetBlendMode();
  return &value_int;
}

void * IllumiSurf_BlendMode_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *(int *)value;
  if( value_int < 0 )
    value_int = 0;

  if( value_int > 4)
    value_int = 4;

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetBlendMode( (IllumiSurf_BlendModes)value_int );
  }

  return &value_int;
}

int IllumiSurf_BlendMode_ListCount( int column, int row, LWItemID id ) {
  return 5;
}

const char * IllumiSurf_BlendMode_ListName( int column, int row, LWItemID id, int index ) {
  if( (index >= 0) && (index < 5) )
    return IllumiSurf_BlendModeStrings[ index ];

  return "";
}

IMPColumn col_IllumiSurf_BlendMode = {
  "Blend Mode",                              /* title                   */
  COLWIDTH_LIST,                             /* default width in pixels */
  IMPCOLTYPE_LIST,                           /* column type             */
  "IllumiSurf, Blend Mode",                  /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_BlendMode_Query,                /* Query function          */
  IllumiSurf_BlendMode_Evaluate,             /* Evaluate function       */
  NULL,                                      /* No compare function     */
  IllumiSurf_BlendMode_ListCount,            /* No list count function  */
  IllumiSurf_BlendMode_ListName,             /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Base Intensity
 */
void * IllumiSurf_BaseIntensity_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetBaseIntensity(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_BaseIntensity_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = *(double *)value;

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetBaseIntensity(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_BaseIntensity_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetBaseIntensity() );
}

IMPColumn col_IllumiSurf_BaseIntensity = {
  "Base Intensity",                          /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Base Intensity",              /* Comment                 */
  IllumiSurf_BaseIntensity_Envelope,         /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_BaseIntensity_Query,            /* Query function          */
  IllumiSurf_BaseIntensity_Evaluate,         /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Alternate Intensity
 */
void * IllumiSurf_AltIntensity_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetAltIntensity(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_AltIntensity_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = *(double *)value;

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetAltIntensity(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_AltIntensity_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetAltIntensity() );
}

IMPColumn col_IllumiSurf_AltIntensity = {
  "Alternate Intensity",                     /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Alternate Intensity",         /* Comment                 */
  IllumiSurf_AltIntensity_Envelope,          /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_AltIntensity_Query,             /* Query function          */
  IllumiSurf_AltIntensity_Evaluate,          /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 *  The Bank
 */
IMPColumn *col_IllumiSurf_Basic[] = {
  &col_IllumiSurf_SurfaceName,
  &col_IllumiSurf_BlendMode,
  &col_IllumiSurf_BaseIntensity,
  &col_IllumiSurf_AltIntensity,
  NULL };

IMPBank bank_IllumiSurfBasic = {
  MakeBankID( 'I', 'L', 'S', 'B' ),          /* id:  IllumiSurf Basic      */
  "IllumiSurf: Basic",                       /* Bank Title                 */
  IMPBASE_ITEM,                              /* Item base type             */
  col_IllumiSurf_Basic,                      /* Columns in bank            */
  IllumiSurf_Rows,                           /* Num Rows callbanks         */
  NULL,                                      /* No begin process function  */
  IllumiSurf_EndProcess,                     /* No end process function    */
};

/*
 * EditVParamEnvelope():
 */
LWChannelID * EditVParamEnvelope( IMPEnvRequests request, LWVParmID vparam ) {
  // Remove the envelope, if requested
  if( request == IMPENVREQ_REMOVE ) {
    vparam_funcs->setState( vparam, vparam_funcs->getState( vparam ) & LWVPSF_ENV );
    return IMPENV_NOT_ENVELOPED;
  }

  // See if we should add the envelope
  if( request == IMPENVREQ_APPLY ) {
    vparam_funcs->setState( vparam, vparam_funcs->getState( vparam ) | LWVPSF_ENV );
    return IMPENV_NOT_ENVELOPED;
  }

  vparam_funcs->getEnv( vparam, value_channels );
  if( value_channels[0] != NULL )
    return value_channels;

  // No envelope; return
  return IMPENV_NOT_ENVELOPED;
}

