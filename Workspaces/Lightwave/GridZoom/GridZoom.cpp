//
// GridZoom.cpp
//

#include "gridzoom.h"

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  ServerUserName GridZoomIn_Usernames[]        = { {"TM-P  Grid Zoom In"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom In"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName GridZoomInMore_Usernames[]    = { {"TM-P  Grid Zoom In (More)"DEBUG_PLUGIN_USERNAME,    LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom In (More)"DEBUG_PLUGIN_USERNAME,    LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName GridZoomInDouble_Usernames[]  = { {"TM-P  Grid Zoom In (Double)"DEBUG_PLUGIN_USERNAME,  LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom In (Double)"DEBUG_PLUGIN_USERNAME,  LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName GridZoomOut_Usernames[]       = { {"TM-P  Grid Zoom Out"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom Out"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName GridZoomOutMore_Usernames[]   = { {"TM-P  Grid Zoom Out (More)"DEBUG_PLUGIN_USERNAME,   LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom Out (More)"DEBUG_PLUGIN_USERNAME,   LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName GridZoomOutDouble_Usernames[] = { {"TM-P  Grid Zoom Out (Double)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid Zoom Out (Double)"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName Grid10_0m_Usernames[]         = { {"TM-P  Grid To 10.0m"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid To 10.0m"DEBUG_PLUGIN_USERNAME,          LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName Grid1_0m_Usernames[]          = { {"TM-P  Grid To 1.0m"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid To 1.0m"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName Grid0_1m_Usernames[]          = { {"TM-P  Grid To 0.1m"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_USERNAME },  { "Grid To 0.1m"DEBUG_PLUGIN_USERNAME,           LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomIn"DEBUG_PLUGIN_PLUGNAME,        GridZoomIn_Activate,        GridZoomIn_Usernames        },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomInDouble"DEBUG_PLUGIN_PLUGNAME,  GridZoomInMore_Activate,    GridZoomInMore_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomInMore"DEBUG_PLUGIN_PLUGNAME,    GridZoomInDouble_Activate,  GridZoomInDouble_Usernames  },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomOut"DEBUG_PLUGIN_PLUGNAME,       GridZoomOut_Activate,       GridZoomOut_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomOutDouble"DEBUG_PLUGIN_PLUGNAME, GridZoomOutMore_Activate,   GridZoomOutMore_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridZoomOutMore"DEBUG_PLUGIN_PLUGNAME,   GridZoomOutDouble_Activate, GridZoomOutDouble_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridTo10.0m"DEBUG_PLUGIN_PLUGNAME,       Grid10_0m_Activate,         Grid10_0m_Usernames         },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridTo1.0m"DEBUG_PLUGIN_PLUGNAME,        Grid1_0m_Activate,          Grid1_0m_Usernames          },
    { LWLAYOUTGENERIC_CLASS, "TM-P_GridTo0.1m"DEBUG_PLUGIN_PLUGNAME,        Grid0_1m_Activate,          Grid0_1m_Usernames          },
    { (const char *)NULL }
  };
}

// GridZoomIn_Activate():
//  Zooms out twice while decreasing the grid size.
XCALL_ (int) GridZoomIn_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomOut" );
  (*generic->evaluate)( generic->data, "ZoomOut" );
  (*generic->evaluate)( generic->data, "DecreaseGrid" );

  return AFUNC_OK;
}

// GridZoomInMore_Activate():
//  Zooms out while decreasing the grid size.
XCALL_ (int) GridZoomInMore_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomOut" );
  (*generic->evaluate)( generic->data, "DecreaseGrid" );

  return AFUNC_OK;
}

// GridZoomInDouble_Activate():
//  Zooms in while decreasing the grid size.
XCALL_ (int) GridZoomInDouble_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomIn" );
  (*generic->evaluate)( generic->data, "DecreaseGrid" );

  return AFUNC_OK;
}

// GridZoomOut_Activate():
//  Zooms in twice while increasing the grid size.
XCALL_ (int) GridZoomOut_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomIn" );
  (*generic->evaluate)( generic->data, "ZoomIn" );
  (*generic->evaluate)( generic->data, "IncreaseGrid" );

  return AFUNC_OK;
}

// GridZoomOutMore_Activate():
//  Zooms in while increasing the grid size.
XCALL_ (int) GridZoomOutMore_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomIn" );
  (*generic->evaluate)( generic->data, "IncreaseGrid" );

  return AFUNC_OK;
}

// GridZoomOutDouble_Activate():
//  Zooms out while increasing the grid size.
XCALL_ (int) GridZoomOutDouble_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "ZoomOut" );
  (*generic->evaluate)( generic->data, "IncreaseGrid" );

  return AFUNC_OK;
}

// Grid110_0m_Activate():
//  Sets the Grid Size to 10.0m.
XCALL_ (int) Grid10_0m_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "GridSize 10.0" );

  return AFUNC_OK;
}

// Grid1_0m_Activate():
//  Sets the Grid Size to 1.0m.
XCALL_ (int) Grid1_0m_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "GridSize 1.0" );

  return AFUNC_OK;
}

// Grid0_1m_Activate():
//  Sets the Grid Size to 0.1m.
XCALL_ (int) Grid0_1m_Activate( long version, GlobalFunc *global,
                                   void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
  (*generic->evaluate)( generic->data, "GridSize 0.1" );

  return AFUNC_OK;
}
