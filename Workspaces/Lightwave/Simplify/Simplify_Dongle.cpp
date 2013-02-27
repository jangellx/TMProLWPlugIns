//
// Simplify Interface Dongle Routines
//

#include <math.h>

#include <portable/pfstream/pfstream.h>

#include "Simplify_ObjRep_Interface.h"
#include "SimplifyLogo.h"

const char *register_text[] = {
  "Simplify is now in Trial Mode; everything is functional",
  "except for the interface.  To unlock the Simplify,",
  "enter your registration code below and click Register Now.",
  "",
  "If you need a key, you can purchase a registration code from",
  "http://www.tmproductions.com . Registration codes are keyed",
  "to specific Lightwave 3D dongles, so you'll need a unique",
  "code for each machine.",
  NULL };

const char *trial_text = "Click OK below to cancel registration.";

extern SimplifyInterface *simp_int;
extern SimplifyLogo       logo;
       SimplifyDongle    *simpd_int = NULL;


// DLLMain():
//  Win32 Only:  This is initialized so we can later use GetModuleFileName()
//   to figure out where our plug-in is, and thus where our key is.
#ifdef WIN32
#include <windows.h>

HINSTANCE hinst;

BOOL WINAPI DllMain( HANDLE hinstance, ULONG reason, LPVOID reserved ) {
  if( reason == DLL_PROCESS_ATTACH )
    hinst = (HINSTANCE)hinstance;
  return TRUE;
}
#endif


// Constructor
SimplifyDongle::SimplifyDongle() {
  MakeRegisterPanel();
}

// Destructor
SimplifyDongle::~SimplifyDongle() {
  simp_int->panel_funcs->Destroy( register_panel );
}

// MakeRegisterPanel():
//  Creates the Register Panel
bool SimplifyDongle::MakeRegisterPanel() {
  if( simp_int == NULL )
    return false;

  // Create the Panel
  register_panel = simp_int->panel_funcs->Create( "Simplify Registration" );

  try {
    if( !( dongle_id = STRRO_CTL( simp_int->panel_funcs->orig_struct, register_panel,
                                  "Dongle ID", 43 ) ) )
      throw false;

    if( !( reg_code = STR_CTL( simp_int->panel_funcs->orig_struct, register_panel,
                               "Reg Code", 43 ) ) )
      throw false;

    if( !( register_now = WBUTTON_CTL( simp_int->panel_funcs->orig_struct, register_panel,
                                       "Register Now", 150 ) ) )
      throw false;

    // Position some things
    MOVE_CON( dongle_id, 25, 212 );
    PanelTools::AlignLabels( dongle_id, reg_code );

    PAN_SETW( simp_int->panel_funcs->orig_struct, register_panel, 400 );
    PAN_SETH( simp_int->panel_funcs->orig_struct, register_panel, 350 );

    PanelTools::PutUnder( reg_code, register_now );

    int pw = PAN_GETW( simp_int->panel_funcs->orig_struct, register_panel );
    int w  = CON_W( register_now );
    int y  = CON_Y( register_now );
    MOVE_CON( register_now, (pw/2 - w/2), y );

    // Default Values
    char buffer[10];
    sprintf( buffer, "%ld", GetDongleID() );
    SET_STR( dongle_id, buffer, strlen( buffer ) );

    // Listener Functions
    CON_SETEVENT( register_now, OnRegisterNow, this );
    CON_SETEVENT( reg_code,     OnRegisterNow, this );

    // Set the Draw Functions
    simp_int->panel_funcs->Set( register_panel, PAN_USERDRAW, DrawRegisterPanel );

    return true;
  } catch( bool a ) {
    if( !a ) {
      simp_int->message->Error("Error creating interface controls; aborting" );
      return false;
    }
  }

  return true;
}

