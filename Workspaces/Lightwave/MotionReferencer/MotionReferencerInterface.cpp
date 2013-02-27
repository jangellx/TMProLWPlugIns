//
// MotionReferencerInterface.cpp
//

#include <stdio.h>

#include <portable/dynarray/dynarray.h>

#include "MotionReferencerInterface.h"

MotionReferencerInterface                   * mr_int = NULL;

extern DynArray< MotionReferencerInstance * > instances;

extern int MotionReferencer_Version;
extern int MotionReferencer_Revision;
extern int MotionReferencer_Subrevision;

// Constructor:
MotionReferencerInterface::MotionReferencerInterface( GlobalFunc *_global,
                                                      MotionReferencerInstance *_instance,
                                                      int &retval )
  : Global(*_global), orig_instance(_instance), panel_funcs(NULL), message(NULL),
    main_panel(NULL), about_panel(NULL), keys_panel(NULL),
    shift_down(false), ctrl_down(false), alt_down(false), last_key_down(-1) {

  // Set the mr_int to this instance
  mr_int = this;

  // Get the Panel Functions and Message Functions
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global(PANEL_SERVICES_NAME, GFUSE_TRANSIENT ),
                                    _global );
  message = CMessage::New( (LWMessageFuncs *) Global("Info Messages", GFUSE_TRANSIENT) );

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

  // Get DirFuncs so we can get the Content Dir
  LWDirInfoFunc * dir_func = (LWDirInfoFunc *)Global("Directory Info", GFUSE_TRANSIENT );
  strcpy( content_dir, (*dir_func)( "Content" ) );
  DirStrings::AddDirSeperator( content_dir );

  // Create the panels
  if( MakeMainPanel() ) {                        // Create the main panel
    if( MakeAboutPanel() ) {                     // Create the about panel
      if( MakeKeysPanel() ) {                    // Create the keyboard help panel
        if( InitMainPanel() ) {                  // Initialize the Main Panel with the Instance Data passed in
          if( OpenMainPanel() ) {                // Open the Main Panel
            TestInstance( orig_instance );       // User closed panel with OK; Make sure the settings are OK and copy the instance data
          }
        }
      }
    }
  }

  retval = AFUNC_OK;
}

// Destructor:
MotionReferencerInterface::~MotionReferencerInterface() {
  panel_funcs->Destroy( main_panel  );
  panel_funcs->Destroy( about_panel );
  panel_funcs->Destroy( keys_panel );

  delete panel_funcs;
  delete message;

  mr_int = NULL;                          // Nuke this; only the interface functions should use it
}

// MakeMainPanel()
//  Creates and initializes the Main Panel
bool MotionReferencerInterface::MakeMainPanel() {
  // Create the Panel
  sprintf( main_title_bar, "MotionReferencer V%d.%d.%d from TM Productions", MotionReferencer_Version, MotionReferencer_Revision, MotionReferencer_Subrevision );
  main_panel = panel_funcs->Create( main_title_bar );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  int x, y, h, w;
  try {
    // Instance Popup
    if( !( instance_popup = CUSTPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                          "Instances", 415, InstanceNames, InstanceCount ) ) )
      throw false;

    x = CON_X( instance_popup );
    y = CON_Y( instance_popup );
    MOVE_CON( instance_popup, x+10, y+65 );

    // F1 (Keyboard Help) Button
    if( !( f1 = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                             "F1", 30 ) ) )
      throw false;

    PanelTools::PutOnLineWith( instance_popup, f1 );

    // About MotionReferencer button
    if( !( about = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                            "About MotionReferencer", 150 ) ) )
      throw false;

    x = CON_X( about );
    y = CON_Y( about );
    MOVE_CON( about, x+10, y+5 );

    // Enable MotionReferencer checkbox
    if( !( enable_mr = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                 "Enable MotionReferencer" ) ) )
      throw false;
    SET_INT( enable_mr, true );

    PanelTools::PutOnLineWith( about, enable_mr, 10 );


    // Load/Save SDF buttons
    if( !( load_mrf = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                   "Load .SDF", 150 ) ) )
      throw false;

    if( !( save_mrf = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                   "Save .SDF", 150 ) ) )
      throw false;

    SET_STR( load_mrf, "", 0 );
    SET_STR( save_mrf, "", 0 );

    PanelTools::PutOnLineWith( enable_mr, load_mrf, 103 );
    PanelTools::PutUnder( load_mrf, save_mrf );
    PanelTools::AlignWith( load_mrf, save_mrf );

    // Move the About/Enable buttons again
    x = CON_X( about );
    y = CON_Y( about );
    MOVE_CON( about, x, y+10 );

    PanelTools::PutOnLineWith( about, enable_mr, 10 );

    // Set up the Event Handlers
    CON_SETEVENT( about,                OnAbout,              this );
    CON_SETEVENT( load_mrf,             OnLoadMRF,            this );
    CON_SETEVENT( save_mrf,             OnSaveMRF,            this );
    CON_SETEVENT( enable_mr,            OnEnableMR,           this );
    CON_SETEVENT( instance_popup,       OnInstancePopup,      this );
    CON_SETEVENT( f1,                   OnF1,                 this );

    // Panel-Level Functions
    panel_funcs->Set( main_panel, PAN_USERDRAW,   DrawMainPanel );
    panel_funcs->Set( main_panel, PAN_USERKEYS,   OnKeyDown     );
    panel_funcs->Set( main_panel, PAN_USERKEYUPS, OnKeyUp       );
  
    return true;
  } catch( bool ) {
    return false;
  }
}

