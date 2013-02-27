//
// SelectOMatic.cpp
//
// SelectOMatic itself consists of 3 plug-ins:
//  TM-P_SelectOMatic:           Adds the Master Handler and opens the interface
//  TM-P_SelectOMatic_Master:    Handles event input and can open the interface
//  TM-P_SelectGroup:            Global for creating and maintaining selection groups
//

#include "SelectOMatic.h"
#include "SelectOMatic-Interface.h"

#include <stdio.h>
#include "assert.h"

const char * selectomatic_server_name = "TM-P_SelectOMatic"DEBUG_PLUGIN_PLUGNAME;
const char * TMPSELECTGROUP_GLOBAL    = "TM-P_SelectOMaticGlobal";

extern SelectOMatic_Interface *som_int;

extern "C" {
  // User Names
  ServerUserName SOM_Usernames_Master[]            = { {"TM-P  Select-O-Matic Master"DEBUG_PLUGIN_PLUGNAME,     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select-O-Matic"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },        { (const char *)NULL } };
  ServerUserName SOM_Usernames_Global[]            = { {"TM-P  Select-O-Matic Global"DEBUG_PLUGIN_PLUGNAME,     LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select-O-Matic Global"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SOM_InterfaceGeneric_Usernames[]  = { {"TM-P  Select-O-Matic Properties"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select-O-Matic Panel"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },  { (const char *)NULL } };

  ServerUserName SelectGoal_Usernames[]            = { {"TM-P  Select IK Goal"DEBUG_PLUGIN_PLUGNAME,            LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select IK Goal"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },        { (const char *)NULL } };
  ServerUserName SelectTarget_Usernames[]          = { {"TM-P  Select Target"DEBUG_PLUGIN_PLUGNAME,             LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Target"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },         { (const char *)NULL } };

  ServerUserName SelectAllRoots_Usernames[]        = { {"TM-P  Select All Roots"DEBUG_PLUGIN_PLUGNAME,          LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select All Roots"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },      { (const char *)NULL } };
  ServerUserName SelectRoot_Usernames[]            = { {"TM-P  Select Root"DEBUG_PLUGIN_PLUGNAME,               LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Root"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },           { (const char *)NULL } };

  ServerUserName SelectAllNull_Usernames[]         = { {"TM-P  Select All Nulls"DEBUG_PLUGIN_PLUGNAME,          LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select All Nulls"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },      { (const char *)NULL } };

  ServerUserName SelectClones_Usernames[]          = { {"TM-P  Select Clones"DEBUG_PLUGIN_PLUGNAME,             LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Clones"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },         { (const char *)NULL } };

  ServerUserName SelectRootGroup_Usernames[]       = { {"TM-P  Select Root Group"DEBUG_PLUGIN_PLUGNAME,         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Root Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },     { (const char *)NULL } };

  ServerUserName SelectFirstGroup_Usernames[]      = { {"TM-P  Select First Group"DEBUG_PLUGIN_PLUGNAME,        LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select First Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },    { (const char *)NULL } };
  ServerUserName SelectLastGroup_Usernames[]       = { {"TM-P  Select Last Group"DEBUG_PLUGIN_PLUGNAME,         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Last Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },     { (const char *)NULL } };

  ServerUserName SelectNextGroup_Usernames[]       = { {"TM-P  Select Next Group"DEBUG_PLUGIN_PLUGNAME,         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Next Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },     { (const char *)NULL } };
  ServerUserName SelectPrevGroup_Usernames[]       = { {"TM-P  Select Prev Group"DEBUG_PLUGIN_PLUGNAME,         LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Prev Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },     { (const char *)NULL } };

  ServerUserName SelectNextInGroup_Usernames[]     = { {"TM-P  Select Next In Group"DEBUG_PLUGIN_PLUGNAME,      LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Next In Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },  { (const char *)NULL } };
  ServerUserName SelectPrevInGroup_Usernames[]     = { {"TM-P  Select Prev In Group"DEBUG_PLUGIN_PLUGNAME,      LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Prev In Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },  { (const char *)NULL } };

  ServerUserName SelectNextInGroupHierarchy_Usernames[] = { {"TM-P  Select Next In Group Hierarchy"DEBUG_PLUGIN_PLUGNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Next In Group Hierarchy"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  ServerUserName SelectPrevInGroupHierarchy_Usernames[] = { {"TM-P  Select Prev In Group Hierarchy"DEBUG_PLUGIN_PLUGNAME, LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Prev In Group Hierarchy"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName SelectFirstInGroup_Usernames[]    = { {"TM-P  Select First In Group"DEBUG_PLUGIN_PLUGNAME,      LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select First In Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },{ (const char *)NULL } };
  ServerUserName SelectLastInGroup_Usernames[]     = { {"TM-P  Select Last In Group"DEBUG_PLUGIN_PLUGNAME,       LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Last In Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

  ServerUserName SelectParentGroup_Usernames[]     = { {"TM-P  Select Parent Group"DEBUG_PLUGIN_PLUGNAME,        LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Parent Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },      { (const char *)NULL } };
  ServerUserName SelectFirstChildGroup_Usernames[] = { {"TM-P  Select First Child Group"DEBUG_PLUGIN_PLUGNAME,   LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select First Child Group"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };

//  ServerUserName SelectChildren_Usernames[]        = { {"TM-P  Select Children"DEBUG_PLUGIN_PLUGNAME,           LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Children"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },       { (const char *)NULL } };
//  ServerUserName SelectDescendants_Usernames[]     = { {"TM-P  Select Descendants"DEBUG_PLUGIN_PLUGNAME,        LANGID_USENGLISH | SRVTAG_USERNAME }, { "Select Descendants"DEBUG_PLUGIN_USERNAME, LANGID_USENGLISH | SRVTAG_BUTTONNAME },    { (const char *)NULL } };

  // Server Record
  ServerRecord ServerDesc[] = {
    { LWMASTER_HCLASS,       selectomatic_server_name,                                SOM_Master_Activate,           SOM_Usernames_Master           },
    { LWMASTER_ICLASS,       selectomatic_server_name,                                SOM_Interface_Activate,        SOM_Usernames_Master           },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectOMaticProperties"DEBUG_PLUGIN_PLUGNAME,      SOM_InterfaceGeneric_Activate, SOM_InterfaceGeneric_Usernames },
    { LWGLOBALSERVICE_CLASS, TMPSELECTGROUP_GLOBAL,                                   SOM_SelectGroup_Activate,      SOM_Usernames_Global           },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectGoal"DEBUG_PLUGIN_PLUGNAME,                  SelectGoal_Activate,           SelectGoal_Usernames           },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectTarget"DEBUG_PLUGIN_PLUGNAME,                SelectTarget_Activate,         SelectTarget_Usernames         },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectAllRoots"DEBUG_PLUGIN_PLUGNAME,              SelectAllRoots_Activate,       SelectAllRoots_Usernames       },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectRoot"DEBUG_PLUGIN_PLUGNAME,                  SelectRoot_Activate,           SelectRoot_Usernames           },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectAllNulls"DEBUG_PLUGIN_PLUGNAME,              SelectAllNulls_Activate,       SelectAllNull_Usernames        },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectClones"DEBUG_PLUGIN_PLUGNAME,                SelectClones_Activate,         SelectClones_Usernames         },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectRootGroup"DEBUG_PLUGIN_PLUGNAME,             SelectRootGroup_Activate,      SelectRootGroup_Usernames      },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectFirstGroup"DEBUG_PLUGIN_PLUGNAME,            SelectFirstGroup_Activate,     SelectFirstGroup_Usernames     },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectLastGroup"DEBUG_PLUGIN_PLUGNAME,             SelectLastGroup_Activate,      SelectLastGroup_Usernames      },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectNextGroup"DEBUG_PLUGIN_PLUGNAME,             SelectNextGroup_Activate,      SelectNextGroup_Usernames      },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectPrevGroup"DEBUG_PLUGIN_PLUGNAME,             SelectPrevGroup_Activate,      SelectPrevGroup_Usernames      },

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectNextInGroup"DEBUG_PLUGIN_PLUGNAME,           SelectNextInGroup_Activate,    SelectNextInGroup_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectPrevInGroup"DEBUG_PLUGIN_PLUGNAME,           SelectPrevInGroup_Activate,    SelectPrevInGroup_Usernames    },

  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectNextInGroupHierarchy"DEBUG_PLUGIN_PLUGNAME,  SelectNextInGroupHierarchy_Activate, SelectNextInGroupHierarchy_Usernames },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectPrevInGroupHierarchy"DEBUG_PLUGIN_PLUGNAME,  SelectPrevInGroupHierarchy_Activate, SelectPrevInGroupHierarchy_Usernames },
  #endif

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectParentGroup"DEBUG_PLUGIN_PLUGNAME,           SelectParentGroup_Activate,    SelectParentGroup_Usernames    },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectFirstChildGroup"DEBUG_PLUGIN_PLUGNAME,       SelectFirstChildGroup_Activate,SelectFirstChildGroup_Usernames},

    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectFirstInGroup"DEBUG_PLUGIN_PLUGNAME,          SelectFirstInGroup_Activate,   SelectFirstInGroup_Usernames   },
    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectLastInGroup"DEBUG_PLUGIN_PLUGNAME,           SelectLastInGroup_Activate,    SelectLastInGroup_Usernames    },

//    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectChildren"DEBUG_PLUGIN_PLUGNAME,              SelectChildren_Activate,       SelectChildren_Usernames       },
//    { LWLAYOUTGENERIC_CLASS, "TM-P_SelectDescendants"DEBUG_PLUGIN_PLUGNAME,           SelectDescendants_Activate,    SelectDescendants_Usernames    },
    { (const char *)NULL }
  };
}

// SOM_SelectGroup_Activate():
XCALL_ (int) SOM_SelectGroup_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if ( version != LWGLOBALSERVICE_VERSION  )
    return AFUNC_BADVERSION;

  return AFUNC_OK;
}

// SelectGoal_Activate():
XCALL_ (int) SelectGoal_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Select IK Goal Error:  At least one item must be selected", NULL );
    return AFUNC_OK;
  }

  bool selected = false;
  LWItemID goal;
  char buffer[100];

  for( int i=0; ui->selItems[i] != NULL; i++ ) {
    goal = (*iteminfo->goal)( ui->selItems[i] );
    if( goal != LWITEM_NULL ) {
      sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), goal );
      generic->evaluate( generic->data, buffer );
      selected = true;
    }
  }

  if( !selected )
    (*message->error)( "Select IK Goal Error:  None of the selected items have IK goals", NULL );

  return AFUNC_OK;
}

// SelectTarget_Activate():
XCALL_ (int) SelectTarget_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Select Target Error:  At least one item must be selected", NULL );
    return AFUNC_OK;
  }

  bool selected = false;
  LWItemID target;
  char buffer[100];

  for( int i=0; ui->selItems[i] != NULL; i++ ) {
    target = (*iteminfo->target)( ui->selItems[i] );
    if( target != LWITEM_NULL ) {
      sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), target );
      generic->evaluate( generic->data, buffer );
      selected = true;
    }
  }

  if( !selected )
    (*message->error)( "Select Target Error:  None of the selected items are targeted", NULL );

  return AFUNC_OK;
}

// SelectAllRoots_Activate():
XCALL_ (int) SelectAllRoots_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Select All Roots Error:  At least one item must be selected", NULL );
    return AFUNC_OK;
  }

  int sel_type = (*iteminfo->type)( ui->selItems[0] );

  // Handle Bones
  char buffer[ 100 ];
  if( sel_type == LWI_BONE ) {
    for( LWItemID owner = ui->selItems[0]; owner != NULL; owner = (*iteminfo->parent)( owner ) ) {
      if( (*iteminfo->type)( owner ) != LWI_BONE ) {
        break;
      }
    }

    assert( owner != NULL );

    bool selected = false;
    for( LWItemID child = (*iteminfo->firstChild)( owner ); child != NULL; child = (*iteminfo->nextChild)( owner, child ) ) {
      if( (*iteminfo->type)( child ) == LWI_BONE ) {
        sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), child );
        (*generic->evaluate)( generic->data, buffer );
        selected = true;
      }
    }

    return AFUNC_OK;
  }

  // Handle all other items
  bool selected = false;
  for( LWItemID item = (*iteminfo->first)( sel_type, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
    if( (*iteminfo->parent)( item ) == NULL ) {
      sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), item );
      (*generic->evaluate)( generic->data, buffer );
      selected = true;
    }
  }

  return AFUNC_OK;
}

