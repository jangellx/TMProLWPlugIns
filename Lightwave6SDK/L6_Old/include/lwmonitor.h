/*
 * LWSDK Header File
 * Copyright 1999, NewTek, Inc.
 *
 * LWMONITOR.H -- LightWave Progress Monitor
 *
 * Monitors are simple data structures defining an interface which the
 * server can use to give feedback to the host on its progress in
 * performing some task.  They are sometimes passed servers to give
 * feedback on the progress of the particular operation, and can sometimes
 * be accessed from within a server that wants to show its progress on a
 * slow operation using the host's normal feedback display.
 */
#ifndef LWSDK_MONITOR_H
#define LWSDK_MONITOR_H


/*
 * An LWMonitor struct consists of some generic data and three functions:
 * init, step and done.  The 'init' function is called first with the
 * number of steps in the process to be monitored, which is computed by
 * the server.  As the task is processed, the 'step' function is called
 * with the number of steps just completed (often one).  These step
 * increments should eventually add up to the total number and then the
 * 'done' function is called, but 'done' may be called early if there was
 * a problem or the process was aborted.  The 'step' function will return
 * one if the user requested an abort and zero otherwise.
 *
 * The server is masked from any errors in the monitor that may occur on
 * the host side of the interface.  If there is a problem with putting up
 * a monitor, the functions will still return normally, since the monitor
 * is for user feedback and is not that critical.
 */
typedef struct st_LWMonitor {
    void         *data;
    void        (*init) (void *, unsigned int);
    int        (*step) (void *, unsigned int);
    void        (*done) (void *);
} LWMonitor;

/*
 * Macros are provided to call a monitor which will do nothing if the
 * monitor pointer is null.  MON_INCR is used for step sizes greater than
 * one and MON_STEP is used for step sizes exactly one.
 */
#define MON_INIT(mon,count)    if (mon) (*mon->init) (mon->data, count)
#define MON_INCR(mon,d)        (mon ? (*mon->step) (mon->data, d) : 0)
#define MON_STEP(mon)        MON_INCR (mon, 1)
#define MON_DONE(mon)        if (mon) (*mon->done) (mon->data)

#endif

