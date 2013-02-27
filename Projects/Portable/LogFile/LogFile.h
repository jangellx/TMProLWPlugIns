//
// LogFile.h
//
// The LogFile class makes it easy to generate complete, accurate log files.
//  Note that although this class uses pofstream, it does not inherit from it.
//
// Every line the log file outputs contains the time and date. It has
//  stream-style controls for outputting log information.  There is
//  also a function to get the time and output the elapsed time from
//  a previously stored time, thus making it simple to tell how long
//  an operation took.
//
// Log files are automatically craeted in Append mode, although this
//  can be overridden to Overwrite mode.
//
// There is a log file setting for the detail level.  When the
//  sream operators are used to output data, the first argument must
//  consist of a log detail level.  If the log file's detail level
//  is greater than the value presented, it will be output to the file.
//  Otherwise, the data will be ignored.  This makes it easy to have
//  the user decide how much information he wants in the log file.  The
//  last value output to the stream must a an endlog, which tells the
//  log file that you have finished outputing the current line of data
//  and outputs end EOL to the buffer.
//  

#pragma once

#include <portable/pfstream/pfstream.h>
#include <portable/dirtools/dirtools.h>

// TypeDefs
typedef int LogDetailLevel;        // A LogDetailLevel must be be the first thing passed when outputing a line to a log!

// Class Declarations
class LogFile;

// Class Definitions
class LogFile {
  friend LogFile& endllog(LogFile& out);
public:
  LogFile( const char *_filename = NULL, LogDetailLevel new_level = 0 );
  ~LogFile();

  // Control
  bool Open();                                                   // Open the log file
  bool Open( bool _append_mode );                                // Open the log file
  bool Open( bool _append_mode, LogDetailLevel new_level );      // Open the log file
  bool Open( const char *filename, bool _append_mode,            // Open the log file
             LogDetailLevel new_level );                         
  bool Close();                                                  // Close the log file
  
  bool IsOpen();                                                 // Returns true if the file is currently open
  
  // Settings
  bool                  SetFilename( const char * new_filename);    // Set the name of the log file.  Can only be done BEFORE the log file is opened!
  inline const char *   GetFilename()    { return log_filename; }   // Get the name of the log file
  LogDetailLevel        SetDetailLevel( LogDetailLevel new_level ); // Set the log detail level.  Returns the current level.  Can only be done BEFORE the log is opened!
  inline LogDetailLevel GetDetailLevel() { return detail_level; }   // Get the current detail level.
  bool                  SetAppendMode( bool new_mode );             // Set the log Append mode.  Returns previous mode.  Can only be set before the log is opened!
  inline bool           GetAppendMode() { return append_mode; }     // Returns the current Append mode.

  // Stream Operators
  LogFile& operator<<(LogFile& (__cdecl * _f)(LogFile&));
  LogFile& operator<<(ios& (__cdecl * _f)(ios&));
  LogFile& operator<<(const char *);
  LogFile& operator<<(const unsigned char *);
  LogFile& operator<<(const signed char *);
  LogFile& operator<<(char);
  LogFile& operator<<(unsigned char);
  LogFile& operator<<(signed char);
  LogFile& operator<<(short);
  LogFile& operator<<(unsigned short);
  LogFile& operator<<(int);
  LogFile& operator<<(unsigned int);
  LogFile& operator<<(long);
  LogFile& operator<<(unsigned long);
  LogFile& operator<<(float);
  LogFile& operator<<(double);
  LogFile& operator<<(long double);
  LogFile& operator<<(const void *);
  LogFile& operator<<(streambuf*);

protected:
  char           log_filename[MAX_PATH_LENGTH];                  // Filename of the log file
  pofstream      log;                                            // Pointer to the output file stream
  bool           append_mode;                                    // Log file append mode

  LogDetailLevel detail_level;                                   // Current detail level

  bool           now_outputting;                                 // true if the stream operator has been outputing data, but hasn't recieved an endlog yet.
  LogDetailLevel line_detail_level;                              // Detail level of the line being output by the stream operators
};

// End-Of-Line function for Log files
inline LogFile& endllog(LogFile& out) { out.log << out.log.endl(out.log); // Use this instead of endl to output the EOL characters
                                        out.log.flush();
                                        return out; }
