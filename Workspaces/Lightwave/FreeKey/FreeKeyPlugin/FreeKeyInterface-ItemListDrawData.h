//
// FreeKeyInterface-ItemListDrawData.h
//
// This contains the ItemListDrawData class.  It's purposes
//  is to generate and store all the item list drawing variables,
//  which are more-or-less shared bettween the OpenGL and Panels
//  versions of the control, as well as the event handlers for
//  both, but which are just a bit different between the two

#include <Lightwave\LW_SDK_CPP_Wrappers\LWWrappers.h>      // C++ Wrappers for LW SDK
#include <Portable/OpenGLTools/OpenGLTools.h>

#include "..\FreeKey\FreeKey.h"

#ifdef WIN32
#define FK_OPENGL_FONT_NAME  "Arial"
#define FK_OPENGL_FONT_SIZE  15
#endif

class FKItemListDrawData {
public:
  FKItemListDrawData();
  ~FKItemListDrawData();

  void Generate( LWControl *_ctl, bool _opengl, fk_drawstyle _style );

  // Accessors
  void SetItemColorName( int color );

public:
  // Generate() Data
  bool           opengl;
  LWControl    * ctl;
  fk_drawstyle   style;


  // Constant Item List Positions
  static const int item_height;                // Height of each line
  static const int item_group_column;          // Group Column left edge
  static const int item_group_column_width;    // Group Column width

  static const int item_type_column;           // Item Type left edge
  static const int item_type_column_width;     // Item Type width

  static const int item_text_column;           // Left edge of the item name

  static const int item_fold_column_width;     // Width of fold column
  static const int item_fold_column_end;       // Right edge of fold column

  static const int item_depth_offset;          // Modifier to item_text_x for hierarchy depth

  static const int item_name_max_scroll;       // Max amount the item name list can be scroll horizontaly

  static const int item_visibility_offset;
  static const int item_lock_offset;
  static const int item_midcol_left_offset;
  
  static const int item_title_height;          // Height of the Item List Title

  // Group Constant Colors
  static const int group_cur_r, group_cur_g, group_cur_b;  // Current Group Color
  static const int group_not_r, group_not_g, group_not_b;  // Not-The-Current Group Color

  // Icon Modes
  PanelDraw::FoldedType fold_type;
  PanelDraw::IconMode   icon_mode;

  // Control Vraiables
  int truey, trueh;                                 // True control size
  int cx, cy, cw, ch;                               // Control Size minus title line

  // Draw style variables
  int r,   g,   b;                                  // Color 1
  int r2,  g2,  b2;                                 // Color 2
  int bgr, bgg, bgb;                                // Background Color
  int c1r, c1g, c1b;                                // Column 1 Color
  int c2r, c2g, c2b;                                // Column 2 Color

  // Item Color (change these with a SetItemColorName(), not directly)
  int item_color_name;                              // Item Color Name
  int item_r, item_g, item_b;                       // Item Color RGB

  // Frame Variables
  double    first_frame;
  double    last_frame;

  double    frame_rate;
  double    total_frames;
  double    total_time;
  double    start_time;
  double    end_time;

  double    zoomed_total_time;

  double    start_frame;
  double    end_frame;
  int       start_edge;
  int       timeline_size;

  double    frame_size;
  int       num_steps;
  int       frame_step;

  static const int label_width;
  int       max_labels;
  double    num_frames;
  int       frame_num_step;

  int       frame_step_offset;

  int bbox_x, bbox_y, bbox_w, bbox_h;
  bool draw_horiz_top, draw_horiz_bottom;

  // OpenGL Support Class Instances
  GLTBitmapFont                   opengl_font;      // OpenGL Item List Font
};

void DrawOpenGLItemList_Title(       LWControl *ctl );
void DrawOpenGLItemList_Names(       LWControl *ctl );
void DrawOpenGLItemList_LeftColumns( LWControl *ctl );
void DrawOpenGLItemList_MidColumns(  LWControl *ctl );
void DrawOpenGLItemList_Keys(        LWControl *ctl );
void DrawOpenGLItemList_Icons(       LWControl *ctl );
bool DrawExtentOpenGL( int x, int y, int width, int height,
                       int min_x, int min_w,
                       int x_thickness, int y_thickness,
                       int shine_r,  int shine_g,  int shine_b,
                       int shadow_r, int shadow_g, int shadow_b,
                       int fill_r,   int fill_g,   int fill_b );
bool DrawKeyframeMarkOpenGL( int x, int y, bool selected );

void DrawOpenGLItemList_ItemBG( int x1, int y1, int x2, int y2, int i );