// SelectRoot_Activate():
XCALL_ (int) SelectRoot_Activate( long version, GlobalFunc *global,
                                  void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Select Root Error:  At least one item must be selected", NULL );
    return AFUNC_OK;
  }

  bool selected = false;
  char buffer[100];

  if( (*iteminfo->type)( ui->selItems[0] ) == LWI_BONE ) {
    for( int i=0; ui->selItems[i] != NULL; i++ ) {
      LWItemID parent = NULL;
      for( LWItemID test_parent = ui->selItems[i]; test_parent != NULL; test_parent = (*iteminfo->parent)( test_parent ) ) {
        if( (*iteminfo->type)( test_parent ) != LWI_BONE )
          break;

        parent = test_parent;
      }

      if( parent == NULL )
        parent = ui->selItems[i];

      sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), parent );
      generic->evaluate( generic->data, buffer );
      selected = true;
    }

    return AFUNC_OK;
  } 

  // Handle all other items
  for( int i=0; ui->selItems[i] != NULL; i++ ) {
    LWItemID parent = NULL;
    for( LWItemID test_parent = ui->selItems[i]; test_parent != NULL; test_parent = (*iteminfo->parent)( test_parent ) )
      parent = test_parent;

    if( parent == NULL )
      parent = ui->selItems[i];

    sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), parent );
    generic->evaluate( generic->data, buffer );
    selected = true;
  }
 
  return AFUNC_OK;
}

