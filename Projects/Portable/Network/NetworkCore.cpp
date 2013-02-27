//
// NetworkCore.cpp
//

#include <assert.h>

#include "NetworkCore.h"

// Constructor
NetworkCore::NetworkCore( bool start )
  : last_error(NETCORE_ERROR_OK), is_started(false) {
  if( start )
    Init();
}

// Destructor
NetworkCore::~NetworkCore() {
  Shutdown();
}

// Init():
//  Initializes the network for access (ie: starts up WinSock under Windows)
NetworkCoreError NetworkCore::Init() {
  if( GetIsStarted() )
    return SetError( NETCORE_ERROR_OK );

  if( GetLastError() != NETCORE_ERROR_OK )
    return GetLastError();
    

#ifdef WIN32
  // Windows-specific variables
  WORD winsock_verion = MAKEWORD( 2, 0 );

  // Start WinSock and test for errors
  int start_error = WSAStartup( winsock_verion, &winsock_data );
  switch( start_error ) {
    case WSASYSNOTREADY:      return SetError( NETCORE_ERROR_SYSTEM_NOT_READY    );    break;
    case WSAVERNOTSUPPORTED:  return SetError( NETCORE_ERROR_UNSUPPORTED_VERSION );    break;
    case WSAEINPROGRESS:      return SetError( NETCORE_ERROR_BLOCKED             );    break;
    case WSAEPROCLIM:         return SetError( NETCORE_ERROR_TOO_MANY_SOCKETS    );    break;
    case WSAEFAULT:           return SetError( NETCORE_ERROR_BAD_DATA            );    break;
  }

  is_started = true;
  return SetError( NETCORE_ERROR_OK );
#else
  assert( false );   // Implement non-Win32 network support here
#endif
}

// Shutdown():
//  Shutsdown the networking system (ie: frees up WinSock under Windows)
NetworkCoreError NetworkCore::Shutdown() {
  if( !GetIsStarted() )
    return SetError( NETCORE_ERROR_OK );

#ifdef WIN32
  WSACleanup();
#else
  assert( false );   // Implement non-Win32 network support here
#endif

  is_started = false;
  return SetError( NETCORE_ERROR_OK );
}
