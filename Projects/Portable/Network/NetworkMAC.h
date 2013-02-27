//
// NetworkMAC.h
//
// The NetworkMAC class obtains MAC addresses from ethernet
//  cards installed in the system.
//

#include <portable/dynarray/dynarray.h>

#include <stdio.h>

#include "networkcore.h"

class NetworkMACAddress;

// class NetworkMAC
class NetworkMAC {
public:
  NetworkMAC();
  ~NetworkMAC();

  // Accessors
  DynArray< NetworkMACAddress * > &GetMACList() {  return mac_list; }
  void ObtainMACs();

protected:
  DynArray< NetworkMACAddress * > mac_list;
};

// class NetworkMACAddress
class NetworkMACAddress {
public:
  inline NetworkMACAddress( const unsigned char * new_mac = NULL ) { (new_mac == NULL) ? SetMAC( (const unsigned char *)"\0\0\0\0\0\0" ) : SetMAC( new_mac ); }
  inline NetworkMACAddress( char a, char b, char c, char d, char e, char f ) { SetMAC( a, b, c, d, e, f ); }
  inline ~NetworkMACAddress() { ; }

  // Accessors
  inline const unsigned char * GetMAC() { return mac; }
  inline                void   SetMAC( const unsigned char * new_mac ) { memcpy( (char *)mac, (const char *)new_mac, 6 ); }
  inline                void   SetMAC( char a, char b, char c, char d, char e, char f ) {
                                       mac[0] = a; mac[1] = b; mac[2] = c; mac[3] = d, mac[4] = e; mac[5] = f; }

  inline                char * MACToString( char * buffer ) { sprintf( buffer, "%02x-%02x-%02x-%02x-%02x-%02x",
                                                                       mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], mac[6] );
                                                              return buffer; }

  inline NetworkMACAddress & operator=( NetworkMACAddress &other_inst ) { SetMAC( other_inst.GetMAC() );
                                                                          return *this; }

protected:
  unsigned char mac[6];
};

