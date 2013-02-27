//
// OpenGLTools-Fonts.h
//
// This file contains the GLTBitmapFont class, which can be used initialize
//  a font for use by OpenGL, as well as called to display the font.
//

#ifndef OPENGLTOOLS_FONTS_HEADER
#define OPENGLTOOLS_FONTS_HEADER

#ifdef WIN32
#include "windows.h"
#include "wingdi.h"
#endif

#include <gl/gl.h>

class GLTBitmapFont;

class GLTBitmapFont {
public:
  GLTBitmapFont( const char *_fontname = NULL,
                 int _height = 12, int _weight = 0,
                 bool _italics = false );

  ~GLTBitmapFont();

  // Initializers
  GLuint Generate();
  void   Flush();

  // Drawing Functions
  bool Write( const char *string );
  bool Write( const char *string, int x, int y );

  int  TextWidth( const char *string, int length );

  // Accessors
  inline       bool   IsFontGenerated() { return (base == 0) ? false : true; }

               bool   SetFontName( const char *_fontname );
  inline const char * GetFontName() { return fontname; }

  inline       bool   SetWeight( int _weight ) { if( base != 0 ) return false;
                                                 weight = _weight; return true; }
  inline        int   GetWeight() { return weight; }


  inline       bool   SetHeight( int _height ) { if( base != 0 ) return false;
                                                 height = _height; return true; }
  inline        int   GetHeight() { return height; }


  inline       bool   SetItalics( bool state ) { if( base != 0 ) return false;
                                                 italics = state; return true; }
  inline       bool   GetItalics() { return italics; }


  inline       bool   SetCharRange( int _low_char, int _num_chars ) { if( base != 0 ) return false;
                                                                     low_char = _low_char; num_chars = _num_chars; return true; }

  inline       bool   SetLowChar( int _low_char ) { if( base != 0 ) return false;
                                                    low_char = _low_char; return true; }
  inline       int    GetLowChar() { return low_char; }


  inline       bool   SetNumChars( int _num_chars ) { if( base != 0 ) return false;
                                                      num_chars = _num_chars; return true; }
  inline       int    GetNumChars() { return num_chars; }


  inline       GLint  GetBase() { return base; }

protected:
  char       *fontname;                 // Font Name
  int         weight;                   // Font Weight (bold, etc.)
  int         height;                   // Font Height
  bool        italics;                  // Toggle for Italics

  int         low_char;                 // Low character index to convert
  int         num_chars;                // Total number of character indices to convert

  GLuint      base;                     // Base of the font bitmaps

#ifdef WIN32
  HFONT       win_font;                 // Windows Font Data
  HDC         hdc;                      // Device Context
#endif
};

#endif