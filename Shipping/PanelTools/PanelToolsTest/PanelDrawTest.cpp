//
// PanelDrawTest.cpp
//

#define NULL 0

#include "..\PanelToolsGlobal.h"

#include <stdio.h>

#include <lwgeneric.h>
#include <lwhost.h>

extern LWPanControlDesc desc;
extern LWValue ival;
extern LWValue ivecval;
extern LWValue fval;
extern LWValue fvecval;
extern LWValue sval;

extern "C" {
  XCALL_ (int) PanelDrawTest_Activate(  long version, GlobalFunc *global, void *local, void *serverData );
}

void DrawPanelDrawTest( LWPanelID panel, void *data, DrMode mode);

struct PanelDrawData {
  TMP_PanelDraw  *draw;
  LWPanelFuncs   *panel_funcs;
  LWMessageFuncs *message;
  LWPanelID       panel;
};

PanelDrawData pddata;

// PanelDrawTest_Activate():
XCALL_ (int) PanelDrawTest_Activate( long version, GlobalFunc *global, void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  pddata.panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,   GFUSE_TRANSIENT );
  pddata.draw        = (TMP_PanelDraw   *)global( TMP_PANELDRAW_GLOBAL,  GFUSE_TRANSIENT );
  pddata.message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );

  // Create the panel
  char title_string[256];
  sprintf( title_string, "PanelTools Test -- %c2001 Joe Angell, TM Productions", 169 );
  pddata.panel = (*pddata.panel_funcs->create)( title_string, pddata.panel_funcs );
  if( !pddata.panel ) {                    // Make sure the panel was created
    (*pddata.message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  (*pddata.panel_funcs->set)( pddata.panel, PAN_USERDRAW, DrawPanelDrawTest );

  PAN_SETW( pddata.panel_funcs, pddata.panel, 600 );
  PAN_SETH( pddata.panel_funcs, pddata.panel, 800 );
  pddata.panel_funcs->open( pddata.panel, PANF_BLOCKING );

  return AFUNC_OK;
}

// DrawPanelDrawTest() {
void DrawPanelDrawTest( LWPanelID panel, void *data, DrMode mode) {
  // Checkerboard
  pddata.draw->checkerboard( pddata.panel, pddata.panel_funcs->drawFuncs,
                             5, 5, 590, 100, 20, 20,
                             RGB_( 0, 0, 0 ), RGB_( 255, 255, 255 ), TMPCHECKMASK_BOTH );

  // Dividers
  pddata.draw->horizontalDivider( pddata.panel, pddata.panel_funcs->drawFuncs, 0, 115, 600, 2 );
  pddata.draw->verticalDivider(   pddata.panel, pddata.panel_funcs->drawFuncs, 5,   0, 200, 2 );

  // Boxes
  pddata.draw->emptyBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                          15, 120,  90, 90, 1, 1, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ) );
  pddata.draw->emptyBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                         115, 120,  90, 90, 1, 2, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ) );

  pddata.draw->emptyBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                         215, 120,  90, 90, 5, 1, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ) );

  pddata.draw->filledBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                           15, 220,  90, 90, 1, 1, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 255, 0, 0 ) );
  pddata.draw->filledBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                          115, 220,  90, 90, 1, 2, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 80, 80, 80 ) );
  pddata.draw->filledBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                          215, 220,  90, 90, 5, 5, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 0, 0, 255 ) );

  pddata.draw->buttonBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                           15, 320,  90, 90, 1, 1, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 255, 0, 255 ) );
  pddata.draw->buttonBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                          115, 320,  90, 90, 1, 2, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 0, 255, 255 ) );
  pddata.draw->buttonBox( pddata.panel, pddata.panel_funcs->drawFuncs,
                          215, 320,  90, 90, 5, 1, RGB_( 0, 0, 0, ), RGB_( 255, 255, 255 ), RGB_( 255, 255, 0 ) );

  // Gradients
  pddata.draw->horizontalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                   315, 120, 100, 20,   0, 0, 0,   255, 255, 255 );

  pddata.draw->horizontalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                   315, 150, 100, 20,   255, 0, 0,   0, 0, 0 );

  pddata.draw->horizontalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                   315, 180, 100, 20,   0, 0, 0,   0, 255, 0 );

  pddata.draw->horizontalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                   315, 210, 100, 20,   0, 0, 255,   0, 0, 0 );


  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 435, 120, 20, 100,   0, 0, 0,   255, 255, 255 );

  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 465, 120, 20, 100,   255, 0, 0,   0, 0, 0 );

  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 495, 120, 20, 100,   0, 0, 0,   0, 255, 0 );

  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 525, 120, 20, 100,   0, 0, 255,   0, 0, 0 );


  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 315, 240, 100, 20,   100, 150, 200,   20, 70, 120 );

  pddata.draw->verticalGradient( pddata.panel, pddata.panel_funcs->drawFuncs,
                                 435, 240, 100, 20,   20, 70, 120,   100, 150, 200 );

  // Buttons
  pddata.draw->button( pddata.panel, pddata.panel_funcs->drawFuncs,
                       315, 270, 100, 20,   184, 196, 184,  0 );

  pddata.draw->button( pddata.panel, pddata.panel_funcs->drawFuncs,
                       435, 270, 100, 20,   164, 164, 164,  1 );

  // Icons
  int x=5, y=420;
  for( int i=0; i <= TMPICON_SLIDER_KNOB_TICK_HORIZONTAL; i++ ) {
    pddata.draw->icon( pddata.panel, pddata.panel_funcs->drawFuncs,
                       x, y, (TMP_IconTypes) i, RGB_( 0, 0, 0 ), RGB_( 200, 0, 0 ) );

    x += pddata.draw->iconWidth( (TMP_IconTypes) i ) + 5;
  }

  // Arrows
  x = 10;
  y = 440;
  for( i=0; i <= TMPARROW_4WAY; i++ ) {
    pddata.draw->arrow( pddata.panel, pddata.panel_funcs->drawFuncs,
                        x, y, (TMP_ArrowTypes)i, TMPARROW_SOLID, RGB_( 0, 0, 0 ) );

    pddata.draw->arrow( pddata.panel, pddata.panel_funcs->drawFuncs,
                        x, y+20, (TMP_ArrowTypes)i, TMPARROW_EMPTY, RGB_( 0, 0, 0 ) );

    pddata.draw->arrow( pddata.panel, pddata.panel_funcs->drawFuncs,
                        x, y+40, (TMP_ArrowTypes)i, TMPARROW_TWO_EDGES, RGB_( 0, 0, 0 ) );
    x += 20;
  }

  // Fold Icons
  x = 10;
  y = 500;
  pddata.draw->foldIcon( pddata.panel, pddata.panel_funcs->drawFuncs,
                         x, y, 10, 10, RGB_( 200, 200, 200 ), 0, TMPICON_FOLD_TRIANGLE );
  pddata.draw->foldIcon( pddata.panel, pddata.panel_funcs->drawFuncs,
                         x + 20, y, 10, 10, RGB_( 200, 200, 200 ), 1, TMPICON_FOLD_TRIANGLE );

  pddata.draw->foldIcon( pddata.panel, pddata.panel_funcs->drawFuncs,
                         x + 40, y, 9, 9, RGB_( 200, 200, 200 ), 0, TMPICON_FOLD_PLUS_MINUS );
  pddata.draw->foldIcon( pddata.panel, pddata.panel_funcs->drawFuncs,
                         x + 60, y, 9, 9, RGB_( 200, 200, 200 ), 1, TMPICON_FOLD_PLUS_MINUS );

  // Resize Handle
  pddata.draw->panelResizeHandle( pddata.panel_funcs, pddata.panel, 0 );
}