// DrawRegisterPanel():
//  Called to draw the Register Panel
void DrawRegisterPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = simp_int->panel_funcs->DrawFuncs;

  // Title Image
  logo.DrawInLWPanels( simp_int->simpd->register_panel, df, -205, 0 );

  int ph = PAN_GETH( simp_int->panel_funcs->orig_struct, simp_int->simpd->register_panel );
  int pw = PAN_GETW( simp_int->panel_funcs->orig_struct, simp_int->simpd->register_panel );

  int y = 85;

  for( int i=0; register_text[i] != NULL; i++ ) {
    df->WriteText( simp_int->simpd->register_panel, (char *)register_text[i], COLOR_BLACK,
                   (pw/2)-(df->TextWidth(simp_int->simpd->register_panel, (char *)(register_text[i]) ))/2,
                   y );
    y += 15;
  }

  y = CON_Y( simp_int->simpd->register_now ) + 50;
  df->WriteText( simp_int->simpd->register_panel, (char *)trial_text, COLOR_BLACK,
                 (pw/2)-(df->TextWidth(simp_int->simpd->register_panel, (char *)(trial_text) ))/2,
                 y );
}

// OpenRegisterPanel():
//  Opens the registration panel. Returns true if the
//   registration key is accepted, and false if not.
bool SimplifyDongle::OpenRegisterPanel() {
  if( !simp_int->panel_funcs->Open( register_panel, PANF_BLOCKING | PANF_MOUSETRAP | PANF_PASSALLKEYS ) )
    return false;

  return true;
}

// ReadKey():
//  Finds and reads a Simplify license, returning false if the
//   key cannot be found.  Returns a pointer to the matching
//   key if the found; otherwise, it returns NULL.
const char * SimplifyDongle::ReadKey( const char *path ) {
  char filename[ MAX_PATH_LENGTH ];

  if( path == NULL ) {
    // Get the Plug-in's Path
    #ifdef WIN32
      GetModuleFileName( hinst, filename, MAX_PATH_LENGTH );
    #endif
    DirStrings::ChangeFile( filename, "simplify.key" );
  } else
    strcpy( filename, path );

  if( !DirInfo::Exists( filename ) )
    return NULL;

  pifstream in( filename );
  if( !in )
    return NULL;

  in.GuessEOLType();
  static char buffer[ 64 ];
  do {
    in.getline( buffer, 1024 );
    if( TestKey( buffer, GetDongleID() ) )
      return buffer;
  } while( !in.eof() );

  return NULL;
}


// WriteKey():
//  Outputs a Simplify license to disk.  If a key file already exists
//   on disk, the new key will be appended to it.
bool SimplifyDongle::WriteKey( const char *key, const char *path  ) {
  char buffer[ MAX_PATH_LENGTH ];

  if( path == NULL ) {
    // Get the Plug-in's Path
    #ifdef WIN32
      GetModuleFileName( hinst, buffer, MAX_PATH_LENGTH );
    #endif
    DirStrings::ChangeFile( buffer, "simplify.key" );

  } else
    strcpy( buffer, path );

  // Open the output file
  ofstream out( buffer, ios::app );
  if( !out ) {
    simp_int->message->Error( "Simplify Error:  Unable add key to license file",
                            buffer );
    return false;
  }

  // Write the new key and close the file
  out << key << endl;

  if( !out ) {
    simp_int->message->Error( "Simplify Error:  File I/O error occured writing key to",
                              buffer );
    return false;
  }

  return true;
}

// GetDongleID():
//  Returns the dongle ID of the system.
long SimplifyDongle::GetDongleID() {
  long system_id = (long)simp_int->Global( "System ID", GFUSE_TRANSIENT );

  if( system_id == 0 )
    return -1;
  else
    return (LWSYS_SERIALBITS & system_id);
}

// OnRegisterNow():
//  Handles registering this copy of Simplify
void OnRegisterNow( LWControl *ctl, SimplifyDongle *panel ) {
  char key[64];
  GET_STR( simp_int->simpd->reg_code, key, 64 );

  if( simp_int->simpd->TestKey( key, simp_int->simpd->GetDongleID() ) ) {
    if( simp_int->simpd->WriteKey( key ) ) {
      simp_int->message->Info( "Registration Code Accepted.  Simplify is now unlocked.",
                             "Thank you for registering Simplify" );
    
      simp_int->panel_funcs->Close( simp_int->simpd->register_panel );
    }
  } else {
    simp_int->message->Error( "Invalid Simplify Registration Code Entered.",
                            "Please enter a valid code." );
  }
}



