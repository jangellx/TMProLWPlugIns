//
// Simplify_ObjRep_Instance.cpp
//

#include <assert.h>
#include <stdio.h>
#include <math.h>

#include "Simplify_ObjRep_Instance.h"
#include "Simplify_Generic.h"
#include "Simplify_Config.h"

#include <iomanip.h>

extern SimplifyGlobal simp_global;
extern SimplifyConfig simp_config;
extern float SDF_Version;

// Static Memeber Initialization
LWItemID     SimplifyInstance::camera_id = 0;
GlobalFunc * SimplifyInstance::global    = NULL;

// Constructor
SimplifyInstance::SimplifyInstance()
  : enabled(false), use_lod(false), use_preview(false),
    store_as_mode(0), saved_state(true),
    lod_use_camera_zoom(false), lod_nominal_camera_zoom(3.2f),
    lod_distance_multiplier(1.0f) {

  if( simp_config.GetSaveInScene() )
    store_as_mode = 1;
  
  // Initialize Strings
  preview_filename[0]     = '\0';
  render_filename[0]      = '\0';
  sdf_filename[0]         = '\0';
  replacement_filename[0] = '\0';
  
  // Initialize the item array
  item[0] = camera_id;
  item[1] = NULL;
  item[2] = LWITEM_NULL;

  CreateDefaultLOD();
}

// Destructor
SimplifyInstance::~SimplifyInstance() {
  lod.Flush();
}

// CreateDefaultLOD():
//  Creates the default LOD mode and adds it to the
//   LOD list
void SimplifyInstance::CreateDefaultLOD() {
  SimplifyLOD * def = new SimplifyLOD;
  def->SetFilename( " <Default Render Object> " );
  def->SetEnabled( true );

  def->GenerateListerName();

  lod.Add( def );
}

// SetPreview():
//  Set the preview object's filename
bool SimplifyInstance::SetPreview( const char *new_preview ) {
  // Check for NULL filename
  if( new_preview == NULL ) {
    preview_filename[0] = '\0';
    return true;
  }

  strcpy( preview_filename, new_preview );
    
  return true;
}

// SetRender():
//  Set the render object's filename
bool SimplifyInstance::SetRender( const char *new_render ) {
  // Check for NULL filename
  if( new_render == NULL ) {
    render_filename[0] = '\0';
    return true;
  }

  strcpy( render_filename, new_render );
    
  return true;
}

// SetSDF():
//  Set the sdf object's filename
bool SimplifyInstance::SetSDF( const char *new_sdf ) {
  // Check for NULL filename
  if( new_sdf == NULL ) {
    sdf_filename[0] = '\0';
    return true;
  }

  strcpy( sdf_filename, new_sdf );
    
  return true;
}

// CopyFrom()
//  Copies another SimplifyInstance into this SimplifyInstance.
//   Note that this does a true copy of the LOD array, meaning that
//   each eliment is copied to a new, unique memeory location in
//   the other array (ie: no shared pointer elements between the two).
void SimplifyInstance::CopyFrom( SimplifyInstance &other_inst ) {
  strcpy( render_filename,  other_inst.render_filename  );
  strcpy( preview_filename, other_inst.preview_filename );
  strcpy( sdf_filename,     other_inst.sdf_filename     );

  enabled       = other_inst.enabled;
  use_lod       = other_inst.use_lod;
  use_preview   = other_inst.use_preview;
  store_as_mode = other_inst.store_as_mode;
  saved_state   = other_inst.saved_state;
  item[1]       = other_inst.item[1];

  lod_use_camera_zoom     = other_inst.lod_use_camera_zoom;
  lod_nominal_camera_zoom = other_inst.lod_nominal_camera_zoom;
  lod_distance_multiplier = other_inst.lod_distance_multiplier;

  // Copy the LOD array
  lod.Flush();
  SimplifyLOD * new_lod;

  for( unsigned int i = 0; i < other_inst.lod.NumElements(); i++ ) {
    new_lod = new SimplifyLOD;
    new_lod->CopyFrom( *other_inst.lod[i] );
    lod.Add( new_lod );
  }
}