// SelectAllNulls_Activate():
XCALL_ (int) SelectAllNulls_Activate( long version, GlobalFunc *global,
                                      void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic    = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo   = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWObjectInfo    *objectinfo = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  LWInterfaceInfo *ui         = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message    = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  bool selected = false;
  char buffer[100];

  for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
    if( ((*objectinfo->numPoints)(   item ) <= 1) &&
        ((*objectinfo->numPolygons)( item ) == 0) ) {

      sprintf( buffer, "%s %x", (selected ? "AddToSelection" : "SelectItem"), item );
      generic->evaluate( generic->data, buffer );
      selected = true;
    }
  }

  if( !selected )
    (message->error)( "Select All Nulls Error:  There are no null objects in the scene; nothing to do", NULL );

  return AFUNC_OK;
}

// SelectClones_Activate():
XCALL_ (int) SelectClones_Activate( long version, GlobalFunc *global,
                                    void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  LWLayoutGeneric *generic  = (LWLayoutGeneric *)local;
  LWItemInfo      *iteminfo = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  LWInterfaceInfo *ui       = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWMessageFuncs  *message  = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );

  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Select Target Error:  At least one item must be selected", NULL );
    return AFUNC_OK;
  }

  int type = iteminfo->type( ui->selItems[0] );

  LWItemID owner = NULL;
  if( type == LWI_BONE ) {
    owner = ui->selItems[0];
    while( iteminfo->type( owner = iteminfo->parent( owner ) ) != LWI_OBJECT ) { ; }
  }

  char name1[ 512 ], buffer[ 100 ];
  for( unsigned long i=0; ui->selItems[i] != NULL; i++ ) {
    strcpy( name1, iteminfo->name( ui->selItems[i] ) );
    if( name1[ strlen( name1 ) - 1 ] != ')' )  // Must end in a ) to be a have clones
      continue;

    char *open_paren = strrchr( name1, '(' );
    if( open_paren == NULL )
      continue;

    open_paren[1] = NULL;
    int len = strlen( name1 );

    for( LWItemID id = iteminfo->first( type, owner ); id != NULL; id = iteminfo->next( id ) ) {
      if( strncmp( name1, iteminfo->name( id ), len ) == 0 ) {
        sprintf( buffer, "AddToSelection %x", id );
        generic->evaluate( generic->data, buffer );
      }
    }
  }

  return AFUNC_OK;
}

