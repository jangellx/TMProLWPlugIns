//
// PopulateCLI.cpp
//
// Populte is design to populate landscapes, streets, etc. with
//  trees, people, etc.  It doesn't add any objects to a scene
//  (use MultiLFS for that); instead, it repositions objects
//  with the same null-object name to new locations.  Placement
//  can be random or based on the position of points in a
//  Lightwave Object File.  I can also scan a directory and
//  replace each item found with a new item from that dir.
//  It will scan the names of both lights and objects
//  in a scene, and thus can reposition both types.
//

// Argument Template:
//  Populate <source> <item name> [-o[utput] <output] [-l[wo] <object>] [-c <content>]
//           [-r[a] <replacement dir>] [-t[a|r] <tx> <ty> <tz>] [-y[a|r] <h> <p> <b>] [-h[a|r] <sx> <sy> <sz>]
//           [-H[a|r] <scale] [-d[a] <time>] [-s[a] <facter>] [-g <generation seed>]
//           [-f[irstpoint] <index>]
//    <source.lws>                the Lightwave scene file to Populate
//    <item name>                 the name (without path) of the item to reposition
//    [-o[utput] <output>]        optional output scene name
//    [-l[wo] <object>]           the full path of the object who's points to use for placement
//    [-c <content>]              the content dir
//    In the following arguments, specifiying 'a' after the argument will cause an absolute (non-random) adjustment.
//                                specifiying 'r' will replace the existing values with the new ones.
//    [-r <object> <rep path>]    the object in the scene to replace with those in the replacement path, which points to either a dir of objects or a text file of objects
//    [-t[a|r] <tx> <ty> <tz>]    the max distance from the point/origin to randomly place the item on each axis
//    [-y[a|r] <h> <p> <b>]       the max. amount to randomly rotate each item on each axis ( 0 == no_change)
//    [-h[a|r] <sx> <sy> <sz>]    the max. amount to randomly scale each item by on each axis (0 == no change)
//    [-H[a|r] <scale>]           the max. amount to randomly scale each item by on all axes (0 == no change)
//    [-d[a]   <time>]            the max. amount of time to shift a motion forward or backward (0 == no change)
//    [-s[a]   <factor>]          the max. amount of to scale a motion in time (0 == no change)
//    [-g <generation seed>]      the random seed to use.  If 0, a random seed will be generated
//    [-f[irst] <index>]          the first point to use from the object for placement.  requires -l flag

#include <iomanip.h>

#include "..\Populate\Populate.h"

Populate * ParseCommands( int argc, char **argv );
void       Help( void );

