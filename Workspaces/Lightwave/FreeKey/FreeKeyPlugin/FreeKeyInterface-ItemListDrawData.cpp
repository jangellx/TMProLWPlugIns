//
// FreeKeyIntreface-ItemListDrawData.cpp
//

#include <math.h>

#include "FreeKey-Interface.h"

extern FreeKeyInterface *fk_int;
extern FreeKey          *fk;

//  Variable Initializations
const int FKItemListDrawData::group_cur_r             =  132;
const int FKItemListDrawData::group_cur_g             =  132;
const int FKItemListDrawData::group_cur_b             =  116;
const int FKItemListDrawData::group_not_r             =  116;
const int FKItemListDrawData::group_not_g             =  132;
const int FKItemListDrawData::group_not_b             =  132;

const int FKItemListDrawData::item_height             =   16;
const int FKItemListDrawData::item_group_column       =    0;
const int FKItemListDrawData::item_group_column_width =   15;
const int FKItemListDrawData::item_type_column        =  item_group_column + item_group_column_width;
const int FKItemListDrawData::item_type_column_width  =   14;
const int FKItemListDrawData::item_text_column        =  item_type_column + item_type_column_width;
const int FKItemListDrawData::item_fold_column_width  =   20;
const int FKItemListDrawData::item_fold_column_end    =  item_text_column + item_fold_column_width;
const int FKItemListDrawData::item_depth_offset       =   10;
const int FKItemListDrawData::item_name_max_scroll    =  500;

const int FKItemListDrawData::item_visibility_offset  =  -32;
const int FKItemListDrawData::item_lock_offset        =  -16;
const int FKItemListDrawData::item_midcol_left_offset = item_visibility_offset;
const int FKItemListDrawData::item_title_height       =   20;

const int FKItemListDrawData::label_width             =   40;

// Constructor
FKItemListDrawData::FKItemListDrawData() {
  Generate( NULL, false, DRAWSTYLE_56 );
}

// Destructor
FKItemListDrawData::~FKItemListDrawData() {

}

