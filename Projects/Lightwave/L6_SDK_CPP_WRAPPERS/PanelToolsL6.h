//
// PanelToolsL6.h
//
// This file contains the PanelTools class, which contains a number
//  of useful functions for creating panels.  All members are static,
//  and thus can be used without instancing the class.  It also
//  relies on the LWPanels macros.
//
// Many of these functions, like AlignLabels, have two versions.
//  The first takes one or two arguments for simple usage, while
//  the second can take an array of controls for complex positioning.
//
// Note that you should call the Init() function before calling any
//  of these tools.  Init() takes a global function pointer, and uses
//  that to get the version of Layout/Modeler to determine control 
//  spacing and other offsets that may be different between Lightwave
//  versions.
//

#ifndef HEADER_L6_PANELTOOLS
#define HEADER_L6_PANELTOOLS

#include <lwserver.h>
#include <lwpanel.h>
#include <lwhost.h>


enum PanelVersions {
  PANELVER_NONE = 0,
  PANELVER_56,
  PANELVER_L6 };

class PanelTools;

class PanelTools {
public:
  static void Init( GlobalFunc *global );
  static bool IsInit() { return (panel_version != PANELVER_NONE) ? true : false; }

  static bool AlignLabels( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool AlignLabels( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool AlignWith( LWControl *reference, LWControl *control );
  static bool AlignWith( LWControl **con_array );

  static bool CenterUnder( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING, bool put_under = true );
  static bool CenterUnder( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool PutUnder( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool PutUnder( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool PutOnLineWith( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_X_PADDING );
  static bool PutOnLineWith( LWControl **con_array, int padding = PT_DEFAULT_X_PADDING );

protected:
  static PanelVersions panel_version;     // Version of LWPanels, used for alignment purposes

  static int PT_DEFAULT_PADDING;          // Default amount of padding (Y)
  static int PT_DEFAULT_X_PADDING;        // Default amount of padding (X)
  static int PT_BASE_PADDING;             // Offset so buttons alight DIRECTLY under each other when padding == 0 (Y)
};

#endif