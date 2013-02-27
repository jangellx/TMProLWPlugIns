//
// ReplaceObjects.cpp
//

#include "ReplaceObjects.h"
#include "ReplaceObjects-Interface.h"

#include <stdio.h>

#ifdef _DEBUG
  #define DEBUG_PLUGIN_USERNAME " (Debug)"
  #define DEBUG_PLUGIN_PLUGNAME "--Debug"
#else
  #define DEBUG_PLUGIN_USERNAME
  #define DEBUG_PLUGIN_PLUGNAME
#endif

extern "C" {
  ServerUserName ReplaceObjects_Usernames[]            = { {"TM-P  Replace Objects"DEBUG_PLUGIN_USERNAME,              LANGID_USENGLISH | SRVTAG_USERNAME },  { "Replace Objects"DEBUG_PLUGIN_USERNAME,              LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName ReplaceWithOriginalObject_Usernames[] = { {"TM-P  Replace With Original Object"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME },  { "Replace With Original Object"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerRecord ServerDesc[] = {
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceObjects"DEBUG_PLUGIN_PLUGNAME,            ReplaceObjects_Activate,            ReplaceObjects_Usernames            },
    { LWLAYOUTGENERIC_CLASS, "TM-P_ReplaceWithOriginalObject"DEBUG_PLUGIN_PLUGNAME, ReplaceWithOriginalObject_Activate, ReplaceWithOriginalObject_Usernames },
    { (const char *)NULL }
  };
}

// Startup
void *Startup( void ) {
  return NULL;
};

// Shutdown
void Shutdown( void *serverData ) {
  ReplaceObjects_Interface::old_swaps.Flush();
}

// ReplaceObjects_Activate():
XCALL_ (int) ReplaceObjects_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Initialize the interface class
  ReplaceObjects_Interface rui( global, local );

  if( rui.ui->selItems[0] == NULL ) {
    (*rui.message->error)( "Replace Objects Error:  At least one object must be selected.", NULL );
    return AFUNC_OK;
  }

  int sel_type = (rui.iteminfo->type)( rui.ui->selItems[0] );
  if( sel_type != LWI_OBJECT ) {
    (*rui.message->error)( "Replace Objects Error:  You must select at least one object.", NULL );
    return AFUNC_OK;
  }

  // Open the UI
  rui.OpenUI();
  if( !rui.do_process )
    return AFUNC_OK;

  rui.Process();
  return AFUNC_OK;
}

// ReplaceWithOriginalObject_Activate():
XCALL_ (int) ReplaceWithOriginalObject_Activate( long version, GlobalFunc *global,
                                                 void *local, void *serverData ) {
  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  ReplaceObjects_Interface rui( global, local );

  if( rui.ui->selItems[0] == NULL ) {
    (*rui.message->error)( "Replace With Original Object Error:  At least one object must be selected.", NULL );
    return AFUNC_OK;
  }

  int sel_type = (rui.iteminfo->type)( rui.ui->selItems[0] );
  if( sel_type != LWI_OBJECT ) {
    (*rui.message->error)( "Replace With Original Object Error:  You must select at least one object.", NULL );
    return AFUNC_OK;
  }

  // Open the progress bar
  for( int count=0; rui.ui->selItems[ count ] != NULL; count++ ) { ; }
  rui.progress_total = count;
  rui.progress_current = 1;
  rui.OpenProgressPanel();

  // Cycle through the selected items
  char buffer[ 1024 ];
  for( int i=0; rui.ui->selItems[i] != NULL; i++ ) {
    // Update the progress bar
    if( rui.progress_open ) {
      sprintf( rui.progress_string, "Replacing \"%s\"...", (*rui.iteminfo->name)( rui.ui->selItems[i] ) );
      rui.progress_current++;
      REDRAW_CON( rui.progress_field );

      // Update the panel and see if the user closed it (ie: cancel operation)
      if( (*rui.panel_funcs->handle)( rui.progress_panel, 0 ) == -1) {
        if( (*rui.message->yesNo)( "Replace With Original Object -- Cancel Processing?",
                                   "Do you really want to abort processing?",
                                   "Note that some objects may have already been replaced" ) == 1 ) {
          (*rui.message->info)( "Replace With Original Object Info:  Processing aborted by user.", NULL );
          rui.progress_open = false;
          return false;
        } else {
          (*rui.panel_funcs->open)( rui.progress_panel, PANF_PASSALLKEYS );
        }
      }
    }

    const char *tag;
    for( int t=1; tag = (*rui.iteminfo->getTag)( rui.ui->selItems[i], t ); t++ ) {
      if( tag == NULL )
        break;

      if( strnicmp( tag, "OriginalItem", 12 ) == 0 )
        break;
    } 

    // No match; try the next one
    if( tag == NULL )
      continue;

    // Get info about the object so we can update the tag
    int orig_type = 1;
    if( ((*rui.objectinfo->numPoints)(   rui.ui->selItems[i] ) <= 1) &&
        ((*rui.objectinfo->numPolygons)( rui.ui->selItems[i] ) == 0) ) {
      orig_type = 0;
    }

    char *orig_name = StringTools::strdup( (*rui.objectinfo->filename)( rui.ui->selItems[i] ) );

    // Replace the object
    sprintf( buffer, "SelectItem %x", rui.ui->selItems[i] );
    (*rui.generic->evaluate)( rui.generic->data, buffer );

    if( atoi( &(tag[13]) ) == 0 )
      sprintf( buffer, "ReplaceWithNull %s", &(tag[15]) );
    else
      sprintf( buffer, "ReplaceWithObject %s", &(tag[15]) );

    (*rui.generic->evaluate)( rui.generic->data, buffer );

    // Update the tag
    sprintf( buffer, "OriginalItem %d %s", orig_type, orig_name );
    (*rui.iteminfo->setTag)( rui.ui->selItems[i], t, buffer );

    delete orig_name;
  }

  // Reselect the original items
  rui.ReselectItems();

  // Close the Progress panel
  if( rui.progress_open )
    (*rui.panel_funcs->close)( rui.progress_panel );
  
  return AFUNC_OK;
}

