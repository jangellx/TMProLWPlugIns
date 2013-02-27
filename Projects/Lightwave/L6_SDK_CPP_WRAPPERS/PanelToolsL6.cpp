//
// PanelToolsL6.cpp
//

#include <assert.h>
#include <stdlib.h>

#include "PanelToolsL6.h"

// Default Valuse
PanelVersions PanelTools::panel_version =  PANELVER_NONE;

int PanelTools::PT_DEFAULT_PADDING   =   3;
int PanelTools::PT_DEFAULT_X_PADDING =   0;
int PanelTools::PT_BASE_PADDING      = -12;

// These values are required by the macros in lwpanels.h
extern LWPanControlDesc   desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

// Init():
//  Initializes PanelTools based on the verision of LW, which
//   is obtained through the provided global function pointer.
void PanelTools::Init( GlobalFunc *global ) {
  // Get the version of Layout/Modeler
	unsigned long prodinfo = (unsigned long)(*global)( "Product Info", GFUSE_TRANSIENT);
  int major = LWINF_GETMAJOR( prodinfo );

  if( (prodinfo & LWINF_PRODUCT) == LWINF_PRODLWAV ) {           // Lightwave
    if( major < 6 )
      panel_version = PANELVER_56;
    else
      panel_version = PANELVER_L6;
  } else if( (prodinfo & LWINF_PRODUCT) == LWINF_PRODINSP3D ) {  // Inspire
    if( major < 2 )
      panel_version = PANELVER_56;
    else
      panel_version = PANELVER_L6;
  }

  if( panel_version == PANELVER_56 ) {
    // Pre-L6 Panels
    panel_version = PANELVER_56;
    PT_DEFAULT_PADDING   =   3;
    PT_DEFAULT_X_PADDING =   0;
    PT_BASE_PADDING      = -12;
  } else {
    // L6 Panels
    panel_version = PANELVER_L6;
    PT_DEFAULT_PADDING   =   3;
    PT_DEFAULT_X_PADDING =   0;
    PT_BASE_PADDING      =   0;
  }
}

// AlignLabels( LWControl *, LWControl *)
//  Aligns the label of the second control with the first, and
//  positions the second control directly under the first one
//  with the given amount of padding (in pixels) between.
//  The X position of the Reference is always used for final
//  positioning.
bool PanelTools::AlignLabels( LWControl *reference,    // Control who's width we're matching
                              LWControl *control,      // Control who's width will be changed
                              int padding ) {          // Spacing between the controls
  assert( panel_version != PANELVER_NONE );

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
  assert( panel_version != PANELVER_NONE );

  if( !con_array[0] )
    return false;

  int i = 0;
  int widest = 0;
  int w;
  // Find the widest label width
  while( con_array[i] != NULL ) {
    w = CON_LW( con_array[i] );
    i++;
    if( w > widest )
      widest = w;
  }

  i = 0;
  int x = CON_X( con_array[0] );
  int lw;
  // Move all the controls to match the widest width
  while( con_array[i] ) {
    lw = CON_LW( con_array[i] );
    MOVE_CON( con_array[i], x+widest-lw, CON_Y( con_array[i] ) );
    i++;
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
  assert( panel_version != PANELVER_NONE );

  if( !reference || !control )
    return false;

  int y = CON_Y( reference );
  int h = CON_H( reference );
  int x = CON_X( control );

  MOVE_CON( control, x,
            y + h + PT_BASE_PADDING + padding );

  return true;
}

// PutUnder( LWControl ** )
//  Same as PutUnder( LWControl *, LWControl *), but puts each control in the control
//  array under the control in the index before it.  The array passed in must
//  end with NULL.
bool PanelTools::PutUnder( LWControl **con_array, int padding ) {
  assert( panel_version != PANELVER_NONE );

  if( !con_array[0] )
    return false;

  int i = 1;
  while( con_array[i] != NULL ) {
    PutUnder( con_array[i-1], con_array[i], padding );
    i++;
  }

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
  assert( panel_version != PANELVER_NONE );

  if( !reference || !control )
    return false;

  int rw = CON_W( reference );
  int cw = CON_W( control );
  int cy = CON_Y( control );
  int x = (abs(rw-cw)/2);

  MOVE_CON( control, x, cy );
  if( put_under )
    PutUnder( reference, control, padding );

  return true;
}

// CenterUnder( LWControl ** )
//  Same as CenterUnder( LWControl *, LWControl *), but centers each control
//  in the control array under the control in the index before it.  The array
//  passed in must end with NULL.
bool PanelTools::CenterUnder( LWControl **con_array, int padding ) {
  assert( panel_version != PANELVER_NONE );

  if( !con_array[0] )
    return false;

  int i = 1;
  while( con_array[i] ) {
    CenterUnder( con_array[0], con_array[i+1], padding, false );
    PutUnder( con_array[i-1], con_array[i+1], padding );
    i++;
  }

  return true;
}

// AlignWith( LWControl ** )
//  Aligns a control with another control.
bool PanelTools::AlignWith( LWControl *reference,    // Control who's X position we're matching
                            LWControl *control ) {   // Control who's X position will be changed
  assert( panel_version != PANELVER_NONE );

  if( !reference || !control )
    return false;

  int rx = CON_X( reference );
  int cy = CON_Y( control );

  MOVE_CON( control, rx, cy );

  return true;
}

// AlignWith( LWControl ** )
//  Same as AlignWith( LWControl *, LWControl *), but centers each control
//  in the control array under the first control in the array.  The array
//  passed in must end with NULL.
bool PanelTools::AlignWith( LWControl **con_array ) {
  assert( panel_version != PANELVER_NONE );

  if( !con_array[0] )
    return false;

  int i = 1;
  while( con_array[i] )
    AlignWith( con_array[0], con_array[i++] );

  return true;
}

// PutOnLineWith( LWControl *, LWControl * )
//   Puts the control on the same line as the reference
bool PanelTools::PutOnLineWith( LWControl *reference,    // Control who's X position we're matching
                                LWControl *control,      // Control who's X position will be changed
                                int padding ) {          // Amount of Horizontal Padding
  assert( panel_version != PANELVER_NONE );

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
  assert( panel_version != PANELVER_NONE );

  if( !con_array[0] )
    return false;

  int i = 1;
  while( con_array[i] )
    PutOnLineWith( con_array[i-1], con_array[i++], padding );

  return true;
}