// ChangeItemIDs():
//  Called when LW changes the ItemIDs (usually after an item has been
//   deleted).  The items we currently care about are the camera (0) and
//   the item itself (1).
void SimplifyInstance::ChangeItemIDs( const LWItemID *new_items ) {
  int i = 0, j;

  while( new_items[i] != NULL ) {     // Scan through each item in the New List
    for( j = 0; j < 3; j++ ) {        // Test against each item in the Old List
      if( new_items[i] == item[j] ) {
        item[0] = new_items[i+1];     // Match Found; change the item ID
        break;
      }
    }
    i+=2;
  };
}

// EvaluateReplacement()
//  This does the actual task of selecting which filename to replace
//   the current object with.  Note that the replacement_filename
//   string persists in this class, but is not stored or copied.
const char * SimplifyInstance::EvaluateReplacement( ObjReplacementAccess *objrep ) {
  // Make sure the ItemIDs are the same.  They should be, anyway...
  assert( objrep->objectID == GetItemID() );

  // Set up the default new filename
  objrep->newFilename = replacement_filename;

  // Check to see if we're enabled
  if( !GetEnabled() ) {
    strcpy( replacement_filename, render_filename );
    return objrep->newFilename;
  }

  switch( objrep->curType ) {
    // No object previously loaded
    case OBJREP_NONE:
      switch( objrep->newType ) {
        case OBJREP_NONE:                                       // Still no object loaded; return NULL
          objrep->newFilename = NULL;
          break;

        case OBJREP_PREVIEW:                                    // Return a preview object, if enabled
          if( GetEnablePreview() && simp_global.preview )
            strcpy( replacement_filename, preview_filename );   //   Preview Enabled; use preview filename
          else {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
          }
          break;

        case OBJREP_RENDER:                                     // Return a Render or LOD object
          if( simp_global.render ) {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
            break;
          } else
            strcpy( replacement_filename, preview_filename );
      }

      break;

    // Preview object previously loaded
    case OBJREP_PREVIEW:
      switch( objrep->newType ) {
        case OBJREP_NONE:                                       // No object requested; return NULL
          objrep->newFilename = NULL;
          break;

        case OBJREP_PREVIEW:                                    // Still a Preview Object
          if( GetEnablePreview() && simp_global.preview ) {
            if( strcmp( objrep->curFilename, preview_filename ) == 0 )
              objrep->newFilename = NULL;
            else if( simp_global.render )
              strcpy( replacement_filename, preview_filename ); //   Preview Enabled; use preview filename
          } else {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else if( simp_global.render )
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
          }
          break;

        case OBJREP_RENDER:                                     // Return a Render or LOD object
          if( simp_global.render ) {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else if( simp_global.render )
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
          } else {
            strcpy( replacement_filename, preview_filename );
          }
          break;

      }
      break;

    // Render object previously loaded
    case OBJREP_RENDER:
      switch( objrep->newType ) {
        case OBJREP_NONE:                                       // No object requested; return NULL
          objrep->newFilename = NULL;
          break;

        case OBJREP_PREVIEW:                                    // Still a Preview Object
          if( GetEnablePreview() && simp_global.preview ) {
            strcpy( replacement_filename, preview_filename );   //   Preview Enabled; use preview filename
          } else {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else if( simp_global.render )
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
          }
          break;

        case OBJREP_RENDER:                                     // Return a Render or LOD object
          if( simp_global.render ) {
            if( GetEnableLOD() && simp_global.lod )
              strcpy( replacement_filename, PickLOD( objrep->newTime ) );
            else
              strcpy( replacement_filename, render_filename );  //   Preview and LOD Disabled; use render filename
          } else {
            strcpy( replacement_filename, preview_filename );
          }
          break;
      }
      break;
  }
  
  // Test to see if the objects are the same.  Return NULL if they are (saves load time; Layout won't load another object if the new one is null)
  if( stricmp( replacement_filename, objrep->curFilename ) == 0 )
    objrep->newFilename = NULL;
  else
    objrep->newFilename = replacement_filename;

  return objrep->newFilename;
}

