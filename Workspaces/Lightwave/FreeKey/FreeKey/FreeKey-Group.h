//
// FreeKeyGroup.h
//
// This file contains the FreeKeyGroup class, which contains a list of items and
//  the settings to be applied to them.
//

class FreeKeyGroup;

class FreeKeyGroup {
public:
  // enums
  enum RemoveMode {
	  remove_none     = 0,
    remove_outside,
		remove_inside,
    remove_before,
    remove_after };

  // Constructor/Destructor
  FreeKeyGroup() : delta(0), use_delta(false), step(0), use_step(false), invert_step(false),
                   remove_mode(remove_none), direction(0), low(0), high(32767),
                   use_scale_keys(false), scale_keys(1.0f), name(NULL) { ; }
  ~FreeKeyGroup() { SetName( NULL ); }

  // Setup/Validation
  int            TestRange();             // Tests to make sure the range is valid

  // List Accessors
  DynArray< LWItem * > & GetItemList() { return items; }

  // Range Accessors
  inline int            GetLow()  { return low;  }
  inline int            GetHigh() { return high; }
  inline void           SetLow( int l )  { low  = l; }
  inline void           SetHigh( int h ) { high = h; }
  inline void           SetRange( int _low, int _high ) {  low = _low;  high = _high; }

  // Mode Accessors
  inline RemoveMode     GetRemoveMode() { return remove_mode; }
  inline void           SetRemoveMode( RemoveMode state ) { remove_mode = state; }

  inline bool           GetUseStep() { return use_step; }
  inline void           SetUseStep( bool state ) { use_step = state; }

	inline int            GetStep() { return step; }
	inline void           SetStep( int _step ) { step = _step; }

  inline bool           GetInvertStep() { return invert_step; }
	inline void           SetInvertStep( bool state ) { invert_step = state; }

  inline void           SetDirection( int _direction ) { direction = _direction; }
	inline int            GetDirection() { return direction; }

  // Time Accessors
  inline int            GetDelta() { return delta; }
  inline void           SetDelta( int _delta ) { delta = _delta; }
	inline bool           GetUseDelta() { return use_delta; }
  inline void           SetUseDelta( bool state ) { use_delta = state; }

  inline float          GetScaleKeys() { return scale_keys; }
  inline void           SetScaleKeys( float _scale_keys ) { scale_keys = _scale_keys; }
	inline bool           GetUseScaleKeys() { return use_scale_keys; }
  inline void           SetUseScaleKeys( bool state ) { use_scale_keys = state; }

  // Name Accessors
  inline const char *   GetName() { return name; }
               void     SetName( const char *new_name );

protected:
  int   low, high;                         // Low and High limits of the range

  enum  RemoveMode remove_mode;            // Remove Inside, Outside or not at all

  bool  use_step;                          // Toggle Step
	bool  invert_step;                       // Toggle Invert Step

  int   step;                              // Step 
  int   delta;                             // Amount of shift to apply to item keys in time
  int   direction;                         // Direction of the keys within the range (forward or reverse)
  float scale_keys;                        // Scale Keys Multiplier

  bool  use_delta;                         // Toggles time delta
  bool  use_scale_keys;                    // Toggles scale keys

  char * name;                             // Group Name

  DynArray< LWItem * > items;              // Items belonging to this group.  This is for interface use only; it is NOT used during processing
};
