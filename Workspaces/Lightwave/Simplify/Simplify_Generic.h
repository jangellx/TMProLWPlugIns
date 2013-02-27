//
// Simplify_Generic.h
//
// This contains the class definition for class SimplifyGlobal,
//  which is used to store global variables used by the Simplify
//  Generic plug-ins
//

class SimplifyGlobal;

class SimplifyGlobal {
public:
  SimplifyGlobal() : preview(true), render(true), lod(true), autoload(true) { ; }

public:
  // Variables
  bool preview;
  bool render;
  bool lod;
  bool autoload;
};