// Generate():
//  Generates all the variables used to draw the item list
void FKItemListDrawData::Generate( LWControl *_ctl, bool _opengl,
                                 fk_drawstyle _style ) {
  ctl    = _ctl;
  opengl = _opengl;
  style  = _style;

  // No control specified; initialize all to 0
  if( ctl == NULL ) {
    cx = cy = cw = ch = 0;
    truey = trueh = 0;
  } else if( opengl ) {
    // OpenGL Control-Specific Variables
    truey = 0;
    trueh = CON_HOTH( ctl );

    cx = 0;
    cy = truey + 20;
    ch = trueh - 20;
    cw = CON_HOTW( ctl );
  } else {
    // Panels Control-Specific Variables
    truey = CON_HOTY( ctl );
    trueh = CON_HOTH( ctl );

    cx = CON_HOTX( ctl );
    cy = CON_HOTY( ctl ) + 20;
    ch = CON_HOTH( ctl ) - 20;
    cw = CON_HOTW( ctl );
  }

  // Other mode-specific settings
  bgr = bgg = bgb = 128;
  if( style == DRAWSTYLE_56 ) {
    r   = g   = b   = 128;
    r2  = g2  = b2  = 0;
    fold_type = PanelDraw::plus_minus;
    icon_mode = PanelDraw::draw_56;
    SetItemColorName( COLOR_WHITE );
    c1r = c1g = c1b = 112;
    c2r = c2g = c2b =  80;
  } else {
    r   = g   = b   = 112;
    r2  = g2  = b2  = 0;
    fold_type = PanelDraw::triangle;
    icon_mode = PanelDraw::draw_L6;
    SetItemColorName( COLOR_BLACK );
    c1r = c1g = c1b =   0;
    c2r = c2g = c2b =   0;
  }

  frame_rate    = 30.0;

  // fk-Dependant Variable Setup
  if( fk != NULL ) {
    first_frame   = (fk->GetDisplayMin() * frame_rate) - 2;
    last_frame    = (fk->GetDisplayMax() * frame_rate) + 2;

    total_frames  = last_frame - first_frame;
    total_time    = total_frames/frame_rate;

    start_time    = (fk->GetTimeScroll() * total_time) + (first_frame/frame_rate);
    end_time      = ( (fk->GetTimeScroll() + fk->GetZoom()) * total_time ) + (first_frame/frame_rate);

    start_edge    = fk->GetDivider() + cx + 1;
    timeline_size = cw - (fk->GetDivider() + 1);
  } else {
    first_frame   = -2;
    last_frame    =  7;

    total_frames  = last_frame - first_frame;
    total_time    = total_frames/frame_rate;

    start_time    = -2.0/frame_rate;
    end_time      =  7.0/frame_rate;

    start_edge    = 200;
    timeline_size = cw - 200;
  }

  total_frames      = last_frame - first_frame;
  total_time        = total_frames/frame_rate;
  zoomed_total_time = end_time - start_time;

  start_frame       = start_time * frame_rate;
  end_frame         = end_time   * frame_rate;

  // Compute the number of pixels for a single frame
  frame_size        = (double)timeline_size / (end_frame - start_frame);
  num_steps         = (int)( frame_size / (double)timeline_size );

  // Figure out the step for timeline ticks
  frame_step = 1;
  while( fabs(frame_size * frame_step) < 3 )
    frame_step++;

  // Figure out where to put frame numbers, and how many
  max_labels     = (timeline_size-label_width)/label_width;
  num_frames     = end_frame - start_frame;
  frame_num_step = 1;

  for( int i=1; true; i*=10 ) {
    if( num_frames <= 10.0 * i ) {
      frame_num_step = 1 * i;
      break;
    } else if( ( num_frames >= 100 ) && ( num_frames <= 25.0 * i ) ) {
      frame_num_step = (int)( (double)2.5 * (double)i );
      break;
    } else if( num_frames <= 50.0 * i ) {
      frame_num_step = 5 * i;
      break;
    }
  }

  frame_step_offset = (int)start_frame % frame_num_step;

  // Figure out the bounding box size, if needed
  if( fk_int != NULL ) {
    if( fk_int->draw_bbox ) {
      // Compute the y position and height of the box
      draw_horiz_top = draw_horiz_bottom = true;
      if( fk_int->bbox_item_base < fk->GetTopItem() ) {
        draw_horiz_top = false;
        bbox_y = 0;
        bbox_h = fk_int->bbox_y2;
      } else if( fk_int->bbox_item_base > fk->GetTopItem() + (fk_int->dd.ch/fk_int->dd.item_height) ) {
        draw_horiz_bottom = false;
        bbox_y = fk_int->dd.ch;
        bbox_h = fk_int->dd.ch-fk_int->bbox_y2;
      } else {
        bbox_y = ( (fk->GetTopItem() - fk_int->bbox_item_base)/fk_int->dd.item_height ) + fk_int->bbox_y1;
        bbox_h = fk_int->bbox_y2 - bbox_y;
      }

      if( bbox_h + bbox_y < 0 )
        bbox_h = -bbox_y;
      else if( bbox_h + bbox_y > fk_int->dd.ch )
        bbox_h = fk_int->dd.ch - bbox_y;

      // Compute the x position and width of the box
      int bbox_x2 = (int)( (fk_int->bbox_t2 - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time * (double)fk_int->dd.timeline_size );
      if( fk_int->bbox_t1 < fk_int->dd.start_time ) {
        bbox_x = 0;
        bbox_w = bbox_x2;
      } else if( fk_int->bbox_t1 > fk_int->dd.end_time ) {
        bbox_x = fk_int->dd.cw;
        bbox_w = bbox_x2 - fk_int->dd.cw;
      } else {
        bbox_x  = (int)( (fk_int->bbox_t1 - fk_int->dd.start_time)/fk_int->dd.zoomed_total_time * (double)fk_int->dd.timeline_size );
        bbox_w = bbox_x2 - bbox_x;
      }
    }
  } else {
    draw_horiz_top = draw_horiz_bottom = false;
    bbox_x = bbox_y = bbox_w = bbox_h = 0;
  }

  // Initialize the OpenGL font, if needed
  if( opengl ) {
    if( !opengl_font.IsFontGenerated() ) {
      opengl_font.SetFontName( FK_OPENGL_FONT_NAME );
      opengl_font.SetHeight( FK_OPENGL_FONT_SIZE );
      opengl_font.Generate();
    }
  }
}

// SetItemColorName():
//  This sets both the item color name index (for LWPanels) and the
//   associated RGBs (for OpenGL).
void FKItemListDrawData::SetItemColorName( int color ) {
  item_color_name = color;

  if( color == COLOR_BLACK ) {
    item_r = item_g = item_b = 0;
  } else if( color == COLOR_WHITE ) {
    item_r = item_g = item_b = 200;
  } else if( color == COLOR_LT_YELLOW ) {
    item_r = 255; item_g = 255; item_b = 191;
  } else {
    item_r = 0; item_g = 255; item_b = 0;
  }
}
