//
// Populate.cpp
//
// This file includes the implementation of the Populate
//  class's functions, as found in Populate.h
//

// Includes
#include <stdlib.h>
#include <time.h>

#include <portable/configfstream/configfstream.h>

#include "../../FreeKey/FreeKeyMotion.h"

#include "Populate.h"

// Error Strings
const char *populate_error_strings[] = {
  "Everything's OK!",
  "The source scene file is does not exist!",
  "The source scene file isn't a Lightwave Scene!",
  "The Point Reference Object isn't a Lightwave Object!",
  "File error occured reading the source scene!",
  "Don't use me -- I'm just the error enum count"
};

const char *populate_rep_error_strings[] = {
  "Everything's OK!",
  "The replacement path does not exist!",
  "Some bad object files were found in the replacement path!",
  "An error occured trying to open the replacement path!",
  "Don't use me -- I'm just the error enum count"
};

// Constructor
Populate::Populate( const char *_source_name,
                    const char *_output_name,
                    const char *_item_name )
  : CLIFileManipBase( _source_name, _output_name ),
  delta(0), scale(0), seed(0), use_lwo(false), display_tree(false),
  first_point(0), _lwo(NULL), point_count(-1), pnts_start(0), base(NULL),
  use_uniform_scale( false ), items_processed(0), lights_processed(0),
  rotation_mode(random), position_mode(random), scale_mode(random),
  delta_mode(random), scale_time_mode(random), use_repdir( false ) {

  // Create Base
  base = new BatarangHierarchy;
  base->SetItemType( BH_ITEM_SCENE );
  base->SetFullName( source_name );

  // Initialize the strings
  SetLWO( NULL );
  SetItem( NULL );
  SetRepDir( NULL );

  // Initialiize the vectors
  for( int i=0; i < 3; i++ )
    t[i] = y[i] = h[i] = 0.0f;
}

// Destructor
Populate::~Populate() {
  delete _lwo;
  delete base;
}

// OpenFiles():
//  Tests to make sure the paths are valid and opens the files.
//  This also generates an output and ubernull name if none are
//  specified.  Returns an error if something went wrong.
int Populate::OpenFiles() {
  GenerateNames( "-Populated" );

  // Open the LWO File, if needed
  if( use_lwo ) {
    _lwo = new bifstream( lwo_name, ios::nocreate );
    if( !(*_lwo) ) {
      CloseFiles();
      return POPULATE_CANT_OPEN_LWO;
    }
    _lwo->SetEndian( ENDIAN_MOTOROLA );
  }

  int error = CLIFileManipBase::OpenFiles();
  if( error != POPULATE_OK )
    return error;

  // Test the file headers (only happens if the input files were opened OK)
  char header[4];
  if( _in != NULL ) {
    // Test the LWS header
    _in->read( header, 4 );
    if( strncmp( header, "LWSC", 4 ) != 0 )
      return POPULATE_SOURCE_ISNT_LWS;
    _in->seekg( 0, ios::beg );
  }

  if( _lwo != NULL ) {
    // Test the LWO header
    if( use_lwo ) {
      _lwo->seekg( 8, ios::beg );
      _lwo->read( header, 4 );
      if( strncmp( header, "LWOB", 4 ) != 0 )
        return POPULATE_OBJECT_ISNT_LWO;
      _lwo->seekg( 0, ios::beg );
    }
  }

  return POPULATE_OK;
}

// CloseFiles():
//  Close the source and output files
void Populate::CloseFiles() {
  delete _lwo;
  _lwo = NULL;

  CLIFileManipBase::CloseFiles();
}

// GetErrorString():
//  Returns the string referenced by the error enum passed in.
const char * Populate::GetErrorString( int error ) {
  if( error < POPULATE_ERROR_BASE )
    return clifilemanipbase_error_strings[ error ];
  else
    return populate_error_strings[ error - POPULATE_ERROR_BASE ];
}

