//
// MotionReferencerInstance.h
//

#include <portable/dirtools/dirtools.h>

struct st_ItemMotionAccess;
struct st_LWItemMotionAccess;

class MotionReferencerInstance;

class MotionReferencerInstance {
public:

public:
  MotionReferencerInstance();
  ~MotionReferencerInstance();

  // Evaluate Functions
        inline unsigned int GetFlags() { return flags; }                  // Returns flags used by the Item Motion plug-in

        void   ChangeItemIDs( const LWItemID *new_items );                       // Change Item IDs
        void   CopyFrom( MotionReferencerInstance &other_inst );                 // Copy From an instance

        bool   LoadFromScene( char *error_buffer, const LWLoadState *loader );   // Load Data From Scene
        bool   SaveToScene(   char *error_buffer, const LWSaveState *saver  );   // Save Data To Scene

        bool   LoadFromFile( const char *filename, char * &error );              // Load data from file
        bool   SaveToFile(   const char *filename, char * &error );              // Save data to file

  // Accessors
  static inline void         SetGlobal( GlobalFunc *_global ) { global = _global; }           // Set the global function
  static inline GlobalFunc * GetGlobal()                      { return global;    }           // Get the global function

         inline void         SetEnabled( bool state )         { enabled = state;  }           // Set the enabled state
         inline bool         GetEnabled()                     { return enabled;   }           // Get the enabled state

         inline void         SetItemID( LWItemID new_item )   { item[0] = new_item; GenerateInstanceName(); } // Set the ID of the item for this instance
         inline LWItemID     GetItemID()                      { return item[0]; }             // Get the ID of the item for this instance

                bool         SetRefScene( const char *new_ref );                              // Set the Reference Scene's Name
                const char * GetRefScene()                    { return ref_scene_filename; }  // Get the Reference Scene's Name

                bool         SetRefItem( const char *new_ref );                               // Set the Reference Item
                const char * GetRefItem()                     { return ref_item_name; }       // Get the Reference Item

                bool         SetRefBone( const char *new_ref );                               // Set the Reference Item
                const char * GetRefBone()                     { return ref_bone_name; }       // Get the Reference Bone

                LWItemID   * GetItemList()                    { return item; }                // Get the Item List (for useItems());

         inline const char * GetInstanceName()                { return instance_name; }       // Returns the instance name

         inline void         SetScale( double _scale )        { scale = _scale; }             // Set the Scale Factor
         inline double       GetScale()                       { return scale;   }             // Get the Scale Factor

         inline void         SetShift( double _shift )        { shift = _shift; }             // Set the shift in time
         inline double       GetShift()                       { return shift;   }             // Get the shift in time

protected:
  void GenerateInstanceName();

  char ref_scene_filename[ MAX_PATH_LENGTH ];   // Reference Scene Path
  char ref_item_name[      MAX_PATH_LENGTH ];   // Reference Item
  char ref_bone_name[      MAX_PATH_LENGTH ];   // Reference Bone
  int  ref_item_index;                          // Reference Item Index

  bool enabled;                                 // true if this instance is enabled

  unsigned int flags;                           // Item Motion Flags

  double shift;                                 // Shift keys over time (seconds)
  double scale;                                 // Scale keys across time (multiplier)

  LWItemID item[2];                             // The list of items we're operating on (this is an array so we can return it as an item list for UseItems/ChangeIDs)

  char instance_name[ 256 ];                    // Name of the instance as shown in the Instance Pop-Up

  static GlobalFunc * global;                   // Lightwave Global Function
};

