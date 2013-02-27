//
// FreeKeyInterface-OpenGLLister.cpp
//

#include "FreeKey-Interface.h"

//#define TEST_OPENGL_AREA

// DrawOpenGLItemList():
//  Called to draw the OpenGL Item List.
void DrawOpenGLItemListbox( LWControl *ctl, FreeKeyInterface *si, DrMode mode ) {
  fk_int->dd.Generate( ctl, true, fk->GetDrawStyle() );

  // Reset the coordinate system so zero-zero is the top-left (3rd quadrant)
  glLoadIdentity();
  glViewport( fk_int->dd.cx, fk_int->dd.truey, fk_int->dd.cw, fk_int->dd.trueh );
  glOrtho( 0.0f, fk_int->dd.cw, fk_int->dd.trueh, 0.0f, 1.0, -1.0 );

  // Clear the control
  glClearColor( ((float)fk_int->dd.bgr)/255.0f, ((float)fk_int->dd.bgg)/255.0f, ((float)fk_int->dd.bgb)/255.0f, 1.0f );
  glClear( GL_COLOR_BUFFER_BIT );

#ifdef TEST_OPENGL_AREA
  // Testing:  Draw an empty box around the perimiters
  glColor3ub( 0, 255, 0 );
  glBegin( GL_LINE_LOOP );
  glVertex2i( 1, 1 );
  glVertex2i( 1, fk_int->dd.truey-1 );
  glVertex2i( fk_int->dd.cw-1, fk_int->dd.truey-1 );
  glVertex2i( fk_int->dd.cw-1, 1 );
  glEnd();
#endif
  glFlush();

  DrawOpenGLItemList_Title( ctl );
  DrawOpenGLItemList_Names( ctl );
  DrawOpenGLItemList_LeftColumns( ctl );
  DrawOpenGLItemList_MidColumns( ctl );
  DrawOpenGLItemList_Keys( ctl );
  DrawOpenGLItemList_Icons( ctl );
}

