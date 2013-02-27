// GRADIENT.H
// Gradient services for layout
// needs Lwpannel services to run
// API version 2 - 12/03/98
// Gregory Duquesne, Newtek
 
#include <stdio.h>
#ifndef PANEL_SERVICES_NAME
  #include <lwpanel.h>
#endif
 
#define GRADIENT_SERVICES_NAME  "GradientServices"
#define GRADIENT_ACCESS_NAME    "GradientAccess"
#define GRADIENT_API_VERSION     2
 
#ifndef LW_RAN_H
  #include "lwran.h"
#endif
 
typedef struct  st_gKey {
        void    *prev, *next;
        double   v[3], t;
        short    type;
}       gKey;

#define LWG_MAX_PARAMS  10
 
typedef struct  st_gParamDesc{
        char     *name;          // parameter name for POPUP
        double    start,end;     // start amd end values for this parameter
}       gParamDesc;
#define         GKT_SCALAR              1
#define         GKT_COLOR               3
 
typedef struct  st_gFilter{
        int             type;
        int             gain,bias;
        double  invGain,invBias;
}       gFilter;

#define GF_LINEAR       0
#define GF_HICLIP       1
#define GF_LOCLIP       2
#define GF_SINE1        3
#define GF_SINE2        4
#define GF_SINE3        5
#define GF_STEPS        6
 
typedef struct  st_LWGradient{
        gKey                   *keyList, *firstKey, *lastKey, *selectKey;
        gFilter                 filter;
        double                  min, max, pStart,pEnd;
        int                     flags, type, paramNb;     // flags, gradient type, selected parameter number (used for evaluation)
 
        // reference object info
        char                    objName[80];
        LWItemID                objID;
        double                  objPos[3];      // a place to save the object position information (speedup pruposes..)
 
        // UI info
        gParamDesc              pDesc[LWG_MAX_PARAMS];
        LWPanelFuncs           *pfunc;
        const char             *title;
}       LWGradient;
 
#define         LWG_MAX_KEYS                    256
#define         LWGRADIENT_SCALAR               1
#define         LWGRADIENT_COLOR                2
 
#define         LWGF_FIXEDMIN                   (1<<0)
#define         LWGF_FIXEDMAX                   (2<<0)
 
typedef struct st_gradInterfaceData {
        const char   *title;
        gParamDesc    pDesc[LWG_MAX_PARAMS];
} gradInterfaceData;
 
 
typedef struct  st_LWGradientHandler{
        LWGradient  *(*create) (short flags, short type, gParamDesc *pDesc);
        int          (*destroy) (LWGradient  *grad);
        int          (*copy) (LWGradient     *from,LWGradient *to);
        int          (*load)(LWGradient      *grad,FILE *fp);
        int          (*save)(LWGradient      *grad,FILE *fp);
        int          (*load_lw)(LWGradient   *grad,const LWLoadState *sState);       // load during load scene
        int          (*save_lw)(LWGradient   *grad,const LWSaveState *sState);       // save during save scene
        int          (*Interface)(LWGradient *grad,char *title);
        void         (*evaluate) (LWGradient *grad,double paramValue,double *returnValue);
        void         (*freeHandler)();
        short        apiVersion;
}       LWGradientHandler;
