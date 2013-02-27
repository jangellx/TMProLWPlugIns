//
// IllumiSurf-IMP-Transmission.cpp
//

#include "IllumiSurf-Instance.h"
#include "IllumiSurf-IMP.h"

#include <string.h>

/*
 * Use Transparency
 */
IMPGhostModes IllumiSurf_Transparency_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseTransparency() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseTransparency_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseTransparency() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseTransparency_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseTransparency( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseTransparency = {
  "Affect Transparency",                     /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Transparency",         /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseTransparency_Query,          /* Query function          */
  IllumiSurf_UseTransparency_Evaluate,       /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Transparency
 */
void * IllumiSurf_Transparency_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetTransparency(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Transparency_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetTransparency(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Transparency_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetTransparency() );
}

IMPColumn col_IllumiSurf_Transparency = {
  "Transparency",                            /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Transparency",                /* Comment                 */
  IllumiSurf_Transparency_Envelope,          /* No envelope function    */
  IllumiSurf_Transparency_Ghost,             /* Ghosted function        */
  IllumiSurf_Transparency_Query,             /* Query function          */
  IllumiSurf_Transparency_Evaluate,          /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Refraction
 */
IMPGhostModes IllumiSurf_Refraction_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseRefraction() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseRefraction_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseRefraction() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseRefraction_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseRefraction( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseRefraction = {
  "Affect Refraction",                       /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Refraction",           /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseRefraction_Query,            /* Query function          */
  IllumiSurf_UseRefraction_Evaluate,         /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Refraction
 */
void * IllumiSurf_Refraction_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetRefraction(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Refraction_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetRefraction(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Refraction_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetRefraction() );
}

IMPColumn col_IllumiSurf_Refraction = {
  "Refraction",                              /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Refraction",                  /* Comment                 */
  IllumiSurf_Refraction_Envelope,            /* No envelope function    */
  IllumiSurf_Refraction_Ghost,               /* Ghosted function        */
  IllumiSurf_Refraction_Query,               /* Query function          */
  IllumiSurf_Refraction_Evaluate,            /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Translucency
 */
IMPGhostModes IllumiSurf_Translucency_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseTranslucency() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseTranslucency_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseTranslucency() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseTranslucency_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseTranslucency( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseTranslucency = {
  "Affect Diffuse",                          /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Diffuse",              /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseTranslucency_Query,          /* Query function          */
  IllumiSurf_UseTranslucency_Evaluate,       /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Translucency
 */
void * IllumiSurf_Translucency_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetTranslucency(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Translucency_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetTranslucency(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Translucency_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetTranslucency() );
}

IMPColumn col_IllumiSurf_Translucency = {
  "Diffuse",                                 /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Diffuse",                     /* Comment                 */
  IllumiSurf_Translucency_Envelope,          /* No envelope function    */
  IllumiSurf_Translucency_Ghost,             /* Ghosted function        */
  IllumiSurf_Translucency_Query,             /* Query function          */
  IllumiSurf_Translucency_Evaluate,          /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

IMPColumn *col_IllumiSurf_TranslucencyCols[] = {
  &col_IllumiSurf_SurfaceName,
  &col_IllumiSurf_UseTransparency,
  &col_IllumiSurf_Transparency,
  &col_IllumiSurf_UseRefraction,
  &col_IllumiSurf_Refraction,
  &col_IllumiSurf_UseTranslucency,
  &col_IllumiSurf_Translucency,
  NULL };

IMPBank bank_IllumiSurfTransmission = {
  MakeBankID( 'I', 'L', 'S', 'T' ),          /* id:  IllumiSurf Basic      */
  "IllumiSurf: Transmission",                /* Bank Title                 */
  IMPBASE_ITEM,                              /* Item base type             */
  col_IllumiSurf_TranslucencyCols,           /* Columns in bank            */
  IllumiSurf_Rows,                           /* Num Rows callbanks         */
  NULL,                                      /* No begin process function  */
  IllumiSurf_EndProcess,                     /* No end process function    */
};
