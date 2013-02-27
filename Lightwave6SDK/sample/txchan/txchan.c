/* TxtrChan.c -- A collection of Plugins using textures in different contexts.
 *
 *   Add a Texture to a channel, motion, image or environment using the texture global.
 *  These are just 4 different implementations of the same plugin.
 *   Instance functions are the same, but the evaluation and flags functions have different
 *   implementations for each plugin.
 *
 *  Gregory Duquesne \ Arnie Cachelin.
 */

#include <lwhost.h>
#include <lwserver.h>
#include <lwhandler.h>
#include <lwenvel.h>
#include <lwchannel.h>
#include <lwmotion.h>
#include <lwrender.h>
#include <lwfilter.h>
#include <lwenviron.h>
#include <lwtxtr.h>
#include <lwtxtred.h>
#include <lwxpanel.h>

#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static   LWXPanelFuncs     *GlobalXPanFun;
static   LWMessageFuncs    *Gmessage;
static   LWTextureFuncs    *GlobalTextureFuncs;
static   LWTxtrEdFuncs     *GlobalTxtrEdFuncs;
static   LWInstUpdate      *GlobalLWUpdate = NULL;
static  GlobalFunc         *GGlobal=NULL;

// subscription IDs for texture editors (one for each plugin class)
static   LWTECltID            chanTxtredID = NULL;
static   LWTECltID            motTxtredID = NULL;
static   LWTECltID            fltTxtredID = NULL;
static   LWTECltID            envTxtredID = NULL;
// texture context for environment plugin class
static   LWTxtrContextID         envCtxtID = NULL;

/* We use the same data structure for the various plugins */
typedef struct st_TxtrData {
   void           *ctxt;
   void           *self;
   double             offset, scale;
   int                axis,type;
   char            desc[100];
   LWTextureID       txtr;
   char           *name;
} TxtrData;

#define  CHANNEL           0
#define  MOTION            1
#define  FILTER            2
#define  ENVIRON           3

// ----------------------------------------------------------- //

static int xax[] = {1,2,0}, yax[]={2,0,1}, zax[] = {0,1,2};

static char *errAllocFailed ="Tiny Allocation Failed, I don't feel so good";
static int  chanActivateCount = 0;
static int  motActivateCount = 0;
static int  fltActivateCount = 0;
static int  envActivateCount = 0;

static LWFrame    gFrame = 0;
static LWTime     gTime = 0;

// ----------------------------------------------------------- //


void  initMP(LWMicropol *mp)
{
   memset(mp,0,sizeof(LWMicropol));
   mp->oXfrm[0] = mp->oXfrm[3] = mp->oXfrm[6] = 1;
   mp->wXfrm[0] = mp->wXfrm[3] = mp->wXfrm[6] = 1;
   mp->oScl[0] = mp->oScl[1] = mp->oScl[2] = 1;
}

TxtrEventFunc(LWTextureID  txtr, void  *userData,int  eventCode)
{
   TxtrData *data;

   if (eventCode == TXEV_ALTER && GlobalLWUpdate)
   {
      // Before evaluating the texture we need to initiase it
      // This is not necessary at render time, since
      (*GlobalTextureFuncs->newtime)(txtr,gTime,gFrame);

      // update layout for the plugin class
      data= (*GlobalTextureFuncs->userData)(txtr);
      if (data->type == CHANNEL)
         (*GlobalLWUpdate)(LWCHANNEL_HCLASS, data);
      else if (data->type == MOTION)
         (*GlobalLWUpdate)(LWITEMMOTION_HCLASS, data);
      else if (data->type == FILTER)
         (*GlobalLWUpdate)(LWIMAGEFILTER_HCLASS, data);
      else if (data->type == ENVIRON)
         (*GlobalLWUpdate)(LWENVIRONMENT_HCLASS, data);

      // cleanup texture after evaluation
      (*GlobalTextureFuncs->cleanup)(txtr);
   }

   return   1;
}

XCALL_(void)   TxtrNewtime (TxtrData *inst,LWFrame f,LWTime t)
{
   XCALL_INIT;

   gTime = t;
   gFrame = f;
   return;
}