// PickLOD()
//  Picks an LOD based on the distance from the camera to the item.
//   This does NOT check to see if LOD Mode is enabled or not, save
//   for within an LOD instance itself.
const char * SimplifyInstance::PickLOD( LWTime lw_time ) {
  double item_pos[3], camera_pos[3];


  // Get the Item Positions
  CLWItemInfo *item_info = CLWItemInfo::New( (LWItemInfo *)( (*global)("LW Item Info", GFUSE_TRANSIENT) ) );

  item_info->Param( GetItemID(),   LWIP_W_POSITION, lw_time, item_pos   );
  item_info->Param( GetCameraID(), LWIP_W_POSITION, lw_time, camera_pos );

  LWCameraInfo *camerainfo = (LWCameraInfo *)( (*global)( "LW Camera Info", GFUSE_TRANSIENT ) );
  float zoom_factor = (float)(*camerainfo->zoomFactor)( GetCameraID(), lw_time );

  delete item_info;

  // Compute the distance
  float dist = (float) sqrt( ((item_pos[0] - camera_pos[0]) * (item_pos[0] - camera_pos[0])) +
                             ((item_pos[1] - camera_pos[1]) * (item_pos[1] - camera_pos[1])) +
                             ((item_pos[2] - camera_pos[2]) * (item_pos[2] - camera_pos[2])) ); 

  dist *= GetLODDistanceMultiplier();   // Modify by the LOD Distance Modifier

/*
  static int count = 0;
  ofstream out;
  if( count == 0 ) {
    out.open( "i:\\testsimp.txt" );
    out << " C           Orig             Zoom           Mod         +60" << endl;
  } else
    out.open( "i:\\testsimp.txt", ios::app );
  out << setw(3) << count++ << "      " << setw(10) << dist << "     " << setw(10) << zoom_factor;
*/
  if( GetLODUseCameraZoom() )
    dist *= GetLODNominalCameraZoom()/zoom_factor;
/*
  out << "     " << setw(10) << dist;
// Delete Me
  // Get the Item Positions
  item_info = CLWItemInfo::New( (LWItemInfo *)( (*global)("LW Item Info", GFUSE_TRANSIENT) ) );

  item_info->Param( GetItemID(),   LWIP_W_POSITION, lw_time + 2.0f, item_pos   );
  item_info->Param( GetCameraID(), LWIP_W_POSITION, lw_time + 2.0f, camera_pos );

  camerainfo = (LWCameraInfo *)( (*global)( "LW Camera Info", GFUSE_TRANSIENT ) );

  delete item_info;

  // Compute the distance
  float dist2 = (float) sqrt( ((item_pos[0] - camera_pos[0]) * (item_pos[0] - camera_pos[0])) +
                              ((item_pos[1] - camera_pos[1]) * (item_pos[1] - camera_pos[1])) +
                              ((item_pos[2] - camera_pos[2]) * (item_pos[2] - camera_pos[2])) ); 

  dist *= GetLODDistanceMultiplier();   // Modify by the LOD Distance Modifier
  out << "     " << setw(10) << dist2 << endl;
  out.close();
// Stop Here
*/

  // Figure out which enabled LOD to use
  unsigned int keeper = -1;
  for( unsigned int i=0; i < lod.NumElements(); i++ ) {
    if( lod[i]->GetEnabled() ) {
      if( !(lod[i]->GetDistance() > dist) )
        keeper = i;
      else
        break;
    }
  }

  // No enabled LODs are in range; return the first one that is enabled
  if( keeper == -1 ) {
    if( lod[0]->GetEnabled() )
      return GetRender();

    for( i = 1; i < lod.NumElements(); i++ ) {
      if( lod[i]->GetEnabled() )
        return lod[i]->GetFilename();
    }
    
    // No LODs are enabled; return NULL
    return NULL;
  }

  // Return the filename of the match
  if( keeper == 0 )
    return GetRender();
  else
    return lod[ keeper ]->GetFilename();

}


