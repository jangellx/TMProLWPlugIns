//
// IllumiSurf-Interface.cpp
//

#include "IllumiSurf-Interface.h"

#include <lightwave/lwaboutpanel/lwaboutpanel.h>
#include <lightwave/lwaboutpanel/logos/pietoolslogo.h>

#include <stdio.h>

const char *about_text[] = {
  LWABOUT_YELLOW,
  "IllumiSurf",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Surface variance through lighting",
  "for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright ©2001 PiE Tools",
  LWABOUT_HDIV,
  "Written by Joe Angell, PiE Tools",
  "http://www.pietools.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "Developed for Savage Frog",
  "http://www.savagefrog.com",
  NULL };

const char *paste_popup_strings[] = {
  "Paste into all instances in same object",
  "Paste into all instances",
  NULL };

// LWPanels Macro Stuff
/*
LWPanControlDesc desc;
LWValue ival    = {LWT_INTEGER};
LWValue ivecval = {LWT_VINT};
LWValue fval    = {LWT_FLOAT};
LWValue fvecval = {LWT_VFLOAT};
LWValue sval    = {LWT_STRING};
*/

IllumiSurf_Interface *is_int = NULL;

// Constructor
IllumiSurf_Interface::IllumiSurf_Interface( IllumiSurf_Instance *_inst ) 
  : inst(_inst), is_open(false), creation_ok(true), panel(NULL), main_xpanel(NULL) {

  assert( is_int == NULL );
  is_int = this;

  message        = (LWMessageFuncs      *)global( LWMESSAGEFUNCS_GLOBAL,      GFUSE_ACQUIRE );
  panel_funcs    = (LWPanelFuncs        *)global( LWPANELFUNCS_GLOBAL,        GFUSE_ACQUIRE );
  xpanels        = (LWXPanelFuncs       *)global( LWXPANELFUNCS_GLOBAL,       GFUSE_ACQUIRE );

  panel_tools    = (TMP_PanelTools      *)global( TMP_PANELTOOLS_GLOBAL,      GFUSE_ACQUIRE );
  panel_controls = (TMP_PanelControls   *)global( TMP_PANELCONTROLS_GLOBAL,   GFUSE_ACQUIRE );
  panel_draw     = (TMP_PanelDraw       *)global( TMP_PANELDRAW_GLOBAL,       GFUSE_ACQUIRE );

  if( !panel_tools || !panel_draw || !panel_controls ) {
    message->error( "IllumiSurf Error:  Unable to find the PanelTools globals, which are required for the interface",
                    "Try adding the plug-in PanelTools.p and try again" );
    creation_ok = false;
    return;
  }

  CreateUI();
}

// Destructor
IllumiSurf_Interface::~IllumiSurf_Interface() {
  // Destroy the panel
  if( panel_funcs != NULL ) {
    if( panel != NULL ) {
      (*panel_funcs->destroy)( panel );
      panel = NULL;
    }
  }

  // Destroy the xpanels
  if( xpanels != NULL ) {
    if( main_xpanel != NULL )
      xpanels->destroy( main_xpanel );
  }

  is_int = NULL;
}

