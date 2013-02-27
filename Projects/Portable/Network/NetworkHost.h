//
// NetworkHost.h
//
// This includes the NetworkHost class, which can be used to get information
//  about the local system and the network.
//
// To initialize this class, you must first initialize and pass a NetworkCore
//  instance to the constructor.  NetworkCore deals with initializing the main
//  network systems and is required for NetworkHost to work.
//
// -- Possible to-do:  Make a NetworkHost instance a member of NetworkCore?
//
// Note that this code is currently Win32 specific.
//
// Note also this class only stores network host information.  Currently
//  it cannot set the information of the host.  The Set series of functions
//  are only for updating the host information from file, recieved buffer,
//  etc.
// 

#ifndef NETWORK_INFO_HEADER
#define NETWORK_INFO_HEADER

#include <portable/dynarray/dynarray.h>

#include "networkipaddress.h"
#include "networkcore.h"

const char *NetHostErrorStrings[];

enum NetworkHostError {
  NETHOST_ERROR_OK = 0,
  NETHOST_ERROR_FAULT,
  NETHOST_ERROR_CORE_NOT_STARTED,
  NETHOST_ERROR_NETWORK_DOWN,
  NETHOST_ERROR_BLOCKED,
  NETHOST_ERROR_HOST_NOT_FOUND,
  NETHOST_ERROR_TRY_AGAIN,
  NETHOST_ERROR_UNRECOVERABLE,
  NETHOST_ERROR_NO_DATA_FOUND,
  NETHOST_ERROR_INTERUPTED,
  NETHOST_ERROR_NOT_CONNECTED,
  NETHOST_ERROR_NOT_A_SOCKET,
  NETHOST_ERROR_UNKNOWN };

class NetworkHost;

class NetworkHost {
public:
  // Constructors/Destructor
  NetworkHost( NetworkCore &_core, bool refresh_local = false );        // Constructs and fills with the local host's information
  NetworkHost( NetworkCore &_core, const char *_name );                 // Constructs and fills with the host with the name of the host provided
  NetworkHost( NetworkCore &_core, NetworkIPAddress &_ip );             // Constructs and fills with the host at the IP address provided
  NetworkHost( NetworkCore &_core, SOCKET socket );                     // Constructs and fills with the host at the other end of the socket provided
  ~NetworkHost();

  // Reset/Refresh
  void               Reset();

  NetworkHostError   Refresh( const char *name = NULL );                   // Refreshes the host information with the host name provided (or local host if name == NULL)
  NetworkHostError   Refresh( NetworkIPAddress &address );                 // Refreshes the host information with the IP provided
  NetworkHostError   Refresh( SOCKET socket );                             // Refreshes the host information with the socket provided

  // Accessors
  inline const char       * GetName() { return host_name; }                               // Returns the host name
  inline       void         SetName( const char *_name ) { strcpy( host_name, _name ); }  // Sets the host name
  inline       short        GetAddressType() { return host_address_type; }                // Returns the address type (as defined in Winsock2.h as AF_*)
  inline       void         SetAddressType( short _type ) { host_address_type = _type; }  // Sets the address type (as defined in Winsock2.h as AF_*)

  inline DynArray< char *> &              GetAliasList()   { return host_alias_list;   }
  inline DynArray< NetworkIPAddress * > & GetAddressList() { return host_address_list; }

  inline NetworkHostError   GetLastError()       { return last_error; }
  inline const char       * GetLastErrorString() { return NetHostErrorStrings[ last_error ]; }
  inline bool               operator!()          { return GetLastError() != NETHOST_ERROR_OK; }

protected:
         NetworkHostError Refresh( const char *name, NetworkIPAddress *address );  // Internal version
  inline NetworkHostError SetError( NetworkHostError _error ) { return (last_error = _error); }

  NetworkHostError last_error;

  NetworkCore &core;

  char                            host_name[512];       // Host name
  DynArray< char * >              host_alias_list;      // Aliases
  short                           host_address_type;    // Type of address (defined in Winsock2.h as AF_*)
  DynArray< NetworkIPAddress * >  host_address_list;    // Host IP addresses
};

#endif