// LoadFromFile():
//  Loads instance data from the .SDF file specified
bool SimplifyInstance::LoadFromFile( const char *filename, char * &error ) {
  if( filename == NULL ) {
    error = "Simplify Load Error: The filename provided is invalid";
    return false;
  }

  if( !DirInfo::Exists( filename ) ) {
    error = "Simplify Load Error: This .SDF filename doesn't exists:";
    return false;
  }

  ifstream in( filename, ios::nocreate );
  if( !in ) {
    error = "Simplify Load Error: Unable to open .SDF file";
    return false;
  }

  char buffer[8192];
  in >> buffer;
  if( strncmp( buffer, "SDF", 3 ) != 0 ) {
    error = "Simplify Load Error: This is not an .SDF file:";
    return false;
  }

  float version;
  in >> version;
  if( version != SDF_Version ) {
    error = "Simplify Load Error: Unsupported .SDF version in";;
    return false;
  }

  // Initial Setup
  enabled                 = false;
  use_preview             = false;
  use_lod                 = false;
  lod_use_camera_zoom     = false;
  lod_nominal_camera_zoom = 3.2f;
  lod_distance_multiplier = 1.0f;

  // Reset the LOD List
  lod.Flush();
  CreateDefaultLOD();
  lod[0]->SetEnabled( false );

  // Data Loop  
  SimplifyLOD *new_lod;
  bool read_new_word = true;

  do {
    if( read_new_word )
      in >> buffer;
    else
      read_new_word = true;

    if( strcmp( buffer, "Enabled" ) == 0 )
      enabled = true;
    else if( strcmp( buffer, "EnablePreview" ) == 0 )
      use_preview = true;
    else if( strcmp( buffer, "EnableLOD" ) == 0 )
      use_lod = true;
    else if( strcmp( buffer, "RenderFilename" ) == 0 )
      in >> render_filename;
    else if( strcmp( buffer, "PreviewFilename" ) == 0 )
      in >> preview_filename;
    else if( strcmp( buffer, "LODUseCameraZoom" ) == 0 )
      SetLODUseCameraZoom( true );
    else if( strcmp( buffer, "LODNominalCameraZoom" ) == 0 )
      in >> lod_nominal_camera_zoom;
    else if( strcmp( buffer, "LODDistanceMultiplier" ) == 0 )
      in >> lod_distance_multiplier;
    else if( strcmp( buffer, "LODEnableDefault" ) == 0 )
      lod[0]->SetEnabled( true );
    else if( strcmp( buffer, "LODList" ) == 0 ) {
      in >> buffer;                                      // Skip the {
      do {
        in >> buffer;
        if( strcmp( buffer, "LODObject" ) == 0 ) {
          new_lod = new SimplifyLOD;
          if( new_lod->LoadFromStream( in ) )
            lod.Add( new_lod );

          // Check for errors
          if( !in && !in.eof() ) {
            error = "Simplify Load Error: File error occured when reading from .SDF file";
            return false;
          }
        } else {
          read_new_word = true;
          break;                   // Presumably, this is a }, so stop
        }
      } while( !in.eof() );
    }


    // Check for errors
    if( !in && !in.eof() ) {
      error = "Simplify Load Error: File error occured when reading from .SDF file";
      return false;
    }

  } while( !in.eof() );

  // Set the SDF Filename
  strcpy( sdf_filename, filename );

  return true;
}

// SaveToFile():
//  Saves an instance to the file specified.  This does NOT handle
//   changing the extension of the file to .SDF
bool SimplifyInstance::SaveToFile( const char *filename, char * &error ) {
  if( filename == NULL ) {
    error = "Simplify Save Error: The filename provided is invalid";
    return false;
  }
  
  ofstream out( filename );
  if( !out ) {
    error = "Simplify Save Error: Unable to write to .SDF file";
    return false;
  }

  // Output the header
  out << "SDF " << SDF_Version << endl;

  // Output basic Simplify Data
  if( GetEnabled() )
    out << "Enabled" << endl;

  out << "RenderFilename "    << GetRender()             << endl;

  if( GetPreview()[0] != '\0' )
    out << "PreviewFilename " << GetPreview()            << endl;

  if( GetEnablePreview() )
    out << "EnablePreview" << endl;

  if( GetEnableLOD() )
    out << "EnableLOD" << endl;


  if( GetLODUseCameraZoom() )
    out << "LODUseCameraZoom" << endl;

  out << "LODNominalCameraZoom " << GetLODNominalCameraZoom() << endl;
  out << "LODDistanceMultiplier " << GetLODDistanceMultiplier() << endl;
 
  if( lod[0]->GetEnabled() )
    out << "LODEnableDefault" << endl;

  if( lod.NumElements() > 1 ) {
    out << "LODList {" << endl;

    for( unsigned int i = 1; i < lod.NumElements(); i++ )
      lod[i]->SaveToStream( out );

    out << "  }" << endl;
  }

  // Test for errors
  if( !out ) {
    error = "Simplify Save Error: File error occured when writing to .SDF file";
    return false;
  }

  // Set the SDF Filename
  strcpy( sdf_filename, filename );

  return true;
}

