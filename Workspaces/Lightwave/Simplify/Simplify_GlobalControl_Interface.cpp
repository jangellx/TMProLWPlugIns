//
// Simplifly_GlobalControl_Interface.cpp
//

#include <stdio.h>

#include "simplify_globalcontrol_interface.h"
#include "simplifylogo.h"

SimplifyGlobalInterface  * simpg_int = NULL;
extern SimplifyLogo        logo;

extern int Simplify_Version;
extern int Simplify_Revision;

// Constructor
SimplifyGlobalInterface::SimplifyGlobalInterface( GlobalFunc *_global,
                                                  void       *local,
                                                  int        &retval )
  : main_panel(NULL), about_panel(NULL),
    Global(*_global), message(NULL), panel_funcs(NULL) {

  // Set the global UberNullInterface pointer so the draw functions and controls can use it
  simpg_int = this;

  // Get the Panel Functions and Scene Info from Lightwave
  message     = CMessage::New( (MessageFuncs *) Global( "Info Messages", GFUSE_TRANSIENT) );
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global( PANEL_SERVICES_NAME, GFUSE_TRANSIENT ), _global );
  scene_info  = CLWSceneInfo::New( (LWSceneInfo *) Global( "LW Scene Info", GFUSE_TRANSIENT ) );

  if( !message ) {
    retval = AFUNC_BADGLOBAL;
    return;
  }

  if( !panel_funcs ) {
    message->Error( "ERROR: Unable to find lwpanels.p to create the GUI",
                    "Add it and try again." );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  // Get the Dir Info Function so we can get the Content Dir
  DirInfoFunc *DirInfo = *( (DirInfoFunc *) Global( "Directory Info", GFUSE_TRANSIENT ) );
  if( DirInfo == NULL ) {
    message->Error( "ERROR: Unable to obtain Directory Info from Lightwave; aborting" );
    retval = AFUNC_BADGLOBAL;
    return;
  }

  content_dir = (*DirInfo)( "Content" );

  // Get/Generate the default source/output filenames
  source_filename[0] = '\0';
  if( DirStrings::HasDrive( scene_info->filename ) )
    strcpy( source_filename, content_dir );
  DirStrings::AddPathPart( source_filename, scene_info->filename );

  strcpy( output_filename, source_filename );
  char *ext = DirStrings::HasExtension( output_filename );
  if( ext != NULL )
    ext[0] = '\0';
  strcat( output_filename, "-Simplified.lws" );

  // Create the panels
  if( MakeMainPanel() ) {                  // Create the main panel
    if( MakeAboutPanel() ) {               // Create the about panel
      if( OpenMainPanel() ) {
        retval = AFUNC_OK;
      } else {
        retval = AFUNC_BADLOCAL;
      }
    } else {
      retval = AFUNC_BADLOCAL;
    }
  } else {
    retval = AFUNC_BADLOCAL;
  }
}

// Destructor
SimplifyGlobalInterface::~SimplifyGlobalInterface() {
  panel_funcs->Destroy( main_panel  );
  panel_funcs->Destroy( about_panel );

  delete panel_funcs;
  delete message;
  delete scene_info;
}

