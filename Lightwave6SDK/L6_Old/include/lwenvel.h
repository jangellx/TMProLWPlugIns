/****
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWENVEL.H -- LightWave Animation Envelopes and Channel Info
 ****
 */
 
#ifndef LWSDK_ENVEL_H
#define LWSDK_ENVEL_H

#include    <lwtypes.h>
#include    <lwio.h>


typedef void *        LWEnvelopeID;
typedef void *        LWChanGroupID;
typedef void *        LWEnvKeyframeID;

typedef enum {    
    LWKEY_VALUE, 
    LWKEY_TIME, 
    LWKEY_SHAPE, 
    LWKEY_TENSION, 
    LWKEY_CONTINUITY, 
    LWKEY_BIAS, 
    LWKEY_PARAM_0, 
    LWKEY_PARAM_1, 
    LWKEY_PARAM_2, 
    LWKEY_PARAM_3
} LWKeyTag;

/* Envelope Type */
#define LWET_FLOAT        2
#define LWET_DISTANCE     3
#define LWET_PERCENT      4
#define LWET_ANGLE        5

typedef enum {    
    LWENVTAG_VISIBLE, 
    LWENVTAG_PREBEHAVE,
    LWENVTAG_POSTBEHAVE,
    LWENVTAG_KEYCOUNT
} LWEnvTag;


typedef enum {
    LWEEVNT_DESTROY,
    LWEEVNT_KEY_INSERT,
    LWEEVNT_KEY_DELETE,
    LWEEVNT_KEY_VALUE,
    LWEEVNT_KEY_TIME 
} LWEnvEvent;

typedef int    (*LWEnvEventFunc) (void *data, LWEnvelopeID env, LWEnvEvent event, void *eventData);

/****
 *
 * The existing EnvAccess API supported multi-valued keyframes.
 * These no longer exist. 
 *
 * The previous syetem also left the number of channels indeterminate
 * until the UI was called, and exposed structures without accepting
 * independently created instances of these.
 * 
 * The replacement global is "Animation Envelopes". Multiple channels
 * will be organized into groups, and plugins can logically gather
 * their parameters.
 *
 ****
 */
#define LWENVELOPEFUNCS_GLOBAL    "Animation Envelopes"

typedef struct st_LWEnvelopeFuncs {
    LWEnvelopeID        (*create)( LWChanGroupID group, const char *name, int type );
    void            (*destroy)( LWEnvelopeID env );
    LWChanGroupID        (*createGroup)( LWChanGroupID parent, const char *name );
    void            (*destroyGroup)( LWChanGroupID group);
    LWError            (*copy)( LWEnvelopeID to, LWEnvelopeID from);
    LWError            (*load)( LWEnvelopeID env, const LWLoadState *load);
    LWError            (*save)( LWEnvelopeID env, const LWSaveState *save);
    double            (*evaluate)( LWEnvelopeID env, LWTime time);
    int            (*edit)(LWChanGroupID group, LWEnvelopeID env, int flags, void *data);
    int            (*envAge)( LWEnvelopeID env );
    LWEnvKeyframeID        (*createKey)( LWEnvelopeID env, LWTime time, double value);
    void            (*destroyKey)( LWEnvelopeID env, LWEnvKeyframeID key);
    LWEnvKeyframeID        (*findKey)( LWEnvelopeID env, LWTime time);
    LWEnvKeyframeID        (*nextKey)( LWEnvelopeID env, LWEnvKeyframeID key);
    LWEnvKeyframeID        (*prevKey)( LWEnvelopeID env, LWEnvKeyframeID key);
    int            (*keySet)( LWEnvelopeID env, LWEnvKeyframeID key, LWKeyTag tag, void *value);
    int            (*keyGet)( LWEnvelopeID env, LWEnvKeyframeID key, LWKeyTag tag, void *value);
    int             (*setEnvEvent)( LWEnvelopeID env, LWEnvEventFunc  ev, void *data); 
    int             (*egSet)( LWEnvelopeID env, LWChanGroupID  group, int tag, void *value); 
    int             (*egGet)( LWEnvelopeID env, LWChanGroupID  group, int tag, void *value); 
} LWEnvelopeFuncs;


/**********************************************************************/


/****
 * 
 * The entire list of grouped envelopes can be accessed with the
 * "Channel Info" global. These envelopes may include plugin effects,
 * and are thus called channels, to distinguish them from the LWEnvelope
 * structures, which can be altered through keyframe manipulations. A
 * channel's underlying envelope data may also be read.
 ****
 */

