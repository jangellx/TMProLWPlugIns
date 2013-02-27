
// Test Project: pifstream_test
//
// Simple test program to make sure pifstream works OK

#include <portable/pfstream/pfstream.h>
#include "stdlib.h"

// Test Bitfield, allowing multiple tests.  OR them together into the test variable in main()
enum TestModes {
  TestEOLManager = 0x000001,
  TestEOLLoader  = 0x000002
};

// Test Variables
int test = TestEOLLoader|TestEOLManager;

// Prototypes
void outint( const char *buffer );
void MakeTest( char *filename, int eolmode, char *eolname );

// Main()
void main( int argc, char **argv) {
  if( argc < 3 ) {
    cerr << " You must provide an input filename and EOL type!" << endl;
    exit(20);
  }

  // Convert the EOL mode
  int EOL;
  if( stricmp( argv[2], "LF" ) == 0)
    EOL = EOLManager::LF;
  else if( stricmp( argv[2], "CR" ) == 0 )
    EOL = EOLManager::CR;
  else if( stricmp( argv[2], "CRLF" ) == 0)
    EOL = EOLManager::CRLF;
  else {
    cerr << "  ERROR:  Illegal EOL value.  Must be LF, CR or CRLF" << endl;
    exit(20);
  }

  // Create the Test Files
  MakeTest( argv[1], EOL, argv[2] );

  // Open a pifstream
  pifstream in( argv[1] );
  if( !in ) {
    cerr << " Unable to open file " << argv[1] << endl;
    exit(20);
  } else {
    cerr << " Successfully opened file " << argv[1] << endl;
  }

  if( test & TestEOLManager ) {
    // Test the GetEOL/SetEOL functions
    cerr << " GetEOL/SetEOL Tests " << endl;
    cerr << "  Default EOL:      " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0] << endl;
    in.SetEOLType( EOLManager::CR );
    cerr << "  CR EOL:           " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0] << endl;
    in.SetEOLType( EOLManager::CRLF );
    cerr << "  CRLF EOL:         " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0]
                                                             << (int)in.EOLType2String(in.GetEOLType())[1] << endl;
    in.SetEOLType( EOLManager::LF );
    cerr << "  LF EOL:           " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0] << endl; 

    in.SetEOLType( -2 );
    cerr << "  Illegal EOL (-1): " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0] << endl;
    in.SetEOLType( 4 );
    cerr << "  Illegal EOL (4):  " << in.GetEOLType() << "/" << (int)in.EOLType2String(in.GetEOLType())[0] << endl;

    // Test the EOLType2String/EOLString2Type functions
    cerr << endl;
    cerr << " EOLType2String() Tests " << endl;
    cerr << "  EOLType2String(LF)   " << (int)in.EOLType2String( EOLManager::LF )[0]   << endl;
    cerr << "  EOLType2String(CR)   " << (int)in.EOLType2String( EOLManager::CR )[0]   << endl;
    cerr << "  EOLType2String(CRLF) " << (int)in.EOLType2String( EOLManager::CRLF )[0] 
                                      << (int)in.EOLType2String( EOLManager::CRLF )[1] << endl;

    // Test GuessEOLType()
    cerr << endl;
    in.GuessEOLType();
    int          guess = in.GetEOLType();
    const char * gEOL  = in.EOLType2String( guess );

    cerr << " Guessed EOL Type:  " << (int)gEOL[0];
    if( gEOL[1] != '\0' )
      cerr << (int)gEOL[1];
    cerr << endl;
  }

  // Test the EOL Loader Functions in pifstream
  if( test & TestEOLLoader ) {
    char buffer[1024];
    int count = 0;
    cerr << endl;

    in.GuessEOLType();
    while( in ) {
      in.getline( buffer, 1024, in.GetEOLType() );
      cerr << " -- Line   "   << ++count << endl;
      cerr << "    ASCII: \"" << buffer << "\"" << endl;
      cerr << "    Dec:   ";
      outint( buffer );
      cerr << endl;
    }
  }

  in.close();
}

// outint():  Outputs the buffer as a string of ints
void outint( const char *buffer ) {
  unsigned int length = strlen(buffer);
  for( unsigned int i=0; i < length; i++ )
    cerr << " " << (int) buffer[i];
  cerr << endl;
}

// MakeTest():  Craetes a test file to try loading
void MakeTest( char *filename, int eolmode, char * eolname ) {
  pofstream out( filename );
  out.SetEOLType( eolmode );

  if( out ) {
    out << "Test File:  " << eolname << out.endl();
    out << "Testing to see if " << eolname << " mode works correctly" << out.endl();
    out << "Here's hopin'...";  // No EOL here, for testing
    cerr << " Succesfully created output file " << filename << endl;
  } else {
    cerr << " Error opening output file!" << endl;
  }
}