/* ----------------- Plug-in Methods: LWInstanceFuncs  -----------------  */


XCALL_(static LWInstance)TxtrChanCreate(void *data, LWChannelID chan, LWError *err)
{
   TxtrData *dat=NULL;
   XCALL_INIT;

   if(dat=malloc(sizeof(TxtrData)))
   {
      memset(dat,0,sizeof(*dat));
      dat->offset = 0.0;
      dat->scale = 1.0;
      dat->self = chan;
      dat->ctxt = data;
      dat->name = NULL;
      dat->type = CHANNEL;
      // create texture (of scalar type)
      dat->txtr = (*GlobalTextureFuncs->create)(TRT_SCALAR,"Texture Channel",NULL,dat);

      sprintf(dat->desc," Scale: %.2f", dat->scale);
   }
   else
      *err = errAllocFailed;

   chanActivateCount ++;
   return dat;
}

XCALL_(static LWInstance)TxtrMotionCreate(void *data, LWItemID item, LWError *err)
{
   TxtrData *dat=NULL;
   XCALL_INIT;

   if(dat=malloc(sizeof(TxtrData)))
   {
      memset(dat,0,sizeof(*dat));
      dat->offset = 0.0;
      dat->scale = 1.0;
      dat->self = (void *)item;
      dat->ctxt = data;
      dat->name = NULL;
      dat->type = MOTION;
      dat->axis = 1;
      // create texture (of scalar type)
      dat->txtr = (*GlobalTextureFuncs->create)(TRT_SCALAR,"Texture Motion",NULL,dat);

      sprintf(dat->desc," Scale: %.2f", dat->scale);
   }
   else
      *err = errAllocFailed;

   motActivateCount ++;
   return dat;
}

XCALL_(static LWInstance)TxtrFilterCreate(void *data, void* ctxt, LWError *err)
{
   TxtrData *dat=NULL;
   XCALL_INIT;


   if(dat=malloc(sizeof(TxtrData)))
   {
      memset(dat,0,sizeof(*dat));
      dat->offset = 0.0;
      dat->scale = 1.0;
      dat->self = ctxt;
      dat->ctxt = data;
      dat->name = NULL;
      dat->type = FILTER;
      dat->axis = 2;
      // create texture (of color type)
      dat->txtr = (*GlobalTextureFuncs->create)(TRT_COLOR,"Texture Filter",NULL,dat);

      sprintf(dat->desc," Scale: %.2f", dat->scale);
   }
   else
      *err = errAllocFailed;

   fltActivateCount ++;
   return dat;
}

LWTxtrContextID      addEnvironInputParams(void);

XCALL_(static LWInstance)TxtrEnvironCreate(void *data, void* ctxt, LWError *err)
{
   TxtrData *dat=NULL;
   XCALL_INIT;

   if(dat=malloc(sizeof(TxtrData)))
   {
      memset(dat,0,sizeof(*dat));
      dat->offset = 0.0;
      dat->scale = 1.0;
      dat->self = ctxt;
      dat->ctxt = data;
      dat->name = NULL;
      dat->type = ENVIRON;
      dat->axis = 2;

      // create a texture context for additional gradient input parameters
      if (!envCtxtID)
         envCtxtID = addEnvironInputParams();
      // create texture (of color type) using the texture context
      dat->txtr = (*GlobalTextureFuncs->create)(TRT_COLOR,"Texture Environment",envCtxtID,dat);

      sprintf(dat->desc," Scale: %.2f", dat->scale);
   }
   else
      *err = errAllocFailed;

   envActivateCount ++;
   return dat;
}



XCALL_(static void)TxtrChanDestroy(TxtrData *dat)
{
   XCALL_INIT;
   if(dat)
   {
      chanActivateCount --;
      if (!chanActivateCount && GlobalTxtrEdFuncs && chanTxtredID)
      {
         (*GlobalTxtrEdFuncs->unsubscribe)(chanTxtredID);
         chanTxtredID = NULL;
      }

      (*GlobalTextureFuncs->destroy)(dat->txtr);

      free(dat);
   }
}

