//
// NetworkCore.h
//
// This includes the NetworkCore class, which contains the core networking
//  stuff, including startup and shutdown of network services.  Many other
//  network classes and functions take an instance of this as an argument.
//
// Currently this code is Win32 specific
//

#ifndef NETWORK_CORE_HEADER
#define NETWORK_CORE_HEADER

#ifdef WIN32
  #define FD_SETSIZE 32767  // Increase the number of sockets from 64 to something reasonable

  #include <winsock2.h>     // Must be before windows.h to avoid inclusion of Winsock.h (Winsock 1.1)
  #include <windows.h>
#endif

enum NetworkCoreError {
  NETCORE_ERROR_OK = 0,
  NETCORE_ERROR_NOT_STARTED,
  NETCORE_ERROR_SYSTEM_NOT_READY,
  NETCORE_ERROR_UNSUPPORTED_VERSION,
  NETCORE_ERROR_BLOCKED,
  NETCORE_ERROR_TOO_MANY_SOCKETS,
  NETCORE_ERROR_BAD_DATA };

class NetworkCore;

class NetworkCore {
public:
  NetworkCore( bool start = true );
  ~NetworkCore();

  NetworkCoreError Init();          // Normally called by the Constructor
  NetworkCoreError Shutdown();      // Normally called by the Destructor

  bool GetIsStarted() { return is_started; }

  inline NetworkCoreError GetLastError() { return last_error; }
  inline bool             operator!()    { return GetLastError() != NETCORE_ERROR_OK; }

protected:
  inline NetworkCoreError SetError( NetworkCoreError _error ) { return (last_error = _error); }

  NetworkCoreError last_error;
  bool is_started;

#ifdef WIN32
  WSADATA winsock_data;
#endif
};

#endif