// LoadFromScene():
//  Loads an instance to a scene file.  This does NOT handle
//   changing the extension of the file to .SDF
bool SimplifyInstance::LoadFromScene( char *error_buffer, CLWLoadState *loader ) {
  char   buffer[8192];
  char * string;
  loader->SimpleRead( buffer, 8192 );

  // Filename Only Stored In .LWS Mode
  string = StringTools::FindWord( buffer, 0 );
  if( !string ) {
    strcpy( error_buffer, "Simplify: Error occured reading plug-in data from scene (1)" );
    return false;
  }

  if( strnicmp( string, "SDFFilename", 11 ) == 0 ) {
    string = StringTools::FindWord( buffer, 1 );

    if( !string ) {
      sprintf( error_buffer, "Simplify: Error occured reading plug-in data from scene (2):  %s", string );
      return false;
    }

    if( !LoadFromFile( string, error_buffer ) )
      return false;
    else {
      store_as_mode = 0;
      return true;
    }
  } else if( strnicmp( string, "SDF", 3 ) == 0 ) {
    // -----------------------

    // All Data Stored In .LWS Mode

    // Load the Version
    string = StringTools::FindWord( buffer, 1 );
    float version = (float)atof( string );
    if( version != SDF_Version ) {
      sprintf( error_buffer, "Simplify Error: Unsupported SDF version found in scene:  %g", version );
      return false;
    }

    // Initial Setup
    enabled     = false;
    use_preview = false;
    use_lod     = false;

    // Reset the LOD List
    lod.Flush();
    CreateDefaultLOD();
    lod[0]->SetEnabled( false );

    // Data Loop  
    SimplifyLOD *new_lod;
    int  read_val;
    bool read_new_line = true;

    do {
      if( read_new_line )
        read_val = loader->SimpleRead( buffer, 8192 );
      else
        read_new_line = true;
  
      if( read_val == -1 )
        break;
      
      string = StringTools::FindWord( buffer, 0 );    // Skip any leading spaces
      if( string != NULL ) {                          // Skip any blank lines
        if( string[0] == '}' )                        // Stop on find a closing brace
          break;
        else if( strnicmp( string, "Enabled", 7 ) == 0 ) {
          enabled = true;
        } else if( strnicmp( string, "EnablePreview", 13 ) == 0 ) {
          use_preview = true;
        } else if( strnicmp( string, "EnableLOD", 9 ) == 0 ) {
          use_lod = true;
        } else if( strnicmp( string, "RenderFilename", 14 ) == 0 ) {
          string = StringTools::FindWord( buffer, 1 );
          if( string == NULL ) {
            sprintf( error_buffer, "Simplify Error: Value missing after keyword RenderFilename" );
            return false;
          }
          strcpy( render_filename, string );
        } else if( strnicmp( string, "LODUseCameraZoom", 16 ) == 0 ) {
          SetLODUseCameraZoom( true );
        } else if( strnicmp( string, "LODNominalCameraZoom", 20 ) == 0 ) {
          string = StringTools::FindWord( buffer, 1 );
          if( string == NULL ) {
            sprintf( error_buffer, "Simplify Error: Value missing after keyword LODNominalCameraZoom" );
            return false;
          }
          SetLODNominalCameraZoom( (float)atof( string ) );
        } else if( strnicmp( string, "LODDistanceMultiplier", 21 ) == 0 ) {
          string = StringTools::FindWord( buffer, 1 );
          if( string == NULL ) {
            sprintf( error_buffer, "Simplify Error: Value missing after keyword LODDistanceMultiplier" );
            return false;
          }
          SetLODDistanceMultiplier( (float)atof( string ) );
        } else if( strnicmp( string, "PreviewFilename", 15 ) == 0 ) {
          string = StringTools::FindWord( buffer, 1 );
          if( string == NULL ) {
            sprintf( error_buffer, "Simplify Error: Value missing after keyword PreviewFilename" );
            return false;
          }
          strcpy( preview_filename, string );
        } else if( strnicmp( buffer, "LODEnableDefault", 16 ) == 0 ) {
            lod[0]->SetEnabled( true );
        } else if( strnicmp( string, "LODList", 7 ) == 0 ) {
          do {
            read_val = loader->SimpleRead( buffer, 8192 );
  
            if( read_val == -1 )
              break;

            string = StringTools::FindWord( buffer, 0 );
            if( string != NULL ) {                    // Skip any blank lines
              if( strnicmp( string, "LODObject", 9 ) == 0 ) {
                new_lod = new SimplifyLOD;
                if( new_lod->LoadFromScene( loader ) )
                  lod.Add( new_lod );
              } else {
                read_new_line = false;
                break;                // Presumably, this is a closing brace, so stop reading in LOD stuff
              }
            }
          } while( true );
        }
      }
    } while( read_val != -1 );

    store_as_mode = 1;
  } else {
    sprintf( error_buffer, "Simplify: Unknown SDF data found in scene: \"%s\"", string );
    return false;
  }

  return true;
}

