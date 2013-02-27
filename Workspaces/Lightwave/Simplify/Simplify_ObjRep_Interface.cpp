//
// Simplify_ObjRep_Interface.cpp
//
// This file contains the user interface for the Simplify
//  Object Replacement plug-in.
//

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "portable/dirtools/dirtools.h"

#include "Simplify_ObjRep_Interface.h"
#include "Simplify_Generic.h"
#include "SimplifyLogo.h"
#include "Simplify_Config.h"

// Globals and Externals
SimplifyInterface                      * simp_int = NULL;

extern DynArray< SimplifyInstance * >    instances;
extern SimplifyConfig                    simp_config;
extern SimplifyLogo                      logo;

#ifdef DEBUG_PANEL_SIZE
int  psize_w;
bool update_size = false;
#endif

// Versioning
int   Simplify_Version  = 1;
int   Simplify_Revision = 5;
float SDF_Version       = 1.0;

// Constructor
//  Note that while this does create everything needed
//   for the panel, it does not open it.  Use OpenMainPanel()
//   for that.
//  Note also that since the destructor frees all allocated structures,
//   nothing is cleared here if an error occurs.
SimplifyInterface::SimplifyInterface( GlobalFunc *_global,
                                      SimplifyInstance *_instance,
                                      int &retval )
  : Global(*_global), orig_instance(_instance), panel_funcs(NULL), message(NULL),
    main_panel(NULL), about_panel(NULL), selected_lod(0), lod_top(0),
    shift_down(false), ctrl_down(false), alt_down(false), last_key_down(false),
    simpd(NULL) {

  // Set the simp_int to this instance
  simp_int = this;

  // Get the Panel Functions and Message Functions
  panel_funcs = CLWPanelFuncs::New( (LWPanelFuncs *) Global(PANEL_SERVICES_NAME, GFUSE_TRANSIENT ),
                                    _global );
  message = CMessage::New( (MessageFuncs *) Global("Info Messages", GFUSE_TRANSIENT) );

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
  DirInfoFunc * dir_func = (DirInfoFunc *)Global("Directory Info", GFUSE_TRANSIENT );
  strcpy( content_dir, (*dir_func)( "Content" ) );
  DirStrings::AddDirSeperator( content_dir );

  // Create the Register Panel
  simpd = new SimplifyDongle;
  if( !simpd->TestKey( simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    if( simpd->OpenRegisterPanel() == 1 ) {    // Unregistered; don't open the UI
      retval = AFUNC_OK;
      return;
    }
  }

  // Create the panels
  if( MakeMainPanel() ) {                        // Create the main panel
    if( MakeAboutPanel() ) {                     // Create the about panel
      if( MakeKeysPanel() ) {                    // Create the keyboard help panel
        if( InitMainPanel() ) {                  // Initialize the Main Panel with the Instance Data passed in
#ifdef DEBUG_PANEL_SIZE
          do {
#endif
            if( OpenMainPanel() ) {                // Open the Main Panel
              TestInstance( orig_instance );       // User closed panel with OK; Make sure the settings are OK and copy the instance data
            }
#ifdef DEBUG_PANEL_SIZE
          } while( update_size );
#endif
        }
      }
    }
  }

  retval = AFUNC_OK;
}

// Destructor
SimplifyInterface::~SimplifyInterface() {
  panel_funcs->Destroy( main_panel  );
  panel_funcs->Destroy( about_panel );

  delete panel_funcs;
  delete message;

  simp_int = NULL;                          // Nuke this; only the interface functions should use it
}

// MakeMainPanel()
//  Creates and initializes the Main Panel
bool SimplifyInterface::MakeMainPanel() {
  // Create the Panel
  sprintf( main_title_bar, "Simplify V%d.%d from TM Productions    ( unnamed )", Simplify_Version, Simplify_Revision );
  main_panel = panel_funcs->Create( main_title_bar );

  if( !main_panel ) {                    // Make sure the panel was created
    message->Error("Error creating main panel, aborting" );
    return false;
  }

  if( !simpd->TestKey( simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    message->Error( "Simplify is not registered;", "the interface is not available" );
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

  // About Simplify button
  if( !( about = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                          "About Simplify", 150 ) ) )
    throw false;

  x = CON_X( about );
  y = CON_Y( about );
  MOVE_CON( about, x+10, y+5 );

  // Enable Simplify checkbox
  if( !( enable_simplify = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                     "Enable Simplify" ) ) )
    throw false;
  SET_INT( enable_simplify, true );

  PanelTools::PutOnLineWith( about, enable_simplify, 10 );

  // Load/Save SDF buttons
  if( !( load_sdf = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                 "Load .SDF", 150 ) ) )
    throw false;

  if( !( save_sdf = FILEBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                 "Save .SDF", 150 ) ) )
    throw false;

  SET_STR( load_sdf, "", 0 );
  SET_STR( save_sdf, "", 0 );

  PanelTools::PutOnLineWith( enable_simplify, load_sdf, 103 );
  PanelTools::PutUnder( load_sdf, save_sdf );
  PanelTools::AlignWith( load_sdf, save_sdf );

  // Move the About/Enable buttons again
  x = CON_X( about );
  y = CON_Y( about );
  MOVE_CON( about, x, y+10 );

  PanelTools::PutOnLineWith( about, enable_simplify, 10 );
  
  // Preview/Render Object
  SET_INT( enable_simplify, false );

  if( !( preview_filename = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                     "Preview Object", 45 ) ) )
    throw false;

  if( !( render_filename = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                     "Render Object", 45 ) ) )
    throw false;

  SET_STR( preview_filename, "", 0 );
  SET_STR( render_filename, "", 0 );

  x = CON_X( preview_filename );
  y = CON_Y( preview_filename );
  MOVE_CON( preview_filename, x, y+6 );
  
  x = CON_X( render_filename );
  y = CON_Y( render_filename );
  MOVE_CON( render_filename, x+2, y );

  if( !( enable_preview = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                    "Use Preview Object" ) ) )
    throw false;
  PanelTools::PutOnLineWith( preview_filename, enable_preview, 5 );
  SET_INT( enable_preview, false );

  if( !( enable_lod = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                "Use LOD Degrades" ) ) )
    throw false;
  PanelTools::PutOnLineWith( render_filename,   enable_lod,    5 );
  SET_INT( enable_lod, false );

  // Instance-Level LOD Controls
  if( !( lod_distance_multiplier = PERCENT_CTL( panel_funcs->orig_struct, main_panel,
                                                "LOD Distance Modifier" ) ) )
    throw false;

  if( !( lod_use_camera_zoom = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                         "Modify LOD Distances By Camera Zoom" ) ) )
    throw false;

  if( !( lod_nominal_camera_zoom = FLOAT_CTL( panel_funcs->orig_struct, main_panel,
                                              "Nominal Camera Zoom Factor" ) ) )
    throw false;

  PanelTools::PutUnder( render_filename, lod_distance_multiplier );
  PanelTools::PutUnder( lod_distance_multiplier, lod_distance_multiplier );
  PanelTools::PutUnder( lod_distance_multiplier, lod_nominal_camera_zoom );

  PanelTools::PutOnLineWith( lod_use_camera_zoom, lod_nominal_camera_zoom, 20 );

  x = CON_X( lod_nominal_camera_zoom );
  y = CON_Y( lod_distance_multiplier );
  MOVE_CON( lod_distance_multiplier, x - 10, y-2 );
  PanelTools::AlignLabels( lod_distance_multiplier, lod_nominal_camera_zoom );

  SET_FLOAT( lod_distance_multiplier,  100.0f );
  SET_FLOAT( lod_nominal_camera_zoom, -3.2    );
  SET_INT(   lod_use_camera_zoom,      0      );

  // LOD Listbox
  if( !( lod_listbox = AREA_CTL( panel_funcs->orig_struct, main_panel,
                                 "", 366, 88 ) ) )
    throw false;

  if( !( lod_scroll = VSLIDER_CTL( panel_funcs->orig_struct, main_panel,
                                  "", 88, 0, 0 ) ) )
    throw false;

  PanelTools::PutUnder( lod_nominal_camera_zoom, lod_listbox );
  x = CON_X( lod_listbox );
  y = CON_Y( lod_listbox );
  MOVE_CON( lod_listbox, x, y+35 );

  w = CON_W( lod_listbox );
  y = CON_Y( lod_listbox );
  MOVE_CON( lod_scroll, x+w-18, y );


