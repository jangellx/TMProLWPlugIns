//
// MotionReferencerInstance.cpp
//

#include <stdio.h>

#include <lwserver.h>
#include <lwmotion.h>

#include "MotionReferencerInstance.h"

GlobalFunc * MotionReferencerInstance::global = NULL;

// Constructor
MotionReferencerInstance::MotionReferencerInstance()
  : enabled(false), flags(0), ref_item_index(0), shift(0.0), scale(1.0) {

  // Initialize Strings
  ref_scene_filename[0]     = '\0';
  ref_item_name[0]          = '\0';
  ref_bone_name[0]          = '\0';

  instance_name[0]          = '\0';
  
  // Initialize the item array
  item[0] = NULL;                 // Item we are applied to
  item[1] = LWITEM_NULL;          // End of list
}

// Destructor
MotionReferencerInstance::~MotionReferencerInstance() {
  ;
}

// SetRefScene():
//  Set the reference scene's filename
bool MotionReferencerInstance::SetRefScene( const char *new_ref ) {
  // Check for NULL filename
  if( new_ref == NULL ) {
    ref_scene_filename[0] = '\0';
    return true;
  }

  strcpy( ref_scene_filename, new_ref );
    
  return true;
}

// SetRefItem():
//  Set the reference item
bool MotionReferencerInstance::SetRefItem( const char *new_ref ) {
  // Check for NULL filename
  if( new_ref == NULL ) {
    ref_item_name[0] = '\0';
    return true;
  }

  strcpy( ref_item_name, new_ref );
    
  return true;
}

// SetRefBone():
//  Set the reference bone
bool MotionReferencerInstance::SetRefBone( const char *new_ref ) {
  // Check for NULL filename
  if( new_ref == NULL ) {
    ref_bone_name[0] = '\0';
    return true;
  }

  strcpy( ref_bone_name, new_ref );
    
  return true;
}

// CopyFrom()
//  Copies another MotionReferencerInstance into this MotionReferencerInstance.
//   Note that this does a true copy of the LOD array, meaning that
//   each eliment is copied to a new, unique memeory location in
//   the other array (ie: no shared pointer elements between the two).
void MotionReferencerInstance::CopyFrom( MotionReferencerInstance &other_inst ) {
  strcpy( ref_scene_filename, other_inst.ref_scene_filename );
  strcpy( ref_item_name,      other_inst.ref_item_name      );
  strcpy( ref_bone_name,      other_inst.ref_bone_name      );

  enabled        = other_inst.enabled;
  ref_item_index = other_inst.ref_item_index;

  shift          = other_inst.shift;
  scale          = other_inst.scale;

  item[0]        = other_inst.item[0];
}

// ChangeItemIDs():
//  Called when LW changes the ItemIDs (usually after an item has been
//   deleted).  The items we currently care about are the camera (0) and
//   the item itself (1).
void MotionReferencerInstance::ChangeItemIDs( const LWItemID *new_items ) {
  int i = 0, j;

  while( new_items[i] != NULL ) {     // Scan through each item in the New List
    for( j = 0; j < 2; j++ ) {        // Test against each item in the Old List
      if( new_items[i] == item[j] ) {
        item[0] = new_items[i+1];     // Match Found; change the item ID
        break;
      }
    }
    i+=2;
  };
}

// LoadFromFile():
//  Loads instance data from the file specified
bool MotionReferencerInstance::LoadFromFile( const char *filename, char * &error ) {
  return false;
}

// SaveToFile():
//  Saves an instance to the file specified.  This does NOT handle
//   changing the extension of the file.
bool MotionReferencerInstance::SaveToFile( const char *filename, char * &error ) {
  return false;
}

// LoadFromScene():
//  Loads an instance to a scene file.  This does NOT handle
//   changing the extension of the file to .SDF
bool MotionReferencerInstance::LoadFromScene( char *error_buffer, const LWLoadState *loader ) {
/*
  char   buffer[8192];
  char * string;
  loader->SimpleRead( buffer, 8192 );

  // Filename Only Stored In .LWS Mode
  string = StringTools::FindWord( buffer, 0 );
  if( !string ) {
    strcpy( error_buffer, "MotionReferencer: Error occured reading plug-in data from scene (1)" );
    return false;
  }

  if( strnicmp( string, "SDFFilename", 11 ) == 0 ) {
    string = StringTools::FindWord( buffer, 1 );

    if( !string ) {
      sprintf( error_buffer, "MotionReferencer: Error occured reading plug-in data from scene (2):  %s", string );
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
      sprintf( error_buffer, "MotionReferencer Error: Unsupported version found in scene:  %g", version );
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
*/
  return true;
}

// SaveToScene():
//  Saves an instance to a scene file.
bool MotionReferencerInstance::SaveToScene( char *error_buffer, const LWSaveState *saver ) {
/*
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
*/
  return true;
}

// GenerateInstanceName():
void MotionReferencerInstance::GenerateInstanceName() {
  sprintf( instance_name, "Item Index:  %x", item[0] );
}


