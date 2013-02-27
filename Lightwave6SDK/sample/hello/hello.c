/*
======================================================================
hello.c

A simple LayoutGeneric example.

Ernie Wright  20 Mar 00
====================================================================== */

#include <lwserver.h>            /* all plug-ins need this        */
#include <lwgeneric.h>           /* for the LayoutGeneric class   */
#include <lwpanel.h>             /* for "classic" panels          */
#include <lwhost.h>              /* for the LWMessageFuncs global */
#include <stdio.h>               /* for NULL #define              */

LWMessageFuncs *msg;             /* the message functions         */
LWPanelFuncs *panf;              /* the panel functions           */
LWPanelID panel;                 /* the panel                     */
LWControl *ctl;                  /* a control on the panel        */
LWPanControlDesc desc;           /* used by macros in lwpanel.h   */
LWValue sval = { LWT_STRING };   /*  read lwpanel.h to see how    */
char edit[ 80 ] =                /* string for the edit control   */
   "This is an edit field.";


/*
======================================================================
DemoGeneric()

The activation function.  Layout calls this when the user selects the
generic from the generic list on the General Options panel.
====================================================================== */

XCALL_( int )
DemoGeneric( long version, GlobalFunc *global, LWLayoutGeneric *local,
   void *serverData )
{
   int ok;
   unsigned long sysid;
   long ver;

   /* Make sure Layout calls us with the right version of the
      LWLayoutGeneric structure.  Because of a version numbering bug
      in build 429, we need to take a couple of extra steps in generic
      class plug-ins to verify the version. */

   sysid = ( unsigned long ) global( LWSYSTEMID_GLOBAL, GFUSE_TRANSIENT );
   ver = ( sysid & LWINF_BUILD ) == 429 ? 2 : 4;
   if ( version != ver ) return AFUNC_BADVERSION;

   /* get the message and panels functions */

   msg = global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
   panf = global( LWPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
   if ( !msg || !panf )
      return AFUNC_BADGLOBAL;

   /* initialize the panels functions and create a panel */

   panf->globalFun = global;

   panel = PAN_CREATE( panf, "Hello World!" );
   if ( !panel ) return AFUNC_BADGLOBAL;

   /* create an edit field on the panel and initialize its contents */

   ctl = STR_CTL( panf, panel, "Edit Me", 40 );
   SET_STR( ctl, edit, sizeof( edit ));

   /* display the panel; this waits until the user closes it */

   ok = panf->open( panel, PANF_BLOCKING | PANF_CANCEL );

   /* ok is TRUE if the user pressed OK, FALSE if he or she pressed Cancel */

   if ( ok )
      GET_STR( ctl, edit, sizeof( edit ));

   /* free the panel */

   PAN_KILL( panf, panel );

   /* display the contents of the edit string */

   msg->info( "The edit string contains:", edit );

   /* while we're here, let's issue a command */

   msg->info( "I'm going to issue the \"About\" command now.", NULL );

   local->evaluate( local->data, "About" );

   /* done! */

   return AFUNC_OK;
}


/*
======================================================================
This is the server description.  LightWave looks at this first to
determine what plug-ins the file contains.  It lists each plug-in's
class and internal name, along with a pointer to the activation
function.  You can optionally add a user name, or more than one in
different languages, if you like.
====================================================================== */

ServerRecord ServerDesc[] = {
   { LWLAYOUTGENERIC_CLASS, "HelloWorld", DemoGeneric },
   { NULL }
};
