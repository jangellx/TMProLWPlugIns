//
// KeyStrainer-Interface.cpp
//

#include "KeyStrainer-Interface.h"
#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>
#include <stdio.h>
#include <math.h>

#include <lightwave/LWChannelScan/LWChannelScan.h>

static KeyStrainer_Interface *kint = NULL;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Key Strainer",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Keyframe Reduction/Optimization for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

const char *for_popup_strings[] =
  { "Selected Items",    "Selected Items And Descendants",
    "Current Item Only", "Current Item And Descendants",
    "All Items", NULL };

const char *mode_popup_strings[] = {
  "Keep/Kill Step",
  "Smart Freeze",
  "Stepped Freeze",
//  "Itterative Reduction",
  NULL };

enum KSReductionModes {
  KSR_KEEP_KILL,
  KSR_SMART_FREEZE,
  KSR_STEPPED_FREEZE,
  KSR_ITTERATIVE_REDUCTION,
};

const char *freeze_key_shape_strings[] = {
  "TCB", "Hermite", "Bezier", "Linear", "Stepped", NULL };

// Constructor():
KeyStrainer_Interface::KeyStrainer_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), ctrl_down(false), do_process(false),
    progress_panel(NULL), progress_open(false), progress_total(1), progress_current(0),
    shift_down(false) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  iteminfo    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  objectinfo  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  sceneinfo   = (LWSceneInfo     *)global( LWSCENEINFO_GLOBAL,     GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );
  chaninfo    = (LWChannelInfo   *)global( LWCHANNELINFO_GLOBAL,   GFUSE_TRANSIENT );
  env_funcs   = (LWEnvelopeFuncs *)global( LWENVELOPEFUNCS_GLOBAL, GFUSE_TRANSIENT );
  raster      = (LWRasterFuncs   *)global( LWRASTERFUNCS_GLOBAL,   GFUSE_TRANSIENT );

  kint = this;

  // Create our work envelope
  workspace = (*env_funcs->create)( NULL, "KeyStrainerWorkspace", LWET_FLOAT );
//  (*env_funcs->egSet)( workspace, NULL, LWENVTAG_VISIBLE, 0 );
}

// Destructor():
KeyStrainer_Interface::~KeyStrainer_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  if( progress_panel != NULL )
    (*panel_funcs->destroy)( progress_panel );

  (*env_funcs->destroy)( workspace );

  kint = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
