
// PanelsTools.cpp

#include <stdlib.h>
#include "PanelTools.h"

// These values are required by the macros in lwpanels.h
extern LWPanControlDesc   desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;


// AlignLabels( LWControl *, LWControl *)
//  Aligns the label of the second control with the first, and
//  positions the second control directly under the first one
//  with the given amount of padding (in pixels) between.
//  The X position of the Reference is always used for final
//  positioning.
bool PanelTools::AlignLabels( LWControl *reference,    // Control who's width we're matching
                              LWControl *control,      // Control who's width will be changed
                              int padding ) {          // Spacing between the controls

  if( !reference || !control )
    return false;

  // Align Labels
  int x = CON_X( reference );
  int r = CON_LW( reference );
  int c = CON_LW( control );
  if( r > c ) {
    x += CON_LW( reference );
    x -= CON_LW( control );
    MOVE_CON( control, x, CON_Y( control ) );
  } else {
    MOVE_CON( control, x, CON_Y( control ) );   // Move to control to the reference's position first
    x += CON_LW( control );
    x -= CON_LW( reference );
    MOVE_CON( reference, x, CON_Y( reference ) );
  }

  // Position the second control under the first
  PutUnder( reference, control, padding );

  return true;
}

// AlignLabels( LWControl ** )
//  Same as AlignLabels( LWControl *, LWControl *), but aligns each control
//  in the control array under the control in the index before it.  The
//  array passed in must end with NULL.  The widest label width is used
//  as the alignment point.
bool PanelTools::AlignLabels( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  int widest = 0;
  int w;
  // Find the widest label width
  for( int i=0; con_array[i] != NULL; i++ ) {
    w = CON_LW( con_array[i] );
    if( w > widest )
      widest = w;
  }

  int x = CON_X( con_array[0] );
  int lw;
  // Move all the controls to match the widest width
  for( i=0; con_array[i] != NULL; i++ ) {
    lw = CON_LW( con_array[i] );
    MOVE_CON( con_array[i], x+widest-lw, CON_Y( con_array[i] ) );
  }

  PutUnder( con_array, padding );

  return true;
}

// PutUnder( LWControl *, LWControl *)
//  Puts the control under the reference with the given amount
//  of padding between them.  Note that this function does not
//  adjust in the X position in any way.
bool PanelTools::PutUnder( LWControl *reference,    // Control who's position we're matching
                           LWControl *control,      // Control who's position will be changed
                           int padding ) {          // Spacing between the controls
  if( !reference || !control )
    return false;

  int y = CON_Y( reference );
  int h = CON_H( reference );
  int x = CON_X( control );

  MOVE_CON( control, x, y + h + PT_BASE_PADDING + padding );

  return true;
}

// PutUnder( LWControl ** )
//  Same as PutUnder( LWControl *, LWControl *), but puts each control in the control
//  array under the control in the index before it.  The array passed in must
//  end with NULL.
bool PanelTools::PutUnder( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    PutUnder( con_array[i-1], con_array[i], padding );

  return true;
}

// CenterUnder( LWControl *reference, LWControl *control )
//  Centers the control horizontally under the reference,
//  with the given amount of vertical padding.
// This probably only works if the reference is wider than
//  the control.
bool PanelTools::CenterUnder( LWControl *reference,    // Control who's position we're matching
                              LWControl *control,      // Control who's position will be changed
                              int padding,             // Spacing between the controls
                              bool put_under ) {       // If the control should be positioned under the reference or not
  if( !reference || !control )
    return false;

  int rx = CON_X( reference );
  int rw = CON_W( reference );
  int cw = CON_W( control );
  int cy = CON_Y( control );
  int x  = (rw/2)-(cw/2);

  MOVE_CON( control, rx + x, cy );
  if( put_under )
    PutUnder( reference, control, padding );

  return true;
}

// CenterUnder( LWControl ** )
//  Same as CenterUnder( LWControl *, LWControl *), but centers each control
//  in the control array under the control in the index before it.  The array
//  passed in must end with NULL.
bool PanelTools::CenterUnder( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    CenterUnder( con_array[i-1], con_array[i], padding, true );

  return true;
}

// AlignWith( LWControl ** )
//  Aligns a control with another control.
bool PanelTools::AlignWith( LWControl *reference,                 // Control who's X position we're matching
                            LWControl *control, int padding ) {   // Control who's X position will be changed and Padding
  if( !reference || !control )
    return false;

  int rx = CON_X( reference );
  int cy = CON_Y( control );

  MOVE_CON( control, rx, cy );
  PutUnder( reference, control, padding );

  return true;
}

// AlignWith( LWControl ** )
//  Same as AlignWith( LWControl *, LWControl *), but centers each control
//  in the control array under the first control in the array.  The array
//  passed in must end with NULL.
bool PanelTools::AlignWith( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    AlignWith( con_array[0], con_array[i] );

  PutUnder( con_array, padding );

  return true;
}

// PutOnLineWith( LWControl *, LWControl * )
//   Puts the control on the same line as the reference
bool PanelTools::PutOnLineWith( LWControl *reference,    // Control who's X position we're matching
                                LWControl *control,      // Control who's X position will be changed
                                int padding ) {          // Amount of Horizontal Padding
  if( !reference || !control )
    return false;

  int rx = CON_X( reference );
  int rw = CON_W( reference );
  int cx = rx + rw + padding;
  int cy = CON_Y( reference );

  MOVE_CON( control, cx, cy );

  return true;
}

