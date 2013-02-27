/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWRENDER.H -- LightWave Rendering State
 *
 * This header contains the basic declarations need to define the
 * simplest LightWave plug-in server.
 */
#ifndef LWSDK_RENDER_H
#define LWSDK_RENDER_H

#include <lwtypes.h>
#include <lwhandler.h>
#include <lwenvel.h>
#include <lwmeshes.h>


typedef void *         LWItemID;
#define LWITEM_NULL     ((LWItemID) 0)

typedef int         LWItemType;
#define LWI_OBJECT     0
#define LWI_LIGHT     1
#define LWI_CAMERA     2
#define LWI_BONE     3

typedef int         LWItemParam;
#define LWIP_POSITION     1
#define LWIP_RIGHT     2
#define LWIP_UP         3
#define LWIP_FORWARD     4
#define LWIP_ROTATION     5
#define LWIP_SCALING     6
#define LWIP_PIVOT     7
#define LWIP_W_POSITION     8
#define LWIP_W_RIGHT     9
#define LWIP_W_UP     10
#define LWIP_W_FORWARD     11
#define LWIP_PIVOT_ROT     12

typedef double         LWRayCastFunc (const LWDVector position,
                    const LWDVector direction);

typedef double         LWRayTraceFunc (const LWDVector position,
                     const LWDVector direction,
                     LWDVector color);

typedef double         LWRayShadeFunc (const LWDVector position,
                     const LWDVector direction,
                     struct st_LWShaderAccess *);

typedef int         LWIlluminateFunc (LWItemID light, const LWDVector position,
                       LWDVector direction, LWDVector color);


/*
 * Animation item handler extensions.
 */
typedef struct st_LWItemFuncs {
    const LWItemID *    (*useItems) (LWInstance);
    void        (*changeID) (LWInstance, const LWItemID *);
} LWItemFuncs;

typedef struct st_LWItemHandler {
    LWInstanceFuncs     *inst;
    LWItemFuncs     *item;
} LWItemHandler;

#define LWITEM_ALL    ((LWItemID) ~0)


/*
 * Render handler extensions.
 */
typedef struct st_LWRenderFuncs {
    LWError        (*init)    (LWInstance, int);
    void        (*cleanup) (LWInstance);
    LWError        (*newTime) (LWInstance, LWFrame, LWTime);
} LWRenderFuncs;

#define LWINIT_PREVIEW     0
#define LWINIT_RENDER     1

typedef struct st_LWRenderHandler {
    LWInstanceFuncs     *inst;
    LWItemFuncs     *item;
    LWRenderFuncs     *rend;
} LWRenderHandler;


/*
 * Globals.
 */
#define LWITEMINFO_GLOBAL    "LW Item Info 3"

typedef struct st_LWItemInfo {
    LWItemID    (*first)  (LWItemType, LWItemID);
    LWItemID    (*next)   (LWItemID);
    LWItemID    (*firstChild) (LWItemID parent);
    LWItemID    (*nextChild)  (LWItemID parent, LWItemID prevChild);
    LWItemID    (*parent) (LWItemID);
    LWItemID    (*target) (LWItemID);
    LWItemID    (*goal)   (LWItemID);
    LWItemType    (*type)   (LWItemID);
    const char *    (*name)   (LWItemID);
    void        (*param)  (LWItemID, LWItemParam, LWTime,
                   LWDVector);
    unsigned int    (*limits) (LWItemID, LWItemParam,
                   LWDVector min, LWDVector max);

    const char *    (*getTag) (LWItemID, int);
    void        (*setTag) (LWItemID, int, const char *);
    LWChanGroupID    (*chanGroup) (LWItemID);
} LWItemInfo;

#define LWVECF_0    (1<<0)
#define LWVECF_1    (1<<1)
#define LWVECF_2    (1<<2)


#define LWOBJECTINFO_GLOBAL    "LW Object Info 2"

typedef struct st_LWObjectInfo {
    const char *    (*filename)    (LWItemID);
    int        (*numPoints)   (LWItemID);
    int        (*numPolygons) (LWItemID);
    unsigned int    (*shadowOpts)  (LWItemID);
    double        (*dissolve)    (LWItemID, LWTime);
    LWMeshInfoID    (*meshInfo)    (LWItemID, int frozen);
} LWObjectInfo;

#define LWOSHAD_SELF    (1<<0)
#define LWOSHAD_CAST    (1<<1)
#define LWOSHAD_RECEIVE    (1<<2)


#define LWBONEINFO_GLOBAL    "LW Bone Info 2"

typedef struct st_LWBoneInfo {
    unsigned int    (*flags)      (LWItemID);
    void        (*restParam)  (LWItemID, LWItemParam, LWDVector);
    double        (*restLength) (LWItemID);
    void        (*limits)     (LWItemID, double *inner, double *outer);
    const char *    (*weightMap)  (LWItemID);
} LWBoneInfo;

#define LWBONEF_ACTIVE        (1<<0)
#define LWBONEF_LIMITED_RANGE    (1<<1)
#define LWBONEF_SCALE_STRENGTH    (1<<2)
#define LWBONEF_WEIGHT_MAP_ONLY    (1<<3)
#define LWBONEF_WEIGHT_NORM    (1<<4)