//  if( !( lod_listbox = LISTBOX_CTL( panel_funcs->orig_struct, main_panel,
//                                    "LOD Model List", 363, 6,
//                                    LODListerNames, LODListerCount)) )

  // LOD List Controls
  if( !( lod_add = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                "Add LOD Object", 150 ) ) )
    throw false;

  if( !( lod_remove = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                   "Remove LOD Object", 150 ) ) )
    throw false;

  if( !( lod_clone = WBUTTON_CTL( panel_funcs->orig_struct, main_panel,
                                  "Clone LOD Object", 150 ) ) )
    throw false;

  // LOD Entry Controls
  if( !( lod_filename = FILE_CTL( panel_funcs->orig_struct, main_panel,
                                  "LOD Object", 45 ) ) )
    throw false;

  if( !( lod_enable = BOOL_CTL( panel_funcs->orig_struct, main_panel,
                                "Enable LOD Object" ) ) )
    throw false;

  if( !( lod_distance = DIST_CTL( panel_funcs->orig_struct, main_panel,
                                  "Swap Distance" ) ) )
    throw false;

  SET_STR( lod_filename, "", 0 );
  SET_FLOAT( lod_distance, 10.0 );

  PanelTools::PutOnLineWith( lod_scroll,  lod_add, 3 );
  PanelTools::AlignWith(     lod_add,     lod_remove );
  PanelTools::PutUnder(      lod_add,     lod_remove );
  PanelTools::AlignWith(     lod_remove,  lod_clone  );
  PanelTools::PutUnder(      lod_remove,  lod_clone  );

  PanelTools::PutUnder( lod_listbox, lod_filename );
  x = CON_X( lod_filename );
  y = CON_Y( lod_filename );
  MOVE_CON(  lod_filename, x+20, y+5 );

  PanelTools::PutOnLineWith( lod_filename, lod_enable, 5 );
  PanelTools::PutUnder( lod_filename, lod_distance, 5 );

  // Store In Scene As
  if( !( store_as = WPOPUP_CTL( panel_funcs->orig_struct, main_panel,
                                "Store as", store_as_strings, 150 ) ) )
    throw false;

  if( simp_config.GetSaveInScene() )
    SET_INT( store_as, 1 );
  else
    SET_INT( store_as, 0 );

  PanelTools::PutUnder( lod_distance, store_as, 10 );
  x = CON_X( store_as );
  y = CON_Y( store_as );
  MOVE_CON( store_as, x+325, y+5 );

  // Set the Panel Size
  y = CON_Y( store_as );
  h = CON_H( store_as );
  PAN_SETH( panel_funcs->orig_struct, main_panel, y + h + 30 );

  x = CON_X( store_as );
  w = CON_W( store_as );
  PAN_SETW( panel_funcs->orig_struct, main_panel, x + w );
//  PAN_SETW( panel_funcs->orig_struct, main_panel, 578 );   // Correct size, but doesn't hold when applied

#ifdef DEBUG_PANEL_SIZE
  psize_w = PAN_GETW( panel_funcs->orig_struct, main_panel );
  char buffer[100];
  int pw = PAN_GETW( panel_funcs->orig_struct, main_panel );
  sprintf( buffer, "Original Panel Width:  %d, (Real %d)", x+w, pw );
  simp_int->message->Info( "FreeKey Panel Size Debugger", buffer );
#endif

  // Envent Handler Functions
  CON_SETEVENT( about,                OnAbout,              this );
  CON_SETEVENT( render_filename,      OnRenderFilename,     this );
  CON_SETEVENT( preview_filename,     OnPreviewFilename,    this );
  CON_SETEVENT( load_sdf,             OnLoadSDF,            this );
  CON_SETEVENT( save_sdf,             OnSaveSDF,            this );
  CON_SETEVENT( enable_simplify,      OnEnableSimplify,     this );
  CON_SETEVENT( enable_preview,       OnEnablePreview,      this );
  CON_SETEVENT( enable_lod,           OnEnableLOD,          this );
  CON_SETEVENT( lod_distance_multiplier, OnLODDistanceMultiplier, this );
  CON_SETEVENT( lod_use_camera_zoom,     OnLODUseCameraZoom,      this );
  CON_SETEVENT( lod_nominal_camera_zoom, OnLODNominalCameraZoom,  this );
  CON_SETEVENT( lod_listbox,          OnLODListbox,         this );
  CON_SETEVENT( lod_scroll,           OnLODScroll,          this );
  CON_SETEVENT( lod_add,              OnLODAdd,             this );
  CON_SETEVENT( lod_remove,           OnLODRemove,          this );
  CON_SETEVENT( lod_clone,            OnLODClone,           this );
  CON_SETEVENT( lod_distance,         OnLODDistance,        this );
  CON_SETEVENT( lod_enable,           OnLODEnable,          this );
  CON_SETEVENT( lod_filename,         OnLODFilename,        this );
  CON_SETEVENT( store_as,             OnStoreAs,            this );
  CON_SETEVENT( instance_popup,       OnInstancePopup,      this );
  CON_SETEVENT( f1,                   OnF1,                 this );

  // Draw Functions
  ival.intv.value = (int)&DrawLODListbox;
  (*lod_listbox->set)( lod_listbox, CTL_USERDRAW, &ival );

  // Panel-Level Functions
  panel_funcs->Set( main_panel, PAN_USERDRAW, DrawMainPanel );
  panel_funcs->Set( main_panel, PAN_USERKEYS,   OnKeyDown     );
  panel_funcs->Set( main_panel, PAN_USERKEYUPS, OnKeyUp       );

  return true;
} catch( bool ) {
  return false;
}
}


