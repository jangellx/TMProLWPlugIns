//
// FreeKeyInterface-Group.cpp
//
// This file contains all the FreeKeyInterface Group-level
//  contol functions.
//

#include <stdio.h>

#include "FreeKey-Interface.h"

// Group Number Controls
char *GroupNames( void *dat, int index ) {
  static char buffer[1024];

	if( index == (int)fk->GetGroupList().NumElements() )
    return " (new group)  ";
	else if( index >= (int)fk->GetGroupList().NumElements() )
		return NULL;
  else {
    if( fk->GetGroupList()[ index ]->GetName() == NULL )
      sprintf( buffer, "(unnamed)", index+1 );
    else
      sprintf( buffer, "%s", fk->GetGroupList()[ index ]->GetName() );
    return buffer;
  }
}

int GroupCount( void *dat ) {
  return fk->GetGroupList().NumElements() + 1;
}

// OnGroupList():
//  Called to change the currently selected group
void OnGroupList( LWControl *ctl, FreeKeyInterface *panel ) {
  int value;
  GET_INT( fk_int->group_list, value );
  fk->SetCurrentGroup( value );

  if( fk->GetCurrentGroup() == (int)fk->GetGroupList().NumElements() ) {
    // Add a new group
    fk->AddGroup();

    // Set it as the current group
    fk->SetCurrentGroup( fk->GetGroupList().NumElements() - 1 );

    // Update the interface
    SET_INT( fk_int->group_list, fk->GetCurrentGroup() );
    fk_int->FillWithFreeKeyGroup( fk->GetCurrentGroup() );
  }

  // Update the interface
  fk_int->FillWithFreeKeyGroup( fk->GetCurrentGroup() );
}

// OnGroupName():
//  Called to change the current group's name
void OnGroupName( LWControl *ctl, FreeKeyInterface *panel ) {
  char buffer[1024];
  GET_STR( fk_int->group_name, buffer, 1024 );

  if( buffer[0] == '\0' )
    fk->GetGroupList()[ fk->GetCurrentGroup() ]->SetName( NULL );
  else
    fk->GetGroupList()[ fk->GetCurrentGroup() ]->SetName( buffer );

//  SET_INT( fk_int->group_list, fk->GetCurrentGroup() );
  REDRAW_CON( fk_int->group_list );
}

// OnGroupDel():
//  Removes an existing group and updates the interface
void OnGroupDel( LWControl *ctl, FreeKeyInterface *panel ) {
  unsigned int i, j;

  // Make sure at least 1 group persists
  if( fk->GetGroupList().NumElements() < 2 ) {
    fk_int->message->Error( "FreeKey:  You must have at least one group!" );
    return;
  }

  // Un-group the elements assigned to this group
  for( i=0; i < fk->GetScene().GetBaseItem().GetChildren().NumElements(); i++ )
    ChangeGroup( fk->GetScene().GetBaseItem().GetChildren()[i], fk->GetCurrentGroup(), -1, true );

  // Remove the old group
  fk->GetGroupList().Remove( fk->GetCurrentGroup() );

  // Update all the other groups after this group
  for( j=fk->GetCurrentGroup(); j < fk->GetGroupList().NumElements(); j++ ) {
    for( unsigned int i=0; i < fk->GetScene().GetBaseItem().GetChildren().NumElements(); i++ )
      ChangeGroup( fk->GetScene().GetBaseItem().GetChildren()[i], j+1, j, true );
  }

  // Update the current group, if needed
  if( fk->GetCurrentGroup() >= fk->GetGroupList().NumElements() - 1 )
    fk->SetCurrentGroup( fk->GetGroupList().NumElements() - 1 );

  // Update the interface
  SET_INT( fk_int->group_list, fk->GetCurrentGroup() );
  fk_int->FillWithFreeKeyGroup( fk->GetCurrentGroup() );
}

// OnRangeLow()
void OnRangeLow( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->range_low,  v1 );
  fk->GetGroupList()[ fk->GetCurrentGroup() ]->SetLow( v1 );
}

// OnRangeHigh()
void OnRangeHigh( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->range_high, v1 );
  fk->GetGroupList()[ fk->GetCurrentGroup() ]->SetHigh( v1 );
}

// OnRemoveMode()
void OnRemoveMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->remove_mode,  v1 );
  switch( v1 ) {
  case 0:
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetRemoveMode( FreeKeyGroup::remove_none );
    break;
  case 1:
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetRemoveMode( FreeKeyGroup::remove_inside );
    break;
  case 2:
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetRemoveMode( FreeKeyGroup::remove_outside );
    break;
  case 3:
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetRemoveMode( FreeKeyGroup::remove_before );
    break;
  case 4:
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetRemoveMode( FreeKeyGroup::remove_after );
    break;
  }

  UpdateGroupGhosts( fk->GetCurrentGroup() );
}

// OnStepAmount()
void OnStepAmount( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->step_amount, v1 );
  fk->GetGroupList()[fk->GetCurrentGroup()]->SetStep( v1 );
}