XCALL_(static void)TxtrMotionDestroy(TxtrData *dat)
{
   XCALL_INIT;
   if(dat)
   {
      motActivateCount --;
      if (!motActivateCount && GlobalTxtrEdFuncs && motTxtredID)
      {
         (*GlobalTxtrEdFuncs->unsubscribe)(motTxtredID);
         motTxtredID = NULL;
      }

      (*GlobalTextureFuncs->destroy)(dat->txtr);

      free(dat);
   }
}

XCALL_(static void)TxtrFilterDestroy(TxtrData *dat)
{
   XCALL_INIT;
   if(dat)
   {
      fltActivateCount --;
      if (!fltActivateCount && GlobalTxtrEdFuncs && fltTxtredID)
      {
         (*GlobalTxtrEdFuncs->unsubscribe)(fltTxtredID);
         fltTxtredID = NULL;
      }

      (*GlobalTextureFuncs->destroy)(dat->txtr);

      free(dat);
   }
}

XCALL_(static void)TxtrEnvironDestroy(TxtrData *dat)
{
   XCALL_INIT;
   if(dat)
   {
      envActivateCount --;
      if (!envActivateCount && GlobalTxtrEdFuncs && envTxtredID)
      {
         (*GlobalTxtrEdFuncs->unsubscribe)(envTxtredID);
         envTxtredID = NULL;
      }

      if (!envActivateCount && GlobalTextureFuncs && envCtxtID)
      {
         (*GlobalTextureFuncs->contextDestroy)(envCtxtID);
         envCtxtID = NULL;
      }

      (*GlobalTextureFuncs->destroy)(dat->txtr);

      free(dat);
   }
}


XCALL_(static LWError)TxtrChanCopy(TxtrData  *to, TxtrData  *from)
{
   LWTextureID    txtr;
   XCALL_INIT;

   (*GlobalTextureFuncs->copy)(to->txtr,from->txtr);

   txtr = to->txtr;
   *to = *from;
   to->txtr = txtr;

   return (NULL);
}

XCALL_(static LWError)TxtrChanLoad(TxtrData *inst,const LWLoadState  *lState)
{
   float fp;
   int      ax;
   XCALL_INIT;

   LWLOAD_FP(lState,&fp,1);
   inst->scale = fp;
   LWLOAD_FP(lState,&fp,1);
   inst->offset = fp;
   LWLOAD_I4(lState,&ax,1);
   inst->axis = ax;

   (*GlobalTextureFuncs->load)(inst->txtr,lState);
   return (NULL);
}

XCALL_(LWError)TxtrChanSave(TxtrData *inst,const LWSaveState   *sState)
{
   float fp;
   XCALL_INIT;

   fp = (float)inst->scale;
   LWSAVE_FP(sState,&fp,1);
   fp = (float)inst->offset;
   LWSAVE_FP(sState,&fp,1);
   LWSAVE_I4(sState,&inst->axis,1);

   (*GlobalTextureFuncs->save)(inst->txtr,sState);
   return (NULL);
}

XCALL_(static const char *)TxtrChanDescribe (LWInstance inst)
{
   TxtrData *dat = (TxtrData *)inst;
   XCALL_INIT;

   sprintf(dat->desc," Scale: %.2f");

   return (dat->desc);
}


/* ----------------- Plug-in Methods: LWMotionHandler  -----------------  */

XCALL_(static unsigned int)TxtrMotionFlags (TxtrData *inst)
{
   XCALL_INIT;

   return 0;
}

XCALL_(static void) TxtrMotionEval (TxtrData *dat,const LWItemMotionAccess *motAcc)
{
   double      val = 0.0,pos[3];
   LWMicropol  mp;
   int         i;

   XCALL_INIT;

   /* fill-in the micropolygon structure
   */
   initMP(&mp);
   gTime = motAcc->time;

   (*motAcc->getParam)(LWIP_W_POSITION,motAcc->time ,pos);
   for (i=0;i<3;i++)
      mp.wPos[i] = pos[i];

   (*motAcc->getParam)(LWIP_POSITION,motAcc->time ,pos);
   for (i=0;i<3;i++)
      mp.oPos[i] = pos[i];

   mp.gNorm[dat->axis] = mp.wNorm[dat->axis] = 1;
   mp.oAxis = mp.wAxis = dat->axis;
   // there is no way we can determine what the spot size is in this context
   // so we just set it to a small value that won't affect image antialiasing.
   mp.spotSize = 0.001;
   mp.txVal = pos[dat->axis];

   // evaluate the texture
   (*GlobalTextureFuncs->evaluate)(dat->txtr,&mp,&val);
   // set item's position with new value
   pos[dat->axis] += dat->scale*val + dat->offset;
   (*motAcc->setParam)(LWIP_POSITION, pos);
}


