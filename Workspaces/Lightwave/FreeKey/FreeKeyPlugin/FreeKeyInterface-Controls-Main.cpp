//
// FreeKeyInterface-Controls-Main.cpp
//
// This file contains the code to create the main controls on 
//  the FreeKey Interface, including the controls in the tool
//  options tabs.
//

#include "FreeKey-Interface.h"

extern ImageClass *decoded_logo_small;

// Misc. UI Strings
const char *source_popup_text[] = {
  "Refresh Scene",
  "Get Layout's Scene Path",
  "Save Scene In Layout",
  "Load Scene Into Layout",
  NULL };

const char *output_popup_text[] = {
  "Auto-Generate Name",
  "Load Scene Into Layout",
  NULL };

const char *selection_name_text[] = {
  "Exact Match",
  "Starting With",
  "Ending With",
  "Containing",
  NULL };

const char *select_toggle_text[] = {
  "Select",
  "Deselect",
  "Invert",
  NULL };

const char *selection_mode_text[] = {
  "Last Clicked",
  "All Selected",
  "All Unselected",
  "All Matching Name",
  "All In Current Group",
  "All",
/*
  "All Nulls",
  "All Objects",
  "All Nulls And Objects",
  "All Bones",
  "All Lights",
  "All Cameras",
*/
  NULL };

const char *remove_mode_text[] = {
  "Don't Remove Any Keys",
  "Remove Keys Inside Range",
  "Remove Keys Outside Range",
  "Remove Keys Before Range",
  "Remove Keys After Range",
  NULL };

const char *step_mode_text[] = {
  "Don't Use The Step (Disable Step)",
  "Delete Keys At Step (Normal Step)",
  "Keep Keys At Step (Invert Step)",
  NULL };

const char *direction_mode_text[] = {
  "Forward",
  "Reverse",
  NULL };

const char *display_mode_text[] = {
  "Hierarchical",
  "Natural Order",
  "Alphabetical",
  "Selected Only",
  "Unselected Only",
  "Grouped Only",
  "Ungrouped Only",
  "Current Group Only",
  NULL };

const char *display_key_mode_text[] = {
  "Extents Only",
  "Keys Only",
  "Extents and Keys",
  NULL };

const char *item_range_preset_text[] = {
  "First and Last Render Frames",
  "First and Last Preview Frames",
  "First and Last Keyframes in Scene",
  "First and Last Keyframes in Selection",
  "First and Last Keyframes in Current Item",
  "First and Last Keyframes in Current Group",
  "First and Last Keyframes in Listed Items",
  NULL };

const char *group_edit_text[] = {
  "Add Selected To Group",
  "Add All Ungrouped To Group",
  "Remove Selected From Group",
  "Remove All From Group",
  NULL };

const char *tool_option_tab_text[] = {
  "Item Select",
  "Key Select",
  "Groups",
  "Numeric",
  "Options",
  NULL };

const char *opt_draw_style_text[] = {
  "Lightwave 5.5/5.6",
  "Lightwave 6",
  NULL };

// Globals
extern FreeKeyInterface *fk_int;
extern FreeKey          *fk;

// MakeMainControls():
bool FreeKeyInterface::MakeMainControls() {
  try {
    int x, y, w;

    // Title Image (OpenGL)
    if( !( title_image = OPENGL_CTL( panel_funcs->orig_struct, main_panel,
                                     "", decoded_logo_small->GetWidth() - 25, decoded_logo_small->GetHeight() ) ) )
      throw false;

    MOVE_CON( title_image, -13, -4 );

    // File ItemSelection Conrols
    if( !( source_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                    "Source Scene", 91 ) ) )
      throw false;

    if( !( source_popup = POPDOWN_CTL( panel_funcs->orig_struct, main_panel,
                                       "Source Tools", source_popup_text ) ) )
      throw false;

    if( !( output_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                    "Output Scene", 91 ) ) )
      throw false;

    if( !( output_popup = POPDOWN_CTL( panel_funcs->orig_struct, main_panel,
                                       " Output Tools", output_popup_text ) ) )
      throw false;

