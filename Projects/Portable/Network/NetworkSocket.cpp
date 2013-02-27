//
// NetworkSocket.cpp
//

#include "NetworkSocket.h"

// Constructor
NetworkSocket::NetworkSocket( NetworkCore &_core )
  : core(_core), port(0), socket_type(SOCK_STREAM),
    protocal(IPPROTO_TCP), family(AF_INET) {
  ;
}

// Destructor
NetworkSocket::~NetworkSocket() {
  Shutdown();
}

// Connect()
bool NetworkSocket::Connect( NetworkIPAddress *_ip ) {
  return true;
}

// Startup()
//  Starts a socket, but doesn't open the connection
bool NetworkSocket::Startup( NetworkIPAddress *_ip, int _type, int _protocal ) {
  return true;
}

// Shutdown()
//  Shuts down the send and/or recieve ports
bool NetworkSocket::Shutdown( int how ) {
  shutdown( socket, how );
  return true;
}