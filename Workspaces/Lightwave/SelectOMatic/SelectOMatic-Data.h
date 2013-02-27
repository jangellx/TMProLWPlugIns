//
// SelectOMatic-Data.h
//

#include <portable/dynarray/dynarray.h>

#include "SelectOMatic-Item.h"

#include  <lwserver.h>

class SelectOMatic_Data {
public:
  SelectOMatic_Data( int _index );
  ~SelectOMatic_Data();

  // File I/O
  char * Load( const LWLoadState *loader );
  void   Save( const LWSaveState *saver  );

  // Management
  inline void                SetActiveGroup( SelectOMatic_Item * _active ) { active_group = _active; }
  inline SelectOMatic_Item * GetActiveGroup() { return active_group; }

  inline void                SetActiveItem( SelectOMatic_Item * _active ) { active_item = _active; }
  inline SelectOMatic_Item * GetActiveItem() { return active_item; }

  // Accessors
  inline SelectOMatic_Item               & GetRootGroup() { return root_group; }
  inline DynArray< SelectOMatic_Item * > & GetGroups()    { return root_group.GetChildren(); }
  inline DynArray< SelectOMatic_Item * > & GetSelected()  { return selected; }

  inline int                               GetInstanceIndex() { return instance_index; }

  // Event Listening
  bool         GetEnableUpdate() { return enable_update; }
  void         SetEnableUpdate( bool state ) { enable_update = state; }

  // Global Function
  static GlobalFunc *global;

protected:
  DynArray< SelectOMatic_Item * > selected;        // Selected item list, as seen in Layout
  SelectOMatic_Item               root_group;      // Base group
  SelectOMatic_Item             * active_group;    // Active Group
  SelectOMatic_Item             * active_item;     // Active Item

  int  instance_index;                             // Index of this instance

  bool enable_update;                              // True if the Master should update the selected list (disabled during Execute)
};

extern SelectOMatic_Data *som_data;