// UpdateInterfaceFromGenerics():
void UpdateInterfaceFromGenerics( SelectOMatic_Item *group, SelectOMatic_Item *item ) {
  som_data->SetActiveGroup( group );
  if( item == NULL ) {
    for( unsigned long i=0; i < group->GetChildren().NumElements(); i++ ) {
      if( group->GetChildren()[i]->GetID() != SOM_GROUP ) {
        som_data->SetActiveItem( group->GetChildren()[i] );
        break;
      }
    }
  } else {
    if( group != item->GetParent() ) {
      assert( group == item->GetParent() );
      som_data->SetActiveItem( NULL );
    } else {
      som_data->SetActiveItem( item );
    }
  }

  ChangeSelectionByItem( "SelectItem", "AddToSelection", (item == NULL) ? group : item );

  if( som_int != NULL ) {
    if( som_int->IsOpen() )
      som_int->panel_controls->multiTreeRedraw( som_int->list );
  }
}

// SelectRootGroup_Activate():
XCALL_ (int) SelectRootGroup_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {

  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  if( som_data->GetActiveGroup() != NULL ) {
    for( SelectOMatic_Item *parent = som_data->GetActiveGroup(); parent->GetParent() != NULL; parent = parent->GetParent() ) { ; }
    UpdateInterfaceFromGenerics( parent, NULL );
  }

  return AFUNC_OK;
}

