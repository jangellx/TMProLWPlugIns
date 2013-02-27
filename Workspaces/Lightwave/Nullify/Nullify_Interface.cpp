//
// Nullify_Interface.cpp
//

#include "Nullify_Interface.h"
#include  <lwdisplce.h>

#include <stdio.h>
#include <portable/dirtools/dirtools.h>

bool Nullify_Interface::nullify_decendants  = false;
bool Nullify_Interface::remove_bones        = false;
bool Nullify_Interface::disable_shadows     = false;
bool Nullify_Interface::store_original_name = true;
char Nullify_Interface::prefix[256]         = "";
char Nullify_Interface::suffix[256]         = "";
bool Nullify_Interface::remove_morph_mixer  = false;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Nullify",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Null Object Replacement Tool for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2000-2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  NULL };

Nullify_Interface *nui = NULL;

// Contructor():
Nullify_Interface::Nullify_Interface( GlobalFunc *_global, void *local )
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

  assert( nui == NULL );
  nui = this;
}

// Destructor():
Nullify_Interface::~Nullify_Interface() {
  if( panel != NULL )
    (*xpanels->destroy)( panel );

  nui = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool Nullify_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*xpanels->destroy)( panel );
    panel = NULL;
  }

  // Create the control array
  static LWXPanelControl controls[] = {
    { NUI_ABOUT,               "About",                       "vButton"  },
    { NUI_PREFIX,              "Prefix With",                 "string"   },
    { NUI_SUFFIX,              "Suffix With",                 "string"   },
    { NUI_NULLIFY_DECENDANTS,  "Nullify Decendants",          "iBoolean" },
    { NUI_REMOVE_BONES,        "Remove Bones",                "iBoolean" },
    { NUI_DISABLE_SHADOWS,     "Disable Shadow Options",      "iBoolean" },
    { NUI_STORE_ORIGINAL_NAME, "Store Original Names",        "iBoolean" },
    { NUI_REMOVE_MORPH_MIXER,  "Remove Morph Mixer",          "iBoolean" },
    { 0 } };

  // Create the data array
  static LWXPanelDataDesc data[] = {
    { NUI_ABOUT,               "About",                       ""        },
    { NUI_PREFIX,              "Prefix With",                 "string"  },
    { NUI_SUFFIX,              "Suffix With",                 "string"  },
    { NUI_NULLIFY_DECENDANTS,  "Nullify Decendants",          "integer" },
    { NUI_REMOVE_BONES,        "Remove Bones",                "integer" },
    { NUI_DISABLE_SHADOWS ,    "Disable Shadow Options",      "integer" },
    { NUI_STORE_ORIGINAL_NAME, "Store Original Names",        "integer" },
    { NUI_REMOVE_MORPH_MIXER,  "Remove Morph Mixer",          "integer" },
    { 0 } };

  // Panel Hints Array
  char title_string[256];
  sprintf( title_string, "Nullify -- %c2000-2001 Joe Angell, TM Productions", 169 );
  static LWXPanelHint panel_hints[] = {
    XpLABEL( 0, title_string ),
    XpGROUP_( NUI_GROUP_NAME ),
      XpH( NUI_PREFIX ),
      XpH( NUI_SUFFIX ),
      XpH( 0 ),

    XpGROUP_( NUI_GROUP_OPTIONS ),
      XpH( NUI_NULLIFY_DECENDANTS ),
      XpH( NUI_REMOVE_BONES ),
      XpH( NUI_DISABLE_SHADOWS ),
      XpH( NUI_REMOVE_MORPH_MIXER ),
      XpH( 0 ),

    XpBUTNOTIFY( NUI_ABOUT, HandleAbout ),

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
  (*xpanels->formSet)( panel, NUI_PREFIX, prefix );
  (*xpanels->formSet)( panel, NUI_SUFFIX, suffix );
  FSETINT( xpanels, panel, NUI_NULLIFY_DECENDANTS,  (nullify_decendants  ? 1 : 0) );
  FSETINT( xpanels, panel, NUI_REMOVE_BONES,        (remove_bones        ? 1 : 0) );
  FSETINT( xpanels, panel, NUI_DISABLE_SHADOWS,     (disable_shadows     ? 1 : 0) );
  FSETINT( xpanels, panel, NUI_STORE_ORIGINAL_NAME, (store_original_name ? 1 : 0) );
  FSETINT( xpanels, panel, NUI_REMOVE_MORPH_MIXER,  (remove_morph_mixer  ? 1 : 0) );

  // Open the panel
  if( (*xpanels->post)( panel ) != 1 )  { // User Cancel
    user_abort = true;
    return false;
  }

  return IsOK();
}