/* ----------------- Plug-in Methods: LWChannelHandler  -----------------  */

XCALL_(static unsigned int)TxtrChanFlags (TxtrData *inst)
{
   XCALL_INIT;

   return 0;
}

XCALL_(static void)TxtrChanEval (TxtrData *dat, const LWChannelAccess *chanAcc)
{
   double      val = 0.0, value;
   LWMicropol  mp;

   XCALL_INIT;

   gFrame = chanAcc->frame;
   gTime = chanAcc->time;

   value = chanAcc->value;
   /* fill-in the micropolygon structure
   */
   initMP(&mp);
   mp.wPos[dat->axis] = mp.oPos[dat->axis] = gTime;
   mp.gNorm[dat->axis] = mp.wNorm[dat->axis] = 1;
   mp.oAxis = mp.wAxis = dat->axis;
   mp.spotSize = 0.001;
   mp.txVal = gTime;

   (*GlobalTextureFuncs->evaluate)(dat->txtr,&mp,&val);

   val *= dat->scale;
   val += dat->offset;
   val += chanAcc->value;
   (*chanAcc->setChannel)(chanAcc->chan, val);
}

/* ----------------- Plug-in Methods: LWFilterlHandler  -----------------  */

XCALL_(static unsigned int)TxtrFiltFlags (TxtrData *inst)
{
   XCALL_INIT;

   return 0;
}

XCALL_(static void)TxtrFiltProcess (TxtrData *dat, const LWFilterAccess *filtAcc)
{
   double      val = 0.0, RGB[3],trans;
   LWMicropol  mp;
   int         x,y;
   float    *r,*g,*b, *a, rgb[3], sx,sy;
   XCALL_INIT;

   gFrame = filtAcc->frame;

   for(y=0; y<filtAcc->height; y++)
   {
      r = (*filtAcc->getLine)(LWBUF_RED, y);
      g = (*filtAcc->getLine)(LWBUF_GREEN, y);
      b = (*filtAcc->getLine)(LWBUF_BLUE, y);
      a = (*filtAcc->getLine)(LWBUF_ALPHA, y);
      for(x=0; x<filtAcc->width; x++)
      {
         rgb[0] = r[x];
         rgb[1] = g[x];
         rgb[2] = b[x];
         (*filtAcc->setRGB)(x,y,rgb);
         (*filtAcc->setAlpha)(x,y,a[x]);
      }
   }

   initMP(&mp);

   mp.wPos[dat->axis] = mp.oPos[dat->axis] = dat->offset;
   if (dat->scale)
   {
      sx = (float)(1.0/filtAcc->width/dat->scale);
      sy = (float)(1.0/filtAcc->height/dat->scale);
   }
   else
   {
      sx = (float)(1.0/filtAcc->width);
      sy = (float)(1.0/filtAcc->height);
   }

   for(y=0; y<filtAcc->height; y++)
   {
      r = (*filtAcc->getLine)(LWBUF_RED, y);
      g = (*filtAcc->getLine)(LWBUF_GREEN, y);
      b = (*filtAcc->getLine)(LWBUF_BLUE, y);
      for(x=0; x<filtAcc->width; x++)
      {
         mp.wPos[xax[dat->axis]] = mp.oPos[xax[dat->axis]] = (float)x*sx - .5f;
         mp.wPos[yax[dat->axis]] = mp.oPos[yax[dat->axis]] = (float)-y*sy + .5f;
         mp.gNorm[dat->axis] = mp.wNorm[dat->axis] = 1;
         mp.oAxis = mp.wAxis = dat->axis;
         mp.spotSize = 1/filtAcc->width;
         mp.txVal = 0.33333*(r[x]+g[x]+b[x]);

         RGB[0] = r[x]; RGB[1] = g[x]; RGB[2] = b[x];
         // the original color is passed to the evaluation function
         // this color will be modified with the texture color affected
         // by its alpha channel.
         trans = (*GlobalTextureFuncs->evaluate)(dat->txtr,&mp,RGB);
         rgb[0] = (float)(RGB[0]);
         rgb[1] = (float)(RGB[1]);
         rgb[2] = (float)(RGB[2]);
         (*filtAcc->setRGB)(x,y,rgb);
      }
   }
}


