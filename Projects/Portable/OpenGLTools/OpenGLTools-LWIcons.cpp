//
// OpenGLTools-LWIcons.h
//

#include "OpenGLTools-LWIcons.h"

int GLTLWIcons::legacy_offset = 0;

// HorizontalDivider():
//  Draws a horizontal divider centered at that y specified and left
//   aligned at the x specified with the given thickness.  Minimum
//   thickness is 3 pixels.
bool GLTLWIcons::HorizontalDivider( int x, int y, int width,
                                    int thickness ) {
  int line_thickness = thickness/3;
  if( line_thickness < 1 )
    line_thickness = 1;

  glColor3ub( 100, 100, 100 );
  glRecti( x+1, y-1, x+width-2, y+line_thickness );

  glColor3ub( 0, 0, 0 );
  glRecti( x, y, x+width, y+line_thickness );

  glColor3ub( 200, 200, 200 );
  glRecti( x+1, y+1, x+width-2, y+line_thickness );

  return true;
}

// VerticalDivider():
//  Draws a vertical divider centered at that x specified and top
//   aligned at the y specified with the given thickness.  Minimum
//   thickness is 3 pixels.
bool GLTLWIcons::VerticalDivider( int x, int y, int height,
                                  int thickness ) {
  int line_thickness = thickness/3;
  if( line_thickness < 1 )
    line_thickness = 1;

  glColor3ub( 100, 100, 100 );
  glRecti( x-1, y+1, x+line_thickness, y+height-2 );

  glColor3ub( 0, 0, 0 );
  glRecti( x, y, x+line_thickness, y+height );

  glColor3ub( 200, 200, 200 );
  glRecti( x+1, y+1, x+line_thickness, y+height-2 );

  return true;
}

// DrawFoldIcon():
//  Draws an fold icon if folded is true, and an unfold icon
//   if false at the given X and Y coordinates at the given
//   width and height.  The icon is not anti-aliased.   
//  If type is triangle, then a triangular, Mac-like fold
//   icon will be draw (pointing down when unfolded, and
//   pointing right when folded).  If it is plus_minus,
//   then a - is drawn if the unfolded, and a + if folded.
//   The later is similar to how Lightwave draws it's
//   hierarchy list.
//  The triangle mode is similar in shape and color to the
//   L6 Item Fold Icon.  Note that w and h are ignored for
//   this type.
void GLTLWIcons::DrawFoldIcon( int x, int y, int w, int h,
                               bool folded, enum PanelDraw::FoldedType type ) {
  if( (w < 1) || (h < 1) )
    return;

  // Triangle
  if( type == PanelDraw::triangle ) {
    if( folded )
      DrawArrow( x+5, y+6, PanelDraw::arrow_right_narrow, PanelDraw::arrow_solid, 200, 200, 200 );
    else
      DrawArrow( x+5, y+6, PanelDraw::arrow_down_narrow, PanelDraw::arrow_solid, 200, 200, 200 );
  }
  
  // Plus/Minus
  if( type == PanelDraw::plus_minus ) {
    glColor3ub( 0,  0,  0 );
    glRecti( x + (int)(w*0.2)+1, y + (h/2), x + + (int)(w*0.2)+1 + (int)(w * 0.6), y + (h/2) + 1 );     // Horiz Line

    if( folded )
      glRecti( x + (w/2), y + (int)(h*0.2)+1, x + (w/2) + 1, y + (int)(h*0.2)+1 + (int)(h * 0.6) );   // Vert Line
  }
}

// GetLWColor():
//  Fills in an array with the RGB color values matching the ItemColor
//   type passed in.  These correlate to the ShowObject colors in
//   Layout.
IMG_BYTE *GLTLWIcons::GetLWColor( IMG_BYTE *rgb, PanelDraw::ItemColor color ) {
  return PanelDraw::GetLWColor( rgb, color );
}

