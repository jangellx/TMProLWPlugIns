//
// bfstream_base.h
//
// This file includes the bfstream class.  The only reason it isn't named bfstream
//  is to cut down the amount of confusion as to which file to include in your
//  program.  bfstream includes functions and enums common to bifstream and
//  bofstream
//
// - ENDIAN CONVERSION -
// Included are a number of endian converstion functions.  The ReverseEndian()
//  functions reverse the endian order for a variable.  Note that int conversion
//  is NOT supported.  This is because the size of an int is platforms specific.
//  Use a short or a long instead.
//
// The ComvertEndian() functions are protected, and are used by the stream operators
//  to convert the endian when reading from/writing to a file.  The endian is changed
//  based on the WIN32 define (ie: Intel byte order) and the endian_mode.
//
// - BYTE ALIGNMENT -
// None of these classes handle byte-alignment issues within files, as that is often
//  very specific to certain parts of a file format.  You will need to do this your
//  self, if needed.
//
// - ENDIAN MODE -
// Currently, the default endian mode for files is ENDIAN_INTEL.  DO NOT ASSUME THIS!
//  be sure to set the endian for the file format you are reading/writing.
//

#ifndef BFSTREAM_BASE_HEADER
#define BFSTREAM_BASE_HEADER

#include "portable/stringtools/stringtools.h"

// Endians
enum bfstream_Endians {
  ENDIAN_MOTOROLA = 0,
  ENDIAN_INTEL,

  NUM_ENDIANS
};

// Byte Alignment
enum bfstream_Alignments {
  ALIGN_BYTE     = 0,     
  ALIGN_WORD,
  ALIGN_LONGWORD,

  NUM_ALIGNMENTS
};

// Class bfstream
class bfstream {
public:
  bfstream();
  ~bfstream();

  // Set/Get Functions
  void             SetEndian( bfstream_Endians _endian ) { endian_mode = _endian; }
  bfstream_Endians GetEndian() { return endian_mode; }

  // Endian Functions
  static inline void ReverseEndian( float  &value ) { StringTools::Reverse( (char *)(&value), 4 ); }
  static inline void ReverseEndian( double &value ) { StringTools::Reverse( (char *)(&value), 8 ); }
  static inline void ReverseEndian( long   &value ) { StringTools::Reverse( (char *)(&value), 4 ); }
  static inline void ReverseEndian( short  &value ) { StringTools::Reverse( (char *)(&value), 2 ); }
  static inline void ReverseEndian( unsigned long  &value ) { StringTools::Reverse( (char *)(&value), 4 ); }
  static inline void ReverseEndian( unsigned short &value ) { StringTools::Reverse( (char *)(&value), 2 ); }

protected:
  // Protected Endian Functions
  void ConvertEndian( float  &value );
  void ConvertEndian( double &value );
  void ConvertEndian( long   &value );
  void ConvertEndian( short  &value );
  void ConvertEndian( unsigned long  &value );
  void ConvertEndian( unsigned short &value );

  bfstream_Endians    endian_mode;               // Current endian mode
};

#endif