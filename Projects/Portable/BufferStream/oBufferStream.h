//
// iBufferStream.h
//
// iBufferStream allows for easy writing to a binary buffer
//  in memory.  The class requires a pointer to a buffer to
//  operate on; however, the buffer is _NOT_ freed by the class
//  on destruction.
//
// The buffer is not scaled to fit the data by the class.  You'll
//  have to make a buffer big enough for the operation you want to
//  perform.
//
// oBuffserStream inherits from bfstream, thus allowing it
//  to seamlessly handle endian conversions, even though it
//  performs no file access.
//
// This class uses longs, and as such supports up to 4 gigabyte
//  buffers.
//

#include <portable/bfstream/bfstream_base.h>

class oBufferStream : public bfstream {
public:
  // Local Enums
  enum obuf_offset { beg = -1, cur = -2, end = -3};                // Offset for seek operations

  // Constructors/Destructor
  oBufferStream();
  oBufferStream( char *_buffer, long _len );
  ~oBufferStream();

  // Open/Close
  bool Open( char *_buffer, long _len );                           // A new buffer to "open" for reading
  void Close();                                                    // "Close" the current buffer (Reset the class)

  // Accessors
  inline       char * GetBuffer() { return buffer; }               // Get the buffer the sream is reading from.  To set, use Open()
  inline       char * GetBufferAtPos() { return &(buffer[ pos]); } // Ger a pointer to the buffer at the current position
  inline       long   GetBufferLength() { return len; }            // Get the length of the buffer.  To set the buffer length use

  // Seek Functions
               bool   Seek( long _pos, obuf_offset offset = cur ); // Set the current position in the buffer relative to offset.  Returns false if the location is beyond the scope of the buffer
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
         bool Put( const          char * _buf, int _len );      // Writes _len characters to the buffer from _buf
  inline bool Put( const unsigned char * _buf, int _len ) { return Put( (const char *)_buf, _len ); } // Writes _len characters to the buffer from _buf
         bool Put(                char byte );                  // Writes a single character to the buffer from byte
         bool PutString( const char * string );                 // Writes a null-terminated string to a buffer

  inline bool Write( const          char *_buf, int _len ) { return Put( _buf, _len); }  // Synonym for Put()
  inline bool Write( const unsigned char *_buf, int _len ) { return Put( _buf, _len); }  // Synonym for Put()

  // Insertion Operators
  oBufferStream & operator<<( char var );
  oBufferStream & operator<<( unsigned char var );
  oBufferStream & operator<<( signed char var );

  oBufferStream & operator<<( const char * var );
  oBufferStream & operator<<( const unsigned char * var );

  oBufferStream & operator<<( short var );
  oBufferStream & operator<<( unsigned short var );
  oBufferStream & operator<<( int var );
  oBufferStream & operator<<( unsigned int var );
  oBufferStream & operator<<( long var );
  oBufferStream & operator<<( unsigned long var );
  oBufferStream & operator<<( float var );
  oBufferStream & operator<<( double var );
  oBufferStream & operator<<( long double var );

protected:
  char *buffer;     // Pointer to the buffer
  long  len;        // Size of the buffer
  long  pos;        // Current location in the buffer

  bool        is_error;   // True if there's an error (probably the buffer is NULL or the len is 0, or a Seek() went past the end of the buffer)
};

