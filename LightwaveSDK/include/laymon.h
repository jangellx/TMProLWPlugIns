/*
 * laymon.h
 *
 * COPYRIGHT (C) 1998 NewTek, Inc.
 *
 * Description:
 * LW Layout Monitor Global-class plugin support
 *  public header file	
 *
 * Author:
 *	Ryan A. Mapes
 *
 * Modification History:
 *  02/23/98 - RAM - Initial creation
 *
 ****
 *
 * Usage:
 * Declare a monitor services global pointer
 *   LWLMonitor *my_mon;
 *   LWLMonID    my_monID;
 *
 * Obtain a monitor services structure
 *   my_mon    = global( LAYMON_SERVICES_NAME, GFUSE_TRANSIENT );
 *
 * Create (initialise) the monitor
 *   >>> LAYMON_TRUE (or LAYMON_FALSE) indicates to include a pause button
 *   my_monID  = LAYMON_DFLT ( my_mon, 69 );  // Uses default settings - most likely you will want LAYMON_INIT instead
 *   my_monID  = LAYMON_INIT ( my_mon, 69, 320, 240, LAYMON_TRUE, "Nifty Title", "Processing Image", "Initialising" );
 *
 * Increment counter by given value and display category (msg) and sub-category (info) messages (strings may be NULL)
 * rc is return code (if == LAYMON_USERCANCEL --> user clicked 'Abort')
 *   rc = LAYMON_INCR ( my_mon, my_monID, 1, "Still Processing ...", "Want some coffee?" );
 * Increment counter (retain existing messages)
 *   rc = LAYMON_STEP ( my_mon, my_monID );
 * Update the sub-category message (does NOT increment counter)
 *   rc = LAYMON_UPD  ( my_mon, my_monID, "Cream or Sugar?" );
 *
 * Reset the counter (e.g., for repeated loops)
 * - useful when wishing to track another something with the same number of steps
 *   without closing and creating a new monitor
 *   rc = LAYMON_RSET ( my_mon, my_monID );
 *
 * Set the percentages for a color change (currently only low, middle, high ranges)
 *   (void)LAYMON_RBAR ( my_mon, my_monID, 20, 80 );  // Low set to 20%, high set to 80%
 * Sets the progress bar color once progress reaches the percent
 *   (void)LAYMON_CLOW  ( my_mon, my_monID, 255, 0, 0 );  // Red
 *   (void)LAYMON_CMID  ( my_mon, my_monID, 0, 255, 0 );  // Green
 *   (void)LAYMON_CHIGH ( my_mon, my_monID, 0, 0, 255 );  // Blue
 * Modifies the color for all ranges
 *   (void)LAYMON_COLR ( my_mon, my_monID, 255, 255, 255 ); // All white
 *
 * Close the monitor
 *   (void)LAYMON_DONE ( my_mon, my_monID );
 ****
 * Minimum subset: MON_INIT, MON_STEP, MON_DONE
 */
                                      
// Execute sub-routine declarations (unless previously performed)
#ifndef LAYMON_H
#define LAYMON_H


////////////////////////////////
//         INCLUDES           //
////////////////////////////////

////////////////////////////////
//         DEFINES            //
////////////////////////////////

// defines
#define LAYMON_NULL                      0
#define LAYMON_SERVICES_NAME             "Layout_Monitor"


// Truth defines
#define LAYMON_TRUE                       1
#define LAYMON_FALSE                      0
#define LAYMON_USERCANCEL                 -1

///////////////////////////////
//      GLOBAL CONSTANTS      //
////////////////////////////////

// eek!!!

////////////////////////////////
//         TYPEDEFS           //
//        STRUCTURES          //
////////////////////////////////
typedef void *LWLMonID;
typedef enum { 	 // Tags to set attributes
  LWLM_RANGE,LWLM_CLOW,LWLM_CMID,LWLM_CHIGH,LWLM_CALL,
  LWLM_RESET,LWLM_VERSION,LWLM_FULLNAME,LWLM_EGO
} LWLM_tag;

