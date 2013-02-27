/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWTEXTURE.H -- LightWave Procedural Textures
 *
 * This header defines the procedural texture render handler.  These
 * are the building blocks of texture channel layers.
 */
#ifndef LWSDK_TEXTURE_H
#define LWSDK_TEXTURE_H

#include <lwrender.h>

#define LWTEXTURE_HCLASS    "TextureHandler"
#define LWTEXTURE_ICLASS    "TextureInterface"
#define LWTEXTURE_VERSION    4


typedef struct st_LWTextureAccess {
    double        wPos[3],tPos[3],size[3],amp;
    double        spotSize;
    double        txGrad[3];
    int            axis,flags;
    double        octaves;        // number of octaves to use (HyperVoxel only)
} LWTextureAccess;

typedef struct st_LWTextureHandler {
    LWInstanceFuncs     *inst;
    LWItemFuncs     *item;
    LWRenderFuncs     *rend;
    double          (*evaluate) (LWInstance, LWTextureAccess *);
    unsigned int    (*flags)    (LWInstance);
} LWTextureHandler;

#define LWTEXF_GRAD            (1<<0)
#define LWTEXF_SLOWPREVIEW    (1<<1)
#define LWTEXF_AXIS            (1<<2)
#define LWTEXF_AALIAS        (1<<3)
#define LWTEXF_DISPLACE        (1<<4)
#define LWTEXF_HV_SRF        (1<<5)
#define LWTEXF_HV_VOL        (1<<6)

#define LWTXEF_VECTOR        (1<<0)
#define LWTXEF_AXISX        (1<<1)
#define LWTXEF_AXISY        (1<<2)
#define LWTXEF_AXISZ        (1<<3)
#define LWTXEF_DISPLACE        (1<<4)

#endif