// OnStepMode()
void OnStepMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->step_mode, v1 );

  // Figure out the mode
  if( v1 == 1 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetInvertStep( false );
  else if( v1 == 2 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetInvertStep( true );

  // Check the enable state
  if( v1 > 0 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseStep( true );
  else
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseStep( false );

  UpdateGroupGhosts( fk->GetCurrentGroup() );
}

// OnDeltaAmount()
void OnDeltaAmount( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->delta_amount, v1 );
  fk->GetGroupList()[fk->GetCurrentGroup()]->SetDelta( v1 );
}

// OnDeltaMode()
void OnDeltaMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->delta_mode, v1 );
  if( v1 > 0 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseDelta( true );
  else
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseDelta( false );

  UpdateGroupGhosts( fk->GetCurrentGroup() );
}

// OnScaleAmount()
void OnScaleAmount( LWControl *ctl, FreeKeyInterface *panel ) {
  double v1;
  GET_FLOAT( fk_int->scale_amount, v1 );

  if( v1 < 0 ) {
    SET_FLOAT( fk_int->scale_amount, fk->GetGroupList()[fk->GetCurrentGroup()]->GetScaleKeys() );
    fk_int->message->Error( "FreeKey Error:  Scale Keys Factor",
                            "must be greater than 0!" );
  }

  GET_FLOAT( fk_int->scale_amount, v1 );
  fk->GetGroupList()[fk->GetCurrentGroup()]->SetScaleKeys( (float)v1 );
}

// OnScaleMode()
void OnScaleMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->scale_mode, v1 );
  if( v1 > 0 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseScaleKeys( true );
  else
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetUseScaleKeys( false );

  UpdateGroupGhosts( fk->GetCurrentGroup() );
}

// OnDirectionMode()
void OnDirectionMode( LWControl *ctl, FreeKeyInterface *panel ) {
  int v1;
  GET_INT( fk_int->direction_mode, v1 );
  if( v1 > 0 )
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetDirection( true );
  else
    fk->GetGroupList()[fk->GetCurrentGroup()]->SetDirection( false );

  UpdateGroupGhosts( fk->GetCurrentGroup() );
}

// FillInFreeKeyGroup()
//  Fills in the FreeKeyGroup index specified with the
//  values of the controls in the interface.
void FreeKeyInterface::FillInFreeKeyGroup( unsigned long group ) {
  if( fk->GetCurrentGroup() == -1 )
    return;

  int  v1, v2;

  // Name
  OnGroupName( fk_int->group_name, fk_int );

  // Range
  GET_INT( fk_int->range_low,  v1 );
  GET_INT( fk_int->range_high, v2 );
  fk->GetGroupList()[group]->SetRange( v1, v2 );

  // Remove Mode
  OnRemoveMode( fk_int->remove_mode, fk_int );

  // Step
  OnStepAmount( fk_int->step_amount, fk_int );
  OnStepMode( fk_int->step_mode, fk_int );

  // Delta
  OnDeltaAmount( fk_int->delta_amount, fk_int );
  OnDeltaMode( fk_int->delta_mode, fk_int );

  // Scale
  OnScaleAmount( fk_int->scale_amount, fk_int );
  OnScaleMode( fk_int->scale_mode, fk_int );

  // Direction
  OnDirectionMode( fk_int->direction_mode, fk_int );
}

// FillWithFreeKeyGroup()
//  Fills in the interface with the FreeKeyGroup index specified
void FreeKeyInterface::FillWithFreeKeyGroup( unsigned long group ) {
  if( fk->GetCurrentGroup() == -1 )
    return;

  if( fk->GetCurrentGroup() >= (int)fk->GetGroupList().NumElements() )
    return;

  // Name
  if( fk->GetGroupList()[group]->GetName() == NULL )
    SET_STR( fk_int->group_name, "", 0 );
  else {
    SET_STR( fk_int->group_name, (char *)fk->GetGroupList()[group]->GetName(),
             strlen( fk->GetGroupList()[group]->GetName() ) );
  }

  // Range
  SET_INT( fk_int->range_low,  fk->GetGroupList()[group]->GetLow()  );
  SET_INT( fk_int->range_high, fk->GetGroupList()[group]->GetHigh() );

  // Remove mode
  switch( fk->GetGroupList()[group]->GetRemoveMode() ) {
  case FreeKeyGroup::remove_none:
    SET_INT( fk_int->remove_mode, 0 );
    break;
  case FreeKeyGroup::remove_inside:
    SET_INT( fk_int->remove_mode, 1 );
    break;
  case FreeKeyGroup::remove_outside:
    SET_INT( fk_int->remove_mode, 2 );
    break;
  }

  if( fk->GetGroupList()[group]->GetRemoveMode() == FreeKeyGroup::remove_inside ) {
    GHOST_CON( fk_int->step_amount  );
    GHOST_CON( fk_int->step_mode    );
    GHOST_CON( fk_int->delta_amount );
    GHOST_CON( fk_int->delta_mode   );
    GHOST_CON( fk_int->scale_amount );
    GHOST_CON( fk_int->scale_mode   );
  } else {
    UNGHOST_CON( fk_int->step_amount  );
    UNGHOST_CON( fk_int->step_mode    );
    UNGHOST_CON( fk_int->delta_amount );
    UNGHOST_CON( fk_int->delta_mode   );
    UNGHOST_CON( fk_int->scale_amount );
    UNGHOST_CON( fk_int->scale_mode   );
  }

  // Step
  SET_INT( fk_int->step_amount, fk->GetGroupList()[group]->GetStep() );

  if( fk->GetGroupList()[group]->GetUseStep() ) {
    if( fk->GetGroupList()[group]->GetInvertStep() )
      SET_INT( fk_int->step_mode, 2 );
    else 
      SET_INT( fk_int->step_mode, 1 );
  } else {
    SET_INT( fk_int->step_mode, 0 );
    GHOST_CON( fk_int->step_amount );
  }

  // Delta
  SET_INT( fk_int->delta_amount, fk->GetGroupList()[group]->GetDelta() );

  if( fk->GetGroupList()[group]->GetUseDelta() )
    SET_INT( fk_int->delta_mode, 1 );
  else 
    SET_INT( fk_int->delta_mode, 0 );

  // Scale
  SET_FLOAT( fk_int->scale_amount, fk->GetGroupList()[group]->GetScaleKeys() );

  if( fk->GetGroupList()[group]->GetUseScaleKeys() )
    SET_INT( fk_int->scale_mode, 1 );
  else 
    SET_INT( fk_int->scale_mode, 0 );

  UpdateGroupGhosts( group );
  fk_int->RedrawItemList();

  // Direction
  SET_INT( fk_int->direction_mode, fk->GetGroupList()[group]->GetDirection() );
}

