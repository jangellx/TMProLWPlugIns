//
// ObjectShadowOptions-Interface.cpp
//

#include "ObjectShadowOptions-Interface.h"

#include <stdio.h>
#include <portable/dirtools/dirtools.h>
#include <lightwave/lwaboutpanel/lwaboutpanel.h>
#include <portable/stringtools/stringtools.h>

int  ObjectShadowOptions_Interface::limit_to         = 1;
int  ObjectShadowOptions_Interface::match_toggle     = 0;
char ObjectShadowOptions_Interface::match_string[]   = "";
int  ObjectShadowOptions_Interface::self_shadow      = 1;
int  ObjectShadowOptions_Interface::receive_shadow   = 1;
int  ObjectShadowOptions_Interface::cast_shadow      = 1;
int  ObjectShadowOptions_Interface::unseen_by_camera = 0;
int  ObjectShadowOptions_Interface::unseen_by_rays   = 0;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Object Shadow Options",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Object Shadow and Rendering Toggles for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Foundation Imaging",
  "http://www.foundation-i.com",
  LWABOUT_HDIV,
  "Written by Joe Angell",
  "jangell@tmproductions.com",
  "http://www.tmproductions.com",
  NULL };

ObjectShadowOptions_Interface *oui = NULL;

// Contructor():
ObjectShadowOptions_Interface::ObjectShadowOptions_Interface( GlobalFunc *_global, void *local )
  : global(*_global), retval(AFUNC_OK), is_ok(true), user_abort(false), panel(NULL),
    sel_type(-1), progress(global) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  iteminfo    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  boneinfo    = (LWBoneInfo      *)global( LWBONEINFO_GLOBAL,      GFUSE_TRANSIENT );
  objectinfo  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );

  if( ui->selItems[0] != NULL )
    sel_type = (iteminfo->type)( ui->selItems[0] );

  assert( oui == NULL );
  oui = this;
}

// Destructor():
ObjectShadowOptions_Interface::~ObjectShadowOptions_Interface() {
  if( panel != NULL )
    (*xpanels->destroy)( panel );

  oui = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool ObjectShadowOptions_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*xpanels->destroy)( panel );
    panel = NULL;
  }

  // Create the control array
  static LWXPanelControl controls[] = {
    { OUI_ABOUT,               "About",                       "vButton"    },
    { OUI_LIMIT_TO,            "Limit To",                    "iPopChoice" },
    { OUI_MATCH_TOGGLE,        "Match Name",                  "iBoolean"   },
    { OUI_MATCH_STRING,          "Name Contains",               "string"     },
    { OUI_SELF_SHADOW,         "Self Shadow",                 "iBoolean"   },
    { OUI_CAST_SHADOW,         "Cast Shadow",                 "iBoolean"   },
    { OUI_RECEIVE_SHADOW,      "Receive Shadow",              "iBoolean"   },
    { OUI_UNSEEN_BY_RAYS,      "Unseen By Rays",              "iBoolean"   },
    { OUI_UNSEEN_BY_CAMERA,    "Unseen By Camera",            "iBoolean"   },
    { 0 } };

  // Create the data array
  static LWXPanelDataDesc data[] = {
    { OUI_ABOUT,               "About",                       ""           },
    { OUI_LIMIT_TO,            "Limit To",                    "integer"    },
    { OUI_MATCH_TOGGLE,        "Match Name",                  "integer"    },
    { OUI_MATCH_STRING,          "Name Contains",               "string"     },
    { OUI_SELF_SHADOW,         "Self Shadow",                 "integer"    },
    { OUI_CAST_SHADOW,         "Cast Shadow",                 "integer"    },
    { OUI_RECEIVE_SHADOW,      "Receive Shadow",              "integer"    },
    { OUI_UNSEEN_BY_RAYS,      "Unseen By Rays",              "integer"    },
    { OUI_UNSEEN_BY_CAMERA,    "Unseen By Camera",            "integer"    },
    { 0 } };

  // Panel Hints Array
  char title_string[256];
  sprintf( title_string, "ObjectShadowOptions -- %c2001 Foundation Imaging, written by Joe Angell", 169 );
  static LWXPanelHint panel_hints[] = {
    XpLABEL( 0, title_string ),

    XpGROUP_( OUI_GROUP_LIMIT ),
      XpH( OUI_LIMIT_TO ),
      XpH( 0 ),

    XpGROUP_( OUI_GROUP_MATCH ),
      XpH( OUI_MATCH_TOGGLE ),
      XpH( OUI_MATCH_STRING ),
      XpH(0),
    
    XpGROUP_( OUI_GROUP_SHADOW ),
      XpH( OUI_SELF_SHADOW ),
      XpH( OUI_CAST_SHADOW ),
      XpH( OUI_RECEIVE_SHADOW ),
      XpH( 0 ),

    XpGROUP_( OUI_GROUP_RENDER ),
      XpH( OUI_UNSEEN_BY_RAYS ),
      XpH( OUI_UNSEEN_BY_CAMERA ),
      XpH( 0 ),

    XpBUTNOTIFY( OUI_ABOUT, HandleAbout ),

    XpPOPFUNCS(  OUI_LIMIT_TO, LimitCount, LimitName ),

    XpENABLE_( OUI_MATCH_TOGGLE ),
      XpH( OUI_MATCH_STRING ),
      XpH( 0 ),

    XpEND };

  // Get XPanels
  xpanels = (LWXPanelFuncs *)global( LWXPANELFUNCS_GLOBAL, GFUSE_TRANSIENT );
  if( xpanels == NULL ) {
    retval = AFUNC_BADGLOBAL;
    return false;
  }

  // Create the Panel
  panel = (*xpanels->create)( LWXP_FORM, controls );
  if( panel == NULL ) {
    (*message->error)( "Nullify Error: Unable to create XPanels interface", NULL );
    is_ok = false;
    return false;
  }

  // Set up other panel data
  (*xpanels->describe)( panel, data, NULL, NULL );
  (*xpanels->hint)( panel, 0, panel_hints );

  // Set up initial values for the panel
  FSETINT( xpanels, panel, OUI_LIMIT_TO,         limit_to         );
  FSETINT( xpanels, panel, OUI_MATCH_TOGGLE,     match_toggle     );
  FSETINT( xpanels, panel, OUI_SELF_SHADOW,      self_shadow      );
  FSETINT( xpanels, panel, OUI_RECEIVE_SHADOW,   receive_shadow   );
  FSETINT( xpanels, panel, OUI_CAST_SHADOW,      cast_shadow      );
  FSETINT( xpanels, panel, OUI_UNSEEN_BY_CAMERA, unseen_by_camera );
  FSETINT( xpanels, panel, OUI_UNSEEN_BY_RAYS,   unseen_by_rays   );

  (*xpanels->formSet)( panel, OUI_MATCH_STRING, match_string );

  // Open the panel
  if( (*xpanels->post)( panel ) != 1 )  { // User Cancel
    user_abort = true;
    return false;
  }

  return IsOK();
}

