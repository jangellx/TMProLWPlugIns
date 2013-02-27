//
// FreeKeyCLI.cpp
//

// Argument Template:
//  FreeKey [-!] <source> <low> <high> [-o <output>] [-r[emoveinside]] [-R[emoveoutside]] [-step <step>] [-invertstep] [-d[elta] <offset>] [-t[ree]] [-[hierarchy] <object>] [-b[one] <bone>]
//   <source>:                      source scene filename and path
//  <low> <high>:                   range of frames that do not get deleted.
//  [-o <output>]:                  optional output name
//  [-r[emoveinside]]:              remove all the keys inside the range.  Disables step and delta.
//  [-R[emoveoutside]]:             removes all the keys outside the range.
//  [-s[tep] <step>]:               deletes every Xth frame within the range.
//  [-i[invertstep]]:               inverts the step method, keeping on every Xth frame within the range. Must be used with Step
//  [-d[elta] <offset>]:            shifts the range by the specified delta in time.  Frames can shift backward up to frame 0.
//  [-h[ierarchy] <object>]:        affect only the object/bone hierarchy starting with root object specified.
//  [-b[one] <bone>]:               limits hierarchy mode to a bone hierarchy.  Must be used with -h
//  [-t[ree]]:                      display hieracrchy tree.  Only works with -s.
//  [-!]:                           Tutoroial/Prompt mode.  Must be the first argument

#include "../FreeKey/FreeKey.h"

FreeKey * ParseCommands( int argc, char **argv, char *base_object, char *base_bone );
void      BuildGroup( FreeKey *fk, char *base_object, char *base_bone );
void      AddSelfAndKids( BatarangHierarchy *base );
void      Help( void );

void main( int argc, char *argv[] ) {
  cout << " FreeKey V" << FreeKey_Version << "." << FreeKey_Revision
       << " --- Copyright 1999 TM Productions" << endl;

  // Set up some variables
  char   base_object[ MAX_PATH_LENGTH ] = "(entire scene)"; // Object at the base of the hierarchy
  char   base_bone[   MAX_PATH_LENGTH ] = "(all bones)";    // Base bone name

  // Scan the command line arguments
  FreeKey *fk = ParseCommands( argc, argv, base_object, base_bone );
  if( fk == NULL )
    exit( 20 );

  try {
    // Open the files
    int error = fk->OpenFiles();
    if( error != FREEKEY_OK ) {
      cout << " ERROR:  " << FreeKey::GetErrorString( error ) << "; aborting" << endl;
      throw false;
    }

    // Build the hierarchy tree
    cout << "-Building Hierarchy Tree... " << endl;
    if( !fk->BuildHierarchy() ) {
      cout << " ERROR:  Failed to read from \"" << fk->GetSource()
           << "\" while building hierarchy; aborting" << endl;
      throw false;
    }

    // Display hierarchy, if applicable
    if( fk->GetDisplayTree() ) {
      cout << "  -------------------- Scene Hierarchy --------------------- " << endl;
      fk->GetBase()->OutputAsText( 4 );
      cout << endl;
    }

    // Build the selection
    BuildGroup( fk, base_object, base_bone );

    // Process the scene
    cerr << "-Processing \"" << fk->GetSource() << "\"..." << endl;
    error = fk->Process();
    if( error != FREEKEY_OK ) {
      cout << " ERROR:  " << FreeKey::GetErrorString( error ) << "; aborting" << endl;
      throw false;
    }

    cerr << "-Processed " << fk->GetNumObjectsProcessed() << " objects, "
         << fk->GetNumBonesProcessed() << " bones and "
         << fk->GetNumLightsProcessed() << " lights into \""
         << fk->GetOutput() << "\"" << endl;
    cerr << "-Sucessfully Freekeyed \"" << fk->GetOutput() << "\"!" << endl;
  } catch( bool ) {
    ;  // This is just here so that, if there's an error, we will call 
       //  the clean-up at the code outside the try...catch block.
  }
    
  delete fk;
}

