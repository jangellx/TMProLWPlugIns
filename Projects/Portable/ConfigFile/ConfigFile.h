//
// ConfigFile.h
//
// The ConfigFile class handles the loading and saving of ASCII configuration files.
//
//  The following types of configs are supported.  Any number of types can be in the
//   same configuration file:
//
//  - Sectioned
//    The file is broken into sections.  Each section is marked by a case-sensitive
//    keyword.  The program can search the file section by section.  This makes it
//    easy to parse user-generated files containa a number of default options
//    in different menus.  A section ends at the next keyword, or the end of file.
//  - Bracketed
//    The file contains a series of braces { and }.  Everything within those braces
//    is considered part of the block.  Blocks can have sub-blocks.  Each block can
//    be parsed as a section.  This makes it quite easy to have very human-readable 
//    configuration file consisting of a series of blocked-out regions.
//  - Value-Based
//    A keyword followed by a value.  There can be more than 1 value per keyword,
//    space delimited.  A line is stored in it's original ASCII form, and thus
//    can be retrieved as a whole or as it's individual components in whatever
//    datatype is needed (ie:  GetInt( "Key", 1 ) will return the second value that
//    matches the word Key as an int, while GetLine( "Key" ) will return a string
//    containing the entire line after the white space after the word Key.
//
//  All configuration file support comments in the form of a # and a //.  Anything
//   after the comment characters on that line is ignored.
//
//  When loading a configuration, the entire file is loaded into memory, at which
//   point the main program can do as it wishes with the data.  There are functions
//   to search the file, change section, and read in from columns or rows of the
//   data.  There are value-based access functions, allowing the program to return
//   the data held by that keyword.  
//
// When saving a configuration, there are a number of helper functions, including
//  functions to write a keyword and its data to the file, to write a section or
//  block to the file, and to write a comment out to the file.
//
// Configuration files can have optional headers.  This is strongly recommended
//  when saving a configuration, but is not required for loading one.
//

enum ConfigFileType {
  CONFIGFILE_SECTIONED = 0,
  CONFIGFILE_BRACKETED,
  CONFIGFILE_VALUE };

// class ConfigFile():
class ConfigFile {
public:
  ConfigFile( ConfigFileType config_type );
  ~ConfigFile();

  Out( const char *path );
  In(  const char *path );

  WriteNewSection( const char *name );

  WriteNewBracketed( const char *name );
  WriteCloseBracket();

  WriteNewValue( const char *value );

protected:

};

