//
// SchemFlow.cpp
//

#include "schemflow-item.h"
#include "schemflow.h"

extern "C" {
  // User Names
  #ifdef _DEBUG
    ServerUserName SchemFlow_Usernames[] = { {"TM-P  SchemFlow", LANGID_USENGLISH | SRVTAG_USERNAME }, { "SchemFlow (Debug)", LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #else
    ServerUserName SchemFlow_Usernames[] = { {"TM-P  SchemFlow", LANGID_USENGLISH | SRVTAG_USERNAME }, { "SchemFlow",         LANGID_USENGLISH | SRVTAG_BUTTONNAME }, { (const char *)NULL } };
  #endif

  // Server Record
  ServerRecord ServerDesc[] = {
  #ifdef _DEBUG
    { LWLAYOUTGENERIC_CLASS, "TM-P_SchemFlow--Debug", SchemFlow_Activate,  SchemFlow_Usernames },
  #else
    { LWLAYOUTGENERIC_CLASS, "TM-P_SchemFlow",        SchemFlow_Activate,  SchemFlow_Usernames },
  #endif
    { (const char *)NULL }
  };
}


// SchemFlow_Activate():
XCALL_ (int) SchemFlow_Activate( long version, GlobalFunc *global,
                                 void *local, void *serverData ) {

  if( version != LWLAYOUTGENERIC_VERSION )
    return AFUNC_BADVERSION;

  LWLayoutGeneric * generic    = (LWLayoutGeneric *)local;
  LWMessageFuncs  * message    = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  LWInterfaceInfo * ui         = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  LWItemInfo      * iteminfo   = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );

  // Item Type Array
  LWItemType item_types[3] = { LWI_OBJECT, LWI_LIGHT, LWI_CAMERA };

  // First pass:  Build a list of all unparented items
  SchemFlow_Item base( 0 );
  LWItemID current_item;

  for( unsigned long i=0; i < 3; i++ ) {
    current_item = (*iteminfo->first)( item_types[i], LWITEM_NULL );
    while( current_item != LWITEM_NULL ) {
      if( (*iteminfo->parent)( current_item ) == LWITEM_NULL )
        base.GetChildren().Add( new SchemFlow_Item( current_item, NULL ) );

      current_item = (*iteminfo->next)( current_item );
    }
  }

  // Second Pass:  Build a hierarchy list
  for( i=0; i < base.GetChildren().NumElements(); i++ )
    base.GetChildren()[i]->BuildChildList( iteminfo );

  // Count the number of leafs in each hierarchy
  base.CountLeafs();

  // Re-arrange the schematic view
  base.ArrangeChildren( ui, generic );

  return AFUNC_OK;
}