// HandleAbout():
void HandleAbout( LWXPanelID pan, int cid ) {
  FI_Logo logo;
  LWAboutPanel( oui->panel_funcs, "ObjectShadowOptions", about_text, &logo, oui->panel, true, NULL );
}

// Process():
bool ObjectShadowOptions_Interface::Process() {
  int total = 0;
  if( limit_to == 0 ) {
    for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) )
      total++;
  } else {
    for( total=0; ui->selItems[total] != NULL; total++ ) { ; }
  }

  LWProgressPanel progress( global, total );
  progress.SetAbortMessage( "ObjectShadowOptions Abort", "Abort Processing?  Note that some object may have already been affected", NULL );
  char buffer[ 1024 ];

  progress.Open( "Object Shadow Options Processing..." );

  int processed = 0;
  bool aborted = false;
  if( limit_to == 0 ) {
    int i = 0;
    for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
      sprintf( buffer, "Processing \"%s\"...", (*iteminfo->name)( item ) );
      if( !progress.Handle( ++i, buffer ) ) {
        (*message->info)( "Object Shadow Options: Processing aborted by user.", NULL );
        aborted = true;
        break;
      }

      if( ProcessObject( item ) )
        processed++;
    }
  } else {
    for( int i=0; ui->selItems[i] != NULL; i++ ) { 
      sprintf( buffer, "Processing \"%s\"...", (*iteminfo->name)( ui->selItems[i] ) );
      if( !progress.Handle( i, buffer ) ) {
        (*message->info)( "Object Shadow Options: Processing aborted by user.", NULL );
        aborted = true;
        break;
      }

      if( ProcessObject( ui->selItems[i] ) )
        processed++;
    }
  }

  if( progress.IsOpen() )
    progress.Close();

  // Reselect Items
  ReselectItems();

  if( !aborted ) {
    sprintf( buffer, "ObjectShadowOptions:  Successfully processed %d of %d objects", processed, total );
    (*message->info)( buffer , NULL );
  }

  return true;
}

