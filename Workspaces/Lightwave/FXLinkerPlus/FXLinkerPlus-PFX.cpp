//
// FXLinkerPlus-ParticleFile.cpp
//

#include "FXLinkerPlus-PFX.h"

#include <fstream.h>
#include <iomanip.h>

// Constructor
FXLinkerPlus_ParticleFile::FXLinkerPlus_ParticleFile( const char *_file )
  : file(NULL), users(0) {
  SetFile( _file );
}

// Destructor
FXLinkerPlus_ParticleFile::~FXLinkerPlus_ParticleFile() {
  particles.Flush();
}

// LoadPFX():
bool FXLinkerPlus_ParticleFile::LoadPFX( LWMessageFuncs *message ) {
  if( file == NULL )
    return false;

  bifstream in( file, ios::nocreate );
  in.SetEndian( ENDIAN_MOTOROLA );
  if( !in ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  File I/O error occured opening PFX", file );

    return false;
  }

  // Read the header
  long format, num_particles, num_frames;
  in >> format;
  if( format != 3 ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  Unsupported PFX format (only version 3 is supported) in ", file );

    return false;
  }

  in >> num_particles >> num_frames;

  // Read in the particles
  FXLinkerPlus_Particle *particle;
  FXLinkerPlus_ParticleFrame   *frame;
  long first_frame, last_frame, life_frame;
  float mass, resistance, size;

  long i, j;
  float x, y, z;
  int pfx_first_frame = 0, pfx_last_frame = 0;
  bool initialize_frame_range = true;

  for( i=0; i < num_particles; i++ ) {
    particle = new FXLinkerPlus_Particle;
    particles.Add( particle );

    // Read the frame range
    in >> first_frame >> last_frame >> life_frame;
    particle->SetLifeFrame( life_frame );

    if( initialize_frame_range ) {
      pfx_first_frame = first_frame;
      pfx_last_frame  = last_frame;
    } else {
      if( first_frame < pfx_first_frame )
        pfx_first_frame = first_frame;

      if( last_frame > pfx_last_frame )
        pfx_last_frame = last_frame;
    }

    // Read the particle properties
    in >> mass >> resistance >> size;
    particle->SetMass( mass );
    particle->SetResistance( resistance );
    particle->SetSize( size );

    // See if there's any positional data
    if( first_frame == last_frame )
      continue;

    // Read in the particle's frame data
    for( j=0; j < ((last_frame-first_frame) + 1); j++ ) {
      frame = new FXLinkerPlus_ParticleFrame;
      particle->GetFrames().Add( frame );
      frame->SetFrame( first_frame + j );

      in >> x >> y >> z;
      frame->GetPosition()[0] = x;
      frame->GetPosition()[1] = y;
      frame->GetPosition()[2] = z;
    }
  }

  SetFirstFrame( pfx_first_frame );
  SetLastFrame( pfx_last_frame );

  return true;
}