// OpenMainPanel()
//  Opens the main panel in blocking mode
bool SimplifyInterface::OpenMainPanel() {
#ifdef DEBUG_PANEL_SIZE
  if( update_size ) {
    PAN_SETW( panel_funcs->orig_struct, main_panel, psize_w );
    char buffer[100];
    int pw = PAN_GETW( panel_funcs->orig_struct, main_panel );
    sprintf( buffer, "New Panel Width:  %d, (Real %d)", psize_w, pw );
    simp_int->message->Info( "FreeKey Panel Size Debugger", buffer );
  }
  update_size = false;
#endif

  if( !simpd->TestKey( simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    message->Error( "Simplify is not registered;", "the interface is not available" );
    return false;
  }

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
void SimplifyInterface::CloseMainPanel() {
  panel_funcs->Close( main_panel );
}

// DrawMainPanel():
//  Draw details on the main panel (ie: dividers)
void DrawMainPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = simp_int->panel_funcs->DrawFuncs;
  int ph = PAN_GETH( simp_int->panel_funcs->orig_struct, simp_int->main_panel );
  int pw = PAN_GETW( simp_int->panel_funcs->orig_struct, simp_int->main_panel );

  // Horizontal Dividers
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, 70, pw, 1 );

  int y = CON_Y( simp_int->instance_popup );
  int h = CON_H( simp_int->instance_popup );
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, y+h+1, pw, 1 );

  y = CON_Y( simp_int->save_sdf );
  h = CON_H( simp_int->save_sdf );
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, y+h+2, pw, 1 );

  y = CON_Y( simp_int->render_filename );
  h = CON_H( simp_int->render_filename );
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( simp_int->lod_use_camera_zoom );
  h = CON_H( simp_int->lod_use_camera_zoom );
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, y+h+5, pw, 1 );

  y = CON_Y( simp_int->lod_distance );
  h = CON_H( simp_int->lod_distance );
  PanelDraw::HorizontalDivider( simp_int->main_panel, df, 0, y+h+2, pw, 1 );

  // LOD Model List Title Text
  int x = CON_X( simp_int->lod_listbox );
      y = CON_Y( simp_int->lod_listbox );
  df->WriteText( simp_int->main_panel, "LOD Model List", COLOR_BLACK,
                 x+7, y-14 );

  // Title Image
  logo.DrawInLWPanels( simp_int->main_panel, df, -25, 0 );
}

