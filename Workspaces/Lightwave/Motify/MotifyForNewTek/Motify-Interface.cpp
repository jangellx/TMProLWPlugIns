//
// Motify-Interface.cpp
//

#include "Motify-Interface.h"
#include "Motify-PiEAboutBox.h"
#include <stdio.h>

#include "Motify-FromLWChannelScan.h"

static Motify_Interface *mint = NULL;

// LWPanels Macro Stuff
LWPanControlDesc desc;
LWValue ival    = {LWT_INTEGER};
LWValue ivecval = {LWT_VINT};
LWValue fval    = {LWT_FLOAT};
LWValue fvecval = {LWT_VFLOAT};
LWValue sval    = {LWT_STRING};

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Mot-ify",
  LWABOUT_BLACK,
  "Delete Motion Keys",
  LWABOUT_VTAB,
  "Improved Delete Key Panel for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Written by Joe Angell",
  "Copyright 2000-2001 PiE Tools",
  LWABOUT_HDIV,
  "http://www.pietools.com",
  "jangell@tmproductions.com",
  NULL };

const char *keys_text[] = {
  "Esc",                  "Cancel (Do Nothing)",
  "Enter/Return",         "OK (Process)",
  "Arrow Up/Down",        "Previous/Next Delete Mode",
  "Ctrl-Arrow Up/Down",   "Previous/Next For Mode",
  "Shift-Z/z",            "Previous/Next Protection Mode",
  "Tab/f",                "Activate From Field",
  "t",                    "Activate Delete Field",
  "e",                    "Activate Threshold Field",
  "1-3",                  "Toggle Individual Position Channels",
  "4-6",                  "Toggle Individual Rotation Channels",
  "7-9",                  "Toggle Individual Scale Channels",
  "p",                    "Toggle all Position Channels",
  "r",                    "Toggle all Rotation Channels",
  "s",                    "Toggle all Scale Channels",
  "Shift-P",              "Invert Position Channels",
  "Shift-R",              "Invert Rotation Channels",
  "Shift-S",              "Invert Scale Channels",
  "F1",                   "About/Keys",
  NULL };

const char *for_popup_strings[] =
  { "Selected Items",    "Selected Items And Descendants",
    "Current Item Only", "Current Item And Descendants",
    "All Items", NULL };

const char *mode_popup_strings[] = {
  "Delete Key",                          // Delete a single key
  "--  --  --  --  --  --  --  --  --  --  --",
  "Delete Keys Within Range",            // Delete all keys within range, inclusive
  "Delete Keys Outside Range",           // Delete all keys outside range, exclusive
  "Delete Keys Before Range",            // Delete all keys before range, exclusive
  "Delete Keys After Range",             // Delete all keys after range, exclusive
  "--  --  --  --  --  --  --  --  --  --  --",
  "Clear Motion",                        // Clear all keys, resting frame 0 to XYZ HPB 0, SX SY SZ 1
/*
  "Clear Motion Except Frame 0",         // Clear all keys except for Frame 0.
  "Clear Motion After Frame 0",
*/
  NULL };  // Clear all keys after Frame 0.

const char * thereshold_popdown_strings[] = {
  "0.00", "0.05", "0.10", "0.25", "0.33", "0.50", "0.99", NULL };

const double threshold_popdown_values[] = {
  0.0, 0.05, 0.1, 0.25, 1.0/3.0,  0.5, 0.9999 };

const char * protect_popup_strings[] = {
  "No Protection", "Protect Frame 0", "Protect Neg & 0",
  "Protect First Key", "Protect Last Key", NULL };

// Static Variable Init
int Motify_Interface::for_state     = 0;
int Motify_Interface::protect_state = 1;

// Constructor():
Motify_Interface::Motify_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), sel_type(-1), ctrl_down(false), do_process(false),
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

  if( ui->selItems[0] != NULL )
    sel_type = (iteminfo->type)( ui->selItems[0] );

  mint = this;
}

// Destructor():
Motify_Interface::~Motify_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  if( progress_panel != NULL )
    (*panel_funcs->destroy)( progress_panel );

  mint = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
int Motify_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Mot-ify Delete Motion Keys -- by Joe Angell, %c2000-2001 PiE Tools", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Mode
  mode_popup                    = WPOPUP_CTL( panel_funcs, panel, "Delete Mode", mode_popup_strings, 215 );
