//
// SelectOMatic-Master.cpp
//

#include <string.h>
#include <stdio.h>

#include "SelectOMatic-Interface.h"

SelectOMatic_Data      *som_data = NULL;
SelectOMatic_Interface *som_int  = NULL;

GlobalFunc *SelectOMatic_Data::global = NULL;

int  master_instance_count = 0;
bool file_io_occured       = false;

extern "C" {
  // Master Functions
  LWInstance       SOM_Master_Create(   void *data, void *context, LWError *error );
  void             SOM_Master_Destroy(  LWInstance _inst );
  LWError          SOM_Master_Copy(     LWInstance _to, LWInstance _from );
  LWError          SOM_Master_Load(     LWInstance _inst, const LWLoadState *loader );
  LWError          SOM_Master_Save(     LWInstance _inst, const LWSaveState *saver );
  const char     * SOM_Master_Describe( LWInstance _inst );

  double           SOM_Master_Event(    LWInstance _inst, const LWMasterAccess *access );
  unsigned int     SOM_Master_Flags(    LWInstance _inst );

  const LWItemID * SOM_Master_UseItems( LWInstance _inst );
  void             SOM_Master_ChangeID( LWInstance _inst, const LWItemID *ids );

  LWError          SOM_Master_OpenUI( void *data );
}


// SOM_Master_Activate():
XCALL_ (int) SOM_Master_Activate( long version, GlobalFunc *global,
                                  void *_local, void *serverData ) {
  if ( version != LWMASTER_VERSION )
    return AFUNC_BADVERSION;

  LWMasterHandler *local = (LWMasterHandler *)_local;

  if( local->inst != NULL ) {
    local->inst->create     = SOM_Master_Create;
    local->inst->destroy    = SOM_Master_Destroy;
    local->inst->load       = SOM_Master_Load;
    local->inst->save       = SOM_Master_Save;
    local->inst->copy       = SOM_Master_Copy;
    local->inst->descln     = SOM_Master_Describe;
  }

  if( local->item != NULL ) {
    local->item->useItems   = SOM_Master_UseItems;
    local->item->changeID   = SOM_Master_ChangeID;
  }

  local->type  = LWMAST_SCENE;
  local->event = SOM_Master_Event;
  local->flags = SOM_Master_Flags;

  SelectOMatic_Data::global = global;

  return AFUNC_OK;
}

// SOM_Master_Create():
XCALL_ (LWInstance) SOM_Master_Create( void *data, void *context, LWError *error ) {
  SelectOMatic_Data *new_data = new SelectOMatic_Data( master_instance_count++ );
  if( som_data == NULL )
    som_data = new_data;

  return new_data;
}

// SOM_Master_Destroy():
XCALL_ (void)  SOM_Master_Destroy( LWInstance inst ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)inst;
  master_instance_count--;

  if( master_instance_count == 0 ) {
    if( som_int != NULL ) {
      if( som_int->IsOpen() )
        (*som_int->panel_funcs->close)( som_int->panel );

      delete som_int;
      som_int  = NULL;
      som_data = NULL;
    }
  }

  delete data;
}

// SOM_Master_Copy():
XCALL_ (LWError) SOM_Master_Copy( LWInstance to, LWInstance from ) {
  return NULL;
}

// SOM_Master_Load():
XCALL_ (LWError) SOM_Master_Load( LWInstance inst, const LWLoadState *state ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)inst;
  return data->Load( state );
}

// SOM_Master_Save():
XCALL_ (LWError) SOM_Master_Save( LWInstance inst, const LWSaveState *state ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)inst;
  data->Save( state );
  return NULL;
}

// SOM_Master_Describe():
XCALL_ (const char *) SOM_Master_Describe( LWInstance inst ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)inst;

  static char desc[ 512 ];
  sprintf( desc, "Select-O-Matic (%d root groups)", data->GetGroups().NumElements() );
  return desc;
}

