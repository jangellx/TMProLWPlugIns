
// PanelTools.h
//
// This file contains the PanelTools class, which contains a number
// of useful functions for creating panels.  All members are static,
// and thus can be used without instancing the class.  It also
// relies on the LWPanels macros.
//
// Many of these functions, like AlignLabels, have two versions.
// The first takes one or two arguments for simple usage, while
// the second can take an array of controls for complex positioning.

#pragma once

#ifdef LW_SDK_CPP_WRAPPERS_L6_HEADER
  #include <lwpanel.h>
#else
  #include <lwbase.h>
  #include <lwpanel5.h>
#endif

class PanelTools;

#ifdef LW_SDK_CPP_WRAPPERS_L6_HEADER
  #define PT_DEFAULT_PADDING    7     // Default amount of padding (Y)
#else
  #define PT_DEFAULT_PADDING    3     // Default amount of padding (Y)
#endif

#define PT_DEFAULT_X_PADDING    0     // Default amount of padding (X)
#define PT_BASE_PADDING       -12     // Offset so buttons alight DIRECTLY under each other when padding == 0 (Y)

class PanelTools {
public:
  static bool AlignLabels( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool AlignLabels( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool AlignWith( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool AlignWith( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool CenterUnder( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING, bool put_under = true );
  static bool CenterUnder( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool PutUnder( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool PutUnder( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool PutOnLineWith( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_X_PADDING );
  static bool PutOnLineWith( LWControl **con_array, int padding = PT_DEFAULT_X_PADDING );

  static bool RightAlign( LWControl *reference, LWControl *control, int h_padding = PT_DEFAULT_X_PADDING, bool put_under = true, int v_padding = PT_DEFAULT_PADDING );
  static bool RightAlign( LWControl **con_array, int h_padding = PT_DEFAULT_X_PADDING, bool put_under = true, int v_padding = PT_DEFAULT_PADDING );

  static bool RightAlignWithPanel( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControl *control, int h_padding = PT_DEFAULT_X_PADDING );
  static bool RightAlignWithPanel( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControl **con_array, int h_padding = PT_DEFAULT_X_PADDING, bool put_under = true, int v_padding = PT_DEFAULT_PADDING );

  static bool BottomAlign( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_X_PADDING );
  static bool BottomAlign( LWControl **con_array, int padding = PT_DEFAULT_X_PADDING );

  static bool PutOver( LWControl *reference, LWControl *control, int padding = PT_DEFAULT_PADDING );
  static bool PutOver( LWControl **con_array, int padding = PT_DEFAULT_PADDING );

  static bool Nudge( LWControl *control, int x, int y );
  static bool Nudge( LWControl **con_array, int x, int y );
};