// MakeAboutPanel()
//  Creates and initializes the About Panel
bool SimplifyInterface::MakeAboutPanel() {
  if( !simpd->TestKey( simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    message->Error( "Simplify is not registered;", "the interface is not available" );
    return false;
  }

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
bool SimplifyInterface::OpenAboutPanel() {
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
bool SimplifyInterface::InitMainPanel( bool initialize ) {
  if( initialize )
    instance.CopyFrom( *orig_instance );

  if( !simpd->TestKey( simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    message->Error( "Simplify is not registered;", "the interface is not available" );
    return false;
  }

  // The #ifdef'ed code is now obsolete.  It is handled instead by the
  //  Simplify_Create function in Simplify_ObjRep.cpp.
#ifdef SIMPLIFY_INIT_OBJECT_NAME_ORIG
  // Fetch this object's name, if not already initialized
  if( instance.GetRender()[0] == '\0' ) {
    // Get the Object Info Functions
    CLWObjectInfo * obj_info = CLWObjectInfo::New( (LWObjectInfo *) Global("LW Object Info", GFUSE_TRANSIENT ) );

    if( !obj_info ) {
      message->Error( "Simplify Error:  Unable to get LW Object Info!" );
      return false;
    }

    // Set the default render filename and zoom factor
    instance.SetRender( obj_info->Filename( instance.GetItemID() ) );
    instance.SetLODNominalCameraZoom( default_zoom_factor );

    // Free obj_info
    delete obj_info;
  }
#endif

  AddContent( &instance );

  // Copy values from the instance into the interface
  SET_STR( render_filename,  (char *)instance.GetRender(),  strlen( instance.GetRender()  ) );
  SET_STR( preview_filename, (char *)instance.GetPreview(), strlen( instance.GetPreview() ) );
  SET_STR( load_sdf,         (char *)instance.GetSDF(),     strlen( instance.GetSDF()     ) );
  SET_STR( save_sdf,         (char *)instance.GetSDF(),     strlen( instance.GetSDF()     ) );

  SET_INT( enable_simplify,  instance.GetEnabled()       );
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

  if( simp_int->instance.GetSDF()[0] != '\0' ) {
    sprintf( simp_int->main_title_bar, "Simplify V%d.%d from TM Productions    ( %s )", Simplify_Version, Simplify_Revision, simp_int->instance.GetSDF() );
    simp_int->panel_funcs->Set( simp_int->main_panel, PAN_TITLE, simp_int->main_title_bar );
  }

  // Figure out what item id this instance matches so we can set the Instance Pop-Up correctly
  for( unsigned int i=0; i < instances.NumElements(); i++ ) {
    if( instances[i]->GetItemID() == instance.GetItemID() )
      SET_INT( instance_popup, i );
  }

  UpdateGhosts();
  return true;
}

// AddContent():
//  Add the content dir to each path in the instance, if needed.
void SimplifyInterface::AddContent( SimplifyInstance *inst ) {
  char new_buffer[ MAX_PATH_LENGTH ];

  if( DirStrings::HasDrive( inst->GetRender() ) == NULL ) {
    strcpy( new_buffer, simp_int->content_dir );
    DirStrings::AddPathPart( new_buffer, inst->GetRender() );
    inst->SetRender( new_buffer );
  }

  if( DirStrings::HasDrive( inst->GetPreview() ) == NULL ) {
    strcpy( new_buffer, simp_int->content_dir );
    DirStrings::AddPathPart( new_buffer, inst->GetPreview() );
    inst->SetPreview( new_buffer );
  }

  if( DirStrings::HasDrive( inst->GetSDF() ) == NULL ) {
    strcpy( new_buffer, simp_int->content_dir );
    DirStrings::AddPathPart( new_buffer, inst->GetSDF() );
    inst->SetSDF( new_buffer );
  }

  for( unsigned int i=1; i < inst->lod.NumElements(); i++ ) {
    if( DirStrings::HasDrive( inst->lod[i]->GetFilename() ) == NULL ) {
      strcpy( new_buffer, simp_int->content_dir );
      DirStrings::AddPathPart( new_buffer, inst->lod[i]->GetFilename() );
      inst->lod[i]->SetFilename( new_buffer );
    }
  }
}

// TestInstance():
//  Tests to make sure the new instance data is valid before
//   copying it back into the original instance.
void SimplifyInterface::TestInstance( SimplifyInstance *output_inst ) {
  if( (instance.GetStoreAs() == 0) &&
      (instance.GetSDF()[0] == '\0') ) {
    message->Error("Simplify Error:  You must save a .SDF to use",
                   "\"Store As Filename of .SDF in .LWS\" mode!" );
  }

  // Strip off the content dir from the paths, if needed
  StripContent( &instance );

  // Copy the new instance into the original instance
  output_inst->CopyFrom( instance );   
}

// StripContent():
//  Removes the content dir from all paths in an instance,
//   if needed.
void SimplifyInterface::StripContent( SimplifyInstance *inst ) {
  int len = strlen( content_dir );
  if( strnicmp( inst->GetRender(), content_dir, len ) == 0 )
    inst->SetRender( &inst->GetRender()[len] );

  if( strnicmp( inst->GetPreview(), content_dir, len ) == 0 )
    inst->SetPreview( &inst->GetPreview()[len] );

  if( strnicmp( inst->GetSDF(), content_dir, len ) == 0 )
    inst->SetSDF( &inst->GetSDF()[len] );

  for( unsigned int i=1; i < inst->lod.NumElements(); i++ ) {
    if( strnicmp( inst->lod[i]->GetFilename(), content_dir, len ) == 0 )
      inst->lod[i]->SetFilename( &inst->lod[i]->GetFilename()[len] );
  }    
}

// OnAbout():
//  Called to open the About Box.
void OnAbout( LWControl *ctl, SimplifyInterface *panel ) {
  if( !simp_int->simpd->TestKey( simp_int->simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    simp_int->message->Error( "Simplify is not registered;", "the interface is not available" );
    return;
  }

  panel->OpenAboutPanel();
}

// DrawAbout():
//  Called to draw the About box
void DrawAboutPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = simp_int->panel_funcs->DrawFuncs;

  logo.DrawInLWPanels( simp_int->about_panel, df, -220, 0 );

  int ph = PAN_GETH( simp_int->panel_funcs->orig_struct, simp_int->about_panel );
  int pw = PAN_GETW( simp_int->panel_funcs->orig_struct, simp_int->about_panel );

  int y = 70;

  char buffer[256];
  sprintf( buffer, "%s V%d.%d", about_text[0], Simplify_Version, Simplify_Revision );
  df->WriteText( simp_int->about_panel, buffer, COLOR_LT_YELLOW,
                 (pw/2)-(df->TextWidth(simp_int->about_panel, buffer))/2,
                  y+8 );

  y += 35;
  for( int i=1; about_text[i] != NULL; i++ ) {
    df->WriteText( simp_int->about_panel, about_text[i], COLOR_BLACK,
             (pw/2)-(df->TextWidth(simp_int->about_panel, about_text[i] ))/2,
             y );
    y += 15;
  }
}

// LOD Lister Functions
char *LODListerNames( void *dat, int index ) {
	if( index >= (int)simp_int->instance.lod.NumElements() )
		return NULL;

	return (char *)simp_int->instance.lod[ index ]->GetListerName();
}

int LODListerCount( void *dat ) {
  return simp_int->instance.lod.NumElements();
}

// Instance Popup Functions
char *InstanceNames( void *dat, int index ) {
	if( index >= (int)instances.NumElements() )
		return NULL;

  char * name = DirStrings::HasFile( instances[ index ]->GetRender() );
  if( name == NULL ) 
    return " <render object unavailable>";
  else
    return name;
}

int InstanceCount( void *dat ) {
  return instances.NumElements();
}

// OnLODListbox():
//  Handles changing with LOD we're editing.
void OnLODListbox( LWControl *ctl, SimplifyInterface *panel ) {
  // Figure out which item was clicked
  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int mx = CON_MOUSEX( ctl );
  int my = CON_MOUSEY( ctl );

  int index = (my / item_height) + simp_int->lod_top;
  
  if( index < (int)simp_int->instance.lod.NumElements() ) {
    simp_int->selected_lod = index;

    // Test for an Enable/Disable click
    int cx = CON_HOTX( ctl );
    int mx = CON_MOUSEX( ctl );

    if( mx < 15 ) {
      simp_int->instance.lod[ simp_int->selected_lod ]->SetEnabled( !simp_int->instance.lod[ simp_int->selected_lod ]->GetEnabled() );
      simp_int->instance.lod[ simp_int->selected_lod ]->GenerateListerName();
      simp_int->instance.SetSavedState( false );
    }

    REDRAW_CON( simp_int->lod_listbox );
    simp_int->UpdateLOD( simp_int->selected_lod );
  }
}

// OnLODScroll():
//  Handles adjusting the visible items via the scroll bar.
void OnLODScroll( LWControl *ctl, SimplifyInterface *panel ) {
  GET_INT( ctl, simp_int->lod_top );
  REDRAW_CON( simp_int->lod_listbox );
}

// DrawLODListbox():
//  Called to draw the Palette Control
void DrawLODListbox( LWControl *ctl, SimplifyInterface *si, DrMode mode ) {
  CDrawFuncs *df = simp_int->panel_funcs->DrawFuncs;

  int cx = CON_HOTX( ctl );
  int cy = CON_HOTY( ctl );
  int ch = CON_HOTH( ctl );
  int cw = CON_HOTW( ctl );

  int item_text_color;

  // Clear the control
  df->DrawRGBBox( simp_int->main_panel, 132, 130, 132, cx, cy, cw, ch );

  // Rows
  for( int j=0; j < ch/item_height; j++ ) {
    df->DrawRGBBox( simp_int->main_panel, 112, 112, 112,
                    cx, cy+(j*item_height), cw, 1 );
  }

  // Main Columns
  df->DrawRGBBox( simp_int->main_panel, 0, 0, 0,
                  cx+item_text_offset-5, cy, 1, ch );

  df->DrawRGBBox( simp_int->main_panel, 0, 0, 0,
                  cx+item_dist_offset-5, cy, 1, ch );

  // Items in the list
  int   baseline;                                // Base of the current line
  char  name_buffer[1024];

  for( unsigned int i=simp_int->lod_top; i < simp_int->instance.lod.NumElements(); i++ ) {
    baseline = cy + ((i - simp_int->lod_top) * item_height );

    // Figure out what color the text should be drawn in
    ctl->get( ctl, CTL_FLAGS, &ival );
    if( (mode == DR_GHOST) || ( ival.intv.value & CTLF_GHOST) )
      item_text_color = i == simp_int->selected_lod ? COLOR_DK_YELLOW : COLOR_LT_GREY;
    else
      item_text_color = i == simp_int->selected_lod ? COLOR_LT_YELLOW : COLOR_WHITE;

    // Make sure the name fits in the lister
    DirStrings::GetFilePart( name_buffer, (char *)simp_int->instance.lod[i]->GetFilename() );
    if( name_buffer[0] == '\0' )
      strcpy( name_buffer, "<No Object Selected>" );
    int len = strlen( name_buffer );
    while( df->TextWidth( simp_int->main_panel, name_buffer ) > (cw - item_text_offset - (cw - item_dist_offset - 15) ) )
      name_buffer[--len] = '\0';

    // Display the item name
    df->WriteText( simp_int->main_panel, name_buffer,
                   item_text_color, cx+item_text_offset, baseline );

    // Display the distance
    sprintf( name_buffer, "%gm", simp_int->instance.lod[i]->GetDistance() );
    df->WriteText( simp_int->main_panel, name_buffer,
                   item_text_color, cx+item_dist_offset, baseline );

    // Enable Checkmark Icons
    if( simp_int->instance.lod[i]->GetEnabled() ) {
      PanelDraw::DrawLWIcon( simp_int->main_panel, df,
                             cx + 1, baseline + 1,
                             PanelDraw::check, PanelDraw::grey );
    }

    // Check for the end of the list
    if( ch/item_height < (int)i - simp_int->lod_top + 2 )
      break;
  }
}

// OnRenderFilename():
//  Called when the Render Filename changes.
void OnRenderFilename( LWControl *ctl, SimplifyInterface *panel ) {
  char buffer[1024];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  // Handle no path entered (restore the old path)
  if( buffer[0] == '\0' ) {
    SET_STR( ctl, (char *)simp_int->instance.GetRender(), strlen( simp_int->instance.GetRender() ) );
    return;
  }

  // Check for a valid filename
  if( DirStrings::HasFile( buffer ) == NULL ) {
    simp_int->message->Error("Simplify Error:  This is not a valid render object:", buffer);
    return;
  }

  simp_int->instance.SetSavedState( false );

  // Filename valid; use it in this instance
  // Tack on the Content Dir, if needed
  if( DirStrings::HasDrive( buffer ) == NULL ) {
    char new_buf[ MAX_PATH_LENGTH ];
    strcpy( new_buf, simp_int->content_dir );
    DirStrings::AddPathPart( new_buf, buffer );

    simp_int->instance.SetRender( new_buf );
    SET_STR( ctl, (char *)simp_int->instance.GetRender(), strlen( simp_int->instance.GetRender() ) );
  } else
    simp_int->instance.SetRender( buffer );
}

// OnPreviewFilename():
//  Called when the Preview Filename changes.
void OnPreviewFilename( LWControl *ctl, SimplifyInterface *panel ) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  // Handle no path entered (restore the old path)
  if( buffer[0] == '\0' ) {
    SET_STR( ctl, (char *)simp_int->instance.GetPreview(), strlen( simp_int->instance.GetPreview() ) );
    if( simp_int->instance.GetPreview()[0] == '\0' ) {   // Still an Invalid path; disable preview
      SET_INT( simp_int->enable_preview, (int)false );
      simp_int->instance.SetEnablePreview( false );
    }

    return;
  }

  // Check for a valid filename
  if( DirStrings::HasFile( buffer ) == NULL ) {
    simp_int->message->Error("Simplify Error:  This is not a valid preview object:", buffer);

    SET_STR( ctl, (char *)simp_int->instance.GetPreview(), strlen( simp_int->instance.GetPreview() ) );
    if( simp_int->instance.GetPreview()[0] == '\0' ) {   // Still an Invalid path; disable preview
      SET_INT( simp_int->enable_preview, (int)false );
      simp_int->instance.SetEnablePreview( false );
    }
    return;
  }

  simp_int->instance.SetSavedState( false );

  // Tack on the Content Dir
  if( DirStrings::HasDrive( buffer ) == NULL ) {
    char new_buf[ MAX_PATH_LENGTH ];
    strcpy( new_buf, simp_int->content_dir );
    DirStrings::AddPathPart( new_buf, buffer );

    simp_int->instance.SetPreview( new_buf );
    SET_STR( ctl, (char *)simp_int->instance.GetPreview(), strlen( simp_int->instance.GetPreview() ) );
  } else {
    simp_int->instance.SetPreview( buffer );
  }
}

// OnLoadSDF():
//  Loads an SDF into this instance.
void OnLoadSDF( LWControl *ctl, SimplifyInterface *panel ) {
  if( !simp_int->simpd->TestKey( simp_int->simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    simp_int->message->Error( "Simplify is not registered;", "the interface is not available" );
    return;
  }

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  // User hit cancel; nothing to do
  if( buffer[0] == '\0' )
    return;

  char * error;
  SimplifyInstance new_inst;
  if( new_inst.LoadFromFile( buffer, error ) ) {
    new_inst.SetItemID( simp_int->instance.GetItemID() );  // Set the ItemID for the new instance, since it will be copied back into the old instance
    simp_int->instance.CopyFrom( new_inst );
    simp_int->instance.SetSDF( buffer );
    simp_int->InitMainPanel( false );
    simp_int->message->Info( "Simplify:  Sucessfully loaded settings from", simp_int->instance.GetSDF() );
    simp_int->instance.SetSavedState( true );

    if( simp_int->instance.GetLODUseCameraZoom() )
      UNGHOST_CON( simp_int->lod_nominal_camera_zoom );
    else
      GHOST_CON( simp_int->lod_nominal_camera_zoom );
  } else
    simp_int->message->Info( error, buffer );

  if( simp_int->instance.GetSDF()[0] != '\0' ) {
    sprintf( simp_int->main_title_bar, "Simplify V%d.%d from TM Productions    ( %s )", Simplify_Version, Simplify_Revision, simp_int->instance.GetSDF() );
    simp_int->panel_funcs->Set( simp_int->main_panel, PAN_TITLE, simp_int->main_title_bar );
  }
}

// OnSaveSDF():
//  Saves an SDF into this instance.
void OnSaveSDF( LWControl *ctl, SimplifyInterface *panel ) {
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  if( !simp_int->simpd->TestKey( simp_int->simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    simp_int->message->Error( "Simplify is not registered;", "the interface is not available" );
    return;
  }

  // User hit cancel; nothing to do
  if( buffer[0] == '\0' )
    return;

  DirStrings::ChangeExtension( buffer, ".SDF" );

  // First clone the instance and strip off the Content Dir
  SimplifyInstance inst;
  inst.CopyFrom( simp_int->instance );
  simp_int->StripContent( &inst );

  // Save the new instance
  char * error;
  if( inst.SaveToFile( buffer, error ) )
    simp_int->message->Info(  "Simplify:  Sucessfully SavedState settings into", simp_int->instance.GetSDF() );
  else
    simp_int->message->Error( error, simp_int->instance.GetSDF() );

  // Set the SDF path in the interface, and add the content dir, if needed
  if( DirStrings::HasDrive( buffer ) == NULL ) {
    char new_buf[ MAX_PATH_LENGTH ];
    strcpy( new_buf, simp_int->content_dir );
    DirStrings::AddPathPart( new_buf, buffer );
    simp_int->instance.SetSDF( new_buf );
    simp_int->instance.SetSavedState( true );
  } else {
    simp_int->instance.SetSDF( buffer );
  }

  SET_STR( ctl,                (char *)simp_int->instance.GetSDF(), strlen( simp_int->instance.GetSDF() ) );
  SET_STR( simp_int->load_sdf, (char *)simp_int->instance.GetSDF(), strlen( simp_int->instance.GetSDF() ) );

  if( simp_int->instance.GetSDF()[0] != '\0' ) {
    sprintf( simp_int->main_title_bar, "Simplify V%d.%d from TM Productions    ( %s )", Simplify_Version, Simplify_Revision, simp_int->instance.GetSDF() );
    simp_int->panel_funcs->Set( simp_int->main_panel, PAN_TITLE, simp_int->main_title_bar );
  }
}

// OnEnableSimplify():
//  Called when the Enable button is toggled
void OnEnableSimplify( LWControl *ctl, SimplifyInterface *panel ) {
  int dummy;
  GET_INT( ctl, dummy );
  simp_int->instance.SetEnabled( (bool)dummy );

  simp_int->instance.SetSavedState( false );
  UpdateGhosts();
}

// OnEnablePreview():
//  Called when the Enable button is toggled
void OnEnablePreview( LWControl *ctl, SimplifyInterface *panel ) {
  int dummy;
  GET_INT( ctl, dummy );

  if( !simp_int->simpd->TestKey( simp_int->simpd->ReadKey(), simp_int->simpd->GetDongleID() ) ) {
    simp_int->message->Error( "Simplify is not registered;", "the interface is not available" );
    return;
  }

  // Only enable the preview if the Preview Path is valid
  if( simp_int->instance.GetPreview()[0] == '\0' ) {
    simp_int->message->Info("You must enter a valid Preview Object to use this feature");
    SET_INT( simp_int->enable_preview, (int)false );
    simp_int->instance.SetEnablePreview( false );
    return;
  }

  simp_int->instance.SetEnablePreview( (bool)dummy );

  simp_int->instance.SetSavedState( false );
  UpdateGhosts();
}

// OnEnableLOD():
//  Called when the Enable LOD button is toggled
void OnEnableLOD( LWControl *ctl, SimplifyInterface *panel ) {
  int dummy;
  GET_INT( ctl, dummy );
  simp_int->instance.SetEnableLOD( (bool)dummy );

  simp_int->instance.SetSavedState( false );
  UpdateGhosts();
}

// OnStoreAs():
//  Called when the Store As mode is changed
void OnStoreAs( LWControl *ctl, SimplifyInterface *panel ) {
  int dummy;
  GET_INT( ctl, dummy );
  simp_int->instance.SetStoreAs( dummy );

  if( simp_int->instance.GetStoreAs() == 0 ) {
    if( simp_int->instance.GetSDF()[0] == '\0' ) {
      simp_int->message->Info( "Simplify:  You must save the .SDF before saving a scene",
                               "to use this Store As mode!" );
    } else if( !simp_int->instance.GetSavedState() ) {
      simp_int->message->Info( "Simplify:  To use this Store As mode with these settings,",
                               "be sure to re-save your .SDF file!" );
    }
  }
}

// OnInstancePopup():
//  Handles changing which instance we are editing.
void OnInstancePopup( LWControl *ctl, SimplifyInterface *panel ) {
  int index;
  GET_INT( ctl, index );
  
  if( (index > -1) && (index < (int)instances.NumElements()) ) {
    simp_int->TestInstance( simp_int->orig_instance );
    simp_int->orig_instance = instances[ index ];
    simp_int->InitMainPanel(true);
  }
}

// OnLODDistanceMultiplier():
//  Handles changing LOD Distance Modifier.
void OnLODDistanceMultiplier( LWControl *ctl, SimplifyInterface *panel ) {
  double value;
  GET_FLOAT( simp_int->lod_distance_multiplier, value );
  value = fabs( value );
  simp_int->instance.SetLODDistanceMultiplier( ((float)value)/100.0f );  // Change it so 100% == 1.0
  SET_FLOAT( simp_int->lod_distance_multiplier, 100.05f );               // Have to do this so the gadget updates with a % symbol
  SET_FLOAT( simp_int->lod_distance_multiplier, (simp_int->instance.GetLODDistanceMultiplier() * 100.0f) );
}

// OnLODUseCameraZoom():
//  Handles toggling the Use Camera Zoom option.
void OnLODUseCameraZoom( LWControl *ctl, SimplifyInterface *panel ) {
  bool state;
  GET_INT( simp_int->lod_use_camera_zoom, state );
  simp_int->instance.SetLODUseCameraZoom( state );

  if( simp_int->instance.GetLODUseCameraZoom() )
    UNGHOST_CON( simp_int->lod_nominal_camera_zoom );
  else
    GHOST_CON( simp_int->lod_nominal_camera_zoom );
}

// OnLODNominalCameraZoom():
//  Handles changing the Nominal Camera Zoom option.
void OnLODNominalCameraZoom( LWControl *ctl, SimplifyInterface *panel ) {
  double value;
  GET_FLOAT( simp_int->lod_nominal_camera_zoom, value );
  value = fabs( value );
  simp_int->instance.SetLODNominalCameraZoom( (float)value );
  SET_FLOAT( simp_int->lod_nominal_camera_zoom, simp_int->instance.GetLODNominalCameraZoom() );
}

// OnLODAdd():
//  Adds a new LOD.
void OnLODAdd( LWControl *ctl, SimplifyInterface *panel ) {
  // Create a new LOD instance
  SimplifyLOD * new_lod = new SimplifyLOD;
  new_lod->SetDistance( simp_int->instance.lod[ simp_int->instance.lod.NumElements() - 1 ]->GetDistance() + 5.0f );  // Set the new distance to the furthest instance + 5
  simp_int->instance.lod.Add( new_lod );

  // Set the new instance in the LOD list
  simp_int->selected_lod = simp_int->instance.lod.NumElements() - 1;

  // Scroll the list, if needed
  int ch = CON_HOTH( simp_int->lod_listbox );
  unsigned int index = ( ch / item_height) + simp_int->lod_top;
  if( simp_int->selected_lod >= index )
    simp_int->lod_top++;

  // Update the controls
  ival.intv.value = simp_int->lod_top;
  simp_int->lod_scroll->set( simp_int->lod_scroll, CTL_RANGEMAX, &ival );
  SET_INT( simp_int->lod_scroll, simp_int->lod_top );

  REDRAW_CON( simp_int->lod_scroll );
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->UpdateLOD( simp_int->selected_lod );

  simp_int->instance.SetSavedState( false );
}

// OnLODRemove():
//  Removes an existing LOD.
void OnLODRemove( LWControl *ctl, SimplifyInterface *panel ) {
  if( simp_int->selected_lod == 0 ) {
    simp_int->message->Error( "Simplify Error:  You can't delete the default LOD object.",
                              "You can, however, disable it with the \"Enable\" checkbox." );
    return;
  }

  delete simp_int->instance.lod[ simp_int->selected_lod ];
  simp_int->instance.lod.Remove( simp_int->selected_lod );

  if( simp_int->selected_lod >= simp_int->instance.lod.NumElements() )
    simp_int->selected_lod--;

  // Scroll the list, if needed
  if( simp_int->selected_lod < (unsigned int)simp_int->lod_top )
    simp_int->lod_top = simp_int->selected_lod;

  // Update the controls
  ival.intv.value = simp_int->lod_top;
  simp_int->lod_scroll->set( simp_int->lod_scroll, CTL_RANGEMAX, &ival );
  SET_INT( simp_int->lod_scroll, simp_int->lod_top );

  REDRAW_CON( simp_int->lod_scroll );
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->UpdateLOD( simp_int->selected_lod );

  simp_int->instance.SetSavedState( false );
}

// OnLODClone():
//  Clones an existing LOD.
void OnLODClone( LWControl *ctl, SimplifyInterface *panel ) {
  // Create a new LOD instance
  SimplifyLOD * new_lod = new SimplifyLOD;
  new_lod->SetDistance( simp_int->instance.lod[ simp_int->instance.lod.NumElements() - 1 ]->GetDistance() + 5.0f );  // Set the new distance to the furthest instance + 5

  // Copy the filename
  if( simp_int->selected_lod == 0 )
    new_lod->SetFilename( simp_int->instance.GetRender() );
  else
    new_lod->SetFilename( simp_int->instance.lod[ simp_int->selected_lod ]->GetFilename() );

  // Generate a middle distance
  if( simp_int->selected_lod == simp_int->instance.lod.NumElements()-1 ) {
    new_lod->SetDistance( simp_int->instance.lod[ simp_int->selected_lod ]->GetDistance() + 5.0f );
    simp_int->instance.lod.Add( new_lod );
  } else {
    new_lod->SetDistance( simp_int->instance.lod[ simp_int->selected_lod ]->GetDistance() +
                          ( ( simp_int->instance.lod[ simp_int->selected_lod + 1]->GetDistance() -
                            simp_int->instance.lod[ simp_int->selected_lod ]->GetDistance() ) / 2 ) );
    simp_int->instance.lod.InsertAt( simp_int->selected_lod + 1, new_lod );
  }

  // Set the new instance in the LOD list
  SET_INT( simp_int->lod_listbox, ++simp_int->selected_lod );
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->UpdateLOD( simp_int->selected_lod );

  simp_int->instance.SetSavedState( false );
}


// OnLODEnable():
//  Toggles an LOD's Enabled state.
void OnLODEnable( LWControl *ctl, SimplifyInterface *panel ) {
  SimplifyLOD *lod = simp_int->instance.lod[ simp_int->selected_lod ];

  int state;
  GET_INT( ctl, state );

  // Handle no path entered (restore the old path)
  if( lod->GetFilename()[0] == '\0' ) {
    SET_STR( ctl, (char *)lod->GetFilename(), strlen( lod->GetFilename() ) );
    if( lod->GetFilename()[0] == '\0' ) {   // Still an Invalid path; disable preview
      simp_int->message->Info("You must enter a valid LOD Object before enabling it");
      SET_INT( simp_int->lod_enable, (int)false );
      lod->SetEnabled( false );
    }

    return;
  }

  lod->SetEnabled( (bool) state );

  // Refresh the LOD Listbox
  lod->GenerateListerName();
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->instance.SetSavedState( false );
}

// OnLODDistance():
//  Changes an LOD's swap distance and re-sorts the array
void OnLODDistance( LWControl *ctl, SimplifyInterface *panel ) {
  double dist;
  GET_FLOAT( ctl, dist );

  // Check for negative distances
  if( dist < 0.0 ) {
    simp_int->message->Error("Simplify Error:  The Swap Distance must be greater than 0!");
    SET_FLOAT( ctl, simp_int->instance.lod[ simp_int->selected_lod ]->GetDistance() );
    return;
  }

  // Set the new value
  double old_dist = simp_int->instance.lod[ simp_int->selected_lod ]->GetDistance();
  simp_int->instance.lod[ simp_int->selected_lod ]->SetDistance( (float)dist );


  // NOTE: The code enclosed withing DISTANCE_INTERNAL is obsolete code.  It works,
  //       but only in the limited scope of this element.  Use the QSort function
  //       instead.
#ifdef DISTANCE_INTERNAL
  // Sort the list by swap distance
  if( dist > old_dist ) {
    // Shift further from the default
    for( unsigned int i = simp_int->selected_lod + 1; i < simp_int->instance.lod.NumElements(); i++ ) {
      if( simp_int->instance.lod[ i ]->GetDistance() > dist )
        break;
    }

    if( simp_int->instance.lod.NumElements() == i ) {
      simp_int->instance.lod.MoveTo( simp_int->instance.lod.NumElements()-1, simp_int->selected_lod );
      simp_int->selected_lod = simp_int->instance.lod.NumElements()-1;
    } else {
      simp_int->instance.lod.MoveTo( i, simp_int->selected_lod );
      simp_int->selected_lod = i;
    }
  } else if( dist < old_dist ) {
    // Shift closer to the default
    for( unsigned int i = simp_int->selected_lod - 1; i > 0; i-- ) {
      if( simp_int->instance.lod[ i ]->GetDistance() < dist ) {
        break;
      }
    }

    if( i < 1 ) {
      simp_int->instance.lod.MoveTo( 1, simp_int->selected_lod );
      simp_int->selected_lod = 1;
    } else {
      if( i+ > simp_int->instance.lod.NumElements() )
        ;  // This is still more extreme than the last element; do nothing
      simp_int->instance.lod.MoveTo( i+1, simp_int->selected_lod );
      simp_int->selected_lod = i;
    }
  } else {
    ;  // Distances equal; don't displace
  }
#else
  SimplifyLOD *temp = simp_int->instance.lod[ simp_int->selected_lod ];
  simp_int->instance.lod.QSort( SortLOD );
  simp_int->selected_lod = simp_int->instance.lod.FindIndexOf( temp );
#endif

  // Update the list
  // simp_int->instance.lod[ simp_int->selected_lod ]->GenerateListerName();
  SET_INT( simp_int->lod_listbox, simp_int->selected_lod );
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->instance.SetSavedState( false );
}

// OnLODFilename():
//  Changes an LOD's object filename.
void OnLODFilename( LWControl *ctl, SimplifyInterface *panel ) {
  SimplifyLOD *lod = simp_int->instance.lod[ simp_int->selected_lod ];

  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( ctl, buffer, MAX_PATH_LENGTH );

  // Handle no path entered (restore the old path)
  if( buffer[0] == '\0' ) {
    SET_STR( ctl, (char *)lod->GetFilename(), strlen( lod->GetFilename() ) );
    if( lod->GetFilename()[0] == '\0' ) {   // Still an Invalid path; disable preview
      SET_INT( simp_int->lod_enable, (int)false );
      lod->SetEnabled( false );
    }

    return;
  }

  // Check for a valid filename
  if( DirStrings::HasFile( buffer ) == NULL ) {
    simp_int->message->Error("Simplify Error:  This is not a valid preview object:", buffer);

    SET_STR( ctl, (char *)lod->GetFilename(), strlen( lod->GetFilename() ) );
    if( lod->GetFilename()[0] == '\0' ) {                   // Still an Invalid path; disable preview
      SET_INT( simp_int->lod_enable, (int)false );
      lod->SetEnabled( false );
    }

    return;
  }

  // Tack on the Content Dir, if needed
  if( DirStrings::HasDrive( buffer ) == NULL ) {
    char new_buf[ MAX_PATH_LENGTH ];
    strcpy( new_buf, simp_int->content_dir );
    DirStrings::AddPathPart( new_buf, buffer );

    lod->SetFilename( new_buf );
    SET_STR( ctl, (char *)lod->GetFilename(), strlen( lod->GetFilename() ) );
  } else
    lod->SetFilename( buffer );

  // Refresh the LOD Listbox
  simp_int->instance.lod[ simp_int->selected_lod ]->GenerateListerName();
  REDRAW_CON( simp_int->lod_listbox );

  simp_int->instance.SetSavedState( false );
}

// UpdateLOD():
//  Updates the LOD controls with the index provided into
//   the instance->lod array.
void SimplifyInterface::UpdateLOD( int index ) {
  SET_STR(   lod_filename, (char *)instance.lod[index]->GetFilename(), strlen( instance.GetSDF() ) );
  SET_INT(   lod_enable,   (int)instance.lod[index]->GetEnabled() );
  SET_FLOAT( lod_distance, instance.lod[index]->GetDistance() );

  // Ghost uneditable controls if the default instance is edited
  if( (index < 1) || (!simp_int->instance.GetEnabled() && !simp_int->instance.GetEnableLOD()) ) {
    GHOST_CON( lod_distance );
    GHOST_CON( lod_filename );
  } else {
    UNGHOST_CON( lod_distance );
    UNGHOST_CON( lod_filename );
  }
}

// SortLOD():
//  qsort() function to sort the LOD array by swap distance
int (__cdecl SortLOD)( const void *arg1, const void *arg2 ) {
  SimplifyLOD * first  = *((SimplifyLOD **) arg1);
  SimplifyLOD * second = *((SimplifyLOD **) arg2);

  // Special Case:  Test for the default instance
  if( first->GetDistance() == 0.0f ) {
    if( strcmp( first->GetFilename(), " <Default Render Object> " ) == 0 )
      return -1;
  }

  if( second->GetDistance() == 0.0f ) {
    if( strcmp( second->GetFilename(), " <Default Render Object> " ) == 0 )
      return 1;
  }

  // Standard case
  if( first->GetDistance() > second->GetDistance() )
    return 1;
  else
    return -1;
}

// UpdateGhosts():
//  Updates the state of items based on what is and isn't enabled
void UpdateGhosts() {
  if( simp_int->instance.GetEnabled() ) {
    UNGHOST_CON( simp_int->preview_filename );
    UNGHOST_CON( simp_int->enable_preview );
    UNGHOST_CON( simp_int->enable_lod );
    UNGHOST_CON( simp_int->render_filename );
  } else {
    GHOST_CON( simp_int->preview_filename );
    GHOST_CON( simp_int->enable_preview );
    GHOST_CON( simp_int->enable_lod );
    GHOST_CON( simp_int->render_filename );
  }

//  Removed due to checks in OnPreview() that disable the Enable Preview
//   button if you haven't entered a valid name (ie: endless loop)
//  if( simp_int->instance.GetEnablePreview() && simp_int->instance.GetEnabled() )
//    UNGHOST_CON( simp_int->preview_filename );
//  else
//    GHOST_CON( simp_int->preview_filename );

  if( simp_int->instance.GetEnableLOD() && simp_int->instance.GetEnabled() ) {
    UNGHOST_CON( simp_int->lod_listbox );
    UNGHOST_CON( simp_int->lod_add );
    UNGHOST_CON( simp_int->lod_remove );
    UNGHOST_CON( simp_int->lod_clone );
    UNGHOST_CON( simp_int->lod_enable );
    UNGHOST_CON( simp_int->lod_distance_multiplier );
    UNGHOST_CON( simp_int->lod_use_camera_zoom );
    if( simp_int->instance.GetLODUseCameraZoom() )
      UNGHOST_CON( simp_int->lod_nominal_camera_zoom );
    else
      GHOST_CON( simp_int->lod_nominal_camera_zoom );
  } else {
    GHOST_CON( simp_int->lod_listbox );
    GHOST_CON( simp_int->lod_add );
    GHOST_CON( simp_int->lod_remove );
    GHOST_CON( simp_int->lod_clone );
    GHOST_CON( simp_int->lod_enable );
    GHOST_CON( simp_int->lod_distance_multiplier );
    GHOST_CON( simp_int->lod_use_camera_zoom );
    GHOST_CON( simp_int->lod_nominal_camera_zoom );
  }

  // Ghost uneditable controls if the default instance is edited
  if( simp_int->instance.GetEnabled() ) {
    if( (simp_int->selected_lod < 1) ) {
      GHOST_CON( simp_int->lod_distance );
      GHOST_CON( simp_int->lod_filename );
    } else {
      UNGHOST_CON( simp_int->lod_distance );
      UNGHOST_CON( simp_int->lod_filename );
    }
  } else {
    GHOST_CON( simp_int->lod_distance );
    GHOST_CON( simp_int->lod_filename );
  }
}
