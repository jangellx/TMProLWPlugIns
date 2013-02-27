//
// LWChannelScan.h
//

#ifndef LWCHANNELSCAN_HEADER
#define LWCHANNELSCAN_HEADER

#ifndef NULL
  #define NULL 0
#endif

#include <lwenvel.h>
#include <lwrender.h>
#include <lwdyna.h>

#include <portable/dynarray/dynarray.h>

// LWChannelScan
class LWChannelScan {
public:
   LWChannelScan( LWChannelInfo *_chan_info, LWChanGroupID _root_group,
                  LWItemInfo *_item_info = NULL, LWItemID _item = NULL,
                  bool _skip_bones = true );
  ~LWChannelScan();

  // Channel Functions
  LWChannelID   FirstChannel();                          // Finds the first channel.  If out of channels, goes to the next group.  If out of groups, returns NULL.
  LWChannelID   NextChannel( bool next_group = true );   // Finds the next channel.  If out of channels, goes to the next group.  If out of groups, returns NULL.
  LWChannelID   NextGroup();                             // Finds the first channel in the next group.  If out of channels and groups, returns NULL.

  unsigned long DoesChannelNameMatch( const char **channel_names );

  // Accessors
  inline bool   GetSkipBones()             { return skip_bones; }
  LWItemID      GetItemID()                { return item;       }

  inline LWChannelID   GetCurrentChannel() { return current_channel; }
  inline LWChanGroupID GetCurrentGroup()   { return current_group;   }
  inline LWChannelID   GetRootGroup()      { return root_group;      }

  inline void SetItemInfo( LWItemInfo *_info )      { item_info = _info; }

protected:
  LWChannelInfo  * chan_info;          // Channel Info
  LWItemInfo     * item_info;          // Item Info

  LWItemID         item;

  LWChannelID      current_channel;
  LWChanGroupID    current_group;
  LWChanGroupID    parent_group;
  LWChanGroupID    root_group;

  DynArray< LWChanGroupID > bone_groups;
  bool                      skip_bones;
};

#endif