// SelectFirstGroup_Activate():
XCALL_ (int) SelectFirstGroup_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {

  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  if( som_data->GetGroups().NumElements() > 0 )
    UpdateInterfaceFromGenerics( som_data->GetGroups()[0], NULL );

  return AFUNC_OK;
}

// SelectLastGroup_Activate():
XCALL_ (int) SelectLastGroup_Activate( long version, GlobalFunc *global,
                                        void *local, void *serverData ) {

  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  if( som_data->GetGroups().NumElements() > 0 )
    UpdateInterfaceFromGenerics( som_data->GetGroups()[ som_data->GetGroups().NumElements() - 1 ], NULL );

  return AFUNC_OK;
}

// SelectNextGroup_Activate():
XCALL_ (int) SelectNextGroup_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {

  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  SelectOMatic_Item *parent = group->GetParent();

  unsigned long index       = -1;
  unsigned long first_index = -1;
  unsigned long i;
  for( i=0; i < parent->GetChildren().NumElements(); i++ ) {
    if( parent->GetChildren()[i]->GetID() == SOM_GROUP ) {
      if( first_index == -1 )
        first_index = i;

      index = i;
      break;
    }
  }

  if( index == -1 )
    index = first_index;

  if( index != -1 )
    UpdateInterfaceFromGenerics( parent->GetChildren()[ index ], NULL );

  return AFUNC_OK;
}

// SelectPrevGroup_Activate():
XCALL_ (int) SelectPrevGroup_Activate( long version, GlobalFunc *global,
                                       void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  SelectOMatic_Item *parent = group->GetParent();

  unsigned long index      = -1;
  unsigned long last_index = -1;
  unsigned long i;
  for( i=parent->GetChildren().NumElements(); i != -1; i-- ) {
    if( parent->GetChildren()[i]->GetID() == SOM_GROUP ) {
      if( last_index == -1 )
        last_index = i;

      index = i;
      break;
    }
  }

  if( index == -1 )
    index = last_index;

  if( index != -1 )
    UpdateInterfaceFromGenerics( parent->GetChildren()[ index ], NULL );

  return AFUNC_OK;
}

// SelectNextInGroup_Activate():
XCALL_ (int) SelectNextInGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  unsigned long index       = -1;
  unsigned long first_index = -1;
  unsigned long i;
  for( i=0; i < group->GetChildren().NumElements(); i++ ) {
    if( group->GetChildren()[i]->GetID() != SOM_GROUP ) {
      if( first_index == -1 )
        first_index = i;

      if( group->GetChildren()[i] == som_data->GetActiveItem() ) {
        index = i;
        break;
      }
    }
  }

  if( index == -1 )
    index = first_index;

  if( index != -1 )
    UpdateInterfaceFromGenerics( group, group->GetChildren()[ index ] );

  return AFUNC_OK;
}

