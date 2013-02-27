//
// FrameJumper-Data.h
//

#include <portable/dynarray/dynarray.h>

#include "FrameJumper-Group.h"

class FrameJumper_Data {
public:
  FrameJumper_Data() : users( 0 ), current_group( 0 ), command( NULL ), file_io_occured( false )
                     { ; }
  ~FrameJumper_Data() { groups.Flush(); SetCommand( NULL ); }

  // Management
  inline int OneMoreUser() { return ++users; }
  inline int OneLessUser() { if( --users < 1 ) { users = 0; groups.Flush(); current_group = -1; } return users; }  // Decrements the user count.  If it becomes 0, the marker list is cleared, as there are no instances left to use it.
  inline int GetUsers()    { return users; }

  inline void SetFileIOOccured( bool state )  { file_io_occured = state; }
  inline int  GetFileIOOccured()              { return file_io_occured;  }

  inline void          SetCurrentGroup( unsigned long _current ) { current_group = _current; }
  inline unsigned long GetCurrentGroup() { return current_group; }

  // Accessors
  inline DynArray< FrameJumper_Group * > & GetGroups() { return groups; }

  // Command Launch
  const char * GetCommand() { return command; }
  void         SetCommand( const char *_command ) { StringTools::strdup2( command, _command ); }

  // Global Function
  GlobalFunc *global;

protected:
  DynArray< FrameJumper_Group * > groups;

  unsigned long current_group;

  char * command;                       // Command to issue when event() is called, or NULL to do nothing.

  int users;                            // Number of instances using this class

  bool file_io_occured;                 // True if any instances have been saved or loaded recently.  Used to insure that all duplicate instances save empty data, and that the duplicates don't load.  Reset after each new event
};

