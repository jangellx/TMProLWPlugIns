//
// PointDump.cpp
//

#include <lwserver.h>
#include <lwcmdseq.h>
#include <lwmeshedt.h>
#include <lwhost.h>

#include <fstream.h>

#include <stdlib.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

// Globals
ofstream *out;

// SelectPointsOfPolys_PolyScan():
EDError PointDump_Scan( void *_op, const EDPointInfo *point ) {
  MeshEditOp *op = (MeshEditOp *)_op;

  if( point->flags & EDDF_SELECT )
	*out << point->position[0] << " " << point->position[1] << " " << point->position[2] << endl;

  return EDERR_NONE;
}

// SelectPointsOfPolys_Activate():
XCALL_ (int) PointDump_Activate( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {
  if ( version != LWMODCOMMAND_VERSION )
    return AFUNC_BADVERSION;

  LWModCommand * command = (LWModCommand *)local;
  if( command == NULL )
    return AFUNC_BADLOCAL;

  // Ask the user for a filename
  LWFileReqFunc *filereq = (LWFileReqFunc *)global( LWFILEREQFUNC_GLOBAL, GFUSE_TRANSIENT );
  if( !filereq )
    return AFUNC_BADGLOBAL;

  char path[     512 ] = ">";
  char filename[ 512 ] = "";
  char buffer[   512 ] = "";

  if( !filereq( "Select Dump File", filename, path, buffer, 512 ) )
    return AFUNC_OK;

  // Open the file
  out = new ofstream( buffer );
  if( !(*out) )
    return AFUNC_OK;

  // Loop through the points
  MeshEditOp * op  = (command->editBegin)( 0, 0, OPSEL_DIRECT | OPSEL_MODIFY );
  EDError      err = op->pointScan( op->state, PointDump_Scan, (void *)op, OPLYR_FG );
  op->done( op->state, err, 0 );

  delete out;

  return AFUNC_OK;
}

// Definitions
extern "C" {
  ServerUserName PointDump_Usernames[]      = { {"TM-P  Point Dump"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Point Dump"   DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWMODCOMMAND_CLASS, "TM-P_PointDump"DEBUG_PLUGIN_PLUGNAME, PointDump_Activate, PointDump_Usernames },
    { (const char *)NULL }
  };
}