//  protect_zero_bool             = BOOL_CTL(   panel_funcs, panel, "Protect Frame 0" );
  protect_popup                 = WPOPUP_CTL( panel_funcs, panel, "", protect_popup_strings, 103 );

  // Delete Key Range
  delete_key_from_float         = FLOAT_CTL(  panel_funcs, panel, "Delete Keys From" );
  delete_key_to_float           = FLOAT_CTL(  panel_funcs, panel, "Through" );
  delete_key_threshold_float    = FLOAT_CTL(  panel_funcs, panel, "Threshold" );
  delete_key_thresholds_popdown = WPOPUP_CTL( panel_funcs, panel, "", thereshold_popdown_strings, 19 );

  // For Popup
  for_popup                     = WPOPUP_CTL( panel_funcs, panel, "For", for_popup_strings, 215 );

  // About
  about_button                  = WBUTTON_CTL( panel_funcs, panel, "About", 60 );

  // Align the above controls
  LWControl *aligner[] = { mode_popup, delete_key_from_float, for_popup, NULL };
  PanelTools::AlignLabels( aligner );
  PanelTools::PutOnLineWith( mode_popup, protect_popup, -1 );
  PanelTools::PutOnLineWith( delete_key_from_float, delete_key_to_float );
  PanelTools::PutOnLineWith( delete_key_to_float, delete_key_threshold_float );
  PanelTools::PutOnLineWith( delete_key_threshold_float, delete_key_thresholds_popdown, -8 );

  MOVE_CON( for_popup, (CON_X( for_popup )), (CON_Y( for_popup )) + 12);
  PanelTools::PutOnLineWith( for_popup, about_button, 5 );

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
  SET_INT( all_other_channels_button, 0 );

  // Set some initial states
  SET_INT( protect_popup, protect_state );
  SET_INT( for_popup, for_state );

  SET_FLOAT( delete_key_from_float, (ui->curTime * sceneinfo->framesPerSecond) );

  HandleModePopup(      mode_popup,      NULL );
  HandlePositionButton( position_button, NULL );
  HandleRotationButton( rotation_button, NULL );
  HandleScaleButton(    scale_button,    NULL );

  // Resize the Panel
  PAN_SETH( panel_funcs, panel, (CON_Y( x_button )) + (CON_H( x_button )) );

  // Event Handlers
  CON_SETEVENT( mode_popup,                    HandleModePopup,         NULL );

  CON_SETEVENT( delete_key_from_float,         HandleFromFloat,         NULL );
  CON_SETEVENT( delete_key_to_float,           HandleToFloat,           NULL );
  CON_SETEVENT( delete_key_thresholds_popdown, HandleThresholdPopdown,  NULL );

  CON_SETEVENT( about_button,                  HandleAboutButton,       NULL );

  CON_SETEVENT( position_button,               HandlePositionButton,    NULL );
  CON_SETEVENT( rotation_button,               HandleRotationButton,    NULL );
  CON_SETEVENT( scale_button,                  HandleScaleButton,       NULL );

  CON_SETEVENT( all_other_channels_button,     HandleAllOtherChannelsButton, NULL );

  // Set the Draw Function
  (*panel_funcs->set)( panel, PAN_USERDRAW,   DrawPanel    );
  (*panel_funcs->set)( panel, PAN_USERKEYS,   HandleKeys   );
  (*panel_funcs->set)( panel, PAN_USERKEYUPS, HandleKeyUps );

  // Open the Panel
  int retval = (*panel_funcs->open)( panel, PANF_BLOCKING | PANF_CANCEL | PANF_PASSALLKEYS );
  if( retval > 0 )
    do_process = true;

  return retval;
}

// DrawMainPanel() {
void DrawPanel( LWPanelID panel, void *data, DrMode mode) {
  DrawFuncs *df = mint->panel_funcs->drawFuncs;

  int ph = PAN_GETH( mint->panel_funcs, mint->panel );
  int pw = PAN_GETW( mint->panel_funcs, mint->panel );

  // Horizontal Dividers
  int y = CON_Y( mint->delete_key_from_float );
  int h = CON_H( mint->delete_key_from_float );
  PanelDraw::HorizontalDivider( mint->panel, df, 0, y + h + 3, pw, 1 );
}