//    x = CON_X( source_scene );
//    y = CON_Y( source_scene );
//    MOVE_CON( source_scene, x, y + 35 );

    PanelTools::PutUnder( source_scene, source_scene, -11 );
    PanelTools::AlignLabels( source_scene, output_scene, 5 );
    PanelTools::PutOnLineWith( source_scene, source_popup, -10 );
    PanelTools::PutOnLineWith( output_scene, output_popup, -10 );

    // Default Settings for String/File and Popup Controls
    SET_STR( source_scene,  (char *)fk->GetSource(),   strlen( fk->GetSource()   ) );
    SET_STR( output_scene,  (char *)fk->GetOutput(),   strlen( fk->GetOutput()   ) );

    const int list_x_offset = 130;

    // List Controls
    if( !( list_fold = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                    "Fold", 50 ) ) )
      throw false;

    if( !( list_unfold = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                      "Unfold", 50 ) ) )
      throw false;

    if( !( select_count = INTRO_CTL( panel_funcs->orig_struct, main_panel,
                                     "" ) ) )
      throw false;

    // Display Controls
    if( !( display_key_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "Key Mode", display_key_mode_text, 130 ) ) )
      throw false;

    if( !( display_item_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "Item List", display_mode_text, 130 ) ) )
      throw false;

    SET_INT( display_key_mode,  fk->GetKeyDisplayMode()  );
    SET_INT( display_item_mode, fk->GetItemDisplayMode() );

    PanelTools::PutUnder( output_scene, list_fold );
    PanelTools::PutUnder( list_fold, list_fold );

    x = CON_X( list_fold );
    y = CON_Y( list_fold );
    MOVE_CON( list_fold, x + 10 + list_x_offset, y-6 );
    PanelTools::PutOnLineWith( list_fold, list_unfold, -7 );
    PanelTools::PutOnLineWith( list_unfold, select_count, 15);
    PanelTools::PutOnLineWith( select_count, display_key_mode, 15 );
    PanelTools::PutOnLineWith( display_key_mode, display_item_mode );

    // Time Controls
    if( !( item_first_frame = FLOAT_CTL( panel_funcs->orig_struct, main_panel,
                                         "Low Frame" ) ) )
      throw false;

    if( !( item_current_frame = FLOAT_CTL( panel_funcs->orig_struct, main_panel,
                                           "Current Frame" ) ) )
      throw false;

    if( !( item_jump_to_frame = WBUTTON_CTL( panel_funcs->orig_struct, main_panel, "", 20 ) ) )
      throw false;

    if( !( item_last_frame = FLOAT_CTL( panel_funcs->orig_struct, main_panel,
                                         "High Frame" ) ) )
      throw false;

    if( !( item_range_presets = POPDOWN_CTL( panel_funcs->orig_struct, main_panel,
                                             " Ranges", item_range_preset_text ) ) )
      throw false;

    SET_FLOAT( item_current_frame, fk->GetCurrentTime() * default_fps );
    SET_FLOAT( item_first_frame,   fk->GetDisplayMin()  * default_fps );
    SET_FLOAT( item_last_frame,    fk->GetDisplayMax()  * default_fps );

    PanelTools::PutUnder( list_fold, item_first_frame );
    x = CON_X( item_first_frame );
    y = CON_Y( item_first_frame );
    MOVE_CON( item_first_frame, x + list_x_offset + 40, y );

    PanelTools::PutOnLineWith( item_first_frame, item_current_frame,   20 );
    PanelTools::PutOnLineWith( item_current_frame, item_jump_to_frame, -5 );
    PanelTools::PutOnLineWith( item_jump_to_frame, item_last_frame,    20 );
    PanelTools::PutOnLineWith( item_last_frame, item_range_presets,    -2 );

    // Item List
    if( !( item_list = CANVAS_CTL( panel_funcs->orig_struct, main_panel,
                                   "", 625, 263 ) ) )
      throw false;

    if( !( opengl_item_list = OPENGL_CTL( panel_funcs->orig_struct, main_panel,
                                          "", 625, 263 ) ) )
      throw false;

    if( !( item_scroll = VSLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                      "", 259, 0, 0 ) ) )
      throw false;

    PanelTools::PutUnder( item_first_frame, item_list );

    x = CON_X( item_list );
    y = CON_Y( item_list );
    MOVE_CON( item_list,        x + list_x_offset, y );
    MOVE_CON( opengl_item_list, x + list_x_offset, y );

    w = CON_W( item_list );
    MOVE_CON( item_scroll, x+w-18 + list_x_offset, y );

    UpdateItemListType( fk->GetUseOpenGLItemList() );

    // MultiSlider/Zoom
    if( !( item_zoom = PERCENT_CTL( panel_funcs->orig_struct, main_panel, "Zoom" ) ) )
      throw false;

    SET_FLOAT( item_zoom, fk->GetZoom() * 100.0 );

    if( !( item_multiscroll = DRAGAREA_CTL( panel_funcs->orig_struct, main_panel,
                                            "", 620 + list_x_offset - CON_W( item_zoom ) - 1, 16 ) ) )
      throw false;

    PanelTools::PutUnder( item_list, item_zoom );
    x = CON_X( item_zoom );
    y = CON_Y( item_zoom );
    MOVE_CON( item_zoom, x+8, y );

    PanelTools::PutOnLineWith( item_zoom, item_multiscroll );
    x = CON_X( item_multiscroll );
    y = CON_Y( item_multiscroll );
    MOVE_CON( item_multiscroll, x, y+1 );

    // Tabs
    if( !( tool_options_tabs = TABCHOICE_CTL( panel_funcs->orig_struct, main_panel,
                                             "", tool_option_tab_text ) ) )
      throw false;

    PanelTools::PutUnder( item_multiscroll, tool_options_tabs );
    PanelTools::PutUnder( tool_options_tabs, tool_options_tabs );

    // Group List
    if( !( group_list = CUSTPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                       "Group", 200,
                                       GroupNames, GroupCount ) ) )
      throw false;

    PanelTools::PutOnLineWith( tool_options_tabs, group_list, 12 );
    x = CON_X( group_list );
    y = CON_Y( group_list );
    MOVE_CON( group_list, x, y-5 );

    // About FreeKey button
    if( !( about = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                            "About", 63 ) ) )
      throw false;

    // F1 (Keyboard Help) Button
    if( !( f1 = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                             "F1", 30 ) ) )
      throw false;

    PanelTools::PutOnLineWith( group_list, about );
    PanelTools::PutOnLineWith( about, f1 );

    // Range Controls
    if( !( range_low = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                       "Range   Low", 30, 0, 1000000 ) ) )
      throw false;

    if( !( range_high = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                        "High", 30, 0, 1000000 ) ) )
      throw false;
    SET_INT( range_high, 1000000 );

    PanelTools::PutUnder( tool_options_tabs, range_low );
    PanelTools::PutUnder( range_low, range_low );
    PanelTools::PutUnder( range_low, range_low );
    PanelTools::PutUnder( range_low, range_low );
    PanelTools::PutUnder( range_low, range_low );
    PanelTools::PutOnLineWith( range_low, range_high );

    // Remove Mode
    if( !( remove_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                     "Remove Mode", remove_mode_text, 250 ) ) )
      throw false;
    PanelTools::PutOnLineWith( range_high, remove_mode, 20 );
    y = CON_Y( remove_mode );
    MOVE_CON( remove_mode, 325, y );

    // Step
    if( !( step_amount = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                         "Step Amount", 30, 0, 32767 ) ) )
      throw false;

    if( !( step_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                   "Step Mode", step_mode_text, 250 ) ) )
      throw false;

    PanelTools::PutUnder( range_low, step_amount );
    PanelTools::PutOnLineWith( step_amount, step_mode );

    // Delta
    if( !( delta_amount = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                          "Delta Amount", 30, -32768, 32767 ) ) )
      throw false;

    if( !( delta_mode = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                  "Enable Time Delta" ) ) )
      throw false;

    PanelTools::PutUnder( step_amount, delta_amount );
    PanelTools::PutOnLineWith( delta_amount, delta_mode );

    // Direction
    if( !( direction_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                   "Direction", direction_mode_text, 250 ) ) )
      throw false;

    PanelTools::PutOnLineWith( delta_amount, direction_mode );

    // Scale
    if( !( scale_amount = FLOAT_CTL( panel_funcs->orig_struct, main_panel,
                                          "Scale Factor" ) ) )
      throw false;

    if( !( scale_mode = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                  "Enable Scale Keys" ) ) )
      throw false;

    PanelTools::PutUnder( delta_amount, scale_amount );
    PanelTools::PutOnLineWith( scale_amount, scale_mode );

    // Fix Some Alignments
    LWControl *align1[] = { range_low, step_amount, delta_amount, scale_amount, NULL };
    PanelTools::AlignLabels( align1 );
    PanelTools::PutOnLineWith( range_low, range_high );

    LWControl *align2[] = { remove_mode, step_mode, direction_mode, NULL };
    PanelTools::AlignLabels( align2 );

    PanelTools::PutOnLineWith( delta_amount, delta_mode );
    x = CON_X( delta_mode );
    y = CON_Y( scale_mode );
    MOVE_CON( scale_mode, x, y );

    // Status Line
    if( !( status_line = CANVAS_CTL( panel_funcs->orig_struct, main_panel,                     // Compare Mode pop-up
                                     "", 600, 15 ) ) )
      throw false;
  
    PanelTools::PutUnder( scale_amount, status_line );
    PanelTools::PutUnder( status_line, status_line );

    // FreeKey-ify button
    if( !( freekey = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                  "FreeKey-ify!", 150 ) ) )
      throw false;
    PanelTools::PutOnLineWith( status_line, freekey, -8 );

    // Right-Align the F1 Button
    int x2 = CON_X( item_scroll );
    int w2 = CON_W( item_scroll );
    y = CON_Y( f1 );
    w = CON_W( f1 );
    MOVE_CON( f1, x2+w2-w, y );

    // Move the About Button just to the left of the F1 Button
    y = CON_Y( about );
    w = CON_W( about );
    x = CON_X( f1 );
    MOVE_CON( about, x-w+5, y );

    PAN_SETW( panel_funcs->orig_struct, main_panel, decoded_logo_small->GetWidth() - 30 );

    // Envent Handler Functions
    CON_SETEVENT( about,                   OnAbout,              this );
    CON_SETEVENT( source_scene,            OnSourceFile,         this );
    CON_SETEVENT( output_scene,            OnOutputFile,         this );
    CON_SETEVENT( source_popup,            OnSourcePopup,        this );
    CON_SETEVENT( output_popup,            OnOutputPopup,        this );
    CON_SETEVENT( item_first_frame,        OnFirstFrame,         this );
    CON_SETEVENT( item_last_frame,         OnLastFrame,          this );
    CON_SETEVENT( item_range_presets,      OnItemRangePresets,   this );
    CON_SETEVENT( item_scroll,             OnItemScroll,         this );
    CON_SETEVENT( item_multiscroll,        OnItemMultiScroll2,   this );
    CON_SETEVENT( item_zoom,               OnItemZoom,           this );
    CON_SETEVENT( list_fold,               OnFoldList,           this );
    CON_SETEVENT( list_unfold,             OnUnFoldList,         this );
    CON_SETEVENT( display_key_mode,        OnKeyFrameMode,       this );
    CON_SETEVENT( display_item_mode,       OnDisplayMode,        this );
    CON_SETEVENT( tool_options_tabs,       OnToolOptionsTabs,    this );
    CON_SETEVENT( group_list,              OnGroupList,          this );

    CON_SETEVENT( range_low,               OnRangeLow,           this );
    CON_SETEVENT( range_high,              OnRangeHigh,          this );
    CON_SETEVENT( remove_mode,             OnRemoveMode,         this );
    CON_SETEVENT( step_amount,             OnStepAmount,         this );
    CON_SETEVENT( step_mode,               OnStepMode,           this );
    CON_SETEVENT( delta_amount,            OnDeltaAmount,        this );
    CON_SETEVENT( delta_mode,              OnDeltaMode,          this );
    CON_SETEVENT( scale_amount,            OnScaleAmount,        this );
    CON_SETEVENT( scale_mode,              OnScaleMode,          this );
    CON_SETEVENT( direction_mode,          OnDirectionMode,      this );

    CON_SETEVENT( f1,                      OnF1,                 this );
    CON_SETEVENT( freekey,                 OnFreeKey,            this );

    CON_SETEVENT( item_list,               OnItemList,           this );
