//
// FrameJumper.h
//

#ifndef NULL
  #define NULL 0
#endif

#include  <lwserver.h>
#include  <lwhost.h>
#include  <lwmonitor.h>
#include  <lwrender.h>
#include  <lwio.h>
#include  <lwdyna.h>

#include  <lwgeneric.h>
#include  <lwmaster.h>

extern "C" {
  // First/Last Jumpers
  XCALL_ (int) JumpToRenderFirstFrame_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );
  XCALL_ (int) JumpToRenderLastFrame_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData );

  XCALL_ (int) JumpToPreviewFirstFrame_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData );
  XCALL_ (int) JumpToPreviewLastFrame_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );

  // Step Jumpers
  XCALL_ (int) NextFrameByRenderStep_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData );
  XCALL_ (int) PreviousFrameByRenderStep_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData );

  XCALL_ (int) NextFrameByPreviewStep_Activate( long version, GlobalFunc *global,
                                                void *local, void *serverData );
  XCALL_ (int) PreviousFrameByPreviewStep_Activate( long version, GlobalFunc *global,
                                                    void *local, void *serverData );

  // Range Setters
  XCALL_ (int) SetPreviewRangeToRenderRange_Activate( long version, GlobalFunc *global,
                                                      void *local, void *serverData );
  XCALL_ (int) SetRenderRangeToPreviewRange_Activate( long version, GlobalFunc *global,
                                                      void *local, void *serverData );

  // Next/Prev Markers
  XCALL_ (int) JumpToNextMarker_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData );
  XCALL_ (int) JumpToPreviousMarker_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  // Next/Prev Marker Groups
  XCALL_ (int) JumpToNextMarkerGroup_Activate( long version, GlobalFunc *global,
                                               void *local, void *serverData );
  XCALL_ (int) JumpToPreviousMarkerGroup_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData );

  // Markers
  XCALL_ (int) MarkFrame_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData );
  XCALL_ (int) UnmarkFrame_Activate( long version, GlobalFunc *global,
                                     void *local, void *serverData );

  XCALL_ (int) MarkFrameAs1_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs2_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs3_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs4_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs5_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs6_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs7_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs8_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs9_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) MarkFrameAs10_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) MarkFrameAs11_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) MarkFrameAs12_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) JumpToMarker1_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker2_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker3_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker4_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker5_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker6_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker7_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker8_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker9_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData );

  XCALL_ (int) JumpToMarker10_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) JumpToMarker11_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  XCALL_ (int) JumpToMarker12_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData );

  // Master
  XCALL_ (int) FrameJumperMaster_Activate( long version, GlobalFunc *global,
                                            void *_local, void *serverData );

  // Interface
  XCALL_ (int) FrameJumperInterface_Activate( long version, GlobalFunc *global,
                                              void *local, void *serverData );

  XCALL_ (int) FrameJumperInterfaceGeneric_Activate( long version, GlobalFunc *global,
                                                     void *_local, void *serverData );
}

int MarkFrameAs( int key, long version, GlobalFunc *global, void *local );
int JumpToMarker( int key, long version, GlobalFunc *global, void *local );
