//
// FreeKey.cpp
//
// FreeKey deletes keyframes from any hierarchy of objects and bones
//  in the scene.  This includes:
//
//  - Deleteing keys outside a user-defined range (ie: keep keys 80 to 150)
//  - Shift keys backward/forward, or shift to frame 0 after deleting.
//  - Delete every Xth frame within the range, or keep only every Xth
//     frame within the range.
//
// Order of operations:
//  - Delete keys within range
//

#ifndef HEADER_BATARANGHIERARHCY
#define HEADER_BATARANGHIERARHCY

#include <string.h>
#include <stdio.h>
#include <fstream.h>
#include <iomanip.h>
#include <math.h>

#include <portable/dirtools/dirtools.h>
#include <portable/dynarray/dynarray.h>
#include <portable/pfstream/pfstream.h>

#include <lightwave/lwscene/lwscene.h>
#include <lightwave/lwscenescan/lwscenescan.h>

#include "FreeKey.h"

// Error Strings
const char *freekey_error_strings[] = {
  "Everything's OK!",
  "The source scene file isn't a Lightwave Scene!",
  "File error occured reading the source scene!",
  "The low frame must be 2 less than high frame in Remove Inside mode!",
  "The low frame must be less than high frame!"
  "User Abort",
  "Item not found in scene; processing aborted!",
  "Unsupported Lightwave Scene version encountered; aborting!",
  "Error loading item motion; aborting!",
  "Don't use me -- I'm just the error enum count" };

// Keyword List
const char *freekey_scan_keywords[] = {
  "LoadObject",
  "AddNullObject",
  "BoneName",
  "LightName",
//  "ShowCamera",
  "ObjectMotion",
  "BoneMotion",
  "LightMotion",
//  "CameraMotion",
  NULL };

enum freekey_scan_indices {
  FKSCAN_LOAD_OBJECT,
  FKSCAN_ADD_NULL_OBJECT,
  FKSCAN_BONE_NAME,
  FKSCAN_LIGHT_NAME,
//  FKSCAN_SHOW_CAMERA,
  FKSCAN_OBJECT_MOTION,
  FKSCAN_BONE_MOTION,
  FKSCAN_LIGHT_MOTION,
//  FKSCAN_CAMERA_MOTION,
  FKSCAN_NUM_KEYWORDS };

// SetName():
//  Sets the group name.  If NULL, the
//   name will be cleared.
void FreeKeyGroup::SetName( const char *new_name ) {
  if( name != NULL )
    free( name );
  
  if( new_name != NULL )
    name = strdup( new_name );
}

// Constructor
FreeKey::FreeKey( const char *_source_name, const char *_output_name ) {
  // Scene-independant variables
  key_display_mode     =   0;
  item_display_mode    =   0;
  tool_options_tab     =   TOOLTAB_ITEM_SELECTION;
  divider              = 200;
  draw_style           =   DRAWSTYLE_56;
  use_opengl_item_list = true;
  use_opengl_logo      = true;

  Init( _source_name, _output_name );
}

// Destructor
FreeKey::~FreeKey() {
  groups.Flush();
}

// Init():
//  Initializes the instance.  If it is already initialized, this
//   will clear and re-initialize it.  This mainly just initializes
//   scene-specific variables
void FreeKey::Init( const char *_source_name, const char *_output_name ) {
  SetSource( _source_name );
  SetOutput( _output_name );

  display_tree      = false;
  items_processed   = 0;
  lights_processed  = 0;
  bones_processed   = 0;

  item_list.Flush();
  groups.Flush();
  scene.Clear();
  scene.SetSceneName( source_name );

  // Interface-level settings
  zoom              =   1.0;
  name_scroll       =   0;
  time_scroll       =   0.0;
  display_min       =   0.0;
  display_max       =   2.0;
  current_time      =   0.0;
  current_item      =  -1;
  item_top          =   0;
  current_group     =   0;
}

// GetErrorString():
//  Returns the string referenced by the error enum passed in.
const char * FreeKey::GetErrorString( int error ) {
  if( error < 0 )
    return LWSceneScanErrorStrings[ -error ];
  else
    return freekey_error_strings[ error - FREEKEY_ERROR_BASE ];
}