#define LWCHANNELINFO_GLOBAL    "Channel Info"

typedef void    *LWChannelID;
typedef int    (*LWChanEventFunc)(void *data, LWChannelID ch, int event, void *eventData);

typedef struct st_LWChannelInfo {
    LWChanGroupID         (*nextGroup)(LWChanGroupID parent, LWChanGroupID group);  /* next group, first on NULL in parent group or root on NULL */
    LWChannelID         (*nextChannel)(LWChanGroupID parent, LWChannelID chan);  /* next channel, first on NULL in parent group or root on NULL */
    const char         *(*groupName)(LWChanGroupID group); 
    const char         *(*channelName)(LWChannelID chan); 
    LWChanGroupID          (*groupParent)(LWChanGroupID group); 
    LWChanGroupID          (*channelParent)(LWChannelID chan); 
    int              (*channelType)(LWChannelID chan); 
    double              (*channelEvaluate)(LWChannelID chan, LWTime time); 
    const LWEnvelopeID     (*channelEnvelope)(LWChannelID chan); 
    int              (*setChannelEvent)(LWChannelID chan, LWChanEventFunc ev, void *data); 
} LWChannelInfo;


/**********************************************************************/


/****  Variant Parameters  ****/

#define LWVPARMFUNCS_GLOBAL  "LWVParmFuncs"

typedef void *LWVParmID;

/* LWVParm types */
#define LWVP_FLOAT        0x10
#define LWVP_PERCENT      0x20
#define LWVP_DIST         0x30
#define LWVP_ANGLE        0x40
#define LWVP_COLOR        0x51
#define LWVPF_VECTOR      0x01

/****
 * Time-Variant parameters are double precision vectors (3 element arrays)
 * which may have a time-dependent nature - i.e., the value at time = 1.0
 * is different at time = 2.0. Time-Variant parameters create and destroy
 * LightWave envelopes as needed based on user interaction, application
 * events, etc.
 ****
 * ATTENTION: When using LWVParms with an LWXPanel, the VParm is "get-only"
 *            data. Never update the plugin's pointer during the XPanel
 *            "set". The plugin should always ever use (and give to an
 *            LWXPanel) the LWVParmID returned by the create method.
 ****
 * API Methods
 * -----------
 *
 * create:    Creates a time-variant parameter instance.
 * destroy:    Releases resources used by instance.
 * setup:    This MUST be called for each parameter created. The envelope
 *         group ID is required. Any envelopes created for the
 *         this parameter will belong to the specified group. A name
 *         is also recommended for the parameter since this is what the
 *         end-user may see when manipulating the envelope.
 * load:    Loads settings for a Time-Variant Parameter.
 * save:    Saves settings for a Time-Variant Parameter.
 * getVal:    Obtains the value of the parameter at a given time where
 *         result is a 3 element array to be popuplated.
 *         The method returns the number of elements which were
 *         actually processed (0, 1, or 3).
 * setVal:      Sets the value of the parameter where value is an array of
 *         3 doubles. If the paramter is enveloped, setting the value
 *         has no effect. The method returns the number of elements
 *         processed (0, 1, or 3).
 * hasEnv:    Returns true if the parameter has an envelope.
 * setEnvState: This creates or destroys the internal envelope of a
 *         parameter instance. A state value of 0 indicates any
 *         existing envelope for the parameter should be destroyed,
 *         essentially making the parameter a double-precision vector.
 *         If state is non-zero, an envelope is created for the parameter
 *         using the name and envelope group specified in the "setup"
 *         method.
 * editEnv:    If an envelope exists for the paramenter, this will open
 *         the envelope editor.
 ****
 */
typedef struct st_LWVParmFuncs {
    LWVParmID    (*create)    ( int type );
    void        (*destroy)    ( LWVParmID parm );
    void        (*setup)    ( LWVParmID      parm,
                      const char     *name,
                      LWChanGroupID  group);
    LWError        (*copy)        ( LWVParmID to, LWVParmID from );
    LWError        (*load)        ( LWVParmID parm,
                      const LWLoadState *load);
    LWError        (*save)        ( LWVParmID parm,
                      const LWSaveState *save);
    int        (*getVal)    ( LWVParmID parm,
                      LWTime t, double *result );
    int        (*setVal)    ( LWVParmID parm, double *value );
    int        (*hasEnv)    ( LWVParmID );
    void        (*setEnvState)    ( LWVParmID parm, int state );
    void        (*editEnv)    ( LWVParmID parm );
} LWVParmFuncs;

#endif

