//
// FrameJumper-Master.cpp
//

#include <stdio.h>

#include "framejumper.h"
#include "framejumper-data.h"

#include "FrameJumper-Interface.h"

extern FrameJumper_Data        fj_data;
extern FrameJumper_Interface * fj_int;

extern const char *framejumper_server_name;

extern "C" {
  LWInstance      FrameJumper_Create(   void *data, void *context, LWError *error );
  void            FrameJumper_Destroy(  LWInstance _inst );
  LWError         FrameJumper_Copy(     LWInstance _to, LWInstance _from );
  LWError         FrameJumper_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError         FrameJumper_Save(     LWInstance _inst, const LWSaveState *saver );
  const char    * FrameJumper_Describe( LWInstance _inst );

  double          FrameJumper_Event(    LWInstance _inst, const LWMasterAccess *access );
  unsigned int    FrameJumper_Flags(    LWInstance _inst );

  LWError         FrameJumper_OpenUI( void *data );
}


// FrameJumper_Activate
XCALL_ (int) FrameJumperMaster_Activate( long version, GlobalFunc *global,
                                         void *_local, void *serverData ) {

  if(version != LWMASTER_VERSION)
    return(AFUNC_BADVERSION);

  LWMasterHandler *local = (LWMasterHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = FrameJumper_Create;
    local->inst->destroy    = FrameJumper_Destroy;
    local->inst->load       = FrameJumper_Load;
    local->inst->save       = FrameJumper_Save;
    local->inst->copy       = FrameJumper_Copy;
    local->inst->descln     = FrameJumper_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = NULL;
    local->item->changeID   = NULL;
  }

  local->type  = LWMAST_SCENE;
  local->event = FrameJumper_Event;
  local->flags = FrameJumper_Flags;

  fj_data.global = global;

  return AFUNC_OK;
}

// FrameJumper_Create():
//  All instances share one FrameJumper_Data instance, referenced as a
//   global variable.
LWInstance FrameJumper_Create( void *data, void *context, LWError *error ) {
  fj_data.OneMoreUser();
  if( fj_data.GetGroups().NumElements() == 0 ) {
    fj_data.GetGroups().Add( new FrameJumper_Group );
    fj_data.GetGroups()[ 0 ]->SetName( "Default" );
    fj_data.SetCurrentGroup( 0 );
  }

  return (void *)fj_data.GetUsers();
}

// FrameJumper_Destroy():
//  All instances share one FrameJumper_Data instance, referenced as a
//   global variable.  If no instances are still using this data, the
//   marker list is flushed.
void FrameJumper_Destroy( LWInstance _inst ) {
  fj_data.OneLessUser();

  if( fj_data.GetUsers() == 0 ) {
    if( fj_int != NULL ) {
      if( fj_int->IsOpen() )
        (*fj_int->panel_funcs->close)( fj_int->panel );

      delete fj_int;
    }
  }
}

// FrameJumper_Copy():
//  Since all instances use the same data, this doesn't actually do anything.
LWError FrameJumper_Copy( LWInstance _to, LWInstance _from ) {
  return NULL;
}