int KeyStrainer_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Key Strainer -- %c2001 Joe Angell, TM Productions", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Reduce Key Range
  key_from_float                = FLOAT_CTL(  panel_funcs, panel, "Reduce Keys From" );
  key_to_float                  = FLOAT_CTL(  panel_funcs, panel, "Through" );

  // Mode
  mode_popup                    = WPOPUP_CTL( panel_funcs, panel, "Reduction Mode", mode_popup_strings, 215 );
  preview_toggle                = BOOLBUTTON_CTL( panel_funcs, panel, "Preview" );

  // - Keep/Kill Range
  keep_step_float               = FLOAT_CTL(  panel_funcs, panel, "Keep Step" );
  kill_step_float               = FLOAT_CTL(  panel_funcs, panel, "Kill Step" );

  // - Reduction Threshold
  reduction_threshold_float     = FLOAT_CTL(  panel_funcs, panel, "Threshold" );

  // - Freeze Step
  freeze_step_float             = FLOAT_CTL(  panel_funcs, panel, "Freeze Step" );

  // - Freeze Modifiers
  freeze_modifiers_toggle       = BOOL_CTL(   panel_funcs, panel, "Freeze Modifiers" );

  // - Freeze Key Shape
  freeze_key_shape              = WPOPUP_CTL( panel_funcs, panel, "Key As", freeze_key_shape_strings, 80 );

  // For Popup
  for_popup                     = WPOPUP_CTL( panel_funcs, panel, "For", for_popup_strings, 215 );

  // About
  about_button                  = WBUTTON_CTL( panel_funcs, panel, "About", 60 );

  // Align the above controls
  LWControl *aligner[] = { key_from_float, mode_popup, reduction_threshold_float, for_popup, NULL };
  PanelTools::AlignLabels( aligner );
  PanelTools::PutOnLineWith( key_from_float, key_to_float );
  PanelTools::PutOnLineWith( mode_popup, preview_toggle, 5 );

  int x = CON_X( for_popup );
  int y = CON_Y( for_popup );
  MOVE_CON( for_popup, x, (y + 12) );
  PanelTools::PutOnLineWith( for_popup, about_button, 5 );

  PanelTools::AlignLabels( reduction_threshold_float, keep_step_float );
  x = CON_X( keep_step_float );
  y = CON_Y( reduction_threshold_float );
  MOVE_CON( keep_step_float, x, y );
  PanelTools::PutOnLineWith( keep_step_float, kill_step_float );

  PanelTools::AlignLabels( reduction_threshold_float, freeze_step_float );
  x = CON_X( freeze_step_float );
  MOVE_CON( freeze_step_float, x, y );
  PanelTools::PutOnLineWith( freeze_step_float, freeze_modifiers_toggle );
  PanelTools::PutOnLineWith( freeze_modifiers_toggle, freeze_key_shape );

  // Channels
  position_button       = WBUTTON_CTL(    panel_funcs, panel, "Position", 67 );
  x_button              = BOOLBUTTON_CTL( panel_funcs, panel, "X" );
  y_button              = BOOLBUTTON_CTL( panel_funcs, panel, "Y" );
  z_button              = BOOLBUTTON_CTL( panel_funcs, panel, "Z" );

  rotation_button       = WBUTTON_CTL(    panel_funcs, panel, "Rotation", 67 );
  h_button              = BOOLBUTTON_CTL( panel_funcs, panel, "H" );
  p_button              = BOOLBUTTON_CTL( panel_funcs, panel, "P" );
  b_button              = BOOLBUTTON_CTL( panel_funcs, panel, "B" );

  scale_button          = WBUTTON_CTL(    panel_funcs, panel, "Scale",    67 );
  sx_button             = BOOLBUTTON_CTL( panel_funcs, panel, "X" );
  sy_button             = BOOLBUTTON_CTL( panel_funcs, panel, "Y" );
  sz_button             = BOOLBUTTON_CTL( panel_funcs, panel, "Z" );

  PanelTools::AlignLabels(   for_popup, position_button );
  PanelTools::PutOnLineWith( position_button, rotation_button, 3 );
  PanelTools::PutOnLineWith( rotation_button, scale_button,    3 );

  PanelTools::AlignLabels(   position_button, x_button );
  PanelTools::PutOnLineWith( x_button, y_button, -5 );
  PanelTools::PutOnLineWith( y_button, z_button, -5 );

  PanelTools::AlignLabels(   rotation_button, h_button );
  PanelTools::PutOnLineWith( h_button, p_button, -5 );
  PanelTools::PutOnLineWith( p_button, b_button, -5 );

  PanelTools::AlignLabels(   scale_button, sx_button );
  PanelTools::PutOnLineWith( sx_button, sy_button, -5 );
  PanelTools::PutOnLineWith( sy_button, sz_button, -5 );

  all_other_channels_button = BOOLBUTTON_CTL( panel_funcs, panel, "All Other Channels" );
  PanelTools::PutOnLineWith( sz_button, all_other_channels_button, 5 );

  // Preview
  preview_show_original_curve = BOOL_CTL( panel_funcs, panel, "Show Original Curve" );
  preview_show_reduced_curve  = BOOL_CTL( panel_funcs, panel, "Show Reduced Curve" );
  preview_show_original_keys  = BOOL_CTL( panel_funcs, panel, "Show Original Keys" );
  preview_show_reduced_keys   = BOOL_CTL( panel_funcs, panel, "Show Reduced Keys" );
  preview_canvas              = CANVAS_CTL( panel_funcs, panel, "", 437, 200 );
  preview_channel             = CHANNEL_CTL( panel_funcs, panel, "All Channels", 440, 100 );

  // Update ghosts and finalize alignment
  HandleModePopup(     mode_popup,      NULL );
  HandlePreviewToggle( preview_toggle,  NULL );

  // Initalize some controls
  SET_FLOAT( key_from_float,  (double)ui->previewStart );
  SET_FLOAT( key_to_float,    (double)ui->previewEnd   );

  SET_FLOAT( reduction_threshold_float, 0.025 );

  SET_FLOAT( keep_step_float, 1.0 );
  SET_FLOAT( kill_step_float, 1.0 );

  SET_FLOAT( freeze_step_float, 1.0 );

  SET_INT(  x_button, 1 );
  SET_INT(  y_button, 1 );
  SET_INT(  z_button, 1 );
  SET_INT(  h_button, 1 );
  SET_INT(  p_button, 1 );
  SET_INT(  b_button, 1 );
  SET_INT( sx_button, 1 );
  SET_INT( sz_button, 1 );
  SET_INT( sy_button, 1 );
  SET_INT( all_other_channels_button, 0 );

  SET_INT( preview_show_original_curve, 1 );
  SET_INT( preview_show_reduced_curve,  1 );
  SET_INT( preview_show_original_keys,  1 );
  SET_INT( preview_show_reduced_keys,   1 );

  LWChanGroupID cg = (*iteminfo->chanGroup)( ui->selItems[0] );
  if( cg != NULL ) {
    SET_INT( preview_channel, (int)(*chaninfo->nextChannel)( cg, NULL) );
  }

  // Resize the Panel
  PAN_SETH( panel_funcs, panel, (CON_Y( x_button )) + (CON_H( x_button )) );

  // Event Handlers
  CON_SETEVENT( key_from_float,              HandleFromFloat,                 NULL );
  CON_SETEVENT( key_to_float,                HandleToFloat,                   NULL );
  CON_SETEVENT( mode_popup,                  HandleModePopup,                 NULL );
  CON_SETEVENT( preview_toggle,              HandlePreviewToggle,             NULL );
  CON_SETEVENT( about_button,                HandleAboutButton,               NULL );

  CON_SETEVENT( keep_step_float,             HandleKeepFloat,                 NULL );
  CON_SETEVENT( kill_step_float,             HandleKillFloat,                 NULL );
  CON_SETEVENT( reduction_threshold_float,   HandleReductionThreshold,        NULL );
  CON_SETEVENT( freeze_step_float,           HandleFreezeFloat,               NULL );
  CON_SETEVENT( freeze_modifiers_toggle,     HandleFreezeModifiers,           NULL );
  CON_SETEVENT( freeze_key_shape,            HandleFreezeKeyShape,            NULL );

  CON_SETEVENT( preview_show_original_curve, HandlePreviewShowOriginalCurve,  NULL );
  CON_SETEVENT( preview_show_reduced_curve,  HandlePreviewShowReducedCurve,   NULL );
  CON_SETEVENT( preview_show_original_keys,  HandlePreviewShowOriginalKeys,   NULL );
  CON_SETEVENT( preview_show_reduced_keys,   HandlePreviewShowReducedKeys,    NULL );
  CON_SETEVENT( preview_channel,             HandlePreviewChannel,            NULL );

  CON_SETEVENT( position_button,             HandlePositionButton,            NULL );
  CON_SETEVENT( rotation_button,             HandleRotationButton,            NULL );
  CON_SETEVENT( scale_button,                HandleScaleButton,               NULL );
  CON_SETEVENT( all_other_channels_button,   HandleAllOtherChannelsButton,    NULL );

  // Set the Draw Callbacks
  (*panel_funcs->set)( panel, PAN_USERDRAW, DrawPanel );

  ival.intv.value = (int)&DrawPreview;
  (*preview_canvas->set)( preview_canvas, CTL_USERDRAW, &ival );

  // Set some other panel callbacks
  (*panel_funcs->set)( panel, PAN_USERKEYS,   HandleKeys   );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS, HandleKeyUps );

  // Open the Panel
  int retval = (*panel_funcs->open)( panel, PANF_BLOCKING | PANF_CANCEL | PANF_PASSALLKEYS );
  if( retval > 0 )
    do_process = true;

  return retval;
}

// BuildItemList():
bool KeyStrainer_Interface::BuildItemList() {
  if( ui->selItems[0] == NULL )
    return false;

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  return true;
}

// DrawMainPanel() {
void DrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( kint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( kint->panel_funcs, kint->panel );
  int pw = PAN_GETW( kint->panel_funcs, kint->panel );

  // Horizontal Dividers
  int y = CON_Y( kint->keep_step_float );
  int h = CON_H( kint->keep_step_float );
  PanelDraw::HorizontalDivider( kint->panel, &df, 0, y + h + 3, pw, 1 );

  int state;
  GET_INT( kint->preview_toggle, state );

  if( state == 1 ) {
    y = CON_Y( kint->x_button );
    h = CON_H( kint->x_button );
    PanelDraw::HorizontalDivider( kint->panel, &df, 0, y + h + 3, pw, 1 );
  }
}

