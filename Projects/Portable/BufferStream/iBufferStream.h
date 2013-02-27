//
// iBufferStream.h
//
// iBufferStream allows for easy reading from a binary buffer
//  in memory.  The class requires a pointer to a buffer to
//  operate on; however, the buffer is _NOT_ freed by the class
//  on destruction.
//
// iBuffserStream inherits from bfstream, thus allowing it
//  to seamlessly handle endian conversions, even though it
//  performs no file access.
//
// This class uses longs, and as such supports up to 4 gigabyte
//  buffers.
//

#include <portable/bfstream/bfstream_base.h>

class iBufferStream : public bfstream {
public:
  // Local Enums
  enum ibuf_offset { beg = -1, cur = -2, end = -3};                // Offset for seek operations

  // Constructors/Destructor
  iBufferStream();
  iBufferStream( const char *_buffer, long _len );
  ~iBufferStream();

  // Open/Close
  bool Open( const char *_buffer, long _len );                     // A new buffer to "open" for reading
  void Close();                                                    // "Close" the current buffer (Reset the class)

  // Accessors
  inline const char * GetBuffer() { return buffer; }               // Get the buffer the sream is reading from.  To set, use Open()

  inline       long   GetBufferLength() { return len; }            // Get the length of the buffer.
  inline       bool   SetBufferLength( long _len) { if( len < pos ) return false;  len = _len; return true;}  // Set the length of the buffer.  Returns false if the length is shorter than the current position

  inline const char * GetBufferAtPos() { return &(buffer[ pos]); } // Ger a pointer to the buffer at the current position
               bool   Seek( long _pos, ibuf_offset offset = cur ); // Set the current position in the buffer relative to offset.  Returns false if the location is beyond the scope of the buffer
  inline       long   Tell() { return pos; }                       // Get the current positino in the buffer 

  inline       bool   SkipChar(   bool forward = true ) { return Seek( (forward ? (int)sizeof(char  ) : -(int)sizeof(char  )) ); }          // Skips forward or backward one char   (1 byte)
  inline       bool   SkipShort(  bool forward = true ) { return Seek( (forward ? (int)sizeof(short ) : -(int)sizeof(short )) ); }          // Skips forward or backward one short  (2 bytes)
  inline       bool   SkipLong(   bool forward = true ) { return Seek( (forward ? (int)sizeof(long  ) : -(int)sizeof(long  )) ); }          // Skips forward or backward one long   (4 bytes)
  inline       bool   SkipFloat(  bool forward = true ) { return Seek( (forward ? (int)sizeof(float ) : -(int)sizeof(float )) ); }          // Skips forward or backward one float  (4 bytes)
  inline       bool   SkipDouble( bool forward = true ) { return Seek( (forward ? (int)sizeof(char  ) : -(int)sizeof(char  )) ); }          // Skips forward or backward one double (8 bytes)
  inline       bool   SkipString()                      { return Seek(  strlen( &(buffer[ Tell() ]) ) + 1); }                               // Skips forward or backward one string length (strlen() + 1 bytes)

  // Errors
  void        ClearErrors() { is_error = false; }  // Clears the error state out

  inline bool IsError() { return is_error; }       // Returns true if en error has occured
  inline bool IsEOB()   { return (pos >= len); }   // Returns true if the end of buffer has been reached

  inline bool operator!() { return IsError(); }    // Same as IsError()

  // Read Functions
                  char * Get(          char * _buf, int _len ); // Reads _len characters from the buffer into _buf
  inline unsigned char * Get( unsigned char * _buf, int _len ) { return (unsigned char *)Get( (char *)_buf, _len ); } // Reads _len characters from the buffer into _buf
                  bool   Get(          char & byte );           // Reads a single character from the buffer into byte
                  char * GetString( char * string, int _len );  // Reads a null-terminated string from a binary file (meaning it stops reading when it finds a null)

  inline          char * Read(          char *_buf, int _len ) { return Get( _buf, _len); }  // Synonym for Get()
  inline unsigned char * Read( unsigned char *_buf, int _len ) { return Get( _buf, _len); }  // Synonym for Get()

  // Extration Operators
          iBufferStream & operator>>( char &var );
          iBufferStream & operator>>( unsigned char &var );
          iBufferStream & operator>>( signed char &var );
          iBufferStream & operator>>( char * var );
  inline  iBufferStream & operator>>( unsigned char * var ) { return (*this)>>((char *)var); }
  inline  iBufferStream & operator>>( signed char * var )   { return (*this)>>((char *)var); }
          iBufferStream & operator>>( short & var );
          iBufferStream & operator>>( unsigned short & var );
          iBufferStream & operator>>( int & var );            // Illegal; use short or long
          iBufferStream & operator>>( unsigned int & var );   // Illegal; use short or long
          iBufferStream & operator>>( long & var );
          iBufferStream & operator>>( unsigned long & var );
          iBufferStream & operator>>( float & var );
          iBufferStream & operator>>( double & var );
          iBufferStream & operator>>( long double & var );

protected:
  const char *buffer;     // Pointer to the buffer
        long  len;        // Size of the buffer
        long  pos;        // Current location in the buffer

  bool        is_error;   // True if there's an error (probably the buffer is NULL or the len is 0, or a Seek() went past the end of the buffer)
};