// FrameJumper_Load():
LWError FrameJumper_Load( LWInstance _inst, const LWLoadState *loader ) {
  assert( loader->ioMode == LWIO_ASCII );

  char buffer[2048];

  // Test for duplicate instances in scene
  if( fj_data.GetFileIOOccured() )
    return NULL;

  fj_data.SetFileIOOccured( true );

  // Check the Header
  for( int i=0; i < 2; i++ ) {
    if( (*loader->read)( loader->readData, buffer, 2048 ) > 0 )
      break;
  }

  StringTools::RemoveHeadWhiteSpace( buffer );

  if( strnicmp( buffer, "FrameJumper", 11 ) != 0 )
    return "FrameJumper Error:  Unrecognized FrameJumper data found; aborting load.";

  char *word = StringTools::FindWord( buffer, 1 );
  if( word == NULL )
    return "FrameJumper Load Error:  Version Number Missing; aborting load.";

  word = StringTools::FindWord( buffer, 1 );
  if( word == NULL )
    return "FrameJumper Load Error:  Version Number Missing; aborting load.";

  if( atoi( word ) != 1 )
    return "FrameJumper Load Error:  Unsupported Version; aborting load.";

  // Read in the rest of the data
  FrameJumper_Group  *group  = NULL;
  FrameJumper_Marker *marker = NULL;
  enum fj_in_what { FJ_DONE = 0, FJ_ROOT, FJ_GROUP, FJ_MARKER };
  int depth = FJ_ROOT;

  bool update_current = false;

  do { 
    (*loader->read)( loader->readData, buffer, 2048 );
    StringTools::RemoveHeadWhiteSpace( buffer );

    if( buffer[0] == '}' ) {
      depth--;

    } else if( depth == FJ_ROOT ) {                            // Root-Level Data
      if( strnicmp( buffer, "Group", 5 ) == 0 ) {              // - Group
        depth = FJ_GROUP;
        if( (fj_data.GetGroups().NumElements() == 1) &&
            (fj_data.GetGroups()[0]->GetMarkers().NumElements() == 0) ) {
            // Only one empty group exists; use it instead of creating a new one.
          group = fj_data.GetGroups()[0];
          update_current = true;
        } else {
          group = new FrameJumper_Group;
          fj_data.GetGroups().Add( group );
        }
      } else if( strnicmp( buffer, "CurrentGroup", 5 ) == 0 ) {// - Current Group
        if( update_current ) {
          word = StringTools::FindWord( buffer, 1 );
          assert( word != NULL );
          if( word != NULL ) {
            fj_data.SetCurrentGroup( atoi( word ) );
            if( fj_data.GetCurrentGroup() > fj_data.GetGroups().NumElements() )
              fj_data.SetCurrentGroup( 0 );
          }
        }
      }

    } else if( depth == FJ_GROUP ) {                           // Group-Level Data
      if( strnicmp( buffer, "Name", 4 ) == 0 ) {               // - Name
        word = StringTools::FindWord( buffer, 1 );
        group->SetName( word );

      } else if( strnicmp( buffer, "Marker", 4 ) == 0 ) {      // - Marker
        depth = FJ_MARKER;
        marker = new FrameJumper_Marker;
        group->GetMarkers().Add( marker );
      }

    } else if( depth == FJ_MARKER ) {                          // Marker-Level Data
      if( strnicmp( buffer, "Name", 4 ) == 0 ) {               // - Name
        word = StringTools::FindWord( buffer, 1 );
        marker->SetName( word );
      } else if( strnicmp( buffer, "Time", 4 ) == 0 ) {        // - Time
        word = StringTools::FindWord( buffer, 1 );
        assert( word != NULL );
        if( word != NULL )
          marker->SetTime( atof( word ) );
      } else if( strnicmp( buffer, "Enabled", 7 ) == 0 ) {     // - Enabled
        marker->SetEnabled( true );
      } else if( strnicmp( buffer, "Key", 3 ) == 0 ) {         // - Key
        word = StringTools::FindWord( buffer, 1 );
        assert( word != NULL );
        if( word != NULL )
          marker->SetKey( atoi( word ) );
      }
    }
  } while( depth > 0 );

  // Update the UI
  if( fj_int != NULL ) {
    SET_INT( fj_int->group_popup, fj_data.GetCurrentGroup() );
    HandleGroupPopup( fj_int->group_popup, NULL );
  }

  return NULL;
}