// ParseCommands()
//  Scans through the argument list and fills in a FreeKey instance.
//   Returns NULL if there was an error
FreeKey * ParseCommands( int argc, char **argv, char *base_object, char *base_bone ) {
  // Check for no arguments
  if( argc < 2 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  // Check for Tutorial/Prompt mode !
  bool use_tutorial = false;
  if( strcmp( argv[1], "-!" ) == 0 )
    use_tutorial = true;

  // Check for too few args
  if( (argc < 4) && !use_tutorial )
    Help();

  // Create the FreeKey instance
  FreeKey *fk = new FreeKey();
  if( fk == NULL ) {
    cerr << " ERROR:  Unable to allocate FreeKey Instance!  Aborting" << endl;
    return NULL;
  }

  fk->AddGroup();

  try {
	  fk->SetSource( argv[1] );

    // Make sure low is at least one less than high
    if( argc > 1 + use_tutorial )
      fk->SetSource( argv[ 1 + use_tutorial ] );

    if( argc > 2 + use_tutorial )
      fk->GetGroupList()[0]->SetRange( atoi( argv[2 + use_tutorial]) , atoi( argv[3 + use_tutorial] ) );

    cerr << "  The range is " << fk->GetGroupList()[0]->GetLow() << " through "
         << fk->GetGroupList()[0]->GetHigh() << endl;

    cerr << "-Settings..." << endl;
    for( int i=4 + use_tutorial; i < argc; i++ ) {
      if( argv[i][0] == '-' ) {
        switch( argv[i][1] ) {
        case 'b':
          // Bone Hierarchy Base
          if( ++i >= argc ) {
            cerr << " ERROR: No hierarchy base bone following argument -b!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }

          strcpy( base_bone, argv[i] );
          cerr << "  Bone Hierarchy Mode enabled; base bone is \"" << base_bone << "\"" << endl;
          break;

        case 'd':
          // Delta Test
          if( ++i >= argc ) {
            cerr << " ERROR: no offset following argument -d!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }

          fk->GetGroupList()[0]->SetDelta( atoi( argv[i] ) );
          fk->GetGroupList()[0]->SetUseDelta( true );
          cerr << "  Delta Active; " << fk->GetGroupList()[0]->GetDelta() << " frame offset will be applied" << endl;
          break;

        case 'h':
          // Hierarchy Base
          if( ++i >= argc ) {
            cerr << " ERROR: No hierarchy base object following argument -h!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }

          if( DirStrings::HasPath( argv[i] ) ) {
            cerr << " ERROR:  The hierarchy base object should be a file name only; no path!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }

          strcpy( base_object, argv[i] );
          cerr << "  Hierarchy Mode enabled; base object is \"" << base_object << "\"" << endl;
          break;

        case 'i':
          // Invert Step
          fk->GetGroupList()[0]->SetInvertStep( true );
          cerr << "  Step Mode is now Inverted (keep every Xth frame)" << endl;
          break;

        case 'o':
          // Output Scene Test
          if( ++i >= argc ) {
            cerr << " ERROR: No output path following argument -o!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }
          fk->SetOutput( argv[i] );
          cerr << "  Output scene will be \"" << fk->GetOutput() << "\"" << endl;
          break;

        case 'r':
          // Remove Keys Inside Range
          if( fk->GetGroupList()[0]->GetRemoveMode() == FreeKeyGroup::remove_outside ) {
            cerr << "  WARNING:  Remove outside already specified; ignoring -R" << endl;
          } else {
            fk->GetGroupList()[0]->SetRemoveMode( FreeKeyGroup::remove_inside );
            cerr << "  Remove Keys Inside Range Active; all Shift and Step operations will be disabled" << endl;
          }
          break;

        case 'R':
          // Remove Keys Outside Range
          if( fk->GetGroupList()[0]->GetRemoveMode() == FreeKeyGroup::remove_inside ) {
            cerr << "  WARNING:  Remove Inside already specified; ignoring -r" << endl;
          } else {
            fk->GetGroupList()[0]->SetRemoveMode( FreeKeyGroup::remove_outside );
            cerr << "  Remove Keys Outside Range Active" << endl;
          }
				  break;

        case 's':
          // Step
          if( ++i >= argc ) {
            cerr << " ERROR: No step amount following argument -s!" << endl;
            cerr << " Type FreeKey ? for help" << endl;
            throw false;
          }
				  fk->GetGroupList()[0]->SetStep( atoi( argv[i] ) );
          fk->GetGroupList()[0]->SetUseStep( true );
          cerr << "  Step Mode Active, deleting every " << fk->GetGroupList()[0]->GetStep()
				       << " frame within the range" << endl;
          break;

        case 't':
			    // Tree
          fk->SetDisplayTree( true );
          cerr << "  Tree Output Enabled" << endl;
          break;

        case '!':
			    // Tutorial Mode
          cerr << " ERROR: The Tutorial/Prompt flag -! must be the first argument!" << endl;
          cerr << " Type FreeKey ? for help" << endl;
          throw false;

        default:
          // Unknown Argument
          cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
          cerr << " Type FreeKey ? for help" << endl;
          throw false;
        }
      } else {
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        cerr << " Type FreeKey ? for help" << endl;
        throw false;
      }
    }

    // Handle Tutorial/Prompt Mode
    if( use_tutorial ) {
      cin.flags( cin.flags() | ios::skipws );

      char buffer[1024];
      cout << "" << endl;
      cout << " --- Tutorial Mode --- " << endl;
      cout << " For command-line information, type FreeKey ?" << endl;
      cout << endl;
      cout << " Tutorial Mode will prompt for the information it needs" << endl
           << "  to process your scene with FreeKey.  To use the default" << endl
           << "  value, enter a ! and hit return." << endl;
      cout << endl;
      cout << " - Source File:" << endl;
      cout << "  Enter the name of the scene you want to process.  You" << endl
           << "   can rag and drop the scene from a file requester or an"  << endl
           << "   Explorer window, or type in the name with full path"  << endl
           << "   here.  Be sure to include the extension! (default is "    << endl
           << "   \"" << fk->GetSource() << "\")" << endl
           << "  <source>: ";
      cin  >> buffer;
      if( (buffer[0] != '\0') && (buffer[0] != '!') )
		    fk->SetSource( buffer );

      fk->GenerateNames( "FreeKeyed" );

      cout << endl;
      cout << " - Output File:" << endl;
      cout << "  Enter the name of the scene you want to output.  You" << endl
           << "   can drag and drop the scene from a file requester or an"  << endl
           << "   Explorer window, or type in the name with full path"  << endl
           << "   here. You cannot overwrite the original scene.  Don't" << endl
           << "   forget the extension! (default is " << endl
           << "   \"" << fk->GetOutput() << "\")" << endl
           << "  [-o <output>]: ";
      cin  >> buffer;
      if( (buffer[0] != '\0') && (buffer[0] != '!') )
        fk->SetOutput( buffer );

      cout << endl;
      cout << " - Hierarchy:" << endl;
      cout << "  You can have FreeKey process the entire scene, or just the" << endl
           << "   object hierarchy you specify.  Enter the name of the object"  << endl
           << "   (just the name; no path) at the top of the hierarchy (say a" << endl
           << "   Master Null or something) here. Type ! and hit enter if" << endl
           << "   you want to use the default value. (default is " << endl
           << "   \"" << base_object << "\")" << endl
           << "  [-h[ierarchy] <object>]: ";
      cin  >> buffer;
      if( (buffer[0] != '\0') && (buffer[0] != '!') && (stricmp( buffer, "(entire scene)") == 0) )
		    strcpy( base_object, buffer );

      if( stricmp( base_object, "(entire scene)" ) != 0  ) {
        cout << " - Bone Hierarchy:" << endl;
        cout << "  You can limit the processing even further so that it will" << endl
             << "   only affect the children of a particular bone in the base" << endl
             << "   object.  Enter the bone name here if you want to use this" << endl
             << "   feature (defeault is \"" << base_bone << "\")" << endl
             << "  [-b[one] <bone>]: ";
        cin  >> buffer;
        if( (buffer[0] != '\0') && (buffer[0] != '!') && (stricmp( buffer, "(all bones)") == 0) )
  		    strcpy( base_bone, buffer );
      }

      cout << endl;
      cout << " - Range:" << endl;
      cout << "  All keyframes outside of the range you enter will be" << endl
           << "   deleted. (defaults are <low> " << fk->GetGroupList()[0]->GetLow()
           << "; <high> " << fk->GetGroupList()[0]->GetHigh() << ")" << endl
           << "   <low>:  ";
      cin  >> buffer;
      if( (buffer[0] != '\0') || (buffer[0] != '!') )
        fk->GetGroupList()[0]->SetLow( atoi( buffer ) );

      cout << "   <high>: ";
      cin  >> buffer;
      if( (buffer[0] != '\0') || (buffer[0] != '!') )
        fk->GetGroupList()[0]->SetHigh( atoi( buffer ) );

      cout << endl;
      cout << " - Remove Keys:" << endl;
      cout << "  You can choose to delete keys inside or outisde the range," << endl
           << "   or not at all.  Type \'o\' for \"outside\", \'i\' for \"inside\"" << endl
           << "   or \'d\' for \"don't\"." << endl;
           if( fk->GetGroupList()[0]->GetRemoveMode() == FreeKeyGroup::remove_outside )
             cout << "   (default is Remove Outside)" << endl;
           else if( fk->GetGroupList()[0]->GetRemoveMode() == FreeKeyGroup::remove_inside )
             cout << "   (default is Remove Inside)" << endl;
           else
             cout << "   (default is Don't Remove Any Keys)" << endl;
           cout << "   [-r]/[-R]:  ";
      cin  >> buffer;
      if( buffer[0] == 'o' ) {
        fk->GetGroupList()[0]->SetRemoveMode( FreeKeyGroup::remove_outside );
      } else if( buffer[0] == 'i' ) {
        fk->GetGroupList()[0]->SetRemoveMode( FreeKeyGroup::remove_inside );
      } else if ( buffer[0] == 'd' ) {
        fk->GetGroupList()[0]->SetRemoveMode( FreeKeyGroup::remove_none );
      }

      if( fk->GetGroupList()[0]->GetRemoveMode() == FreeKeyGroup::remove_outside ) {
        cout << endl;
        cout << " - Delta:" << endl;
        cout << "  You can shift the keyframes after the deletion. Negative" << endl
             << "   numbers shift the motion backward, and positive numbers" << endl
             << "   shift forward. 0 means no shift occurs.  (default is "
             << fk->GetGroupList()[0]->GetDelta() << ")" << endl
             << "   [-d[elta] <offset>]:  ";
        cin  >> buffer;
        if( (buffer[0] != '\0') && (buffer[0] != '!') ) {
          fk->GetGroupList()[0]->SetDelta( atoi( buffer ) );
          fk->GetGroupList()[0]->SetUseDelta( true );
        }

        cout << endl;
        cout << " - Step:" << endl;
        cout << "  You can delete or keep every Xth key within the undeleted" << endl
             << "   range.  If you want to use this feature, enter the step" << endl
             << "   here (default is " << fk->GetGroupList()[0]->GetStep() << ")" << endl
             << "   [-s[tep] <step>]:  ";
        cin  >> buffer;
        if( (buffer[0] != '\0') && (buffer[0] != '!') ) {
          fk->GetGroupList()[0]->SetStep( atoi( buffer ) );
          fk->GetGroupList()[0]->SetUseStep( true );
        }

        if( fk->GetGroupList()[0]->GetUseStep() ) {
          cout << endl;
          cout << " - Invert Step:" << endl;
          cout << "  By default, every Xth keyframe will be deleted.  To reverse" << endl
               << "   this (so every Xth keyframe will be kept), type Y here." << endl;
               if( fk->GetGroupList()[0]->GetInvertStep() )
                 cout << "   (default is Yes)" << endl;
               else
                 cout << "   (default is No)" << endl;
               cout << "   [-i[nvertstep]]:  ";
          cin  >> buffer;
          if( (buffer[0] != '\0') && (buffer[0] != '!') && (buffer[0] != 'n') && (buffer[0] != 'N') ) {
            fk->GetGroupList()[0]->SetInvertStep( true );
          }
        }
      }

      if( stricmp( base_object, "(entire scene)" ) != 0  ) {
        cout << endl;
        cout << " - Tree:" << endl;
        cout << "  When Hierarchy Mode is active, you can display a tree of" << endl
             << "   the scene file for your own reference." << endl;
             if( fk->GetDisplayTree() )
               cout << "   (default is Yes)" << endl;
             else
               cout << "   (default is No)" << endl;
             cout << "   [t[ree]]:  ";
        cin  >> buffer;
        if( (buffer[0] != '\0') && (buffer[0] != '!') && (buffer[0] != 'n') ) {
          fk->SetDisplayTree( true );
        }
      }

      cout << " --- End Tutorial Mode --- ";
      cout << endl;
    }

    return fk;

  } catch( bool ) {
    delete fk;
  }

  // We should never get here
  return NULL;
}

// BuildGroup():
//  Sets the group of each of the items matching base_object
//   and base_bone.  If neither are used, then all objects are
//   added.  If base_bone is used, then only the bones found
//   in base_object will be added
void BuildGroup( FreeKey *fk, char *base_object, char *base_bone ) {
  unsigned int i, j;


  for( i=0; i < fk->GetItemList().NumElements(); i++ ) {
    if( (stricmp( base_object, "(entire scene)" ) == 0) ||                   // If we are adding all objects OR
        (stricmp( base_object, fk->GetItemList()[i]->GetName() ) == 0) ) {   // We're adding all objects matching this name
     
      if( stricmp( base_bone, "(all bones)" ) == 0 )                         // Add this item only if we're adding all bones too
        AddSelfAndKids( fk->GetItemList()[i] );

      for( j=0; j < fk->GetItemList()[i]->skeleton.NumElements(); j++ ) {
        if( (stricmp( base_bone, "(all bones)" ) == 0) ||                                 // If we're adding all bones OR
            (stricmp( base_bone, fk->GetItemList()[i]->skeleton[j]->GetName()) == 0) ) {  // This particular bone matches
          AddSelfAndKids( fk->GetItemList()[i]->skeleton[j] );
        }
      }
    }
  }

  // Display a list of the selected items
  if( fk->GetDisplayTree() ) {
    cout << "-Selected Items:" <<endl;

    for( i=0; i < fk->GetItemList().NumElements(); i++ ) {
      if( fk->GetItemList()[i]->GetGroup() == 0 )
        cout << "  " << fk->GetItemList()[i]->GetName() << endl;

      for( j=0; j < fk->GetItemList()[i]->skeleton.NumElements(); j++ ) {
        if( fk->GetItemList()[i]->skeleton[j]->GetGroup() == 0 )
          cout << "    " << fk->GetItemList()[i]->skeleton[j]->GetName() << endl;
      }
    }

    cout << endl;
  }
}

// AddSelfAndKids()
//  Adds children and bones to the group
void AddSelfAndKids( BatarangHierarchy *base ) {
  base->SetGroup( 0 );

  for( unsigned int i=0; i < base->children.NumElements(); i++ )
    AddSelfAndKids( base->children[i] );

  for( i=0; i < base->skeleton.NumElements(); i++ )
    AddSelfAndKids( base->skeleton[i] );
}

// Help():
//  Provides help and quits.
void Help( void ) {
  cout << "              http://www.tmproductions.com" << endl;
  cout << "  Deletes frames outside a range in the scene.  Useful for\n"
       << "   making mocap more controllable..  Can also delete or keep\n"
       << "   every Xth frame in the range, shift the keys around, and\n"
       << "   affect only one hierarchy in the scene.\n" << endl;
  cout << endl;
  cout << "  Usage:   FreeKey [-!] <source> <low> <high> [-o <output>] [-r] [-step <step>] [-invertstep] [-d[elta] <offset>] [-t[ree]] [-h[ierarchy] <object>] [-b[one] <bone>]" << endl;
  cout << "    <source>:               source scene filename and path." << endl; 
  cout << "    <low> <high>:           range of frames that do not get deleted." << endl;
  cout << "   [-o[utput] <output>]:    optional output name." << endl;
  cout << "   [-r[emoveinside]]:       remove all the keys inside the range.  Disables step and delta." << endl;
  cout << "   [-R[emoveoutside]]:      removes all the keys outside the range." << endl;
  cout << "   [-s[tep] <step>]:        deletes every Xth frame within the range." << endl;
  cout << "   [-i[invertstep]]:        inverts the step method, keeping on every Xth frame within the range." << endl;
  cout << "   [-d[elta] <offset>]:     shifts the range by the specified delta in time (ie: shift keys)." << endl;
  cout << "   [-h[ierarchy] <object>]: affect only the object/bone hierarchy starting with root object specified." << endl;
  cout << "   [-b[one] <bone>]:        limits hierarchy mode to a bone hierarchy.  Must be used with -h." << endl;
  cout << "   [-t[ree]]:               display hieracrchy tree.  Only works with -s" << endl;
  cout << "   [-!]:                    Tutoroial/Prompt mode.  Must be the first argument." << endl;

  exit(0);
}