// OpenMainPanel()
//  Opens the main panel in blocking mode
bool MotionReferencerInterface::OpenMainPanel() {
  int retval = panel_funcs->Open( main_panel, PANF_BLOCKING | PANF_MOUSETRAP | PANF_PASSALLKEYS | PANF_CANCEL);

  if( retval == 0 )       // User hit Cancel
    return false;
  else if( retval == 1 )  // User hit OK
    return true;
  else                    // Unknown; error?
    return false;
}

// CloseMainPanel()
//  Closes the main panel
void MotionReferencerInterface::CloseMainPanel() {
  panel_funcs->Close( main_panel );
}

// DrawMainPanel():
//  Draw details on the main panel (ie: dividers)
void DrawMainPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = mr_int->panel_funcs->DrawFuncs;
  int ph = PAN_GETH( mr_int->panel_funcs->orig_struct, mr_int->main_panel );
  int pw = PAN_GETW( mr_int->panel_funcs->orig_struct, mr_int->main_panel );

/*
  // Horizontal Dividers
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, 70, pw, 1 );

  int y = CON_Y( mr_int->instance_popup );
  int h = CON_H( mr_int->instance_popup );
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, y+h+1, pw, 1 );

  y = CON_Y( mr_int->save_mrf );
  h = CON_H( mr_int->save_mrf );
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, y+h+2, pw, 1 );

  y = CON_Y( mr_int->render_filename );
  h = CON_H( mr_int->render_filename );
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( mr_int->lod_use_camera_zoom );
  h = CON_H( mr_int->lod_use_camera_zoom );
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( mr_int->lod_distance );
  h = CON_H( mr_int->lod_distance );
  PanelDraw::HorizontalDivider( mr_int->main_panel, df, 0, y+h+2, pw, 1 );

  // LOD Model List Title Text
  int x = CON_X( mr_int->lod_listbox );
      y = CON_Y( mr_int->lod_listbox );
  df->WriteText( mr_int->main_panel, "LOD Model List", COLOR_BLACK,
                 x+7, y-14 );

  // Title Image
  logo.DrawInLWPanels( mr_int->main_panel, df, -25, 0 );
*/
}

