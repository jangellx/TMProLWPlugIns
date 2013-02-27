//
// LWAboutPanel.cpp
//

#include "LWAboutPanel.h"

#include <stdio.h>

// Constructor
LWAboutPanel::LWAboutPanel( LWPanelFuncs *_panel_funcs,
                            const char *title, const char **_lines,
                            CSourceData *_logo,
                            LWPanelID _parent, bool open,
                            const char **_keys )
  : panel_funcs(_panel_funcs), lines(_lines), logo(_logo), panel(NULL),
    parent(_parent), keys(_keys), current_tab(0) {

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

  int logo_width  = 0;
  int logo_height = 0;

  if( logo != NULL ) {
    logo_width  = logo->GetWidth();
    logo_height = logo->GetHeight();
  }

  // Figure out the panel's size
  text_top = (keys == NULL) ? 0 : 25;

  int panel_height = height;
  if( (height + text_top) < (logo_height + text_top) ) {
    text_top += ((logo_height + 6) - height)/2;
    panel_height = logo_height;
  } else {
    text_top = 10;
  }

  // Add some controls
  static const char *tab_strings[] = {"About", "Keys", NULL};

  int panel_width = (width + logo_width + 20);
  if( keys != NULL ) {
    tabs = TABCHOICE_CTL( panel_funcs, panel, "", tab_strings );
    MOVE_CON( tabs, logo_width + 10, 0 );

    ival.intv.value = (int)200;
    (*tabs->set)( tabs, CTL_W, &ival );

    int y = CON_H( tabs );
    if( ((height + y) - panel_height) > 0 )
      panel_height = height + y;

    key_lister = MULTILIST_CTL( panel_funcs, panel, "Keys",
                                panel_width - logo_width,
                                (panel_height - y - (metrics->textHeight * 3))/metrics->textHeight,
                                AboutPanelKeyListNames,
                                AboutPanelKeyListCount,
                                AboutPanelKeyListColumnWidths );
    PanelTools::PutUnder( tabs, key_lister );
    y = CON_Y( key_lister );
    MOVE_CON( key_lister, logo_width + 10, y );
    GHOST_CON( key_lister );
    ERASE_CON( key_lister );

    CON_SETEVENT( tabs,       HandleAboutPanelTabs, this );
    CON_SETEVENT( key_lister, NULL,                 this );
  }

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
LWAboutPanel::~LWAboutPanel() {
  if( panel != NULL )
    (panel_funcs->destroy)( panel );
}

// DrawAboutPanel() {
void DrawAboutPanel( LWPanelID panel, LWAboutPanel *data, DrMode mode ) {
  CDrawFuncs df( data->panel_funcs->drawFuncs );
  const LWDisplayMetrics * metrics = df.DispMetrics();

  if( data->logo != NULL )
    data->logo->DrawInLWPanels( data->panel, &df, 3, 3 );

  if( data->current_tab != 0 )
    return;

  int ph = PAN_GETH( data->panel_funcs, data->panel );
  int pw = PAN_GETW( data->panel_funcs, data->panel );

  int logo_width = 0;
  if( data->logo != NULL )
    logo_width  = data->logo->GetWidth();

  int text_left   = logo_width + 10;
  int text_width  = pw - logo_width - 20;
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
LWAboutPanel::Open() {
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

  if( panel_w > parent_w ) {
    int temp = panel_w;
    panel_w = parent_w;
    parent_w = temp;
  }

  if( panel_h > parent_h ) {
    int temp = panel_h;
    panel_h = parent_h;
    parent_h = temp;
  }

  MOVE_PAN( panel_funcs, panel, parent_x + (parent_w/2 - panel_w/2),
                                parent_y + (parent_h/2 - panel_h/2) );

  // Open the panel
  (*panel_funcs->open)( panel, PANF_PASSALLKEYS );

  MOVE_PAN( panel_funcs, panel, parent_x + (parent_w/2 - panel_w/2),
                                parent_y + (parent_h/2 - panel_h/2) );

  (*panel_funcs->handle)( panel, EVNT_BLOCKING | EVNT_ALL );
}

// HandleKeys():
void AboutPanelKeys( LWPanelID panel, LWAboutPanel *data, LWDualKey key ) {
  switch( key ) {
    case '\r':      // Close Panel
      (*data->panel_funcs->close)( data->panel );
      return;
  }
}

// HandleAboutPanelTabs():
void HandleAboutPanelTabs( LWControl *con, void *_data ) {
  LWAboutPanel *data = (LWAboutPanel *)_data;
  GET_INT( data->tabs, data->current_tab );

  if( data->current_tab != 0 ) {
    UNGHOST_CON( data->key_lister );
    REDRAW_CON( data->key_lister );
    (*data->panel_funcs->draw)( data->panel, DR_REFRESH );
  } else {
    GHOST_CON( data->key_lister );
    ERASE_CON( data->key_lister );
    (*data->panel_funcs->draw)( data->panel, DR_REFRESH );
  }
}

// MultiList Callbacks
char * about_panel_list_names[]         = { "Key(s)", "Command", NULL };

int AboutPanelKeyListCount( void *_data ) {
  if( _data == NULL )
    return 0;

  LWAboutPanel *data = (LWAboutPanel *)_data;

  for( int i=0; data->keys[i] != NULL; i++ ) { ; }
  assert( (i % 2) == 0 );
  data->key_count = i/2 + 1;

  return data->key_count;
}

char * AboutPanelKeyListNames( void *_data, int item, int col ) {
  if( _data == NULL )
    return "";

  LWAboutPanel *data = (LWAboutPanel *)_data;

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
    LWAboutPanel *data = (LWAboutPanel *)_data;
    int w = CON_W( data->key_lister );
    return w - 130;
  } else {
    return 0;
  }
}
