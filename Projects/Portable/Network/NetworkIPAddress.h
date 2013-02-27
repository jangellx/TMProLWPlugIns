//
// NetworkIPAddress.h
//
// This contains the NetworkIPAddress class, which is a simple datatype
//  to hold internet IP addresses and manupulate them.
//

#ifndef NETWORK_IP_ADDRESS_HEADER
#define NETWORK_IP_ADDRESS_HEADER

#include <portable/pfstream/pfstream.h>

class NetworkIPAddress;

class NetworkIPAddress {
public:
  NetworkIPAddress( unsigned char a=0, unsigned char b=0,
                    unsigned char c=0, unsigned char d=0 );
  NetworkIPAddress( unsigned long _ip );
  ~NetworkIPAddress();

  // Accessors
  inline const unsigned char * GetIP() { return ip; }
  inline const unsigned char * SetIP( unsigned char a, unsigned char b, unsigned char c, unsigned char d ) { ip[0] = a; ip[1] = b; ip[2]=c; ip[3]=d; return ip; }
  inline const unsigned char * SetIP( unsigned char *_ip ) { ip[0] = _ip[0]; ip[1] = _ip[1]; ip[2]=_ip[2]; ip[3]=_ip[3]; return ip; }

         const unsigned char * SetIPFromLong( unsigned long _ip );
               unsigned long   GetIPAsLong();

         const unsigned char * StringToIP( const char *string );
                        char * IPToString(       char *string );

  // Staitc Members
  static       unsigned char * StringToIP( const char *string, unsigned char *_ip );
  static                void   StringToIP( const char *string, unsigned char &a, unsigned char &b, unsigned char &c, unsigned char &d );
  static                char * IPToString( char       *string, unsigned char a,  unsigned char b,  unsigned char c,  unsigned char d  );
  static                char * IPToString( char       *string, unsigned char *_ip );

  // Operators
  NetworkIPAddress & operator=( NetworkIPAddress &other_ip );            // Copy

  // Tests
  bool                         IsEmpty() { return ( ((long)ip == 0) ? true : false ); }

protected:
  unsigned char ip[4];
};

ostream   & operator<<( ostream &out,   NetworkIPAddress &ip );   // Output to stream
istream   & operator>>( istream &in,    NetworkIPAddress &ip );   // Input from stream

#endif