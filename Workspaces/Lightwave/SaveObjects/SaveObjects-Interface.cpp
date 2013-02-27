//
// SaveObjects-Interface.cpp
//

#include "SaveObjects-Interface.h"
#include <Lightwave/LWAboutPanel_TMPro/LWAboutPanel_TMPro.h>
#include <stdio.h>

static SaveObjects_Interface *SO_Int = NULL;

const char *about_text[] = {
  LWABOUT_YELLOW,
  "SaveObjects",
  LWABOUT_VTAB,
  LWABOUT_BLACK,
  "Object Save Tools for Lightwave 3D",
  LWABOUT_HDIV,
  __DATE__,
  "Copyright 2000-2001 Joe Angell, TM Productions",
  LWABOUT_HDIV,
  "http://www.tmproductions.com",
  "jangell@tmproductions.com",
  LWABOUT_HDIV,
  "SaveObjects,",
  "BackupAndSaveObject, ReplaceWithBackupObject,",
  "SaveNewObjectRevision, ReplaceWithPreviousObjectRevision,",
  "ReplaceWithNextObjectRevision",
  NULL };

const char *limit_to_popup_strings[] =
  { "Current Object", "Selected Objects", "All Objects", NULL };

const char *pick_popdown_strings[] =
  { "Mark All",     "Mark Read-Only",   "Mark Read/Write",    "--  --  --  --  --  --  --  --",
    "Unmark All",   "Unmark Read-Only", "Unmark Read/Write",  "--  --  --  --  --  --  --  --",
    "Invert Marked", NULL };

const char *post_save_popup_strings[] =
  { "Set to Read-Only", "Set to Read/Write", "Restore Previous State", NULL };

const char *versioning_popup_strings[] =
  { "Disabled", "Backup Original", "New Revision", "Revision Backup",
    "Duplicate", "Duplicate And Replace", NULL };

const char *seperator_strings[] =
  { "-", "_", NULL };

// Static Memember Init
int  SaveObjects_Interface::limit_to_state       = 1;
int  SaveObjects_Interface::alpha_state          = 0;
int  SaveObjects_Interface::post_save_state      = 2;
int  SaveObjects_Interface::save_read_only_state = 0;
int  SaveObjects_Interface::versioning_state     = 0;
int  SaveObjects_Interface::min_digits_state     = 2;
char SaveObjects_Interface::path_string_state[   MAX_PATH_LENGTH ] = "";
char SaveObjects_Interface::prefix_string_state[ MAX_PATH_LENGTH ] = "";
char SaveObjects_Interface::suffix_string_state[ MAX_PATH_LENGTH ] = "";
int  SaveObjects_Interface::seperator_state      = 0;

// Constructor():
SaveObjects_Interface::SaveObjects_Interface( GlobalFunc *_global, void *local )
  : global(*_global), panel(NULL), sel_type(-1), progress(0.0f) {

  strcpy( progress_string, "Welcome to SaveObjects!" );

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

  SO_Int = this;
}

// Destructor():
SaveObjects_Interface::~SaveObjects_Interface() {
  if( panel != NULL )
    (*panel_funcs->destroy)( panel );

  objects.Flush();

  SO_Int = NULL;
}

