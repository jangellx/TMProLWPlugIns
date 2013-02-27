/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWTXTRED.H -- LightWave Texture Editor
 */
 
#ifndef LWSDK_TXTRED_H
#define LWSDK_TXTRED_H

#include    <lwtxtr.h>

#define        LWTXTREDFUNCS_GLOBAL    "Texture Editor"

/*
 * Clients can edit textures by subscribing to the texture editor services.
 * The subscription returns an identifier this identifier holds information 
 * about the client, some UI flags, etc, that are used in function calls.
 */
typedef struct st_TxtrEdClient    *LWTECltID;

/*
 * Clients can set call backs for the 'Remove Texture',
 * and the 'Automatic Sizing' buttons.
 * For the AutoSize call back, the client needs to fill in bbox
 * with the corners of the texture's bounding box:
 * bbox[][0] = min[],  bbox[][1] = max[]
 */
typedef void    LW_TxtrRemoveFunc(LWTextureID, void    *userData);
typedef int        LW_TxtrAutoSizeFunc(LWTextureID, void    *userData,double    bbox[3][2]);
typedef int        LW_TxtrEventFunc(LWTextureID, void    *userData,int    eventCode);

typedef    struct    st_LWTxtrEdFuncs {
    LWTECltID        (*subscribe)(char    *title,int    flags,void    *userData,LW_TxtrRemoveFunc    *,LW_TxtrAutoSizeFunc    *,LW_TxtrEventFunc    *);
    void            (*unsubscribe)(LWTECltID);

    void            (*open)(LWTECltID, LWTextureID,char    *title);
    void            (*setTexture)(LWTECltID, LWTextureID, char    *title);
    void            (*setPosition)(LWTECltID, int, int);
    void            (*close)(LWTECltID);
    int                (*isOpen)(LWTECltID);
    int                (*setParam)(int    tag,void    *data);
    int                (*getParam)(int    tag,void    *data);
} LWTxtrEdFuncs;

/*
Editor Flags
*/
#define    TEF_USEBTN        (1<<0)    // adds use/remove buttons at the bottom of the pane.
#define    TEF_OPACITY        (1<<1)    // adds layer opacity settings
#define    TEF_BLEND        (1<<2)    // adds blend options to the layer global settings
#define    TEF_TYPE        (1<<3)    // adds layer type control on top of pane.
#define    TEF_LAYERS        (1<<4)    // adds layer list pane on left side of the pane.
#define    TEF_ALL            31        // standard set of flags for the texture editor

/*
Event Codes passed to the event callback
*/
#define TXEV_ALTER         1
#define TXEV_TRACK         2
#define TXEV_DELETE         4

/* parameter tags */
typedef    enum 
{
    TXEDTAG_POSI,
    TXEDTAG_ROTA,
    TXEDTAG_SIZE,
    TXEDTAG_FALL,
    TXEDTAG_PROJ,
    TXEDTAG_AXIS,
    TXEDTAG_WWRP,
    TXEDTAG_HWRP
};

/* projection modes */
typedef enum {
    TXPRJ_PLANAR,
    TXPRJ_CYLINDRICAL,
    TXPRJ_SPHERICAL,
    TXPRJ_CUBIC,
    TXPRJ_FRONT,
    TXPRJ_UVMAP
};

#endif