// MakeMainPanel():
bool SimplifyGlobalInterface::MakeMainPanel() {
  // Set up Main Title String
  sprintf( main_title_bar, "Simplify Global Control V%d.%d from TM Productions",
           Simplify_Version, Simplify_Revision );

  // Create the Panel
  main_panel = panel_funcs->Create( main_title_bar );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  int x, y, w, h;
  try {
    // Source/Output Paths
    if( !( source_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                    "Source Scene", 70 ) ) )
      throw false;

    if( !( output_scene = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                    "Output Scene", 70 ) ) )
      throw false;

    x = CON_X( source_scene );
    y = CON_X( source_scene );
    MOVE_CON( source_scene, x+16, y+70 );

    SET_STR( source_scene,  source_filename, strlen( source_filename ) );
    SET_STR( output_scene,  output_filename, strlen( output_filename ) );

    PanelTools::AlignLabels( source_scene, output_scene );

    // Instance Lister
    if( !( instance_listbox = LISTBOX_CTL( panel_funcs->orig_struct, main_panel,
                                           "Instance List", 363, 15,
                                           InstanceListerNames, InstanceListerCount)) )

    x = CON_X( instance_listbox );
    y = CON_Y( instance_listbox );
    MOVE_CON( instance_listbox, x+5, y );

    // Set the Panel Size
    y = CON_Y( instance_listbox );
    h = CON_H( instance_listbox );
    PAN_SETH( panel_funcs->orig_struct, main_panel, y + h + 30 );

    w = CON_X( output_scene );
    x = CON_W( output_scene );
    PAN_SETW( panel_funcs->orig_struct, main_panel, x + w );

    // Set the Draw Functions
    panel_funcs->Set( main_panel, PAN_USERDRAW, SG_DrawMainPanel );

    return true;
  } catch( bool a ) {
    if( !a ) {
      message->Error("Error creating interface controls; aborting" );
      return false;
    }
  }

  return true;
}

// OpenMain():
bool SimplifyGlobalInterface::OpenMainPanel() {
  if( !panel_funcs->Open( main_panel, PANF_BLOCKING | PANF_MOUSETRAP ) )
    return false;

  return true;
}

// DrawMainPanel():
//  Draw details on the main panel (ie: dividers)
void SG_DrawMainPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = simpg_int->panel_funcs->DrawFuncs;
  int ph = PAN_GETH( simpg_int->panel_funcs->orig_struct, simpg_int->main_panel );
  int pw = PAN_GETW( simpg_int->panel_funcs->orig_struct, simpg_int->main_panel );

  // Horizontal Dividers
  int y = CON_Y( simpg_int->output_scene );
  int h = CON_H( simpg_int->output_scene );
  PanelDraw::HorizontalDivider( simpg_int->main_panel, df, 0, y+h+2, pw, 1 );

/*
  y = CON_Y( simpg_int->render_filename );
  h = CON_H( simpg_int->render_filename );
  PanelDraw::HorizontalDivider( simpg_int->main_panel, df, 0, y+h+2, pw, 1 );

  y = CON_Y( simpg_int->lod_distance );
  h = CON_H( simpg_int->lod_distance );
  PanelDraw::HorizontalDivider( simpg_int->main_panel, df, 0, y+h+2, pw, 1 );
*/

  // Title Image
//  PanelDraw::DrawImage( simp_int->main_panel, df, logo, -25, 0 );
  logo.DrawInLWPanels( simpg_int->main_panel, df, -25, 0 );
}

// MakeAboutPanel()
//  Creates and initializes the About Panel
bool SimplifyGlobalInterface::MakeAboutPanel() {
  about_panel = panel_funcs->Create( "Simplify, by TM Productions" );

  if( !about_panel ) {   // Make sure the panel was created
    message->Error("Error creating about panel, aborting" );
    return false;
  }

  PAN_SETW( panel_funcs->orig_struct, about_panel, 385 );
  PAN_SETH( panel_funcs->orig_struct, about_panel, 254 );

  panel_funcs->Set( about_panel, PAN_USERDRAW, DrawAboutPanel );

  return true;
}

// OpenAboutPanel()
//  Opens the about panel in blocking mode
bool SimplifyGlobalInterface::OpenAboutPanel() {
  if( !panel_funcs->Open( about_panel, PANF_BLOCKING ) )
    return false;

  return true;
}

// Instance Lister Functions
char *InstanceListerNames( void *dat, int index ) {
//	if( index >= (int)simp_int->instance.lod.NumElements() )
//		return NULL;

   if( index > 0 )
     return NULL;

   return "(none)";
//	return (char *)simp_int->instance.lod[ index ]->GetListerName();
}

int InstanceListerCount( void *dat ) {
  return 0;
//  return simp_int->instance.lod.NumElements();
}
