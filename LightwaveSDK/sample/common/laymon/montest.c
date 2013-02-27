/****
 *
 * montest.c
 *
 * Description:
 *  LayoutGeneric plug-in to test operation of the LAYMON
 *   progress monitor global class plug-in
 *
 ****
 ****
 *
 * Modification History:
 *   RAM - 02/25/98 - Initial Creation
 *
 * Author:
 *   Ryan A. Mapes (NewTek, Inc.)
 *
 */


////////////////////////////////
//         INCLUDES           //
////////////////////////////////

#include <stdio.h>

// LW includes
#include <splug.h>
#include <lwran.h>
#include <hdisp.h>

// LW Panel includes
#include <lwpanel.h>
#include <laymon.h>

////////////////////////////////
//      MAIN PROCESSING       //
////////////////////////////////

// A quick macro to slow procesing done so we
//  can see a little more of what's happening
//  uncomment if you desire
//#define SLOW_DOWN  Sleep(10);
#define SLOW_DOWN

// The follow are not required - just used to set the total steps
#define BARE_COUNT    10
#define TINY_COUNT    255
#define SMALL_COUNT   480
#define BIG_COUNT     640

////////////////////////////////
////////////////////////////////
//
//     ACTIVATION FUNCTION
//        (ENTRY POINT)
//
////////////////////////////////
////////////////////////////////
int LWLMTEST_activate ( long, GlobalFunc *, void *, void * );

////////
// Activation function
////////
// layoutgeneric contains single point of entry
// once it exits, that is all there is for
// this plug-in's execution :)
////////

