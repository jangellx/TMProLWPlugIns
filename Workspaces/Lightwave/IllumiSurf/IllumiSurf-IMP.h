//
// IllumiSurf-IMP.h
//

#include "IllumiSurf-Instance.h"
#include "../imp/impglobal.h"

//
extern IMPBank bank_IllumiSurfBasic;
extern IMPBank bank_IllumiSurfShading;
extern IMPBank bank_IllumiSurfReflection;
extern IMPBank bank_IllumiSurfTransmission;

extern IMPColumn col_IllumiSurf_SurfaceName;

extern int         value_int;
extern double      value_double[3];
extern LWChannelID value_channels[4];

int             IllumiSurf_Rows(       long bank_id, void * id );
void            IllumiSurf_EndProcess( long bank_id );
void            IllumiSurf_JumpTo(     int column, int row, void * id );
IMPGhostModes   IllumiSurf_Ghost(      int column, int row, LWItemID id );
LWChannelID   * EditVParamEnvelope(    IMPEnvRequests request, LWVParmID vparam );

#define COLWIDTH_TOGGLE       50
#define COLWIDTH_LIST        120
#define COLWIDTH_STRING      120
#define COLWIDTH_NUMERIC      65
#define COLWIDTH_NUMERIC_ENV  90
#define COLWIDTH_COLOR       130
#define COLWIDTH_COLOR_ENV   150

// FindInstance():
inline IllumiSurf_Instance * FindInstance( LWItemID id, int index ) {
  const char * item_name = object_info->filename( id );

  int instance_count = 0;
  for( unsigned long i=0; i < instances.NumElements(); i++ ) {
    const char * object_name = surf_funcs->sceneObject( instances[i]->GetSurface() );
    if( strcmp( item_name, object_name ) == 0 ) {
      if( instance_count == index )
        return instances[i];

      instance_count++;
    }
  }

  return NULL;
}
