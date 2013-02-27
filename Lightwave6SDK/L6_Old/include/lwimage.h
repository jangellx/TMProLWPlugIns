/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWIMAGE.H -- LightWave Images
 */
#ifndef LWSDK_IMAGE_H
#define LWSDK_IMAGE_H

#include <lwtypes.h>
#include <lwimageio.h>
#include <lwio.h>


#define LWIMAGELIST_GLOBAL    "LW Image List 2"
#define LWIMAGEUTIL_GLOBAL    "Image Utility"

typedef struct st_LWImageList {
    LWImageID    (*first)    (void);
    LWImageID    (*next)     (LWImageID);
    LWImageID    (*load)     (const char *);
    const char *    (*name)     (LWImageID);
    const char *    (*filename) (LWImageID, LWFrame);
    int        (*isColor)  (LWImageID);
    void        (*needAA)   (LWImageID);
    void        (*size)     (LWImageID, int *w, int *h);
    LWBufferValue    (*luma)     (LWImageID, int x, int y);
    void        (*RGB)      (LWImageID, int x, int y, LWBufferValue[3]);
    double        (*lumaSpot) (LWImageID, double x, double y,
                     double spotSize, int blend);
    void        (*RGBSpot)  (LWImageID, double x, double y,
                     double spotSize, int blend, double[3]);
    void        (*clear)    (LWImageID);
    LWImageID       (*sceneLoad) (const LWLoadState *);
    void            (*sceneSave) (const LWSaveState *, LWImageID);

} LWImageList;


typedef struct st_LWImageUtil {
    LWImageID    (*create) (int w, int h, LWImageType type);
    void        (*destroy) (LWImageID img);
    int        (*save) (LWImageID img, int saver,const char *name);
    void        (*setPixel) (LWImageID img, int x, int y, void *pix);
    void        (*getPixel) (LWImageID img, int x, int y, void *pix);
    void        (*getInfo) (LWImageID img, int *w, int *h, int *type);
    LWImageID    (*resample) (LWImageID img, int w, int h, int mode);
    int        (*saverCount) ();
    const char *    (*saverName) (int saver);
} LWImageUtil;

/* resample modes */
enum {
        LWISM_SUBSAMPLING = 0,
        LWISM_MEDIAN,        /* for shrinking */
        LWISM_SUPERSAMPLING,
        LWISM_BILINEAR,        /* for expanding */
        LWISM_BSPLINE,        /* for expanding */
        LWISM_BICUBIC        /* for expanding */
};

#endif

