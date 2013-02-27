//
// Freekey Interface Dongle Routines
//

#include <math.h>

#include "FreeKey-Interface.h"
#include "FreeKeyLogo.h"

const char *register_text[] = {
  "Freekey is now in Trial Mode; everything is functional",
  "except for the Process button.  To unlock the FreeKey,",
  "enter your registration code below and click Register Now.",
  "",
  "If you need a key, you can purchase a registration code from",
  "http://www.tmproductions.com . Registration codes are keyed",
  "to specific Lightwave 3D dongles, so you'll need a unique",
  "code for each machine.",
  NULL };

const char *trial_text = "Click OK below to continue running FreeKey in Trial Mode.";

extern FreeKeyInterface *fk_int;
extern FreeKeyLogo       logo;
       FreeKeyDongle    *fkd_int = NULL;


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
FreeKeyDongle::FreeKeyDongle() {
  MakeRegisterPanel();
}

// Destructor
FreeKeyDongle::~FreeKeyDongle() {
  fk_int->panel_funcs->Destroy( register_panel );
}

// MakeRegisterPanel():
//  Creates the Register Panel
bool FreeKeyDongle::MakeRegisterPanel() {
  // Create the Panel
  register_panel = fk_int->panel_funcs->Create( "FreeKey Registration" );

  try {
    if( !( dongle_id = STRRO_CTL( fk_int->panel_funcs->orig_struct, register_panel,
                                  "Dongle ID", 43 ) ) )
      throw false;

    if( !( reg_code = STR_CTL( fk_int->panel_funcs->orig_struct, register_panel,
                               "Reg Code", 43 ) ) )
      throw false;

    if( !( register_now = WBUTTON_CTL( fk_int->panel_funcs->orig_struct, register_panel,
                                       "Register Now", 150 ) ) )
      throw false;

    // Position some things
    MOVE_CON( dongle_id, 25, 212 );
    PanelTools::AlignLabels( dongle_id, reg_code );

    PAN_SETW( fk_int->panel_funcs->orig_struct, register_panel, 400 );
    PAN_SETH( fk_int->panel_funcs->orig_struct, register_panel, 350 );

    PanelTools::PutUnder( reg_code, register_now );

    int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, register_panel );
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
    fk_int->panel_funcs->Set( register_panel, PAN_USERDRAW, DrawRegisterPanel );

    return true;
  } catch( bool a ) {
    if( !a ) {
      fk_int->message->Error("Error creating interface controls; aborting" );
      return false;
    }
  }

  return true;
}

// DrawRegisterPanel():
//  Called to draw the Register Panel
void DrawRegisterPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode ) {
  CDrawFuncs *df = fk_int->panel_funcs->DrawFuncs;

  // Title Image
  logo.DrawInLWPanels( fk_int->fkd->register_panel, df, -280, 0 );

  int ph = PAN_GETH( fk_int->panel_funcs->orig_struct, fk_int->fkd->register_panel );
  int pw = PAN_GETW( fk_int->panel_funcs->orig_struct, fk_int->fkd->register_panel );

  int y = 85;

  for( int i=0; register_text[i] != NULL; i++ ) {
    df->WriteText( fk_int->fkd->register_panel, (char *)register_text[i], COLOR_BLACK,
                   (pw/2)-(df->TextWidth(fk_int->fkd->register_panel, (char *)(register_text[i]) ))/2,
                   y );
    y += 15;
  }

  y = CON_Y( fk_int->fkd->register_now ) + 50;
  df->WriteText( fk_int->fkd->register_panel, (char *)trial_text, COLOR_BLACK,
                 (pw/2)-(df->TextWidth(fk_int->fkd->register_panel, (char *)(trial_text) ))/2,
                 y );
}

// OpenRegisterPanel():
//  Opens the registration panel. Returns true if the
//   registration key is accepted, and false if not.
bool FreeKeyDongle::OpenRegisterPanel() {
  if( !fk_int->panel_funcs->Open( register_panel, PANF_BLOCKING | PANF_MOUSETRAP | PANF_PASSALLKEYS ) )
    return false;

  return true;
}

// ReadKey():
//  Finds and reads a FreeKey license, returning false if the
//   key cannot be found.  Returns a pointer to the matching
//   key if the found; otherwise, it returns NULL.
const char * FreeKeyDongle::ReadKey( const char *path ) {
  char filename[ MAX_PATH_LENGTH ];

  if( path == NULL ) {
    // Get the Plug-in's Path
    #ifdef WIN32
      GetModuleFileName( hinst, filename, MAX_PATH_LENGTH );
    #endif
    DirStrings::ChangeFile( filename, "freekey.key" );
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
    in.getline( buffer, 64 );
    if( TestKey( buffer, GetDongleID() ) )
      return buffer;
  } while( !in.eof() );

  return NULL;
}


