//
// LWProgressPanel.cpp
//

#include "LWProgressPanel.h"

// Constructor
LWProgressPanel::LWProgressPanel( GlobalFunc *_global, int _total ) :
  is_open(false), total(_total), current(0), string(NULL),
  abort_title(NULL), abort_string1(NULL), abort_string2(NULL),
  panel(NULL), global(_global) {

  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );
}

// Destructor
LWProgressPanel::~LWProgressPanel() {
  if( is_open )
    Close();

  if( panel != NULL )
    (*panel_funcs->destroy)( panel );
}

// Open():
int LWProgressPanel::Open( const char * _title ) {
  if( is_open )
    return 1;

  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  // Create the panel
  panel = (*panel_funcs->create)( (char *)_title, panel_funcs );
  if( !panel ) {              // Make sure the panel was created
    (*message->error)("Error creating progress panel, aborting", NULL );
    return -1;
  }

  // Progress Field
  bar = CANVAS_CTL( panel_funcs, panel, "", 400, 20 );
  CON_SETEVENT( bar, NULL, this );

  // Draw Callbacks
  ival.intv.value = (int)&DrawProgressBar;
  (*bar->set)( bar, CTL_USERDRAW, &ival );

  // Set up some variables
  is_open = true;

  // Open the Panel
  int retval = (*panel_funcs->open)( panel, PANF_PASSALLKEYS );

  return retval;
}

// Close()
void LWProgressPanel::Close() {
  (*panel_funcs->close)( panel );
}

// DrawProgressBar():
void DrawProgressBar( LWControl *con, void *data, DrMode mode ) {
  LWProgressPanel *inst = (LWProgressPanel *)data;

  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = inst->panel_funcs->drawFuncs;

  // Clear the bar
  (*df->drawRGBBox)( inst->panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

  // Fill the progress portion of the bar
  (*df->drawRGBBox)( inst->panel, 0, 60, 150, cx+1, cy+1,
                    (int)((cw-2) * ( ((double)inst->GetCurrent()) / ((double)inst->GetTotal()) ) ), ch-2 );

  // Write some text
  if( inst->GetString() != NULL )
    (*df->drawText)( inst->panel, (char *)inst->GetString(), COLOR_WHITE, cx + 3, cy + 2 );
}

// Handle():
bool LWProgressPanel::Handle() {
  if( is_open ) {
    REDRAW_CON( bar );

    // Update the panel and see if the user closed it (ie: cancel operation)
    if( (*panel_funcs->handle)( panel, 0 ) == -1) {
      if( (*message->yesNo)( ((abort_title   == NULL) ? "Abort Processing?" : abort_title),
                             ((abort_string1 == NULL) ? "Abort Processing?" : abort_string1),
                             ((abort_string2 == NULL) ? NULL                : abort_string2) ) == 1 ) {
        is_open = false;
        return false;
      } else {
        (*panel_funcs->open)( panel, PANF_PASSALLKEYS );
      }
    }
  }

  return true;
}

