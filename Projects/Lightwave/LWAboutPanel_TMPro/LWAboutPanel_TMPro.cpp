//
// LWAboutPanel_TMPro.cpp
//

#include "LWAboutPanel_TMPro.h"

#include <stdio.h>

// Constructor
LWAboutPanel_TMPro::LWAboutPanel_TMPro( LWPanelFuncs *_panel_funcs,
                                        const char *title, const char **_lines,
                                        LWPanelID _parent, bool open,
                                        const char **_keys, bool keyed )
  : panel_funcs(_panel_funcs), lines(_lines), panel(NULL),
    parent(_parent), keys(_keys), current_tab(0), return_true_count(false),
    TABID_ABOUT(0), TABID_KEYS(-1), TABID_DONGLE(-1), key_lister(NULL), license(NULL) {

  tmpro_logo = new TMPro_MiniLogo;

  // Create the panel
  char title_string[256];
  sprintf( title_string, "About %s", title );
  panel = (*panel_funcs->create)( title_string, panel_funcs );

  CDrawFuncs df( panel_funcs->drawFuncs );
  const LWDisplayMetrics * metrics = df.DispMetrics();

  // Figure out the panel size
  int temp_width, height=0, width = 0;
  for( unsigned long i=0; lines[i] != NULL; i++ ) {
    if( stricmp( lines[i], LWABOUT_HDIV ) == 0 ) {          // HDIV:   Horizontal divider; +5 to line size
      height += metrics->textHeight + 5;
    } else if( stricmp( lines[i], LWABOUT_VTAB ) == 0 ) {   // VTAB:   Just make an empty line
      height += metrics->textHeight;
    } else if( stricmp( lines[i], LWABOUT_YELLOW ) == 0 ) { // YELLOW: Doesn't affect panel height
      continue;
    } else if( stricmp( lines[i], LWABOUT_BLACK ) == 0 ) {  // BLACK:  Doesn't affect panel height
      continue;
    } else {
      // Text
      temp_width = df.TextWidth( panel, (char *)lines[i] );
      if( temp_width > width )
        width = temp_width;

      height += metrics->textHeight;
    }
  }

  // Figure out the panel's size
  text_top = 10;
  int panel_height = height;
  if( (height + text_top) < tmpro_logo->GetHeight() ) {
    text_top = ((tmpro_logo->GetHeight() + 6) - (height + text_top))/2;
    panel_height = tmpro_logo->GetHeight();
  }

  // Set up the tabs
  static const char *tab_strings[4] = { "About", NULL };
  if( keys != NULL ) {
    TABID_KEYS = 1;
    tab_strings[ 1 ] = "Keys";
    tab_strings[ 2 ] = NULL;
  }

  if( keyed ) {
    TABID_DONGLE = (TABID_KEYS == -1) ? 1 : 2;
    tab_strings[ TABID_DONGLE ] = "Dongle";
    tab_strings[ TABID_DONGLE + 1 ] = NULL;
  }

  // Add some controls
  int panel_width = (width  + tmpro_logo->GetWidth() + 20);
  int page_top = 5;

  // - Tabs
  if( (TABID_KEYS != -1) || (TABID_DONGLE != -1) ) {
    tabs = TABCHOICE_CTL( panel_funcs, panel, "", tab_strings );
    MOVE_CON( tabs, tmpro_logo->GetWidth() + 10, 0 );

    ival.intv.value = (int)200;
    (*tabs->set)( tabs, CTL_W, &ival );

    page_top = CON_H( tabs );
    CON_SETEVENT( tabs, HandleAboutPanelTabs, this );
  }

  // - Keys
  if( TABID_KEYS != -1 ) {
    key_lister = MULTILIST_CTL( panel_funcs, panel, "Keys",
                                panel_width - tmpro_logo->GetWidth(),
                                (panel_height - page_top - (metrics->textHeight * 3))/metrics->textHeight,
                                AboutPanelKeyListNames,
                                AboutPanelKeyListCount,
                                AboutPanelKeyListColumnWidths );
    PanelTools::PutUnder( tabs, key_lister );
    int y = CON_Y( key_lister );
    MOVE_CON( key_lister, tmpro_logo->GetWidth() + 10, y );
    GHOST_CON( key_lister );
    ERASE_CON( key_lister );

    CON_SETEVENT( key_lister, NULL, this );
  }

  // - Dongle
  if( TABID_DONGLE != -1 ) {
    license = STR_CTL( panel_funcs, panel, "License", 30 );
    MOVE_CON( license, tmpro_logo->GetWidth() + 10, 80 );
    CON_SETEVENT( license, NULL, this );

    ERASE_CON( license );
  }

  if( ((height + page_top) - panel_height) > 0 )
    panel_height = height + page_top;

  PAN_SETW( panel_funcs, panel, panel_width );
  PAN_SETH( panel_funcs, panel, (panel_height + 45) );

  // Set some callbacks
  (*panel_funcs->set)( panel, PAN_USERDATA, this           );
  (*panel_funcs->set)( panel, PAN_USERDRAW, DrawAboutPanel );
  (*panel_funcs->set)( panel, PAN_USERKEYS, AboutPanelKeys );

  if( open )
    Open();
}