// OpenUI():
//  Opens the user interface.  Returns after the panel has been closed.
bool SaveObjects_Interface::OpenUI() {
  // Destroy the panel if it hasn't been destroyed yet
  if( panel != NULL ) {
    (*panel_funcs->destroy)( panel );
    panel = NULL;
  }
  
  char title_string[256];
  sprintf( title_string, "SaveObjects -- %c2000-2001 Joe Angell, TM Productions", 169 );
  panel = (*panel_funcs->create)( title_string, panel_funcs );
  if( !panel ) {                    // Make sure the panel was created
    (*message->error)("Error creating main panel, aborting", NULL );
    return false;
  }

  // Limit To Pop-Up
  limit_to_popup = WPOPUP_CTL( panel_funcs, panel, "Limit To",
                               limit_to_popup_strings, 110 );

  // Alpha
  alpha_toggle = BOOLBUTTON_CTL( panel_funcs, panel, "Alpha" );
  PanelTools::PutOnLineWith( limit_to_popup, alpha_toggle );

  // Refresh Button
  refresh_button = WBUTTON_CTL( panel_funcs, panel, "Refresh", 56 );
  PanelTools::PutOnLineWith( alpha_toggle, refresh_button );

  // Pick Pop-Up
  pick_popdown = POPDOWN_CTL( panel_funcs, panel, "Mark Objects To Save       ",
                              pick_popdown_strings );
  PanelTools::PutOnLineWith( refresh_button, pick_popdown/*, -112*/ );

  // About
  about = WBUTTON_CTL( panel_funcs, panel, "?", 20 );
  PanelTools::PutOnLineWith( pick_popdown, about );

  // List
  list = MULTILIST_CTL( panel_funcs, panel, "Selected Items",
                        420, 20, ListNames, ListCount, ListColumnWidths );

  // Post-Save Attributes
  post_save_popup = WPOPUP_CTL( panel_funcs, panel, "Post-Save File Mode",
                                post_save_popup_strings, 164 );

  // Save Read-Only Objects
  save_read_only_toggle = BOOL_CTL( panel_funcs, panel, "Save Read-Only Objects" );
  PanelTools::PutOnLineWith( post_save_popup, save_read_only_toggle );

  // Versioning Popup
  versioning_popup = WPOPUP_CTL( panel_funcs, panel, "Versioning",
                                 versioning_popup_strings, 164 );
  PanelTools::AlignLabels( post_save_popup, versioning_popup );

  // Min Revision Digits
  min_digits_int   = INT_CTL( panel_funcs, panel, "Min. Digits" );
  PanelTools::PutOnLineWith( versioning_popup, min_digits_int, 2 );

  // Seperator
  seperator_popup  = WPOPUP_CTL( panel_funcs, panel, "", seperator_strings, 30 );
  PanelTools::PutOnLineWith( min_digits_int, seperator_popup, -5 );

  // Path
  path_string = FILE_CTL( panel_funcs, panel, "Path", 60 );
  PanelTools::AlignLabels( versioning_popup, path_string );
  MOVE_CON( path_string, (CON_X(path_string)) - 3, (CON_Y(path_string)) );

  // Prefix/Suffix
  prefix_string = STR_CTL(  panel_funcs, panel, "Prefix", 25 );
  suffix_string = STR_CTL(  panel_funcs, panel, "Suffix", 25 );
  PanelTools::AlignLabels( path_string, prefix_string );
  PanelTools::PutOnLineWith( prefix_string, suffix_string, 10 );

  // Progress Field
  progress_field = CANVAS_CTL( panel_funcs, panel, "", 282, (CON_HOTH( refresh_button )) );

  // Save Marked Objects
  save_marked_objects_button = WBUTTON_CTL( panel_funcs, panel, "Save Marked Objects", 150 );
  PanelTools::PutOnLineWith( progress_field, save_marked_objects_button, 3 );

  // Event Handlers
  CON_SETEVENT( list,                       HandleList,              NULL );
  CON_SETEVENT( limit_to_popup,             HandleLimitToPopup,      NULL );
  CON_SETEVENT( alpha_toggle,               HandleAlphaToggle,       NULL );
  CON_SETEVENT( pick_popdown,               HandlePickPopdown,       NULL );
  CON_SETEVENT( about,                      HandleAbout,             NULL );
  CON_SETEVENT( refresh_button,             HandleRefreshButton,     NULL );
  CON_SETEVENT( versioning_popup,           HandleVersioningPopup,   NULL );
  CON_SETEVENT( min_digits_int,             HandleMinDigitsInt,      NULL );
  CON_SETEVENT( path_string,                HandlePathString,        NULL );
  CON_SETEVENT( prefix_string,              HandlePathString,        NULL );
  CON_SETEVENT( suffix_string,              HandlePathString,        NULL );
  CON_SETEVENT( save_marked_objects_button, HandleSaveMarkedObjects, NULL );

  // Draw Callbacks
  ival.intv.value = (int)&DrawProgressField;
  (*progress_field->set)( progress_field, CTL_USERDRAW, &ival );

  // Set some default control states
  SET_INT( limit_to_popup,        limit_to_state       );
  SET_INT( alpha_toggle,          alpha_state          );
  SET_INT( post_save_popup,       post_save_state      );
  SET_INT( save_read_only_toggle, save_read_only_state );
  SET_INT( versioning_popup,      versioning_state     );
  SET_INT( min_digits_int,        min_digits_state     );
  SET_INT( seperator_popup,       seperator_state      );
  SET_STR( path_string,           path_string_state,   strlen( path_string_state   ) );
  SET_STR( prefix_string,         prefix_string_state, strlen( prefix_string_state ) );
  SET_STR( suffix_string,         suffix_string_state, strlen( suffix_string_state ) );

  HandleVersioningPopup( versioning_popup, NULL );

  // Build a list of selected objects
  FillObjectList();

  // Open the Panel
  int panel_retval = (*panel_funcs->open)( panel, PANF_BLOCKING );

  // Store the UI state
  GET_INT( limit_to_popup,        limit_to_state       );
  GET_INT( alpha_toggle,          alpha_state          );
  GET_INT( post_save_popup,       post_save_state      );
  GET_INT( save_read_only_toggle, save_read_only_state );
  GET_INT( versioning_popup,      versioning_state     );
  GET_INT( min_digits_int,        min_digits_state     );
  GET_INT( seperator_popup,       seperator_state      );
  GET_STR( path_string,           path_string_state,   MAX_PATH_LENGTH );
  GET_STR( prefix_string,         prefix_string_state, MAX_PATH_LENGTH );
  GET_STR( suffix_string,         suffix_string_state, MAX_PATH_LENGTH );

  return true;
}