// UpdateGroupGhosts()
//  Updates the ghosted state of all the group controls
void UpdateGroupGhosts( unsigned long group ) {
  if( fk->GetGroupList()[group]->GetRemoveMode() == FreeKeyGroup::remove_inside ) {
    GHOST_CON( fk_int->step_amount    );
    GHOST_CON( fk_int->step_mode      );
    GHOST_CON( fk_int->delta_amount   );
    GHOST_CON( fk_int->delta_mode     );
    GHOST_CON( fk_int->scale_amount   );
    GHOST_CON( fk_int->scale_mode     );
    GHOST_CON( fk_int->direction_mode );
  } else {
    if( fk->GetGroupList()[group]->GetUseStep() )
      UNGHOST_CON( fk_int->step_amount  );
    else
      GHOST_CON( fk_int->step_amount  );

    UNGHOST_CON( fk_int->step_mode    );

    if( fk->GetGroupList()[group]->GetUseDelta() )
      UNGHOST_CON( fk_int->delta_amount );
    else
      GHOST_CON( fk_int->delta_amount );

    UNGHOST_CON( fk_int->delta_mode   );

    if( fk->GetGroupList()[group]->GetUseScaleKeys() )
      UNGHOST_CON( fk_int->scale_amount );
    else
      GHOST_CON( fk_int->scale_amount );

    UNGHOST_CON( fk_int->scale_mode   );

    UNGHOST_CON( fk_int->direction_mode );
  }
}

// OnGroupEditApply():
//  Applies the Group Edit Mode to the group, thus adding or
//   removing items.
void OnGroupEditApply( LWControl *ctl, FreeKeyInterface *panel ) {
  int value;
  GET_INT( fk_int->group_edit_mode, value );

  switch( value ) {
    case 0:            // Add Selected To Group
      SetSelectionToGroup( fk->GetCurrentGroup() );
      break;

    case 1:            // Add All Ungrouped To Group
      ChangeGroup( &fk->GetScene().GetBaseItem(), -1, fk->GetCurrentGroup(), true );
      break;

    case 2:            // Remove Selected From Group
      SetSelectionToGroup( -1 );
      break;

    case 3:            // Remove All From Group
      ChangeGroup( &fk->GetScene().GetBaseItem(), fk->GetCurrentGroup(), -1, true );
      break;
  }

  fk_int->RedrawItemList();
}

// SetSelectionToGroup():
//  Changes the group of the current selection
void SetSelectionToGroup( unsigned long group ) {
  for( unsigned int i=0; i < fk->GetSelectedItems().NumElements(); i++ )
    fk->GetSelectedItems()[i]->SetGroup( group );
}

// ChangeGroup():
//  Changes a an item's group to new_group if it already belongs
//   to old_group.  If children is true, then children and bones
//   will also be changed.
void ChangeGroup( LWItem *item, unsigned long old_group, unsigned long new_group, bool children ) {
  if( item->GetGroup() == old_group )
    item->SetGroup( new_group );

  if( children ) {
    for( unsigned int i=0; i < item->GetChildren().NumElements(); i++ )
      ChangeGroup( item->GetChildren()[i], old_group, new_group, true );

    if( item->HasSkeleton() ) {
      ChangeGroup( &item->GetSkeleton(), old_group, new_group, true );
      item->GetSkeleton().SetGroup( 0 );
    }
  }
}