// SelectPrevInGroup_Activate():
XCALL_ (int) SelectPrevInGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  unsigned long index      = -1;
  unsigned long last_index = -1;
  unsigned long i;
  for( i = group->GetChildren().NumElements() - 1; i != -1; i-- ) {
    if( group->GetChildren()[i]->GetID() != SOM_GROUP ) {
      if( last_index == -1 )
        last_index = i;

      if( group->GetChildren()[i] == som_data->GetActiveItem() ) {
        index = i;
        break;
      }
    }
  }

  if( index == -1 )
    index = last_index;

  if( index != -1 )
    UpdateInterfaceFromGenerics( group, group->GetChildren()[ index ] );

  return AFUNC_OK;
}

// SelectNextInGroupHierarchy_Activate():
XCALL_ (int) SelectNextInGroupHierarchy_Activate( long version, GlobalFunc *global,
                                                  void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  SelectOMatic_Item *item  = som_data->GetActiveItem();

  if( group == NULL ) {
    if( som_data->GetGroups().NumElements() == 0 )
      return AFUNC_OK;

    som_data->SetActiveGroup( som_data->GetGroups()[0] );
    return SelectFirstInGroup_Activate( version, global, local, serverData );
  }

  if( item == NULL )
    return SelectFirstInGroup_Activate( version, global, local, serverData );

  unsigned long i = group->GetChildren().FindIndexOf( item );
  if( i == DA_NO_MATCH ) {
    assert( i != DA_NO_MATCH );
    return AFUNC_OK;
  }

  item = group->GetChildren()[ i ];
  if( item->GetID() != SOM_GROUP ) {
    if( i+1 < group->GetChildren().NumElements() ) {
      item = group->GetChildren()[ i + 1 ];
      if( item->GetID() != SOM_GROUP ) {
        // Specific item; select it
        UpdateInterfaceFromGenerics( group, item );
        return AFUNC_OK;
      }
    } else {
      item = NULL;
    }
  }

  // No more items in group; go up through the levels and find one we can use
  if( item == NULL ) {
  }

  // Item is a group; walk into the group and look for items
  if( item->GetID() == SOM_GROUP ) {
  }

  return AFUNC_OK;
}

// SelectPrevInGroupHierarchy_Activate():
XCALL_ (int) SelectPrevInGroupHierarchy_Activate( long version, GlobalFunc *global,
                                                  void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();

  return AFUNC_OK;
}

// SelectParentGroup_Activate():
XCALL_ (int) SelectParentGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  UpdateInterfaceFromGenerics( group->GetParent(), NULL );

  return AFUNC_OK;
}

// SelectFirstChildGroup_Activate():
XCALL_ (int) SelectFirstChildGroup_Activate( long version, GlobalFunc *global,
                                             void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return SelectFirstGroup_Activate( version, global, local, serverData );

  unsigned long i;
  for( i=0; i < group->GetChildren().NumElements(); i++ ) {
    if( group->GetChildren()[i]->GetID() == SOM_GROUP )
      break;
  }

  if( i != group->GetChildren().NumElements() )
    UpdateInterfaceFromGenerics( group->GetChildren()[i], NULL );

  return AFUNC_OK;
}

// SelectFirstInGroup_Activate():
XCALL_ (int) SelectFirstInGroup_Activate( long version, GlobalFunc *global,
                                          void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return AFUNC_OK;

  unsigned long i;
  for( i=0; i < group->GetChildren().NumElements(); i++ ) {
    if( group->GetChildren()[i]->GetID() != SOM_GROUP )
      break;
  }

  if( i != group->GetChildren().NumElements() )
    UpdateInterfaceFromGenerics( group, group->GetChildren()[ i ] );

  return AFUNC_OK;
}

// SelectLastInGroup_Activate():
XCALL_ (int) SelectLastInGroup_Activate( long version, GlobalFunc *global,
                                         void *local, void *serverData ) {
  if ( version != LWLAYOUTGENERIC_VERSION  )
    return AFUNC_BADVERSION;

  if( som_data == NULL )
    return AFUNC_OK;

  SelectOMatic_Item *group = som_data->GetActiveGroup();
  if( group == NULL )
    return AFUNC_OK;

  unsigned long i;
  for( i = group->GetChildren().NumElements() - 1; i != -1; i-- ) {
    if( group->GetChildren()[i]->GetID() != SOM_GROUP )
      break;
  }

  if( i != group->GetChildren().NumElements() )
    UpdateInterfaceFromGenerics( group, group->GetChildren()[ i ] );

  return AFUNC_OK;
}

