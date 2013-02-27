/*
 * LAZYPNTS.C -- Lazy Points Displacement Plug-in
 *
 * by Allen Hastings
 * last revision 4/8/96
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
 * Global -- Item Info functions.
 */
static LWItemInfo *ii;


/*
 * LazyPoints instance structure.  Lag-rate is in seconds per meter.
 */
struct lpdata {
    LWItemID id;
    LWTime time;
    float lagrate;
};


/*
 * Standard instance handler functions.  Only the lagrate needs to
 * be saved and loaded to preserve the behavior of the instance.
 */
	XCALL_(static LWInstance)
create(LWError *err,LWItemID id)
{
    struct lpdata *lp;

    XCALL_INIT;
    lp = calloc(sizeof(struct lpdata),1);
    lp->id = LWITEM_NULL;
    lp->time = 0.0;
    lp->lagrate = .25;
    return ((LWInstance) lp);
}

	XCALL_(static LWInstance)
create_1(LWError *err)
{
    return (create(err,LWITEM_NULL));
}

	XCALL_(static void)
destroy(LWInstance inst)
{
    XCALL_INIT;
    if (inst)
        free(inst);
}

	XCALL_(static LWError)
copy(LWInstance from,LWInstance to,LWItemID id)
{
    struct lpdata *old,*new;

    XCALL_INIT;
    old = (struct lpdata *) from;
    new = (struct lpdata *) to;
    *new = *old;
    return (NULL);
}

	XCALL_(static LWError)
copy_1(LWInstance from,LWInstance to)
{
    return (copy(from,to,LWITEM_NULL));
}

	XCALL_(static LWError)
load(LWInstance inst,const LWLoadState *ls)
{
    struct lpdata *lp;
    char line[80];

    XCALL_INIT;
    lp = (struct lpdata *) inst;
    (*ls->read)(ls->readData,line,80);
    sscanf(line,"%*s %f",&lp->lagrate);
    return (NULL);
}

	XCALL_(static LWError)
save(LWInstance inst,const LWSaveState *ss)
{
    struct lpdata *lp;
    char line[80];

    XCALL_INIT;
    lp = (struct lpdata *) inst;
    sprintf(line," LagRate %f",lp->lagrate);
    (*ss->write)(ss->writeData,line,80);
    return (NULL);
}


/*
 * Newtime initialization.  For each new time and item we cache the
 * time and the item ID.
 */
	XCALL_(static LWError)
newtime(LWInstance inst,LWItemID id,LWFrame f,LWTime t)
{
    struct lpdata *lp;

    XCALL_INIT;
    lp = (struct lpdata *) inst;
    lp->id = id;
    lp->time = t;
    return (NULL);
}


/*
 * The evaluation function is the core of the Lazy Points algorithm.
 * For each point affected by the Lazy Points field, we first calculate
 * its distance from the object's pivot point.  That distance (in
 * meters) times the lagrate (in seconds/meter) gives the time by which
 * that point will be offset backwards in time.  At that point in the
 * past we get the object's position and orientation and transform the
 * object point to those values.
 */
	XCALL_(static void)
evaluate(LWInstance inst,DisplacementAccess *da)
{
    struct lpdata *lp;
    double xax[3],yax[3],zax[3],piv[3],pos[3],*old,*new,dist,t;

    XCALL_INIT;
    lp = (struct lpdata *) inst;
    old = da->oPos;
    new = da->source;
    (*ii->param)(lp->id,LWIP_PIVOT,lp->time,piv);
    old[0] -= piv[0];
    old[1] -= piv[1];
    old[2] -= piv[2];
    dist = sqrt(old[0] * old[0] + old[1] * old[1] + old[2] * old[2]);
    t = lp->time - lp->lagrate * dist;
    (*ii->param)(lp->id,LWIP_RIGHT,t,xax);
    (*ii->param)(lp->id,LWIP_UP,t,yax);
    (*ii->param)(lp->id,LWIP_FORWARD,t,zax);
    (*ii->param)(lp->id,LWIP_W_POSITION,t,pos);
    new[0] = pos[0] + old[0] * xax[0] + old[1] * yax[0] + old[2] * zax[0]; 
    new[1] = pos[1] + old[0] * xax[1] + old[1] * yax[1] + old[2] * zax[1]; 
    new[2] = pos[2] + old[0] * xax[2] + old[1] * yax[2] + old[2] * zax[2];
} 


/*
 * Flags signals the type of transformation.  This is a world-coordinate
 * transform since we're computing the result in world-coordinates.
 */
	XCALL_(static unsigned int)
flags(LWInstance inst)
{
    XCALL_INIT;
    return (LWDMF_WORLD);
}


	XCALL_(static const char *)
descln(LWInstance inst)
{
    struct lpdata *lp;
    static char line[80];

    XCALL_INIT;
    lp = (struct lpdata *) inst;
    sprintf(line,"Lazy Points - Lag Rate: %f sec/m",lp->lagrate);
    return (line);
}


/*
 * Activation function.  Remembers required globals and sets up the
 * handler structure.  This will work in 4.0 and 5.0.
 */
	XCALL_(static int)
lazypoints (long version,GlobalFunc *global,
	    DisplacementHandler *local,void *serverData)
{
    DisplacementHandler_V1 *v1;

    XCALL_INIT;
    if (version == 1) {
	v1 = (DisplacementHandler_V1 *) local;
	v1->create = create_1;
	v1->destroy = destroy;
	v1->copy = copy_1;
	v1->load = load;
	v1->save = save;
	v1->newTime = newtime;
	v1->evaluate = evaluate;
	v1->flags = flags;
    } else if (version == 2) {
	local->create = create;
	local->destroy = destroy;
	local->copy = copy;
	local->load = load;
	local->save = save;
	local->newTime = newtime;
	local->evaluate = evaluate;
	local->flags = flags;
	local->descln = descln;
    } else
        return (AFUNC_BADVERSION);

    ii = (LWItemInfo *) (*global)("LW Item Info",GFUSE_TRANSIENT);
    if (!ii)
	return (AFUNC_BADGLOBAL);

    return (AFUNC_OK);
}


ServerRecord ServerDesc[] = {
    { "DisplacementHandler",	"Demo_LazyPoints",	lazypoints},
    { NULL }
};
