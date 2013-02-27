//
// FreezerHierarchy.h
//

#ifndef FREEZERHIERARCHY_HEADER
#define FREEZERHIERARCHY_HEADER
#include "../Batarang/BatarangHierarchy.h"

// Class Declarations
class FreezerHierarchy;

// Class FreezerHierarchy
//  Adds Has PSM, Birth Frame and Movement Frame functionality to
//   BatarangHierarchy.
class FreezerHierarchy : public BatarangHierarchy {
public:
  // Constructor
  FreezerHierarchy( const char *name = NULL,
                   int new_type = BH_ITEM_OBJECT,
                   int new_index = 0,
                   int new_parent_index = 0,
                   BatarangHierarchy *new_parent = NULL )
    : BatarangHierarchy( name, new_type, new_index, new_parent_index, new_parent ),
      birth_frame(-1), movement_frame(0), has_movement(false), has_psm(false) { ; }

  // Accessors
  // Birth Frame
  inline int GetBirthFrame() { return birth_frame; }
  void SetBirthFrame( int new_bf ) {
    birth_frame = new_bf;
    if( birth_frame < 0 )
      birth_frame = 0;
  }

  inline void SetNoBirthFrame() { birth_frame = -1; }

  // Movement Frame
  // Note:  The MovementFrame should already have the shift_offset
  //        subtracted from it.
  inline int GetMovementFrame() { return movement_frame; }
  void SetMovementFrame( int new_mf ) {
    movement_frame = new_mf;
    if( movement_frame < 0 ) {
      SetNoMovementFrame();
    } else {
      has_movement = true;
    }
  }

  inline void SetNoMovementFrame() { movement_frame = -1; has_movement = false;}
  inline bool HasMovement()        { return has_movement; }

  // Has PSM
  inline bool HasPSM() { return has_psm; }
  void HasPSM( bool new_state ) { has_psm = new_state; }

protected:
  // Protected Memebers (variables)
  int  birth_frame;
  int  movement_frame;
  bool has_psm;
  bool has_movement;
};

#endif