int LWLMTEST_activate ( long           version,
                        GlobalFunc    *global,
                        void          *locPtr,
                        void          *serverData ) {

  LWLMonitor   *mon = global ( LAYMON_SERVICES_NAME, GFUSE_TRANSIENT );
  LWLMonID      monID, row_monID, col_monID;

  MessageFuncs *mf = global ( "Info Messages", GFUSE_TRANSIENT );

  char  str_buf[128];
  int   i,j,k;
  int   tot_size = 0;


  ////////
  // Example one
  ////////
  // Sample using LAYMON_DFLT, LAYMON_INCR, LAYMON_DONE
  // (just like mom ... I mean Modeler ... does it:)
  ////////

  (*mf->info) ( "Starting Example One", "Basic operations with defaults" );

  monID = LAYMON_DFLT ( mon, BIG_COUNT );
  
  while ( tot_size < BIG_COUNT ) {

    SLOW_DOWN;

    if ( LAYMON_INCR ( mon, monID, 1, "Processing Nothing!!!", "Blah blah excitement blah" ) == LAYMON_USERCANCEL ) break;
    tot_size++;

  } // End while

  LAYMON_DONE ( mon, monID );


  ////////
  // Example two
  ////////
  // Now let's try a
  //  wacky,
  //  fun-filled, 
  //  not-so-visually-pleasing,
  //  nor informative,
  //  and rather annoying,
  //  monitor test
  ////////

  (*mf->info) ( "Starting Example Two", "Multiple monitors - NO reset" );
  row_monID = LAYMON_INIT ( mon, BARE_COUNT, 100, 0, LAYMON_FALSE, "Rows", "Processing Row", "Initializing" );

  for ( i=0; i<BARE_COUNT; i++ ) {

    SLOW_DOWN;
    if ( LAYMON_STEP ( mon, row_monID ) == LAYMON_USERCANCEL ) break;

    SLOW_DOWN;
    col_monID = LAYMON_INIT ( mon, BIG_COUNT, 240, 240, LAYMON_TRUE, "Columns", "Processing Column", "Initializing" );

    for ( j=0; j<BIG_COUNT; j++ ) {

      SLOW_DOWN;
      if ( LAYMON_STEP ( mon, col_monID ) == LAYMON_USERCANCEL ) break;

    } // Columns

    SLOW_DOWN;
    LAYMON_DONE ( mon, col_monID );

    if ( LAYMON_UPD ( mon, row_monID, "Row complete" ) == LAYMON_USERCANCEL ) break;

  } // Rows

  LAYMON_DONE ( mon, row_monID );


  ////////
  // Example three
  ////////
  // Repeat example two above using the LAYMON_RSET macro
  // BTW: I usually mistype that as LAYMON_REST but, alas,
  //  the compiler likes to scream and yell at me anyway
  //  so what's one more :)
  ////////
  (*mf->info) ( "Starting Example Three", "Color changes and reset" );
  // (also position the monitors to make a little sense :)
  row_monID = LAYMON_INIT ( mon, SMALL_COUNT, 200, 200, LAYMON_FALSE, "Rows", "Processing Row", "Initializing" );
  col_monID = LAYMON_INIT ( mon, BIG_COUNT, 450, 200, LAYMON_TRUE, "Columns", "Processing Column", "Initializing" );

  // Do some color shifting during processing
  LAYMON_RBAR ( mon, row_monID, 90, 95 );
  LAYMON_RBAR ( mon, col_monID, 66, 14 );
  LAYMON_CLOW  ( mon, col_monID, 190, 200, 15 );
  LAYMON_CMID  ( mon, col_monID, 32, 6, 242 );
  LAYMON_CHIGH ( mon, col_monID, 14, 102, 174 );

  LAYMON_CLOW  ( mon, row_monID, 211, 110, 0 );
  LAYMON_CMID  ( mon, row_monID, 132, 99, 7 );
  LAYMON_CHIGH ( mon, row_monID, 69, 0, 174 );

  for ( i=0; i<SMALL_COUNT; i++ ) {

    SLOW_DOWN;
    if ( LAYMON_STEP ( mon, row_monID ) == LAYMON_USERCANCEL ) break;

    SLOW_DOWN;
    LAYMON_RSET ( mon, col_monID, NULL, "Starting next line" );

    for ( j=0; j<BIG_COUNT; j++ ) {

      SLOW_DOWN;
      if ( LAYMON_STEP ( mon, col_monID ) == LAYMON_USERCANCEL ) break;

    } // Columns

    SLOW_DOWN;
    if ( LAYMON_UPD ( mon, row_monID, "Row complete" ) == LAYMON_USERCANCEL ) break;

  } // Rows

  LAYMON_DONE ( mon, col_monID );
  LAYMON_DONE ( mon, row_monID );



  ////////
  // Example four
  ////////
  // Let's try the same thing in a slightly nicer style with LAYMON_UPD
  ////////
  (*mf->info) ( "Starting Example Four", "Updates to user info messages" );
  monID = LAYMON_INIT ( mon, SMALL_COUNT, 400, 400, LAYMON_TRUE, "Processing Image", "Initializing", "Initializing" );
  LAYMON_COLR ( mon, monID, 255, 255, 255 );

  for ( i=0; i<SMALL_COUNT; i++ ) {

    SLOW_DOWN;
    sprintf ( str_buf, "Processing Row #%4d", i );
    if ( LAYMON_INCR ( mon, monID, 1, str_buf, "Starting Columns" ) == LAYMON_USERCANCEL) break;

    for ( j=0; j<BIG_COUNT; j++ ) {

      if ( !( j % 100 ) ) {

        SLOW_DOWN;
        sprintf ( str_buf, "Column #%4d", j );
        if ( LAYMON_UPD ( mon, monID, str_buf ) == LAYMON_USERCANCEL ) break;

      } // End if should update this cylce

    } // Columns

    SLOW_DOWN;
    if ( LAYMON_UPD ( mon, monID, "Row complete" ) == LAYMON_USERCANCEL ) break;

  } // Rows

  LAYMON_DONE ( mon, monID );

  ////////
  // Example five
  ////////
  // And, just so you all think I am going slightly overboard ...
  // Like, y'know, world harmony and all =)
  ////////
  (*mf->info) ( "Starting Example Five", "My favorite :)" );
  monID = LAYMON_INIT ( mon, (5*TINY_COUNT), -1, -1, LAYMON_TRUE, "Ryan's Favorite", "(: oooOOOooo :)", "Dynamic Colors =)" );

  i=j=k=0;

  for ( i=0; i<TINY_COUNT; i++ ) {
        SLOW_DOWN;
        sprintf ( str_buf, "R:%3d G:%3d B:%3d", i, j, k );
        LAYMON_COLR ( mon, monID, i, j, k );
        if ( LAYMON_INCR ( mon, monID, 1, NULL, str_buf ) == LAYMON_USERCANCEL ) break;
  }
  for ( j=0; j<TINY_COUNT; j++ ) {
        SLOW_DOWN;
        sprintf ( str_buf, "R:%3d G:%3d B:%3d", i, j, k );
        LAYMON_COLR ( mon, monID, i, j, k );
        if ( LAYMON_INCR ( mon, monID, 1, NULL, str_buf ) == LAYMON_USERCANCEL ) break;
  }
  for ( i=TINY_COUNT; i>0; i-- ) {
        SLOW_DOWN;
        sprintf ( str_buf, "R:%3d G:%3d B:%3d", i, j, k );
        LAYMON_COLR ( mon, monID, i, j, k );
        if ( LAYMON_INCR ( mon, monID, 1, NULL, str_buf ) == LAYMON_USERCANCEL ) break;
  }
  for ( k=0; k<TINY_COUNT; k++ ) {
        SLOW_DOWN;
        sprintf ( str_buf, "R:%3d G:%3d B:%3d", i, j, k );
        LAYMON_COLR ( mon, monID, i, j, k );
        if ( LAYMON_INCR ( mon, monID, 1, NULL, str_buf ) == LAYMON_USERCANCEL ) break;
  }
  for ( j=TINY_COUNT; j>0; j-- ) {
        SLOW_DOWN;
        sprintf ( str_buf, "R:%3d G:%3d B:%3d", i, j, k );
        LAYMON_COLR ( mon, monID, i, j, k );
        if ( LAYMON_INCR ( mon, monID, 1, NULL, str_buf ) == LAYMON_USERCANCEL ) break;
  }

  LAYMON_DONE ( mon, monID );


  return ( AFUNC_OK );
  
} // End TESTLWLM_Activate sub_routine


////////
// Plugin service description (entry points)
////////
ServerRecord ServerDesc[] = {
  { "LayoutGeneric", "Laymon_Test", LWLMTEST_activate },
  { NULL }
}; // End ServerRecord declare (note the semi-colon)
