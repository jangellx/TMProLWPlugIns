/*
======================================================================
plugin.c

Activation functions and callbacks for generic and displacement
handler class plug-ins.  These demonstrate the use of the object
database functions.

Ernie Wright  1 Jun 00
====================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <lwhost.h>
#include <lwgeneric.h>
#include <lwdisplce.h>
#include "objectdb.h"
#include "vecmat.h"

#define TEST_FILENAME "testodb.txt"    /* output file for the generic */


/*
======================================================================
ODBTest()

Activation function for the generic.  This creates and prints an
object database for the first object in the scene.
====================================================================== */

XCALL_( int )
ODBTest( long version, GlobalFunc *global, LWLayoutGeneric *local,
   void *serverData )
{
   LWMessageFuncs *msg;
   LWItemInfo *lwi;
   LWItemID objid;
   ObjectDB *odb;
   FILE *fp;


   msg = global( LWMESSAGEFUNCS_GLOBAL, GFUSE_TRANSIENT );
   if ( !msg ) return AFUNC_BADGLOBAL;

   lwi = global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );
   if ( !lwi ) {
      msg->error( "Couldn't get item info global.", NULL );
      return AFUNC_BADGLOBAL;
   }

   objid = lwi->first( LWI_OBJECT, 0 );
   if ( !objid ) {
      msg->error( "No object in the scene.", NULL );
      return AFUNC_OK;
   }

   odb = getObjectDB( objid, global );
   if ( !odb ) {
      msg->error( "Couldn't alloc object database.", NULL );
      return AFUNC_OK;
   }

   if ( fp = fopen( TEST_FILENAME, "w" )) {
      printObjectDB( odb, fp, 0 );
      fclose( fp );
   }
   else
      msg->error( "Couldn't open output file", TEST_FILENAME );

   freeObjectDB( odb );

   return AFUNC_OK;
}


/* stuff for the displacement handler */

typedef struct {
   LWItemID  objid;
   char      name[ 80 ];
   ObjectDB *odb;
   int       index;
   LWFrame   frame;
} MyData;


static GlobalFunc *sglobal;


/*
======================================================================
Create()

DisplacementHandler callback.  Allocate and initialize instance data.
====================================================================== */

XCALL_( static LWInstance )
Create( void *priv, LWItemID item, LWError *err )
{
   MyData *dat;
   LWItemInfo *iteminfo;

   if ( dat = calloc( 1, sizeof( MyData ))) {
      dat->objid = item;
      if ( iteminfo = sglobal( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT ))
         strcpy( dat->name, iteminfo->name( item ));
   }

   return dat;
}


/*
======================================================================
Destroy()

Handler callback.  Free the instance.
====================================================================== */

XCALL_( static void )
Destroy( MyData *dat )
{
   if( dat ) {
      if ( dat->odb ) freeObjectDB( dat->odb );
      free( dat );
   }
}


/*
======================================================================
Copy()

Handler callback.  Copy instance data.
====================================================================== */

XCALL_( static LWError )
Copy( MyData *to, MyData *from )
{
   to->objid = from->objid;
   return NULL;
}


/*
======================================================================
Describe()

Handler callback.  Write a short, human-readable string describing
the instance data.
====================================================================== */

XCALL_( static const char * )
Describe( MyData *dat )
{
   return "Object Database Demo";
}


/*
======================================================================
Init()

Handler callback, called at the start of rendering.
====================================================================== */

XCALL_( static LWError )
Init( MyData *dat, int mode )
{
   return NULL;
}


/*
======================================================================
Cleanup()

Handler callback, called at the end of rendering.
====================================================================== */

XCALL_( static void )
Cleanup( MyData *dat )
{
   return;
}


/*
======================================================================
NewTime()

Handler callback, called at the start of each sampling pass.
====================================================================== */

XCALL_( static LWError )
NewTime( MyData *dat, LWFrame fr, LWTime t )
{
   dat->index = 0;
   dat->frame = fr;
   dat->odb = getObjectDB( dat->objid, sglobal );
   return dat->odb ? NULL : "Couldn't allocate object database.";
}


/*
======================================================================
Flags()

Handler callback.
====================================================================== */

XCALL_( static int )
Flags( MyData *dat )
{
   return LWDMF_WORLD;
}


/*
======================================================================
Evaluate()

Handler callback.  Called for each point in the object.
====================================================================== */

XCALL_( static void )
Evaluate( MyData *dat, LWDisplacementAccess *da )
{
   int i;

   /* just in case points are scanned more than once per newTime */

   if ( !dat->odb ) return;
   if ( dat->index >= dat->odb->npoints ) return;

   /* store the world-coordinate position of the point */

   i = findVert( dat->odb, da->point );
   if ( i != -1 ) {
      dat->odb->pt[ i ].pos[ 1 ][ 0 ] = ( float ) da->source[ 0 ];
      dat->odb->pt[ i ].pos[ 1 ][ 1 ] = ( float ) da->source[ 1 ];
      dat->odb->pt[ i ].pos[ 1 ][ 2 ] = ( float ) da->source[ 2 ];
   }

   ++dat->index;

   /* last point?  do something with the object database info */

   if ( dat->index == dat->odb->npoints ) {
      FILE *fp;
      char buf[ 80 ];

      /* calculate world-coordinate normals */

      getPolyNormals( dat->odb, 1 );
      getVertNormals( dat->odb, 1 );

      sprintf( buf, "%s%03d.txt", dat->name, dat->frame );
      if ( fp = fopen( buf, "w" )) {
         printObjectDB( dat->odb, fp, 0 );
         printObjectDB( dat->odb, fp, 1 );
         fclose( fp );
      }

      freeObjectDB( dat->odb );
      dat->odb = NULL;
   }
}


/*
======================================================================
ODBDisplace()

Handler activation function.
====================================================================== */

XCALL_( static int )
ODBDisplace( long version, GlobalFunc *global, LWDisplacementHandler *local,
   void *serverData)
{
   if ( version != LWDISPLACEMENT_VERSION ) return AFUNC_BADVERSION;

   local->inst->create   = Create;
   local->inst->destroy  = Destroy;
   local->inst->copy     = Copy;
   local->inst->descln   = Describe;
   local->rend->init     = Init;
   local->rend->cleanup  = Cleanup;
   local->rend->newTime  = NewTime;
   local->evaluate       = Evaluate;
   local->flags          = Flags;

   sglobal = global;

   return AFUNC_OK;
}


ServerRecord ServerDesc[] = {
   { LWLAYOUTGENERIC_CLASS, "ODBTest", ODBTest },
   { LWDISPLACEMENT_HCLASS, "Demo_Object_Database", ODBDisplace },
   { NULL }
};