// MultiList Callbacks
static char * list_names[]         = { "Save", "Object", "Read Only", "Rev/Backup", NULL };
static int    list_column_widths[] = { 35, 235, 60, 70, 0 };

int ListCount( LWPanelID panel ) {
  return SO_Int->objects.NumElements();
}

char * ListNames( LWPanelID panel, int item, int col ) {
  if( item == -1 ) { // Column Headers
    return list_names[ col ];
  }

  if( item > (int)SO_Int->objects.NumElements() )
    return "";

  switch( col ) {
    case 0:         // Save toggle
      return (SO_Int->objects[ item ]->GetSaveObject() ? SO_Int->checkmark_string : "");

    case 1:         // Item Name
      return ((SO_Int->objects[ item ]->GetFileName() == NULL) ? "" : (char *)SO_Int->objects[ item ]->GetFileName());

    case 2:         // Read Only Flag
      return (SO_Int->objects[ item ]->GetExists() ? (SO_Int->objects[ item ]->GetReadOnly() ? "Read-Only" : "") : "Not Found");

    case 3: {       // Revision/Backup
      int versioning;
      GET_INT( SO_Int->versioning_popup, versioning );
      if( (versioning == SOVM_NEW_REVISION) ||
          (versioning == SOVM_REVISION_BACKUP) ) {
        assert( SO_Int->objects[ item ]->GetLatestRevisionString() != NULL );
        return (char *)SO_Int->objects[ item ]->GetLatestRevisionString();
      } else if( versioning == SOVM_BACKUP ) {
        return (SO_Int->objects[ item ]->DoesBackupExist()    ? "Backed Up" : "" );
      } else if( (versioning == SOVM_DUPLICATE) ||
                 (versioning == SOVM_DUPLICATE_AND_REPLACE) ) {
        return (SO_Int->objects[ item ]->DoesDuplicateExist() ? "Copy Found" : "" );
      } else {
        return "";
      }
    }

    default:
      return "";
  }
}

int ListColumnWidths( LWPanelID panel, int col ) {  // pixel width of column i, up to 8 columns terminate with 0
  return list_column_widths[ col ];
}


// BuildItemList():
bool SaveObjects_Interface::BuildItemList() {
  if( ui->selItems[0] == NULL )
    return false;

  for( unsigned long i=0; ui->selItems[i] != NULL; i++ )
    items.Add( ui->selItems[i] );

  return true;
}

// ReselectItems():
//  Reselects the previously selected items
void SaveObjects_Interface::ReselectItems() {
  (*generic->evaluate)( generic->data, "EditObjects" );

  if( items.NumElements() == 0 )
    return;

  char buffer[100];
  sprintf( buffer, "SelectItem %x", items[ items.NumElements()-1 ] );
  (*generic->evaluate)( generic->data, buffer );

  for( unsigned long i=items.NumElements()-1; i != -1; i-- ) {
    sprintf( buffer, "AddToSelection %x", items[i] );
    (*generic->evaluate)( generic->data, buffer );
  }
}