// HandleKeys():
//  Used mainly to close the panel with Enter/Esc, since Enter
//   doesn't seem to be doing it.  It also allows for a bit of
//   keyboard control of the panel, too.
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  int state;

  switch( key ) {
    case LWDK_CTRL:             // Control
      kint->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      kint->shift_down = true;
      break;


    case '\r':                  // OK
      (*kint->panel_funcs->close)( kint->panel );
      kint->do_process = true;
      break;

    case 27:                    // Cancel (ESC)
      (*kint->panel_funcs->close)( kint->panel );
      break;

/*
    case LWDK_SC_UP:
      if( kint->ctrl_down ) {   // Previous Mode
        GET_INT( kint->mode_popup, state );
        if( state > 0 ) {
          SET_INT( kint->mode_popup, state-1 );
          HandleModePopup( kint->mode_popup, NULL );
        }
      } else {                  // Previous For
        GET_INT( kint->for_popup, state );
        if( state > 0 ) {
          SET_INT( kint->for_popup, state-1 );
          HandleModePopup( kint->for_popup, NULL );
        }
      }
      break;

    case LWDK_SC_DOWN:
      if( kint->ctrl_down ) {   // Next Mode
        GET_INT( kint->mode_popup, state );
        if( state < 9 ) {
          if( (state == 0) || (state == 5) )
            SET_INT( kint->mode_popup, state+2 );
          else
            SET_INT( kint->mode_popup, state+1 );
          HandleModePopup( kint->mode_popup, NULL );
        }
      } else {                  // Next For
        GET_INT( kint->for_popup, state );
        if( state < 4 ) {
          SET_INT( kint->for_popup, state+1 );
          HandleModePopup( kint->for_popup, NULL );
        }
      }
      break;
*/

    case 9:                     // Activate First Field
      ACTIVATE_CON( kint->key_from_float );
      break;

    // Channels
    case '1':
      GET_INT( kint->x_button, state );
      SET_INT( kint->x_button, (state ? 0 : 1) );
      break;

    case '2':
      GET_INT( kint->y_button, state );
      SET_INT( kint->y_button, (state ? 0 : 1) );
      break;

    case '3':
      GET_INT( kint->z_button, state );
      SET_INT( kint->z_button, (state ? 0 : 1) );
      break;


    case '4':
      GET_INT( kint->h_button, state );
      SET_INT( kint->h_button, (state ? 0 : 1) );
      break;

    case '5':
      GET_INT( kint->p_button, state );
      SET_INT( kint->p_button, (state ? 0 : 1) );
      break;

    case '6':
      GET_INT( kint->b_button, state );
      SET_INT( kint->b_button, (state ? 0 : 1) );
      break;


    case '7':
      GET_INT( kint->sx_button, state );
      SET_INT( kint->sx_button, (state ? 0 : 1) );
      break;

    case '8':
      GET_INT( kint->sy_button, state );
      SET_INT( kint->sy_button, (state ? 0 : 1) );
      break;

    case '9':
      GET_INT( kint->sz_button, state );
      SET_INT( kint->sz_button, (state ? 0 : 1) );
      break;


    case 'p':
    case 'P':
      HandlePositionButton( kint->position_button, NULL );
      break;

    case 'r':
    case 'R':
      HandleRotationButton( kint->rotation_button, NULL );
      break;

    case 's':
    case 'S':
      HandleScaleButton( kint->scale_button, NULL );
      break;

    case LWDK_F1:
      HandleAboutButton( kint->about_button, NULL );
      break;
  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  switch( key ) {
    case LWDK_CTRL:            // Control
      kint->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      kint->shift_down = false;
      break;
  }
}

//
// Event Handlers
//

// HandleFromFloat():
void HandleFromFloat( LWControl *con, void *data ) {
  double from, to;
  GET_FLOAT( kint->key_from_float, from );
  GET_FLOAT( kint->key_to_float,   to   );

  if( from < 0.0 )
    from = 0.0;

  if( from > to )
    to = from;

  SET_FLOAT( kint->key_from_float, from );
  SET_FLOAT( kint->key_to_float,   to   );

  kint->ReduceForPreview();
}

// HandleToFloat():
void HandleToFloat( LWControl *con, void *data ) {
  double from, to;
  GET_FLOAT( kint->key_from_float, from );
  GET_FLOAT( kint->key_to_float,   to   );

  if( to < 0.0 )
    to = 0.0;

  if( from > to )
    from = to;

  SET_FLOAT( kint->key_from_float, from );
  SET_FLOAT( kint->key_to_float,   to   );

  kint->ReduceForPreview();
}

// HandleModePopup():
void HandleModePopup( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->mode_popup, state );

  GHOST_CON( kint->keep_step_float );
  GHOST_CON( kint->kill_step_float );
  GHOST_CON( kint->reduction_threshold_float );
  GHOST_CON( kint->freeze_step_float );
  GHOST_CON( kint->freeze_step_float );
  GHOST_CON( kint->freeze_modifiers_toggle );
  GHOST_CON( kint->freeze_key_shape );

  ERASE_CON( kint->keep_step_float );
  ERASE_CON( kint->kill_step_float );
  ERASE_CON( kint->reduction_threshold_float );
  ERASE_CON( kint->freeze_step_float );
  ERASE_CON( kint->freeze_step_float );
  ERASE_CON( kint->freeze_modifiers_toggle );
  ERASE_CON( kint->freeze_key_shape );

  switch( state ) {
    case KSR_KEEP_KILL:             // Kill/Keep
      UNGHOST_CON( kint->keep_step_float );
      UNGHOST_CON( kint->kill_step_float );
      break;

    case KSR_ITTERATIVE_REDUCTION:  // Itterative Reduction
      UNGHOST_CON( kint->reduction_threshold_float );
      break;

    case KSR_SMART_FREEZE:          // Smart Freeze
      UNGHOST_CON( kint->reduction_threshold_float );
      UNGHOST_CON( kint->freeze_modifiers_toggle );
      UNGHOST_CON( kint->freeze_key_shape );
      PanelTools::PutOnLineWith( kint->reduction_threshold_float, kint->freeze_modifiers_toggle );
      PanelTools::PutOnLineWith( kint->freeze_modifiers_toggle, kint->freeze_key_shape, 10 );
      break;

    case KSR_STEPPED_FREEZE:        // Total Freeze
      UNGHOST_CON( kint->freeze_step_float );
      UNGHOST_CON( kint->freeze_modifiers_toggle );
      UNGHOST_CON( kint->freeze_key_shape );
      PanelTools::PutOnLineWith( kint->freeze_step_float, kint->freeze_modifiers_toggle );
      PanelTools::PutOnLineWith( kint->freeze_modifiers_toggle, kint->freeze_key_shape, 10 );
      break;
  }

  kint->ReduceForPreview();
}

// HandlePreviewToggle():
void HandlePreviewToggle( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->preview_toggle, state );

  if( state == 0 ) {
    MOVE_CON( kint->preview_show_original_curve, 4, -1000 );
    MOVE_CON( kint->preview_show_reduced_curve,  4, -1000 );
    MOVE_CON( kint->preview_show_original_keys,  4, -1000 );
    MOVE_CON( kint->preview_show_reduced_keys,   4, -1000 );
    MOVE_CON( kint->preview_channel, 4, -1000 );
    MOVE_CON( kint->preview_canvas,  4, -1000 );

    GHOST_CON( kint->preview_show_original_curve );
    GHOST_CON( kint->preview_show_reduced_curve  );
    GHOST_CON( kint->preview_show_original_keys  );
    GHOST_CON( kint->preview_show_reduced_keys   );
    GHOST_CON( kint->preview_channel );
    GHOST_CON( kint->preview_canvas );

    ERASE_CON( kint->preview_show_original_curve );
    ERASE_CON( kint->preview_show_reduced_curve  );
    ERASE_CON( kint->preview_show_original_keys  );
    ERASE_CON( kint->preview_show_reduced_keys   );
    ERASE_CON( kint->preview_channel );
    ERASE_CON( kint->preview_canvas );

    int y = CON_Y( kint->x_button );
    int h = CON_H( kint->x_button );

    PAN_SETH( (kint->panel_funcs), (kint->panel), (y + h + 37) );
    (*kint->panel_funcs->draw)( kint->panel, DR_REFRESH );
  } else {
    UNGHOST_CON( kint->preview_show_original_curve );
    UNGHOST_CON( kint->preview_show_reduced_curve  );
    UNGHOST_CON( kint->preview_show_original_keys  );
    UNGHOST_CON( kint->preview_show_reduced_keys   );
    UNGHOST_CON( kint->preview_channel );
    UNGHOST_CON( kint->preview_canvas );

    PanelTools::AlignLabels( kint->x_button, kint->preview_show_original_curve, 20 );
    PanelTools::PutOnLineWith( kint->preview_show_original_curve, kint->preview_show_reduced_curve, 20 );
    PanelTools::AlignLabels( kint->preview_show_original_curve, kint->preview_show_original_keys );
    PanelTools::AlignLabels( kint->preview_show_reduced_curve, kint->preview_show_reduced_keys );

    PanelTools::PutUnder( kint->preview_show_original_keys, kint->preview_canvas, 10 );
    PanelTools::PutUnder( kint->preview_canvas, kint->preview_channel );

    int y = CON_Y( kint->preview_channel );
    int h = CON_H( kint->preview_channel );

    PAN_SETH( (kint->panel_funcs), (kint->panel), (y + h + 37) );
    (*kint->panel_funcs->draw)( kint->panel, DR_REFRESH );

    kint->ReduceForPreview();
  }
}