// HandleAbout():
void HandleAbout( LWXPanelID pan, int cid ) {
  LWAboutPanel_TMPro( nui->panel_funcs, "Nullify", about_text, NULL, true );
}

// BuildItemList():
bool Nullify_Interface::BuildItemList() {
  if( ui->selItems[0] == NULL )
    return false;

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  return true;
}

// ObtainUIStates():
bool Nullify_Interface::ObtainUIStates() {
  if( panel == NULL ) 
    return false;

  nullify_decendants  = (FGETINT( xpanels, panel, NUI_NULLIFY_DECENDANTS  ) > 0) ? true : false;
  remove_bones        = (FGETINT( xpanels, panel, NUI_REMOVE_BONES        ) > 0) ? true : false;
  disable_shadows     = (FGETINT( xpanels, panel, NUI_DISABLE_SHADOWS     ) > 0) ? true : false;
  store_original_name = (FGETINT( xpanels, panel, NUI_STORE_ORIGINAL_NAME ) > 0) ? true : false;
  remove_morph_mixer  = (FGETINT( xpanels, panel, NUI_REMOVE_MORPH_MIXER  ) > 0) ? true : false;

  char * new_prefix = (char *)(*xpanels->formGet)( panel, NUI_PREFIX );
  strcpy( prefix, ((new_prefix == NULL) ? "" : new_prefix) );

  char * new_suffix = (char *)(*xpanels->formGet)( panel, NUI_SUFFIX );
  strcpy( suffix, ((new_suffix == NULL) ? "" : new_suffix) );

  return true;
}

// TestInputs():
bool Nullify_Interface::TestInputs() {
  // Makes sure at least one item is selected
  if( ui->selItems[0] == NULL ) {
    (*message->error)( "Nullify Error: At least one object must be selected", NULL );
    return false;
  }

  // Make sure that item is a bone or object
  if( sel_type != LWI_OBJECT ) {
    (*message->error)( "Nullify Error: Nullify only works on objects", NULL );
    return false;
  }

  return true;
}

// Process():
bool Nullify_Interface::Process() {
  // Build Item List
  BuildItemList();

  // Build Hierarchy
  LWItemHierarchy base( LWITEM_NULL );
  base.BuildChildList( iteminfo );
  base.ReadLayoutSelection( iteminfo, ui );

  #ifdef _DEBUG
    ofstream out( "i:\\Nullify_Hierarchy.txt" );
    base.OutputToStream( out, iteminfo );
    out.close();
  #endif

  // Select decendants, if applicable
  if( nullify_decendants )
    base.SelectSelectedsChildren( true );

  // Set up the progress bar
  char title_string[256];
  sprintf( title_string, "Nullify -- %c2000-2001 Joe Angell, TM Productions", 169 );

  progress.SetTotal( items.NumElements() - 1 );
  progress.SetCurrent( 1 );
  progress.SetAbortMessage( "Confirm Nullify Abort", "Do you really want to abort nullification?",
                            "Note that some objects may have already been converted to nulls." );
  progress.Open( title_string );

  // Process each object
  (*generic->evaluate)( generic->data, "AutoConfirm 1" );
  ProcessObject( &base );
  (*generic->evaluate)( generic->data, "AutoConfirm 0" );

  // Reselect Items
  ReselectItems();

  return true;
}

