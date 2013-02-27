//
// PanelTools_Global.cpp
//

#define NULL 0

#include "PanelToolsGlobal.h"
#include "PanelTools_Global.h"
#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>

TMP_PanelTools panel_tools_functions = {
  alignLabels, alignLabelsArray, alignWith, alignWithArray, centerUnder, centerUnderArray,
  putUnder, putUnderArray, putOnLineWith, putOnLineWithArray, rightAlign, rightAlignArray,
  rightAlignWithPanel, rightAlignWithPanelArray, bottomAlign, bottomAlignArray, putOver, putOverArray,
  nudge, nudgeArray
};


// alignLabels():
int alignLabels( LWControlID reference, LWControlID control, int padding ) {
  return (PanelTools::AlignLabels( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding) ? 1 : 0);
}

// alignLabelsArray():
int alignLabelsArray( LWControlID *array, int padding ) {
  return (PanelTools::AlignLabels( (LWControl **)array, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// alignWith():
int alignWith( LWControlID reference, LWControlID control ) {
  return (PanelTools::AlignWith( (LWControl *)reference, (LWControl *)control ) ? 1 : 0);
}

// alignWithArray():
int alignWithArray( LWControlID *array ) {
  return (PanelTools::AlignWith( (LWControl **)array ) ? 1 : 0);
}

// centerUnder():
int centerUnder( LWControlID reference, LWControlID control, int padding, int put_under ) {
  return (PanelTools::CenterUnder( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding, ((putUnder == 0) ? false : true) ) ? 1 : 0);
}

// centerUnderArray():
int centerUnderArray( LWControlID *array, int padding ) {
  return (PanelTools::CenterUnder( (LWControl **)array, ((padding == 0) ? PT_DEFAULT_PADDING : padding) ) ? 1 : 0);
}

// putUnder():
int putUnder( LWControlID reference, LWControlID control, int padding ) {
  return (PanelTools::PutUnder( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// putUnderArray():
int putUnderArray( LWControlID *array, int padding ) {
  return (PanelTools::PutUnder( (LWControl **)array, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// putOnLineWith():
int putOnLineWith( LWControlID reference, LWControlID control, int padding ) {
  return (PanelTools::PutOnLineWith( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// putOnLineWithArray():
int putOnLineWithArray( LWControlID *array, int padding ) {
  return (PanelTools::PutOnLineWith( (LWControl **)array, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// rightAlign():
int rightAlign( LWControlID reference, LWControlID control, int h_padding, int put_under, int v_padding ) {
  return (PanelTools::RightAlign( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_X_PADDING + h_padding, ((put_under == 0) ? false : true), PT_DEFAULT_PADDING + v_padding ) ? 1 : 0);
}

// rightAlignArray():
int rightAlignArray( LWControlID *array, int h_padding, int put_under, int v_padding ) {
  return (PanelTools::RightAlign( (LWControl **)array, PT_DEFAULT_X_PADDING + h_padding, ((put_under == 0) ? false : true), PT_DEFAULT_PADDING + v_padding ) ? 1 : 0);
}

// rightAlignWithPanel():
int rightAlignWithPanel( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControlID control, int padding ) {
  return (PanelTools::RightAlignWithPanel( panel_funcs, panel, (LWControl *)control, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// rightAlignWithPanelArray():
int rightAlignWithPanelArray( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControlID *array, int h_padding, int put_under, int v_padding ) {
  return (PanelTools::RightAlignWithPanel( panel_funcs, panel, (LWControl **)array, PT_DEFAULT_X_PADDING + h_padding, ((put_under == 0) ? false : true), PT_DEFAULT_PADDING + v_padding ) ? 1 : 0);
}

// bottomAlign():
int bottomAlign( LWControlID reference, LWControlID control, int padding ) {
  return (PanelTools::BottomAlign( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// bottomAlignArray():
int bottomAlignArray( LWControlID *array, int padding ) {
  return (PanelTools::BottomAlign( (LWControl **)array, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// putOver():
int putOver( LWControlID reference, LWControlID control, int padding ) {
  return (PanelTools::PutOver( (LWControl *)reference, (LWControl *)control, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// putOverArray():
int putOverArray( LWControlID *array, int padding ) {
  return (PanelTools::PutOver( (LWControl **)array, PT_DEFAULT_PADDING + padding ) ? 1 : 0);
}

// nudge():
int nudge( LWControlID control, int x, int y ) {
  return (PanelTools::Nudge( (LWControl *)control, x, y ) ? 1 : 0);
}

// nudgeArray():
int nudgeArray( LWControlID *array, int x, int y ) {
  return (PanelTools::Nudge( (LWControl **)array, x, y ) ? 1 : 0);
}