// Process():
int FreeKey::Process( bool(__cdecl Update)(char *name, int current, int total) ) {
  LWSceneScan scanner( freekey_scan_keywords, GetSource(), GetOutput() );

  // Start Scan
  int retval = scanner.StartScan( LWSceneScan::read_write );
  if( retval < 0 ) {
    return retval;
  } else if( retval > 2 ) {         // Change this when we support other versions
    scanner.EndScan();
    DirInfo::Delete( GetOutput() );  // Destroy an partial output files
    return FREEKEY_UNSUPPORTED_SCENE_VERSION;
  }

  // Variable Setup
  char item_name[ MAX_PATH_LENGTH ] = "\0";
  int error;

  int object_index = 0;
  int bone_index   = 0;
  int light_index  = 0;
  int g = -1;                                  // group the current item belongs to

  // - Parent/Child Variables
  LWItem * parent_base = NULL;
  LWItem * obj_base    = NULL;

  LWMotion motion;

  // - In-Item Variables
  enum InItem {
    in_none,
    in_object,
    in_bone,
    in_light,
    in_camera };

  InItem in_item = in_none;
  int this_item_index = 0;

  // - UI-Level Variables
  int total_items = scene.GetBaseItem().CountItems( false, true );

  items_processed  = 0;
  lights_processed = 0;
  bones_processed  = 0;

  // Scan Loop
  retval = scanner.ScanNext();
  do {
    // Error/EOF test
    if( retval == LWSCAN_END_OF_FILE_REACHED )
      return FREEKEY_OK;
    else if( retval < 0 )
      return retval;

    switch( retval ) {
    // New Item Checks
    case FKSCAN_LOAD_OBJECT:
    case FKSCAN_ADD_NULL_OBJECT:
      strcpy( item_name, DirStrings::HasFile( scanner.GetBufferSansKeyword() ) );
      in_item = in_object;

      parent_base = scene.GetBaseItem().FindIndexInScene( ++object_index, LWI_ITEM_OBJECT | LWI_ITEM_NULL_OBJECT );       // Also increments object_index!
      obj_base = parent_base;                                                     //  Used later by bones within this object
      bone_index = 0;
      retval = scanner.OutputAndScanNext();
      break;

    case FKSCAN_LIGHT_NAME:
      strcpy( item_name, scanner.GetBufferSansKeyword() );
      in_item = in_light;

      light_index++;
      retval = scanner.OutputAndScanNext();
      break;

    case FKSCAN_BONE_NAME:
      strcpy( item_name, scanner.GetBufferSansKeyword() );
      in_item = in_bone;

      if( obj_base != NULL ) {
        if( obj_base->HasSkeleton() )
          parent_base = obj_base->GetSkeleton().FindIndexInScene( ++bone_index, LWI_ITEM_BONE );     // Also increments bone_index!
      }
      retval = scanner.OutputAndScanNext();
      break;

    // Item Motion Checks
    case FKSCAN_OBJECT_MOTION:
    case FKSCAN_LIGHT_MOTION:
    case FKSCAN_BONE_MOTION:
//    case FKSCAN_CAMERA_MOTION:    // Will be handled in later releases
      if( &Update ) {
        if(  !( (*Update)( item_name, ++this_item_index, total_items ) )  ) {
          // User Abort
          DirInfo::Delete( GetOutput() );
          return FREEKEY_USER_ABORT;
        }
      }

      if( parent_base == NULL )
        return FREEKEY_ITEM_NOT_IN_SCENE;

      g = parent_base->GetGroup();
      if( g == -1 ) {
        scanner.GetOutputStream() << scanner.GetBuffer() << scanner.GetOutputStream().endl();          // <item>Motion (unnamed) line
        retval = scanner.ScanNext();
      } else {
        switch( in_item ) {
        case in_bone:
          cerr << "   Processing Bone \"" << item_name << "\"..." << endl;
          bones_processed++;
          break;
        case in_object:
          cerr << "  Processing Object \"" << item_name << "\"..." << endl;
          items_processed++;
          break;
        case in_light:
          cerr << "  Processing Light \"" << item_name << "\"..." << endl;
          lights_processed++;
          break;
        case in_camera:
          cerr << "  Processing Camera \"" << item_name << "\"..." << endl;
          items_processed++;
          break;
        }

        // Load the motion in
        error = motion.LoadMotion( scanner.GetInputStream(), LWS_LOAD_MOTIONS, scanner.GetVersion() );
        if( error != LWSERROR_OK ) {
          cerr << " ERROR: failed to load motion for \"" << item_name << "\"; aborting" << endl;
          return FREEKEY_ERROR_LOADING_MOTION;
        } else {
          if( motion.channel[0]->GetNumKeys() > 1 ) {  // Don't bother if there's only 1 key.
            if( groups[g]->GetRemoveMode() == FreeKeyGroup::remove_inside ) {     // Remove keys inside range; no step or delta
              // Delete keyframes in the range, if applicable
              motion.DeleteRange( groups[g]->GetLow()/30.0, groups[g]->GetHigh()/30.0 );
            } else {
              // Remove keys outside range, if applicable
              switch( groups[g]->GetRemoveMode() ) {
                case FreeKeyGroup::remove_outside:
                  motion.DeleteRange( -100000, groups[g]->GetLow()/30.0 );
                  motion.DeleteRange( groups[g]->GetHigh()/30.0, 100000.0 );
                  break;

                case FreeKeyGroup::remove_before:
                  motion.DeleteRange( -100000, groups[g]->GetLow()/30.0 );
                  break;

                case FreeKeyGroup::remove_after:
                  motion.DeleteRange( groups[g]->GetHigh()/30.0, 100000.0 );
                  break;
              }

              // Reverse the keys, if applicable
              if( groups[g]->GetDirection() == 1 )
                motion.ReverseKeys( groups[g]->GetLow()/30.0, groups[g]->GetHigh()/30.0 );

              // Scale keys, if applicable
              float scalar = 1.0;                    // Used in all later operations so they affect the newly modified range
              if( groups[g]->GetUseScaleKeys() ) {
                scalar = groups[g]->GetScaleKeys();
                motion.ScaleKeys( groups[g]->GetLow()/30.0, groups[g]->GetHigh()/30.0,
                                  groups[g]->GetScaleKeys(), groups[g]->GetLow()/30.0,
                                  LWCHANKEYPOST_SCRUNCH_FRAMES );
              }

              // Delete every Xth frame, if applicable
              if( groups[g]->GetUseStep() )
                motion.DeleteStep( groups[g]->GetLow()/30.0, (groups[g]->GetHigh()/30.0) * scalar,
                                   groups[g]->GetStep()/30.0, 1.0/30.0, groups[g]->GetInvertStep() );

              // Shift the keys, if applicable
              if( groups[g]->GetUseDelta() )
                motion.ShiftKeys( groups[g]->GetLow()/30.0, (groups[g]->GetHigh()/30.0) * scalar,
                                  groups[g]->GetDelta()/30.0, LWCHANKEYPOST_SCRUNCH_FRAMES );
            }
          }
        }

        scanner.GetOutputStream() << scanner.GetBuffer() << scanner.GetOutputStream().endl();          // <item>Motion (unnamed) line
        motion.SaveMotion( scanner.GetOutputStream(), scanner.GetVersion() );
        retval = scanner.ScanNext();
      }
      break;

    default:
      retval = scanner.OutputAndScanNext();
    }

  } while( true );

  return FREEKEY_OK;
}