//    CON_SETEVENT( opengl_item_list,        OnItemList,           this );

    // Set the Draw Functions
    ival.intv.value = (int)&DrawItemJumpToFrame;
    (*item_jump_to_frame->set)( item_jump_to_frame, CTL_USERDRAW, &ival );

    ival.intv.value = (int)&DrawItemListbox;
    (*item_list->set)( item_list, CTL_USERDRAW, &ival );

    // For some inexplicable reason, the draw function is an event for OpenGL controls.  Go fig.
//    ival.intv.value = (int)&DrawOpenGLItemListbox;
//    (*opengl_item_list->set)( opengl_item_list, CTL_USERDRAW, &ival );
    CON_SETEVENT( opengl_item_list, DrawOpenGLItemListbox, this );

    ival.intv.value = (int)&DrawItemMultiScroll;
    (*item_multiscroll->set)( item_multiscroll, CTL_USERDRAW, &ival );

    ival.intv.value = (int)&DrawStatusLine;
    (*status_line->set)( status_line, CTL_USERDRAW, &ival );

    CON_SETEVENT( title_image, DrawTitleImage, this );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// MakeItemSelectionControls():
bool FreeKeyInterface::MakeItemSelectionControls() {
  try {
    int x, y;
    // ItemSelection Controls
    if( !( select_item_name = STR_CTL( panel_funcs->orig_struct, main_panel,
                                       "Match String", 65 ) ) )
      throw false;

    if( !( select_name_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "Compare", selection_name_text, 100 ) ) )
      throw false;

    if( !( select_toggle = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                       "Mode", select_toggle_text, 83 ) ) )
      throw false;

    if( !( select_item_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "", selection_mode_text, 257 ) ) )
      throw false;

    if( !( select_children_toggle = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                             "And Children" ) ) )
      throw false;

    if( !( apply_select_item_name = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                                 "Apply", 100 ) ) )
      throw false;

    SET_INT( select_item_mode, 1 );
    SET_INT( select_name_mode, 1 );
    SET_INT( select_children_toggle, 1 );
    SET_INT( select_toggle, 0 );

    PanelTools::PutUnder( tool_options_tabs, select_item_name );
    x = CON_X( select_item_name );
    y = CON_Y( select_item_name );
    MOVE_CON( select_item_name, x, y+4 );

    PanelTools::PutOnLineWith( select_item_name, select_name_mode, -2 );
    PanelTools::AlignLabels( select_item_name, select_toggle );
    PanelTools::PutOnLineWith( select_toggle, select_item_mode );
    PanelTools::PutOnLineWith( select_item_mode, select_children_toggle );
    PanelTools::PutOnLineWith( select_children_toggle, apply_select_item_name );

        x  = CON_X( apply_select_item_name );
    int w  = CON_W( apply_select_item_name );
    int x2 = CON_X( item_scroll );
    int w2 = CON_W( item_scroll );

    int y2 = CON_HOTY( select_name_mode );
        y  = CON_HOTY( apply_select_item_name );
    int h  = CON_HOTH( apply_select_item_name );

    // Item Selection Info
    if( !( select_info_area = AREA_CTL( panel_funcs->orig_struct, main_panel,
                                        "", (x2+w2) - (x+w) - 20, (y+h) - y2 ) ) )
      throw false;

    PanelTools::PutOnLineWith( select_name_mode, select_info_area );

    // Event Handler Functions
    CON_SETEVENT( apply_select_item_name,  OnApplySelection,     this );
    CON_SETEVENT( select_item_mode,        OnSelectionMode,      this );
    CON_SETEVENT( select_toggle,           OnSelectionMode,      this );

    // Set the Draw Functions
    ival.intv.value = (int)&DrawSelectInfo;
    (*select_info_area->set)( select_info_area, CTL_USERDRAW, &ival );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// ShowItemSelectionTab():