// DrawLWIcon():
//  Draws a Lightwave icon into the interface.  The icon drawn is
//   determined by the type passed in.  Icons are 13x11 pixels,
//   drawn in the color specified.
void GLTLWIcons::DrawLWIcon( int x, int y, enum PanelDraw::IconType type,
                             int r, int g, int b, PanelDraw::IconMode mode ) {
  glColor3ub( r, g, b );

  switch( type ) {
  case PanelDraw::object:
    if( mode == PanelDraw::draw_56 ) {
      glRecti( x+6, y+3, x+ 9, y+4 );
      glRecti( x+5, y+4, x+10, y+7 );
      glRecti( x+6, y+7, x+ 9, y+8 );
    } else {
      glRecti( x+ 5, y+3, x+10, y+ 4 );
      glRecti( x+10, y+4, x+11, y+ 9 );
      glRecti( x+ 4, y+5, x+ 9, y+10 );

      glColor3ub( 0, 0, 0 );
      glRecti( x+ 5, y+ 2, x+12, y+ 3 );
      glRecti( x+ 4, y+ 4, x+ 9, y+ 5 );
      glRecti( x+ 3, y+ 4, x+ 4, y+11 );
      glRecti( x+ 4, y+10, x+ 9, y+11 );
      glRecti( x+ 9, y+ 4, x+10, y+11 );
      glRecti( x+11, y+ 3, x+12, y+10 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 4, y+  4 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+  4 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+ 10 + legacy_offset + legacy_offset );
      glEnd();
    }
    break;

  case PanelDraw::light:
    if( mode == PanelDraw::draw_56 ) {
      glRecti( x+5, y+4, x+10, y+7 );

      glBegin( GL_POINTS );
      glVertex2i( x+11, y+3 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+4 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+8 + legacy_offset + legacy_offset );
      glVertex2i( x+11, y+9 + legacy_offset + legacy_offset );
      glEnd();
    } else {
      glRecti( x+ 4, y+ 5, x+ 9, y+8 );

      glColor3ub( 0, 0, 0 );
      glRecti( x+ 4, y+ 4, x+ 9, y+5 );
      glRecti( x+ 3, y+ 4, x+ 4, y+9 );
      glRecti( x+ 9, y+ 4, x+10, y+9 );
      glRecti( x+ 4, y+ 8, x+ 9, y+9 );

      glBegin( GL_POINTS );
      glVertex2i( x+11, y+ 3 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+ 4 + legacy_offset + legacy_offset );
      glVertex2i( x+10, y+10 + legacy_offset + legacy_offset );
      glVertex2i( x+11, y+11 + legacy_offset + legacy_offset );
      glEnd();
    }
    break;

  case PanelDraw::camera:
    if( mode == PanelDraw::draw_56 ) {
      glRecti( x+ 4, y+3, x+ 7, y+4 );
      glRecti( x+ 8, y+3, x+11, y+4 );
      glRecti( x+ 5, y+5, x+10, y+8 );
      glRecti( x+10, y+6, x+12, y+7 );

      glBegin( GL_POINTS );
      glVertex2i( x+5, y+3 + legacy_offset + legacy_offset );
      glVertex2i( x+9, y+3 + legacy_offset + legacy_offset );
      glVertex2i( x+5, y+5 + legacy_offset + legacy_offset );
      glVertex2i( x+9, y+5 + legacy_offset + legacy_offset );
      glEnd();
    } else {
      glRecti( x+ 4, y+5, x+ 7, y+8 );
      glRecti( x+ 6, y+5, x+ 9, y+8 );
      glRecti( x+ 5, y+6, x+10, y+9 );

      glColor3ub( 0, 0, 0 );
      glRecti( x+ 4, y+4,  x+ 7, y+ 5 );
      glRecti( x+ 8, y+4,  x+11, y+ 5 );
      glRecti( x+ 3, y+5,  x+ 6, y+ 6 );
      glRecti( x+11, y+3,  x+14, y+ 4 );
      glRecti( x+ 4, y+ 6, x+11, y+ 7 );
      glRecti( x+ 4, y+10, x+11, y+11 );
      glRecti( x+ 4, y+ 6, x+ 5, y+11 );
      glRecti( x+ 4, y+10, x+ 5, y+15 );
      glRecti( x+11, y+8,  x+13, y+ 9 );
      glRecti( x+13, y+7,  x+14, y+10 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 5, y+7 + legacy_offset + legacy_offset );
      glEnd();
    }
    break;

  case PanelDraw::bone:
    if( mode == PanelDraw::draw_56 ) {
      glRecti( x+5, y+5, x+10, y+6 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 4, y+5 + legacy_offset );
      glVertex2i( x+10, y+5 + legacy_offset );
      glVertex2i( x+ 4, y+7 + legacy_offset );
      glVertex2i( x+10, y+7 + legacy_offset );
      glEnd();
    } else {
      glRecti( x+ 4, y+ 5, x+ 7, y+ 6 );
      glRecti( x+ 9, y+ 9, x+12, y+10 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 5, y+ 5 + legacy_offset );
      glVertex2i( x+ 6, y+ 7 + legacy_offset );
      glVertex2i( x+ 7, y+ 8 + legacy_offset );
      glVertex2i( x+ 8, y+ 9 + legacy_offset );
      glVertex2i( x+ 9, y+11 + legacy_offset );
      glEnd();

      glColor3ub( 0, 0, 0 );
      glRecti( x+ 6, y+4, x+ 7, y+6 );
      glRecti( x+ 4, y+6, x+ 6, y+7 );
      glRecti( x+ 9, y+8, x+11, y+9 );
      glRecti( x+ 8, y+9, x+ 9, y+11 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 5, y+4 + legacy_offset  );
      glVertex2i( x+ 4, y+5 + legacy_offset  );
      glVertex2i( x+ 3, y+6 + legacy_offset  );
      glVertex2i( x+ 7, y+7 + legacy_offset  );
      glVertex2i( x+ 6, y+8 + legacy_offset  );
      glVertex2i( x+ 8, y+8 + legacy_offset  );
      glVertex2i( x+ 7, y+9 + legacy_offset  );
      glVertex2i( x+ 9, y+12 + legacy_offset );
      glVertex2i( x+10, y+11 + legacy_offset );
      glVertex2i( x+11, y+10 + legacy_offset );
      glEnd();
    }
    break;

  case PanelDraw::check:
    if( mode == PanelDraw::draw_56 ) {  // Actual check mark
      glRecti( x+10, y+4, x+12, y+ 5 );
      glRecti( x+ 9, y+5, x+11, y+ 6 );
      glRecti( x+ 2, y+6, x+ 4, y+ 7 );
      glRecti( x+ 8, y+6, x+10, y+ 7 );
      glRecti( x+ 7, y+7, x+ 9, y+ 8 );
      glRecti( x+ 2, y+7, x+ 5, y+ 8 );
      glRecti( x+ 3, y+8, x+ 8, y+ 9 );
      glRecti( x+ 4, y+9, x+ 7, y+10 );

      glBegin( GL_POINTS );
      glVertex2i( x+11, y+ 4 + legacy_offset );
      glVertex2i( x+ 5, y+11 + legacy_offset );
      glEnd();
    } else {                 // A dot
      glRecti( x+5, y+3, x+8, y+4 );
      glRecti( x+4, y+4, x+9, y+7 );
      glRecti( x+5, y+7, x+8, y+8 );
    }
    break;

  case PanelDraw::draw_bounding_box:
    if( mode == PanelDraw::draw_56 ) {
      GLTDraw2D::EmptyBox( x+3, y+2, 7, 7, 1, 1,
                           r, g, b,  r, g, b );
    } else {
      GLTDraw2D::EmptyBox( x+3, y+2, 7, 7, 1, 1,
                           r, g, b,  r, g, b );
    }
    break;

  case PanelDraw::draw_every_other_solid:
    // Not supprted in L6; use the same draw routine for both
    glRecti( x+3, y+2, x+ 7, y+6 );
    glRecti( x+6, y+5, x+10, y+9 );
    break;

  case PanelDraw::draw_every_other_wire:
    // Not supprted in L6; use the same draw routine for both
    GLTDraw2D::EmptyBox( x+3, y+2, 4, 4, 1, 1,
              r, g, b,  r, g, b );
    GLTDraw2D::EmptyBox( x+6, y+5, 4, 4, 1, 1,
              r, g, b,  r, g, b );
    break;

  case PanelDraw::draw_front_face:
    if( mode == PanelDraw::draw_56 ) {
      GLTDraw2D::EmptyBox( x+3, y+2, 7, 7, 1, 1,
                r, g, b,  r, g, b );

      glBegin( GL_POINTS );
      glVertex2i( x+5, y+4 + legacy_offset );
      glVertex2i( x+7, y+4 + legacy_offset );
      glVertex2i( x+9, y+4 + legacy_offset );

      glVertex2i( x+4, y+5 + legacy_offset );
      glVertex2i( x+6, y+5 + legacy_offset );
      glVertex2i( x+8, y+5 + legacy_offset );

      glVertex2i( x+5, y+6 + legacy_offset );
      glVertex2i( x+7, y+6 + legacy_offset );
      glVertex2i( x+9, y+6 + legacy_offset );

      glVertex2i( x+4, y+7 + legacy_offset );
      glVertex2i( x+6, y+7 + legacy_offset );
      glVertex2i( x+8, y+7 + legacy_offset );

      glVertex2i( x+5, y+8 + legacy_offset );
      glVertex2i( x+7, y+8 + legacy_offset );
      glVertex2i( x+9, y+8 + legacy_offset );
      glEnd();
    } else {
      GLTDraw2D::EmptyBox( x+2, y+1, 9, 9, 1, 1,
                           r, g, b,  r, g, b );
      glRecti( x+7, y+2, x+ 8, x+9 );
      glRecti( x+7, y+5, x+14, x+6 );
    }
    break;

  case PanelDraw::draw_points_only:
    if( mode == PanelDraw::draw_56 ) {
      glBegin( GL_POINTS );
      glVertex2i( x+4, y+3 + legacy_offset );
      glVertex2i( x+6, y+3 + legacy_offset );
      glVertex2i( x+8, y+3 + legacy_offset );

      glVertex2i( x+4, y+5 + legacy_offset );
      glVertex2i( x+6, y+5  + legacy_offset);
      glVertex2i( x+8, y+5 + legacy_offset );

      glVertex2i( x+4, y+7 + legacy_offset );
      glVertex2i( x+6, y+7 + legacy_offset );
      glVertex2i( x+8, y+7 + legacy_offset );
      glEnd();
    } else {
      glVertex2i( x+ 2, y+3 + legacy_offset );
      glVertex2i( x+ 6, y+3 + legacy_offset );
      glVertex2i( x+10, y+3 + legacy_offset );

      glVertex2i( x+ 4, y+5 + legacy_offset );
      glVertex2i( x+ 8, y+5 + legacy_offset );

      glVertex2i( x+ 2, y+7 + legacy_offset );
      glVertex2i( x+ 6, y+7 + legacy_offset );
      glVertex2i( x+10, y+7 + legacy_offset );

      glVertex2i( x+ 4, y+9 + legacy_offset );
      glVertex2i( x+ 8, y+9 + legacy_offset );

      glVertex2i( x+ 2, y+11 + legacy_offset );
      glVertex2i( x+ 6, y+11 + legacy_offset );
      glVertex2i( x+10, y+11 + legacy_offset );
    }
    break;

  case PanelDraw::draw_solid:
    if( mode == PanelDraw::draw_56 )
      glRecti( x+3, y+2, x+10, y+ 9 );
    else
      glRecti( x+2, y+1, x+11, y+10 );
    break;

  case PanelDraw::draw_textured:
    if( mode == PanelDraw::draw_56 ) {
      glRecti( x+ 3, y+2, x+10, y+9 );

      glColor3ub( 0, 0, 0 );
      glRecti( x+ 4, y+3, x+ 9, y+4 );
      glRecti( x+ 6, y+3, x+ 7, y+8 );
    } else {
      glRecti( x+ 2, y+1, x+11, y+10 );

      glColor3ub( 128, 128, 128 );
      glRecti( x+ 4, y+3, x+ 9, y+ 4 );
      glRecti( x+ 6, y+3, x+ 7, y+ 8 );
    }
    break;

  case PanelDraw::draw_wireframe:
    if( mode == PanelDraw::draw_56 ) {
      GLTDraw2D::EmptyBox( x+3, y+2, 7, 7, 1, 1,
                           r, g, b,  r, g, b );
      glRecti( x+ 6, y+3, x+ 7, y+ 8 );
      glRecti( x+ 4, y+5, x+ 9, y+ 6 );
    } else {
      GLTDraw2D::EmptyBox( x+2, y+1, 9, 9, 1, 1,
                           r, g, b,  r, g, b );

      glRecti( x+7, y+2, x+ 8, x+14 );
      glRecti( x+7, y+5, y+14, y+ 6 );

      glBegin( GL_POINTS );
      glVertex2i( x+ 3, y+ 3 + legacy_offset );
      glVertex2i( x+ 4, y+ 4 + legacy_offset );
      glVertex2i( x+ 5, y+ 5 + legacy_offset );
      glVertex2i( x+ 7, y+ 5 + legacy_offset );
      glVertex2i( x+ 8, y+ 4 + legacy_offset );
      glVertex2i( x+ 9, y+ 3 + legacy_offset );

      glVertex2i( x+ 3, y+ 7 + legacy_offset );
      glVertex2i( x+ 4, y+ 8 + legacy_offset );
      glVertex2i( x+ 5, y+ 9 + legacy_offset );
      glVertex2i( x+ 7, y+ 9 + legacy_offset );
      glVertex2i( x+ 8, y+ 8 + legacy_offset );
      glVertex2i( x+ 9, y+ 7 + legacy_offset );
      glEnd();
    }
    break;

  case PanelDraw::lock:
    // Not in 5.6; use the same draw routine for both
    glRecti( x+5, y+2, x+ 8, y+ 3 );
    glRecti( x+4, y+3, x+ 5, y+ 6 );
    glRecti( x+8, y+3, x+ 9, y+ 6 );
    glRecti( x+3, y+6, x+10, y+11 );
    break;

  case PanelDraw::visibility_eye:
    // Not in 5.6; use the same draw routine for both

    glRecti( x+ 1, y+4, x+ 3, y+ 5 );
    glRecti( x+ 3, y+3, x+ 9, y+ 4 );
    glRecti( x+ 9, y+4, x+11, y+ 5 );

    glRecti( x+ 2, y+6, x+ 4, y+ 7 );
    glRecti( x+ 4, y+5, x+ 5, y+ 9 );
    glRecti( x+ 5, y+5, x+ 8, y+ 6 );
    glRecti( x+ 6, y+6, x+10, y+ 7 );
    glRecti( x+ 6, y+7, x+ 8, y+ 8 );
    glRecti( x+ 5, y+8, x+ 8, y+ 9 );
    glRecti( x+ 5, y+9, x+ 7, y+10 );

    glRecti( x+ 9, y+7,  x+11, y+ 8 );
    glRecti( x+ 3, y+10, x+ 9, y+11 );

    glBegin( GL_POINTS );
    glVertex2i( x+11, y+6  + legacy_offset );
    glVertex2i( x+ 1, y+8  + legacy_offset );
    glVertex2i( x+10, y+9  + legacy_offset );
    glVertex2i( x+ 9, y+10 + legacy_offset );
    glVertex2i( x+ 2, y+10 + legacy_offset );
    glEnd();
    break;
  }
}