//
// ------------------------------------------------------------------
//

// TestRange()
//  Make sure the low part of the range is less than the high part
int FreeKeyGroup::TestRange() {
  if( GetRemoveMode() == remove_inside ) {
    if( low + 1 >= high - 1)
      return FREEKEY_LOW_GREATER_THAN_HIGH_INSIDE;
  } else {
    if( low >= high )
      return FREEKEY_LOW_GREATER_THAN_HIGH_OUTSIDE;
  }

  return FREEKEY_OK;
}

// GenerateNames():
//  Generates new names for the output file.
//   New names are generated only if the original names are
//   blank (ie: first char is \0).  Returns false if the
//   source is also empty.
bool FreeKey::GenerateNames( const char *suffix ) {
  // Generate an output name, if needed
  if( output_name[0] == '\0' ) {
    if( suffix == NULL ) {                                 // No suffix and invalid name; abort
      return false;
    } else {
      strcpy( output_name, source_name );                   // Copy the in name
      char * ext = DirStrings::HasExtension( output_name ); // Find any extension
      if( ext )                                             // Strip the extension
        *ext = '\0';
      strcat( output_name, suffix );                        // Append -<suffix>
      ext = DirStrings::HasExtension( source_name );        // Find the original extension
      if( ext )                                             // Append the origianl extension to the output_name
        strcat( output_name, ext );
    }
  }

  return true;
}

#endif