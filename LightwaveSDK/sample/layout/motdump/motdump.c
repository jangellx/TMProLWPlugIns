/*
 * MOTDUMP.C -- Motion Dump Generic Plug-in
 *		Write to a file the full motion for the item whose name
 *		begins with '$', or the camera.
 *
 * by Allen Hastings
 * revised 4/8/96
 */
#include <splug.h>
#include <moni.h>
#include <lwran.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*
 * Disable warnings for various conversions.
 */
#ifdef _WIN32
 #pragma warning(disable:4244)
#endif


/*
 * Saved global pointers.
 */
static LWItemInfo   *ii;
static LWSceneInfo  *si;
static MessageFuncs *mf;


/*
 * Activation function is called from the generic plug-ins popup.
 */
XCALL_(static int) motiondump (long version, GlobalFunc *global, void *local, void *serverData) {
    FILE *fp;
    LWTime t;
    double channels[9];
    LWItemID id;
    LWFrame i;
    int found,j;
    char *name;

    XCALL_INIT;
    if (version != 1)
        return (AFUNC_BADVERSION);

    ii = (LWItemInfo *) (*global)("LW Item Info",GFUSE_TRANSIENT);
    si = (LWSceneInfo *) (*global)("LW Scene Info",GFUSE_TRANSIENT);
    mf = (MessageFuncs *) (*global)("Info Messages",GFUSE_TRANSIENT);
    if (!ii || !si || !mf)
	return (AFUNC_BADGLOBAL);

    /*
     * Open output file.
     */
    if (!(fp = fopen("MotDump.txt","w"))) {
        (*mf->error)("Can't open MotDump.txt.",NULL);
        return (AFUNC_OK);
    }

    /*
     * Scan through objects looking for a name that starts with '$'.
     */
    found = 0;
    id = (*ii->first)(LWI_OBJECT,LWITEM_NULL);
    while (id != LWITEM_NULL) {
        name = (char *) (*ii->name)(id);
        if (name[0] == '$') {
            found = 1;
            break;
        }
	/*
	 * The object's bones could be looped through here
	 */
        id = (*ii->next)(id);
    }

    /*
     * Scan through lights looking for a name that starts with '$'.
     */
    if (!found) {
        id = (*ii->first)(LWI_LIGHT,LWITEM_NULL);
        while (id != LWITEM_NULL) {
            name = (char *) (*ii->name)(id);
            if (name[0] == '$') {
                found = 1;
                break;
            }
            id = (*ii->next)(id);
        }
    }

    /*
     * If no object or light name begins with '$', the camera motion
     * is dumped:
     */
    if (!found) {
        id = (*ii->first)(LWI_CAMERA,LWITEM_NULL);
        name = (char *) (*ii->name)(id);
    }

    /*
     * Write position, rotation and scaling for each frame in the scene.
     * Rotations are converted to degrees.
     */
    (*mf->info)("Writing motion channel data for:",name);
    for (i = 0; i <= si->frameEnd; i++) {
        t = (double) i / si->framesPerSecond;
        (*ii->param)(id,LWIP_POSITION,t,&channels[0]);
        (*ii->param)(id,LWIP_ROTATION,t,&channels[3]);
        (*ii->param)(id,LWIP_SCALING,t,&channels[6]);
        for (j = 0; j < 8; j++) {
            if (j >= 3 && j <= 5)
                channels[j] *= 180.0 / 3.14159265;
            fprintf(fp,"%g ",channels[j]);
        }
        fprintf(fp,"%g\n",channels[8]);
    }

    fclose(fp);
    (*mf->info)("Motion channels have been written to MotDump.txt",NULL);
    return (AFUNC_OK);
}


ServerRecord ServerDesc[] = {
    { "LayoutGeneric", "Demo_MotionDump", motiondump},
    { NULL }
};
