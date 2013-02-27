//
// Old, obsolete FreeKey code
//

#include "FreeKeyMotion.h"

// OpenFiles():
//  Tests to make sure the paths are valid and opens the files.
//  This also generates an output and ubernull name if none are
//  specified.  Returns an error if something went wrong.
int FreeKey::OpenFiles() {
  GenerateNames( "-FreeKeyed" );

  int error = CLIFileManipBase::OpenFiles();
  if( error != FREEKEY_OK )
    return error;

  error = FreeKey::OpenSource();
  if( error != FREEKEY_OK )
    return error;

  return FREEKEY_OK;
}

// OpenSource():
//  Tests and opens only the source file.  Used by the plug-in
//   version of FreeKey so it can scan the hierarchy quickly.
//  --OBSOLETE--
int FreeKey::OpenSource() {
  int error = CLIFileManipBase::OpenSource();
  if( error != CLIFILEMANIPBASE_OK )
    return error;

  // Test the file headers (only happens if the input files were opened OK)
  char header[4];
  if( _in != NULL ) {
    // Test the LWS header
    _in->read( header, 4 );
    if( strncmp( header, "LWSC", 4 ) != 0 )
      return FREEKEY_SOURCE_ISNT_LWS;
    _in->seekg( 0, ios::beg );
  }

  return FREEKEY_OK;
}

