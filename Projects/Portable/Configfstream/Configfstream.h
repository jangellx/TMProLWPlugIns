//
// Configfstream.h
//
// The Configifstream class is a Configuration File Stream.  It is
//  designed to parse user-created configuration files, but can
//  also parse config files written by other programs.  For the
//  most part, the class functions as a standard ifstream class.
//
// The main difference between this class and a normal stream class
//  is that operations are performed on a line-by-line basis.  This
//  makes the comment-code easier to deal with and makes it easy to
//  keep track of the number of lines in the file.  The stream is
//  based on the pfstream classes, so it will work with any EOL
//  type.
//
// There are a few limitations of this system:
// - Each line is read into an 8K buffer.  Lines more than 8K
//    will fail, but this should be very rare (average lines are
//    less than 0.25K )
// - You must call the ReadNextLine() function to read in a new
//    line from the config file before using the stream operators.
//    This automatically skips blank lines or lines that contain
//    only comments, returning the line number that we are now on.
// - seekg(), while supported, can result in very, very strange
//    behavior, and as such is not recommended.  Instead, use the
//    function SeekToLine().
//
// Be warned that while most of the stream operators and getline()
//  are overloaded, many of the standard functions, such as get(),
//  are not.
//
// Extra features include:
// - Comments:
//   Comments are defined by the following characters:
//    - C++-style comment of "//"; everything beyond the // is
//      considered a comment and ignored
//    - Assembly/Script comment of '#'; everything beyond the #
//      is considered a comment and ignored.
//    - MS-DOS-style comment of ';'; everything beyond the ;
//      is considered a comment and ignored.
//   You can also pass in an array of char strings to use in place
//   of the standard comment strings outlined above.
//

// Includes
#include <portable/dynarray/dynarray.h>
#include <portable/stringtools/stringtools.h>
#include <portable/pfstream/pfstream.h>

// Defines
#define CONFIGFSTREAM_LINE_BUFFER_SIZE  8192

// Prototypes
class Configifstream;

// Default Comment Strings
static const char *configifstream_comment_strings[] = {
  "//",
  "#",
  ";",
  NULL };

// Class Configifstream
class Configifstream : public pifstream {
public:
  // Constructors/Destructor
  Configifstream();
  Configifstream( const char *_name, int _ios = ios::in,
             int _filebuf = filebuf::openprot );
  Configifstream( filedesc _filedesc );
  Configifstream( filedesc _fildesc, char *name, int _value );
  ~Configifstream();

  void C_Init(void);

  // New Functions
  long    ReadNextLine();                              // Reads in a new line; returns the line number
  bool    SeekToLine( long line_number );              // Seek to the line nubmer specified

  // Overloads
  int     setmode( int = filebuf::text );              // Special overloaded setmode that can ONLY set files to ASCII mode

          Configifstream & operator>>( char &var );
          Configifstream & operator>>( unsigned char &var );
          Configifstream & operator>>( signed char &var );
          Configifstream & operator>>( char * var );
  inline  Configifstream & operator>>( unsigned char * var ) { return (*this)>>((char *)var); }
  inline  Configifstream & operator>>( signed char * var )   { return (*this)>>((char *)var); }
          Configifstream & operator>>( short & var );
          Configifstream & operator>>( unsigned short & var );
          Configifstream & operator>>( int & var );
          Configifstream & operator>>( unsigned int & var );
          Configifstream & operator>>( long & var );
          Configifstream & operator>>( unsigned long & var );
          Configifstream & operator>>( float & var );
          Configifstream & operator>>( double & var );
          Configifstream & operator>>( long double & var );

          Configifstream & getline( char* string, int count, char delim = '\n' );
  inline  Configifstream & getline( unsigned char* string, int count, char delim = '\n' ) { return this->getline( (char *)string, count, delim ); }
  inline  Configifstream & getline( signed char* string, int count, char delim = '\n' ) { return this->getline( (char *)string, count, delim ); }

  // Accessors
  void          SetCommentStrings( const char **strings ) { comment_strings = strings; }
  const char ** GetCommentStrings() { return comment_strings; }

  long          GetLineNumber() { return current_line_number; }
  long          GetNextLineNumber() { return next_line_number; }
  long          GetNumLines() { return (long) line_positions.NumElements(); }

  // Use these to parse the line buffer yourself
  const char *  GetLineBuffer() { return line_buffer; }           // Returns a pointer to the line buffer.  Should be considered Read-Only!
  const char *  GetCarrot() { return carrot; }                    // Returns a pointer to the current location within the line buffer.  Should be considered Read-Only!
  inline void   SkipWhiteSpace() { carrot = StringTools::SkipWhiteSpace( carrot ); }  // Skips leading white space in the buffer

  bool          eob() { return end_of_buffer; }                   // Returns true if the end of the buffer has been reached

protected:
  bool KillComment( char *var );                       // Processes a string and removes the commented part, if any.  Returns false if the string is only a comment and/or blank spaces/tabs
  void TestForEOB() { if( carrot[0] == '\0' ) end_of_buffer = true; }  // Tests for the end of the buffer and sets end_of_buffer appropriately

  const char ** comment_strings;                       // List of strings that should be considered comment markers
  long          current_line_number;                   // The current line number we're reading in
  long          next_line_number;                      // The number of the next line that will be read in by ReadNextLine()

  char          line_buffer[8192];                     // Buffer to hold the current line
  char *        carrot;                                // Where we currently are in the line buffer
  bool          end_of_buffer;                         // True when the end of the buffer has been reached

  DynArray< streampos > line_positions;                // Array containing the file positions of each line in the file
};