// FillObjectList():
//  Fills in the object DynArray with a list of items and their
//   details based on the Limit To pop-up state.  Null objects
//   are removed, and multiple object layers and cloned objects
//   are only listed once.
void SaveObjects_Interface::FillObjectList() {
  int limit_mode;
  GET_INT( limit_to_popup, limit_mode );

  objects.Flush();

  int min_revision_digits = 1;

  // List all the objects
  if( limit_mode == 0 ) {         // Current Object
    if( sel_type == LWI_OBJECT )
      objects.Add( new SaveObjects_Object( items[0], objectinfo, min_revision_digits ) );

  } else if( limit_mode == 1 ) {  // Selected Objects
    if( sel_type == LWI_OBJECT ) {
      for( unsigned long i=0; i < items.NumElements(); i++ )
        objects.Add( new SaveObjects_Object( items[i], objectinfo, min_revision_digits ) );
    }

  } else {                        // All Objects
    LWItemID current_item = (*iteminfo->first)( LWI_OBJECT, NULL );
    while( current_item != NULL ) {
      objects.Add( new SaveObjects_Object( current_item, objectinfo, min_revision_digits ) );
      current_item = (*iteminfo->next)( current_item );
    }
  }

  // Update the data for all the objects in the list
  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    // Remove null objects from thel ist
    if( ((*SO_Int->objectinfo->numPoints)(   SO_Int->objects[i]->GetItemID() ) <= 1) &&
        ((*SO_Int->objectinfo->numPolygons)( SO_Int->objects[i]->GetItemID() ) == 0) ) {
      delete SO_Int->objects[i];
      SO_Int->objects.Remove( i );
      i--;
    } else {
      // Remove duplicates from the list
      for( unsigned long j=0; j < i; j++ ) {
        if( stricmp( SO_Int->objects[i]->GetFullPath(), SO_Int->objects[j]->GetFullPath() ) == 0 ) {
          delete SO_Int->objects[i];
          SO_Int->objects.Remove( i );
          j--; i--;
          break;
        }
      }

      if( j == i )
        SO_Int->objects[i]->SetIndex( i );
    }
  }

  HandleAlphaToggle(   alpha_toggle,   NULL );
  HandleRefreshButton( refresh_button, NULL );
}

//
// -- Event Handlers ---------------------------------------------------------
//

// HandleList():
void HandleList( LWControl *con, void *data ) {
  int x = CON_MOUSEX( con );
  int y = CON_MOUSEY( con );
  int index;
  GET_INT( con, index );

  if( x < list_column_widths[0] + 8 )
    SO_Int->objects[ index ]->SetSaveObject( !SO_Int->objects[ index ]->GetSaveObject() );

  REDRAW_CON( con );
}

// HandleLimitToPopup():
void HandleLimitToPopup( LWControl *con, void *data ) {
  int limit_mode;
  GET_INT( SO_Int->limit_to_popup, limit_mode );

  if( limit_mode != SO_Int->limit_to_state ) {
    SO_Int->limit_to_state = limit_mode;
    SO_Int->FillObjectList();
  }
}

// HandleAlphaToggle():
void HandleAlphaToggle( LWControl *con, void *data ) {
  int alpha;
  GET_INT( SO_Int->alpha_toggle, alpha );

  SO_Int->objects.QSort( (alpha ? SortObjectsAlpha : SortObjectsNatural) );

  REDRAW_CON( SO_Int->list );
}

// SortObjectsAlpha():
int __cdecl SortObjectsAlpha( const void *elem1, const void *elem2 ) {
  SaveObjects_Object * obj1 = *((SaveObjects_Object **) elem1);
  SaveObjects_Object * obj2 = *((SaveObjects_Object **) elem2);

  return stricmp( obj1->GetFileName(), obj2->GetFileName() );
}

// SortObjectsNatural():
int __cdecl SortObjectsNatural( const void *elem1, const void *elem2 ) {
  SaveObjects_Object * obj1 = *((SaveObjects_Object **) elem1);
  SaveObjects_Object * obj2 = *((SaveObjects_Object **) elem2);

  return (obj2->GetIndex() - obj1->GetIndex());
}


