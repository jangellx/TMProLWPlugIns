//
// ReplaceObjects-Interface.cpp
//

#include "ReplaceObjects-Interface.h"
#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <portable/pfstream/pfstream.h>

static ReplaceObjects_Interface *rint = NULL;

DynArray< ReplaceObjects_SwapObject * > ReplaceObjects_Interface::old_swaps;
int                                     ReplaceObjects_Interface::store_original_name_value = 1;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "Replace Objects",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Object Replacement Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "ReplaceObjects, ReplaceWithOriginalObject",
  NULL };

const char *limit_to_popup_strings[] =
  { "Selected Objects", "All Objects", NULL };

const char *sort_by_popup_strings[] = 
  { "Name", "Weight", NULL };

// Constructor():
ReplaceObjects_Interface::ReplaceObjects_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), sel_type(-1), progress_current(0), progress_total(1),
  progress_panel(NULL), do_process(false) {

  generic     = (LWLayoutGeneric *)local;
  message     = (LWMessageFuncs  *)global( LWMESSAGEFUNCS_GLOBAL,  GFUSE_TRANSIENT );
  ui          = (LWInterfaceInfo *)global( LWINTERFACEINFO_GLOBAL, GFUSE_TRANSIENT );
  iteminfo    = (LWItemInfo      *)global( LWITEMINFO_GLOBAL,      GFUSE_TRANSIENT );
  objectinfo  = (LWObjectInfo    *)global( LWOBJECTINFO_GLOBAL,    GFUSE_TRANSIENT );
  panel_funcs = (LWPanelFuncs    *)global( LWPANELFUNCS_GLOBAL,    GFUSE_TRANSIENT );

  if( ui->selItems[0] != NULL )
    sel_type = (iteminfo->type)( ui->selItems[0] );

  unsigned long prodinfo = (unsigned long) global( LWPRODUCTINFO_GLOBAL, GFUSE_TRANSIENT );
  unsigned long major    = LWINF_GETMAJOR( prodinfo );
  unsigned long minor    = LWINF_GETMINOR( prodinfo );

  if( ((major >= 6) && (minor >= 1)) && (major < 7) )
    checkmark_string = "\03(i:MIMG_CHECKMARK)";
  else
    checkmark_string = "Yes";

  rint = this;
}