// CreateUI():
void IllumiSurf_Interface::CreateUI() {
  creation_ok = false;

  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  panel = (*panel_funcs->create)( "IllumiSurf", panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return;
  }

  // Instance Popup
  instance_popup = CUSTPOPUP_CTL( panel_funcs, panel, "Instances", 250, InstanceName, InstanceCount );

  // Main XPanel Control
  main_xpanel = xpanels->create( LWXP_VIEW, main_controls );
  xpanels->describe( main_xpanel, main_data, ISXPanel_Get, ISXPanel_Set );
  xpanels->hint(     main_xpanel, 0, main_hint );
  xpanels->viewInst( main_xpanel, inst );

  main_control = XPANEL_CTL( panel_funcs, panel, "", main_xpanel );

  // Copy/Paste Buttons
  copy_button  = WBUTTON_CTL( panel_funcs, panel, "Copy",  40 );
  paste_button = WBUTTON_CTL( panel_funcs, panel, "Paste", 40 );
  panel_tools->putOnLineWith( copy_button, paste_button, -5 );

  paste_popup = WPOPUP_CTL( panel_funcs, panel, "", paste_popup_strings, 18 );
  panel_tools->putOnLineWith( paste_button, paste_popup, -15 );

  if( copy_inst == NULL ) {
    GHOST_CON( ((LWControl *)paste_button) );
    GHOST_CON( ((LWControl *)paste_popup)  );
  }

  // About Button
  about_button  = WBUTTON_CTL( panel_funcs, panel, "About", 40 );
  panel_tools->putOnLineWith( paste_button, about_button, 0 );
  int pw = PAN_GETW( panel_funcs, panel );
  int cw = CON_W( ((LWControl *)about_button) );
  int cy = CON_Y( ((LWControl *)about_button) );
  MOVE_CON( ((LWControl *)about_button), (pw/2 - cw/2), cy );

  // Load/Save Buttons
  load_button = LOADBUTTON_CTL( panel_funcs, panel, "Load...", 50 );
  save_button = SAVEBUTTON_CTL( panel_funcs, panel, "Save...", 50 );

  panel_tools->putOnLineWith( paste_button, load_button, 0 );
  panel_tools->putOnLineWith( load_button,  save_button, 0 );

  panel_tools->rightAlignWithPanel( panel_funcs, panel, save_button, 0 );
  panel_tools->rightAlignWithPanel( panel_funcs, panel, load_button, 55 );

  // Fix Panel Size
  cy = CON_Y( ((LWControl *)is_int->copy_button) );
  int ch = CON_H( ((LWControl *)is_int->copy_button) );
  PAN_SETH( panel_funcs, panel, (cy + ch) );

  // Update the control with the instance data
  SET_INT( ((LWControl *)instance_popup), instances.FindIndexOf( inst ) );
  xpanels->viewRefresh( main_xpanel );

  // Panel Callbacks
  (*panel_funcs->set)( panel, PAN_USERCLOSE,    HandlePanelClose    );
  (*panel_funcs->set)( panel, PAN_USERDRAW,     HandleDrawPanel     );

  // Event Handlers
  CON_SETEVENT( ((LWControl *)instance_popup), HandleInstancePopup, NULL );
  CON_SETEVENT( ((LWControl *)copy_button),    HandleCopy,          NULL );
  CON_SETEVENT( ((LWControl *)paste_button),   HandlePaste,         NULL );
  CON_SETEVENT( ((LWControl *)paste_popup),    HandlePastePopup,    NULL );
  CON_SETEVENT( ((LWControl *)load_button),    HandleLoad,          NULL );
  CON_SETEVENT( ((LWControl *)save_button),    HandleSave,          NULL );
  CON_SETEVENT( ((LWControl *)about_button),   HandleAbout,         NULL );

  creation_ok = true;
}

// HandlePanelClose():
void HandlePanelClose( LWPanelID pan, void *data ) {
  is_int->is_open = false;
}

// HandleDrawPanel() {
void HandleDrawPanel( LWPanelID panel, void *data, DrMode mode) {
  int pw = PAN_GETW( is_int->panel_funcs, is_int->panel );

  int cy = CON_Y( ((LWControl *)is_int->instance_popup) );
  int ch = CON_H( ((LWControl *)is_int->instance_popup) );
  is_int->panel_draw->horizontalDivider( is_int->panel, is_int->panel_funcs->drawFuncs, 0, cy+ch, pw, 1 );

  cy = CON_Y( ((LWControl *)is_int->main_control) );
  ch = CON_H( ((LWControl *)is_int->main_control) );
  is_int->panel_draw->horizontalDivider( is_int->panel, is_int->panel_funcs->drawFuncs, 0, cy+ch - 5, pw, 1 );
}

// Instance Popup Callbacks
int InstanceCount( void *userdata ) {
  return instances.NumElements();
}