#define LWLIGHTINFO_GLOBAL    "LW Light Info 2"

typedef struct st_LWLightInfo {
    void        (*ambient)    (LWTime, LWDVector color);
    int        (*type)       (LWItemID);
    void        (*color)      (LWItemID, LWTime, LWDVector color);
    int        (*shadowType) (LWItemID);
    void        (*coneAngles) (LWItemID, double *radius, double *edge);
    unsigned int    (*flags)      (LWItemID);
    double        (*range)      (LWItemID);
} LWLightInfo;

#define LWLIGHT_DISTANT     0
#define LWLIGHT_POINT     1
#define LWLIGHT_SPOT     2
#define LWLIGHT_LINEAR     3
#define LWLIGHT_AREA     4

#define LWLSHAD_OFF     0
#define LWLSHAD_RAYTRACE 1
#define LWLSHAD_MAP     2

#define LWLFL_LIMITED_RANGE    (1<<0)
#define LWLFL_NO_DIFFUSE    (1<<1)
#define LWLFL_NO_SPECULAR    (1<<2)
#define LWLFL_NO_CAUSTICS    (1<<3)
#define LWLFL_LENS_FLARE    (1<<4)
#define LWLFL_VOLUMETRIC    (1<<5)


#define LWCAMERAINFO_GLOBAL    "LW Camera Info"

typedef struct st_LWCameraInfo {
    double        (*zoomFactor)    (LWItemID, LWTime);
    double        (*focalLength)   (LWItemID, LWTime);
    double        (*focalDistance) (LWItemID, LWTime);
    double        (*fStop)         (LWItemID, LWTime);
    double        (*blurLength)    (LWItemID, LWTime);
    void        (*fovAngles)     (LWItemID, LWTime, double *horizontal,
                                double *vertical);
} LWCameraInfo;


#define LWSCENEINFO_GLOBAL    "LW Scene Info 2"

typedef struct st_LWSceneInfo {
    const char    *name;
    const char    *filename;
    int         numPoints;
    int         numPolygons;
    int         renderType;
    int         renderOpts;
    LWFrame         frameStart;
    LWFrame         frameEnd;
    LWFrame         frameStep;
    double         framesPerSecond;
    int         frameWidth;
    int         frameHeight;
    double         pixelAspect;
    int         minSamplesPerPixel;
    int         maxSamplesPerPixel;
    int         limitedRegion[4];    /* x0, y0, x1, y1 */
    int         recursionDepth;
} LWSceneInfo;

#define LWRTYPE_WIRE         0
#define LWRTYPE_QUICK         1
#define LWRTYPE_REALISTIC     2

#define LWROPT_SHADOWTRACE    (1<<0)
#define LWROPT_REFLECTTRACE    (1<<1)
#define LWROPT_REFRACTTRACE    (1<<2)
#define LWROPT_FIELDS        (1<<3)
#define LWROPT_EVENFIELDS    (1<<4)
#define LWROPT_MOTIONBLUR    (1<<5)
#define LWROPT_DEPTHOFFIELD    (1<<6)
#define LWROPT_LIMITEDREGION    (1<<7)


#define LWCOMPINFO_GLOBAL    "LW Comp Info"

typedef struct st_LWCompInfo {
    LWImageID      bg;
    LWImageID     fg;
    LWImageID     fgAlpha;
} LWCompInfo;


#define LWBACKDROPINFO_GLOBAL    "LW Backdrop Info"

typedef struct st_LWBackdropInfo {
    void        (*backdrop) (LWTime, const double ray[3], double color[3]);
} LWBackdropInfo;


#define LWFOGINFO_GLOBAL    "LW Fog Info"

typedef struct st_LWFogInfo {
    int          type;
    int          flags;
    double        (*minDist) (LWTime);
    double        (*maxDist) (LWTime);
    double        (*minAmt)  (LWTime);
    double        (*maxAmt)  (LWTime);
    void        (*color)   (LWTime, double col[3]);
} LWFogInfo;

#define LWFOG_NONE        0
#define LWFOG_LINEAR        1
#define LWFOG_NONLINEAR1    2
#define LWFOG_NONLINEAR2    3

#define LWFOGF_BACKGROUND    (1<<0)


#define LWINTERFACEINFO_GLOBAL    "LW Interface Info"

typedef struct st_LWInterfaceInfo {
    LWTime          curTime;
    const LWItemID   *selItems;
    unsigned int    (*itemFlags) (LWItemID);
} LWInterfaceInfo;

#define LWITEMF_SELECTED    (1<<0)
#define LWITEMF_SHOWCHILDREN    (1<<1)
#define LWITEMF_SHOWCHANNELS    (1<<2)
#define LWITEMF_LOCKED        (1<<3)


#define LWGLOBALPOOL_RENDER_GLOBAL    "Global Render Memory"
#define LWGLOBALPOOL_GLOBAL        "Global Memory"

typedef void *        LWMemChunk;

typedef struct st_LWGlobalPool {
    LWMemChunk    (*first) (void);
    LWMemChunk    (*next)  (LWMemChunk);
    const char *    (*ID)    (LWMemChunk);
    int        (*size)  (LWMemChunk);

    LWMemChunk    (*find)   (const char *ID);
    LWMemChunk    (*create) (const char *ID, int size);
} LWGlobalPool;

#endif