// HandleRefreshButton():
//  Refreshes the Read Only state of all the items in the list.
void HandleRefreshButton( LWControl *con, void *data ) {
  int versioning_mode, digits;
  GET_INT( SO_Int->versioning_popup, versioning_mode );
  GET_INT( SO_Int->min_digits_int,   digits          );

  char path[ MAX_PATH_LENGTH ], prefix[ MAX_PATH_LENGTH ], suffix[ MAX_PATH_LENGTH ];
  GET_STR( SO_Int->path_string,   path,   MAX_PATH_LENGTH );
  GET_STR( SO_Int->prefix_string, prefix, MAX_PATH_LENGTH );
  GET_STR( SO_Int->suffix_string, suffix, MAX_PATH_LENGTH );

  // Update the data for all the objects in the list
  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    SO_Int->objects[i]->RefreshPath( SO_Int->objectinfo );
    SO_Int->objects[i]->RefreshReadOnly();
    SO_Int->objects[i]->FindBackup();
    SO_Int->objects[i]->FindDuplicate( path, prefix, suffix );

    SO_Int->objects[i]->FindRevision( digits );
    SO_Int->objects[i]->FindLatestRevision( digits, path );
  }

  // Redraw the list
  REDRAW_CON( SO_Int->list );
}

// HandlePickPopdown():
void HandlePickPopdown(  LWControl *con, void *data ) {
  int index;
  GET_INT( con, index );

  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    switch( index ) {
      case 0:         // All
        SO_Int->objects[i]->SetSaveObject( true );
        break;

      case 1:         // Mark Read-Only
        if( SO_Int->objects[i]->GetReadOnly() )
          SO_Int->objects[i]->SetSaveObject( true );
        break;

      case 2:         // Mark Read/Write
        if( !SO_Int->objects[i]->GetReadOnly() )
          SO_Int->objects[i]->SetSaveObject( true );
        break;


      case 4:         // None
        SO_Int->objects[i]->SetSaveObject( false );
        break;

      case 5:         // Unmark Read-Only
        if( SO_Int->objects[i]->GetReadOnly() )
          SO_Int->objects[i]->SetSaveObject( false );
        break;

      case 6:         // Unmark Read/Write
        if( !SO_Int->objects[i]->GetReadOnly() )
          SO_Int->objects[i]->SetSaveObject( false );
        break;


      case 8:         // Invert
        SO_Int->objects[i]->SetSaveObject( !SO_Int->objects[i]->GetSaveObject() );
        break;
    }
  }

  REDRAW_CON( SO_Int->list );
}

// HandleAbout():
void HandleAbout( LWControl *con, void *data ) {
  LWAboutPanel_TMPro( SO_Int->panel_funcs, "SaveObjects", about_text, SO_Int->panel, true );
}

// HandleVersioningPopup():
void HandleVersioningPopup( LWControl *con, void *data ) {
  int index;
  GET_INT( con, index );

  switch( index ) {
    case 0:         // Disable
      GHOST_CON( SO_Int->min_digits_int  );
      GHOST_CON( SO_Int->seperator_popup );
      GHOST_CON( SO_Int->path_string     );
      GHOST_CON( SO_Int->prefix_string   );
      GHOST_CON( SO_Int->suffix_string   );
      break;

    case 1:         // Backup Original
      GHOST_CON( SO_Int->min_digits_int  );
      GHOST_CON( SO_Int->seperator_popup );
      GHOST_CON( SO_Int->path_string     );
      GHOST_CON( SO_Int->prefix_string   );
      GHOST_CON( SO_Int->suffix_string   );
      break;

    case 2:         // Increment Revision
      UNGHOST_CON( SO_Int->min_digits_int  );
      UNGHOST_CON( SO_Int->seperator_popup );
      GHOST_CON(   SO_Int->path_string     );
      GHOST_CON(   SO_Int->prefix_string   );
      GHOST_CON(   SO_Int->suffix_string   );
      break;

    case 3:         // Revision Backup
      UNGHOST_CON( SO_Int->min_digits_int  );
      UNGHOST_CON( SO_Int->seperator_popup );
      UNGHOST_CON( SO_Int->path_string     );
      GHOST_CON(   SO_Int->prefix_string   );
      GHOST_CON(   SO_Int->suffix_string   );
      break;

    case 4:         // Duplicate
    case 5:         // Duplicate And Replace
      GHOST_CON(   SO_Int->min_digits_int  );
      GHOST_CON(   SO_Int->seperator_popup );
      UNGHOST_CON( SO_Int->path_string     );
      UNGHOST_CON( SO_Int->prefix_string   );
      UNGHOST_CON( SO_Int->suffix_string   );
      break;
  }

  REDRAW_CON( SO_Int->list );
}