// ProcessObject():
//  If this returns false, this index in the child array should be
//   processed again, since what was there has been deleted.
bool Nullify_Interface::ProcessObject( LWItemHierarchy *object ) {
  if( object->GetType() == LWI_OBJECT ) {
    if( object->GetIsSelected() ) {
      // Update the progress bar
      char progress_string[1024];
      sprintf( progress_string, "Nullifying \"%s\"...", (*iteminfo->name)( object->GetID() ) );

      progress.IncrementBar( progress_string );
      if( !progress.Handle() )
        return true;

      // Store the original name
      char buffer[1024];
      if( store_original_name ) {
        const char *tag;
        for( int t=1; tag = (*iteminfo->getTag)( object->GetID(), t ); t++ ) {
          if( tag == NULL )
            break;

          if( strnicmp( tag, "OriginalItem", 12 ) == 0 )
            break;
        } 

        if( tag == NULL )
          t = 0;

        int type = 1;
        if( ((*objectinfo->numPoints)(   object->GetID() ) <= 1) &&
            ((*objectinfo->numPolygons)( object->GetID() ) == 0) ) {
          type = 0;
        }

        sprintf( buffer, "OriginalItem %d %s", type, (*objectinfo->filename)( object->GetID() ) );
        (*iteminfo->setTag)( object->GetID(), t, buffer );
      }

      // Replace the object
      sprintf( buffer, "SelectItem %x", object->GetID() );
      (*generic->evaluate)( generic->data, buffer );
      
      sprintf( buffer, "ReplaceWithNull %s%s%s", prefix, DirStrings::HasFile( (*objectinfo->filename)( object->GetID() ) ), suffix );
      (*generic->evaluate)( generic->data, buffer );

      // Disable Shadows
      if( disable_shadows ) {
        unsigned int shadow = (*objectinfo->shadowOpts)( object->GetID() );
        if( shadow & LWOSHAD_SELF )
          (*generic->evaluate)( generic->data, "SelfShadow" );

        if( shadow & LWOSHAD_CAST )
          (*generic->evaluate)( generic->data, "CastShadow" );

        if( shadow & LWOSHAD_RECEIVE )
          (*generic->evaluate)( generic->data, "ReceiveShadow" );
      }

      // Remove the bones
      if( remove_bones ) {
        if( object->AreAnyChildrenBones() )
          (*generic->evaluate)( generic->data, "ClearAllBones" );
      }

      // Remove MorphMixer
      if( remove_morph_mixer ) {
        const char *server_name = (const char *)1;
        for( unsigned long i=1; server_name != NULL; i++ ) {
          server_name = (*iteminfo->server)( ui->selItems[0], LWDISPLACEMENT_HCLASS, i );
          if( server_name == NULL )
            break;

          if( stricmp( server_name, "LW_MorphMixer" ) == 0 ) {
            sprintf( buffer, "RemoveServer %s %d", LWDISPLACEMENT_HCLASS, i );
            (*generic->evaluate)( generic->data, buffer );
            break;
          }
        }
      }
    }
  }

  // Process Children
  for( unsigned long i=0; i < object->GetChildren().NumElements(); i++ ) {
    if( !ProcessObject( object->GetChildren()[i] ) )
      i--;  // Item was deleted; do this index again

    if( !progress.IsOpen() )
      return true;
  }

  return true;
}

// ReselectItems():
//  Reselects the previously selected items
void Nullify_Interface::ReselectItems() {
  if( sel_type == -1 )
    return;

  char buffer[100];
  if( items.NumElements() > 0 )
    sprintf( buffer, "SelectionItem %x", items[ items.NumElements()-1 ] );

  // Set up the progress bar
  char title_string[256];
  sprintf( title_string, "Nullify -- %c2000-2001 Joe Angell, TM Productions", 169 );

  progress.SetTotal( items.NumElements() - 1 );
  progress.SetCurrent( 1 );
  progress.Open( title_string );

  // Reselect the items
  char progress_string[ 1024 ];
  for( unsigned long i=items.NumElements()-1; i != -1; i-- ) {
    sprintf( progress_string, "Selecting \"%s\"...", (*iteminfo->name)( items[i] ) );
    progress.Handle( progress.GetCurrent() + 1, progress_string );

    sprintf( buffer, "AddToSelection %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}
