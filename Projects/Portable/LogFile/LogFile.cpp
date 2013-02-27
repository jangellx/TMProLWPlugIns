//
// LogFile.cpp
//
// Implements the LogFile class
//

#include "LogFile.h"

// Constructor
LogFile::LogFile( const char *_filename, LogDetailLevel new_level )
  : detail_level( new_level ), now_outputting( false ), line_detail_level( 0 ),
    log( NULL ), append_mode(false) {

  if( _filename != NULL )
    strcpy( log_filename, _filename );
  else
    log_filename[0] = '\0';

}

// Destructor
LogFile::~LogFile() {
  if( IsOpen() )
    log.close();
}

// Open()
//  Open the file with the current settings.  Returns true
//  if successful
bool LogFile::Open() {
  if( IsOpen() )
    return false;

  if( append_mode )
    log.open( log_filename, ios::out | ios::app );  // Append Mode
  else
    log.open( log_filename, ios::out );             // Overwrite Mode

  if( !log )
    return false;

  return true;
}

// Open( bool ):
//  Open the log file with the current settings.  Returns true
//  if successful
bool LogFile::Open( bool _append_mode ) {
  if( IsOpen() )
    return false;

  append_mode = _append_mode;

  return Open();
}

// Open( bool, LogDetailLevel ):
//  Open the log file with the current settings.  Returns true
//  if successful.
bool LogFile::Open( bool _append_mode, LogDetailLevel new_level ) {
  if( IsOpen() )
    return false;

  if( log_filename[0] == '\0' )
    return false;

  append_mode  = _append_mode;
  detail_level = new_level;

  return Open();
}

// Open( const char *, bool, LogDetailLevel )
//  Open the log file with the current settings.  Returns true
//  if successful.
bool LogFile::Open( const char *filename, bool _append_mode,
                    LogDetailLevel new_level ) {
  if( IsOpen() )
    return false;

  if( !filename )                            // Test for NULL filename
    return false;

  if( filename[0] == '\0' )                  // Test for empty path
    return false;

  strcpy( log_filename, filename );
  append_mode  = _append_mode;
  detail_level = new_level;

  return Open();
}

// Close()
//  Close the log file
bool LogFile::Close() {
  log.close();
  now_outputting = false;

  return true;
}

// IsOpen()
//  Returns true if the file is currently open
bool LogFile::IsOpen() {
  if( log.is_open() )
    return true;

  return false;
}

// SetFilename()
//  Set the filename of the log.  Only possible when the
//  log is not yet open.
bool LogFile::SetFilename( const char *new_filename ) {
  if( IsOpen() )
    return false;

  if( new_filename == NULL )
    return false;

  strcpy( log_filename, new_filename );
  return true;
}

// SetDetailLevel()
//  Set the detail level of the log.  This is only possible
//  when the log is not yet open.  Returns  the last detail
//  level, or 0 if the log is already opened and cannot be
//  Since you can have a detail level of 0, it's best to
//  check to see if the log file has already been opened
//  with IsOpen() before trying to change this.
LogDetailLevel LogFile::SetDetailLevel( LogDetailLevel new_level ) {
  if( IsOpen() )
    return 0;

  LogDetailLevel old_level = detail_level;
  detail_level = new_level;
  return old_level;
}

// SetAppendMode()
//  Set the append mode.  Only valid if the log is not yet open.
//  Returns the previous append mode.  Note that IsOpen() should
//  be checked to insure that the append mode would actually be
//  changed.
bool LogFile::SetAppendMode( bool new_mode ) {
  if( IsOpen() )
    return false;

  bool old_mode = append_mode;
  append_mode = new_mode;

  return append_mode;
}

// ---
// ------------------------------------------ Stream Operators ---
// --- 
//LogFile& LogFile::operator<<(LogFile& (__cdecl *_f)(LogFile&) ) {
//  return 
//}
