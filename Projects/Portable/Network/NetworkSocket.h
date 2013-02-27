//
// NetworkSocket.h
//

#ifndef HEADER_NETWORK_SOCKET
#define HEADER_NETWORK_SOCKET

#include "NetworkCore.h"
#include "NetworkIPAddress.h"

class NetworkSocket {
public:
  // Constructor/Destructor
  NetworkSocket( NetworkCore &_core );
  ~NetworkSocket();

  // Startup/Shutdown
  bool Startup( NetworkIPAddress *_ip = NULL, int _type = -1, int _protocal = -1 );  // Starts a socket, but doesn't open the connection
  bool Shutdown( int how = SD_BOTH );                                                // Frees a socket, inlcuding shutting down the connection

  // Connect/Disconnect
  bool Connect( NetworkIPAddress *_ip );
  bool Disconnect();

  bool Bind( int port, NetworkIPAddress &_ip );

  bool Listen( int backlog = -1 );
  bool Accept();

  // Accessors
  inline void              SetAddress( NetworkIPAddress &_ip ) { ip = _ip; }
  inline NetworkIPAddress &GetAddress() { return ip; }

  inline int               GetPort() { return port; }
  inline void              SetPort( int _port ) { port = _port; }

  inline int               GetProtocal() { return protocal; }
  inline void              SetProtocal( int _protocal ) { protocal = _protocal; }

  inline int               GetFamily() { return family; }
  inline void              SetFamily( int _family ) { family = _family; }

  inline int               GetSocketType( int _type ) { socket_type = _type; }
  inline void              SetSocketType( int _type ) { socket_type = _type; }

  inline bool              IsStarted() { return (socket == NULL) ? false : true; }
  inline bool              IsConnected();

protected:
  NetworkIPAddress  ip;
  NetworkCore      &core;
  
  int               port;

  int               socket_type;
  int               protocal;
  int               family;

#ifdef WIN32
  SOCKET            socket;
#endif
};

#endif