// Destructor():
ReplaceObjects_Interface::~ReplaceObjects_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  swaps.Flush();
  rint = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool ReplaceObjects_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }

  char title_string[256];
  sprintf( title_string, "ReplaceObjects -- %c2001 Joe Angell, TM Productions", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Limit To Pop-Up
  limit_to_popup = WPOPUP_CTL( panel_funcs, panel, "Limit To",
                               limit_to_popup_strings, 110 );

  // Sort By Pop-Up
  sort_by_popup = WPOPUP_CTL( panel_funcs, panel, "Sort By",
                              sort_by_popup_strings, 110 );
  PanelTools::PutOnLineWith( limit_to_popup, sort_by_popup, 10 );

  // Match
  match_toggle = BOOL_CTL( panel_funcs, panel, "Match Name" );
  match_string = STR_CTL(  panel_funcs, panel, "Name Contains", 41 );
  PanelTools::PutOnLineWith( match_toggle, match_string, 10 );

  // List
  list = MULTILIST_CTL( panel_funcs, panel, "Swap Objects",
                        300, 20, ListNames, ListCount, ListColumnWidths );
  PanelTools::PutUnder( match_toggle, list, 20 );

  // Add
  add = WBUTTON_CTL( panel_funcs, panel, "Add", 60 );
  PanelTools::PutOnLineWith( list, add, 3);

  // Clone
  clone = WBUTTON_CTL( panel_funcs, panel, "Clone", 60 );
  PanelTools::AlignLabels( add, clone );

  // Remove
  remove = WBUTTON_CTL( panel_funcs, panel, "Remove", 60 );
  PanelTools::AlignLabels( clone, remove );

  // Clear All
  clear_all  = WBUTTON_CTL( panel_funcs, panel, "Clear All", 60 );
  PanelTools::AlignLabels( remove, clear_all );
  PanelTools::PutUnder( clear_all, clear_all );

  // About
  about = WBUTTON_CTL( panel_funcs, panel, "About", 60 );
  PanelTools::PutOnLineWith( sort_by_popup, about );

  // Load
  load = LOADBUTTON_CTL( panel_funcs, panel, "Load", 60 );
  PanelTools::PutOnLineWith( sort_by_popup, about );

  // Save
  save = SAVEBUTTON_CTL( panel_funcs, panel, "Save", 60 );
  PanelTools::PutOnLineWith( sort_by_popup, about );

  // Path
  path = FILE_CTL( panel_funcs, panel, "Object Path", 59 );
  PanelTools::PutUnder( list, path, 18 );

  // Weight
  weight = FLOAT_CTL( panel_funcs, panel, "Weight" );
  PanelTools::AlignLabels( path, weight );

  // Enable
  enable = BOOL_CTL( panel_funcs, panel, "Enable" );
  PanelTools::PutOnLineWith( weight, enable );

  // Store Original Names
  store_original_name = BOOL_CTL( panel_funcs, panel, "Store Original Object Name As Tag" );
  SET_INT( store_original_name, store_original_name_value );
  PanelTools::AlignLabels( weight, store_original_name, 18 );

  int x  = CON_X( add );
  int y  = CON_Y( list );
  int h  = CON_H( list );
  int h2 = CON_H( about );
  MOVE_CON( about, x, (y + h) - h2 );
  MOVE_CON( save,  x, (y + h) - (h2 * 3) + 10 );
  MOVE_CON( load,  x, (y + h) - (h2 * 4) + 15 );

  // Default Values
  SET_INT( list, -1 );
  HandleList( list, NULL );

  // Event Handlers
  CON_SETEVENT( list,          HandleList,        NULL );
  CON_SETEVENT( about,         HandleAbout,       NULL );

  CON_SETEVENT( sort_by_popup, HandleSortBy,      NULL );
  CON_SETEVENT( match_toggle,  HandleMatchToggle, NULL );

  CON_SETEVENT( add,           HandleAdd,         NULL );
  CON_SETEVENT( clone,         HandleClone,       NULL );
  CON_SETEVENT( remove,        HandleRemove,      NULL );
  CON_SETEVENT( clear_all,     HandleClearAll,    NULL );

  CON_SETEVENT( load,          HandleLoad,        NULL );
  CON_SETEVENT( save,          HandleSave,        NULL );

  CON_SETEVENT( path,          HandlePath,        NULL );
  CON_SETEVENT( weight,        HandleWeight,      NULL );
  CON_SETEVENT( enable,        HandleEnable,      NULL );

  // Panel-Level Callbacks
  (*panel_funcs->set)( panel, PAN_USERDRAW, DrawPanel );

  // Get a copy of the swaps
  swaps.Flush();
  ReplaceObjects_SwapObject *new_swap;
  for( unsigned long i=0; i < old_swaps.NumElements(); i++ ) {
    new_swap = new ReplaceObjects_SwapObject;
    *new_swap = *old_swaps[i];
    swaps.Add( new_swap );
  }

  HandleMatchToggle( match_toggle, NULL );

  // Open the Panel
  int retval = (*panel_funcs->open)( panel, PANF_BLOCKING | PANF_CANCEL | PANF_PASSALLKEYS );
  if( retval > 0 ) {
    do_process = true;
    GET_INT( store_original_name, store_original_name_value );

    old_swaps.Flush();
    ReplaceObjects_SwapObject *new_swap;
    for( unsigned long i=0; i < swaps.NumElements(); i++ ) {
      new_swap = new ReplaceObjects_SwapObject;
      *new_swap = *swaps[i];
      old_swaps.Add( new_swap );
    }
  }

  return do_process;
}