// HandleKeepFloat():
void HandleKeepFloat( LWControl *con, void *data ) {
  double value;
  GET_FLOAT( kint->keep_step_float, value );

  if( value < 0.0 )
    value = 1.0;

  SET_FLOAT( kint->keep_step_float, value );
  kint->ReduceForPreview();
}

// HandleKillFloat():
void HandleKillFloat( LWControl *con, void *data ) {
  double value;
  GET_FLOAT( kint->kill_step_float, value );

  if( value < 0.0 )
    value = 1.0;

  SET_FLOAT( kint->kill_step_float, value );
  kint->ReduceForPreview();
}

// HandleFreezeFloat():
void HandleFreezeFloat( LWControl *con, void *data ) {
  double value;
  GET_FLOAT( kint->freeze_step_float, value );

  if( value < 0.01 )
    value = 1.0;

  SET_FLOAT( kint->freeze_step_float, value );
  kint->ReduceForPreview();
}

// HandleFreezeModifiers():
void HandleFreezeModifiers( LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandleFreezeKeyShape():
void HandleFreezeKeyShape( LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandleAboutButton():
void HandleAboutButton( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( kint->panel_funcs, "Key Strainer", about_text, kint->panel, true );
}

// HandlePreviewShowOriginalCurve():
void HandlePreviewShowOriginalCurve( LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandlePreviewShowReducedCurve():
void HandlePreviewShowReducedCurve(  LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandlePreviewShowOriginalKeys():
void HandlePreviewShowOriginalKeys(  LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandlePreviewShowReducedKeys():
void HandlePreviewShowReducedKeys(   LWControl *con, void *data ) {
  kint->ReduceForPreview();
}

// HandlePreviewChannel():
void HandlePreviewChannel( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->preview_channel, state );
  SET_INT( kint->preview_channel, state );

  kint->ReduceForPreview();
}

// HandleReductionThreshold():
void HandleReductionThreshold( LWControl *con, void *data ) {
  double value;
  GET_FLOAT( kint->reduction_threshold_float, value );

  if( value < 0.0 )
    value = 0.01;

  if( value > 1.0 )
    value = 1.0;

  SET_FLOAT( kint->reduction_threshold_float, value );
  kint->ReduceForPreview();
}

// HandlePositionButton():
void HandlePositionButton( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->x_button, state );

  if( kint->shift_down ) {  // Invert selection
    SET_INT( kint->x_button, (state ? 0 : 1 ) );

    GET_INT( kint->y_button, state );
    SET_INT( kint->y_button, (state ? 0 : 1 ) );

    GET_INT( kint->z_button, state );
    SET_INT( kint->z_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( kint->x_button, (state ? 0 : 1 ) );
    SET_INT( kint->y_button, (state ? 0 : 1 ) );
    SET_INT( kint->z_button, (state ? 0 : 1 ) );
  }
}

// HandleRotationButton():
void HandleRotationButton( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->h_button, state );

  if( kint->shift_down ) {  // Invert selection
    SET_INT( kint->h_button, (state ? 0 : 1 ) );

    GET_INT( kint->p_button, state );
    SET_INT( kint->p_button, (state ? 0 : 1 ) );

    GET_INT( kint->b_button, state );
    SET_INT( kint->b_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( kint->h_button, (state ? 0 : 1 ) );
    SET_INT( kint->p_button, (state ? 0 : 1 ) );
    SET_INT( kint->b_button, (state ? 0 : 1 ) );
  }
}

// HandleScaleButton():
void HandleScaleButton( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->sx_button, state );

  if( kint->shift_down ) {  // Invert selection
    SET_INT( kint->sx_button, (state ? 0 : 1 ) );

    GET_INT( kint->sy_button, state );
    SET_INT( kint->sy_button, (state ? 0 : 1 ) );

    GET_INT( kint->sz_button, state );
    SET_INT( kint->sz_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( kint->sx_button, (state ? 0 : 1 ) );
    SET_INT( kint->sy_button, (state ? 0 : 1 ) );
    SET_INT( kint->sz_button, (state ? 0 : 1 ) );
  }
}

// HandleAllOtherChannelsButton():
void HandleAllOtherChannelsButton( LWControl *con, void *data ) {
  int state;
  GET_INT( kint->all_other_channels_button, state );

  if( state != 0 ) {
    if( (*kint->message->yesNo)( "Process All Other Channels?",
                                 "Warning:  This feature will affect keys on all non-motion channels, including",
                                 "envelopes and plug-in channels for the chosen items.  Are you sure?" ) == 0 ) {
      SET_INT( kint->all_other_channels_button, 0 );
    }
  }
}

// DrawPreview():
void DrawPreview( LWControl *con, void *data, DrMode mode ) {
  CDrawFuncs df( kint->panel_funcs->drawFuncs );

  int cx = CON_HOTX( kint->preview_canvas );
  int cy = CON_HOTY( kint->preview_canvas );
  int cw = CON_HOTW( kint->preview_canvas );
  int ch = CON_HOTH( kint->preview_canvas );

  int gx = cx+40;
  int gy = cy+10;
  int gw = cw-50;
  int gh = ch-30;

  // Fill the BG to grey and draw a bounding box
  df.DrawRGBBox( kint->panel, 128, 128, 128, cx, cy, cw, ch );
  PanelDraw::EmptyBox( kint->panel, &df, gx-1, gy-1, gw+2, gh+2, 1, 1, 0,0,0, 0,0,0 );

  // Label the extents
  char buffer[100];
  double start_frame, end_frame;

  // - Time
  GET_FLOAT( kint->key_from_float, start_frame );
  GET_FLOAT( kint->key_to_float, end_frame );
  start_frame -= 5.0;
  end_frame += 5.0;
  double total_frames = end_frame - start_frame;

  sprintf( buffer, "%g", start_frame + 5.0 );
  df.WriteText( kint->panel, buffer, COLOR_BLACK, gx + (int)(5.0/total_frames * gw)- (df.TextWidth( kint->panel, buffer)/2), gy + gh + 2 );

  sprintf( buffer, "%g", end_frame - 5.0 );
  df.WriteText( kint->panel, buffer, COLOR_BLACK, gx + (int)((total_frames - 5.0)/total_frames * gw) - (df.TextWidth( kint->panel, buffer)/2), gy + gh + 2 );

  df.WriteText( kint->panel, "Frame", COLOR_BLACK, gx + gw/2 - df.TextWidth( kint->panel, "Frame")/2, gy + gh + 2 );

  // - Value
  int tempi;
  LWChannelID channel;
  GET_INT( kint->preview_channel, tempi );
  channel = (LWChannelID)tempi;

  double fps        = kint->sceneinfo->framesPerSecond;
  double value_low  = (*kint->chaninfo->channelEvaluate)( channel, (start_frame-5.0)/fps );
  double value_high = value_low;
  double value;

  for( double z=start_frame+1.0; z < end_frame; z++ ) {
    value = (*kint->chaninfo->channelEvaluate)( channel, z/fps );
    if( value < value_low )
      value_low = value;

    if( value > value_high )
      value_high = value;
  }

  if( fabs(value_high - value_low) < 0.000000001 ) {
    value_low  -= 0.5;
    value_high += 0.5;
  }

  sprintf( buffer, "%.2g", value_low );
  df.WriteText( kint->panel, buffer, COLOR_BLACK, gx - df.TextWidth( kint->panel, buffer) - 5, gy + gh-15 );

  sprintf( buffer, "%.2g", value_high );
  df.WriteText( kint->panel, buffer, COLOR_BLACK, gx - df.TextWidth( kint->panel, buffer) - 5, gy + 5 );

  df.WriteText( kint->panel, "Value", COLOR_BLACK, gx - df.TextWidth( kint->panel, "Value") - 5, gy + (gh/2 - 8) );

  // Pad Low and High to be graph ranges
  value_low  += value_low  * 0.1;
  value_high += value_high * 0.1;

  // Create the graph raster image
  LWRasterID graph = (*kint->raster->create)( gw, gh, 0 );
  (*kint->raster->drawRGBBox)( graph, 128, 128, 128, 0, 0, gw, gh );

  double now;
  double value_range  = value_high - value_low;

  // Draw the 0 value line
  int x, y;
  if( (0 < value_high) && (0 > value_low) ) {
    y = int((value_range + value_low)/value_range * (double)gh);
    (*kint->raster->drawLine)( graph, RGB_(100,100,100), 0, y, gw, y );
  }

  // Mark the Start Frame and End Frame
  x = (int)(5.0/total_frames * gw);
  (*kint->raster->drawLine)( graph, RGB_(120,120,120), x, 0, x, gh );

  x = (int)((total_frames - 5.0)/total_frames * gw);
  (*kint->raster->drawLine)( graph, RGB_(120,120,120), x, 0, x, gh );

  // Draw the Before and After curves/keys
  LWChannelID channels[]     = { channel, kint->workspace };
  int         curve_colors[] = { RGB_( 88, 80, 96), RGB_(176,160,192) };
  int         key_colors[]   = { RGB_(150, 50,  0), RGB_(255,255,255) };

  int show_original_curve, show_reduced_curve, show_original_keys, show_reduced_keys;
  GET_INT( kint->preview_show_original_curve, show_original_curve );
  GET_INT( kint->preview_show_reduced_curve,  show_reduced_curve  );
  GET_INT( kint->preview_show_original_keys,  show_original_keys  );
  GET_INT( kint->preview_show_reduced_keys,   show_reduced_keys   );

  bool draw_curve, draw_keys;
  for( int c = 0; c < 2; c++ ) {
    draw_curve = false;
    if( (c == 0) && (show_original_curve != 0) )
      draw_curve = true;
    else if( (c == 1) && (show_reduced_curve != 0) )
      draw_curve = true;

    // Draw the curve
    if( draw_curve ) {
      int y1 = -1, y2;
      for( int i=0; i < gw; i++ ) {
        now = start_frame + (total_frames * (((double)i) / (double)gw) );
        value = (*kint->chaninfo->channelEvaluate)( channels[c], now/fps );

        y2 = (int)( ((double)gh) * ((value_range - (value - value_low))/value_range) );
        if( y1 < 0 )
          y1 = y2;

        (*kint->raster->drawLine)( graph, curve_colors[c], ((i-1 < 0) ? 0 : i), y1, i, y2 );
        y1 = y2;
      }
    }

    // Draw the keys
    draw_keys = false;
    if( (c == 0) && (show_original_keys != 0) )
      draw_keys = true;
    else if( (c == 1) && (show_reduced_keys != 0) )
      draw_keys = true;

    if( draw_keys ) {
      LWEnvelopeID    env = (*kint->chaninfo->channelEnvelope)( channels[c] );
      LWEnvKeyframeID key = NULL;

      for( key = (*kint->env_funcs->nextKey)( env, key ); key != NULL; key = (*kint->env_funcs->nextKey)( env, key ) ) {
        (*kint->env_funcs->keyGet)( env, key, LWKEY_TIME, &now );
        now *= fps;
        if( now < start_frame )
          continue;

        if( now > end_frame )
          break;

        (*kint->env_funcs->keyGet)( env, key, LWKEY_VALUE, &value );

        x = int((now - start_frame)/total_frames * (double)gw);
        y = int((value_range - (value - value_low))/value_range * (double)gh);

        (*kint->raster->drawLine)( graph, key_colors[c], x-3, y,   x+3, y   );
        (*kint->raster->drawLine)( graph, key_colors[c], x,   y-3, x,   y+3 );
      }
    }
  }

  // Blit to the panel and free the image
  (*kint->raster->blitPanel)( graph, 0, 0, kint->panel, gx, gy, gw, gh );
  (*kint->raster->destroy)( graph );
}

// ReduceChannel():
void KeyStrainer_Interface::ReduceChannel( LWChannelID channel, int mode ) {
  const char * channel_name = (*chaninfo->channelName)( channel );
  if( strcmp( channel_name, "External") == 0 ) {
    LWChanGroupID cg = (*iteminfo->chanGroup)( ui->selItems[0] );
    SET_INT( preview_channel, (int)(*chaninfo->nextChannel)( cg, NULL) );
  }

  switch( mode ) {
    case KSR_KEEP_KILL:             // Keep/Kill Step
      ReduceByKeepKill( channel );
      break;

    case KSR_ITTERATIVE_REDUCTION:  // Itterative Threshold
      ReduceByIterativeThreshold( channel );
      break;

    case KSR_SMART_FREEZE:          // Smart Freeze
      ReduceBySmartFreeze( channel );
      break;

    case KSR_STEPPED_FREEZE:        // Total Freeze
      ReduceByTotalFreeze( channel );
      break;
  }
}

// ReduceByKeepKill():
void KeyStrainer_Interface::ReduceByKeepKill( LWChannelID channel ) {
  // Get the Keep/Kill range
  double keep, kill;
  GET_FLOAT( keep_step_float, keep );
  GET_FLOAT( kill_step_float, kill );

  keep /= sceneinfo->framesPerSecond;
  kill /= sceneinfo->framesPerSecond;

  // Get the Start/End times
  double start_time, end_time;
  GET_FLOAT( key_from_float, start_time );
  GET_FLOAT( key_to_float,   end_time   );

  start_time /= sceneinfo->framesPerSecond;
  end_time   /= sceneinfo->framesPerSecond;

  // Get the envelope
  LWEnvelopeID env = (*chaninfo->channelEnvelope)( channel );

  // Delete any existing keys in our workspace
  ClearEnvelope( workspace );

  // Copy keys in the Keep range into our workspace
  double now, diff, fraction_float;
  int fraction_int;
  double total_step = keep + kill;
  double kill_percent = ((double)kill) / total_step;

  for( LWEnvKeyframeID key = (*kint->env_funcs->nextKey)( env, NULL ); key != NULL; key = (*kint->env_funcs->nextKey)( env, key ) ) {
    (*kint->env_funcs->keyGet)( env, key, LWKEY_TIME, &now );

    // Keep all keys outside the range
    if( (now < start_time) || (now > end_time) ) {
      CopyKey( workspace, channel, key );
      continue;
    }

    // Fake a floating-point modulus operation
    fraction_float = (now - start_time)/(total_step + 0.0000000001);
    fraction_int   = (int)fraction_float;
    diff = fraction_float - (double)fraction_int;

    if( (diff > kill_percent) || (diff < 0.0000000001) )  // Fraction eliminates round-off problem (same as in FreeKey)
      CopyKey( workspace, channel, key );
  }
}

// ReduceByIterativeThreshold():
void KeyStrainer_Interface::ReduceByIterativeThreshold( LWChannelID channel ) {
  ;
}

// ReduceBySmartFreeze():
void KeyStrainer_Interface::ReduceBySmartFreeze( LWChannelID channel ) {
  double threshold;
  GET_FLOAT( reduction_threshold_float, threshold );

  if( threshold < 0.0001 ) {
    double value;
    GET_FLOAT( freeze_step_float, value );
    SET_FLOAT( freeze_step_float, 1.0 );

    ReduceByTotalFreeze( channel );

    SET_FLOAT( freeze_step_float, value );
    return;
  }

  // Get the freeze modifiers state and key shape
  int modifiers, shape;
  GET_INT( freeze_modifiers_toggle, modifiers );
  GET_INT( freeze_key_shape, shape );

  // Get the Start/End times
  double start_time, end_time;
  GET_FLOAT( key_from_float, start_time );
  GET_FLOAT( key_to_float,   end_time   );

  start_time /= sceneinfo->framesPerSecond;
  end_time   /= sceneinfo->framesPerSecond;

  // Get the envelope
  LWEnvelopeID env = (*chaninfo->channelEnvelope)( channel );

  // Delete any existing keys in our workspace and copy those outside the range
  ClearEnvelope( workspace );
  CopyEnvelope( workspace, env, start_time, end_time, false );

  // Ensure keys exist at Start Time and End Time
  LWEnvKeyframeID start_key = (*env_funcs->findKey)( workspace, start_time );
  double value;
  if( start_key == NULL ) {
    if( modifiers == 0 )
      value = (*env_funcs->evaluate)( env, start_time );
    else
      value = (*chaninfo->channelEvaluate)( channel, start_time );

    start_key = (*env_funcs->createKey)( workspace, start_time, value );
    (*env_funcs->keySet)( workspace, start_key, LWKEY_SHAPE, &shape );
  }

  LWEnvKeyframeID end_key = (*env_funcs->findKey)( workspace, end_time );
  if( end_key == NULL ) {
    if( modifiers == 0 )
      value = (*env_funcs->evaluate)( env, end_time );
    else
      value = (*chaninfo->channelEvaluate)( channel, end_time );

    end_key = (*env_funcs->createKey)( workspace, end_time, value );
    (*env_funcs->keySet)( workspace, end_key, LWKEY_SHAPE, &shape );
  }

  // Loop or something
  LWEnvKeyframeID scan_start_key, scan_end_key, key;
  double time, compare_value, scan_start_time, scan_end_time;
  double max_divergence, most_divergent_value, most_divergent_time, abs_value;
  double min_value, max_value;
  double orig_min_value, orig_max_value, max_linear_divergence = 0.000000001;
  double step = 1.0/sceneinfo->framesPerSecond;
  bool   init_max_divergence = true;

  DynArray< KSKey * > new_keys;

  // Iterative loop:  Cycle through until rescan is false
  while( true ) {
    scan_start_key = start_key;

    // Start/End loop:  Test to see if each pair of keys diverges from the original motion by more than the threshold
    while( true ) {
      scan_end_key = (*env_funcs->nextKey)( workspace, scan_start_key );
      assert( scan_end_key != NULL );

      (*env_funcs->keyGet)( workspace, scan_start_key, LWKEY_TIME, &scan_start_time );
      (*env_funcs->keyGet)( workspace, scan_end_key,   LWKEY_TIME, &scan_end_time   );

      most_divergent_value = 0.0;
      most_divergent_time  = start_time - step;

      (*env_funcs->keyGet)( workspace, scan_start_key, LWKEY_VALUE, &min_value );
      max_value = min_value;
      orig_min_value = min_value;
      orig_max_value = max_value;

      for( time = scan_start_time + step; time < scan_end_time; time += step ) {
        // Get the values at this step
        if( modifiers == 0 )
          value = (*env_funcs->evaluate)( env, time );
        else
          value = (*chaninfo->channelEvaluate)( channel, time );

        compare_value = (*env_funcs->evaluate)( workspace, time );

        // Fill the Min/Max values, if needed
        if( init_max_divergence ) {
          if( min_value > value )
            min_value = value;

          if( max_value < value )
            max_value = value;
        }

        // Check for linear divergence
        if( value < orig_min_value )
          orig_min_value = value;

        if( value > orig_max_value )
          orig_max_value = value;

        // See if it's more divergent than the previous most divergent value
        if( value > compare_value )
          abs_value = fabs( value - compare_value );
        else
          abs_value = fabs( compare_value - value );

        if( abs_value > most_divergent_value ) {
          most_divergent_value = abs_value;
          most_divergent_time  = time;
        }
      }

      // Initialize Max Divergence, if needed
      if( init_max_divergence ) {
        init_max_divergence = false;
        max_divergence = (max_value - min_value) * threshold;
      }

      // See if the divergence is less than the max allowed linear divergence
      if( fabs(orig_max_value - orig_min_value) < max_linear_divergence ) {
        int linear = 3;
        (*env_funcs->keySet)( workspace, scan_end_key, LWKEY_SHAPE, &linear );
      } else {
        // See if the divergence is greater than the max allowed divergence
        if( most_divergent_value > max_divergence ) {
          if( modifiers == 0 )
            value = (*env_funcs->evaluate)( env, most_divergent_time );
          else
            value = (*chaninfo->channelEvaluate)( channel, most_divergent_time );

          new_keys.Add( new KSKey( most_divergent_time, value ) );
        }
      }

      #ifdef _DEBUG
        #define DEBUG_SMART_FREEZE
        #ifdef DEBUG_SMART_FREEZE
          static bool pause = true;
          if( pause ) {
            REDRAW_CON( preview_canvas );
            char buffer1[ 1024 ], buffer2[ 1024 ];
            sprintf( buffer1, "Start: %g; End: %g; Total: %g-%g", scan_start_time, scan_end_time, start_time, end_time );
            sprintf( buffer2, "MDV: %g; MDT: %g;  MaxD: %d", most_divergent_value, most_divergent_time, max_divergence );
            if( (*message->yesNo)( "Click No to stop the pauses", buffer1, buffer2 ) == 0 )
              pause = false;
          }
        #endif
      #endif

      // Stop if we're at the end of the range
      if( scan_end_key == end_key )
        break;

      scan_start_key = scan_end_key;
    }

    // See if we're done
    if( new_keys.NumElements() == 0 )
      break;                           // No one else was divergent; exit loop

    // Create new keys
    for( unsigned long i=0; i < new_keys.NumElements(); i++ ) {
      key = (*env_funcs->createKey)( workspace, new_keys[i]->time, new_keys[i]->value );
      (*env_funcs->keySet)( workspace, key, LWKEY_SHAPE, &shape );
    }

    new_keys.Flush();
  }
}

// ReduceByTotalFreeze():
void KeyStrainer_Interface::ReduceByTotalFreeze( LWChannelID channel ) {
  // Get the freeze step
  double step;
  GET_FLOAT( freeze_step_float, step );
  assert( step > .01 );
  step /= sceneinfo->framesPerSecond;

  // Get the freeze modifiers state and key shape
  int modifiers, shape;
  GET_INT( freeze_modifiers_toggle, modifiers );
  GET_INT( freeze_key_shape, shape );

  // Get the Start/End times
  double start_time, end_time;
  GET_FLOAT( key_from_float, start_time );
  GET_FLOAT( key_to_float,   end_time   );

  start_time /= sceneinfo->framesPerSecond;
  end_time   /= sceneinfo->framesPerSecond;

  // Get the envelope
  LWEnvelopeID env = (*chaninfo->channelEnvelope)( channel );

  // Delete any existing keys in our workspace and copy those outside the range
  ClearEnvelope( workspace );
  CopyEnvelope( workspace, env, start_time, end_time, false );

  // Copy keys outside the range into our workspace
  double value, now;
  LWEnvKeyframeID key;

  // Freeze new keys into our workspace
  for( now = start_time; now <= end_time; now += step ) {
    if( modifiers == 0 )
      value = (*env_funcs->evaluate)( env, now );
    else
      value = (*chaninfo->channelEvaluate)( channel, now );

    key = (*env_funcs->createKey)( workspace, now, value );
    (*env_funcs->keySet)( workspace, key, LWKEY_SHAPE, &shape );
  }
}

// CopyKey():
//  Creates a copy of a keyframe from one envelope into another envelope.
void KeyStrainer_Interface::CopyKey( LWEnvelopeID env_to, LWEnvelopeID env_from, LWEnvKeyframeID from ) {
  // Get the time and value
  double time, value;
  (*env_funcs->keyGet)( env_from, from, LWKEY_TIME,  &time  );
  (*env_funcs->keyGet)( env_from, from, LWKEY_VALUE, &value );

  // Create the new keyframe
  LWEnvKeyframeID to = (*env_funcs->createKey)( env_to, time, value );

  // Copy the key's shape
  int shape;
  (*env_funcs->keyGet)( env_from, from, LWKEY_SHAPE, &shape );
  (*env_funcs->keySet)( env_to,   to,   LWKEY_SHAPE, &shape );

  switch( shape ) {
    case 0:        // TCB
      (*env_funcs->keyGet)( env_from, from, LWKEY_TENSION, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_TENSION, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_CONTINUITY, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_CONTINUITY, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_BIAS, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_BIAS, &value );
      break;

    case 1:        // Hermite
    case 2:        // Bezier
      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_0, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_0, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_1, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_1, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_2, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_2, &value );

      (*env_funcs->keyGet)( env_from, from, LWKEY_PARAM_3, &value );
      (*env_funcs->keySet)( env_to,   to,   LWKEY_PARAM_3, &value );
      break;

    case 3:        // Linear
      break;

    case 4:        // Stepped
      break;
  }
}

// CopyEnvelope():
//  Copies all the keyframes from one envelope into another between
//   start_time and end_time inclusive.  All existing keyframes within
//   the range will be deleted.  The inside flag inverts the range.
void KeyStrainer_Interface::CopyEnvelope( LWEnvelopeID to, LWEnvelopeID from, double start_time, double end_time, bool inside ) {
  ClearKeyRange( to, start_time, end_time, inside );

  double now;
  for( LWEnvKeyframeID key = (*env_funcs->nextKey)( from, NULL ); key != NULL; key = (*env_funcs->nextKey)( from, key ) ) {
    (*env_funcs->keyGet)( from, key, LWKEY_TIME, &now );
    if( ((now < start_time) &&  inside) ||
        ((now > start_time) && !inside) ) {
      continue;
    }

    if( ((now > end_time) &&  inside) ||
        ((now < end_time) && !inside) ) {
      continue;
    }

    CopyKey( to, from, key );
  }
}

// ClearChannel():
//  Deletes all keys in a channel
void KeyStrainer_Interface::ClearEnvelope( LWEnvelopeID env ) {
  LWEnvKeyframeID next_key;
  for( LWEnvKeyframeID key = (*env_funcs->nextKey)( env, NULL ); key != NULL; key = next_key ) {
    next_key = (*env_funcs->nextKey)( env, key );   // Get the next key now, since we're going to delete this one soon
    (*env_funcs->destroyKey)( env, key );
  }
}

// ClearKeyRange():
//  Deletes all keys in a channel between start_time and end_time
//   inclusive.  The inside flag inverts the range.
void KeyStrainer_Interface::ClearKeyRange( LWEnvelopeID env, double start_time, double end_time, bool inside ) {
  assert( start_time <= end_time );

  LWEnvKeyframeID next_key;
  double when;

  for( LWEnvKeyframeID key = (*env_funcs->nextKey)( env, NULL ); key != NULL; key = next_key ) {
    next_key = (*env_funcs->nextKey)( env, key );   // Get the next key now, since we might delete this one soon

    // See
    when = (*env_funcs->keyGet)( env, key, LWKEY_TIME, &when );
    if( ((when < start_time) &&  inside) ||
        ((when > start_time) && !inside) ) {
      continue;
    }

    if( ((when > end_time) &&  inside) ||
        ((when < end_time) && !inside) ) {
      continue;
    }

    (*env_funcs->destroyKey)( env, key );
  }
}

// ReduceForPreview():
void KeyStrainer_Interface::ReduceForPreview() {
  int state;
  GET_INT( preview_toggle, state );

  if( state == 0 )
    return;

  int mode, channel;
  GET_INT( kint->mode_popup, mode );
  GET_INT( kint->preview_channel, channel );
  kint->ReduceChannel( (LWChannelID)channel, mode );

  REDRAW_CON( kint->preview_canvas );
}

// Process():
bool KeyStrainer_Interface::Process() {
  int which, mode;
  GET_INT( for_popup, which );
  GET_INT( mode_popup, mode );

  // Turn off Dynamic Update for speed
  int dynamic_update = ui->dynaUpdate;
  (*generic->evaluate)( generic->data, "DynamicUpdate 0" );

  // Build Hierarchy
  LWItemHierarchy base( LWITEM_NULL );
  base.BuildChildList( iteminfo );

  // Select items in the hierarchy
  switch( which ) {
    case 0:      // Selected Items
      base.ReadLayoutSelection( iteminfo, ui );
      break;

    case 1:      // Selected Items And Descendants
      base.ReadLayoutSelection( iteminfo, ui );
      base.SelectSelectedsChildren( true );
      break;

    case 2:      // Current Item Only
      base.ReadLayoutSelection( iteminfo, ui, true );
      break;

    case 3:      // Current Item And Descendants
      base.ReadLayoutSelection( iteminfo, ui, true );
      base.SelectSelectedsChildren( true );
      break;

    case 4:      // All Items
      base.SelectAll();
      break;
  }

  #ifdef _DEBUG
    ofstream out( "i:\\KeyStrainer_Hierarchy.txt" );
    base.OutputToStream( out, iteminfo );
    out.close();
  #endif

  // Figure out which channels to process
  int channels[9];
  GET_INT( x_button,  channels[0] );
  GET_INT( y_button,  channels[1] );
  GET_INT( z_button,  channels[2] );

  GET_INT( h_button,  channels[3] );
  GET_INT( p_button,  channels[4] );
  GET_INT( b_button,  channels[5] );

  GET_INT( sx_button, channels[6] );
  GET_INT( sy_button, channels[7] );
  GET_INT( sz_button, channels[8] );

  int all_other_channels;
  GET_INT( all_other_channels_button, all_other_channels );

  if( (which == 1) || (which == 3) || (which == 4) ) {
    progress_total = base.CountSelected();
    OpenProgressPanel();
  }

  // Process the items
  ProcessItem( &base, mode, channels, ((all_other_channels == 0) ? false : true) );

  if( progress_open )
    (*panel_funcs->close)( progress_panel );


  // Restore Dynamic Update to it's previous state
  char buffer[100];
  sprintf( buffer, "DynamicUpdate %d", dynamic_update );
  (*generic->evaluate)( generic->data, buffer );

  return true;
}

const char *motion_channel_names[] = {
  "Position.X", "Position.Y", "Position.Z",
  "Rotation.H", "Rotation.P", "Rotation.B",
  "Scale.X",    "Scale.Y",    "Scale.Z",
  NULL };

// ProcessItem():
//  Actually deletes keyframes from the item.  Threshold is used as a +/-
//   value; all keys within the range +/- the threshold will be deleted.
bool KeyStrainer_Interface::ProcessItem( LWItemHierarchy *item, int mode, int *channels, bool all_other_channels ) {
  if( item->GetIsSelected() ) {
    LWChanGroupID   parent_group = (*iteminfo->chanGroup)( item->GetID() );
    LWChannelID     chan;
    LWEnvelopeID    env;
    LWEnvKeyframeID key;

    // Update the progress bar
    if( kint->progress_open ) {
      sprintf( progress_string, "Processing \"%s\"...", (*iteminfo->name)( item->GetID() ) );
      kint->progress_current++;
      REDRAW_CON( kint->progress_field );

      // Update the panel and see if the user closed it (ie: cancel operation)
      if( (*kint->panel_funcs->handle)( kint->progress_panel, 0 ) == -1) {
        if( (*kint->message->yesNo)( "Key Strainer -- Cancel Processing?",
                                     "Do you really want to abort processing?",
                                     "Note that some objects may have already processed" ) == 1 ) {
          (*kint->message->info)( "Key Strainer Info:  Processing aborted by user.", NULL );
          progress_open = false;
          return false;
        } else {
          (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );
        }
      }
    }

    unsigned long i;
    LWChannelScan scanner( chaninfo, parent_group, iteminfo, item->GetID(), true );

    // Cycle through each channel group
    for( chan = scanner.FirstChannel(); chan != NULL; chan = scanner.NextChannel( true ) ) {
      // See if this is a motion channel
      i = -1;
      if( scanner.GetCurrentGroup() == parent_group )   // We only test for motion channels in the parent group
        i = scanner.DoesChannelNameMatch( motion_channel_names );

      if( i == -1 ) {  // No Match; check All Other Channels
        if( !all_other_channels )
          continue;
      } else {         // Channel matches name; figure out which one
        if( channels[i] == 0 )     // Make sure that channel is active; if not, skip it
          continue;
      }

      switch( mode ) {
        case KSR_KEEP_KILL:              // Keep/Kill Step
          ReduceByKeepKill( chan );
          break;

        case KSR_ITTERATIVE_REDUCTION:   // Iterative Threshold
          ReduceByIterativeThreshold( chan );
          break;

        case KSR_SMART_FREEZE:           // Smart Freeze
          ReduceBySmartFreeze( chan );
          break;

        case KSR_STEPPED_FREEZE:         // Stepped Freeze
          ReduceByTotalFreeze( chan );
          break;
      }

      // Copy the workspace into the actual envelope
      env = (*chaninfo->channelEnvelope)( chan );
      CopyEnvelope( env, workspace, -100000000.0, 100000000.0, true );

      // If there aren't any keys left in this channel, add a default Frame 0
      key = (*env_funcs->nextKey)( env, NULL );
      if( key == NULL )
        (*env_funcs->createKey)( env, 0.0, ((i < 6) ? 0.0 : 1.0) );
    }
  }

  // Process Children
  for( unsigned long i=0; i < item->GetChildren().NumElements(); i++ ) {
    if( !ProcessItem( item->GetChildren()[i], mode, channels, all_other_channels ) ) {
      return false;        // User Abort
    }
  }

  return true;
}

// OpenProgressPanel():
int KeyStrainer_Interface::OpenProgressPanel() {
  // Destroy the panel if it hasn't been destroyed yet
  if( progress_panel != NULL ) {
    (*panel_funcs->destroy)( progress_panel );
    progress_panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Mot-ify -- %c2000-2001 Joe Angell, TM Productions", 169 );
  progress_panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !progress_panel ) {              // Make sure the panel was created
    (*message->error)("Error creating progress panel, aborting", NULL );
    return false;
  }

  // Progress Field
  progress_field = CANVAS_CTL( panel_funcs, progress_panel, "", 400, (CON_HOTH( mode_popup )) );

  // Draw Callbacks
  ival.intv.value = (int)&DrawProgressField;
  (*progress_field->set)( progress_field, CTL_USERDRAW, &ival );

  // Set up some variables
  progress_open = true;
  strcpy( progress_string, "Processing..." );

  // Open the Panel
  int retval = (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );

  return retval;
}

// DrawProgressField():
void DrawProgressField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = kint->panel_funcs->drawFuncs;

  // Clear the bar
  (*df->drawRGBBox)( kint->progress_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

  // Fill the progress portion of the bar
  (*df->drawRGBBox)( kint->progress_panel, 0, 60, 150, cx+1, cy+1,
                    (int)((cw-2) * ( ((double)kint->progress_current) / ((double)kint->progress_total) ) ), ch-2 );

  // Write some text
  (*df->drawText)( kint->progress_panel, kint->progress_string, COLOR_WHITE, cx + 3, cy + 2 );
}