// ProcessObject():
bool ObjectShadowOptions_Interface::ProcessObject( LWItemID item ) {
  if( match_toggle != 0 ) {
    if( StringTools::stristr( (*iteminfo->name)( item ), match_string ) == 0 )
      return false;
  }

  char buffer[100];
  sprintf( buffer, "SelectItem %x", item );
  (*generic->evaluate)( generic->data, buffer );

  unsigned int shadow = (*objectinfo->shadowOpts)( item );
  if( (self_shadow != 0) != ( (shadow & LWOSHAD_SELF) != 0 ) ) {
    (*generic->evaluate)( generic->data, "SelfShadow" );
  }

  if( (cast_shadow != 0) != ( (shadow & LWOSHAD_CAST) != 0 ) ) {
    (*generic->evaluate)( generic->data, "CastShadow" );
  }

  if( (receive_shadow != 0) != ( (shadow & LWOSHAD_RECEIVE) != 0 ) ) {
    (*generic->evaluate)( generic->data, "ReceiveShadow" );
  }

  unsigned int flags = (*objectinfo->flags)( item );
  if( (unseen_by_camera != 0) != ( (flags & LWOBJF_UNSEEN_BY_CAMERA) != 0 ) ) {
    (*generic->evaluate)( generic->data, "UnseenByCamera" );
  }

  if( (unseen_by_rays != 0) != ( (flags & LWOBJF_UNSEEN_BY_CAMERA) != 0 ) ) {
    (*generic->evaluate)( generic->data, "UnseenByRays" );
  }

  return true;
}

// ObtainUIStates():
bool ObjectShadowOptions_Interface::ObtainUIStates() {
  if( panel == NULL ) 
    return false;

  limit_to         = FGETINT( xpanels, panel, OUI_LIMIT_TO         );
  match_toggle     = FGETINT( xpanels, panel, OUI_MATCH_TOGGLE     );
  self_shadow      = FGETINT( xpanels, panel, OUI_SELF_SHADOW      );
  receive_shadow   = FGETINT( xpanels, panel, OUI_RECEIVE_SHADOW   );
  cast_shadow      = FGETINT( xpanels, panel, OUI_CAST_SHADOW      );
  unseen_by_camera = FGETINT( xpanels, panel, OUI_UNSEEN_BY_CAMERA );
  unseen_by_rays   = FGETINT( xpanels, panel, OUI_UNSEEN_BY_RAYS   );

  char * new_string = (char *)(*xpanels->formGet)( panel, OUI_MATCH_STRING );
  strcpy( match_string, ((new_string == NULL) ? "" : new_string) );

  return true;
}

// TestInputs():
bool ObjectShadowOptions_Interface::TestInputs() {
  // Makes sure at least one item is selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "ObjectShadowOptions Error: At least one object must be selected", NULL );
    return false;
  }

  // Make sure that item is an object
  if( sel_type != LWI_OBJECT ) {
    (*message->error)( "ObjectShadowOptions Error: ObjectShadowOptions only works on objects", NULL );
    return false;
  }
  
  return true;
}

// ReselectItems():
//  Reselects the previously selected items
void ObjectShadowOptions_Interface::ReselectItems() {
  for( int total=0; ui->selItems[total] != NULL; total++ ) { ; }
  LWProgressPanel progress( global, total );
  progress.SetAbortMessage( "ObjectShadowOptions Abort", "Abort Processing?  Note that some object may have already been affected", NULL );
  progress.Open( "ObjectShadowOptions Processing..." );

  char buffer[1024];
  if( total > 0 )
    sprintf( buffer, "SelectItem %x", ui->selItems[ total - 1 ] );

  for( int i = total - 2; i >= 0; i-- ) {
    sprintf( buffer, "Reselecting \"%s\"...", (*iteminfo->name)( ui->selItems[i] ) );
    if( !progress.Handle( i, buffer ) ) {
      (*message->info)( "Object Shadow Options: Processing aborted by user.", NULL );
      break;
    }
    
    sprintf( buffer, "AddToSelection %x", ui->selItems[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}

// Popup Callbacks
const char * limit_to_strings[] = { "All Objects", "Selected Objects" };
XCALL_ (int) LimitCount( void *data ) {
  return 2;
}

XCALL_ (const char *) LimitName(  void *data, int idx ) {
  if( (idx < 0) || (idx > 1) )
    return NULL;

  return limit_to_strings[ idx ];
}