// WriteKey():
//  Outputs a FreeKey license to disk.  If a key file already exists
//   on disk, the new key will be appended to it.
bool FreeKeyDongle::WriteKey( const char *key, const char *path  ) {
  char buffer[ MAX_PATH_LENGTH ];

  if( path == NULL ) {
    // Get the Plug-in's Path
    #ifdef WIN32
      GetModuleFileName( hinst, buffer, MAX_PATH_LENGTH );
    #endif
    DirStrings::ChangeFile( buffer, "freekey.key" );

/*
    // In case you need to know where some paths point...
    DirInfoFunc &dirfunc = *( (DirInfoFunc *)fk_int->Global( "Directory Info", GFUSE_TRANSIENT ) );
    ofstream out( "e:\\lwpaths.txt" );
    out << " Fonts         " << (dirfunc("Fonts") == NULL ? "NULL" : dirfunc("Fonts")) << endl;
    out << " Macros        " << (dirfunc("Macros") == NULL ? "NULL" : dirfunc("Macros")) << endl;
    out << " Install       " << (dirfunc("Install") == NULL ? "NULL" : dirfunc("Install")) << endl;
    out << " Objects       " << (dirfunc("Objects") == NULL ? "NULL" : dirfunc("Objects")) << endl;
    out << " Images        " << (dirfunc("Images") == NULL ? "NULL" : dirfunc("Images")) << endl;
    out << " Motions       " << (dirfunc("Motions") == NULL ? "NULL" : dirfunc("Motions")) << endl;
    out << " Temp          " << (dirfunc("Temp") == NULL ? "NULL" : dirfunc("Temp")) << endl;
    out << " Plugins       " << (dirfunc("Plugins") == NULL ? "NULL" : dirfunc("Plugins")) << endl;
    out << " Settings      " << (dirfunc("Settings") == NULL ? "NULL" : dirfunc("Settings")) << endl;
    out << " Hierarchies   " << (dirfunc("Hierarchies") == NULL ? "NULL" : dirfunc("Hierarchies")) << endl;
    out << " Surfaces      " << (dirfunc("Surfaces") == NULL ? "NULL" : dirfunc("Surfaces")) << endl;
    out << " Output        " << (dirfunc("Output") == NULL ? "NULL" : dirfunc("Output")) << endl;
    out << " Animations    " << (dirfunc("Animations") == NULL ? "NULL" : dirfunc("Animations")) << endl;
    out << " Previews      " << (dirfunc("Previews") == NULL ? "NULL" : dirfunc("Previews")) << endl;
    out << " Command       " << (dirfunc("Command") == NULL ? "NULL" : dirfunc("Command")) << endl;
    out.close();
*/
  } else
    strcpy( buffer, path );

  // Open the output file
  ofstream out( buffer, ios::app );
  if( !out ) {
    fk_int->message->Error( "FreeKey Error:  Unable add key to license file",
                            buffer );
    return false;
  }

  // Write the new key and close the file
  out << key << endl;

  if( !out ) {
    fk_int->message->Error( "FreeKey Error:  File I/O error occured writing key to",
                            buffer );
    return false;
  }

  return true;
}

// GetDongleID():
//  Returns the dongle ID of the system.
long FreeKeyDongle::GetDongleID() {
  long system_id = (long)fk_int->Global( "System ID", GFUSE_TRANSIENT );

  if( system_id == 0 )
    return -1;
  else
    return (LWSYS_SERIALBITS & system_id);
}

// OnRegisterNow():
//  Handles registering this copy of FreeKey
void OnRegisterNow( LWControl *ctl, FreeKeyDongle *panel ) {
  char key[64];
  GET_STR( fk_int->fkd->reg_code, key, 64 );

  if( fk_int->fkd->TestKey( key, fk_int->fkd->GetDongleID() ) ) {
    if( fk_int->fkd->WriteKey( key ) ) {
      fk_int->message->Info( "Registration Code Accepted.  Freekey is now unlocked.",
                             "Thank you for registering FreeKey" );
    
      fk_int->panel_funcs->Close( fk_int->fkd->register_panel );
    }
  } else {
    fk_int->message->Error( "Invalid FreeKey Registration Code Entered.",
                            "Please enter a valid code." );
  }
}