void main( int argc, char **argv ) {
	cout << " -- Populate V" << Populate_Version << " -- Copyright 1999 TM Productions" << endl;
	if( argc < 2 )          // Not Enough Arguments
		Help();

  if( argv[1][0] == '?' ) // 1st argument is a ?; display help
    Help();

  // Parse the command line arguments
  Populate * pop = ParseCommands( argc, argv );
  if( pop == NULL )
    exit(20);

  try {
    // Open the files
    int error = pop->OpenFiles();
    if( error != POPULATE_OK ) {
      cout << " ERROR:  " << Populate::GetErrorString( error ) << "; aborting" << endl;
      throw false;
    }

    // Test the Replacement Object Path
    error = pop->LoadReplacementPath();
    switch( error ) {
      case POPULATEREP_OK:    // No Error
        break;
      case POPULATEREP_PATH_DOESNT_EXIST:
        cout << " ERROR:  " << populate_rep_error_strings[ error ] << endl;
        break;
      case POPULATEREP_BAD_OBJECTS:
        cout << " The following replacement objects in the replacement path \""
             << pop->GetRepDir() << "\" are invalid:" << endl;
        for( unsigned int i=0; i < pop->GetRepList().NumElements(); i++ ) {
          if( !pop->GetRepList()[i]->IsValid() )
            cout << "   " << pop->GetRepList()[i]->GetFilename() << endl;
        }

        break;
    }

    // Output the replacement object paths, if applicable:
    if( pop->GetRepList().NumElements() > 0 ) {
      cout << "- Replacement Objects:  " << endl;
      for( unsigned int i = 0; i < pop->GetRepList().NumElements(); i++ ) {
        cout << "    " << setw( 7 ) << pop->GetRepList()[i]->GetProb()
             << "    " << pop->GetRepList()[i]->GetFilename() << endl; 
      }
    }

    // Find PNTS chunk in object, if needed
    if( !pop->FindPNTS() ) {
      cout << " ERROR:  Failed to read PNTS chunk from LWO Point Reference Object \"" << pop->GetLWO() << "\"; aborting" << endl;
      throw false;
    }

    if( pop->GetUseLWO() ) {
      if( pop->GetFirstPoint() > pop->GetLWOPointCount() ) {
        cout << " ERROR:  The number of First Point of " << pop->GetFirstPoint()
             << " exceeds the " << pop->GetLWOPointCount() << " in the LWO Point Reference Object \""
             << pop->GetLWO() << "\"; aborting" << endl;
        throw false;
      }
      cout << "  Number of points in LWO Point Reference Object: " << pop->GetLWOPointCount()
           << "; starting at " << pop->GetFirstPoint() << endl;
    }

    // Build the hierarchy tree
    cout << "-Building Hierarchy Tree... " << endl;
    if( !pop->BuildHierarchy() ) {
      cout << " ERROR:  Failed to read from \"" << pop->GetSource()
           << "\" while building hierarchy; aborting" << endl;
      throw false;
    }

    // Display hierarchy, if applicable
    if( pop->GetDisplayTree() ) {
      cout << "  -------------------- Scene Hierarchy --------------------- " << endl;
      pop->GetBase()->OutputAsText( 4 );
      cout << endl;
    }

    // Process the scene
    cerr << "-Processing \"" << pop->GetSource() << "\"..." << endl;
    error = pop->Process();
    if( error != POPULATE_OK ) {
      cout << " ERROR:  " << Populate::GetErrorString( error ) << "; aborting" << endl;
      throw false;
    }

    cerr << "-Processed " << pop->GetNumObjectsProcessed() << " objects and "
         << pop->GetNumLightsProcessed() << " lights and replaced "
         << pop->GetNumItemsReplaced() << " objects into \""
         << pop->GetOutput() << "\"" << endl;
    cerr << "-Sucessfully Populated \"" << pop->GetOutput() << "\"!" << endl;

  } catch( bool ) {
    ;  // This is just here so that, if there's an error, we will call 
       //  the clean-up at the code outside the try...catch block.
  }
    
  delete pop;
}


// Help():
//  Provides a command template and instructions.
//  The program exits at the end of the function.
void Help(void) {
  cout << "                 http://www.tmproductions.com" << endl;
  cout << endl;
  cout << " Populate repositions objects and lights in a scene.  It can do this"  << endl;
  cout << "  randomly or by using the positions of the points of a Lightwave Object." << endl;
  cout << "  It can also randomly rotate and scale the items key values, and shift" << endl;
  cout << "  and scalethose keys in time." << endl;
  cout << endl;
  cout << "  Usage:  Populate <source> <item name> [-o[utput] <output] [-l[wo] <object>]" << endl;
  cout << "             [-c <content>] [-r <object <path>] [-t[a|r] <tx> <ty> <tz>] [-y[a|r] <h> <p> <b>]" << endl;
  cout << "             [-h[a|r] <sx> <sy> <sz>] [-H[a|r] <scale>] [-d[a] <time>] [-s[a] <factor>]" << endl;
  cout << "             [-g <generation seed>] [-f[irstpoint] <index>]" << endl;
  cout << "    <source.lws>                the Lightwave scene file to Populate" << endl;
  cout << "    <item name>                 the name (without path) of the item to reposition" << endl;
  cout << "    [-o[utput] <output>]        optional output scene name" << endl;
  cout << "    [-l[wo] <object>]           the full path of the object who's points to use for placement" << endl;
  cout << "    [-c <content>]              the content directory" << endl;
  cout << "    [-r <object> <path>]        the object will be replaceed with the objects in the replacement path, which points to either a dir of objects or a text file of objects" << endl;
  cout << "      The [a] in the following means an absolute, non-random change is added to the existing values." << endl;
  cout << "                                Specifiying [r] will instead replace the existing values with the new ones." << endl;
  cout << "    [-t[a|r] <tx> <ty> <tz>]    the max distance from the point/origin to randomly place the item on each axis" << endl;
  cout << "    [-y[a|r] <h> <p> <b>]       the max. amount to randomly rotate each item on each axis ( 0 == no_change)" << endl;
  cout << "    [-h[a|r] <sx> <sy> <sz>]    the max. amount to randomly scale each item by on each axis (0 == no change)" << endl;
  cout << "    [-H[a|r] <scale>]           the max. amount to randomly scale each item by on all axes (0 == no change)" << endl;
  cout << "    [-d[a] <time>]              the max. amount of time to shift a motion forward or backward (0 == no change)" << endl;
  cout << "    [-s[a] <factor>]            the max. amount of to scale a motion in time (0 == no change)" << endl;
  cout << "    [-g <generation seed>]      the random seed to use.  If 0, a random seed will be generated" << endl;
  cout << "    [-f[irstpoint] <index>]     the first point to use from the object for placement.  Requires -l" << endl;

  exit(20);
}

