//
// IllumiSurf-IMP-Reflection.cpp
//

#include "IllumiSurf-Instance.h"
#include "IllumiSurf-IMP.h"

#include <string.h>

/*
 * Use Specular
 */
IMPGhostModes IllumiSurf_Specular_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseSpecular() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseSpecular_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseSpecular() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseSpecular_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseSpecular( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseSpecular = {
  "Affect Specular",                         /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Specular",             /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseSpecular_Query,              /* Query function          */
  IllumiSurf_UseSpecular_Evaluate,           /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Specular
 */
void * IllumiSurf_Specular_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetSpecular(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Specular_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetSpecular(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Specular_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetSpecular() );
}

IMPColumn col_IllumiSurf_Specular = {
  "Specular",                                /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Specular",                    /* Comment                 */
  IllumiSurf_Specular_Envelope,              /* No envelope function    */
  IllumiSurf_Specular_Ghost,                 /* Ghosted function        */
  IllumiSurf_Specular_Query,                 /* Query function          */
  IllumiSurf_Specular_Evaluate,              /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Glossiness
 */
IMPGhostModes IllumiSurf_Glossiness_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseGlossiness() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseGlossiness_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseGlossiness() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseGlossiness_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseGlossiness( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseGlossiness = {
  "Affect Glossiness",                       /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Glossiness",           /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseGlossiness_Query,            /* Query function          */
  IllumiSurf_UseGlossiness_Evaluate,         /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,                                      /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Glossiness
 */
void * IllumiSurf_Glossiness_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetGlossiness(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Glossiness_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetGlossiness(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Glossiness_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetGlossiness() );
}

IMPColumn col_IllumiSurf_Glossiness = {
  "Glossiness",                              /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Glossiness",                  /* Comment                 */
  IllumiSurf_Glossiness_Envelope,            /* No envelope function    */
  IllumiSurf_Glossiness_Ghost,               /* Ghosted function        */
  IllumiSurf_Glossiness_Query,               /* Query function          */
  IllumiSurf_Glossiness_Evaluate,            /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Reflection
 */
IMPGhostModes IllumiSurf_Reflection_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseReflection() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseReflection_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseReflection() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseReflection_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseReflection( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseReflection = {
  "AFfect Reflection",                       /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Reflection",           /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseReflection_Query,            /* Query function          */
  IllumiSurf_UseReflection_Evaluate,         /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Reflection
 */
void * IllumiSurf_Reflection_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetReflection(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Reflection_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetReflection(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Reflection_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetReflection() );
}

IMPColumn col_IllumiSurf_Reflection = {
  "Reflection",                              /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Reflection",                  /* Comment                 */
  IllumiSurf_Reflection_Envelope,            /* No envelope function    */
  IllumiSurf_Reflection_Ghost,               /* Ghosted function        */
  IllumiSurf_Reflection_Query,               /* Query function          */
  IllumiSurf_Reflection_Evaluate,            /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

IMPColumn *col_IllumiSurf_ReflectionCols[] = {
  &col_IllumiSurf_SurfaceName,
  &col_IllumiSurf_UseSpecular,
  &col_IllumiSurf_Specular,
  &col_IllumiSurf_UseGlossiness,
  &col_IllumiSurf_Glossiness,
  &col_IllumiSurf_UseReflection,
  &col_IllumiSurf_Reflection,
  NULL };

IMPBank bank_IllumiSurfReflection = {
  MakeBankID( 'I', 'L', 'S', 'R' ),          /* id:  IllumiSurf Basic      */
  "IllumiSurf: Reflection",                  /* Bank Title                 */
  IMPBASE_ITEM,                              /* Item base type             */
  col_IllumiSurf_ReflectionCols,             /* Columns in bank            */
  IllumiSurf_Rows,                           /* Num Rows callbanks         */
  NULL,                                      /* No begin process function  */
  IllumiSurf_EndProcess,                     /* No end process function    */
};