// Destructor
LWAboutPanel_TMPro::~LWAboutPanel_TMPro() {
  if( panel != NULL )
    (panel_funcs->destroy)( panel );

  if( tmpro_logo != NULL )
    delete tmpro_logo;
}

// DrawAboutPanel() {
void DrawAboutPanel( LWPanelID panel, LWAboutPanel_TMPro *data, DrMode mode ) {
  CDrawFuncs df( data->panel_funcs->drawFuncs );
  const LWDisplayMetrics * metrics = df.DispMetrics();
  data->tmpro_logo->DrawInLWPanels( data->panel, &df, 3, 3 );

  if( data->current_tab != 0 )
    return;

  int ph = PAN_GETH( data->panel_funcs, data->panel );
  int pw = PAN_GETW( data->panel_funcs, data->panel );

  int text_left   = data->tmpro_logo->GetWidth() + 10;
  int text_width  = pw - data->tmpro_logo->GetWidth() - 20;
  int line_height = metrics->textHeight;
  int line_pos    = data->text_top;
  int color       = COLOR_BLACK;

  int line_width;
  for( unsigned long i=0; data->lines[i] != NULL; i++ ) {
    if( stricmp( data->lines[i], LWABOUT_HDIV ) == 0 ) {          // HDIV
      PanelDraw::HorizontalDivider( data->panel, &df, text_left + 10, line_pos + 8, text_width - 20, 1 );
      line_pos += line_height + 5;
    } else if( stricmp( data->lines[i], LWABOUT_VTAB ) == 0 ) {   // VTAB:  Just make an empty line
      line_pos += line_height;
    } else if( stricmp( data->lines[i], LWABOUT_YELLOW ) == 0 ) { // YELLOW
      color = COLOR_LT_YELLOW;
    } else if( stricmp( data->lines[i], LWABOUT_BLACK ) == 0 ) {  // BLACK
      color = COLOR_BLACK;
    } else {                                                      // Text
      line_width = df.TextWidth( data->panel, (char *)data->lines[i] );

      df.WriteText( data->panel, (char *)data->lines[i], color,
                   text_left + (text_width/2) - (line_width/2), line_pos );
      line_pos += line_height;
    }
  }
}