// SaveToScene():
//  Saves an instance to a scene file.
bool SimplifyInstance::SaveToScene( char *error_buffer, CLWSaveState *saver ) {
  // Set up some variables
  char buffer[2048];

  // Store Filename In .LWS Mode
  if( GetStoreAs() == 0 ) {
    // Make sure the SDF exists and has been saved
    if( GetSDF()[0] == '\0' ) {
      sprintf( error_buffer, "Simplify Error:  You must save the .SDF before it's name can be stored in the .LWS for \"%s\"!", GetRender() );
      return false;
    }
    
    sprintf( buffer, " SDFFilename %s", GetSDF() );
    saver->SimpleWrite( buffer, strlen( buffer ) );

    return true;
  }

  // --------------------------------------------

  // Store All Data In .LWS Mode

  // Save out the header and version
  sprintf( buffer, " SDF %.2f {", SDF_Version );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  // Output basic Simplify Data
  if( GetEnabled() )
    saver->SimpleWrite( " Enabled", strlen( " Enabled" ) );

  sprintf( buffer, " RenderFilename %s", GetRender() );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  if( GetPreview()[0] != '\0' ) {
    sprintf( buffer, " PreviewFilename %s", GetPreview() );
    saver->SimpleWrite( buffer, strlen( buffer ) );
  }

  if( GetEnablePreview() )
    saver->SimpleWrite( " EnablePreview", strlen( " EnablePreview") );

  if( GetEnableLOD() )
    saver->SimpleWrite( " EnableLOD", strlen( " EnableLOD") );

  if( GetLODUseCameraZoom() )
    saver->SimpleWrite( " LODUseCameraZoom", strlen( " LODUseCameraZoom") );

  sprintf( buffer, " LODNominalCameraZoom %g", GetLODNominalCameraZoom() );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  sprintf( buffer, " LODDistanceMultiplier %g", GetLODDistanceMultiplier() );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  if( lod[0]->GetEnabled() )
    saver->SimpleWrite( " LODEnableDefault", strlen( " LODEnableDefault" ) );

  if( lod.NumElements() > 1 ) {
    saver->SimpleWrite( " LODList {", strlen( " LODList {") );

    for( unsigned int i=1; i < lod.NumElements(); i++ )
      lod[i]->SaveToScene( saver );
  }

  saver->SimpleWrite( " }", strlen( " }" ) );
    
  return true;
}


//
// SimplifyLOD Functions
//

// Constructor
SimplifyLOD::SimplifyLOD()
  : enabled(false), swap_distance(0.0) {

  SetFilename( NULL );
  GenerateListerName();
}

// Destructor
SimplifyLOD::~SimplifyLOD() {
  ;
}

