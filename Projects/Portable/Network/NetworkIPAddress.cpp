//
// NetworkIPAddress.cpp
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "NetworkIPAddress.h"

// Constructor
NetworkIPAddress::NetworkIPAddress( unsigned char a, unsigned char b,
                                    unsigned char c, unsigned char d ) {
  SetIP( a, b, c, d );
}

// Constructor
NetworkIPAddress::NetworkIPAddress( unsigned long _ip ) {
  SetIPFromLong( _ip );
}

// Destructor
NetworkIPAddress::~NetworkIPAddress() {
  ;
}

// SetIPFromLong():
//  Sets the IP address from a long.  Handles endian conversions.
const unsigned char * NetworkIPAddress::SetIPFromLong( unsigned long _ip ) {
#ifdef WIN32
  char *buffer = (char *)&_ip;
  ip[0] = buffer[3];
  ip[1] = buffer[2];
  ip[2] = buffer[1];
  ip[3] = buffer[0];

  return ip;
#endif
}

// SetIPFromLong():
//  Gets a long representing the IP address.  Handles endian conversions.
unsigned long NetworkIPAddress::GetIPAsLong() {
#ifdef WIN32
  unsigned long output;
  char *buffer = (char *)&output;
  buffer[0] = ip[3];
  buffer[1] = ip[2];
  buffer[2] = ip[1];
  buffer[3] = ip[0];

  return output;
#endif
}

// StringToIP( char * ):
//  Converts a string in the format of "0.0.0.0" into an IP address
const unsigned char * NetworkIPAddress::StringToIP( const char *string ) {
  return StringToIP( string, ip );
}

// StringToIP( char *, unsigned char, unsigned char, unsigned char, unsigned char ):
//  Converts a string in the format of "0.0.0.0" into an IP address
void NetworkIPAddress::StringToIP( const char *string, unsigned char &a, unsigned char &b, unsigned char &c, unsigned char &d ) {
  unsigned char _ip[4];

  StringToIP( string, _ip );
  a = _ip[0];
  b = _ip[1];
  c = _ip[2];
  d = _ip[3];
}

// StringToIP( char *, unsigned char * ):
//  Converts a string in the format of "0.0.0.0" into an IP address
unsigned char * NetworkIPAddress::StringToIP( const char *string, unsigned char *_ip ) {
  _ip[0] = _ip[1] = _ip[2] = _ip[3] = 0;

  char *data    = strdup( string );
  char *old_dot = data;
  char *dot;

  for( int i=0; i < 4; i++ ) {
    dot = strchr( old_dot, '.' );
    if( dot == NULL ) {
      _ip[i] = atoi( old_dot );
      break;
    } else {
      dot[0] = ' ';
      _ip[i] = atoi( old_dot );
      old_dot = dot;
    }
  }
  
  free( data );
  return _ip;
}

// IPToString( string  ):
//  Converts an IP address into a string
char * NetworkIPAddress::IPToString( char *string  ) {
  return IPToString( string, ip );
}

// IPToString( string, unsigned char * ):
//  Converts an IP address into a string
char * NetworkIPAddress::IPToString( char *string, unsigned char *_ip ) {
  return IPToString( string, _ip[0], _ip[1], _ip[2], _ip[3] );
}

// IPToString( string, a, b, c, d ):
//  Converts an IP address into a string
char * NetworkIPAddress::IPToString( char *string, unsigned char a, unsigned char b, unsigned char c, unsigned char d ) {
  sprintf( string, "%d.%d.%d.%d", a, b, c, d );
  return string;
}

// operator=():
NetworkIPAddress & NetworkIPAddress::operator=( NetworkIPAddress &other_ip ) {
  memcpy( ip, other_ip.ip, sizeof( unsigned char ) * 4 );
  return *this;
}

// operator<<;
ostream & operator<<( ostream &out, NetworkIPAddress &ip ) {
  out << (int)ip.GetIP()[0] << "." << (int)ip.GetIP()[1] << "."
      << (int)ip.GetIP()[2] << "." << (int)ip.GetIP()[3];

  return out;
}

// operator>>;
istream & operator>>( istream &in, NetworkIPAddress &ip) {
  char buffer[100];
  in >> buffer;
  ip.StringToIP( buffer );
  return in;
}

