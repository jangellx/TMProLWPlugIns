// Method One: Frame-Level information
//  This scans the PSM as it is store, as a series of frames containing the positions
//   of particles.  This is the opposite of the LW method, which stores a series of
//   items containing frames.
void PSMReader_Method1 {
          for( int frames = 0; /*NULL*/; frames++ ) {
            // Error test
            if( (psm.bad() || psm.fail()) && !psm.eof() ) {
              cerr << " ERROR:  File error reading from source \""
                   << source << "\"; aborting" << endl;
              exit(20);
            }

            // EOF test
            if( psm.eof() )
              break;

            // Read in the chunk data
            chunk_start = psm.tellg();
            psm.get( line, 5 );
            psm.read( (char *)(&chunk_size), 4 );
            MtoI(chunk_size);

            cerr << "    Chunk Size (frame " << frames << "):  "
                 << line << " " << chunk_size <<  " Loc: " << psm.tellg() << endl;

            // Read in the POSN data
            psm.ignore( 16 );
            psm.get( line, 5 );
            if( strncmp( line, "POSN", 4 ) != 0 ) {
              cerr << " ERROR:  Expected POSN; found \""
                   << line << "\" while scanning PSM; aborting" << endl;
              exit(20);
            }

            unsigned long num_particles;
            double xyz[3];
            psm.read( (char *)(&num_particles), 4 );
            MtoI(num_particles);

            cerr << "      NumParticles:  " << num_particles/12 << endl;
            for( int m = 0; m < num_particles/12; m+=12 ) {
              psm.read( (char *)(&xyz), 12 );
              MtoI(xyz[0]);
              MtoI(xyz[1]);
              MtoI(xyz[2]);
              cerr << "        Item " << m << ":  X/Y/Z   "
                   << xyz[0] << "  " << xyz[1] << "  " << xyz[2] << endl;
            }

            // Scan to the next chunk
            int dummy = psm.tellg();
            psm.seekg( abs( chunk_size - (psm.tellg() - chunk_start ) + 8 ), ios::cur );
}


// ----------------------------------------------------------------------------


    if( (in.bad() || in.fail()) && !in.eof() ) {
      cerr << " ERROR:  File error reading from source \""
           << source << "\"; aborting" << endl;
      exit(20);
    }

    // Look for an object and grab it's name
    if( strnicmp( line, "LoadObject", 10 ) == 0 )
      strcpy( object_name, &(line[11]) );

    if( strnicmp( line, "AddNullObject", 13 ) == 0 )
      strcpy( object_name, &(line[14]) );

    // Find an ObjectMotion and mark it
    if( strncmp( line, "ObjectMotion", 12 ) == 0 )
      output_motion_pos = out.tellp();

    // Find an EndBehavior and mark it
    if( strncmp( line, "EndBehavior", 11 ) == 0 )
      source_motion_end = in.tellg();

    // Check for Particle Storm Data
    if( !in_psm )
      streampos source_pstorm_start = in.tellg();

    if( strncmp( line, "Plugin", 6 ) == 0 ) {
      if( strstr( line, "ParticleStorm" ) != 0 )
        in_psm = true;
    }

    if( in_psm ) {
      // Don't output lines if we're in a PSM.
      output_line = false;

      // Check for end of PS data
      if( strncmp( line, "EndPlugin", 9 ) == 0 )
        in_psm = false;

      // Check for a new PSM, or increment from an old one.
      if( strncmp( line, "MotionDataFile", 14 ) == 0 ) {
        psm_match = -1;
        for( int i=0; i < (int)psm_names.NumElements(); i++ ) {
          if( stricmp( psm_names[i], &(line[15]) ) == 0 ) {
            psm_match = i;
            break;
          }
        }

        // Not loaded yet; add it as a new one.
        if( psm_match == -1 ) {
          psm_names.Add( strdup( &(line[15]) ) );
          psm_match = psm_names.NumElements()-1;
          psm_counts.Add( -1 );
        }

        // Store the filename for later
        strcpy( psm_file, &(line[16]) );           // Skip both
        psm_file[ strlen( psm_file ) - 1 ] = '\0'; //  quotes
      }

      // Check for MotionIndex and increment it.
      if( strncmp( line, "MotionIndex", 11 ) == 0 ) {
        psm_index = atoi( &(line[12]) );
        if( psm_counts[ psm_match ] == -1 )   // No index yet assigned; get it from the file
            psm_counts[ psm_match ] = psm_index;

        // Only scan a PSM once.  Needed so that when we look through
        //  a second time to find the data between the motion and the
        //  Particle Storm data, we don't process the PSM again.
        if( ++psm_scan_count > 1 )
          psm_scan_count = 0;
        else {

          // We now have enough data to freeze the PSM
          try {
            // Test for PSM existance
            if( !DirInfo::Exists( psm_file ) ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  The PSM file \"" << psm_file << "\" doesn't exist!" << endl;
              throw( false );
            }

            // Open the PSM file
            ifstream psm( psm_file, ios::binary );
            if( !psm ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  Unable to open file \"" << psm_file << "\"!" << endl;
              throw( false );
            }

            // Make sure it really is a PSM file
            psm.ignore( 8 );
            psm.read( line, 4 );
            if( strnicmp( line, "PSM2", 4 ) != 0 ) {
              cerr << "   ERROR:   " << DirStrings::HasFile( object_name ) << ":  \"" << psm_file << "\" isn't a Particle Storm 2 PSM file!" 
                   << "  (header:  " << line << ")" << endl;
              throw( false );
            }

            // Find out the size of the first chunk
            unsigned long chunk_size;
            int num_particles;

            // Header Data
//            psm.ignore( 14 );
            int temp;
            short eh;
            psm.get( line, 5);
            psm.read( (char *)(&temp), 4);
            psm.read( (char *)(&num_particles), 4);
            psm.read( (char *)(&eh),   2);
            MtoI(temp);                     // 6???
            MtoI(num_particles);            // Number of particles
            MtoI(eh);                       // 0???

            cerr << "    Header:  " << line << " " << temp << " " << num_particles << " " << eh << endl;

            streampos first_frame_pos = psm.tellg();
            streampos chunk_start = -1;

            // Figure out how many frames are in the PSM
            int frames_per_particle = FindNumPSMFrames( psm );
            if( frames_per_particle < 0 ) {
              cerr << " ERROR:  File error reading from PSM \""
                   << psm_file << "\" while counting frames; skipping" << endl;
              throw( false );
            }

            if( frames_per_particle == 0 ) {
              cerr << " ERROR:  PSM \""
                   << psm_file << "\" contains no frames; skipping" << endl;
              throw( false );
            }

#ifdef debug_psm_loader
            cerr << endl << " ---------------------------------------------------------------------------------- " << endl << endl;
#endif
            cerr << "  PSM:  " << DirStrings::HasFile( psm_file )
                 << ";  " << frames_per_particle << " frames total "
                 << endl;
            cerr << "    Index " << psm_index << ":" << endl;

            // Output initial motion data
            out.seekp( output_motion_pos, ios::beg );
            out << "ObjectMotion (unnamed)" << endl;
            out << "  9" << endl;                        // Num Channels
            out << "  "  << frames_per_particle << endl; // Frame Count


            for( int frames = 0; /*NULL*/; frames++ ) {
              // Read in the chunk data
              chunk_start = psm.tellg();
              psm.get( line, 5 );
              psm.read( (char *)(&chunk_size), 4 );
              MtoI(chunk_size);

#ifdef debug_psm_loader
              cerr << "    Chunk Size (frame " << frames << "):  "
                   << line << " " << chunk_size <<  " Loc: " << psm.tellg() << endl;
#endif
              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from PSM \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;

              short dummys;
              int   dummyi;
              float dummyf;
              psm.get( line, 5);
#ifdef debug_psm_loader
              cerr << "     " << line << endl;
#endif
              psm.get( line, 5);                  // FRFL?
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif
              psm.read( (char *)(&dummyi), 4 );
              MtoI( dummyi );
#ifdef debug_psm_loader
              cerr << " " << dummyi;
#endif
              psm.read( (char *)(&dummyi), 4 );
              MtoI( dummyi );
#ifdef debug_psm_loader
              cerr << " " << dummyi << endl;
#endif

              // Read in the POSN data
//              psm.ignore( 16 );
              psm.get( line, 5 );
              if( strncmp( line, "POSN", 4 ) != 0 ) {
                cerr << " ERROR:  Expected POSN; found \""
                     << line << "\" while scanning PSM; aborting" << endl;
                exit(20);
              }

              float xyz[3];
              int posn_size;
              psm.read( (char *)(&posn_size), 4 );
              MtoI(posn_size);

#ifdef debug_psm_loader
              cerr << "      NumParticles/POSN size:  " << num_particles << "/" << posn_size << endl;
#endif
              psm.seekg( 12 * psm_index, ios::cur );        // Read in just the one particle's data
              psm.read( (char *)(&xyz), 12 );
              MtoI(xyz[0]);
              MtoI(xyz[1]);
              MtoI(xyz[2]);
              cerr << "        Frame " << setw(3) << frames << ":  X/Y/Z   "
                   << xyz[0] << "  " << xyz[1] << "  " << xyz[2] << endl;

              // Output the motion to the scene file
              out << "  " << xyz[0] << " " << xyz[1] << " " << xyz[2]
                  << " 0 0 0 1 1 1" << endl;
              out << "  " << frames << " 0 0 0 0" << endl;

              // Skip the rest of the POSN data
              psm.seekg( posn_size - (12 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Flags
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              SkipFLAG( psm );
              cerr << "  <skipped>" << endl;

              psm.get( line, 5);           // Color
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int colr_size;
              psm.read( (char *)(&colr_size), 4 );
              MtoI( colr_size );
#ifdef debug_psm_loader
              cerr << " " << colr_size;
#endif

              unsigned char rgba[4];
              psm.seekg( psm_index * 4, ios::cur );
              psm.read( rgba, 4 );

#ifdef debug_psm_loader
              cerr << "  RGBA: " << setw(4) << (int)rgba[0] << " "
                                 << setw(4) << (int)rgba[1] << " "
                                 << setw(4) << (int)rgba[2] << "  "
                                 << setw(4) << (int)rgba[3] << endl;
#endif

              // Skip the rest of the COLR data
              psm.seekg( colr_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Size
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif
              int size_size;
              psm.read( (char *)(&size_size), 4 );
              MtoI( size_size );
#ifdef debug_psm_loader
              cerr << " " << size_size;
#endif

              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyf), 4 );
#ifdef debug_psm_loader
              cerr << "  Size: " << setw(4) << dummyf << endl;
#endif

              // Skip the rest of the SIZE data
              psm.seekg( size_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // PLNK?
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int plnk_size;
              psm.read( (char *)(&plnk_size), 4 );
              MtoI( plnk_size );
#ifdef debug_psm_loader
              cerr << " " << plnk_size;
#endif
              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyi), 4 );
#ifdef debug_psm_loader
              cerr << "  PLNK: " << setw(4) << dummyi << endl;
#endif

              // Skip the rest of the PLNK data
              psm.seekg( plnk_size - (4 * (psm_index+1)), ios::cur );

              psm.get( line, 5);           // Particle Age
#ifdef debug_psm_loader
              cerr << "     " << line;
#endif

              int page_size;
              psm.read( (char *)(&page_size), 4 );
              MtoI( page_size );
#ifdef debug_psm_loader
              cerr << " " << page_size;
#endif

              psm.seekg( psm_index * 4, ios::cur );
              psm.read( (char *)(&dummyi), 4 );
#ifdef debug_psm_loader
              cerr << "  PAge: " << setw(4) << dummyi << endl;
#endif

              // Skip the rest of the PAGE data
              psm.seekg( page_size - (4 * psm_index), ios::cur );


              // Error test
              if( (psm.bad() || psm.fail()) && !psm.eof() ) {
                cerr << " ERROR:  File error reading from psm \""
                     << psm << "\"; aborting" << endl;
                exit(20);
              }

              // EOF test
              if( psm.eof() )
                break;

              // Scan to the next chunk
              int dummy = psm.tellg();
              psm.seekg( abs( chunk_size - (psm.tellg() - chunk_start ) + 8 ), ios::cur );

              int test = psm.tellg();
#ifdef debug_psm_loader
              cerr << "   PSM Location:  " << test << endl;
#endif
            }
          
            // Output the end of the motion data
            out << "EndBehavior 1" << endl;
            in.seekg( source_motion_end, ios::beg );

            psm.close();

          } catch( bool a ) {
            ; // Catch errors here, if needed
          }
        }
      }
    }

    // See if we should output the line or not
    if( output_line == false )
      output_line = true;
    else
      out << line << endl;

