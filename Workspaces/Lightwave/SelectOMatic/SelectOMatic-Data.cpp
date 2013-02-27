//
// SelectOMatic-Data.cpp
//

#include "SelectOMatic-Data.h"

// Constructor
SelectOMatic_Data::SelectOMatic_Data( int _index )
  : active_group( NULL ), active_item( NULL ), enable_update(true),
    root_group( "Root", NULL ), instance_index(_index) {

  // Default group
  GetGroups().Add( new SelectOMatic_Item( "Default", &GetRootGroup() ) );
  SetActiveGroup( GetGroups()[0] );
}

// Destructor
SelectOMatic_Data::~SelectOMatic_Data() {
  root_group.GetChildren().Flush();
  selected.Flush();
}

// File I/O Setup
#define SOMDATAIOPRE_SOMV  LWID_( 'S','O','M','V' )
#define SOMDATAIOPRE_ROOT  LWID_( 'R','O','O','T' )
#define SOMDATAIOPRE_ITEM  LWID_( 'I','T','E','M' )

static enum selectomatic_data_io_ids {
  SOMDATAIOID_SOMV = 0,
  SOMDATAIOID_ROOT = 0,
};

static const LWBlockIdent selectomatic_data_io_somv[] = {
  { SOMDATAIOPRE_SOMV, "SelectOMatic" },
  0
};

static const LWBlockIdent selectomatic_data_io_root[] = {
  { SOMDATAIOPRE_ROOT, "Root" },
  0
};

static const LWBlockIdent selectomatic_data_io_item[] = {
  { SOMDATAIOPRE_ITEM, "Item" },
  0
};


// Load():
char * SelectOMatic_Data::Load( const LWLoadState *loader ) {
  char cvalue;
  int  id;

  GetGroups().Flush();
  active_group = NULL;
  active_item  = NULL;

  // Scan for keywords
  if( SOMDATAIOPRE_SOMV == LWLOAD_FIND( loader, selectomatic_data_io_somv ) ) {
    LWLOAD_I1( loader, &cvalue, 1 );
    if( cvalue > 1 )
      return "Select-O-Matic Error:  Unsupported data version found, aborting load";

    while( id = LWLOAD_FIND( loader, selectomatic_data_io_root ) ) {
      // Base
      switch( id ) {
        // Root Item
        case SOMDATAIOPRE_ROOT:
          if( SOMDATAIOPRE_ITEM == LWLOAD_FIND( loader, selectomatic_data_io_item ) )
            root_group.Load( loader );
          break;
      }

      LWLOAD_END( loader ); // End Keyword
    }
  }

  // Finish Up
  if( GetGroups().NumElements() == 0 )
    GetGroups().Add( new SelectOMatic_Item( "Default", &GetRootGroup() ) );

  active_group = GetGroups()[0];

  return NULL;
}

// Save():
void SelectOMatic_Data::Save( const LWSaveState *saver ) {
  char cvalue;

  // Instance Base
  LWSAVE_BEGIN( saver, &selectomatic_data_io_somv[ SOMDATAIOID_SOMV ], 0 );
  cvalue = 1;
  LWSAVE_I1( saver, &cvalue, 1 );

  // Root
  LWSAVE_BEGIN( saver, &selectomatic_data_io_root[ SOMDATAIOID_ROOT ], 0 );
  root_group.Save( saver );
  LWSAVE_END( saver );

  LWSAVE_END( saver );  // End Instance
}