// DrawOpenGLItemList_Title():
//  Draws the OpenGL Item List title area
void DrawOpenGLItemList_Title( LWControl *ctl ) {
  // Reset the coordinate system so zero-zero is the top-left (3rd quadrant) of the title
  glViewport( 0, 0, fk_int->dd.cw, fk_int->dd.trueh );
  glLoadIdentity();
  glOrtho( 0.0f, fk_int->dd.cw, fk_int->dd.trueh, 0.0f, 1.0, -1.0 );

  // Title Drawing
  if( fk->GetDrawStyle() == DRAWSTYLE_L6 ) {
    glColor3ub( 112, 112, 112 );
    glRecti( 0, 0, fk_int->dd.cw, 19 );
  }

  glColor3i( 0, 0, 0);
  glRecti( 0, 19, fk_int->dd.cw, 20 );

  fk_int->dd.opengl_font.Write( "Item", fk_int->dd.item_text_column + 14, 17 );

  GLTLWIcons::DrawLWIcon( fk->GetDivider() + item_visibility_offset + 2, fk_int->dd.truey+6, PanelDraw::visibility_eye, PanelDraw::black );
  GLTLWIcons::DrawLWIcon( fk->GetDivider() + item_lock_offset + 2,       fk_int->dd.truey+6, PanelDraw::lock,           PanelDraw::black );

  glFlush();

  // Draw the timeline
  glViewport( fk->GetDivider() + 1, fk_int->dd.trueh - fk_int->dd.item_title_height,
              fk_int->dd.cw - fk->GetDivider() - 1, fk_int->dd.item_title_height );
  glLoadIdentity();
  glOrtho( fk->GetDivider() + 1, fk_int->dd.cw, fk_int->dd.item_title_height, 0.0f, 1.0, -1.0 );

  assert( fk->GetDisplayMin() < fk->GetDisplayMax() );

  // Draw the ticks
  double d;

  glColor3ub( 0, 0, 0 );
  int x = 0;
  for( d = (float)((int)fk_int->dd.start_frame); x < fk_int->dd.timeline_size; d += fk_int->dd.frame_step ) {
    x = (int)( ((d/fk_int->dd.frame_rate - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    if( x < fk_int->dd.timeline_size ) {
      glBegin( GL_LINES );
      glVertex2i( fk_int->dd.start_edge + x, fk_int->dd.item_title_height - 4 );
      glVertex2i( fk_int->dd.start_edge + x, fk_int->dd.item_title_height - 1 );
      glEnd();
    }
  }

  char buffer[10];
  int last_label_x = 0;
  d = 0.0;

  // Write some frame numbers
  for( int i = (int)fk_int->dd.start_frame - fk_int->dd.frame_step_offset; d < fk_int->dd.start_edge + fk_int->dd.timeline_size; i+=fk_int->dd.frame_num_step ) {
    d = fk_int->dd.start_edge + ((double)i - fk_int->dd.start_frame) * fk_int->dd.frame_size;
    if( d > fk_int->dd.start_edge + fk_int->dd.timeline_size - 1)
      break;

    // Full Mark (at frame number labels)
    glBegin( GL_LINES );
    glVertex2i( (int)d, fk_int->dd.item_title_height - 8 );
    glVertex2i( (int)d, fk_int->dd.item_title_height - 1 );
    glEnd();

    // Half Mark (between frame number labels)
    if( fk_int->dd.frame_num_step >= 10 ) {               // Only draw if >=10 frames are displayed (we don't want half-frame ticks; it's confusing to the user)
      int half_d = (int)( d - (((double)fk_int->dd.frame_num_step * fk_int->dd.frame_size)/2.0) );
      glBegin( GL_LINES );
      glVertex2i( half_d, fk_int->dd.item_title_height - 6 );
      glVertex2i( half_d, fk_int->dd.item_title_height - 1 );
      glEnd();
    }

    // Draw text
    if( (d - last_label_x > fk_int->dd.label_width) ) {            // Keep enough space between eafk_int->dd.ch label
      last_label_x = (int)d;
      sprintf( buffer, "%d", i );

//      glRasterPos2i( fk->GetDivider() + 1, fk_int->dd.item_title_height - 8 );
//      glBitmap( 0, 0, 0, 0, (int)d - (fk_int->dd.opengl_font.TextWidth( buffer, strlen(buffer) )/2) - (fk->GetDivider() + 1), 0, NULL );
//      fk_int->dd.opengl_font.Write( buffer );

      fk_int->dd.opengl_font.Write( buffer, (int)d - (fk_int->dd.opengl_font.TextWidth( buffer, strlen(buffer) )/2),
                                    fk_int->dd.item_title_height - 9 );
    }
  }

  glFlush();
}

// DrawOpenGLItemList_Names():
//  Draws the OpenGL Item Names
void DrawOpenGLItemList_Names( LWControl *ctl ) {
  // Reset the coordinate system for the item name drawing area
  glViewport( fk_int->dd.item_text_column, 0,
              fk->GetDivider() + fk_int->dd.item_midcol_left_offset - fk_int->dd.item_text_column,
              fk_int->dd.trueh - fk_int->dd.item_title_height );
  glLoadIdentity();
  glOrtho( fk_int->dd.item_text_column,
           fk->GetDivider() + fk_int->dd.item_midcol_left_offset,
           fk_int->dd.trueh,
           fk_int->dd.item_title_height,
           1.0, -1.0 );

  // Rows
  glColor3ub( 112, 112, 112 );
  for( int j=1; j < fk_int->dd.trueh/fk_int->dd.item_height; j++ ) {
    glRecti( fk_int->dd.item_text_column + 1,                        fk_int->dd.item_title_height + (j * fk_int->dd.item_height),
             fk->GetDivider() + fk_int->dd.item_midcol_left_offset,  fk_int->dd.item_title_height + (j * fk_int->dd.item_height) + 1 );
  }

  // Items in the list
  if( fk_int->sorted_items.NumElements() == 0 ) {  // Check for no items in list
    glColor3ub( fk_int->dd.item_r, fk_int->dd.item_g, fk_int->dd.item_b );
    fk_int->dd.opengl_font.Write( "(none)", fk_int->dd.item_text_column + 4,
                                  fk_int->dd.item_title_height + fk_int->dd.item_height - 4 );

  } else {
    int depth_offset;                              // Amount to shift current item inward in Hierarchical mode
    int indent;                                    // Total amount ot indent the name
    int baseline;                                  // Base of the current line

    // Draw Group BG
    //  We do this first so that we don't chop of the bottom of the letters from the prev. line
    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      DrawOpenGLItemList_ItemBG( fk_int->dd.item_text_column + 1,                        baseline + 1,
                                 fk->GetDivider() + fk_int->dd.item_midcol_left_offset,  baseline + item_height, i );

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }

    // Draw Item Names
    for( i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      depth_offset = 0;
      indent       = 0;
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );

      if( fk->GetItemDisplayMode() == 0 ) {        // Hierarchical Mode Only (indent based on hierarchy depth)
        depth_offset = fk_int->dd.item_depth_offset * fk_int->sorted_items[i]->GetDepth();
        indent = fk_int->dd.item_fold_column_width;
      }

      if( fk->GetItemDisplayMode() == 1 ) {        // Natural Mode Only (indent the bones)
        if( fk_int->sorted_items[i]->GetItemType() == LWI_ITEM_BONE )
          depth_offset = fk_int->dd.item_depth_offset;
      }

      static char name_buffer[1024];
      strcpy( name_buffer, (char *)fk_int->sorted_items[i]->GetName() );
      indent += fk_int->dd.item_text_column + depth_offset + 4;

      // Add the item's clone index, if applicable
      if( fk_int->sorted_items[i]->GetCloneNumber() > 0 )
        sprintf( name_buffer, "%s (%d)", name_buffer, fk_int->sorted_items[i]->GetCloneNumber() );

      // Display the item name
      if( fk->GetDrawStyle() == DRAWSTYLE_L6 )
        fk_int->dd.SetItemColorName( COLOR_BLACK );
      else {
        if( fk_int->sorted_items[i]->GetSelected() )
          fk_int->dd.SetItemColorName( COLOR_LT_YELLOW );
        else
          fk_int->dd.SetItemColorName( COLOR_WHITE );
      }

      glColor3ub( fk_int->dd.item_r, fk_int->dd.item_g, fk_int->dd.item_b );
      glRasterPos2i( fk_int->dd.item_text_column, item_height + baseline - 4 );
      glBitmap( 0, 0, 0, 0, (float)(indent - fk->GetNameScroll() - fk_int->dd.item_text_column), 0, NULL );
      fk_int->dd.opengl_font.Write( name_buffer );
      
      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }


/*
  glColor3ub( fk_int->dd.item_r, fk_int->dd.item_g, fk_int->dd.item_b );
  for( j = 0; j < 500; j++ ) {
    glRecti( fk_int->dd.item_text_column + 1, j * 2,
             fk_int->dd.item_text_column + 3, j * 2 + 1);
  }
*/

#ifdef TEST_OPENGL_AREA
  // Testing:  Draw an empty box around the perimiters
  glColor3ub( fk_int->dd.bgr, fk_int->dd.bgg, fk_int->dd.bgb );
  glBegin( GL_LINE_LOOP );
  glVertex2i( fk_int->dd.item_text_column + 1, fk_int->dd.item_title_height + 1 );
  glVertex2i( fk_int->dd.item_text_column + 1, fk_int->dd.trueh-1 );
  glVertex2i( fk->GetDivider() + fk_int->dd.item_midcol_left_offset, fk_int->dd.trueh-1 );
  glVertex2i( fk->GetDivider() + fk_int->dd.item_midcol_left_offset, fk_int->dd.item_title_height + 1 );
  glEnd();
#endif
  glFlush();
}

// DrawOpenGLItemList_LeftColumns():
//  Draws the OpenGL Item List's Left-Side Columns
void DrawOpenGLItemList_LeftColumns( LWControl *ctl ) {
  // Reset the coordinate system for the column drawing area
  glViewport( 0, 0, fk_int->dd.item_text_column + 1, fk_int->dd.trueh );
  glLoadIdentity();
  glOrtho( 0, fk_int->dd.item_text_column + 1,
           fk_int->dd.trueh, 0,
           1.0, -1.0 );

  // Erase the left column area
  glColor3ub( fk_int->dd.bgr, fk_int->dd.bgg, fk_int->dd.bgb );
  glRecti( 0, fk_int->dd.item_title_height, fk_int->dd.item_text_column, fk_int->dd.trueh );
  
  // Rows
  glColor3ub( 112, 112, 112 );
  for( int j=1; j < fk_int->dd.trueh/fk_int->dd.item_height; j++ ) {
    glRecti( 0,                           fk_int->dd.item_title_height + (j * fk_int->dd.item_height),
             fk_int->dd.item_text_column, fk_int->dd.item_title_height + (j * fk_int->dd.item_height) + 1 );
  }

  // Columns (outside title)
  glColor3ub( fk_int->dd.c1r, fk_int->dd.c1g, fk_int->dd.c1b );
  glRecti( fk_int->dd.item_group_column_width - 1, fk_int->dd.item_title_height,
           fk_int->dd.item_group_column_width,     CON_HOTH( ctl ) );
  glRecti( fk_int->dd.item_text_column,            fk_int->dd.item_title_height,
           fk_int->dd.item_text_column + 1,        CON_HOTH( ctl ) );

  // Columns (inside title)
  glColor3ub( fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2b );
  glRecti( fk_int->dd.item_group_column_width - 1, 0,
           fk_int->dd.item_group_column_width,     fk_int->dd.item_title_height - 1 );
  glRecti( fk_int->dd.item_text_column,            0,
           fk_int->dd.item_text_column + 1,        fk_int->dd.item_title_height - 1 );


#ifdef TEST_OPENGL_AREA
  // Testing:  Draw an empty box around the perimiters
  glColor3ub( 150, 0, 0 );
  glBegin( GL_LINE_LOOP );
  glVertex2i( 1, fk_int->dd.item_title_height + 1 );
  glVertex2i( 1, fk_int->dd.trueh-1 );
  glVertex2i( fk_int->dd.item_text_column - 1, fk_int->dd.trueh-1 );
  glVertex2i( fk_int->dd.item_text_column - 1, fk_int->dd.item_title_height + 1 );
  glEnd();
#endif
  glFlush();
}

// DrawOpenGLItemList_MidColumns():
//  Draws the OpenGL Item List's Middle Columns
void DrawOpenGLItemList_MidColumns( LWControl *ctl ) {
  // Reset the coordinate system for the colunm drawing area
  glViewport( fk->GetDivider() + fk_int->dd.item_midcol_left_offset, 0,
              -fk_int->dd.item_midcol_left_offset + 1, fk_int->dd.trueh );
  glLoadIdentity();
  glOrtho( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,
           fk->GetDivider() + 1,
           fk_int->dd.trueh, 0,
           1.0, -1.0 );

  // Erase the middle column area
  glColor3ub( fk_int->dd.bgr, fk_int->dd.bgg, fk_int->dd.bgb );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset, fk_int->dd.item_title_height,
           fk->GetDivider() + 1,                                  fk_int->dd.trueh );

#ifdef TEST_OPENGL_AREA
  glColor3ub( 255, 0, 0 );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,     0,
           fk->GetDivider() + fk_int->dd.item_midcol_left_offset + 1, fk_int->dd.trueh );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,     0,
           fk->GetDivider() + 1,                                      1 );
  glRecti( fk->GetDivider(), 0,
           fk->GetDivider() + 1,  fk_int->dd.trueh );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,     fk_int->dd.trueh - 1,
           fk->GetDivider() + 1,                                      fk_int->dd.trueh );