// MakeAboutPanel()
//  Creates and initializes the About Panel
bool MotionReferencerInterface::MakeAboutPanel() {
  about_panel = panel_funcs->Create( "MotionReferencer, by TM Productions" );

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
bool MotionReferencerInterface::OpenAboutPanel() {
  if( !panel_funcs->Open( about_panel, PANF_BLOCKING ) )
    return false;

  return true;
}

// InitMainPanel():
//  Initializes the main panel with data from the instance
//   provided to the Activate function.  The initialize 
//   variable should be set to true only when the panel
//   is first opened or when the instance we're editing
//   changes (ie: the Instance Popup).
bool MotionReferencerInterface::InitMainPanel( bool initialize ) {
  if( initialize )
    instance.CopyFrom( *orig_instance );

  AddContent( &instance );
/*
  // Copy values from the instance into the interface
  SET_STR( render_filename,  (char *)instance.GetRender(),  strlen( instance.GetRender()  ) );
  SET_STR( preview_filename, (char *)instance.GetPreview(), strlen( instance.GetPreview() ) );
  SET_STR( load_mrf,         (char *)instance.GetSDF(),     strlen( instance.GetSDF()     ) );
  SET_STR( save_mrf,         (char *)instance.GetSDF(),     strlen( instance.GetSDF()     ) );

  SET_INT( enable_MotionReferencer,  instance.GetEnabled()       );
  SET_INT( enable_preview,   instance.GetEnablePreview() );
  SET_INT( enable_lod,       instance.GetEnableLOD()     );
  SET_INT( store_as,         instance.GetStoreAs()       );

  SET_INT(   lod_use_camera_zoom,     instance.GetLODUseCameraZoom() );
  SET_FLOAT( lod_nominal_camera_zoom, instance.GetLODNominalCameraZoom() );
  SET_FLOAT( lod_distance_multiplier, (instance.GetLODDistanceMultiplier() * 100.0f) );

  if( !instance.GetLODUseCameraZoom() )
    GHOST_CON( lod_nominal_camera_zoom );

  REDRAW_CON( lod_listbox );
  UpdateLOD( 0 );

  if( mr_int->instance.GetSDF()[0] != '\0' ) {
    sprintf( mr_int->main_title_bar, "MotionReferencer V%d.%d.%d from TM Productions    ( %s )", MotionReferencer_Version, MotionReferencer_Revision, mr_int->instance.GetSDF() );
    mr_int->panel_funcs->Set( mr_int->main_panel, PAN_TITLE, mr_int->main_title_bar );
  }

  // Figure out what item id this instance matches so we can set the Instance Pop-Up correctly
  for( unsigned int i=0; i < instances.NumElements(); i++ ) {
    if( instances[i]->GetItemID() == instance.GetItemID() )
      SET_INT( instance_popup, i );
  }

  UpdateGhosts();
*/
  return true;
}

// TestInstance():
//  Tests to make sure the new instance data is valid before
//   copying it back into the original instance.
void MotionReferencerInterface::TestInstance( MotionReferencerInstance *output_inst ) {
  // Strip off the content dir from the paths, if needed
  StripContent( &instance );

  // Copy the new instance into the original instance
  output_inst->CopyFrom( instance );   
}

// AddContent():
//  Add the content dir to each path in the instance, if needed.
void MotionReferencerInterface::AddContent( MotionReferencerInstance *inst ) {
/*
  char new_buffer[ MAX_PATH_LENGTH ];

  if( DirStrings::HasDrive( inst->GetSDF() ) == NULL ) {
    strcpy( new_buffer, mr_int->content_dir );
    DirStrings::AddPathPart( new_buffer, inst->GetSDF() );
    inst->SetSDF( new_buffer );
  }
*/
}


// StripContent():
//  Removes the content dir from all paths in an instance,
//   if needed.
void MotionReferencerInterface::StripContent( MotionReferencerInstance *inst ) {
/*
  int len = strlen( content_dir );
  if( strnicmp( inst->GetRender(), content_dir, len ) == 0 )
    inst->SetRender( &inst->GetRender()[len] );
*/
}

// OnAbout():
//  Called to open the About Box.
void OnAbout( LWControl *ctl, MotionReferencerInterface *panel ) {
  panel->OpenAboutPanel();
}

// DrawAbout():
//  Called to draw the About box
void DrawAboutPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = mr_int->panel_funcs->DrawFuncs;

//  logo.DrawInLWPanels( mr_int->about_panel, df, -220, 0 );

  int ph = PAN_GETH( mr_int->panel_funcs->orig_struct, mr_int->about_panel );
  int pw = PAN_GETW( mr_int->panel_funcs->orig_struct, mr_int->about_panel );

  int y = 70;

  char buffer[256];
  sprintf( buffer, "%s V%d.%d.%d", about_text[0], MotionReferencer_Version, MotionReferencer_Revision, MotionReferencer_Subrevision );
  df->WriteText( mr_int->about_panel, buffer, COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(mr_int->about_panel, buffer))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( mr_int->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(mr_int->about_panel, about_text[i] ))/2,
             y );
    y += 15;
  }
}

// Instance Popup Functions
char *InstanceNames( void *dat, int index ) {
	if( index >= (int)instances.NumElements() )
		return NULL;

  return (char *)instances[ index ]->GetInstanceName();
}

int InstanceCount( void *dat ) {
  return instances.NumElements();
}

// OnEnableMR():
void OnEnableMR( LWControl *ctl, MotionReferencerInterface *panel ) {
  int state;
  GET_INT( mr_int->enable_mr, state );

  mr_int->instance.SetEnabled( (bool) state );
}

// OnLoadMRF():
void OnLoadMRF(         LWControl *ctl, MotionReferencerInterface *panel ) {

}

// OnSaveMRF():
void OnSaveMRF(         LWControl *ctl, MotionReferencerInterface *panel ) {

}

void OnInstancePopup(   LWControl *ctl, MotionReferencerInterface *panel ) {

}
