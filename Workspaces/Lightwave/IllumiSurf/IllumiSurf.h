//
// IllumiSurf.h
//

#ifndef NULL
#define NULL 0
#endif

#include <lwserver.h>
#include <lwhost.h>
#include <lwmonitor.h>
#include <lwrender.h>
#include <lwio.h>
#include <lwdyna.h>

#include <lwshader.h>

#include <portable/dynarray/dynarray.h>

#include "illumisurf-instance.h"

extern "C" {
  // IllumiSurf Master
  XCALL_ (int) IllumiSurf_Activate( long version, GlobalFunc *global,
                                    void *_local, void *serverData );

  // IllumiSurf Master Interface
  XCALL_ (int) IllumiSurf_Interface_Activate( long version, GlobalFunc *_global,
                                              void *local, void *serverData );

  LWInstance       IllumiSurf_Create(   void *data, void *context, LWError *error );
  void             IllumiSurf_Destroy(  LWInstance _inst );
  LWError          IllumiSurf_Copy(     LWInstance _to, LWInstance _from );
  LWError          IllumiSurf_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError          IllumiSurf_Save(     LWInstance _inst, const LWSaveState *saver );
  const char     * IllumiSurf_Describe( LWInstance _inst );

  void             IllumiSurf_Evaluate( LWInstance _inst, LWShaderAccess *access );
  unsigned int     IllumiSurf_Flags   ( LWInstance _inst );

  const LWItemID * IllumiSurf_UseItems( LWInstance _inst );
  void             IllumiSurf_ChangeID( LWInstance _inst, const LWItemID *ids );

  LWError          IllumiSurf_Init    ( LWInstance _inst, int mode );
  void             IllumiSurf_CleanUp ( LWInstance _inst);
  LWError          IllumiSurf_NewTime ( LWInstance _inst, LWFrame frame, LWTime time);

  void             Intensify( double *outputs, int output_count, double total_intensity, double *base, double *alt, double low, double high, bool base_low, IllumiSurf_BlendModes blend_mode );
}