// SetFilename():
//  Set the filename object's filename
bool SimplifyLOD::SetFilename( const char *new_filename ) {
  // Check for NULL filename
  if( new_filename == NULL ) {
    filename[0] = '\0';
  } else {
    strcpy( filename, new_filename );
  }

  GenerateListerName();
  return true;
}

// GenerateListerName():
//  Geneartes a name for this degrade that appears in the
//   LOD Listbox.
const char * SimplifyLOD::GenerateListerName() {
  // Set the enabled/disabled modes
  if( enabled )
    strcpy( lister_name, "Y   " );
  else
    strcpy( lister_name, "N   " );

  char *name = DirStrings::HasFile( filename );
  if( name == NULL )
    strcat( lister_name, "<No Object Selected>" );
  else if( name[0] == '\0' )
    strcat( lister_name, "<No Object Selected>" );
  else
    strcat( lister_name, name );

  return lister_name;
}

// CopyFrom()
//  Copies another SimplifyLOD instance into this SimplifyLOD instance.
void SimplifyLOD::CopyFrom( SimplifyLOD &other_inst ) {
  enabled       = other_inst.enabled;
  swap_distance = other_inst.swap_distance;

  strcpy( filename,    other_inst.filename    );
  strcpy( lister_name, other_inst.lister_name );
}

// LoadFromStream():
//  Loads an LOD instance from the stream provided.
//   Note that this assumes that the LODObject line
//   has already been read in.
bool SimplifyLOD::LoadFromStream( ifstream &in ) {
  char  buffer[1024];

  enabled = false;

  in >> buffer;  // Skip the {
  do {
    in >> buffer;

    if( strcmp( buffer, "Enabled" ) == 0 ) {
      enabled = true;
    } else if( strcmp( buffer, "Filename" ) == 0 ) {
      in >> filename;
    } else if( strcmp( buffer, "SwapDistance" ) == 0 ) {
      in >> swap_distance;
    }

    // Check for errors
    if( !in && !in.eof() )
      return false;
  } while( (buffer[0] != '}') && !in.eof());  // Search until we find }

  GenerateListerName();
  return true;
}

// SaveToStream():
//  Saves an LOD instance to the stream provided
bool SimplifyLOD::SaveToStream( ofstream &out ) {
  out << "  LODObject { " << endl;
  if( enabled )
    out << "    Enabled" << endl;
  out << "    Filename "     << GetFilename()     << endl;
  out << "    SwapDistance " << GetDistance()     << endl;
  out << "    }" << endl;

  if( !out )
    return false;

  return true;
}

// LoadFromScene():
//  Loads an instance to a scene file.  This does NOT handle
//   changing the extension of the file to .SDF
bool SimplifyLOD::LoadFromScene( CLWLoadState *loader ) {
  char   buffer[1024];
  char * string;

  enabled = false;

  do {
    if( loader->SimpleRead( buffer, 8192 ) == -1 )
      return false;

    string = StringTools::FindWord( buffer, 0);

    if( strnicmp( string, "Enabled", 7 ) == 0 ) {
      enabled = true;
    } else if( strnicmp( string, "Filename", 8 ) == 0 ) {
      string = StringTools::FindWord( buffer, 1 );
      strcpy( filename, string );
    } else if( strnicmp( string, "SwapDistance", 12 ) == 0 ) {
      string = StringTools::FindWord( buffer, 1 );
      swap_distance = (float)atof( string );
    } else if( string[0] == '}' )
      break;

    // Check for errors
  } while( true );  // Search until we find }

  GenerateListerName();
  return true;
}

// SaveToScene():
//  Saves an LOD isntance to a scene file
bool SimplifyLOD::SaveToScene( CLWSaveState *saver ) {
  // Set up some variables
  char buffer[2048];

  saver->SimpleWrite( "  LODObject {", strlen( "   LODObject {" ) );
  
  if( enabled )
    saver->SimpleWrite( "   Enabled", strlen( "   Enabled" ) );

  sprintf( buffer, "   Filename %s", GetFilename() );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  sprintf( buffer, "   SwapDistance %g", GetDistance() );
  saver->SimpleWrite( buffer, strlen( buffer ) );

  saver->SimpleWrite( "   }", strlen( "   }" ) );

  return true;
}