// ParseCommands():
//  Parses the argument string passed into the command line.
//   Exits if the arguments are incorrect (or maybe returns
//   NULL); otherwise, it returns a pointer to a Populate
//   class
Populate * ParseCommands( int argc, char **argv ) {
  // Check for too few arguments
  if( argc < 3 )
    Help();

  // Check for help ?
  if( strchr( argv[1], '?' ) != 0 )
    Help();

  Populate *pop = new Populate( argv[1] );
  if( pop == NULL ) {
    cerr << " ERROR: Unable to allocate memory for Populate!" << endl;
    return NULL;
  }
          
  try {
    if( !pop->SetItem( argv[2] ) ) {
      cerr << " ERROR: The item name \"" << argv[2] << "\"is invalid!" << endl;
      throw( false );
    }

    cerr << "-Settings..." << endl;
    cerr << "  Source File will be \"" << pop->GetSource() << "\"" << endl;
    cerr << "  Item name starts with \"" << pop->GetItem() << "\"" << endl;

    for( int i=3; i < argc; i++ ) {
      switch( argv[i][1] ) {
      case 'c':
        // Content Dir Test
        if( ++i >= argc ) {
          cerr << " ERROR: No content directory following argument -c!" << endl;
          throw false;
        }
        pop->SetContentDir( argv[i] );
        cerr << "  Content Dir is " << pop->GetContentDir() << endl;
        break;

      case 'd':
        // Delta Test
        if( i+1 >= argc ) {
          cerr << " ERROR: No time delta following argument -d!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetDeltaMode( Populate::absolute );

        pop->SetDelta( atoi( argv[++i] ) );
        cerr << "  Max random time delta will be " << pop->GetDelta() << endl;
        break;

      case 'f':
        // First Point Test
        if( ++i >= argc ) {
          cerr << " ERROR: No first point index following argument -f!" << endl;
          throw false;
        }
        pop->SetFirstPoint( atoi( argv[i] ) );
        cerr << "  First point will be " << pop->GetFirstPoint() << endl;
        break;

      case 'g':
        // Seed Test
        if( ++i >= argc ) {
          cerr << " ERROR: No generation seed following argument -g!" << endl;
          throw false;
        }
        pop->SetSeed( atoi( argv[i] ) );
        cerr << "  Generation Seed for random numbers will be " << pop->GetSeed() << endl;
        break;

      case 'h':
        // Stretch Test
        if( i+3 >= argc ) {
          cerr << " ERROR: 3 scalar values must follow argument -h (one for each axis)!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetScaleMode( Populate::absolute );
        else if( argv[i][2] == 'r' )
          pop->SetScaleMode( Populate::replace );

        pop->SetScale( (float)atof( argv[i+1] ), (float)atof( argv[i+2] ), (float)atof( argv[i+3] ) );
        i+=3;
        cerr << "  Max random scale from point/origin will be " << pop->GetScale()[0] << ", "
            <<  pop->GetScale()[1] << ", " << pop->GetScale()[2] << endl;
        break;

      case 'H':
        // Scale Uniformly Test
        if( i+1 >= argc ) {
          cerr << " ERROR: a scalar values must follow argument -H!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetScaleMode( Populate::absolute );
        else if( argv[i][2] == 'r' )
          pop->SetScaleMode( Populate::replace );

        pop->SetUseUniformScale( true );
        i++;
        pop->SetScale( (float)atof( argv[i] ), (float)atof( argv[i] ), (float)atof( argv[i] ) );
        cerr << "  Max random uniform scale from point/origin will be " << pop->GetScale()[0] << endl;
        break;

      case 'l':  // that's a lower-case 'L'
        // LWO File Test
        if( ++i >= argc ) {
          cerr << " ERROR: No LWO Point Reference Object following argument -l!" << endl;
          throw false;
        }
        pop->SetUseLWO( true );
        pop->SetLWO( argv[i] );
        cerr << "  LWO Point Reference Object will be \"" << pop->GetLWO() << "\"" << endl;
        break;

      case 'o':
        // Output File Test
        if( ++i >= argc ) {
          cerr << " ERROR: No output path following argument -o!" << endl;
          throw false;
        }
        pop->SetOutput( argv[i] );
        cerr << "  Output File will be \"" << pop->GetOutput() << "\"" << endl;
        break;

      case 'r':
        // Replacement Dir Test
        if( i+2 >= argc ) {
          cerr << " ERROR: No Replacement Item and Path following argument -r!" << endl;
          throw false;
        }
        pop->SetUseRepDir( true );
        pop->SetRepItem( argv[++i] );
        pop->SetRepDir( argv[++i] );
        cerr << "  Replacement Path will be \"" << pop->GetRepDir() << "\" on item\""
				     << pop->GetRepItem() << "\"" << endl;
        break;

      case 's':
        // Scale Time Test
        if( i+1 >= argc ) {
          cerr << " ERROR: No scale time delta following argument -s!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetScaleTimeMode( Populate::absolute );

        pop->SetScaleTime( (float)atof( argv[++i] ) );
        cerr << "  Max random time delta will be " << pop->GetScaleTime() << endl;
        break;

      case 't':
        // Position Test
        if( i+3 >= argc ) {
          cerr << " ERROR: 3 positional values must follow argument -t (one for each axis)!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetPositionMode( Populate::absolute );
        else if( argv[i][2] == 'r' )
          pop->SetPositionMode( Populate::replace );

        pop->SetPosition( (float)atof( argv[i+1] ), (float)atof( argv[i+2] ), (float)atof( argv[i+3] ) );
        i+=3;
        cerr << "  Max random offset from point/origin will be " << pop->GetPosition()[0] << ", "
             <<  pop->GetPosition()[1] << ", " << pop->GetPosition()[2] << endl;
        break;

      case 'y':
        // Rotation Test
        if( i+3 >= argc ) {
          cerr << " ERROR: 3 rotational values must follow argument -y (one for each axis)!" << endl;
          throw false;
        }
        if( argv[i][2] == 'a' )
          pop->SetRotationMode( Populate::absolute );
        else if( argv[i][2] == 'r' )
          pop->SetRotationMode( Populate::replace );

        pop->SetRotation( (float)atof( argv[i+1] ), (float)atof( argv[i+2] ), (float)atof( argv[i+3] ) );
        i+=3;
        cerr << "  Max random rotation from point/origin will be " << pop->GetRotation()[0] << ", "
             <<  pop->GetRotation()[1] << ", " << pop->GetRotation()[2] << endl;
        break;

      default:
        // Unknown Argument
        cerr << " ERROR: Unrecognized argument \"" << argv[i] << "\"!" << endl;
        throw false;
      }
    }

    return pop;
  } catch( bool ) {
    // Common error stuff
    cerr << " Type Populate ? for help" << endl;
    delete pop;
    exit(20);
  }
}

