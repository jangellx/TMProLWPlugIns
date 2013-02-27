//
// FreeKeyInterface-Dongle.h
//
// This contains the FreeKeyDongle class, which includes functions
//  for testing the dongle ID, creating a key, testing a key, reporting
//  to the user that a key cannot be found, and presenting a registration
//  panel.
//

#include <stdio.h>

#include "SimplifyLicenseGen/SimplifyLicense.h"

class SimplifyDongle {
public:
  SimplifyDongle();
  ~SimplifyDongle();

  bool          OpenRegisterPanel();                                   // Opens the Register panel, prompting the user to register FreeKey

  const char *  ReadKey(  const char *path = NULL );                   // Reads a license key from disk
  bool          WriteKey( const char *key, const char *path = NULL);   // Writes a license key to disk
  inline bool   TestKey(  const char *key, long dongle_id );           // Tests a license key against a dongle ID

  long          GetDongleID();

public:
  bool           MakeRegisterPanel();

  LWPanelID      register_panel;                 // The Main Panel

  LWControl     *dongle_id;                      // Dongle ID read-only text entry
  LWControl     *reg_code;                       // Registration Code text entry
  LWControl     *register_now;                   // Register Now button
};

void DrawRegisterPanel( LWPanelID panel_id, void *panel_user, DrMode panel_draw_mode );

void OnRegisterNow( LWControl *ctl, SimplifyDongle *panel );

// TestSimplifyLicense():
//  Tests the license key against the dongle ID; if it matches,
//   this returns true.
inline bool SimplifyDongle::TestKey( const char * key, long dongle_id ) {
  if( key == NULL )
    return false;

  if( strcmp( GenerateSimplifyLicense( dongle_id ), key ) == 0 )
    return true;

  return false;
}
