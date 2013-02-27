/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWPRTCL.H -- LightWave Particles
 */
#ifndef LWSDK_PRTCL_H
#define LWSDK_PRTCL_H

#include <lwmeshes.h>
#include <lwrender.h>
#include <lwio.h>
#include <lwglobsrv.h>


#define LWPSYSFUNCS_GLOBAL    "Particle Services"

typedef struct st_LWPSys                *LWPSysID;
typedef struct st_LWPSBuf               *LWPSBufID;

/* particle types */
#define LWPST_PRTCL     0
#define LWPST_TRAIL     1
/* buffer flags */
#define LWPSB_POS       (1<<0) /* position (float[3]) */
#define LWPSB_SIZ       (1<<1) /* size (float) */
#define LWPSB_SCL       (1<<2) /* scale (float[3]) */
#define LWPSB_ROT       (1<<3) /* rotation (float[3]) */
#define LWPSB_VEL       (1<<4) /* velocity (float[3]) */
#define LWPSB_AGE       (1<<5) /* age (float) */
#define LWPSB_FCE       (1<<6) /* force (float) */
#define LWPSB_PRS       (1<<7) /* pressure     (float) */
#define LWPSB_TMP       (1<<8) /* temperature (float) */
#define LWPSB_MAS       (1<<9) /* mass (float) */
#define LWPSB_LNK       (1<<10)/* link to particle (for trails) (int) */
#define LWPSB_ID        (1<<11)/* particle ID, unique identifier for a particle (int) */
#define LWPSB_ENB       (1<<12)/* particle state (on/off) (char) */
#define LWPSB_RGBA      (1<<13)/* particle color,alpha (for display) (char[4]) */

typedef struct st_LWPSBufDesc { 
    const char      *name;
    int             dataType,dataSize;
}LWPSBufDesc;
/* data types */
#define LWPSBT_FLOAT    0
#define LWPSBT_INT      1
#define LWPSBT_CHAR     2

/* draw styles */
#define LWPSDS_POINT1   0       /* point 1 pixel size */
#define LWPSDS_POINT2   1       /* point 2 pixels size */
#define LWPSDS_POINT3   2       /* point 3 pixels size */
#define LWPSDS_TICKS    3       /* tick marks */
#define LWPSDS_TRIANGLE 4       /* triangle in XY plane, pointing towards Z */
#define LWPSDS_PYRAMID  5       /* pyramid, pointing towards Z */
#define LWPSDS_DISC     6       /* disc facing camera. */
#define LWPSDS_SPHERE   7       /* 3 discs on the 3 planes: XY,YZ,ZX */
/* draw flags */
#define LWPSDF_HIDDEN   (1<<0) /* makes particles invisible */
#define LWPSDF_FLAT     (1<<1) /* render particles flat (opposed to wireframe) */
#define LWPSDF_USECOLOR (1<<2) /* use particle colors set in color buffer */

/* mesh presets */
#define LWPSM_POINT     0       /* 1 point polygon */
#define LWPSM_LINE      1       /* line 1 unit long */
#define LWPSM_SQUARE    2       /* square side 1 unit long. */
#define LWPSM_SPHERE1   3       /* low def sphere (double pyramid) */
#define LWPSM_SPHERE2   4       /* higher definiton sphere. */
#define LWPSM_DISC      5       /* disc facing camera. */

typedef struct st_LWPSysFuncs {
    LWPSysID        (*create)(int flags,int type);
    int             (*destroy)(LWPSysID     ps);
    int             (*init)(LWPSysID        ps,int  np);    /* allocates all buffers with specified number of particles */
    void            (*cleanup) (LWPSysID    ps);            /* frees all buffers */
    void            (*load)(LWPSysID        ps,LWLoadState *lstate);
    void            (*save)(LWPSysID        ps,LWSaveState *sstate);
    int             (*getPCount)(LWPSysID       ps);    /* returns number of particles */
    void            (*setDraw)(LWPSysID     ps,int  style,int       flags); /* sets particle GL drawing */
    void            (*setMesh)(LWPSysID     ps,int  preset,LWMeshInfoID     mesh,double    size);  /* sets the particle mesh. This mesh can then be instanciated for all particles. */

    void            (*attach)(LWPSysID      ps,LWItemID     item);  /* attaches data to an item, multiple PSys can be attached to an item and multiple items can share the same data */
    void            (*detach)(LWPSysID      ps,LWItemID     item);  /* detaches data */
    LWPSysID        *(*getPSys)(LWItemID    item);                  /* returns null terminated list of PSys attached to item. */

    LWPSBufID       (*addBuf)(LWPSysID      ps,LWPSBufDesc  desc);
    LWPSBufID       (*getBufID)(LWPSysID    ps,int  bufFlag);
    void            (*setBufData)(LWPSBufID buf,void        *data);
    void            (*getBufData)(LWPSBufID buf,void        *data);

    int             (*addParticle)(LWPSysID            ps);
    void            (*remParticle)(LWPSysID            ps,int  idx);
    void            (*setParticle)(LWPSBufID        buf,int idx,void        *data);
    void            (*getParticle)(LWPSBufID        buf,int idx,void        *data);
} LWPSysFuncs;

#endif  LWSDK_PRTCL_H