// Open():
void LWAboutPanel_TMPro::Open( bool request_key ) {
  int parent_x, parent_y, parent_w, parent_h, panel_w, panel_h;
  if( parent == NULL ) {
    CDrawFuncs df( panel_funcs->drawFuncs );
    const LWDisplayMetrics * metrics = df.DispMetrics();

    parent_x = 0;
    parent_y = 0;
    parent_w = metrics->width;
    parent_h = metrics->height;
  } else {
    // Center the panel in it's parent's window
    parent_x = PAN_GETX( panel_funcs, parent );
    parent_y = PAN_GETY( panel_funcs, parent );
    parent_w = PAN_GETW( panel_funcs, parent );
    parent_h = PAN_GETH( panel_funcs, parent );
  }

  panel_w  = PAN_GETW( panel_funcs, panel );
  panel_h  = PAN_GETH( panel_funcs, panel );

  int center_x = parent_w/2 + parent_x;
  int center_y = parent_h/2 + parent_y;
  int new_x    = center_x - panel_w/2;
  int new_y    = center_y - panel_h/2;

  MOVE_PAN( panel_funcs, panel, new_x, new_y );

  // Open the panel
  (*panel_funcs->open)( panel, PANF_PASSALLKEYS );

  MOVE_PAN( panel_funcs, panel, new_x, new_y );

  // Work around a bug where the last item in the list
  //  won't be drawn the first time the count function
  //  is called.
  return_true_count = true;

  if( request_key ) {
    SET_INT( tabs, TABID_DONGLE );
  }

  (*panel_funcs->handle)( panel, EVNT_BLOCKING | EVNT_ALL );
}

// HandleKeys():
void AboutPanelKeys( LWPanelID panel, LWAboutPanel_TMPro *data, LWDualKey key ) {
  switch( key ) {
    case '\r':      // Close Panel
      (*data->panel_funcs->close)( data->panel );
      return;
  }
}

// HandleAboutPanelTabs():
void HandleAboutPanelTabs( LWControl *con, void *_data ) {
  LWAboutPanel_TMPro *data = (LWAboutPanel_TMPro *)_data;
  GET_INT( data->tabs, data->current_tab );

  if( data->current_tab == data->TABID_KEYS ) {
    if( data->license != NULL ) {
      ERASE_CON( data->license );
    }

    UNGHOST_CON( data->key_lister );
    REDRAW_CON( data->key_lister );
    (*data->panel_funcs->draw)( data->panel, DR_REFRESH );

  } else if( data->current_tab == data->TABID_DONGLE ) {
    if( data->key_lister != NULL ) {
      ERASE_CON( data->key_lister );
    }

    (*data->panel_funcs->draw)( data->panel, DR_REFRESH );

  } else {
    if( data->key_lister != NULL ) {
      ERASE_CON( data->key_lister );
    }

    if( data->license != NULL ) {
      ERASE_CON( data->key_lister );
    }
    (*data->panel_funcs->draw)( data->panel, DR_REFRESH );
  }
}

// MultiList Callbacks
char * about_panel_list_names[] = { "Key(s)", "Command", NULL };

int AboutPanelKeyListCount( void *_data ) {
  if( _data == NULL )
    return 0;

  LWAboutPanel_TMPro *data = (LWAboutPanel_TMPro *)_data;

  // Work around a bug where the last item in the list
  //  won't be drawn the first time this function returns.
  if( data->return_true_count == false )
    return 0;

  for( int i=0; data->keys[i] != NULL; i++ ) { ; }
  assert( (i % 2) == 0 );
  data->key_count = i/2;

  return data->key_count;
}

char * AboutPanelKeyListNames( void *_data, int item, int col ) {
  if( _data == NULL )
    return "";

  LWAboutPanel_TMPro *data = (LWAboutPanel_TMPro *)_data;

  if( item == -1 )  // Column Headers
    return about_panel_list_names[ col ];

  if( item < data->key_count )
    return (char *)data->keys[ (item * 2) + col ];

  return "";
}

int AboutPanelKeyListColumnWidths( void *_data, int col ) {
  if( col == 0 ) {
    return 100;
  } else if( col == 1) {
    LWAboutPanel_TMPro *data = (LWAboutPanel_TMPro *)_data;
    int w = CON_W( data->key_lister );
    return w - 140;
  } else {
    return 0;
  }
}
