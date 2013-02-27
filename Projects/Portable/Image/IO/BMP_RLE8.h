//
// BMP_RLE8.h
//
// This contains the code to compress an 8-bit BMP.  This is my 4th attempt,
//  and it actually works this time.  These classes are used by BMP::Save8Bit
//  when RLE8 compression is used (as found in Image/IO/BMP.cpp)
//
// There are two classes:  BMP_RLE8_Line, which contains a line worth of
//  BMP_RLE8_Code classes.  The BMP_RLE8_Code class contains the number
//  of pixels and the encoding mode for a run, plus the file output
//  functions.
//

#ifndef HEADER_BMP_RLE8
#define HEADER_BMP_RLE8

#include "portable\dynarray\dynarray.h"
#include "portable\bfstream\bfstream.h"

#include "ImageData.h"

// Class Declarations
class BMP_RLE8_Line;
class BMP_RLE8_Code;

//
// Class BMP_RLE8_Code
//
class BMP_RLE8_Code {
public:
  // Enums
  enum RLE8_Modes { RLE8_ENCODED, RLE8_ABSOLUTE };

  // Constructor/Destructor
  BMP_RLE8_Code( RLE8_Modes _mode = RLE8_ENCODED, int _num_pixels = 0 )
    : num_pixels(_num_pixels), mode(_mode) {;}
  ~BMP_RLE8_Code() {;}

  // File I/O
  bool Write( bofstream &out, IMG_BYTE *line, int offset );
  bool OutputAsAscii( ostream &out, IMG_BYTE *line, int offset );

  // Checks
  inline bool       IsValid();

  // Accessors
  inline void       SetMode( RLE8_Modes _mode ) { mode = _mode; }
  inline RLE8_Modes GetMode() { return mode; }

  inline void          SetNumPixels( unsigned char _np ) { num_pixels = _np; }
  inline unsigned char GetNumPixels() { return num_pixels; }

protected:
  unsigned char  num_pixels;
  RLE8_Modes     mode;
};

// IsValid():
inline bool BMP_RLE8_Code::IsValid() {
  if( mode == RLE8_ENCODED )
    return true;

  if( (mode == RLE8_ABSOLUTE) && (GetNumPixels() > 2) )
    return true;

  return false;
}

//
// Class BMP_RLE8_Line
//
class BMP_RLE8_Line {
public:
  ~BMP_RLE8_Line() { codes.Flush(); }

  inline int  CountPixels();                           // Counts the number of pixels in the row
         int  MergeAsAbsolute( int c1, int c2);        // Merges two codes together.  The first code must be in absolute mode!
         void Optimize();                              // Finds occurances of Encoded codes of less than 3 pixels between two Absolute codes, and merges them to save space

  // File I/O
  bool Write( bofstream &out, IMG_BYTE *line );
  bool OutputAsAscii( ostream &out, IMG_BYTE *line, int line_num = -1 );
 
  // Encoded Data
  DynArray< BMP_RLE8_Code * >codes;

protected:
  
};

// CountPixels():
inline int BMP_RLE8_Line::CountPixels() {
  int count = 0;

  for( unsigned int i=0; i < codes.NumElements(); i++ )
    count += codes[i]->GetNumPixels();

  return count;
}

#endif