// Process():
//  Actually processes the scene file.  It does everything related
//   to processing, returning FreeKey_OK if there are no errors.
//  Update is a callback function used to upadate UIs.  It is passed
//   a const pointer to the name of the item currently being
//   processed, the index of that item out of the total number of
//   items (ie: 6 of 10), and the total number of items.  These last
//   two are useful for filling progress bars.  Update should return
//   true if processing should continue, and false if the user requests
//   an abort.
int FreeKey::Process( bool(__cdecl Update)(char *name, int current, int total) ) {
  ifstream &in  = *_in;
  ofstream &out = *_out;

  if( !AreFilesOpen() )
    return false;

  char line[8192];

  int  in_item_name_start = 0;
  char in_item_name[ MAX_PATH_LENGTH ] = "\0";
  bool output_line = true;

  int item_name_start = 0;
  char cur_item_name[ 256 ] = "\0";

  int object_index = 0;
  int bone_index   = 0;
  int light_index  = 0;
  int g = -1;                                  // group the current item belongs to

  LWItem * parent_base = NULL;
  LWItem * obj_base    = NULL;

  enum InItem {
    in_none,
    in_object,
    in_bone,
    in_light,
    in_camera };

  InItem in_item = in_none;

  int this_item_index = 0;
  int total_items = scene.GetBaseItem().CountItems( false, true );

  items_processed  = 0;
  lights_processed = 0;
  bones_processed  = 0;

  // The Loop
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() ) {
      return FREEKEY_ERROR_READING_FROM_SOURCE;
    }

    // Test for an item's name
    if( strnicmp( line, "LoadObject",    10 ) == 0 ) {
      in_item    = in_object;
      bone_index = 0;
      item_name_start = 11;
      this_item_index++;
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
      in_item    = in_object;
      bone_index = 0;
      item_name_start = 14;
      this_item_index++;
    }

    if( strnicmp( line, "BoneName",      8 ) == 0 ) {
      strcpy( cur_item_name, &(line[9]) );
      in_item = in_bone;
      this_item_index++;
    }

    if( strnicmp( line, "LightName",     9 ) == 0 ) {
      strcpy( cur_item_name, &(line[10]) );
      in_item = in_light;
      this_item_index++;
    }

  //  if( strnicmp( line, "ShowCamera",    10 ) == 0 ) {
  //    strcpy( cur_item_name, "Camera" );
  //    in_object = in_camera;
  //  }

    if( item_name_start > 0 ) {
      strcpy( cur_item_name, DirStrings::HasFile( &(line[item_name_start]) ) );
      item_name_start = 0;

      // Call the Update function
      if( &Update ) {
        if(  !( (*Update)( cur_item_name, this_item_index, total_items ) )  ) {
          // User Abort
          DirInfo::Delete( GetOutput() );
          return FREEKEY_USER_ABORT;
        }
      }
    }

    // Find an Item's Motion and mark it.
    if( ( strncmp( line, "ObjectMotion", 12 ) == 0 ) ||
        ( strncmp( line, "LightMotion",  11 ) == 0 ) ||
        //( strncmp( line, "CameraMotion", 12 ) == 0 ) ||
        ( strncmp( line, "BoneMotion",   10 ) == 0 ) ) {
      g = -1;

      if( in_item == in_object ) {
        parent_base = scene.GetBaseItem().FindIndexInScene( ++object_index );       // Also increments object_index!
        obj_base = parent_base;                                                     //  Used later by bones within this object
      } else if( in_item == in_bone ) {
        parent_base = obj_base->GetSkeleton().FindIndexInScene( ++bone_index );     // Also increments object_index!
      } else if( in_item == in_light )
        parent_base = scene.GetBaseItem().FindIndexInScene( -(++light_index) );     // Also increments light_index!

      if( parent_base == NULL )
        return FREEKEY_ITEM_NOT_IN_SCENE;

      // See if we should process
      g = parent_base->GetGroup();
      if( g != -1 ) {
        switch( in_item ) {
        case in_bone:
          cerr << "   Processing Bone \"" << cur_item_name << "\"..." << endl;
          bones_processed++;
          break;
        case in_object:
          cerr << "  Processing Object \"" << cur_item_name << "\"..." << endl;
          items_processed++;
          break;
        case in_light:
          cerr << "  Processing Light \"" << cur_item_name << "\"..." << endl;
          lights_processed++;
          break;
        case in_camera:
          cerr << "  Processing Camera \"" << cur_item_name << "\"..." << endl;
          items_processed++;
          break;
        }

        // Load the motion in
        FreeKeyMotion * motion = FreeKeyMotion::NewMotionFromFile( in );
        if( !motion ) {
          cerr << " ERROR: failed to load motion for \"" << cur_item_name << "\"; skipping" << endl;
          break;
        }

        if( motion->GetLastFrameNumber() > 0 ) {  // Don't bother if there's only 1 key.
          if( groups[g]->GetRemoveMode() == FreeKeyGroup::remove_inside ) {     // Remove keys inside range; no step or delta
            // Delete keyframes in the range, if applicable
            motion->DeleteRange( groups[g]->GetLow()+1, groups[g]->GetHigh()-1 );
          } else {
            // Remove keys outside range, if applicable
            if( groups[g]->GetRemoveMode() == FreeKeyGroup::remove_outside ) {
              if( FreeKeyGroup::remove_outside ) {
                motion->DeleteRange( 0, groups[g]->GetLow()-1 );
                motion->DeleteRange( groups[g]->GetHigh()+1, motion->GetLastFrameNumber() );
              }
            }

            // Scale keys, if applicable
            float scalar = 1.0;                    // Used in all later operations so they affect the newly modified range
            if( groups[g]->GetUseScaleKeys() ) {
              scalar = groups[g]->GetScaleKeys();
              motion->ScaleKeys( groups[g]->GetLow(), groups[g]->GetHigh(),
                                 groups[g]->GetScaleKeys() );
            }

            // Delete every Xth frame, if applicable
            if( groups[g]->GetUseStep() )
              motion->DeleteStep( groups[g]->GetLow(), (int)(groups[g]->GetHigh() * scalar),
                                  groups[g]->GetStep(), groups[g]->GetInvertStep() );

            // Shift the keys, if applicable
            if( groups[g]->GetUseDelta() )
              motion->ShiftKeys( groups[g]->GetLow(), (int)(groups[g]->GetHigh() * scalar),
                                 groups[g]->GetDelta() );
          }
        }

        out << line << endl;
        motion->WriteMotionToFile( out );

        output_line = false;
      }
    }

    // See if we should output the line or not
    if( output_line == false )
      output_line = true;
    else
      out << line << endl;

  } while( !in.eof() );

  return FREEKEY_OK;
}