char * InstanceName( void *userdata, int index ) {
  if( (index < 0) || (index >= (int)instances.NumElements()) )
    return "";

  const char * surface_name = surf_funcs->name(        instances[ index ]->GetSurface() );
  const char * object_path  = surf_funcs->sceneObject( instances[ index ]->GetSurface() );
  const char * object_name  = strrchr( object_path, '\\' );

  object_name = (object_name == NULL) ? object_path : object_name + 1;

  static char buffer[ 1024 ];
  sprintf( buffer, "%s   ->    %s", object_name, surface_name );
  return buffer;
}

// OpenUI():
void IllumiSurf_Interface::OpenUI() {
  assert( is_int != NULL );

  if( !is_int->IsOpen() ) {
    is_int->panel_funcs->open( is_int->panel, PANF_NOBUTT );
    is_open = true;
  }
}

// ISXPanel_Get():
void * ISXPanel_Get( void *inst, unsigned long vid ) {
  static int retvali = 0;

  switch ( vid ) {
    case ISX_BLEND_MODE:
      retvali = is_int->inst->GetBlendMode();
      return &retvali;

    case ISX_BASE_INTENSITY:
      return is_int->inst->GetBaseIntensity();

    case ISX_ALT_INTENSITY:
      return is_int->inst->GetAltIntensity();

    case ISX_USE_COLOR:
      retvali = is_int->inst->GetUseColor() ? 1 : 0;
      return &retvali;

    case ISX_COLOR:
      return is_int->inst->GetColor();

    case ISX_USE_DIFFUSION:
      retvali = is_int->inst->GetUseDiffusion() ? 1 : 0;
      return &retvali;

    case ISX_DIFFUSION:
      return is_int->inst->GetDiffusion();

    case ISX_USE_LUMINOSITY:
      retvali = is_int->inst->GetUseLuminosity() ? 1 : 0;
      return &retvali;

    case ISX_LUMINOSITY:
      return is_int->inst->GetLuminosity();

    case ISX_USE_SPECULAR:
      retvali = is_int->inst->GetUseSpecular() ? 1 : 0;
      return &retvali;

    case ISX_SPECULAR:
      return is_int->inst->GetSpecular();

    case ISX_USE_GLOSSINESS:
      retvali = is_int->inst->GetUseGlossiness() ? 1 : 0;
      return &retvali;

    case ISX_GLOSSINESS:
      return is_int->inst->GetGlossiness();

    case ISX_USE_REFLECTION:
      retvali = is_int->inst->GetUseReflection() ? 1 : 0;
      return &retvali;

    case ISX_REFLECTION:
      return is_int->inst->GetReflection();

    case ISX_USE_TRANSPARENCY:
      retvali = is_int->inst->GetUseTransparency() ? 1 : 0;
      return &retvali;

    case ISX_TRANSPARENCY:
      return is_int->inst->GetTransparency();

    case ISX_USE_REFRACTION:
      retvali = is_int->inst->GetUseRefraction() ? 1 : 0;
      return &retvali;

    case ISX_REFRACTION:
      return is_int->inst->GetRefraction();

    case ISX_USE_TRANSLUCENCY:
      retvali = is_int->inst->GetUseTranslucency() ? 1 : 0;
      return &retvali;

    case ISX_TRANSLUCENCY:
      return is_int->inst->GetTranslucency();
  }

  return NULL;
}