// HandleMinDigitsInt():
void HandleMinDigitsInt( LWControl *con, void *data ) {
  int versioning_mode, digits;
  GET_INT( SO_Int->versioning_popup, versioning_mode );
  GET_INT( SO_Int->min_digits_int,   digits          );
  GET_STR( SO_Int->path_string, SO_Int->path_string_state, MAX_PATH_LENGTH );

  // Check the range
  if( digits < 1 )
    digits = 1;

  if( digits > 8 )
    digits = 8;

  SET_INT( SO_Int->min_digits_int, digits );

  // Update the objects
  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    SO_Int->objects[i]->FindRevision( digits );
    SO_Int->objects[i]->FindLatestRevision( digits, SO_Int->path_string_state );
  }

  REDRAW_CON( SO_Int->list );
}

// HandlePathString():
void HandlePathString( LWControl *con, void *data ) {
  GET_STR( SO_Int->path_string,   SO_Int->path_string_state,   MAX_PATH_LENGTH );
  GET_STR( SO_Int->prefix_string, SO_Int->prefix_string_state, MAX_PATH_LENGTH );
  GET_STR( SO_Int->suffix_string, SO_Int->suffix_string_state, MAX_PATH_LENGTH );

  int versioning_mode, min_digits;
  GET_INT( SO_Int->versioning_popup, versioning_mode );
  GET_INT( SO_Int->min_digits_int,   min_digits      );

  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    SO_Int->objects[i]->FindDuplicate( SO_Int->path_string_state,
                                       SO_Int->prefix_string_state,
                                       SO_Int->suffix_string_state );

    if( versioning_mode == SOVM_REVISION_BACKUP )
      SO_Int->objects[i]->FindLatestRevision( min_digits, SO_Int->path_string_state );
  }

  REDRAW_CON( SO_Int->list );
}