bool FreeKeyInterface::ShowItemSelectionTab() {
  UNGHOST_CON( select_item_name       );
  UNGHOST_CON( select_name_mode       );
  UNGHOST_CON( apply_select_item_name );
  UNGHOST_CON( select_toggle          );
  UNGHOST_CON( select_item_mode       );
  UNGHOST_CON( select_info_area );
  UNGHOST_CON( fk_int->select_children_toggle );

  REDRAW_CON( select_item_name );

  // Decide if the And Children toggle should be ghosted
  int sel_opt, sel_mode;
  GET_INT( fk_int->select_item_mode, sel_mode );
  GET_INT( fk_int->select_toggle, sel_opt );

  if( (sel_opt == 2) && (sel_mode > 0) )
    GHOST_CON( fk_int->select_children_toggle );

  return true;
}

// HideItemSelectionTab():
bool FreeKeyInterface::HideItemSelectionTab() {
  ERASE_CON( select_item_name       );
  ERASE_CON( select_name_mode       );
  ERASE_CON( apply_select_item_name );
  ERASE_CON( select_toggle          );
  ERASE_CON( select_item_mode       );
  ERASE_CON( select_children_toggle );
  ERASE_CON( select_info_area );
  return true;
}


// MakeKeySelectionControls():
bool FreeKeyInterface::MakeKeySelectionControls() {
  try {
    int x = CON_HOTW( select_info_area );
    int y = CON_HOTH( select_info_area );

    // Keyframe Selection Info
    if( !( key_select_info_area = AREA_CTL( panel_funcs->orig_struct, main_panel,
                                            "", x, y ) ) )
      throw false;

    x = CON_X( select_info_area );
    y = CON_Y( select_info_area );
    MOVE_CON( key_select_info_area, x, y );

    // Set the Draw Functions
    ival.intv.value = (int)&DrawKeySelectInfo;
    (*key_select_info_area->set)( key_select_info_area, CTL_USERDRAW, &ival );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// ShowKeySelectionTab():
bool FreeKeyInterface::ShowKeySelectionTab() {
  UNGHOST_CON( key_select_info_area );

  return true;
}

// HideKeySelectionTab():
bool FreeKeyInterface::HideKeySelectionTab() {
  ERASE_CON( key_select_info_area );

  return true;
}


// MakeGroupControls():
bool FreeKeyInterface::MakeGroupControls() {
  try {
    int x, y;

    // Group Controls
    if( !( group_name = STR_CTL( panel_funcs->orig_struct, main_panel,
                                 "Group Name", 75 ) ) )
      throw false;

    if( !( group_delete = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                       "Del", 50 ) ) )
      throw false;

    if( !( group_edit_mode = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                         "Modify", group_edit_text, 250 ) ) )
      throw false;

    if( !( group_edit_apply = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                           "Apply", 100 ) ) )
      throw false;

    PanelTools::PutUnder( tool_options_tabs, group_name );
    x = CON_X( group_name );
    y = CON_Y( group_name );
    MOVE_CON( group_name, x, y+4 );

    PanelTools::PutOnLineWith( group_name, group_delete    );
    PanelTools::AlignLabels( group_name, group_edit_mode );
    PanelTools::PutOnLineWith( group_edit_mode, group_edit_apply );

    // Event Handler Functions
    CON_SETEVENT( group_name,              OnGroupName,          this );
    CON_SETEVENT( group_delete,            OnGroupDel,           this );
    CON_SETEVENT( group_edit_apply,        OnGroupEditApply,     this );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// ShowGroupTab():
bool FreeKeyInterface::ShowGroupTab() {
  UNGHOST_CON( group_name       );
  UNGHOST_CON( group_delete     );
  UNGHOST_CON( group_edit_mode  );
  UNGHOST_CON( group_edit_apply );

  REDRAW_CON( group_name );
  return true;
}

// HideGroupTab():
bool FreeKeyInterface::HideGroupTab() {
  ERASE_CON( group_name       );
  ERASE_CON( group_delete     );
  ERASE_CON( group_edit_mode  );
  ERASE_CON( group_edit_apply );
  return true;
}


// MakeNumericControls():
bool FreeKeyInterface::MakeNumericControls() {
  try {
//    int x, y, w, h, pw;

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// ShowNumericTab():
bool FreeKeyInterface::ShowNumericTab() {
  return true;
}

// HideNumericTab():
bool FreeKeyInterface::HideNumericTab() {
  return true;
}

// MakeOptionControls():
bool FreeKeyInterface::MakeOptionControls() {
  try {
    int x, y;

    // Undo Levels
    if( !( opt_undo_levels = MINISLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                             "Undo Levels", 30, 0, 128 ) ) )
      throw false;

    // Fractional Keyframes
    if( !( opt_fractional_frames = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                             "Allow Fractional Frames" ) ) )
      throw false;

    // Item List Draw Style
    if( !( opt_draw_style = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                        "Item List Draw Style", opt_draw_style_text, 150 ) ) )
      throw false;

    SET_INT( opt_draw_style, fk->GetDrawStyle() );

    // Use OpenGL Item List
    if( !( opt_use_opengl_item_list = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                                "Use OpenGL Item List" ) ) )
      throw false;

    SET_INT( opt_use_opengl_item_list, (int)fk->GetUseOpenGLItemList() );

    // Use OpenGL Legacy Alignment
    if( !( opt_use_opengl_legacy = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                             "Use Legacy OpenGL Icon Alignment" ) ) )
      throw false;

    SET_INT( opt_use_opengl_legacy, (int)GLTLWIcons::GetLegacyOffset() );

    PanelTools::PutUnder( tool_options_tabs, opt_undo_levels );
    x = CON_X( opt_undo_levels );
    y = CON_Y( opt_undo_levels );
    MOVE_CON( opt_undo_levels, x, y+4 );

    PanelTools::PutOnLineWith( opt_undo_levels, opt_fractional_frames, 25 );
    PanelTools::PutOnLineWith( opt_fractional_frames, opt_draw_style,  25 );

    PanelTools::PutUnder( opt_fractional_frames, opt_use_opengl_item_list );
    x = CON_X( opt_fractional_frames );
    y = CON_Y( opt_use_opengl_item_list );
    MOVE_CON( opt_use_opengl_item_list, x, y );

    PanelTools::PutUnder( opt_use_opengl_item_list, opt_use_opengl_legacy );
    y = CON_Y( opt_use_opengl_legacy );
    MOVE_CON( opt_use_opengl_legacy, x, y );

    // Event Handler Functions
    CON_SETEVENT( opt_draw_style,           OnOptDrawStyle,         this );
    CON_SETEVENT( opt_use_opengl_item_list, OnOptUseOpenGLItemList, this );
    CON_SETEVENT( opt_use_opengl_legacy,    OnOptUseOpenGLLegacy,   this );

    return true;
  } catch( bool ) {
    return false;
  }

  return true;
}

