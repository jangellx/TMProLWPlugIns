//
// LWSceneScan.cpp
//

#include "LWSceneScan.h"

const char *LWSceneScanErrorStrings[] = {
  "LWScan:  Not a valid error string",
  "LWScan:  No Matching Keyword Found",
  "LWScan:  Source isn't a Lightwave Scene File",
  "LWScan:  Source scene doesn't exists",
  "LWScan:  An error occured opening the source scene",
  "LWScan:  An error occured reading from the source scene",
  "LWScan:  Unexpected end of file found",
  "LWScan:  Bad Source Scene name; unable to open file",
  "LWScan:  End of source scen reached",

  "LWScan:  An error occured opening the output scene",
  "LWScan:  An error occured writing to the output scene",
  "LWScan:  Bad Output Scene name; unable to open file",
  "LWScan:  I'm just the error count -- ignore me" };

// Constructor
LWSceneScan::LWSceneScan( const char **_keywords, const char *_source, const char *_output )
  : keywords(_keywords), last_key_matched(LWSCAN_NO_MATCH), source(NULL), output(NULL),
    version(0), are_files_open(false), scan_mode(read_only), progress_events(false),
    progress_line_count(0), case_sensitive(true) {

  buffer[0] = '\0';

  SetSource( _source );
  SetOutput( _output );
}

// Destructor
LWSceneScan::~LWSceneScan() {
  EndScan();

  SetSource( NULL );
  SetOutput( NULL );
}

// StartScan():
//  This opens the Source and Output file streams.   It performs a LWSC header
//   test and returns the version of the scene file, or an LWSceneScanError if
//   something went wrong.  This does not actually perform any keyword matches.
//  If mode is read_write, then all data that doesn't match a keyword will be
///  passed into the output file.  If in read_only mode, the output file will
//   not be opened.
int LWSceneScan::StartScan( LWScanMode mode, const char *_source,
                            const char *_output, const char **_keywords ) {
  progress_line_count = 0;

  if( _source != NULL )
    SetSource( _source );

  if( _output != NULL )
    SetOutput( _output );

  if( _keywords != NULL )
    SetKeywordList( _keywords );

  // Test the paths
  if( GetSource() == NULL )
    return LWSCAN_BAD_SOURCE_NAME;

  if( mode == read_write ) {
    if( GetOutput() == NULL )
      return LWSCAN_BAD_OUTPUT_NAME;
  }

  // Initialize instance
  last_key_matched = LWSCAN_NO_MATCH;
  buffer[0] = '\0';
  version   = 0;

  in.close();
  in.clear();
  out.close();
  out.clear();

  // Open the source file
  if( !DirInfo::Exists( GetSource() ) )
    return LWSCAN_SOURCE_DOESNT_EXIST;

  in.open( GetSource(), ios::nocreate | ios::binary );
  if( !in )
    return LWSCAN_ERROR_OPENING_SOURCE;

//  in.GuessEOLType();
  in.SetAnyEOL( true );

  // Header and Version Check
  in.getline( buffer, 8192 );
  if( strncmp( buffer, "LWSC", 4 ) != 0 )
    return LWSCAN_NOT_A_LW_SCENE;

  in.getline( buffer, 8192 );
  version = atoi( buffer );

  // Open the output file
  if( mode == read_write ) {
    out.open( GetOutput(), ios::binary );
    if( !out ) {
      in.close();
      return LWSCAN_ERROR_OPENING_OUTPUT;
    }

    out.SetEOLType( in.GetEOLType() );
    out << "LWSC" << out.endl();
    out << version << out.endl();
  }

  // All OK, return
  are_files_open = true;
  return version;
}

// EndScan():
//  Close the files and stop scanning
void LWSceneScan::EndScan() {
  in.close();
  out.close();

  are_files_open = false;
}

// ScanNext():
//  Finds the next keyword match.  If the scan mode is read_write,
//   then anything not recognized will be passed to the output file.
int LWSceneScan::ScanNext() {
  int i;

  do {
    // Handle Progress Events, if enabled
    if( progress_events ) {
      progress_line_count++;
      if( progress_line_count > 9 ) {
        progress_line_count = 0;
        return LWSCAN_PROGRESS_EVENT;
      }
    }

    // Read in the next line
    in.getline( buffer, 8192 );

    // Error Test
    if( (in.bad() || in.fail()) && !in.eof() )
      return LWSCAN_READ_ERROR;

    // Keyword Comparison
    for( i=0; /*NULL*/; i++ ) {
      if( GetKeywordList()[i] == NULL )
        break;

      if( case_sensitive ) {
        if( strncmp( buffer, GetKeywordList()[i], strlen( GetKeywordList()[i] ) ) == 0 ) {
          last_key_matched = i;
          return GetLastKeyMatched();
        }
      } else {
        if( strnicmp( buffer, GetKeywordList()[i], strlen( GetKeywordList()[i] ) ) == 0 ) {
          last_key_matched = i;
          return GetLastKeyMatched();
        }
      }
    }

    if( scan_mode == read_write ) {
      // Unrecognized keyword; output it and continue
      out << buffer << out.endl();

      if( out.bad() || out.fail() )
        return LWSCAN_WRITE_ERROR;
    }
  } while( !in.eof() );

  return LWSCAN_END_OF_FILE_REACHED;
}

// OutputAndScanNext():
//  Same as ScanNext(), but first outputs whatever's in the buffer.
//   This is useful when you only want to read a particular value.
int LWSceneScan::OutputAndScanNext() {
  if( scan_mode == read_write ) {
    out << buffer << out.endl();

    if( out.bad() || out.fail() )
      return LWSCAN_WRITE_ERROR;
  }

  return ScanNext();
}