// DrawMainPanel() {
void DrawPanel( LWPanelID panel, void *data, DrMode mode) {
  CDrawFuncs df( rint->panel_funcs->drawFuncs );

  int ph = PAN_GETH( rint->panel_funcs, rint->panel );
  int pw = PAN_GETW( rint->panel_funcs, rint->panel );

  // Horizontal Dividers
  int y = CON_Y( rint->match_toggle );
  int h = CON_H( rint->match_toggle );
  PanelDraw::HorizontalDivider( rint->panel, &df, 0, y + h + 3, pw, 1 );

  y = CON_Y( rint->list );
  h = CON_H( rint->list );
  PanelDraw::HorizontalDivider( rint->panel, &df, 0, y + h + 3, pw, 1 );

  y = CON_Y( rint->enable );
  h = CON_H( rint->enable );
  PanelDraw::HorizontalDivider( rint->panel, &df, 0, y + h + 3, pw, 1 );
}

// MultiList Callbacks
static char * list_names[]         = { "Use", "Object", "Weight", "Percent", NULL };
static int    list_column_widths[] = { 30, 165, 45, 50, 0 };

int ListCount( LWPanelID panel ) {
  return rint->swaps.NumElements();
}

char * ListNames( LWPanelID panel, int item, int col ) {
  if( item == -1 ) { // Column Headers
    return list_names[ col ];
  }

  if( item > (int)rint->swaps.NumElements() )
    return "";

  static char buffer[ 100 ];
  switch( col ) {
    case 0:         // Use
      return (rint->swaps[ item ]->GetUse() ? rint->checkmark_string : "");

    case 1:         // Object Name
      return ((rint->swaps[ item ]->GetFilename() == NULL) ? "(original object)" : (char *)rint->swaps[ item ]->GetFilename());

    case 2: {       // Weight
      sprintf( buffer, "%g", rint->swaps[item]->GetWeight() );
      return buffer;
    }

    case 3: {       // Percent
      if( !rint->swaps[ item ]->GetUse() )
        return "-";

      double total_weight = 0.0;
      for( unsigned long i=0; i < rint->swaps.NumElements(); i++ ) {
        if( rint->swaps[i]->GetUse() )
          total_weight += rint->swaps[i]->GetWeight();
      }

      if( total_weight < 0.000000001 )  // Possible divide by zero
        return "0%";

      sprintf( buffer, "%d%%", (int)((rint->swaps[item]->GetWeight() / total_weight) * 100.0) );
      return buffer;
    }

    default:
      return "";
  }
}

int ListColumnWidths( LWPanelID panel, int col ) {  // pixel width of column i, up to 8 columns terminate with 0
  return list_column_widths[ col ];
}

//
// --- Event Handlers ---------------------------------
//

// HandleList():
void HandleList( LWControl *con, void *data ) {
  int index, column, other;
  GET_IVEC( rint->list, index, column, other );

  if( index >= (int)rint->swaps.NumElements() ) {
    index = ((int)rint->swaps.NumElements()) - 1;
    SET_IVEC( rint->list, index, 1, 0 );
  } else {
    if( index != -1 ) {
      int x = CON_MOUSEX( con );
      int y = CON_MOUSEY( con );

      if( x < list_column_widths[0] + 8 )
        rint->swaps[ index ]->SetUse( !rint->swaps[ index ]->GetUse() );
    }
  }

  if( index == -1 ) {
    SET_STR( rint->path, "", 0 );
    SET_FLOAT( rint->weight, 1.0 );
    SET_INT( rint->enable, 1 );

    GHOST_CON( rint->path   );
    GHOST_CON( rint->weight );
    GHOST_CON( rint->enable );
  } else {
    char * string = (char *)rint->swaps[ index ]->GetFullPath();
    if( string == NULL ) {
      SET_STR( rint->path, "", 0 );
    } else {
      SET_STR( rint->path, string, strlen( string ) );
    }

    SET_FLOAT( rint->weight, rint->swaps[ index ]->GetWeight() );
    SET_INT( rint->enable,   rint->swaps[ index ]->GetUse() );

    UNGHOST_CON( rint->path   );
    UNGHOST_CON( rint->weight );
    UNGHOST_CON( rint->enable );
  }
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( rint->panel_funcs, "Replace Objects", about_text, rint->panel, true );
}