// HandleKeys():
//  Used mainly to close the panel with Enter/Esc, since Enter
//   doesn't seem to be doing it.  It also allows for a bit of
//   keyboard control of the panel, too.
void HandleKeys( LWPanelID panel, void *data, LWDualKey key ) {
  int state;

  switch( key ) {
    case LWDK_CTRL:             // Control
      mint->ctrl_down = true;
      break;

    case LWDK_SHIFT:            // Shift
      mint->shift_down = true;
      break;

    case '\r':                  // OK
      (*mint->panel_funcs->close)( mint->panel );
      mint->do_process = true;
      break;

    case 27:                    // Cancel (ESC)
      (*mint->panel_funcs->close)( mint->panel );
      break;

    case LWDK_SC_UP:
      if( mint->ctrl_down ) {   // Previous Mode
        GET_INT( mint->mode_popup, state );
        if( state > 0 ) {
          SET_INT( mint->mode_popup, state-1 );
          HandleModePopup( mint->mode_popup, NULL );
        }
      } else {                  // Previous For
        GET_INT( mint->for_popup, state );
        if( state > 0 ) {
          SET_INT( mint->for_popup, state-1 );
          HandleModePopup( mint->for_popup, NULL );
        }
      }
      break;

    case LWDK_SC_DOWN:
      if( mint->ctrl_down ) {   // Next Mode
        GET_INT( mint->mode_popup, state );
        if( state < 9 ) {
          if( (state == 0) || (state == 5) )
            SET_INT( mint->mode_popup, state+2 );
          else
            SET_INT( mint->mode_popup, state+1 );
          HandleModePopup( mint->mode_popup, NULL );
        }
      } else {                  // Next For
        GET_INT( mint->for_popup, state );
        if( state < 4 ) {
          SET_INT( mint->for_popup, state+1 );
          HandleModePopup( mint->for_popup, NULL );
        }
      }
      break;

    case 'z':
    case 'Z': {                 // Previous Protection Mode
      if( mint->shift_down ) {
        GET_INT( mint->protect_popup, state );
        if( state > 0 )
          SET_INT( mint->protect_popup, state - 1 );
      } else {                  // Next Protection Mode
        GET_INT( mint->protect_popup, state );
        if( state < 4 )
          SET_INT( mint->protect_popup, state + 1 );
      }
    } break;

    case 9:
    case 'f':                   // Activate From
      ACTIVATE_CON( mint->delete_key_from_float );
      break;

    case 't':                   // Activate To
      ACTIVATE_CON( mint->delete_key_to_float );
      break;

    case 'e':                   // Activate Threshold
      ACTIVATE_CON( mint->delete_key_threshold_float );
      break;

    // Channels
    case '1':
      GET_INT( mint->x_button, state );
      SET_INT( mint->x_button, (state ? 0 : 1) );
      break;

    case '2':
      GET_INT( mint->y_button, state );
      SET_INT( mint->y_button, (state ? 0 : 1) );
      break;

    case '3':
      GET_INT( mint->z_button, state );
      SET_INT( mint->z_button, (state ? 0 : 1) );
      break;


    case '4':
      GET_INT( mint->h_button, state );
      SET_INT( mint->h_button, (state ? 0 : 1) );
      break;

    case '5':
      GET_INT( mint->p_button, state );
      SET_INT( mint->p_button, (state ? 0 : 1) );
      break;

    case '6':
      GET_INT( mint->b_button, state );
      SET_INT( mint->b_button, (state ? 0 : 1) );
      break;


    case '7':
      GET_INT( mint->sx_button, state );
      SET_INT( mint->sx_button, (state ? 0 : 1) );
      break;

    case '8':
      GET_INT( mint->sy_button, state );
      SET_INT( mint->sy_button, (state ? 0 : 1) );
      break;

    case '9':
      GET_INT( mint->sz_button, state );
      SET_INT( mint->sz_button, (state ? 0 : 1) );
      break;


    case 'p':
    case 'P':
      HandlePositionButton( mint->position_button, NULL );
      break;

    case 'r':
    case 'R':
      HandleRotationButton( mint->rotation_button, NULL );
      break;

    case 's':
    case 'S':
      HandleScaleButton( mint->scale_button, NULL );
      break;

    case LWDK_F1:
      HandleAboutButton( mint->about_button, NULL );
      break;
  }
}

// HandleKeyUps():
void HandleKeyUps( LWPanelID panel, void *data, LWDualKey key ) {
  switch( key ) {
    case LWDK_CTRL:            // Control
      mint->ctrl_down = false;
      break;

    case LWDK_SHIFT:            // Shift
      mint->shift_down = false;
      break;
  }
}