const char * test_commands[] = {
  "SelectItem",
  "AddToSelection",
  "RemoveFromSelection",
  "EditObjects",
  "EditBones",
  "EditLights",
  "EditCameras",
  "PreviousItem",
  "NextItem",
  "FirstItem",
  "LastItem",
  "SelectParent",
  "SelectChild",
  "PreviousSibling",
  "NextSibling",
  "AddNull",
  "AddBone",
  "AddChildBone",
  "AddDistantLight",
  "AddPointLight",
  "AddSpotlight",
  "AddLinearLight",
  "AddAreaLight",
  "AddCamera",
  "SelectAllObjects",
  "SelectAllBones",
  "SelectAllLights",
  "SelectAllCameras",
  "SelectByName",
  "ClearSelected",
  "Clone",
  "ClearAllObjects",
  "ClearAllBones",
  "ClearAllLights",
  "ClearAllCameras",
  "LoadObject",
  "LoadObjectLayer",

  "Rename",
  "ItemColor",
  NULL };

// SOM_Master_Event():
double SOM_Master_Event( LWInstance _inst, const LWMasterAccess *access ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)_inst;
  bool destroy_others = true;

  // Check for any commands we might care about
  if( access->eventCode == LWEVNT_COMMAND ) {
    if( strncmp( (const char *)access->eventData, "LoadFromScene", strlen( "LoadFromScene" ) ) == 0 ) {
      destroy_others = false;

      if( data->GetInstanceIndex() > 0 ) {
        // Load From Scene:  Merge the new instance into the existing instance

        // Add groups only if there is something besides Default there
        bool add_groups = true;
        if( data->GetGroups()[0]->GetName() != NULL ) {
          if( strcmp( data->GetGroups()[0]->GetName(), "Default" ) == 0 ) {
            if( data->GetGroups()[0]->GetChildren().NumElements() == 0 )
              add_groups = false;
          }
        }

        if( add_groups ) {
          // Clear out the default group in the main data if it's the only one
          if( som_data->GetGroups().NumElements() == 1 ) {
            if( som_data->GetGroups()[0]->GetName() != NULL ) {
              if( strcmp( som_data->GetGroups()[0]->GetName(), "Default" ) == 0 ) {
                if( som_data->GetGroups()[0]->GetChildren().NumElements() == 0 ) {
                  som_data->GetGroups().Flush();
                  som_data->SetActiveGroup( NULL );
                }
              }
            }
          }

          // Add the second instance's groups to the first instance
          for( unsigned long i=0; i < data->GetGroups().NumElements(); i++ )
            som_data->GetGroups().Add( data->GetGroups()[i] );
        }

        // Reset the data's group list
        data->GetGroups().Reset();

        // Destroy self
        LWItemInfo * iteminfo = (LWItemInfo *)data->global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );

        bool first_found = false;
        const char *server_name = (const char *)-1;
        for( unsigned long i=1; server_name != NULL; i++ ) {
          server_name = (*iteminfo->server)( NULL, LWMASTER_HCLASS, i );
          if( server_name == NULL )
            break;

          if( stricmp( server_name, selectomatic_server_name ) == 0 ) {
            if( first_found == false ) {
              first_found = true;
            } else {
              char buffer[ 256 ];
              sprintf( buffer, "RemoveServer %s %i", LWMASTER_HCLASS, i );
              (*access->evaluate)( access->data, buffer );
              break;
            }
          }
        }

        if( som_data->GetActiveGroup() == NULL ) {
          if( som_data->GetGroups().NumElements() > 0 )
            som_data->SetActiveGroup( som_data->GetGroups()[0] );
        }

        som_data->SetActiveItem( NULL );

        // Update the UI
        if( som_int != NULL ) {
          if( som_int->IsOpen() ) {
            HandleFetch( som_int->fetch, NULL );
          }
        }
      }
    } else {
      // Command Updates
      if( data == som_data ) {             // Only the main instance should do this
        if( data->GetEnableUpdate() ) {
          if( som_int != NULL ) {
            if( som_int->IsOpen() ) {
              for( int i=0; test_commands[i] != NULL; i++ ) {
                if( strncmp( (const char *)access->eventData, test_commands[i], strlen( test_commands[i] ) ) == 0 ) {
                  HandleFetch( som_int->fetch, NULL );
                  break;
                }
              }
            }
          }
        }
      }
    }
  }

  // Test for duplicate instances
  if( (master_instance_count > 1) && destroy_others ) {
    file_io_occured = false;

    // Check for extra instances of SelectOMatic and remove them
    LWItemInfo * iteminfo = (LWItemInfo *)data->global( LWITEMINFO_GLOBAL, GFUSE_TRANSIENT );

    bool first_found = false;
    char buffer[ 256 ];

    // Find all duplicate instances of SelectOMatic
    const char *server_name = (const char *)-1;
    for( unsigned long i=1; server_name != NULL; i++ ) {
      server_name = (*iteminfo->server)( NULL, LWMASTER_HCLASS, i );
      if( server_name == NULL )
        break;

      if( stricmp( server_name, selectomatic_server_name ) == 0 ) {
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

// SOM_Master_Flags():
unsigned int SOM_Master_Flags( LWInstance _inst ) {
  return 0;
}

// SOM_Master_UseItems():
const LWItemID * SOM_Master_UseItems( LWInstance _inst ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)_inst;
  return (LWItemID *)LWITEM_ALL;
}

// SOM_Master_ChangeID():
void SOM_Master_ChangeID( LWInstance _inst, const LWItemID *ids ) {
  SelectOMatic_Data *data = (SelectOMatic_Data *)_inst;

  DynArray< SelectOMatic_Item * > items;
  data->GetRootGroup().FindAllItems( items );

  unsigned long i, j;
  for( i=0; ids[i] != NULL; i+=2 ) {
    for( j=0; j < items.NumElements(); j++ ) {
      if( items[j] == NULL )
        continue;

      if( items[j]->GetID() == ids[i] ) {
        if( ids[i+1] == NULL ) {
          unsigned long index = items[j]->GetParent()->GetChildren().FindIndexOf( items[j] );
          items[j]->GetParent()->GetChildren().Remove( index );
          if( items[j] == data->GetActiveItem() )
            data->SetActiveItem( NULL );
          delete items[j];
        } else {
          items[j]->SetID( ids[i+1] );
        }

        items[j] = NULL;
        // Don't break; one item can be in the list multiple times
      }
    }
  }
  
  if( som_int != NULL )
    HandleFetch( som_int->fetch, NULL );
}

// SOM_Interface_Activate():
XCALL_ (int) SOM_Interface_Activate( long version, GlobalFunc *global,
                                     void *_local, void *serverData ) {

  if ( version != LWINTERFACE_VERSION )
     return AFUNC_BADVERSION;

  LWInterface *local = (LWInterface *)_local;

  local->panel   = NULL;
  local->options = SOM_Master_OpenUI;
  local->command = NULL;

  if( SelectOMatic_Interface::global == NULL )
    SelectOMatic_Interface::global = global;

  return AFUNC_OK;
}

// SOM_InterfaceGeneric_Activate():
XCALL_ (int) SOM_InterfaceGeneric_Activate( long version, GlobalFunc *global,
                                            void *local, void *serverData ) {

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  // Apply SelectOMatic if it isn't already there
  if( master_instance_count == 0 ) {
    LWLayoutGeneric *generic = (LWLayoutGeneric *)local;
    char buffer[ 256 ];
    sprintf( buffer, "ApplyServer %s %s", LWMASTER_HCLASS, selectomatic_server_name );
    (*generic->evaluate)( generic->data, buffer );
  }

  // Open the UI
  if( SelectOMatic_Interface::global == NULL )
    SelectOMatic_Interface::global = global;

  SOM_Master_OpenUI( NULL );

  return AFUNC_OK;
}

// SOM_Master_OpenUI():
//  Common function to open the SelectOMatic UI
LWError SOM_Master_OpenUI( void *data ) {
  // Open the UI if it isn't already open
  if( som_int == NULL )
    new SelectOMatic_Interface();

  if( !som_int->IsOpen() )
    som_int->OpenUI();

  return NULL;
}