// HandleSortBy():
void HandleSortBy( LWControl *con, void *data ) {
  int state, index;
  GET_INT( rint->sort_by_popup, state );
  GET_INT( rint->list, index );

  // Store the item
  ReplaceObjects_SwapObject *swap = NULL;
  if( index != -1 ) {
    swap = rint->swaps[ index ];
    assert( swap != NULL );
  }

  // Sort the list
  if( state == 0 )
    rint->swaps.QSort( SortSwapObjects_Alpha );
  else
    rint->swaps.QSort( SortSwapObjects_Weight );

  // Reselect the item
  if( swap != NULL ) {
    unsigned long new_index = rint->swaps.FindIndexOf( swap );
    SET_IVEC( rint->list, new_index, 1, 0 );
  }

  REDRAW_CON( rint->list );
}

// HandleMatchToggle():
void HandleMatchToggle( LWControl *con, void *data ) {
  int state;
  GET_INT( rint->match_toggle, state );

  if( state == 0 ) {
    GHOST_CON( rint->match_string );
  } else {
    UNGHOST_CON( rint->match_string );
  }
}

// HandleAdd():
void HandleAdd( LWControl *con, void *data ) {
  ReplaceObjects_SwapObject *new_swap = new ReplaceObjects_SwapObject;
  rint->swaps.Add( new_swap );
  
  SET_IVEC( rint->list, (rint->swaps.NumElements() - 1), 1, 0 );
  HandleSortBy( rint->sort_by_popup, NULL );
  HandleList( rint->list, NULL );
}

// HandleClone():
void HandleClone( LWControl *con, void *data ) {
  int index;
  GET_INT( rint->list, index );

  if( index == -1 )
    return;

  assert( index < (int)rint->swaps.NumElements() );

  ReplaceObjects_SwapObject *new_swap = new ReplaceObjects_SwapObject;
  *new_swap = rint->swaps[ index ];
  rint->swaps.Add( new_swap );

  SET_IVEC( rint->list, (rint->swaps.NumElements() - 1), 1, 0 );
  HandleSortBy( rint->sort_by_popup, NULL );
  HandleList( rint->list, NULL );
}

// HandleRemove():
void HandleRemove( LWControl *con, void *data ) {
  int index;
  GET_INT( rint->list, index );

  if( index == -1 )
    return;

  assert( index < (int)rint->swaps.NumElements() );
  delete rint->swaps[ index ];
  rint->swaps.Remove( index );

  if( index == (int)rint->swaps.NumElements() )
    index--;

  SET_IVEC( rint->list, index, 1, 0 );
  REDRAW_CON( rint->list );
  HandleList( rint->list, NULL );
}

// HandleClearAll():
void HandleClearAll( LWControl *con, void *data ) {
  if( (*rint->message->yesNo)( "Clear Swap List?", "Do you really want to clear the swap list?", NULL ) == 0 )
    return;

  rint->swaps.Flush();
  REDRAW_CON( rint->list );
  HandleList( rint->list, NULL );
}

