//
// NetworkHost.cpp
//

#include <assert.h>

#include "NetworkHost.h"

const char *NetHostErrorStrings[] = {
  "No error",
  "Network fault",
  "Network core not started",
  "Network is down",
  "Action blocked",
  "Host not found",
  "Error; try again",
  "Unrecoverable error",
  "No data found",
  "Action interupted",
  "Not connected",
  "Not a socket",
  "Unknown error" };


// Constructor( bool )
NetworkHost::NetworkHost( NetworkCore &_core, bool refresh_host_info )
  : core(_core) {

  Reset();
  if( refresh_host_info )
    Refresh();
}

// Constructor( const char * )
NetworkHost::NetworkHost( NetworkCore &_core, const char *_name )
  : core(_core) {

  Reset();
  if( _name != NULL )
    Refresh( _name );
}

// Constructor( const NetworkAddress & )
NetworkHost::NetworkHost( NetworkCore &_core, NetworkIPAddress &_ip )
  : core(_core) {

  Refresh( _ip );
}

// Destructor
NetworkHost::~NetworkHost() {
  Reset();
}

// Reset():
void NetworkHost::Reset() {
  last_error = NETHOST_ERROR_OK;
  host_name[0] = '\0';
  host_address_type = AF_UNSPEC;
  host_address_list.Flush();

  for( unsigned int i=0; i < host_alias_list.NumElements(); i++ )
    free( host_alias_list[i] );
}

// Refresh( SOCKET ):
//  Obtains the latest host information.  Note this only gets
//   the IP address via getpeername()
NetworkHostError NetworkHost::Refresh( SOCKET socket ) {
  SOCKADDR_IN temp_addr;
  int         length = sizeof( temp_addr );

  Reset();
  int error = getpeername( socket, (SOCKADDR *)&temp_addr, &length );
  if( error == 0 ) {
    host_address_list.Add( new NetworkIPAddress( temp_addr.sin_addr.S_un.S_un_b.s_b1,
                                                 temp_addr.sin_addr.S_un.S_un_b.s_b2,
                                                 temp_addr.sin_addr.S_un.S_un_b.s_b3,
                                                 temp_addr.sin_addr.S_un.S_un_b.s_b4 ) );
  } else {
    Reset();
    switch( error ) {
      case WSAEFAULT:            return SetError( NETHOST_ERROR_FAULT            );    break;
      case WSANOTINITIALISED:    return SetError( NETHOST_ERROR_CORE_NOT_STARTED );    break;
      case WSAENETDOWN:          return SetError( NETHOST_ERROR_NETWORK_DOWN     );    break;
      case WSAEINPROGRESS:       return SetError( NETHOST_ERROR_BLOCKED          );    break;
      case WSAENOTCONN:          return SetError( NETHOST_ERROR_NOT_CONNECTED    );    break;
      case WSAENOTSOCK:          return SetError( NETHOST_ERROR_NOT_A_SOCKET     );    break;
    }
    return SetError( NETHOST_ERROR_UNKNOWN );
  }

  return NETHOST_ERROR_OK;
}

// Refresh( const NetworkIPAddress & ):
//  Obtains the latest host information (name, IP, etc).
NetworkHostError NetworkHost::Refresh( NetworkIPAddress &address) {
  return Refresh( NULL, &address );
}

// Refresh( const char *name ):
//  Obtains the latest host information (name, IP, etc).
NetworkHostError NetworkHost::Refresh( const char *name ) {
  return Refresh( name, NULL );
}

// Refresh( const char *name ):
//  Obtains the latest host information (name, IP, etc).
NetworkHostError NetworkHost::Refresh( const char *name, NetworkIPAddress *address ) {
  if( !core.GetIsStarted() )
    return SetError( NETHOST_ERROR_CORE_NOT_STARTED );

#ifdef WIN32
  try {
    // Reset the data
    Reset();

    // Obtain the Host Name
    if( address == NULL ) {
      if( name == NULL ) {
        int refresh_error = gethostname( host_name, sizeof(host_name) );
        if( refresh_error != 0 )
          throw( refresh_error );
      }
    }

    // Obtain other Host Info
    PHOSTENT hostinfo = NULL;
    if( address == NULL )
      hostinfo = gethostbyname( host_name );
    else
      hostinfo = gethostbyaddr( (const char *)address->GetIP(), 4, AF_INET );

    if( hostinfo == NULL )
      throw( WSAGetLastError() );

    // Store the host name
    strcpy( host_name, hostinfo->h_name );

    // Store the address type
    host_address_type = hostinfo->h_addrtype;

    // Store the IP address list
    struct in_addr ** host_ip_list = (struct in_addr **)hostinfo->h_addr_list;
    for( int i=0; hostinfo->h_addr_list[i] != NULL; i++ ) {
      host_address_list.Add( new NetworkIPAddress( host_ip_list[i]->S_un.S_un_b.s_b1,
                                                   host_ip_list[i]->S_un.S_un_b.s_b2,
                                                   host_ip_list[i]->S_un.S_un_b.s_b3,
                                                   host_ip_list[i]->S_un.S_un_b.s_b4 ) );
    } 

    // Store the Alias list
    for( i=0; hostinfo->h_aliases[i] != NULL; i++ )
      host_alias_list.Add( strdup( hostinfo->h_aliases[i] ) );

    return SetError( NETHOST_ERROR_OK );
  } catch( int error ) {
    Reset();
    switch( error ) {
      case WSAEFAULT:            return SetError( NETHOST_ERROR_FAULT            );    break;
      case WSANOTINITIALISED:    return SetError( NETHOST_ERROR_CORE_NOT_STARTED );    break;
      case WSAENETDOWN:          return SetError( NETHOST_ERROR_NETWORK_DOWN     );    break;
      case WSAEINPROGRESS:       return SetError( NETHOST_ERROR_BLOCKED          );    break;
      case WSAHOST_NOT_FOUND:    return SetError( NETHOST_ERROR_HOST_NOT_FOUND   );    break;
      case WSATRY_AGAIN:         return SetError( NETHOST_ERROR_TRY_AGAIN        );    break;
      case WSANO_RECOVERY:       return SetError( NETHOST_ERROR_UNRECOVERABLE    );    break;
      case WSANO_DATA:           return SetError( NETHOST_ERROR_UNRECOVERABLE    );    break;
      case WSAEINTR:             return SetError( NETHOST_ERROR_INTERUPTED       );    break;
    }
    return SetError( NETHOST_ERROR_UNKNOWN );
  }
#else
  assert( false );   // Insert non-Win32 network code here
#endif
}
