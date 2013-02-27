//
// FreeKeyInterface-Options.cpp
//

#include "FreeKey-Interface.h"

extern FreeKeyInterface *fk_int;
extern FreeKey          *fk;

// OnOptDrawStyle():
//  Called to change the draw style of the item list
void OnOptDrawStyle(LWControl *ctl, FreeKeyInterface *panel) {
  int style;
  GET_INT( fk_int->opt_draw_style, style );
  fk->SetDrawStyle( (fk_drawstyle)style );

  fk_int->RedrawItemList();
}

// OnOptUseOpenGLItemList():
//  Called to change the control used for the item list
void OnOptUseOpenGLItemList(LWControl *ctl, FreeKeyInterface *panel) {
  int style;
  GET_INT( fk_int->opt_use_opengl_item_list, style );
  fk->SetUseOpenGLItemList( (bool)style );

  fk_int->UpdateItemListType( (bool)style );

  fk_int->RedrawItemList();
}

// OnOptUseOpenGLLegacy():
//  Called to change some minor pixel offsets in the OpenGL item list
void OnOptUseOpenGLLegacy(LWControl *ctl, FreeKeyInterface *panel) {
  int state;
  GET_INT( fk_int->opt_use_opengl_legacy, state );

  GLTLWIcons::SetLegacyOffset( (bool) state );

  fk_int->RedrawItemList();
}
