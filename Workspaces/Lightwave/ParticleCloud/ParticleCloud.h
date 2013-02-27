//
// ParticleCloud.h
//

#include <lightwave/lw_sdk_cpp_wrappers/LWWrappers_L6.h>

#include <lwgeneric.h>
#include <lwcustobj.h>

extern "C" {
  // Particle Cloud Custom Object
  XCALL_ (int) ParticleCloud_Activate( long version, GlobalFunc *global,
                                       void *_local, void *serverData );

  // Particle Cloud Interface
  XCALL_ (int) ParticleCloud_Interface_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );

  // Particle Cloud Generics
  XCALL_ (int) ParticleCloud_Apply_Activate( long version, GlobalFunc *global,
                                            void *_local, void *serverData );

  XCALL_ (int) ParticleCloud_Properties_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData );

  XCALL_ (int) ParticleCloud_SaveAll_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) ParticleCloud_Capture_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

//  void * Startup( void );
//  void   Shutdown( void *serverData );

  // Custom Object functions
  LWInstance       ParticleCloud_Create(   void *data, void *context, LWError *error );
  void             ParticleCloud_Destroy(  LWInstance _inst );
  LWError          ParticleCloud_Copy(     LWInstance _to, LWInstance _from );
  LWError          ParticleCloud_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError          ParticleCloud_Save(     LWInstance _inst, const LWSaveState *saver );
  const char     * ParticleCloud_Describe( LWInstance _inst );

  void             ParticleCloud_Evaluate( LWInstance _inst, const LWCustomObjAccess *access );
  unsigned int     ParticleCloud_Flags   ( LWInstance _inst );

  const LWItemID * ParticleCloud_UseItems( LWInstance _inst );
  void             ParticleCloud_ChangeID( LWInstance _inst, const LWItemID *ids );

  LWError          ParticleCloud_Init    ( LWInstance _inst, int);
  void             ParticleCloud_CleanUp ( LWInstance _inst );
  LWError          ParticleCloud_NewTime ( LWInstance _inst, LWFrame frame, LWTime time );

  // Scan Functions
  int PointScan_GetPositions( void *data, LWPntID id );
}

// ParticleCloud_Point
class ParticleCloud_Point {
public:
  ParticleCloud_Point() { ; }

         LWFVector      position;
  static LWMeshInfoID   mesh;
};