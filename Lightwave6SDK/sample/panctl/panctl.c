/*
======================================================================
panctl.c

Test all of the LWPanels controls.

Ernie Wright  28 Jun 00
====================================================================== */

#include <lwserver.h>
#include <lwhost.h>
#include <lwgeneric.h>
#include <lwpanel.h>
#include <stdio.h>


/* defined in other source files */

int open_editpan( LWPanelFuncs * );
int open_sliderpan( LWPanelFuncs * );
int open_choicepan( LWPanelFuncs * );
int open_treepan( LWPanelFuncs * );
int open_colorpan( LWPanelFuncs * );
int open_filepan( LWPanelFuncs * );
int open_drawpan( LWPanelFuncs * );


static LWPanelFuncs *panf;


/*
======================================================================
event()

Button callback.  Each button opens a different subpanel.
====================================================================== */

static void event( LWControl *ctl, int ( *open )( LWPanelFuncs * ))
{
   open( panf );
}


/*
======================================================================
get_panel()

Create the main panel.
====================================================================== */

static LWPanelID get_panel( void )
{
   LWPanControlDesc desc;
   LWPanelID panel;
   LWValue ival = { LWT_INTEGER };
   LWControl *ctl[ 9 ];
   int i;

   if( !( panel = PAN_CREATE( panf, "Panels Control Test" )))
      return NULL;

   ctl[ 0 ] = BUTTON_CTL( panf, panel, "Edit" );
   ctl[ 1 ] = BUTTON_CTL( panf, panel, "Slider" );
   ctl[ 2 ] = BUTTON_CTL( panf, panel, "Choice" );
   ctl[ 3 ] = BUTTON_CTL( panf, panel, "Tree" );
   ctl[ 4 ] = BUTTON_CTL( panf, panel, "Color" );
   ctl[ 5 ] = BUTTON_CTL( panf, panel, "File" );
   ctl[ 6 ] = BUTTON_CTL( panf, panel, "Draw" );

   ctl[ 7 ] = BOOL_CTL( panf, panel, "Boolean" );
   ctl[ 8 ] = BOOLBUTTON_CTL( panf, panel, "Bool Button" );

   CON_SETEVENT( ctl[ 0 ], event, open_editpan );
   CON_SETEVENT( ctl[ 1 ], event, open_sliderpan );
   CON_SETEVENT( ctl[ 2 ], event, open_choicepan );
   CON_SETEVENT( ctl[ 3 ], event, open_treepan );
   CON_SETEVENT( ctl[ 4 ], event, open_colorpan );
   CON_SETEVENT( ctl[ 5 ], event, open_filepan );
   CON_SETEVENT( ctl[ 6 ], event, open_drawpan );

   /* align */

   for ( i = 0; i < 9; i++ ) {
      ival.intv.value = 100 - CON_LW( ctl[ i ] );
      ctl[ i ]->set( ctl[ i ], CTL_X, &ival );
   }

   return panel;
}


/*
======================================================================
PanControlTest()

Entry point.
====================================================================== */

XCALL_( int )
PanControlTest( long version, GlobalFunc *global, LWLayoutGeneric *local,
   void *serverData )
{
   LWPanelID panel;
   unsigned long sysid;
   long ver;

   sysid = ( unsigned long ) global( LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT );
   ver = ( sysid & LWINF_BUILD ) == 429 ? 2 : 4;
   if ( version != ver ) return AFUNC_BADVERSION;

   panf = global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
   if ( !panf ) return AFUNC_BADGLOBAL;

   panf->globalFun = global;

   panel = get_panel();
   if ( !panel ) return AFUNC_BADGLOBAL;

   panf->open( panel, PANF_BLOCKING );

   PAN_KILL( panf, panel );

   return AFUNC_OK;
}


ServerRecord ServerDesc[] = {
   { LWLAYOUTGENERIC_CLASS, "PanControlTest", PanControlTest },
   { NULL }
};