// FrameJumper_Save();
LWError FrameJumper_Save( LWInstance _inst, const LWSaveState *saver ) {
  assert( saver->ioMode == LWIO_ASCII );

  // Handle duplicate instances of FrameJumper
  if( fj_data.GetFileIOOccured() )
    return NULL;

  fj_data.SetFileIOOccured( true );

  char buffer[ 512 ] = "FrameJumper 1 {";
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  unsigned long i, j;
  for( i=0; i < fj_data.GetGroups().NumElements(); i++ ) {
    (*saver->write)( saver->writeData, "  Group {", 9 );
    
    // Group Data
    if( fj_data.GetGroups()[ i ]->GetName() != NULL ) {
      sprintf( buffer, "    Name %s", fj_data.GetGroups()[ i ]->GetName() );
      (*saver->write)( saver->writeData, buffer, strlen( buffer ) );
    }

    // Markers In Group
    for( j=0; j < fj_data.GetGroups()[i]->GetMarkers().NumElements(); j++ ) {
      (*saver->write)( saver->writeData, "    Marker {", 10 );

      if( fj_data.GetGroups()[i]->GetMarkers()[j]->GetName() != NULL ) {
        sprintf( buffer, "      Name %s", fj_data.GetGroups()[i]->GetMarkers()[j]->GetName() );
        (*saver->write)( saver->writeData, buffer, strlen( buffer ) );
      }

      if( fj_data.GetGroups()[i]->GetMarkers()[j]->GetKey() != 0 ) {
        sprintf( buffer, "      Key %d", fj_data.GetGroups()[i]->GetMarkers()[j]->GetKey() );
        (*saver->write)( saver->writeData, buffer, strlen( buffer ) );
      }

      sprintf( buffer, "      Time %g", fj_data.GetGroups()[i]->GetMarkers()[j]->GetTime() );
      (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

      if( fj_data.GetGroups()[i]->GetMarkers()[j]->GetEnabled() )
        (*saver->write)( saver->writeData, "      Enabled", 11 );

      (*saver->write)( saver->writeData, "    }", 5 );
    }

    (*saver->write)( saver->writeData, "  }", 3 );
  }

  // Interface

/*
  assert( fj_int != NULL );

  sprintf( buffer, "  PanelPosition %d %d", fj_int->panel_x, fj_int->panel_y );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  ListHeight %d", fj_int->list_height );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  sprintf( buffer, "  TimeUnits %d", fj_int->units_state );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );
*/

  sprintf( buffer, "  CurrentGroup %d", fj_data.GetCurrentGroup() );
  (*saver->write)( saver->writeData, buffer, strlen( buffer ) );

  (*saver->write)( saver->writeData, "}", 1 );

  return NULL;
}

// FrameJumper_Describe():
const char * FrameJumper_Describe( LWInstance _inst ) {
  static char desc[ 512 ];

  FrameJumper_Group *group = fj_data.GetGroups()[ fj_data.GetCurrentGroup() ];

  sprintf( desc, "FrameJumper (Group \"%s\" with %d Enabled Markers)",
           ((group->GetName() == NULL) ? "(unnamed)" : group->GetName()),
           group->GetMarkers().NumElements() );
  return desc;
}

// FrameJumper_Event():
double FrameJumper_Event( LWInstance _inst, const LWMasterAccess *access ) {
  fj_data.SetFileIOOccured( false );

  if( (int)_inst != 1 )
    return 0.0;

  if( access->eventCode == LWEVNT_COMMAND ) {
    if( fj_int != NULL ) {
      if( fj_int->IsOpen() ) {
        if( strncmp( (const char *)access->eventData, "FractionalFrames", 16 ) == 0 ) {
          fj_int->ReaquireVariableGlobals();
          fj_int->panel_funcs->draw( fj_int->panel, DR_REFRESH );
        }
      }
    }
  }

  if( fj_data.GetCommand() != NULL ) {
    (*access->evaluate)( access->data, fj_data.GetCommand() );
    fj_data.SetCommand( NULL );
  }

  if( (fj_data.GetUsers() > 1) && ((int)_inst == 1) ) {
    // Check for extra instances of FrameJumper and remove them
    LWItemInfo * iteminfo = (LWItemInfo *)fj_data.global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );

    const char *server_name;
    bool first_found = false;
    char buffer[ 256 ];

    // Find all duplicate instances of FrameJumper
    server_name = (const char *)-1;
    for( unsigned long i=1; server_name != NULL; i++ ) {
      server_name = (*iteminfo->server)( NULL, LWMASTER_HCLASS, i );
      if( server_name == NULL )
        break;

      if( stricmp( server_name, framejumper_server_name ) == 0 ) {
        if( first_found == false ) {
          first_found = true;
        } else {
          sprintf( buffer, "RemoveServer %s %i", LWMASTER_HCLASS, i );
          (*access->evaluate)( access->data, buffer );
          i--;
        }
      }
    }
  }

  return 0.0;
}

// FrameJumper_Flags():
unsigned int FrameJumper_Flags( LWInstance _inst ) {
  return 0;
}

// FrameJumperInterface_Activate():
XCALL_ (int) FrameJumperInterface_Activate( long version, GlobalFunc *global,
                                            void *_local, void *serverData ) {

  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  local->panel   = NULL;
  local->options = FrameJumper_OpenUI;
  local->command = NULL;

  if( FrameJumper_Interface::global == NULL )
    FrameJumper_Interface::global = global;

  return AFUNC_OK;
}

// FrameJumperInterfaceGeneric_Activate():
XCALL_ (int) FrameJumperInterfaceGeneric_Activate( long version, GlobalFunc *global,
                                                   void *local, void *serverData ) {

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Apply FrameJumper if it isn't already there
  if( fj_data.GetUsers() == 0 ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWMASTER_HCLASS, framejumper_server_name );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Open the UI
  if( FrameJumper_Interface::global == NULL )
    FrameJumper_Interface::global = global;

  FrameJumper_OpenUI( NULL );

  return AFUNC_OK;
}

// FrameJumper_OpenUI():
//  Common function to open the FrameJumper UI
LWError FrameJumper_OpenUI( void *data ) {
  // Open the UI if it isn't already open
  if( fj_int == NULL )
    new FrameJumper_Interface();

  if( !fj_int->IsOpen() )
    fj_int->OpenUI();

  return NULL;
}