#endif

  // Draw Group BG
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line
    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      DrawOpenGLItemList_ItemBG( fk->GetDivider() + fk_int->dd.item_midcol_left_offset, baseline + 1,
                                 fk->GetDivider(), baseline + item_height, i );

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  // Divider
  glColor3ub( 0, 0, 0 );
  glRecti( fk->GetDivider(),
           0,
           fk->GetDivider() + 1,
           fk_int->dd.trueh );

  // Columns (outside title)
  glColor3ub( fk_int->dd.c1r, fk_int->dd.c1g, fk_int->dd.c1b );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,
           fk_int->dd.item_title_height,
           fk->GetDivider() + fk_int->dd.item_midcol_left_offset + 1,
           fk_int->dd.trueh );

  glRecti( fk->GetDivider() + fk_int->dd.item_lock_offset,
           fk_int->dd.item_title_height,
           fk->GetDivider() + fk_int->dd.item_lock_offset + 1,
           fk_int->dd.trueh );

  // Columns (inside title)
  glColor3ub( fk_int->dd.c2r, fk_int->dd.c2g, fk_int->dd.c2b );
  glRecti( fk->GetDivider() + fk_int->dd.item_midcol_left_offset,
           0,
           fk->GetDivider() + fk_int->dd.item_midcol_left_offset + 1,
           fk_int->dd.item_title_height - 1 );

  glRecti( fk->GetDivider() + fk_int->dd.item_lock_offset,
           0,
           fk->GetDivider() + fk_int->dd.item_lock_offset + 1,
           fk_int->dd.item_title_height - 1 );

  glFlush();
}