// DrawLWIcon():
//  Alternate DrawLWIcont() that takes an ItemColor instead
//   of RGB values
void GLTLWIcons::DrawLWIcon( int x, int y, enum PanelDraw::IconType type,
                             enum PanelDraw::ItemColor color, PanelDraw::IconMode mode ) {
  IMG_BYTE rgb[3];
  GetLWColor( rgb, color );

  DrawLWIcon( x, y, type, rgb[0], rgb[1], rgb[2], mode );
}

// DrawArrow():
//  Draws different triangular arrow shapes.  The type determines
//   the type of arrow that is drown (up, left, pointing away, etc.),
//   and the method detrmines the syle it is drawn in (solid, empty,
//   etc.)  In this case, x and y are the center of the arrow to be
//   drawn.
void GLTLWIcons::DrawArrow( int x, int y, enum PanelDraw::ArrowType type,
                            enum PanelDraw::ArrowMethod method,
                            int r, int g, int b ) {
  glColor3ub( r, g, b );
  switch( type ) {
  // Arrow Up
  case PanelDraw::arrow_up:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glBegin( GL_POINTS );
      glVertex2i( x-2, y+4 + legacy_offset );
      glEnd();

      glRecti( x-1,  y+2,  x+ 2, y+3 );
      glRecti( x,    y+1,  x+ 5, y+2 );
      glRecti( x+1,  y,    x+ 8, y+1 );
      glRecti( x+2,  y-1,  x+11, y );
      break;

    case PanelDraw::arrow_empty:
      glRecti( x-5,  y+2, x+5, y+3 );
      glRecti( x-6,  y+3, x+6, y+4 );
      // No break; we use the code below for the rest of the arrow

    case PanelDraw::arrow_two_edges:
      glRecti( x-1,  y-3, x+1, y-2 );
      glRecti( x-2,  y-2, x,   y-1 );
      glRecti( x+0,  y-2, x+2, y-1 );
      glRecti( x-3,  y-1, x-1, y   );
      glRecti( x+1,  y-1, x+3, y   );
      glRecti( x-4,  y,   x-2, y+1 );
      glRecti( x+2,  y,   x+4, y+1 );
      glRecti( x-5,  y+1, x-3, y+2 );
      glRecti( x+3,  y+1, x+5, y+2 );
      glRecti( x-6,  y+2, x-4, y+3 );
      glRecti( x+4,  y+2, x+6, y+3 );
      glRecti( x-7,  y+3, x-5, y+4 );
      glRecti( x+5,  y+3, x+7, y+4 );
      break;
    }
    break;

  // Arrow Down
  case PanelDraw::arrow_down:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glBegin( GL_POINTS );
      glVertex2i( x+2, y+4 + legacy_offset );
      glEnd();

      glRecti( x+1,  y+2,  x+4, y+3 );
      glRecti( x,    y+1,  x+5, y+2 );
      glRecti( x-1,  y,    x+6, y+1 );
      glRecti( x-2,  y-1,  x+7, y );
      break;
    }
    break;

  // Arrow Right
  case PanelDraw::arrow_right:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glBegin( GL_POINTS );
      glVertex2i( x+2, y+1 + legacy_offset );
      glEnd();

      glRecti( x+1,  y-1,  x+2,  y+2 );
      glRecti( x,    y-2,  x+1,  y+3 );
      glRecti( x-1,  y-3,  x,    y+4 );
      glRecti( x-2,  y-4,  x-1,  y+5 );
      break;
    }
    break;

  // Arrow Left
  case PanelDraw::arrow_left:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glBegin( GL_POINTS );
      glVertex2i( x-2, y+1 + legacy_offset );
      glEnd();

      glRecti( x-1,  y-1,  x,    y+2 );
      glRecti( x,    y-2,  x+1,  y+3 );
      glRecti( x+1,  y-3,  x+2,  y+4 );
      glRecti( x+2,  y-4,  x+3,  y+5 );
      break;
    }
    break;

  // Arrow Up Narrow
  case PanelDraw::arrow_up_narrow:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x+2,  y-2,  x+3, y );
      glRecti( x+1,  y,    x+4, y+2 );
      glRecti( x,    y+2,  x+5, y+4 );
      break;
    }
    break;

  // Arrow Down Narrow
  case PanelDraw::arrow_down_narrow:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x+2,  y+2,  x+3, y+4 );
      glRecti( x+1,  y,    x+4, y+2 );
      glRecti( x,    y-2,  x+5, y );
      break;
    }
    break;

  // Arrow Right
  case PanelDraw::arrow_right_narrow:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x+2,  y,    x+4, y+1 );
      glRecti( x,    y-1,  x+2, y+2 );
      glRecti( x-2,  y-2,  x,   y+3 );
      break;
    }
    break;

  // Arrow Left
  case PanelDraw::arrow_left_narrow:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x-2,  y,    x,   y+1 );
      glRecti( x,    y-1,  x+2, y+2 );
      glRecti( x+2,  y-2,  x+4, y+3 );
      break;
    }
    break;

  // Arrow Double Right
  case PanelDraw::arrow_double_right:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glBegin( GL_POINTS );
      glVertex2i( x+5, y+1 + legacy_offset );
      glVertex2i( x-1, y+1 + legacy_offset );
      glEnd();

      glRecti( x+4,  y-1,  x+5, y+2 );
      glRecti( x+3,  y-2,  x+4, y+3 );
      glRecti( x+2,  y-3,  x+3, y+4 );
      glRecti( x+1,  y-4,  x+2, y+5 );

      glRecti( x-2,  y-1,  x-1, 2 );
      glRecti( x-3,  y-2,  x-2, 3 );
      glRecti( x-4,  y-3,  x-2, 4 );
      glRecti( x-5,  y-4,  x-4, 5 );
      break;
    }
    break;

  // Arrow Away Vertical (<> rotated 90 degrees)
  case PanelDraw::arrow_away_vert:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x-1,  y-4,  x+1, y-3 );
      glRecti( x-2,  y-3,  x+2, y-2 );
      glRecti( x-3,  y-2,  x+3, y-1 );
      glRecti( x-4,  y-1,  x+4, y );

      glRecti( x-4,  y+1,  x+4, y+2 );
      glRecti( x-3,  y+2,  x+3, y+3 );
      glRecti( x-2,  y+3,  x+2, y+4 );
      glRecti( x-1,  y+4,  x+1, y+5 );
      break;

    case PanelDraw::arrow_two_edges:
      glRecti( x-1,  y-4, x+1, y-3 );
      glRecti( x-2,  y-3, x,   y-2 );
      glRecti( x+0,  y-3, x+2, y-2 );
      glRecti( x-3,  y-2, x-1, y-1 );
      glRecti( x+1,  y-2, x+3, y-1 );
      glRecti( x-4,  y-1, x-2, y );
      glRecti( x+2,  y-1, x+4, y );

      glRecti( x+2,  y+1, x+4, y+2 );
      glRecti( x-4,  y+1, x-2, y+2 );
      glRecti( x+1,  y+2, x+3, y+3 );
      glRecti( x-3,  y+2, x-1, y+3 );
      glRecti( x+0,  y+3, x+2, y+4 );
      glRecti( x-2,  y+3, x,   y+4 );
      glRecti( x-1,  y+4, x+1, y+5 );
    }
    break;

  // Arrow Towards Vertical (>< rotated 90 degrees)
  case PanelDraw::arrow_toward_vert:
    switch( method ) {
    case PanelDraw::arrow_solid:
      glRecti( x-1,  y-4,  x+4, y   );
      glRecti( x-2,  y-3,  x+3, y-1 );
      glRecti( x-3,  y-2,  x+2, y-2 );
      glRecti( x-4,  y-1,  x+1, y-3 );

      glRecti( x-4,  y+1,  x+1, y+5 );
      glRecti( x-3,  y+2,  x+2, y+4 );
      glRecti( x-2,  y+3,  x+3, y+3 );
      glRecti( x-1,  y+4,  x+4, y+2 );
      break;

    case PanelDraw::arrow_two_edges:
      glRecti( x-1,  y-1, x+1, y   );
      glRecti( x-2,  y-2, x,   y   );
      glRecti( x+0,  y-2, x+2, y-1 );
      glRecti( x-3,  y-3, x-1, y-1 );
      glRecti( x+1,  y-3, x+3, y-2 );
      glRecti( x-4,  y-4, x-2, y-2 );
      glRecti( x+2,  y-4, x+4, y-3 );

      glRecti( x+2,  y+4, x+4, y+5 );
      glRecti( x-4,  y+4, x-2, y+4);
      glRecti( x+1,  y+3, x+3, y+4 );
      glRecti( x-3,  y+3, x-1, y+3 );
      glRecti( x+0,  y+2, x+2, y+3 );
      glRecti( x-2,  y+2, x,   y+2 );
      glRecti( x-1,  y+1, x+1, y+2 );
    }
    break;
  }
}
