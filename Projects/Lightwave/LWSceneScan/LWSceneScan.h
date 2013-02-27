//
// LWSceneScan.h
//
// This class is designed for programs like FreeKey, UberNull, etc. which
//  modify small parts of a scene file, leaving the rest intact.  It will
//  simply scan the scene file for a series of keywords provided, returning
//  the index of the one that is matched, optionally passing unrecognized
//  data to an output file.  When a key is found, it's index is returned
//  so the calling program can process it.
//
// LWSceneScan handles opening the source and output files, plues any
//  preliminary header checks on the source.
//
// The keyword lists should be a null-terminated const char * array.  The
//  scan functions will return the index of the keyword found.
//
// By default, the keyword scan is case sensitive.  Use SetCaseSenseitive()
//  to toggle this
//
// If the progress events flags is true, then a progress event will be sent
//  every 10 lines.  This simply means that ScanNext() will stop and return
//  LWSCAN_PROGRESS_EVENT.  This can be used to update a progress bar with
//  the status of the scan.
//

#include <portable/dirtools/dirtools.h>
#include <portable/pfstream/pfstream.h>
#include <portable/stringtools/stringtools.h>

// LWSceneScanError
//  These are all negative, so if the retval of a function is >= 0,
//   there is no error.
enum LWSceneScanError {
  LWSCAN_NO_MATCH                =  -1,
  LWSCAN_NOT_A_LW_SCENE          =  -2,
  LWSCAN_SOURCE_DOESNT_EXIST     =  -3,
  LWSCAN_ERROR_OPENING_SOURCE    =  -4,
  LWSCAN_READ_ERROR              =  -5,
  LWSCAN_UNEXPECTED_EOF          =  -6,
  LWSCAN_BAD_SOURCE_NAME         =  -7,
  LWSCAN_END_OF_FILE_REACHED     =  -8,

  LWSCAN_ERROR_OPENING_OUTPUT    =  -9,
  LWSCAN_WRITE_ERROR             = -10,
  LWSCAN_BAD_OUTPUT_NAME         = -11,

  LWSCAN_PROGRESS_EVENT          = -12,
  LWSCAN_NUM_ERRORS              = -13 };

extern const char *LWSceneScanErrorStrings[];

class LWSceneScan;

class LWSceneScan {
public:
  // Enums
  enum LWScanMode {
    read_only   = 0,
    read_write };

  // Constructor/Destructor
  LWSceneScan( const char **keywords = NULL, const char *source = NULL,
               const char *output = NULL );
  ~LWSceneScan();

  // Scan Functions
  int  StartScan( LWScanMode mode, const char *_source = NULL, const char *_output = NULL,
                  const char **_keywords = NULL );                             // Begin scan, including opening the file
  int  RestartScan();                                                          // Restart the scan from the beinging (ie: EndScan(), then StartScan())
  int  ScanNext();                                                             // Scan to the next keyword
  int  OutputAndScanNext();                                                    // Output the buffer, and then scan to the next keyword
  void EndScan();                                                              // Stop scanning, including closing the file

  LWScanMode GetScanMode() { return scan_mode; }

  inline void SetProgressEvents( bool state ) { progress_events = state; }
  inline bool GetProgressEvents() { return progress_events; }

  // Path Accessors
  inline       void   SetSource( const char *_source ) { StringTools::strdup2( source, _source ); }
  inline const char * GetSource() { return source; }
  inline       void   SetOutput( const char *_output ) { StringTools::strdup2( output, _output ); }
  inline const char * GetOutput() { return output; }

  // Keyword Functions
         void    SetKeywordList( const char **_keywords ) { keywords = _keywords; }
  const  char ** GetKeywordList() { return keywords; }

  inline int     GetVersion() { return version; }                              // Returns the version of the scene file

         void    SetCaseSensitive( bool new_case ) { case_sensitive = new_case; }
         bool    GetCaseSensitive() { return case_sensitive; }

  // Buffer Accessors
  inline int    GetLastKeyMatched() { return last_key_matched; }
  inline char * GetBuffer()  { return buffer; }                                                   // Returns the buffer last read in; this is read/write
  inline char * GetBufferSansKeyword() { return &(buffer[ strlen( keywords[last_key_matched] ) + 1 ]); } // Returns the buffer, minus the keyword and the pressumed space after it

  // Stream Accessors
  bool               AreFilesOpen() { return are_files_open; }
  inline pifstream & GetInputStream() { return in; }
  inline pofstream & GetOutputStream() { return out; }

protected:
  pifstream  in;
  pofstream  out;
  bool       are_files_open;

  char     * source;
  char     * output;

  LWScanMode scan_mode;
  bool       case_sensitive;

  int        version;

  const char **keywords;

  int  last_key_matched;
  char buffer[ 8192 ];

  bool       progress_events;
  int        progress_line_count;
};