// PutOnLineWith( LWControl ** )
//  Same as PutOnLineWith( LWControl *, LWControl *), but puts each control
//  in the control array next the the previous control in the array.  The
//  array passed in must end with NULL.
bool PanelTools::PutOnLineWith( LWControl **con_array,
                                int padding ) {
  if( !con_array[0] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    PutOnLineWith( con_array[i-1], con_array[i], padding );

  return true;
}

// RightAlign():
bool PanelTools::RightAlign( LWControl *reference, LWControl *control, int h_padding, bool put_under, int v_padding ) {
  if( reference == NULL )
    return false;

  if( control == NULL )
    return false;

  int cw = CON_W( control );

  int rx = CON_X( reference );
  int rw = CON_W( reference );

  int cy = CON_Y( control );

  MOVE_CON( control, (rx + rw - cw) - h_padding, cy );

  if( put_under )
    PutUnder( reference, control, v_padding );

  return true;
}

// RightAlign():
bool PanelTools::RightAlign( LWControl **con_array, int h_padding, bool put_under, int v_padding ) {
  if( !con_array[0] )
    return false;

  if( !con_array[1] )
    return false;

  int widest = 0;
  int w, x;
  // Find the widest control
  for( int i=0; con_array[i] != NULL; i++ ) {
    w = CON_W( con_array[i] );
    if( w > widest ) {
      widest = w;
      x = CON_X( con_array[i] );
    }
  }

  int cw, cy;
  // Move all the controls to match the widest width
  for( i=0; con_array[i] != NULL; i++ ) {
    cw = CON_W( con_array[i] );
    cy = CON_Y( con_array[i] );

    MOVE_CON( con_array[i], (x + widest - cw) - h_padding, cy );
  }

  if( put_under )
    PutUnder( con_array, v_padding );

  return true;
}

// RightAlignWithPanel():
bool PanelTools::RightAlignWithPanel( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControl *control, int padding ) {
  if( panel == NULL )
    return false;

  if( panel_funcs == NULL ) 
    return false;

  int y = CON_Y( control );
  int w = CON_W( control );
  int pw = PAN_GETW( panel_funcs, panel );
  MOVE_CON( control, ((pw - w) - padding), y );

  return true;
}

// RightAlignWithPanel():
bool PanelTools::RightAlignWithPanel( LWPanelFuncs *panel_funcs, LWPanelID panel, LWControl **con_array,
                                      int h_padding, bool put_under, int v_padding ) {
  if( panel == NULL )
    return false;

  if( panel_funcs == NULL ) 
    return false;

  for( int i=0; con_array[i] != NULL; i++ )
    RightAlignWithPanel( panel_funcs, panel, con_array[i], h_padding );

  PutUnder( con_array, v_padding );

  return true;
}

// BottomAlign():
//  Like PutOnLineWith(), but aligns to the bottom of the reference control instead of the top
bool PanelTools::BottomAlign( LWControl *reference, LWControl *control, int padding ) {
  if( !reference || !control )
    return false;

  int rx = CON_X( reference );
  int rw = CON_W( reference );
  int cx = rx + rw + padding;

  int ry = CON_Y( reference );
  int rh = CON_H( reference );
  int ch = CON_H( control );

  MOVE_CON( control, cx, ( (ry+rh) - ch) );

  return true;
}

// BottomAlign():
//  Like PutOnLineWith(), but aligns to the bottom of the reference control instead of the top
bool PanelTools::BottomAlign( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  if( !con_array[1] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    BottomAlign( con_array[i-1], con_array[i], padding );

  return true;
}

// PutOver( LWControl *, LWControl *)
bool PanelTools::PutOver( LWControl *reference,    // Control who's position we're matching
                          LWControl *control,      // Control who's position will be changed
                          int padding ) {          // Spacing between the controls
  if( !reference || !control )
    return false;

  int ry = CON_Y( reference );
  int ch = CON_H( control );
  int cx = CON_X( control );

  MOVE_CON( control, cx, ry - ch - PT_BASE_PADDING - padding );

  return true;
}

// PutOver( LWControl ** )
bool PanelTools::PutOver( LWControl **con_array, int padding ) {
  if( !con_array[0] )
    return false;

  for( int i=1; con_array[i] != NULL; i++ )
    PutOver( con_array[i-1], con_array[i], padding );

  return true;
}

// Nudge( LWControl *, LWControl *)
bool PanelTools::Nudge( LWControl *control,      // Control who's position we're matching
                        int x, int y ) {         // Nudge amount
  if( !control )
    return false;

  int cx = CON_X( control );
  int cy = CON_Y( control );

  MOVE_CON( control, cx + x, cy + y );

  return true;
}

// Nudge( LWControl ** )
bool PanelTools::Nudge( LWControl **con_array, int x, int y ) {
  if( !con_array[0] )
    return false;

  for( int i=0; con_array[i] != NULL; i++ )
    Nudge( con_array[i], x, y );

  return true;
}