typedef struct st_LWLMonitor {
  LWLMonID        (*init)  ( unsigned int,int,int,unsigned int,char *,char *,char *); // Monitor create
  int             (*step)  ( LWLMonID,unsigned int,char *,char *);                    // Monitor increment
  int             (*set)   ( LWLMonID, LWLM_tag,int,int,int );                        // Change monitor settings
  void            (*done)  ( LWLMonID );                                              // Monitor destroy
} LWLMonitor;

// Lightwave Layout Monitor macro fields
// mon       - your monitor structure
// monID     - monitor ID from create (MON_INIT)
// steps     - total steps to track in progress monitor
// pawz_flg  - enable a pause button
//             (blocks further processing until click continue)
// msg       - Informative message - "Category"
// info      - Informative message - "Sub-Category"
// inc       - number of steps to increment
// title     - Progress Monitor titlebar string
// posx,posy - x,y (max 639x479) position of monitor (-1,-1 uses default)
// r,g,b     - Red, Green and Blue values for desired color

// Create a monitor
#define LAYMON_INIT(mon,steps,posx,posy,pawz_flag,title,msg,info)  (mon ? (*mon->init) (steps,posx,posy,pawz_flag,title,msg,info) : 0 )
#define LAYMON_DFLT(mon,steps)                                     (mon ? (*mon->init) (steps,-1,-1,LAYMON_FALSE,"Progress Monitor","Please standby...", "Processing") : 0 )

// Increment counters (these three return LAYMON_USERCANCEL if user click 'Abort')
#define LAYMON_INCR(mon,monID,inc,msg,info)              (mon ? (*mon->step) (monID,inc,msg,info) : 0)
#define LAYMON_STEP(mon,monID)                           LAYMON_INCR(mon,monID,1,NULL,NULL)
#define LAYMON_UPD(mon,monID,info)                       LAYMON_INCR(mon,monID,0,NULL,info)

// Close the monitor
#define LAYMON_DONE(mon,monID)                           if(mon)(*mon->done) (monID)

// Monitor setting macros - What they do:
//  LAYMON_RBAR  - Set low and high  values for color shift (integer percent - eg, 33% as 33, not .33)
//  LAYMON_CLOW  - Low range color (default 0-33%)
//  LAYMON_CMID  - Middle range color (default 33-66%)
//  LAYMON_CHIGH - High range color (default 66-100%)
// NOTE: For the above, entire bar will become the indicated color at the given transition point
//  LAYMON_COLR  - Set all colors for all ranges to specified rgb values
//  LAYMON_RSET  - Reset the monitor counter and messages (if msg or info=NULL - retains current messages)
//  LAYMON_VER   - Retrieve the current version of Layout Monitor services

#define LAYMON_RBAR(mon,monID,low,high)                  if(mon)(*mon->set)(monID,LWLM_RANGE,low,high,(int)NULL)
#define LAYMON_COLR(mon,monID,r,g,b)                     if(mon)(*mon->set)(monID,LWLM_CALL,r,g,b)
#define LAYMON_CLOW(mon,monID,r,g,b)                     if(mon)(*mon->set)(monID,LWLM_CLOW,r,g,b)
#define LAYMON_CMID(mon,monID,r,g,b)                     if(mon)(*mon->set)(monID,LWLM_CMID,r,g,b)
#define LAYMON_CHIGH(mon,monID,r,g,b)                    if(mon)(*mon->set)(monID,LWLM_CHIGH,r,g,b)
#define LAYMON_RSET(mon,monID,msg,info)                  if(mon)(*mon->set)(monID,LWLM_RESET,(int)msg,(int)info,(int)NULL)

#define LAYMON_VER(mon,monID)                            if(mon)(*mon->set)(monID,LWLM_VERSION,(int)NULL,(int)NULL,(int)NULL)


// Close header
#endif // LAYMON_H
