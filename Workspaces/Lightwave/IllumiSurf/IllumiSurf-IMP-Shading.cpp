//
// IllumiSurf-IMP-Shading.cpp
//

#include "IllumiSurf-Instance.h"
#include "IllumiSurf-IMP.h"

#include <string.h>

/*
 * Use Color
 */
IMPGhostModes IllumiSurf_Color_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseColor() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseColor_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseColor() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseColor_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseColor( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseColor = {
  "Affect Color",                            /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Color",                /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseColor_Query,                 /* Query function          */
  IllumiSurf_UseColor_Evaluate,              /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Color
 */
void * IllumiSurf_Color_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetColor(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Color_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetColor(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Color_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetColor() );
}

IMPColumn col_IllumiSurf_Color = {
  "Color",                                   /* title                   */
  COLWIDTH_COLOR_ENV,                        /* default width in pixels */
  IMPCOLTYPE_COLOR,                          /* column type             */
  "IllumiSurf, Color",                       /* Comment                 */
  IllumiSurf_Color_Envelope,                 /* No envelope function    */
  IllumiSurf_Color_Ghost,                    /* Ghosted function        */
  IllumiSurf_Color_Query,                    /* Query function          */
  IllumiSurf_Color_Evaluate,                 /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Luminosity
 */
IMPGhostModes IllumiSurf_Luminosity_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseLuminosity() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseLuminosity_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseLuminosity() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseLuminosity_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseLuminosity( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseLuminosity = {
  "Affect Luminosity",                       /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Luminosity",           /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseLuminosity_Query,            /* Query function          */
  IllumiSurf_UseLuminosity_Evaluate,         /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Luminosity
 */
void * IllumiSurf_Luminosity_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetLuminosity(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Luminosity_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetLuminosity(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Luminosity_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetLuminosity() );
}

IMPColumn col_IllumiSurf_Luminosity = {
  "Luminosity",                              /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Luminosity",                  /* Comment                 */
  IllumiSurf_Luminosity_Envelope,            /* No envelope function    */
  IllumiSurf_Luminosity_Ghost,               /* Ghosted function        */
  IllumiSurf_Luminosity_Query,               /* Query function          */
  IllumiSurf_Luminosity_Evaluate,            /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Use Diffusion
 */
IMPGhostModes IllumiSurf_Diffusion_Ghost( int column, int row, LWItemID id ) {
  if( item_info->type( id ) != LWI_OBJECT )
    return IMPGHOST_BLANK;

  IllumiSurf_Instance *inst = FindInstance( id, row );
  if( inst == NULL )
    return IMPGHOST_BLANK;

  return (inst->GetUseDiffusion() ? IMPGHOST_ENABLED : IMPGHOST_DISABLED);
}

void * IllumiSurf_UseDiffusion_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  value_int = inst->GetUseDiffusion() ? 1 : 0;
  return &value_int;
}

void * IllumiSurf_UseDiffusion_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_int = *((int *)value);

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    inst->SetUseDiffusion( value_int != 0 );
  }

  return &value_int;
}

IMPColumn col_IllumiSurf_UseDiffusion = {
  "Affect Diffuse",                          /* title                   */
  COLWIDTH_TOGGLE,                           /* default width in pixels */
  IMPCOLTYPE_TOGGLE,                         /* column type             */
  "IllumiSurf, Affect Diffuse",              /* Comment                 */
  NULL,                                      /* No envelope function    */
  IllumiSurf_Ghost,                          /* Ghosted function        */
  IllumiSurf_UseDiffusion_Query,             /* Query function          */
  IllumiSurf_UseDiffusion_Evaluate,          /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

/*
 * Diffusion
 */
void * IllumiSurf_Diffusion_Query( int column, int row, LWItemID id, LWTime time ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  assert( inst != NULL );

  vparam_funcs->getVal( inst->GetDiffusion(), time, NULL, value_double );
  return &value_double;
}

void * IllumiSurf_Diffusion_Evaluate( int column, int row, LWItemID id, LWTime time, void *value, int apply ) {
  value_double[0] = ((double *)value)[0];
  value_double[1] = ((double *)value)[1];
  value_double[2] = ((double *)value)[2];

  if( apply ) {
    IllumiSurf_Instance *inst = FindInstance( id, row );
    assert( inst != NULL );

    vparam_funcs->setVal( inst->GetDiffusion(), value_double );
  }

  return &value_double;
}

LWChannelID *IllumiSurf_Diffusion_Envelope( int column, int row, void * id, IMPEnvRequests request ) {
  IllumiSurf_Instance *inst = FindInstance( id, row );
  return EditVParamEnvelope( request, inst->GetDiffusion() );
}

IMPColumn col_IllumiSurf_Diffusion = {
  "Diffuse",                                 /* title                   */
  COLWIDTH_NUMERIC_ENV,                      /* default width in pixels */
  IMPCOLTYPE_PERCENT,                        /* column type             */
  "IllumiSurf, Diffuse",                     /* Comment                 */
  IllumiSurf_Diffusion_Envelope,             /* No envelope function    */
  IllumiSurf_Diffusion_Ghost,                /* Ghosted function        */
  IllumiSurf_Diffusion_Query,                /* Query function          */
  IllumiSurf_Diffusion_Evaluate,             /* Evaluate function       */
  NULL,                                      /* No compare function     */
  NULL,                                      /* No list count function  */
  NULL,                                      /* No list name function   */
  NULL,                                      /* No test item function   */
  NULL,
  IllumiSurf_JumpTo,                         /* No jump to Function     */
  NULL,                                      /* No custom xpanel        */
};

IMPColumn *col_IllumiSurf_Shading[] = {
  &col_IllumiSurf_SurfaceName,
  &col_IllumiSurf_UseColor,
  &col_IllumiSurf_Color,
  &col_IllumiSurf_UseLuminosity,
  &col_IllumiSurf_Luminosity,
  &col_IllumiSurf_UseDiffusion,
  &col_IllumiSurf_Diffusion,
  NULL };

IMPBank bank_IllumiSurfShading = {
  MakeBankID( 'I', 'L', 'S', 'S' ),          /* id:  IllumiSurf Basic      */
  "IllumiSurf: Shading",                     /* Bank Title                 */
  IMPBASE_ITEM,                              /* Item base type             */
  col_IllumiSurf_Shading,                    /* Columns in bank            */
  IllumiSurf_Rows,                           /* Num Rows callbanks         */
  NULL,                                      /* No begin process function  */
  IllumiSurf_EndProcess,                     /* No end process function    */
};