// HandleLoad():
void HandleLoad( LWControl *con, void *data ) {
  char path[ MAX_PATH_LENGTH ];
  GET_STR( rint->load, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  if( !DirInfo::Exists( path ) ) {
    rint->message->error( "Replace Objects Error:  Settings file doesn't exist:", path );
    return;
  }

  pifstream in( path );
  if( !in ) {
    (*rint->message->error)( "Replace Objects Error:  File I/O error occured opening settings file", path );
    return;
  }

  // Read in the header
  in.GuessEOLType();

  char buffer[ 2048 ];
  in >> buffer;
  if( stricmp( buffer, "TMP_RPS" ) != 0 ) {
    (*rint->message->error)( "Replace Objects Error:  File isn't a Replace Objects settings file", path );
    return;
  }

  int version;
  in >>version;
  if( version != 1 ) {
    (*rint->message->error)( "Replace Objects Error:  Replace Objects settings file is an unsupported version", path );
    return;
  }

  // Loop through and load the settings
  DynArray< ReplaceObjects_SwapObject * > swaps;
  ReplaceObjects_SwapObject *new_swap = NULL;
  bool store_original_name = false;

  while( true ) {
    in >> buffer;
    if( in.eof() )
      break;

    if( in.bad() || in.fail() ) {
      swaps.Flush();
      (*rint->message->error)( "Replace Objects Error:  File I/O error occured reading settings file", path );
      return;
    }

    if( new_swap == NULL ) {
      if( stricmp( "Swap", buffer ) == 0 ) {
        new_swap = new ReplaceObjects_SwapObject;
        new_swap->SetUse( false );
        swaps.Add( new_swap );
        in >> buffer;  // Skip the {
      } else if( stricmp( "StoreOriginalNames", buffer ) == 0 ) {
        store_original_name = true;
      }
    } else {
      if( buffer[0] == '}' ) {
        new_swap = NULL;
      } else if( stricmp( buffer, "Path" ) == 0 ) {
        in.getline( buffer, 2048 );
        StringTools::RemoveHeadWhiteSpace( buffer );
        new_swap->SetFullPath( buffer );
      } else if( stricmp( buffer, "Weight" ) == 0 ) {
        in >> buffer;
        new_swap->SetWeight( atof( buffer) );
      } else if( stricmp( buffer, "Enable" ) == 0 ) {
        new_swap->SetUse( true );
      }
    }
  }

  // Copy the new array over
  rint->swaps.Flush();
  rint->swaps = swaps;
  SET_INT( rint->store_original_name, (store_original_name ? 1 : 0) );

  SET_IVEC( rint->list, -1, 1, 0 );
  HandleSortBy( rint->sort_by_popup, NULL );
  HandleList( rint->list, NULL );
  (*rint->message->info)( "Replace Object:  Successfully loaded settings from", path );
}

// HandleSave():
void HandleSave( LWControl *con, void *data ) {
  char path[ MAX_PATH_LENGTH ];
  GET_STR( rint->save, path, MAX_PATH_LENGTH );

  if( path[0] == '\0' )
    return;

  DirStrings::ChangeExtension( path, ".rps" );
  pofstream out( path );
  if( !out ) {
    (*rint->message->error)( "Replace Objects Error:  File I/O error occured opening settings file", path );
    return;
  }

  // Output the header
  out << "TMP_RPS 1" << out.endl();

  // Loop through and save the items
  for( unsigned long i=0; i < rint->swaps.NumElements(); i++ ) {
    out << "Swap {" << out.endl();

    if( rint->swaps[i]->GetFullPath() != NULL )
      out << "  Path " << rint->swaps[i]->GetFullPath() << out.endl();

    if( rint->swaps[i]->GetWeight() != 1.0 )
      out << "  Weight " << rint->swaps[i]->GetWeight() << out.endl();

    if( rint->swaps[i]->GetUse() )
      out << "  Enable" << out.endl();

    out << "}" << endl;
  }

  // Save the Store Original Names state
  int state;
  GET_INT( rint->store_original_name, state );
  if( state != 0 )
    out << "StoreOriginalNames" << out.endl();

  // Test for errors
  if( !out ) {
    (*rint->message->error)( "Replace Objects Error:  File I/O error occured writing settings file", path );
    return;
  }

  (*rint->message->info)( "Replace Objects:  Successfully saved settings to", path );
}


// HandlePath():
void HandlePath( LWControl *con, void *data ) {
  int index;
  GET_INT( rint->list, index );

  if( index == -1 )
    return;

  assert( index < (int)rint->swaps.NumElements() );
  
  char buffer[ MAX_PATH_LENGTH ];
  GET_STR( rint->path, buffer, MAX_PATH_LENGTH );
  rint->swaps[ index ]->SetFullPath( buffer );

  HandleSortBy( rint->sort_by_popup, NULL );
}

// HandleWeight():
void HandleWeight( LWControl *con, void *data ) {
  int index;
  GET_INT( rint->list, index );

  if( index == -1 )
    return;

  assert( index < (int)rint->swaps.NumElements() );

  double weight;
  GET_FLOAT( rint->weight, weight );

  if( weight < 0.0 )
    weight = 0.0;

  rint->swaps[ index ]->SetWeight( weight );
  SET_FLOAT( rint->weight, weight );

  HandleSortBy( rint->sort_by_popup, NULL );
}

// HandleEnable():
void HandleEnable( LWControl *con, void *data ) {
  int index;
  GET_INT( rint->list, index );

  if( index == -1 )
    return;

  assert( index < (int)rint->swaps.NumElements() );

  int state;
  GET_INT( rint->enable, state );
  rint->swaps[ index ]->SetUse( (state == 0) ? false : true );

  REDRAW_CON( rint->list );
}

// ReselectItems():
//  Reselects the previously selected items
void ReplaceObjects_Interface::ReselectItems() {
  if( sel_type == -1 )
    return;

  for( int count=0; ui->selItems[ count ] != NULL; count++ ) { ; }
  progress_total = count;
  progress_current = 1;

  char buffer[100];
  sprintf( buffer, "SelectItem %x", ui->selItems[ count - 1 ] );
  (*generic->evaluate)( generic->data, buffer );

  for( int i=count-1; i != -1; i-- ) {
    // Update the progress bar
    if( rint->progress_open ) {
      sprintf( progress_string, "Selecting \"%s\"...", (*iteminfo->name)( ui->selItems[i] ) );
      rint->progress_current++;
      REDRAW_CON( rint->progress_field );

      // Update the panel and see if the user closed it (ie: cancel operation)
      if( (*rint->panel_funcs->handle)( rint->progress_panel, 0 ) == -1) {
        if( (*rint->message->yesNo)( "Replace Objects -- Cancel Reselection?",
                                     "Do you really want to abort the reselection process?",
                                     NULL ) == 1 ) {
          (*rint->message->info)( "Replace Objects Info:  Reselection aborted by user.", NULL );
          progress_open = false;
        } else {
          (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );
        }
      }
    }

    sprintf( buffer, "AddToSelection %x", ui->selItems[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}

// Process():
void ReplaceObjects_Interface::Process() {
  int limit_to, remember_name;
  GET_INT( limit_to_popup, limit_to );
  GET_INT( store_original_name, remember_name );

  char match_buffer[ 1024 ];
  GET_STR( rint->match_string, match_buffer, 1024 );
  char * match_data = (match_buffer[0] == '\0') ? NULL : match_buffer;

  // Compute the Total Weight
  double total_weight = 0.0;
  for( unsigned long i=0; i < swaps.NumElements(); i++ ) {
    if( rint->swaps[i]->GetUse() )
      total_weight += rint->swaps[i]->GetWeight();
  }

  if( total_weight < 0.000000001 ) { // Handle possible divide by zero
    (*message->error)( "Replace Objects Error:  All weights are 0.0 or no swaps are enabled; nothing to do; aborting", NULL );
    return;
  }

  // Turn off Dynamic Update for speed
  int dynamic_update = ui->dynaUpdate;
  (*generic->evaluate)( generic->data, "DynamicUpdate 0" );

  // Count the number of items
  if( limit_to == 0 ) {  // Selected Items
    for( int i=0; ui->selItems[i] != NULL; i++ ) { ; }
    progress_total = i;
  } else {               // All Items
    progress_total = 0;
    for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) )
      progress_total++;
  }

  // Open the Progress Panel
  OpenProgressPanel();

  (*generic->evaluate)( generic->data, "AutoConfirm 1" );
  // Process all the objects in the list
  srand( (unsigned)time( NULL ) * 1000 );
  if( limit_to == 0 ) {  // Selected Items
    for( int i=0; ui->selItems[i] != NULL; i++ ) {
      if( !ProcessItem( ui->selItems[i], total_weight, ((remember_name == 0) ? false : true) , match_data ) )
        break;
    }
  } else {               // All Items
    for( LWItemID item = (*iteminfo->first)( LWI_OBJECT, NULL ); item != NULL; item = (*iteminfo->next)( item ) ) {
      if( !ProcessItem( item, total_weight, ((remember_name == 0) ? false : true), match_data ) )
        break;
    }
  }
  (*generic->evaluate)( generic->data, "AutoConfirm 0" );

  // Reselect the previously-selected items
  if( !progress_open )
    OpenProgressPanel();

  ReselectItems();

  // Close the Progress panel
  if( progress_open )
    (*panel_funcs->close)( progress_panel );

  // Restore Dynamic Update to it's previous state
  char buffer[ 100 ];
  sprintf( buffer, "DynamicUpdate %d", dynamic_update );
  (*generic->evaluate)( generic->data, buffer );
}

// ProcessItem():
bool ReplaceObjects_Interface::ProcessItem( LWItemID item, double total_weight, bool remember_name, const char *match_data ) {
  // See if the name matches
  if( match_data != NULL ) {
    if( StringTools::stristr( (*iteminfo->name)( item ), (char *)match_data ) == 0 )
      return true;
  }

  // Update the progress bar
  if( rint->progress_open ) {
    sprintf( progress_string, "Replacing \"%s\"...", (*iteminfo->name)( item ) );
    rint->progress_current++;
    REDRAW_CON( rint->progress_field );

    // Update the panel and see if the user closed it (ie: cancel operation)
    if( (*rint->panel_funcs->handle)( rint->progress_panel, 0 ) == -1) {
      if( (*rint->message->yesNo)( "Replace Objects -- Cancel Processing?",
                                   "Do you really want to abort processing?",
                                   "Note that some objects may have already been replaced" ) == 1 ) {
        (*rint->message->info)( "Replace Objects Info:  Processing aborted by user.", NULL );
        progress_open = false;
        return false;
      } else {
        (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );
      }
    }
  }

  // Decide which item to swap with
  double value = ( ((double)rand()) / (double)RAND_MAX ) * total_weight;
  double cur_weight = 0.0;
  ReplaceObjects_SwapObject *swap = NULL;

  for( unsigned long i=0; i < swaps.NumElements(); i++ ) {
    if( swaps[i]->GetUse() ) {
      cur_weight += swaps[i]->GetWeight();
      if( value <= cur_weight ) {
        swap = swaps[i];
        break;
      }
    }
  }

  // Path invalid; do nothing
  if( swap->GetFullPath() == NULL )
    return true;

  // Store the original name
  char buffer[1024];
  if( remember_name ) {
    const char *tag;
    for( int t=1; tag = (*iteminfo->getTag)( item, t ); t++ ) {
      if( tag == NULL )
        break;

      if( strnicmp( tag, "OriginalItem", 12 ) == 0 )
        break;
    } 

    if( tag == NULL )
      t = 0;

    int type = 1;
    if( ((*objectinfo->numPoints)(   item ) <= 1) &&
        ((*objectinfo->numPolygons)( item ) == 0) ) {
      type = 0;
    }

    sprintf( buffer, "OriginalItem %d %s", type, (*objectinfo->filename)( item ) );
    (*iteminfo->setTag)( item, t, buffer );
  }

  // Replace the item
  sprintf( buffer, "SelectItem %x", item );
  (*generic->evaluate)( generic->data, buffer );

  sprintf( buffer, "ReplaceWithObject %s", swap->GetFullPath() );
  (*generic->evaluate)( generic->data, buffer );

  return true;
}

// OpenProgressPanel():
int ReplaceObjects_Interface::OpenProgressPanel() {
  // Destroy the panel if it hasn't been destroyed yet
  if( progress_panel != NULL ) {
    (*panel_funcs->destroy)( progress_panel );
    progress_panel = NULL;
  }

  // Create the panel
  char title_string[256];
  sprintf( title_string, "Replace Objects -- %c2001 Joe Angell, TM Productions", 169 );
  progress_panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !progress_panel ) {              // Make sure the panel was created
    (*message->error)("Error creating progress panel, aborting", NULL );
    return false;
  }

  // Progress Field
  progress_field = CANVAS_CTL( panel_funcs, progress_panel, "", 400, 20 );

  // Draw Callbacks
  ival.intv.value = (int)&DrawProgressField;
  (*progress_field->set)( progress_field, CTL_USERDRAW, &ival );

  // Set up some variables
  progress_open = true;
  strcpy( progress_string, "Processing..." );

  // Open the Panel
  int retval = (*panel_funcs->open)( progress_panel, PANF_PASSALLKEYS );

  return retval;
}

// DrawProgressField():
void DrawProgressField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = rint->panel_funcs->drawFuncs;

  // Clear the bar
  (*df->drawRGBBox)( rint->progress_panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

  // Fill the progress portion of the bar
  (*df->drawRGBBox)( rint->progress_panel, 0, 60, 150, cx+1, cy+1,
                    (int)((cw-2) * ( ((double)rint->progress_current) / ((double)rint->progress_total) ) ), ch-2 );

  // Write some text
  (*df->drawText)( rint->progress_panel, rint->progress_string, COLOR_WHITE, cx + 3, cy + 2 );
}

// SortSwapObjects_Alpha():
//  QSort function to sort alphabetically
int SortSwapObjects_Alpha(const void *elem1, const void *elem2 ) {
  ReplaceObjects_SwapObject * first  = *((ReplaceObjects_SwapObject **) elem1);
  ReplaceObjects_SwapObject * second = *((ReplaceObjects_SwapObject **) elem2);

  
  int value = 0;
  if( first->GetFilename() == NULL ) {
    if( second->GetFilename() == NULL ) {
      value = 0;
    } else {
      value = 1;
    }
  } else {
    if( second->GetFilename() == NULL ) {
      value = -1;
    } else {
      value = stricmp( first->GetFilename(), second->GetFilename() );
    }
  }

  if( value == 0 )
    return ((first->GetWeight() < second->GetWeight()) ? -1 : 1);

  return value;
}

// SortSwapObjects_Weight():
//  QSort function to sort alphabetically
int SortSwapObjects_Weight(const void *elem1, const void *elem2 ) {
  ReplaceObjects_SwapObject * first  = *((ReplaceObjects_SwapObject **) elem1);
  ReplaceObjects_SwapObject * second = *((ReplaceObjects_SwapObject **) elem2);

  if( first->GetWeight() == second->GetWeight() ) {
    if( first->GetFilename() == NULL ) {
      if( second->GetFilename() == NULL ) {
        return 0;
      } else {
        return 1;
      }
    } else {
      if( second->GetFilename() == NULL ) {
        return -1;
      } else {
        return stricmp( first->GetFilename(), second->GetFilename() );
      }
    }
  }

  return ((first->GetWeight() < second->GetWeight()) ? -1 : 1);
}
