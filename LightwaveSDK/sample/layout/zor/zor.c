/*
 * ZOR.C -- Z-Axis Opacity Ramp Plug-in
 *		Use null objects to control the transparency of a shader.
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
#include <string.h>

/*
 * Disables warnings for various conversions.
 */
#ifdef _WIN32
 #pragma warning(disable:4244)
#endif


/*
 * Saved globals.
 */
static LWItemInfo *ii;
static MessageFuncs *mf;


/*
 * ZOR instance data.
 */
struct zordata {
    LWTime time;
    double zmin,zmax;
    int findnulls;
};


	XCALL_(static LWInstance)
ZORcreate(LWError *err)
{
    struct zordata *zor;

    zor = calloc(sizeof(struct zordata),1);
    return ((LWInstance) zor);
}

	XCALL_(static void)
ZORdestroy(LWInstance inst)
{
    if (inst)
        free(inst);
}

	XCALL_(static LWError)
ZORcopy(LWInstance from,LWInstance to)
{
    return (NULL);
}

	XCALL_(static LWError)
ZORload(LWInstance inst,const LWLoadState *ls)
{
    return (NULL);
}

	XCALL_(static LWError)
ZORsave(LWInstance inst,const LWSaveState *ss)
{
    return (NULL);
}

	XCALL_(static LWError)
ZORinit(LWInstance inst)
{
    return (NULL);
}

	XCALL_(static void)
ZORcleanup(LWInstance inst)
{
    return;
}

	XCALL_(static LWError)
ZORnewtime(LWInstance inst,LWFrame f,LWTime t)
{
    struct zordata *zor;

    zor = (struct zordata *) inst;
    zor->time = t;
    zor->findnulls = 1;
    return (NULL);
}

	XCALL_(static void)
ZORevaluate(LWInstance inst,ShaderAccess *sa)
{
    LWItemID minid,maxid,id;
    struct zordata *zor;
    double pos[3],falloff;
    char *name;

    zor = (struct zordata *) inst;
    if (zor->findnulls) {
        zor->zmin = zor->zmax = 0.0;
        minid = maxid = LWITEM_NULL;
        id = (*ii->firstChild)(sa->objID);
        while (id != LWITEM_NULL) {
            name = (char *) (*ii->name)(id);
            if (!strncmp(name,"Zmin",4))
                minid = id;
            else if (!strncmp(name,"Zmax",4))
                maxid = id;
            id = (*ii->nextChild)(sa->objID,id);
        }
        if (minid != LWITEM_NULL) {
            (*ii->param)(minid,LWIP_POSITION,zor->time,pos);
            zor->zmin = pos[2];
        }
/*
        else
            (*mf->error)("Can't find \"Zmin\" object.",NULL);
*/
        if (maxid != LWITEM_NULL) {
            (*ii->param)(maxid,LWIP_POSITION,zor->time,pos);
            zor->zmax = pos[2];
        }
/*
        else
            (*mf->error)("Can't find \"Zmax\" object.",NULL);
*/
        if (zor->zmin >= zor->zmax) {
/*
            (*mf->error)("\"Zmin\" must have a lower Z coordinate than "
              "\"Zmax\".",NULL);
*/
            zor->zmin = -1.0;
            zor->zmax = 1.0;
        }
        zor->findnulls = 0;
    }
    if (sa->oPos[2] <= zor->zmin)
        falloff = 0.0;
    else if (sa->oPos[2] >= zor->zmax)
        falloff = 1.0;
    else
        falloff = (sa->oPos[2] - zor->zmin) / (zor->zmax - zor->zmin);
    sa->transparency = 1.0 - (1.0 - sa->transparency) * (1.0 - falloff);
}

	XCALL_(static unsigned int)
ZORflags(LWInstance inst)
{
    return (LWSHF_TRANSP);
}

	XCALL_(static const char *)
ZORdescln(LWInstance inst)
{
    return ("Z-Axis Opacity Ramp");
}

	XCALL_(static int)
zor(long version,GlobalFunc *global,ShaderHandler *local, void *serverData)
{
    XCALL_INIT;
    if (version != 2)
        return (AFUNC_BADVERSION);

    ii = (LWItemInfo *) (*global)("LW Item Info",GFUSE_TRANSIENT);
    mf = (MessageFuncs *) (*global)("Info Messages",GFUSE_TRANSIENT);
    if (!ii || !mf)
	return (AFUNC_BADGLOBAL);

    local->create   = ZORcreate;
    local->destroy  = ZORdestroy;
    local->load     = ZORload;
    local->save     = ZORsave;
    local->copy     = ZORcopy;
    local->init     = ZORinit;
    local->cleanup  = ZORcleanup;
    local->newTime  = ZORnewtime;
    local->evaluate = ZORevaluate;
    local->flags    = ZORflags;
    local->descln   = ZORdescln;

    return (AFUNC_OK);
}


ServerRecord ServerDesc[] = {
    { "ShaderHandler", "Demo_ZOR", zor },
    { NULL }
};
