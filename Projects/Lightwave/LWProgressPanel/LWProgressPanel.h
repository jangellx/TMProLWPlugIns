//
// LWProgressPanel.h
//

#ifndef LW_PROGRESS_PANEL_HEADER
#define LW_PROGRESS_PANEL_HEADER

#ifndef NULL
  #define NULL 0
#endif

#include <lightwave/lw_sdk_cpp_wrappers/lwwrappers_l6.h>
#include <portable/stringtools/stringtools.h>

// class LWProgressPanel
class LWProgressPanel {
public:
  LWProgressPanel( GlobalFunc *_global, int _total = 0 );
  ~LWProgressPanel();

  // Management
  int  Open( const char * _title );
  void Close();
  bool Handle();

  inline bool Handle( int _current, const char *_string ) { SetBar( _current, _string ); return Handle(); }

  // Accessors
  inline bool         IsOpen() { return is_open; }

  inline int          GetTotal() { return total; }
  inline void         SetTotal( int _total ) { total = _total; }

  inline int          GetCurrent() { return current; }
  inline void         SetCurrent( int _current ) { current = _current; }

  inline void         SetString( const char *_string ) { StringTools::strdup2( string, _string ); }
  inline const char * GetString() { return string; }

  inline void         IncrementBar( const char *_string = NULL ) { current++; if( _string != NULL ) SetString(_string); }
  inline void         SetBar( int _current, const char *_string ) { SetCurrent(_current); SetString(_string); }

  inline void         SetAbortMessage( const char *_title, const char *_string1, const char *_string2 ) {
                                       StringTools::strdup2( abort_title,   _title   );
                                       StringTools::strdup2( abort_string1, _string1 );
                                       StringTools::strdup2( abort_string2, _string2 ); }

public:
  GlobalFunc      *global;

  LWMessageFuncs  *message;
  LWPanelFuncs    *panel_funcs;

  LWPanelID        panel;
  LWControl       *bar;

protected:
  int    total;
  int    current;
  char * string;

  bool   is_open;

  char * abort_title;
  char * abort_string1;
  char * abort_string2;
};

void DrawProgressBar( LWControl *con, void *data, DrMode mode );

#endif