// FindPNTS():
//  Finds the PNTS chunk in the LWO.  Returns false if the chunk
//   wasn't found or if another error occured.  Also sets the point
//   count so GetLWOPointCount() will work.
bool Populate::FindPNTS() {
  // Check to see if we're using an LWO.
  if( !use_lwo )
    return true;        // Not using it, but it's not an error, so we return true

  bifstream &lwo = *_lwo;

  char chunk_id[4];
  long chunk_size;

  lwo.read( chunk_id, 4 );
  lwo >> chunk_size;
  lwo.read( chunk_id, 4 );

  do {
    // Check for errors
    if( (lwo.bad() || lwo.fail()) && !lwo.eof() )
      return false;

    lwo.read( chunk_id, 4 );
    lwo >> chunk_size;

    if( lwo.eof() )
      break;

    // Chunk Comparisons
    if( strncmp( chunk_id, "PNTS", 4 ) == 0 ) {      // Points Chunk
      pnts_start = lwo.tellg();                      //   Store where the Points Chunk Data is
      point_count = chunk_size/12;                   //   Compute the chunk size
      if( first_point < point_count )
        lwo.seekg( first_point * 12, ios::cur );     //   Seek to the first point we'll be using

      return true;                                   // We're done; exit out
    } else {                                         // Unknown Chunk; skip it
      lwo.seekg( chunk_size, ios::cur );
   }
  } while( !lwo.eof() );

  return false;
}

// LoadReplacementPath():
//  Tests the Replacement Path and fills the Replacement Array
//   with the objects within.  Returns POPULATEREP_ enums
populate_rep_error_ids Populate::LoadReplacementPath() {
  if( !GetUseRepDir() )
    return POPULATEREP_OK;

  if( !DirInfo::Exists( GetRepDir() ) )
    return POPULATEREP_PATH_DOESNT_EXIST;

  DirInfo di( GetRepDir() );
  if( di.FindFirstMatch() == NULL )
    return POPULATEREP_PATH_DOESNT_EXIST;

  if( di.IsDir() ) {
    // Insert dir handling code here
  } else {
    // File-Handling Code
    Configifstream cfg( GetRepDir() );
    if( !cfg )
      return POPULATEREP_ERROR_OPENING_PATH;

    float prob;
    char  path[ MAX_PATH_LENGTH ];
    do {
      if( cfg.ReadNextLine() > -1 ) {
        cfg >> prob;
        cfg.SkipWhiteSpace();
        cfg.getline( path, MAX_PATH_LENGTH );

        PopulateReplacement *rep = new PopulateReplacement;
        rep->SetProb( prob );
        rep->SetFilename( path );
        rep->SetIsValid( DirInfo::Exists( path ) );

        rep_list.Add( rep );
      } else
        break;
    } while( !cfg.eof() );

    NormalizeReplacements();
  }

  return POPULATEREP_OK;
}

// NormalizeReplacements():
//  Adjusts the probabilities of the Replacement Objects to
//   the range of 0.0 to 1.0, assigning each it's own unique
//   space within that range.
void Populate::NormalizeReplacements() {
  if( rep_list.NumElements() == 0 )
    return;

  // Fetch the last rep's probability before it is processed
  float highest_prob = rep_list[ rep_list.NumElements() - 1 ]->GetProb();

  // Give each probability a unique space
  float base = 0.0f;
  for( unsigned int i=0; i < rep_list.NumElements(); i++ )
    rep_list[i]->SetProb( base += rep_list[i]->GetProb() );

  // First find the high and low probabilities
  float low_prob  = rep_list[0]->GetProb();
  float high_prob = rep_list[ rep_list.NumElements() - 1 ]->GetProb();

  // Test for max. of 0; do this so we don't divide by 0 later on
  if( high_prob == 0 )
    return;

  // Normalize the replacements
  float normalizer = 1/(high_prob + highest_prob);
  for( i=0; i < rep_list.NumElements(); i++ )
    rep_list[i]->SetProb( (rep_list[i]->GetProb() - low_prob) * normalizer );
}