/* ----------------- Plug-in Methods: LWEnvironment  -----------------  */

XCALL_(static unsigned int)TxtrEnvironFlags (TxtrData *inst)
{
   XCALL_INIT;

   return 0;
}

XCALL_(static LWError)TxtrEnvironProcess (TxtrData *dat, LWEnvironmentAccess *envAcc)
{
   double      val = 0.0, trans,scale;
   LWMicropol  mp;
   int         i;

   XCALL_INIT;

   if (dat->scale)
      scale = 1/dat->scale;
   else
      scale = 1;

   initMP(&mp);
   for (i=0;i<3;i++)
   {
      mp.wPos[i] = envAcc->dir[i]*scale;
      mp.oPos[i] = envAcc->dir[i]*scale;
      mp.gNorm[i] = mp.wNorm[i] = envAcc->dir[i];
   }
   mp.wPos[dat->axis] += dat->offset;
   mp.oPos[dat->axis] += dat->offset;
   mp.oAxis = mp.wAxis = dat->axis;

   envAcc->color[0] = 0;
   envAcc->color[1] = 0;
   envAcc->color[2] = 0;
   trans = (*GlobalTextureFuncs->evaluate)(dat->txtr,&mp,envAcc->color);

   return   NULL;
}

// Environment gradient input parameter funcs:
// 1/ set an init function
static gParamData envInitParam(void *userData,LWTxtrParamDesc  *param,int  paramNb,LWTime t,LWFrame   f)
{
   return   NULL;
}
// 2/ set a cleanup function
static void envCleanupParam(LWTxtrParamDesc  *param,int  paramNb,gParamData   data)
{
   return;
}
// 3/ evaluate parameter by switching on input parameter.
// Note that parameter 0 is "previous layer", this parameter always exists.
static double  envEvalParam(LWTxtrParamDesc  *param,int  paramNb,LWMicropol   *mp,gParamData    data)
{
   double      t=0,vec[3],norm;

   vec[0] = mp->wPos[0];
   vec[1] = mp->wPos[1];
   vec[2] = mp->wPos[2];
   norm = sqrt(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
   if (norm)
   {
      vec[0] /= norm;
      vec[1] /= norm;
      vec[2] /= norm;
   }

   switch (paramNb)
   {
   case  1: // heading
      return   atan2(vec[0],vec[2]);
   case  2: // pitch
      return   -asin(vec[1]);
   }

   return   t;
}

// This is where we create the texture context. It is made of the funcs we created
// below plus some input parameter descriptions.
LWTxtrContextID      addEnvironInputParams(void)
{
   LWTxtrParamFuncs     envCtxtFuncs;
   LWTxtrContextID         ctxt;

   static LWTxtrParamDesc  envDesc[] ={
      {"Heading",-3.1415,3.1415,LWIPT_ANGLE,LWGF_FIXED_START | LWGF_FIXED_END,LWGI_NONE},
      {"Pitch",-1.5707,1.5707,LWIPT_ANGLE,LWGF_FIXED_START | LWGF_FIXED_END,LWGI_NONE},
      {NULL,0,10,0,0,0,NULL}
   };

   // set gradient funcs
   envCtxtFuncs.paramEvaluate = envEvalParam;
   envCtxtFuncs.paramTime = envInitParam;
   envCtxtFuncs.paramCleanup = envCleanupParam;
   // create texture context
   ctxt = (*GlobalTextureFuncs->contextCreate)(envCtxtFuncs);
   // add gradient input params
   (*GlobalTextureFuncs->contextAddParam)(ctxt,envDesc[0]);
   (*GlobalTextureFuncs->contextAddParam)(ctxt,envDesc[1]);

   return   ctxt;
}

/* ----------------- Plug-in Activation  -----------------  */

XCALL_(static int) TxtrChannel (
   long            version,
   GlobalFunc        *global,
   LWChannelHandler  *local,
   void           *serverData)
{
   XCALL_INIT;
   if (version != LWCHANNEL_VERSION)
      return (AFUNC_BADVERSION);

   Gmessage = (*global) ("Info Messages", GFUSE_TRANSIENT);
   if (!Gmessage )
      return AFUNC_BADGLOBAL;

   GlobalTextureFuncs = (*global) (LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalTextureFuncs )
      return AFUNC_BADGLOBAL;

   GGlobal = global;

   local->inst->create  = TxtrChanCreate;
   local->inst->destroy = TxtrChanDestroy;
   local->inst->load    = TxtrChanLoad;
   local->inst->save    = TxtrChanSave;
   local->inst->copy    = TxtrChanCopy;

   local->evaluate   = TxtrChanEval;
   local->flags      = TxtrChanFlags;
   local->inst->descln     = TxtrChanDescribe;
   return (AFUNC_OK);
}

XCALL_(static int) TxtrMotion (
   long            version,
   GlobalFunc        *global,
   LWItemMotionHandler  *local,
   void           *serverData)
{
   XCALL_INIT;
   if (version != LWCHANNEL_VERSION)
      return (AFUNC_BADVERSION);

   Gmessage = (*global) ("Info Messages", GFUSE_TRANSIENT);
   if (!Gmessage )
      return AFUNC_BADGLOBAL;

   GlobalTextureFuncs = (*global) (LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalTextureFuncs )
      return AFUNC_BADGLOBAL;

   GGlobal = global;

   local->inst->create  = TxtrMotionCreate;
   local->inst->destroy = TxtrMotionDestroy;
   local->inst->load    = TxtrChanLoad;
   local->inst->save    = TxtrChanSave;
   local->inst->copy    = TxtrChanCopy;

   local->evaluate   = TxtrMotionEval;
   local->flags      = TxtrMotionFlags;
   local->inst->descln     = TxtrChanDescribe;
   return (AFUNC_OK);
}


XCALL_(static int) TxtrFilter (
   long               version,
   GlobalFunc           *global,
   LWImageFilterHandler *local,
   void              *serverData)
{
   XCALL_INIT;
   if (version != LWCHANNEL_VERSION)
      return (AFUNC_BADVERSION);

   Gmessage = (*global) ("Info Messages", GFUSE_TRANSIENT);
   if (!Gmessage )
      return AFUNC_BADGLOBAL;

   GlobalTextureFuncs = (*global) (LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalTextureFuncs )
      return AFUNC_BADGLOBAL;

   GGlobal = global;

   local->inst->create  = TxtrFilterCreate;
   local->inst->destroy = TxtrFilterDestroy;
   local->inst->load    = TxtrChanLoad;
   local->inst->save    = TxtrChanSave;
   local->inst->copy    = TxtrChanCopy;

   local->process    = TxtrFiltProcess;
   local->flags      = TxtrFiltFlags;
   local->inst->descln     = TxtrChanDescribe;
   return (AFUNC_OK);
}

XCALL_(static int) TxtrEnviron (
   long               version,
   GlobalFunc           *global,
   LWEnvironmentHandler *local,
   void              *serverData)
{
   XCALL_INIT;
   if (version != LWCHANNEL_VERSION)
      return (AFUNC_BADVERSION);

   Gmessage = (*global) ("Info Messages", GFUSE_TRANSIENT);
   if (!Gmessage )
      return AFUNC_BADGLOBAL;

   GlobalTextureFuncs = (*global) (LWTEXTUREFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalTextureFuncs )
      return AFUNC_BADGLOBAL;

   GGlobal = global;

   local->inst->create  = TxtrEnvironCreate;
   local->inst->destroy = TxtrEnvironDestroy;
   local->inst->load    = TxtrChanLoad;
   local->inst->save    = TxtrChanSave;
   local->inst->copy    = TxtrChanCopy;

   local->evaluate   = TxtrEnvironProcess;
   local->flags      = TxtrEnvironFlags;
   local->inst->descln     = TxtrChanDescribe;
   return (AFUNC_OK);
}

/* -----------------  User Interface  ----------------- */

#define STR_Offset_TEXT    "Offset"
#define STR_Scale_TEXT     "Scale"
#define STR_Axis_TEXT      "Axis"
#define STR_Txtr_TEXT      "Texture"

enum  {  CH_OFFSET = 0x8001, CH_SCALE, CH_AXIS, CH_TXTR};
static   char  *axisList[] = {"X","Y","Z",0};

static LWXPanelControl ctrl_list[] = {
   { CH_OFFSET,       STR_Offset_TEXT,          "distance" },
   { CH_SCALE,        STR_Scale_TEXT,          "distance" },
   { CH_AXIS,        STR_Axis_TEXT,          "iChoice" },
   {0}
};
static LWXPanelDataDesc data_descrip[] = {
   { CH_OFFSET,        STR_Offset_TEXT,          "distance" },
   { CH_SCALE,        STR_Scale_TEXT,          "distance" },
   { CH_AXIS,        STR_Axis_TEXT,          "integer" },
   {0},
};


void *NoiseData_get ( void *myinst, unsigned long vid )
{
  TxtrData *dat = (TxtrData*)myinst;
  void *result = NULL;
  static double val = 0.0;
  static int   ival = 0;

  if ( dat )
     switch ( vid ) {
      case CH_OFFSET:
        val = dat->offset;
        result = &val;
        break;
      case CH_SCALE:
        val = dat->scale;
        result = &val;
        break;
      case CH_AXIS:
        ival = dat->axis;
        result = &ival;
        break;
     }
  return result;
}

int NoiseData_set ( void *myinst, unsigned long vid, void *value )
{
  TxtrData *dat = (TxtrData*)myinst;
  int rc=0;
  if ( dat )
     switch ( vid ) {
      case CH_OFFSET:
        dat->offset = *((double*)value);
        rc = 1;
        break;
      case CH_SCALE:
        dat->scale = *((double*)value);
        rc = 1;
        break;
      case CH_AXIS:
        dat->axis = *((int*)value);
        break;
     }

  return rc;
}

void  XPaneChangeFunc( LWXPanelID   pan,unsigned long cid,unsigned long vid,int     event_type )
{
   TxtrData *dat;

   dat = (*GlobalXPanFun->getData)(pan,0);
   if (event_type == LWXPEVENT_VALUE && GlobalLWUpdate)
   {
      (*GlobalTextureFuncs->newtime)(dat->txtr,gTime,gFrame);

      if (dat->type == CHANNEL)
         (*GlobalLWUpdate)(LWCHANNEL_HCLASS, dat);
      else if (dat->type == MOTION)
         (*GlobalLWUpdate)(LWITEMMOTION_HCLASS, dat);
      else if (dat->type == FILTER)
         (*GlobalLWUpdate)(LWIMAGEFILTER_HCLASS, dat);
      else if (dat->type == ENVIRON)
         (*GlobalLWUpdate)(LWENVIRONMENT_HCLASS, dat);

      (*GlobalTextureFuncs->cleanup)(dat->txtr);
   }
}

void TxtrButtonEvent(LWXPanelID     panID,unsigned long cid)
{
   TxtrData    *dat = NULL;

   dat = (*GlobalXPanFun->getData)(panID,CH_TXTR);
   if (!dat)
      return;

   if (dat->type == CHANNEL)
      (*GlobalTxtrEdFuncs->open)(chanTxtredID,dat->txtr,dat->name);
   else if (dat->type == MOTION)
      (*GlobalTxtrEdFuncs->open)(motTxtredID,dat->txtr,dat->name);
   else if (dat->type == FILTER)
      (*GlobalTxtrEdFuncs->open)(fltTxtredID,dat->txtr,dat->name);
   else if (dat->type == ENVIRON)
      (*GlobalTxtrEdFuncs->open)(envTxtredID,dat->txtr,dat->name);
}


LWXPanelID TxtrXPanel(GlobalFunc *global, TxtrData *dat)
{
   LWXPanelFuncs *lwxpf = NULL;
   LWXPanelID     panID = NULL;
   static LWXPanelHint hint[] = {
      XpLABEL(0,"Apply Texture"),
      XpSTRLIST (CH_AXIS,axisList),
      // texture button
      XpADD(CH_TXTR,"vButton",NULL),
      XpLABEL(CH_TXTR,STR_Txtr_TEXT),
      XpBUTNOTIFY(CH_TXTR,TxtrButtonEvent),
      XpCHGNOTIFY(XPaneChangeFunc),
      XpEND
   };

   lwxpf = GlobalXPanFun;
   if ( lwxpf )
   {
      panID = (*lwxpf->create)( LWXP_VIEW, ctrl_list );
      if(panID)
      {
         (*lwxpf->hint) ( panID, 0, hint );
         (*lwxpf->describe)( panID, data_descrip, NoiseData_get, NoiseData_set );
         (*lwxpf->viewInst)( panID, dat );
         (*lwxpf->setData)(panID,CH_TXTR,dat);
         (*lwxpf->setData)(panID,0,dat);
      }
    }

   return panID;
}

XCALL_(static int) TxtrChannel_UI (
   long        version,
   GlobalFunc     *global,
   LWInterface    *UI,
   void        *serverData)
{
   TxtrData *dat;
   XCALL_INIT;

   if (version != LWINTERFACE_VERSION)
      return (AFUNC_BADVERSION);

   GlobalTxtrEdFuncs = (*global) (LWTXTREDFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalTxtrEdFuncs )
      return AFUNC_BADGLOBAL;

   GlobalXPanFun = (*global) (LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalXPanFun )
      return AFUNC_BADGLOBAL;

   GlobalLWUpdate = (*global) (LWINSTUPDATE_GLOBAL, GFUSE_TRANSIENT);
   if (!GlobalLWUpdate )
      return AFUNC_BADGLOBAL;

   dat = UI->inst;
   if (!chanTxtredID && dat->type == CHANNEL)
      chanTxtredID = (*GlobalTxtrEdFuncs->subscribe)("TextureChannel",TEF_ALL - TEF_USEBTN,NULL,NULL,NULL,TxtrEventFunc);
   if (!fltTxtredID && dat->type == FILTER)
      fltTxtredID = (*GlobalTxtrEdFuncs->subscribe)("TextureFilter",TEF_ALL - TEF_USEBTN,NULL,NULL,NULL,TxtrEventFunc);
   if (!motTxtredID && dat->type == MOTION)
      motTxtredID = (*GlobalTxtrEdFuncs->subscribe)("TextureMotion",TEF_ALL - TEF_USEBTN,NULL,NULL,NULL,TxtrEventFunc);
   if (!envTxtredID && dat->type == ENVIRON)
      envTxtredID = (*GlobalTxtrEdFuncs->subscribe)("TextureEnvironment",TEF_ALL - TEF_USEBTN,NULL,NULL,NULL,TxtrEventFunc);

   UI->panel = TxtrXPanel(global,dat);

   return AFUNC_OK;
}

ServerRecord ServerDesc[] = {
   { LWCHANNEL_HCLASS,     "LW_TextureChannel",    TxtrChannel },
   { LWCHANNEL_ICLASS,     "LW_TextureChannel",    TxtrChannel_UI },
   { LWITEMMOTION_HCLASS,  "LW_TextureMotion",     TxtrMotion },
   { LWITEMMOTION_ICLASS,  "LW_TextureMotion",     TxtrChannel_UI },
   { LWIMAGEFILTER_HCLASS, "LW_TextureFilter",     TxtrFilter },
   { LWIMAGEFILTER_ICLASS, "LW_TextureFilter",     TxtrChannel_UI },
   { LWENVIRONMENT_HCLASS, "LW_TextureEnvironment",TxtrEnviron },
   { LWENVIRONMENT_ICLASS, "LW_TextureEnvironment",TxtrChannel_UI },
   { NULL }
};