// ISXPanel_Set():
en_LWXPRefreshCodes ISXPanel_Set( void *_inst, unsigned long vid, void *value ) {
  en_LWXPRefreshCodes retval = LWXPRC_DFLT;

  switch ( vid ) {
    case ISX_BLEND_MODE:
      is_int->inst->SetBlendMode( (IllumiSurf_BlendModes) *((int*)value) );
      break;

    case ISX_USE_COLOR:
      is_int->inst->SetUseColor( *((int*)value) != 0 );
      break;

    case ISX_USE_DIFFUSION:
      is_int->inst->SetUseDiffusion( *((int*)value) != 0 );
      break;

    case ISX_USE_LUMINOSITY:
      is_int->inst->SetUseLuminosity( *((int*)value) != 0 );
      break;

    case ISX_USE_SPECULAR:
      is_int->inst->SetUseSpecular( *((int*)value) != 0 );
      break;

    case ISX_USE_GLOSSINESS:
      is_int->inst->SetUseGlossiness( *((int*)value) != 0 );
      break;

    case ISX_USE_REFLECTION:
      is_int->inst->SetUseReflection( *((int*)value) != 0 );
      break;

    case ISX_USE_TRANSPARENCY:
      is_int->inst->SetUseTransparency( *((int*)value) != 0 );
      break;

    case ISX_USE_REFRACTION:
      is_int->inst->SetUseRefraction( *((int*)value) != 0 );
      break;

    case ISX_USE_TRANSLUCENCY:
      is_int->inst->SetUseTranslucency( *((int*)value) != 0 );
      break;
  }

  return retval;
}

// HandleInstancePopup():
void HandleInstancePopup( LWControl *control, void *user_data ) {
  int index;
  GET_INT( control, index );
  is_int->inst = instances[ index ];

  is_int->xpanels->viewRefresh( is_int->main_xpanel );
}

// HandleCopy():
void HandleCopy( LWControl *control, void *user_data ) {
  copy_inst = is_int->inst;
  UNGHOST_CON( ((LWControl *)is_int->paste_button) );
  UNGHOST_CON( ((LWControl *)is_int->paste_popup)  );
}

// HandlePaste():
void HandlePaste( LWControl *control, void *user_data ) {
  if( copy_inst == NULL )
    return;

  if( copy_inst == is_int->inst )
    return;

  *is_int->inst = *copy_inst;
  is_int->xpanels->viewRefresh( is_int->main_xpanel );
}

// HandlePastePopup():
void HandlePastePopup( LWControl *control, void *user_data ) {
  if( copy_inst == NULL )
    return;

  int index;
  GET_INT( control, index );

  if( is_int->message->yesNo( "IllumiSurf: Confirm Multiple Paste",
                              (index == 0) ? "Paste into all instances applied to this object?" :
                                             "Paste into all applied instances?", NULL ) == 0 ) {
    return;
  }

  const char * copy_path  = surf_funcs->sceneObject( copy_inst->GetSurface() );
  for( unsigned long i=0; i < instances.NumElements(); i++ ) {
    if( instances[i] == copy_inst )
      continue;

    if( index == 0 ) {   // Only other instances in this object
      if( strcmp( copy_path, surf_funcs->sceneObject( instances[i]->GetSurface() ) ) != 0 )
        continue;
    }

    *(instances[i]) = *copy_inst;
  }

  is_int->xpanels->viewRefresh( is_int->main_xpanel );

  is_int->message->info( "IllumiSurf:  Multiple paste complete", NULL );
}

// HandleLoad():
void HandleLoad( LWControl *control, void *user_data ) {
  char buffer[ 1024 ];
  GET_STR( control, buffer, 1024 );
  if( buffer[0] == '\0' )
    return;

  const char *error = is_int->inst->LoadFromFile( buffer );

  if( error == NULL )
    is_int->message->info( "IllumiSurf:  Successfully loaded settings from ", buffer );
  else
    is_int->message->error( error, buffer );

  is_int->xpanels->viewRefresh( is_int->main_xpanel );
}

// HandleSave():
void HandleSave( LWControl *control, void *user_data ) {
  char buffer[ 1024 ];
  GET_STR( control, buffer, 1024 );
  if( buffer[0] == '\0' )
    return;

  const char *error = is_int->inst->SaveToFile( buffer );

  if( error == NULL )
    is_int->message->info( "IllumiSurf:  Successfully saved settings into ", buffer );
  else
    is_int->message->error( error, buffer );
}

// HandleAbout():
void HandleAbout( LWControl *control, void *user_data ) {
  PiEToolsLogo logo;
  LWAboutPanel( is_int->panel_funcs, "IllumiSurf", about_text, &logo, is_int->panel, true, NULL );
}