// Process():
//  Actually processes the scene file.  It does everything related
//   to processing, returning POPULATE_OK if there are no errors.
int Populate::Process() {
  ifstream &in  = *_in;
  ofstream &out = *_out;

  bifstream &lwo = *_lwo;        // BIG NOTE:  this is only valid if use_lwo is true!!!

  char line[8192];

  int  object_index  = 0;
  int  light_index   = 0;
  bool output_line = true;
  bool in_light;
  bool do_process;
  bool is_base;
  BatarangHierarchy * parent_base = NULL;

  int  in_item_name_start = 0;
  char in_item_name[ MAX_PATH_LENGTH ] = "\0";

  lights_processed = 0;
  items_processed  = 0;
  items_replaced   = 0;

  float point_pos[3];

  // Seed the random number generator
  if( seed == 0 )
    srand( (unsigned)time( NULL ) );
  else 
    srand( seed );

  // Begin Processing
  do {
    in.getline( line, 8192 );
    if( (in.bad() || in.fail()) && !in.eof() )
      return POPULATE_ERROR_READING_FROM_SOURCE;

    // Test for an item's name
    if( strnicmp( line, "LoadObject",    10 ) == 0 ) {
      in_light = false;
      in_item_name_start = 11;
    }

    if( strnicmp( line, "AddNullObject", 13 ) == 0 ) {
      in_light = false;
      in_item_name_start = 14;
    }

    // -- No need to test for bones; we always process them

    if( strnicmp( line, "LightName",     9 ) == 0 ) {
      in_light = true;
      in_item_name_start = 10;
    }

    if( in_item_name_start > 0 ) {
      // Handle Replacement Objects Here
      strcpy( in_item_name, DirStrings::HasFile( &(line[in_item_name_start]) ) );
      if( !in_light ) {
        if( strnicmp( repitem_name, in_item_name, strlen( repitem_name ) ) == 0 ) {
          // Pick a replacement object
          float rep = ((float)rand())/RAND_MAX;
          for( long i=(long)rep_list.NumElements() - 1; i > -1; i-- ) {
            if( rep >= rep_list[i]->GetProb() )
              break;
          }

          // Strip the content dir, if needed
          char new_filename[ MAX_PATH_LENGTH ];
          if( strncmp( GetContentDir(), rep_list[i]->GetFilename(), strlen( GetContentDir() ) - 1 ) == 0 )
            strcpy( new_filename, &(rep_list[i]->GetFilename()[ strlen( GetContentDir() ) ]) );
          else
            strcpy( new_filename, rep_list[i]->GetFilename() );

          // Output the new line
          out << "LoadObject " << new_filename << endl;
          output_line = false;

          items_replaced++;
        }
      }

      in_item_name_start = 0;
    }

    // Find an ObjectMotion and mark it.
    if( ( strncmp( line, "ObjectMotion", 12 ) == 0 ) ||
        ( strncmp( line, "BoneMotion",   10 ) == 0 ) ||
        ( strncmp( line, "LightMotion",  11 ) == 0 )) {
      do_process = false;

      // Get a pointer to this item in the scene
      if( in_light )
        parent_base = base->FindIndexInScene( -(++light_index) );
      else
        parent_base = base->FindIndexInScene( ++object_index );

      // See if this is the base object
      if( strnicmp( parent_base->GetName(), GetItem(), strlen( GetItem() ) ) == 0 ) {
        is_base = true;
        do_process = true;
      } else {
        is_base = false;
        // See if the item has a parent that matches the item name from the user
        if( parent_base != NULL ) {
          parent_base = parent_base->HasAncestorNamed( GetItem(), strlen( GetItem() ) );
          if( parent_base != NULL )
            do_process = true;
        }
      }

      // See if we should process this item
      if( do_process ) {
        if( in_light ) {  // Technically, this class shouldn't output any text, but it's useful for debugging
          if( is_base )
            cerr << "   Processing Light \"" << in_item_name << "\", match " << lights_processed << "(base)..." << endl;
          else
            cerr << "   Processing Light \"" << in_item_name << "\", match " << lights_processed << "..." << endl;
          lights_processed++;
        } else {
          if( is_base )
            cerr << "   Processing Object \"" << in_item_name << "\", match " << items_processed << "(base)..." << endl;
          else
            cerr << "   Processing Object \"" << in_item_name << "\", match " << items_processed << "..." << endl;
          items_processed++;
        }

        // Load the motion in
        FreeKeyMotion * motion = FreeKeyMotion::NewMotionFromFile( in );
        if( !motion ) {
          cerr << " ERROR: failed to load motion for \"" << in_item_name << "\"; skipping" << endl;
          break;
        }

        // Adjust the motion
        if( is_base ) {  // Only apply Shift Values to the base object, not to it's children!
          // Shift the keys' positions by the positions of the LWO's points, if applicable
          if( use_lwo ) {
            if( (point_count * 12) >= (lwo.tellg() - pnts_start) ) {  // Make sure there are still some points left in the object
              lwo >> point_pos[0] >> point_pos[1] >> point_pos[2];

              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::x, point_pos[0] );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::y, point_pos[1] );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::z, point_pos[2] );
            }
          }

          if( GetPositionMode() == absolute ) {          // Position
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::x,  GetPosition()[0] );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::y,  GetPosition()[1] );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::z,  GetPosition()[2] );
          } else if( GetPositionMode() == replace ) {
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::x,  GetPosition()[0] );
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::y,  GetPosition()[1] );
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::z,  GetPosition()[2] );
          } else {
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::x,  ( ((GetPosition()[0] *  2) * (((float)rand())/RAND_MAX)) - GetPosition()[0] ) );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::y,  ( ((GetPosition()[1] *  2) * (((float)rand())/RAND_MAX)) - GetPosition()[1] ) );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::z,  ( ((GetPosition()[2] *  2) * (((float)rand())/RAND_MAX)) - GetPosition()[2] ) );
          }

          if( GetRotationMode() == absolute ) {          // Rotation
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::h,  GetRotation()[0] );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::p,  GetRotation()[1] );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::b,  GetRotation()[2] );
          } else if( GetRotationMode() == replace ) {
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::h,  GetRotation()[0] );
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::p,  GetRotation()[1] );
            motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::b,  GetRotation()[2] );
          } else {
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::h,  ( ((GetRotation()[0] *  2) * (((float)rand())/RAND_MAX)) - GetRotation()[0] ) );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::p,  ( ((GetRotation()[1] *  2) * (((float)rand())/RAND_MAX)) - GetRotation()[1] ) );
            motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::b,  ( ((GetRotation()[2] *  2) * (((float)rand())/RAND_MAX)) - GetRotation()[2] ) );
          }

          if( GetScaleMode() == absolute ) {          // Scaling
            if( use_uniform_scale ) {
              float uni_scale = GetScale()[0];
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, uni_scale );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, uni_scale );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, uni_scale );
            } else {
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, GetScale()[0] );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, GetScale()[1] );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, GetScale()[2] );
            }
          } else if( GetScaleMode() == replace ) {
            if( use_uniform_scale ) {
              float uni_scale = GetScale()[0];
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, uni_scale );
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, uni_scale );
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, uni_scale );
            } else {
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, GetScale()[0] );
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, GetScale()[1] );
              motion->SetValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, GetScale()[2] );
            }
          } else {
            if( use_uniform_scale ) {
              float uni_scale = ( ((GetScale()[0] *  2) * (((float)rand())/RAND_MAX)) - GetScale()[0] );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, uni_scale );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, uni_scale );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, uni_scale );
            } else {
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sx, ( ((GetScale()[0] *  2) * (((float)rand())/RAND_MAX)) - GetScale()[0] ) );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sy, ( ((GetScale()[1] *  2) * (((float)rand())/RAND_MAX)) - GetScale()[1] ) );
              motion->ShiftValues( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(), FreeKeyFrame::sz, ( ((GetScale()[2] *  2) * (((float)rand())/RAND_MAX)) - GetScale()[2] ) );
            }
          }
        }

        // Shift the keys in time, if applicable.
        if( GetDeltaMode() == absolute )
          motion->ShiftKeys( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(),  GetDelta() );
        else
          motion->ShiftKeys( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(),  ( ( (GetDelta() * 2) * (rand()/RAND_MAX)) - GetDelta() ) );

        if( GetScaleMode() == absolute )
          motion->ScaleKeys( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(),  GetScaleTime() );
        else
          motion->ScaleKeys( motion->GetFirstFrameNumber(), motion->GetLastFrameNumber(),  ( ( (GetScaleTime() * 2) * (rand()/RAND_MAX)) - GetScaleTime() ) );

        // Save the motion back out
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

  return POPULATE_OK;
}

// FreeHierarchy():
//  Frees all the RAM used by the hierarchy
void Populate::FreeHierarchy() {
  // Free the Item List
  for( int i=0; i < (int)item_list.NumElements(); i++ )
    item_list[i]->skeleton.Flush();

  item_list.Flush();
}
