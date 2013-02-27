//
// OpenGLTools-Fonts.h
//

#include <string.h>

#include "OpenGLTools-Fonts.h"

// Constructor
GLTBitmapFont::GLTBitmapFont( const char *_fontname,
                              int _height, int _weight, bool _italics ) {

  // Set some default values
  fontname = NULL;
  base     = 0;
  weight   = _weight;
  height   = _height;
  italics  = _italics;

  SetFontName( fontname );

  // Set chars to default range of acceptable values (space through end of alphanumerics)
  low_char  = 32;
  num_chars = 96;

  win_font  = NULL;

#ifdef WIN32
  hdc = CreateCompatibleDC( NULL );
#endif
}

// Destructor
GLTBitmapFont::~GLTBitmapFont() {
  Flush();
#ifdef WIN32
  if( hdc != NULL )
    DeleteDC( hdc );
#endif
}

// Generate():
//  Actually generates bitmaps of the font
GLuint GLTBitmapFont::Generate() {
  if( fontname == NULL )
    return 0;

  Flush();

#ifdef WIN32
  // Generate the font bitmaps (Windows Specific)
  win_font = CreateFont( height, 0, 0, 0, weight, italics, false, false,
                         ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
                         DRAFT_QUALITY, DEFAULT_PITCH, fontname );

  SelectObject( hdc, win_font );
  base = glGenLists( 96 );
  wglUseFontBitmaps( hdc, low_char, num_chars, base );
#endif

  return base;
}


// Flush():
//  Flushes the bitmaps of the font.
void GLTBitmapFont::Flush() {
  if( base == 0 )
    return;

  glDeleteLists( base, num_chars );
  base = 0;

#ifdef WIN32
  DeleteObject( win_font );
  win_font = NULL;
#endif
}

// Write( const char * ):
//  Writes a string of text to the OpenGL viewport
bool GLTBitmapFont::Write( const char *string ) {
  if( base == 0 )
    return false;

  if( string == NULL )
    return false;

  glPushAttrib( GL_LIST_BIT );
  glListBase( base - low_char );
  glCallLists( strlen( string ), GL_UNSIGNED_BYTE, string );
  glPopAttrib();

  return true;
}

// Write( const char *, float, float ):
//  Writes a string of text to the OpenGL viewport at the coordinates
//   specifed.
bool GLTBitmapFont::Write( const char *string, int x, int y ) {
  glRasterPos2i( x, y );
  return Write( string );
}

// TextWidth():
//  Returns the width of the string provided
int GLTBitmapFont::TextWidth( const char *string, int length ) {
#ifdef WIN32
  SIZE size;
  SelectObject( hdc, win_font );
  GetTextExtentPoint32( hdc, string, length, &size );
  return size.cx;
#endif
}

// SetFont():
bool GLTBitmapFont::SetFontName( const char *_fontname ) {
  if( base != 0 )
    return false;

  if( fontname != NULL )
    free( fontname );

  if( _fontname != NULL )
    fontname = strdup( _fontname );

  return true;
}