// HandleSaveMarkedObjects():
void HandleSaveMarkedObjects( LWControl *con, void *data ) {
  // Make sure there's at least one item
  if( SO_Int->objects.NumElements() == 0 ) {
    SO_Int->PostError( "Error:  No objects; nothing to do",
                       "SaveObjects Error:  There are no objects to save" );
    return;
  }

  // Make sure at least one object is marked for saving
  int save_read_only;
  GET_INT( SO_Int->save_read_only_toggle, save_read_only );

  SaveObjects_ProcessData process_data( SO_Int->generic, (save_read_only ? true : false) );
  process_data.message = SO_Int->message;
  GET_INT( SO_Int->post_save_popup,  process_data.post_save_mode  );
  GET_INT( SO_Int->versioning_popup, process_data.versioning_mode );
  GET_INT( SO_Int->min_digits_int,   process_data.min_digits      );
  GET_INT( SO_Int->seperator_popup,  process_data.seperator       );
  GET_STR( SO_Int->path_string,      process_data.path,   MAX_PATH_LENGTH );
  GET_STR( SO_Int->prefix_string,    process_data.prefix, MAX_PATH_LENGTH );
  GET_STR( SO_Int->suffix_string,    process_data.suffix, MAX_PATH_LENGTH );

  if( process_data.versioning_mode == SOVM_DUPLICATE ) {
    if( (process_data.path[0]   == '\0') &&
        (process_data.prefix[0] == '\0') &&
        (process_data.suffix[0] == '\0') ) {
      SO_Int->PostError( "Error:  Duplicate Path, Prefix and Suffix are empty",
                         "SaveObjects Error:  Duplicate Path, Prefix and Suffix are empty.",
                         "At least one must be filled in to avoid saving over the source object." );
      return;
    }
  }

  bool at_least_one = false;
  for( unsigned long i=0; i < SO_Int->objects.NumElements(); i++ ) {
    if( SO_Int->objects[i]->GetSaveObject() ) {
      at_least_one = true;
      if( save_read_only && ((process_data.versioning_mode == SOVM_BACKUP) ||
                             (process_data.versioning_mode == SOVM_REVISION_BACKUP)) ) {
        break;
      } else {
        if( (process_data.versioning_mode == SOVM_NEW_REVISION) ||
            (process_data.versioning_mode == SOVM_DUPLICATE) ||
            (process_data.versioning_mode == SOVM_DUPLICATE_AND_REPLACE) ) {
          break;
        }

        if( SO_Int->objects[i]->GetReadOnly() && process_data.overwrite_read_only )
          break;

        if( !SO_Int->objects[i]->GetReadOnly() )
          break;
      }
    }
  }

  if( i == SO_Int->objects.NumElements() ) {
    if( at_least_one ) {
      SO_Int->PostError( "Error:  All objects are Read-Only; nothing to do",
                         "SaveObjects Error:  All marked objects are Read-Only and",
                         "\"Save Read-Only Objects\" is disabled; nothing to do" );
    } else {
      SO_Int->PostError( "Error:  No objects marked for saving",
                         "SaveObjects Error:  No objects have been marked for saving.  Click the \"Save\"",
                         "column of the object list or use the \"Mark Objects To Save\" pop-up and try again." );
    }
    return;
  }

  // Set up some variables
  GHOST_CON( con );
  int   items_saved = 0;

  // Loop through and save each object
  for( i=0; i < SO_Int->objects.NumElements(); i++ ) {
    // Skip Unmarked Objects
    if( !SO_Int->objects[i]->GetSaveObject() )
      continue;

    // Save the object
    SO_Int->objects[i]->SaveObject( &process_data );

    // Update the progress bar
    SO_Int->progress = ((float)i)/((float)SO_Int->objects.NumElements() - 1);
    sprintf( SO_Int->progress_string, "Saving \"%s\"...", SO_Int->objects[i]->GetFileName() );
    DrawProgressField( SO_Int->progress_field, NULL, DR_RENDER );

    items_saved++;
  }

  SO_Int->progress = 1.0f;
  sprintf( SO_Int->progress_string, "Finished Saving %d Objects", items_saved );
  DrawProgressField( SO_Int->progress_field, NULL, DR_RENDER );

  UNGHOST_CON( con );

  HandleRefreshButton( SO_Int->refresh_button, NULL );
}

// PostError():
void SaveObjects_Interface::PostError( const char *progress_error,
                                       const char *buffer1, const char *buffer2 ) {
  progress = -1.0f;
  if( progress_string != NULL )
    strcpy( progress_string, progress_error );

  DrawProgressField( progress_field, NULL, DR_RENDER );

  if( buffer1 != NULL )
    (*message->error)( buffer1, buffer2 );
}

// DrawProgressField():
void DrawProgressField( LWControl *con, void *data, DrMode mode ) {
  int cx = CON_HOTX( con );
  int cy = CON_HOTY( con );
  int cw = CON_HOTW( con );
  int ch = CON_HOTH( con );

  DrawFuncs *df = SO_Int->panel_funcs->drawFuncs;

  if( SO_Int->progress < 0.0f ) {
    // Errors
    (*df->drawRGBBox)( SO_Int->panel, 175, 0, 0, cx, cy + 1, cw, ch-2 );
    (*df->drawText)( SO_Int->panel, SO_Int->progress_string, COLOR_LT_YELLOW, cx + 3, cy + 2 );
  } else {
    // Progress Bars/Status
    (*df->drawRGBBox)( SO_Int->panel, 132, 130, 132, cx, cy + 1, cw, ch-2 );

    // Draw the progress bar
    (*df->drawRGBBox)( SO_Int->panel, 0, 60, 150, cx+1, cy+1,
                      (int)((cw-2) * SO_Int->progress), ch-2 );

    // Write some text
    (*df->drawText)( SO_Int->panel, SO_Int->progress_string, COLOR_WHITE, cx + 3, cy + 2 );
  }
}