// LoadPSM():
bool FXLinkerPlus_ParticleFile::LoadPSM( LWMessageFuncs *message ) {
  if( file == NULL )
    return false;

  bifstream in( file, ios::nocreate );
  in.SetEndian( ENDIAN_MOTOROLA );
  if( !in ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  File I/O error occured opening PSM", file );

    return false;
  }

  // Read the header
  const unsigned long PSM_FORM = ('F'<<24) | ('O'<<16) | ('R'<<8) | 'M';
  const unsigned long PSM_PSM2 = ('P'<<24) | ('S'<<16) | ('M'<<8) | '2';
  const unsigned long PSM_HEAD = ('H'<<24) | ('E'<<16) | ('A'<<8) | 'D';
  const unsigned long PSM_FRAM = ('F'<<24) | ('R'<<16) | ('A'<<8) | 'M';
  const unsigned long PSM_POSN = ('P'<<24) | ('O'<<16) | ('S'<<8) | 'N';
  const unsigned long PSM_PSIZ = ('P'<<24) | ('S'<<16) | ('I'<<8) | 'Z';
  const unsigned long PSM_PAGE = ('P'<<24) | ('A'<<16) | ('G'<<8) | 'E';

  unsigned long chunk_id;
  in >> chunk_id;
  if( chunk_id != PSM_FORM ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  File is not a PSM (expected FORM)", file );

    return false;
  }

  long file_size;
  in >> file_size >> chunk_id;
  if( chunk_id != PSM_PSM2 ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  File is not a version 2 PSM (expected PSM2)", file );

    return false;
  }

  long chunk_size;
  in >> chunk_id >> chunk_size;
  if( chunk_id != PSM_HEAD ) {
    if( message != NULL )
      (*message->error)( "FX Linker Plus (Motion) Error:  Missing PSM header (expected HEAD) in ", file );

    return false;
  }

  unsigned long num_particles, i;
  streampos chunk_start = in.tellg();
  in >> num_particles;
  in.seekg( chunk_start + chunk_size, ios::beg );

  // Create all the particles
  for( i=0; i < num_particles; i++ )
    particles.Add( new FXLinkerPlus_Particle );

/*
  // Count the number of frames
  long      chunk_size;
  streampos orig_pos = in.tellg();
  streampos chunk_start;
  
  for( num_frames = 0; /*NULL/; num_frames++ ) {
    // Read in the chunk data
    chunk_start = in.tellg();
    in >> chunk_id >> chunk_size;

    // Scan to the next chunk
    psm.seekg( abs( chunk_size - (in.tellg() - chunk_start ) + 8 ), ios::cur );

    // Error test
    if( (psm.bad() || psm.fail()) && !psm.eof() )
      return -1;

    // EOF test
    if( psm.eof() )
      break;
  }

  // Seek back to the start of the frame data
  psm.seekg( orig_pos, ios::beg );
  psm.clear();
*/

  // Read in the frame data for the particles
  streampos frame_start, frame_size;
  long current_frame = -1;
  float x, y, z, scale;
  FXLinkerPlus_ParticleFrame *frame;

  while( true ) {
    // Error test
    if( (in.bad() || in.fail()) && !in.eof() )
      return false;

    // EOF test
    if( in.eof() )
      break;

    // Look for a FORM????FRAM
    in >> chunk_id >> frame_size;
    if( chunk_id != PSM_FORM ) {
      in.seekg( chunk_size, ios::cur );
      continue;
    }

    frame_start = in.tellg();
    in >> chunk_id;
    if( chunk_id != PSM_FRAM ) {
      in.seekg( frame_start + frame_size + (frame_size % 2), ios::beg );
      continue;
    }

    // Add a new frame to each particle
    current_frame++;
    for( i=0; i < particles.NumElements(); i++ ) {
      frame = new FXLinkerPlus_ParticleFrame;
      particles[i]->GetFrames().Add( frame );
      frame->SetFrame( current_frame );
    }

    // Read in the chunk data we're interested in
    while( in.tellg() < frame_start + frame_size ) {
      // Error test
      if( (in.bad() || in.fail()) && !in.eof() )
        return false;

      // EOF test
      if( in.eof() )
        break;

      in >> chunk_id >> chunk_size;
      chunk_start = in.tellg();
      switch( chunk_id ) {
        case PSM_POSN:
          for( i=0; i < particles.NumElements(); i++ ) {
            frame = particles[i]->GetFrames()[ current_frame ];
            in >> x >> y >> z;
            frame->GetPosition()[0] = x;
            frame->GetPosition()[1] = y;
            frame->GetPosition()[2] = z;
          }
          break;

        case PSM_PSIZ: {
          for( unsigned long i=0; i < particles.NumElements(); i++ ) {
            in >> scale;
            particles[i]->GetFrames()[ current_frame ]->SetScale( scale );
          }
        } break;

        case PSM_PAGE: {
          ofstream out( "i:\\PSMTest.txt", ios::app );
          out << endl;
          out << "PAGE at Frame " << current_frame << ", chunk size " << chunk_size << endl;

          char age;
          for( unsigned long i=0; i < particles.NumElements(); i++ ) {
            in >> age;
            out << setw( 4 ) << i << "   " << (int)age << endl;
          }
        } break;
      }

      in.seekg( chunk_start + chunk_size + (chunk_size % 2), ios::beg );
    }
  }

  SetFirstFrame( 0 );
  SetLastFrame( current_frame );

  return true;
}

//
// --------------------------------------------------------------
//

// Constructor():
FXLinkerPlus_Particle::FXLinkerPlus_Particle()
  : life_frame(0), mass(0.0f), resistance(0.0f), size(1.0f) {

}

// Destructor():
FXLinkerPlus_Particle::~FXLinkerPlus_Particle() {
  frames.Flush();
}

//
// --------------------------------------------------------------
//

// Constructor():
FXLinkerPlus_ParticleFrame::FXLinkerPlus_ParticleFrame()
  : frame(0), scale(1.0f) {

  position[0] = 0.0f;
  position[1] = 0.0f;
  position[2] = 0.0f;
}

// Destructor():
FXLinkerPlus_ParticleFrame::~FXLinkerPlus_ParticleFrame() {
  ;
}

