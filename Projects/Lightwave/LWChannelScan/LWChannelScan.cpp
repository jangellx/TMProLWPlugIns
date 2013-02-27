//
// LWChannelScan.cpp
//

#include "LWChannelScan.h"

// Constructor
LWChannelScan::LWChannelScan( LWChannelInfo *_chan_info, LWChanGroupID _root_group,
                              LWItemInfo *_item_info,
                              LWItemID _item, bool _skip_bones )
  : chan_info( _chan_info ), item_info( _item_info ), root_group( _root_group ),
    item( _item ), skip_bones( _skip_bones ), current_channel( NULL ) {

  current_group = root_group;
  parent_group  = root_group;

  if( (item_info != NULL) && (item != NULL) ) {
    LWItemID bone_id_cur = NULL;
    LWItemID bone_id_parent = item;

    while( true ) {
      // Read the next child
      if( bone_id_cur == NULL )
        bone_id_cur = (*item_info->firstChild)( bone_id_parent );
      else
        bone_id_cur = (*item_info->nextChild)( bone_id_parent, bone_id_cur );

      // Out of children at root; break out
      if( (bone_id_cur == NULL) && (bone_id_parent == item) )
        break;
     
      // No Children; try the next sibling
      if( bone_id_cur == NULL ) {
        bone_id_cur = bone_id_parent;
        bone_id_parent = (*item_info->parent)(bone_id_parent);
        continue;
      }

      // If the item is a bone, add it to the list
      if( (*item_info->type)( bone_id_cur ) == LWI_BONE )
        bone_groups.Add( (*item_info->chanGroup)( bone_id_cur ) );
    }
  }
}

// Destructor
LWChannelScan::~LWChannelScan() {
  ;
}

// FirstChannel():
LWChannelID LWChannelScan::FirstChannel() {
  // Reset the group history
  current_group = root_group;
  parent_group  = root_group;

  // Find a channel
  return NextChannel();
}

// NextChannel():
//  Returns the next channel, or NULL if there are no more channels.
//   If next_group is true, then this will check the remaining
//   channel groups before failing returning NULL.
LWChannelID LWChannelScan::NextChannel( bool next_group ) {
  current_channel = (*chan_info->nextChannel)( current_group, current_channel );
  if( current_channel == NULL ) {
    if( next_group )
      return NextGroup();
    else
      return NULL;
  }

  return current_channel;
}

// NextGroup():
//  Returns the first channel in the next group, or NULL if there
//   are no more groups/channels.
LWChannelID LWChannelScan::NextGroup() {
  current_channel = NULL;

  // set parent to root if NULL
  if( parent_group == NULL )
    parent_group = root_group;

  // starting out, so return first child
  if( current_group == NULL ) {
    current_group = (*chan_info->nextGroup)( parent_group, NULL );
    return NextChannel();
  }

  // try to go down the tree
  LWChanGroupID next = (*chan_info->nextGroup)( current_group, NULL );
  if( next != NULL ) {
    parent_group = current_group;
    current_group = next;
    return NextChannel();
  }

  if( parent_group == current_group )
    return NULL;

  // couldn't go down
  while( true ) {
    // try to go sideways
    current_group = (*chan_info->nextGroup)( parent_group, current_group );
    if( current_group != NULL )
      return NextChannel();

    // if I'm at the root of the tree, end
    if( parent_group == root_group )
      return NULL;

    // couldn't go sideways, go up
    current_group = parent_group;
    parent_group = (*chan_info->groupParent)( current_group );
  }
}

// DoesChannelNameMatch():
//  Returns the index into the channel name array that the name of the
//   channel matches, or -1 if there is no match or invalid data.
unsigned long LWChannelScan::DoesChannelNameMatch( const char **channel_names ) {
  if( current_channel == NULL )
    return -1;

  if( channel_names == NULL )
    return -1;

  const char * this_name = (*chan_info->channelName)( current_channel );
  for( unsigned long i=0; channel_names[i] != NULL; i++ ) {
    if( stricmp( this_name, channel_names[i] ) == 0 )
      return i;
  }

  return -1;
}

