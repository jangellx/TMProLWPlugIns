//
// BMP_RLE8.cpp
//

#include "BMP_RLE8.h"

//
// BMP_RLE8_Code Function Definitions
//

// Write():
//  Writes out a code run to the specified output file.  The line argument
//   is the line of IMG_BYTE's the image we are outputing.  Offset is
//   where in the line this code is.
bool BMP_RLE8_Code::Write( bofstream &out, IMG_BYTE *line, int offset ) {
  if( mode == RLE8_ENCODED ) {
    out << GetNumPixels() << line[offset];
  } else {
    if( IsValid() ) {
      out << (char)0x00 << GetNumPixels();
      for( int i=0; i < GetNumPixels(); i++ )
        out << line[ offset + i ];
      
      if( GetNumPixels() % 2 > 0 )
        out << (char)0x00;
    } else {
      for( int i=0; i < GetNumPixels(); i++ )
        out << (char)0x01 << line[ offset + i ];
    }
  }

  return true;
}

// OutputAsAscii():
//  Outputs the code as ASCII text to the output file specified. The
//   line argument is a line of IMG_BYTE's from the image we are saving.
//   offset is where in the lin this code is.
bool BMP_RLE8_Code::OutputAsAscii( ostream &out, IMG_BYTE *line, int offset ) {
  if( mode == RLE8_ENCODED ) {
    out << "   (" << offset << ") Encoded Mode,  " << (int)GetNumPixels() << " pixels of index " << (int)line[ offset ] << endl;
  } else {
    if( GetNumPixels() < 3 ) {
      out << "   (" << offset << ") Aboslute Mode, " << (int)GetNumPixels() << " pixels, converted to " << (int)GetNumPixels() << " 1-pixel Encoded Mode outputs: ";
      for( int i=0; i < GetNumPixels(); i++ )
        out << " " << (int)line[ offset + i ];
      out << endl;
    } else {
      out << "   (" << offset << ") Aboslute Mode, " << (int)GetNumPixels() << " pixels, indices";
      for( int i=0; i < GetNumPixels(); i++ )
        out << " " << (int)line[ offset + i ];
      out << endl;
      
      if( GetNumPixels() % 2 > 0 )
        out << "      Adding 1 pad byte for Absolute Mode word alignment" << endl;
    }
  }

  return true;
}


//
// BMP_RLE8_Line Function Definitions
//

// MergeAsAbsolute():
//  Combines two codes together, deleting the second code.  The first code will be changed
//   to Absolute mode, and will contain both codes pixels.
inline int BMP_RLE8_Line::MergeAsAbsolute( int c1, int c2 ) {
  codes[c1]->SetMode( BMP_RLE8_Code::RLE8_ABSOLUTE );                                // Set mode to Absolute
  codes[c1]->SetNumPixels( codes[c1]->GetNumPixels() + codes[c2]->GetNumPixels() );  // Add the pixel counts

  BMP_RLE8_Code * kill = codes[c2];                                                  // Remove the redundant code
  codes.Remove( c2 );
  delete kill;

  return codes[c1]->GetNumPixels();                                                  // Returnt he new pixel count
}

// Optimize():
//  Optimizes the line by finding an Encoded code that is less than 3 pixels
//   in size between two Absolute codes and merges them together.  This requires
//   2 bytes less space per occurance.
void BMP_RLE8_Line::Optimize() {
  for( int i=0; i < (int)codes.NumElements() - 2; i++ ) {
    if( codes[i]->GetMode() == BMP_RLE8_Code::RLE8_ABSOLUTE ) {
      if( codes[i+1]->GetMode() == BMP_RLE8_Code::RLE8_ENCODED ) {
        if( codes[i+2]->GetMode() == BMP_RLE8_Code::RLE8_ABSOLUTE ) {
          if( codes[i+1]->GetNumPixels() < 3 ) {
            // Merge the three codes together
            MergeAsAbsolute( i, i+1 );
            MergeAsAbsolute( i, i+1 );           // One code was deleted, so i+1 here is equal to i+2 above

            // Do this code again so we can compare against the next code
            i--;
          }
        }
      }
    }
  }
}

// Write():
//  Writes out the entire line to the file stream specified.  line is a line
//   from the image we are saving that pertains to this data.  If you really
//   wanted to, you could compress the image in RAM first and make line a
//   protected memeber of BMP_REL8_Line, but that could require a lot of RAM
//   and really isn't nessesary.
bool BMP_RLE8_Line::Write( bofstream &out, IMG_BYTE *line  ) {
  int offset = 0;
  for( unsigned int i=0; i < codes.NumElements(); i++ ) {
    codes[i]->Write( out, line, offset );
    offset += codes[i]->GetNumPixels();
  }

  // Output an End Of Row
  out << (char)0x00 << (char)0x00;

  return true;
}

// OutputAsAscii():
//  Outputs the line as ASCII text to the output file specified.  The
//   line argument is a line of IMG_BYTE's from the image we are saving.
//   line_num is the number of this line.
bool BMP_RLE8_Line::OutputAsAscii( ostream &out, IMG_BYTE *line, int line_num ) {
  out << " Outputting Line " << line_num << ";  " << codes.NumElements() << " codes, " << CountPixels() << " pixels" << endl;

  int offset = 0;
  for( unsigned int i=0; i < codes.NumElements(); i++ ) {
    codes[i]->OutputAsAscii( out, line, offset );
    offset += codes[i]->GetNumPixels();
  }

  return true;
}