// ShowOptionTab():
bool FreeKeyInterface::ShowOptionTab() {
  UNGHOST_CON( opt_fractional_frames    );
  UNGHOST_CON( opt_undo_levels          );
  UNGHOST_CON( opt_draw_style           );
  UNGHOST_CON( opt_use_opengl_item_list );
  UNGHOST_CON( opt_use_opengl_legacy    );

  return true;
}

// HideOptionTab():
bool FreeKeyInterface::HideOptionTab() {
  ERASE_CON( opt_fractional_frames    );
  ERASE_CON( opt_undo_levels          );
  ERASE_CON( opt_draw_style           );
  ERASE_CON( opt_use_opengl_item_list );
  ERASE_CON( opt_use_opengl_legacy    );

  return true;
}

// OnToolOptionsTabs()
//  Handles changing which tool tab has been selected
void OnToolOptionsTabs(LWControl *ctl, FreeKeyInterface *panel) {
  int tab;
  GET_INT( fk_int->tool_options_tabs, tab );

  fk->SetToolOptionsTab( (fk_tool_option_tab_ids) tab );
  fk_int->ShowToolOptionsTab( fk->GetToolOptionsTab() );
}

// ShowToolOptionsTab():
//  Toggles which tool tab is currently displayed.  This also changes
//   the currently displayed tab
bool FreeKeyInterface::ShowToolOptionsTab( fk_tool_option_tab_ids tab_id ) {
  SET_INT( fk_int->tool_options_tabs, (int)tab_id );

  // First erase all the other tabs
  if( tab_id != TOOLTAB_ITEM_SELECTION )
    HideItemSelectionTab();

  if( tab_id != TOOLTAB_KEY_SELECTION )
    HideKeySelectionTab();

  if( tab_id != TOOLTAB_GROUP )
    HideGroupTab();

  if( tab_id != TOOLTAB_NUMERIC )
    HideNumericTab();

  if( tab_id != TOOLTAB_OPTIONS )
    HideOptionTab();

  // Then show the visible one
  switch( tab_id ) {
  case TOOLTAB_ITEM_SELECTION:
    ShowItemSelectionTab();
    break;

  case TOOLTAB_KEY_SELECTION:
    ShowKeySelectionTab();
    break;

  case TOOLTAB_GROUP:
    ShowGroupTab();
    break;

  case TOOLTAB_NUMERIC:
    ShowNumericTab();
    break;

  case TOOLTAB_OPTIONS:
    ShowOptionTab();
    break;
  }

  return true;
}
