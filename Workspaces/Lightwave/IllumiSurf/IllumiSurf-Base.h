//
// IllumiSurf-Base.h
//

#include <lwserver.h>
#include <lwshader.h>
#include <lwsurf.h>
#include <lwvparm.h>
#include <lwsurf.h>

#include <portable/dynarray/dynarray.h>

class IllumiSurf_Instance;
class IllumiSurf_Interface;

// Plug-in Name
#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

#define SHADER_PLUGIN_NAME "IllumiSurf"DEBUG_PLUGIN_PLUGNAME

// Globals
extern GlobalFunc *global;
extern DynArray< IllumiSurf_Instance * >   instances;
extern IllumiSurf_Instance               * copy_inst;

extern LWSurfaceFuncs  * surf_funcs;
extern LWEnvelopeFuncs * env_funcs;
extern LWVParmFuncs    * vparam_funcs;
extern LWItemInfo      * item_info;
extern LWLightInfo     * light_info;
extern LWObjectInfo    * object_info;
extern LWFileIOFuncs   * fileio;

extern IllumiSurf_Interface *is_int;

LWError IllumiSurf_OpenUI( void *_inst );