// DrawOpenGLItemList_Keys():
//  Draws the OpenGL Item List's Keyframe Data
void DrawOpenGLItemList_Keys( LWControl *ctl ) {
  // Reset the coordinate system for the keyframe drawing area
  glViewport( fk->GetDivider() + 1,
              0,
              fk_int->dd.cw - fk->GetDivider() - 1,
              fk_int->dd.trueh );
  glLoadIdentity();
  glOrtho( fk->GetDivider() + 1,
           fk_int->dd.cw,
           fk_int->dd.trueh, 0,
           1.0, -1.0 );

  // Erase the keyframe area
  glColor3ub( fk_int->dd.bgr, fk_int->dd.bgg, fk_int->dd.bgb );
  glRecti( fk->GetDivider() + 1, fk_int->dd.item_title_height,
           fk_int->dd.cw,        fk_int->dd.trueh );

  double extent_start_time;
  double extent_end_time;
  int    extent_start_edge;
  int    extent_width;

#ifdef TEST_OPENGL_AREA
  glColor3ub( 255, 0, 0 );
  glRecti( fk->GetDivider() + 1, fk_int->dd.item_title_height,
           fk->GetDivider() + 2, fk_int->dd.trueh );
  glRecti( fk->GetDivider() + 1, fk_int->dd.item_title_height,
           fk_int->dd.cw,        fk_int->dd.item_title_height + 1 );
  glRecti( fk_int->dd.cw - 1,    fk_int->dd.item_title_height,
           fk_int->dd.cw,        fk_int->dd.trueh );
  glRecti( fk->GetDivider() + 1, fk_int->dd.trueh - 1,
           fk_int->dd.cw,        fk_int->dd.trueh );
#endif

  // Draw Group BG
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line

    int rx = fk_int->dd.item_text_column - 1;
    if( fk->GetItemDisplayMode() == 0 ) 
      rx += fk_int->dd.item_fold_column_width;

    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      DrawOpenGLItemList_ItemBG( fk->GetDivider() + 1, baseline + 1,
                                 fk_int->dd.cw, baseline + item_height, i );

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  // Draw the keyframe columns
  double d = 0;
  for( int i = (int)fk_int->dd.start_frame - fk_int->dd.frame_step_offset; d < fk_int->dd.start_edge + fk_int->dd.timeline_size; i+=fk_int->dd.frame_num_step ) {
    d = fk_int->dd.start_edge + ((double)i - fk_int->dd.start_frame) * fk_int->dd.frame_size;
    if( d > fk_int->dd.start_edge + fk_int->dd.timeline_size - 1)
      break;

    if( d > fk_int->dd.start_edge ) {
      // Full Mark (at frame number labels)
      glColor3ub( 112, 112, 112 );
      glRecti( (int)d,     fk_int->dd.item_title_height,
               (int)d + 1, fk_int->dd.cy + fk_int->dd.ch );

      // Half Mark (between frame number labels)
      if( fk_int->dd.frame_num_step >= 10 ) {               // Only draw if >=10 frames are displayed (we don't want half-frame ticks; it's confusing to the user)
        int half_d = (int)( d - (((double)fk_int->dd.frame_num_step * fk_int->dd.frame_size)/2.0) );
        if( half_d > fk_int->dd.start_edge ) {
          glColor3ub( 124, 124, 124 );
          glRecti( half_d,     fk_int->dd.item_title_height,
                   half_d + 1, fk_int->dd.cy + fk_int->dd.ch );
        }
      }
    }
  }

  // Draw the Keyframe Rows
  glColor3ub( 112, 112, 112 );
  for( int j=1; j < fk_int->dd.ch/fk_int->dd.item_height; j++ ) {
    glRecti( fk->GetDivider()+1, fk_int->dd.item_title_height + j*fk_int->dd.item_height,
             fk_int->dd.cw  - 1, fk_int->dd.item_title_height + j*fk_int->dd.item_height + 1 );
  }

  // Draw the First Frame/Last Frame/Now markers
  int x;
  if( (fk->GetScene().GetStartTime() > fk_int->dd.start_time) && (fk->GetScene().GetStartTime() < fk_int->dd.end_time) ) {
    x = (int)( ((fk->GetScene().GetStartTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    glColor3ub( 0, 172, 0 );
    glRecti( x + fk_int->dd.start_edge,     fk_int->dd.truey + 14,
             x + fk_int->dd.start_edge + 1, fk_int->dd.trueh );
  }

  if( (fk->GetScene().GetEndTime() > fk_int->dd.start_time) && (fk->GetScene().GetEndTime() < fk_int->dd.end_time) ) {
    x = (int)( ((fk->GetScene().GetEndTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );
    glColor3ub( 172, 0, 0 );
    glRecti( x + fk_int->dd.start_edge,     fk_int->dd.truey + 14,
             x + fk_int->dd.start_edge + 1, fk_int->dd.trueh );
  }


  // Draw the Keyframe Data
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line
    int timeline_width = fk_int->dd.cw - fk->GetDivider();
    LWMotion *motion;

    for( int i=fk->GetTopItem(); i < (long)fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );
      motion = &fk_int->sorted_items[i]->GetMotion();

      extent_start_time = motion->FindEarliestKey();
      extent_end_time   = motion->FindLatestKey();
      int total_keys    = motion->FindMaxKeys();

      if( total_keys > 0 ) {
        // Only try drawing if the keys are in the visible part of the list
        if( (extent_start_time <= fk_int->dd.end_time) && (extent_end_time >= fk_int->dd.start_time) ) {
          if( extent_start_time <= fk_int->dd.start_time )
            extent_start_edge = -2;
          else
            extent_start_edge = (int)( ((extent_start_time - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size - 1.0 );

          if( total_keys == 1 ) 
            extent_width =  (int)( fk_int->dd.frame_size / 8 );
          else if( extent_end_time > fk_int->dd.end_time )
            extent_width = fk_int->dd.timeline_size - extent_start_edge + 2;
          else {
            extent_width = (int)( ((extent_end_time - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size ) - extent_start_edge + 2;
            if( extent_start_edge < 0 )
              extent_width -= extent_start_edge;
          }

          if( extent_width < 3 )
            extent_width = 3;

          // Make pastely versions of the item color
          IMG_BYTE item_color[3], hsv[3];
          PanelDraw::GetLWColor( item_color, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor() );
          ImageClass::RGB2HSV( item_color, hsv );
          hsv[1] /= 2;
          ImageClass::HSV2RGB( hsv, item_color );

          // Keyframe Extents
          if( fk->GetKeyDisplayMode() != 1 ) {
              DrawExtentOpenGL( fk_int->dd.start_edge + extent_start_edge, baseline+1, extent_width, fk_int->dd.item_height - 2,
                                fk_int->dd.start_edge, fk_int->dd.timeline_size, 1, 1,
                                (int)(item_color[0] * 0.90), (int)(item_color[1] * 0.90), (int)(item_color[2] * 0.90),
                                (int)(item_color[0] * 0.70), (int)(item_color[1] * 0.70), (int)(item_color[2] * 0.70),
                                (int)(item_color[0] * 0.80), (int)(item_color[1] * 0.80), (int)(item_color[2] * 0.80) );
          }

          // Keyframes
          if( fk->GetKeyDisplayMode() != 0 ) {
            LWKeyFrame * key = NULL;
            double       old_time = fk_int->dd.start_time;
            int          key_x;

            // Draw a line from start to end
            glColor3ub( item_color[0], item_color[1], item_color[2] );
            glBegin( GL_LINES );
            glVertex2i( fk_int->dd.start_edge + extent_start_edge,                baseline+7 );
            glVertex2i( fk_int->dd.start_edge + extent_start_edge + extent_width, baseline+8 );
            glEnd();

            // Draw the keyframe marks
            if( !fk_int->quickdraw_item_list ) {
              do {
                key = motion->FindNextKey( old_time + .001 );  // that's 1/1000th of a second, or .03 frames.  Should be small enough...
                if( key == NULL )
                  break;
                else if( key->GetTime() > fk_int->dd.end_time )
                  break;
                else {
                  old_time = key->GetTime();
                  key_x = (int)( ((key->GetTime() - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time) * fk_int->dd.timeline_size );

                  // See if a keyframe on any channel has been selected
                  bool is_sel = false;
                  for( unsigned int m = 0; m < motion->GetChannelList().NumElements(); m++ ) {
                    for( unsigned int n=0; n < motion->GetChannel(m).GetSelected().NumElements(); n++ ) {
                      if( motion->GetChannel(m).GetSelected()[n]->GetTime() == key->GetTime() ) {
                        is_sel = true;
                        break;
                      }
                    }

                    if( is_sel )
                      break;
                  }
                  DrawKeyframeMarkOpenGL( fk->GetDivider() + key_x - 2, baseline + 5, is_sel );
                }
              } while( key != NULL );
            }

          }
        }
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }


/*
  // Draw the Bounding Box Selection again (this time over the keys), if applicable
  if( fk_int->draw_bbox ) {
    if( (fk_int->bbox_t1 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Left Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+bbox_x, fk_int->dd.cy+bbox_y,
                      1, bbox_h );
    }

    if( (fk_int->bbox_t2 <= fk_int->dd.end_time) && (fk_int->bbox_t1 >= fk_int->dd.start_time) ) {  // Right Edge
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+bbox_x+bbox_w, fk_int->dd.cy+bbox_y,
                      1, bbox_h );
    }

    if( draw_horiz_top ) {                                                    // Top
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+bbox_x, fk_int->dd.cy+bbox_y,
                      bbox_w, 1 );
    }

    if( draw_horiz_bottom ) {                                                 // Bottom
      df->DrawRGBBox( fk_int->main_panel, 200, 200, 100, fk_int->dd.cx+fk->GetDivider()+bbox_x, fk_int->dd.cy+bbox_y+bbox_h,
                      bbox_w, 1 );
    }
  }
*/
  glFlush();
}

// DrawExtentOpenGL():
//  Draws a keyframe extent bar (OpenGL)
bool DrawExtentOpenGL( int x, int y, int width, int height,
                       int min_x, int min_w,
                       int x_thickness, int y_thickness,
                       int shine_r,  int shine_g,  int shine_b,
                       int shadow_r, int shadow_g, int shadow_b,
                       int fill_r,   int fill_g,   int fill_b ) {

  if( (x + width < min_x) || (x > min_x + min_w ) )
    return false;

  if( fk->GetDrawStyle() == 0 ) {
    GLTDraw2D::Button( x, y, width, height, 1, 1,
                       fill_r,   fill_g,   fill_b,
                       fill_r,   fill_g,   fill_b,
                       fill_r,   fill_g,   fill_b );
  } else {
    GLTDraw2D::Button( x, y, width, height, 1, 1,
                       shine_r,  shine_g,  shine_b,
                       shadow_r, shadow_g, shadow_b,
                       fill_r,   fill_g,   fill_b );
  }
  // Draw the Extent Handles
  //  Left
  if( min_x < x ) {
    glColor3ub( 0, 0, 0 );
    glBegin( GL_POINTS );
    glVertex2i( x-1, y+2 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x-2, y+3 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x-3, y+4 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x-4, y+5 + GLTLWIcons::GetLegacyOffsetAmount() );

    glColor3ub( shadow_r, shadow_g, shadow_b );
    glVertex2i( x-2, y+4 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x-3, y+5 + GLTLWIcons::GetLegacyOffsetAmount() );
    glEnd();

    glColor3ub( 0, 0, 0 );
    glRecti( x-5, y+5,        x+1, y+height-1 );
    glRecti( x-5, y+height-2, x+1, y+height-1 );

    glColor3ub( shine_r, shine_g, shine_b );
    glRecti( x-1, y+2,        x,   y+height-2 );
    glRecti( x-4, y+height-3, x,   y+height-2 );

    glColor3ub( shadow_r, shadow_g, shadow_b );
    glRecti( x-4, y+5,        x-3, y+height-3 );

    glColor3ub( fill_r, fill_g, fill_b );
    glRecti( x-2, y+4,        x-1, y+height-3 );
    glRecti( x-3, y+5,        x-2, y+height-3 );
  }

  //  Right
  if( min_x+min_w > x+width ) {
    glColor3ub( 0, 0, 0 );
    glBegin( GL_POINTS );
    glVertex2i( x+width,   y+2 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x+width+1, y+3 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x+width+2, y+4 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x+width+3, y+5 + GLTLWIcons::GetLegacyOffsetAmount() );

    glColor3ub( shadow_r, shadow_g, shadow_b );
    glVertex2i( x+width+1, y+4 + GLTLWIcons::GetLegacyOffsetAmount() );
    glVertex2i( x+width+2, y+5 + GLTLWIcons::GetLegacyOffsetAmount() );
    glEnd();

    glColor3ub( 0, 0, 0 );
    glRecti( x+width+4, y+5,        x+width+5, y+height-1 );
    glRecti( x+width,   y+height-2, x+width+5, y+height-1 );

    glColor3ub( shine_r, shine_g, shine_b );
    glRecti( x+width,   y+2,        x+width+1, y+height-2 );
    glRecti( x+width+1, y+height-3, x+width+4, y+height-2    );

    glColor3ub( shadow_r, shadow_g, shadow_b );
    glRecti( x+width+3, y+5,    x+width+4, y+height-3 );

    glColor3ub( fill_r, fill_g, fill_b );
    glRecti( x+width+1, y+4,    x+width+2, y+height-3 );
    glRecti( x+width+2, y+5,    x+width+3, y+height-3 );
  }
  return true;
}

// DrawKeyframeMarkOpenGL():
//  Draws a keyframe mark in the item list.  If selected
//   is true, then a selected key will be drawn (OpenGL)
bool DrawKeyframeMarkOpenGL( int x, int y, bool selected ) {
  if( selected ) {
    glColor3ub( 255, 255, 191 );
    glRecti( x, y, x+5, y+5 );
  } else {
    glColor3ub( 255, 255, 255 );
    glRecti( x+2, y,   x+3, y+5 );
    glRecti( x,   y+2, x+5, y+3 );
  }

  return true;
}

// DrawOpenGLItemList_Icons
//  Draws icons for the items in the list
void DrawOpenGLItemList_Icons( LWControl *ctl ) {
  // Reset the coordinate system so zero-zero is the top-left (3rd quadrant)
  glLoadIdentity();
  glViewport( fk_int->dd.cx, fk_int->dd.truey, fk_int->dd.cw, fk_int->dd.trueh );
  glOrtho( 0.0f, fk_int->dd.cw, fk_int->dd.trueh, 0.0f, 1.0, -1.0 );

  // Icons for items in the list
  if( fk_int->sorted_items.NumElements() != 0 ) {  // Check for no items in list
    int baseline;                                  // Base of the current line
    for( unsigned int i=fk->GetTopItem(); i < fk_int->sorted_items.NumElements(); i++ ) {
      baseline = fk_int->dd.cy + ((i - fk->GetTopItem()) * fk_int->dd.item_height );

      // Figure out what color the text should be drawn in
      int item_text_color = fk_int->sorted_items[i]->GetSelected() ? COLOR_LT_YELLOW : COLOR_WHITE;
      static char name_buffer[1024];

      // Display the group index
      if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
        int item_text_color;
        if( fk->GetDrawStyle() == DRAWSTYLE_L6 )
          item_text_color = fk_int->dd.item_color_name;
        else
          item_text_color = fk_int->sorted_items[i]->GetSelected() ? COLOR_LT_YELLOW : fk_int->dd.item_color_name;

        sprintf( name_buffer, "%d", fk_int->sorted_items[i]->GetGroup() + 1 );

        glColor3ub( fk_int->dd.item_r, fk_int->dd.item_g, fk_int->dd.item_b );
        glRasterPos2i( fk_int->dd.item_group_column+4, item_height + baseline - 4 );
        fk_int->dd.opengl_font.Write( name_buffer );
      }

      // Fold Icons
      if( fk->GetItemDisplayMode() == 0 ) {        // Hierarchical Mode Only
        glColor3ub( fk_int->dd.bgr, fk_int->dd.bgg, fk_int->dd.bgb );
        glRecti( fk_int->dd.item_text_column + 1, baseline + 1,
                 fk_int->dd.item_fold_column_end, baseline + fk_int->dd.item_height - 1 );
        if( (fk_int->sorted_items[i]->GetChildren().NumElements() > 0) ||
            (fk_int->sorted_items[i]->HasSkeleton()) ) {
          GLTLWIcons::DrawFoldIcon( fk_int->dd.cx + fk_int->dd.item_text_column + 3, baseline,
                                    fk_int->dd.item_height, fk_int->dd.item_height,
                                    fk_int->sorted_items[i]->GetFolded(), (PanelDraw::FoldedType)fk_int->dd.fold_type );
        }
      }

      // Item Type Icons
      switch( fk_int->sorted_items[i]->GetItemType() ) {
      case LWI_ITEM_OBJECT:
      case LWI_ITEM_NULL_OBJECT:
        GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                                PanelDraw::object, (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor(), (PanelDraw::IconMode)fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_BONE:
        GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                                PanelDraw::bone, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), (PanelDraw::IconMode)fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_LIGHT:
        GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                                PanelDraw::light, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), (PanelDraw::IconMode)fk_int->dd.icon_mode );
        break;

      case LWI_ITEM_CAMERA:
        GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk_int->dd.item_type_column, baseline + 2,
                                PanelDraw::camera, (PanelDraw::ItemColor)fk_int->sorted_items[i]->GetDrawColor(), (PanelDraw::IconMode)fk_int->dd.icon_mode );
        break;
      }


      // Item Draw Mode Icons (Visibility)
      switch( fk_int->sorted_items[i]->GetItemType() ) {
      case LWI_ITEM_OBJECT:
      case LWI_ITEM_NULL_OBJECT:
        if( fk_int->sorted_items[i]->GetDrawMode() != LWIDRAW_NONE ) {
          if( fk->GetDrawStyle() == DRAWSTYLE_56 ) {
            GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                    (PanelDraw::IconType)(PanelDraw::draw_bounding_box + fk_int->sorted_items[i]->GetDrawMode() - 1),
                                    (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor() );
          } else {
            GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                    (PanelDraw::IconType)(PanelDraw::draw_bounding_box + fk_int->sorted_items[i]->GetDrawMode() - 1),
                                    80, 80, 80, PanelDraw::draw_L6 );
          }
        }
        break;

      case LWI_ITEM_BONE:
      case LWI_ITEM_CAMERA:
      case LWI_ITEM_LIGHT:
        if( fk_int->sorted_items[i]->GetDrawMode() != LWIDRAW_NONE ) {
          if( fk->GetDrawStyle() == DRAWSTYLE_56 ) {
            GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                    PanelDraw::check,
                                    (PanelDraw::ItemColor) fk_int->sorted_items[i]->GetDrawColor(), fk_int->dd.icon_mode );
          } else {
            GLTLWIcons::DrawLWIcon( fk_int->dd.cx + fk->GetDivider() + fk_int->dd.item_visibility_offset + 2, baseline + 3,
                                    PanelDraw::check,
                                    80, 80, 80, PanelDraw::draw_L6 );
          }
        }
        break;
      }

      // Check for the end of the list
      if( fk_int->dd.ch/fk_int->dd.item_height < (int)i - fk->GetTopItem() + 2 )
        break;
    }
  }

  glFlush();
};

// DrawOpenGLItemList_ItemBG():
void DrawOpenGLItemList_ItemBG( int x1, int y1, int x2, int y2, int i ) {
  if( (fk->GetDrawStyle() == DRAWSTYLE_L6) && fk_int->sorted_items[i]->GetSelected() ) {
    // Item is selected
    glColor3ub( 160, 160, 160 );
    glRecti( x1, y1, x2, y2 );
  } else {
    if( fk_int->sorted_items[i]->GetGroup() != -1 ) {
      if( fk_int->sorted_items[i]->GetGroup() == fk->GetCurrentGroup() ) {
        // Item belongs to current group
        glColor3ub( fk_int->dd.group_cur_r, fk_int->dd.group_cur_g, fk_int->dd.group_cur_b );
        glRecti( x1, y1, x2, y2 );
      } else {
        // Item belongs to another group
        glColor3ub( fk_int->dd.group_not_r, fk_int->dd.group_not_g, fk_int->dd.group_not_b );
        glRecti( x1, y1, x2, y2 );
      }
    }
  }
}