// BuildItemList():
bool Motify_Interface::BuildItemList() {
  if( ui->selItems[0] == NULL )
    return false;

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  return true;
}

// ReselectItems():
//  Reselects the previously selected items
void Motify_Interface::ReselectItems() {
  if( items.NumElements() == 0 )
    return;

  char buffer[100];
  sprintf( buffer, "SelectItem %x", items[ items.NumElements()-1 ] );
  (*generic->evaluate)( generic->data, buffer );

  for( unsigned long i=items.NumElements()-1; i != -1; i-- ) {
    sprintf( buffer, "AddToSelection %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}

//
// -- Event Handlers ---------------------------------------------------------
//

// HandleAboutButton():
void HandleAboutButton( LWControl *con, void *data ) {
  PiEToolsLogo logo;
  LWAboutPanel( mint->panel_funcs, "Mot-ify", about_text, &logo, mint->panel, true, keys_text );
}

// HandleModePopup():
void HandleModePopup( LWControl *con, void *data ) {
  int mode;
  GET_INT( mint->mode_popup, mode );

  double from, to;
  GET_FLOAT( mint->delete_key_from_float,      from );
  GET_FLOAT( mint->delete_key_to_float,        to   );

  switch( mode ) {
    case 0:           // Delete Key
      UNGHOST_CON( mint->delete_key_from_float      );
      GHOST_CON(   mint->delete_key_to_float        );
      UNGHOST_CON( mint->delete_key_threshold_float );

      SET_FLOAT( mint->delete_key_to_float,   from );
      break;

    case 2:           // Delete Keys Within Range
      UNGHOST_CON( mint->delete_key_from_float        );
      UNGHOST_CON( mint->delete_key_to_float          );
      UNGHOST_CON( mint->delete_key_threshold_float   );
      break;

    case 3:           // Delete Keys Outside Range
      UNGHOST_CON( mint->delete_key_from_float      );
      UNGHOST_CON( mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );
      break;

    case 4:           // Delete Keys Before Range
      UNGHOST_CON( mint->delete_key_from_float      );
      GHOST_CON(   mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );

      SET_FLOAT( mint->delete_key_to_float,   from );
      break;

    case 5:           // Delete Keys After Range
      GHOST_CON(   mint->delete_key_from_float      );
      UNGHOST_CON( mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );

      SET_FLOAT( mint->delete_key_from_float, to );
      break;


    case 7:           // Clear Motion
      GHOST_CON(   mint->delete_key_from_float      );
      GHOST_CON(   mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );
      break;

    case 8:           // Clear Motion Except Frame 0
      GHOST_CON(   mint->delete_key_from_float      );
      GHOST_CON(   mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );
      break;

    case 9:           // Clear Motion After Frame 0
      GHOST_CON(   mint->delete_key_from_float      );
      GHOST_CON(   mint->delete_key_to_float        );
      GHOST_CON(   mint->delete_key_threshold_float );
      break;

    default:
      SET_INT( mint->mode_popup, mode - 1 );
      HandleModePopup( mint->mode_popup, NULL );
      break;
  }

}

// HandleFromFloat():
//  Simple ensures that From is always less than or equal to To
void HandleFromFloat( LWControl *con, void *data ) {
  double from, to;
  GET_FLOAT( mint->delete_key_from_float, from );
  GET_FLOAT( mint->delete_key_to_float,   to   );

  if( to < from )
    SET_FLOAT( mint->delete_key_to_float, from );

  HandleModePopup( mint->mode_popup, NULL );
}

// HandleToFloat():
//  Simple ensures that From is always less than or equal to To
void HandleToFloat( LWControl *con, void *data ) {
  double from, to;
  GET_FLOAT( mint->delete_key_from_float, from );
  GET_FLOAT( mint->delete_key_to_float,   to   );

  if( from > to )
    SET_FLOAT( mint->delete_key_from_float, to );

  HandleModePopup( mint->mode_popup, NULL );
}

// HandleThresholdPopdown():
void HandleThresholdPopdown( LWControl *con, void *data ) {
  int preset;
  GET_INT( mint->delete_key_thresholds_popdown, preset );

  SET_FLOAT( mint->delete_key_threshold_float, threshold_popdown_values[ preset ] );
}

// HandlePositionButton():
void HandlePositionButton( LWControl *con, void *data ) {
  int state;
  GET_INT( mint->x_button, state );

  if( mint->shift_down ) {  // Invert selection
    SET_INT( mint->x_button, (state ? 0 : 1 ) );

    GET_INT( mint->y_button, state );
    SET_INT( mint->y_button, (state ? 0 : 1 ) );

    GET_INT( mint->z_button, state );
    SET_INT( mint->z_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( mint->x_button, (state ? 0 : 1 ) );
    SET_INT( mint->y_button, (state ? 0 : 1 ) );
    SET_INT( mint->z_button, (state ? 0 : 1 ) );
  }
}

// HandleRotationButton():
void HandleRotationButton( LWControl *con, void *data ) {
  int state;
  GET_INT( mint->h_button, state );

  if( mint->shift_down ) {  // Invert selection
    SET_INT( mint->h_button, (state ? 0 : 1 ) );

    GET_INT( mint->p_button, state );
    SET_INT( mint->p_button, (state ? 0 : 1 ) );

    GET_INT( mint->b_button, state );
    SET_INT( mint->b_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( mint->h_button, (state ? 0 : 1 ) );
    SET_INT( mint->p_button, (state ? 0 : 1 ) );
    SET_INT( mint->b_button, (state ? 0 : 1 ) );
  }
}

// HandleScaleButton():
void HandleScaleButton( LWControl *con, void *data ) {
  int state;
  GET_INT( mint->sx_button, state );

  if( mint->shift_down ) {  // Invert selection
    SET_INT( mint->sx_button, (state ? 0 : 1 ) );

    GET_INT( mint->sy_button, state );
    SET_INT( mint->sy_button, (state ? 0 : 1 ) );

    GET_INT( mint->sz_button, state );
    SET_INT( mint->sz_button, (state ? 0 : 1 ) );
  } else {
    SET_INT( mint->sx_button, (state ? 0 : 1 ) );
    SET_INT( mint->sy_button, (state ? 0 : 1 ) );
    SET_INT( mint->sz_button, (state ? 0 : 1 ) );
  }
}

// HandleAllOtherChannelsButton():
void HandleAllOtherChannelsButton( LWControl *con, void *data ) {
  int state;
  GET_INT( mint->all_other_channels_button, state );

  if( state != 0 ) {
    if( (*mint->message->yesNo)( "Process All Other Channels?",
                                 "Warning:  This feature will delete keys on all non-motion channels, including",
                                 "envelopes and plug-in channels for the chosen items.  Are you sure?" ) == 0 ) {
      SET_INT( mint->all_other_channels_button, 0 );
    }
  }
}

// Process():
bool Motify_Interface::Process() {
  int which, mode;
  GET_INT( for_popup, which );
  GET_INT( mode_popup, mode );

/*
  int affect_bones;
  GET_INT( affect_bones_toggle, affect_bones );
*/

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
    ofstream out( "i:\\Motify_Hierarchy.txt" );
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

  // Figure out the range
  double from, to, threshold;
  GET_FLOAT( delete_key_from_float,      from      );
  GET_FLOAT( delete_key_to_float,        to        );
  GET_FLOAT( delete_key_threshold_float, threshold );

  int protect_zero;
  GET_INT( protect_popup, protect_zero );

  if( (mode > 1) && ((which == 1) || (which == 3) || (which == 4)) ) {
    progress_total = base.CountSelected();
    OpenProgressPanel();
  }

  // Process the items
  ProcessItem( &base, mode, channels, from/sceneinfo->framesPerSecond,
               to/sceneinfo->framesPerSecond, threshold/sceneinfo->framesPerSecond,
               protect_zero, ((all_other_channels == 0) ? false : true) );

  if( progress_open )
    (*panel_funcs->close)( progress_panel );

  (*generic->evaluate)( generic->data, "Refresh" );

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
bool Motify_Interface::ProcessItem( LWItemHierarchy *item, int mode, int *channels,
                                    double from, double to, double threshold,
                                    int protection, bool all_other_channels ) {

  if( item->GetIsSelected() ) {
    LWChanGroupID   parent_group = (*iteminfo->chanGroup)( item->GetID() );
    LWChannelID     chan;
    LWEnvelopeID    env;
    LWEnvKeyframeID key, new_key;

    double value;

    // Update the progress bar
    if( mint->progress_open ) {
      sprintf( progress_string, "Processing \"%s\"...", (*iteminfo->name)( item->GetID() ) );
      mint->progress_current++;
      REDRAW_CON( mint->progress_field );

      // Update the panel and see if the user closed it (ie: cancel operation)
      if( (*mint->panel_funcs->handle)( mint->progress_panel, 0 ) == -1) {
        if( (*mint->message->yesNo)( "Mot-ify -- Cancel Processing?",
                                     "Do you really want to abort processing?",
                                     "Note that some objects may have already processed" ) == 1 ) {
          (*mint->message->info)( "Mot-ify Info:  Processing aborted by user.", NULL );
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

      // Get the envelope for this channel
      env = NULL;

      // Channel/Envelope not found; skip the channel
      env = (*chaninfo->channelEnvelope)( scanner.GetCurrentChannel() );
      if( env == NULL ) 
        continue;

      // Delete keys on enabled channels
      bool key_protected;
      bool is_first = true;

      key = (*env_funcs->nextKey)( env, NULL );
      while( key != NULL ) {
        new_key = (*env_funcs->nextKey)( env, key );          // Get the next key now, since we might be deleting this one
        (*env_funcs->keyGet)( env, key, LWKEY_TIME, &value ); // Get the time of this key

        // Protect Frame 0 if enabled
        switch( protection ) {
          case 0:             // No Protection
            key_protected = false;
            break;

          case 1:             // Protect Frame 0
            key_protected = (value == 0.0) ? true : false;
            break;

          case 2:             // Protect Neg & 0
            key_protected = (value > 0.0) ? false : true;
            break;

          case 3:             // Protect First Key
            key_protected = is_first;
            break;

          case 4:             // Protect Last Key
            key_protected = (new_key == NULL) ? true : false;
            break;
        }

        is_first = false;

        if( !key_protected ) {
          switch( mode ) {
            case 0:           // Delete Key
              if( (value >= (from - threshold)) && (value <= (from + threshold)) )
                (*env_funcs->destroyKey)( env, key );

              if( value > (from + threshold) )
                break;
              break;

            case 2:           // Delete Keys Within Range
              if( (value >= (from - threshold)) && (value <= (to + threshold)) )
                (*env_funcs->destroyKey)( env, key );

              if( value > (to + threshold) )
                break;
              break;

            case 3:           // Delete Keys Outside Range
              if( (value < from) || (value > to) )
                (*env_funcs->destroyKey)( env, key );
              break;

            case 4:           // Delete Keys Before Range
              if( value < from )
                (*env_funcs->destroyKey)( env, key );
              else
                break;
              break;

            case 5:           // Delete Keys After Range
              if( value > to )
                (*env_funcs->destroyKey)( env, key );
              break;


            case 7:           // Clear Motion
              (*env_funcs->destroyKey)( env, key );
              break;

            case 8:           // Clear Motion Except Frame 0
              if( value != 0.0 ) 
                (*env_funcs->destroyKey)( env, key );
              break;

            case 9:           // Clear Motion After Frame 0
              if( value > 0.0 )
                (*env_funcs->destroyKey)( env, key );
              break;
          }
        }

        key = new_key;
      }

      // If there aren't any keys left in this channel, add a default Frame 0
      key = (*env_funcs->nextKey)( env, NULL );
      if( key == NULL )
        (*env_funcs->createKey)( env, 0.0, ((i < 6) ? 0.0 : 1.0) );
    }
  }

  // Process Children
  for( unsigned long i=0; i < item->GetChildren().NumElements(); i++ ) {
    if( !ProcessItem( item->GetChildren()[i], mode, channels, from, to, threshold,
                                              protection, all_other_channels ) ) {
      return false;        // User Abort
    }
  }

  return true;
}


// OpenProgressPanel():
int Motify_Interface::OpenProgressPanel() {
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

  DrawFuncs *df = mint->panel_funcs->drawFuncs;

  // Clear the bar
  (*df->drawRGBBox)( mint->progress_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

  // Fill the progress portion of the bar
  (*df->drawRGBBox)( mint->progress_panel, 0, 60, 150, cx+1, cy+1,
                    (int)((cw-2) * ( ((double)mint->progress_current) / ((double)mint->progress_total) ) ), ch-2 );

  // Write some text
  (*df->drawText)( mint->progress_panel, mint->progress_string, COLOR_WHITE, cx + 3, cy + 2 );
}

