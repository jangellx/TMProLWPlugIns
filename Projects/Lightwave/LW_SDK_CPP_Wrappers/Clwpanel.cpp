// Clwpanel.cpp
// 
// This file contains the class definitions for the CLWPANEL classes.
// Basically, it just uses the contructor to convert the function
// pointers from the structure provided into function references,
// converts and data member pointers into references to those
// pointers (functionally, they work the same as the did in the
// struct), and adds some accessor functions along the way.

#include "Clwpanel.h"

// For convenience, the following code is added in this file.
// It is required by macros in lwpanel.h.  These variables
// are declared extern in LWWrappers.h.  Once that is included
// in your executable, you'll have full access to these vairables.

LWPanControlDesc   desc;
LWValue ival    = {LWT_INTEGER};
LWValue ivecval = {LWT_VINT};
LWValue fval    = {LWT_FLOAT};
LWValue fvecval = {LWT_VFLOAT};
LWValue sval    = {LWT_STRING};

// CLWControl
CLWControl::CLWControl( LWControl *control)
  : Draw(        *control->draw ),
    Get(         *control->get  ),
    Set(         *control->set  ),
    priv_data(    control->priv_data ),
    orig_struct(  control ) {
  ;
}

CLWControl * CLWControl::New( LWControl *control ) {
  if( control == NULL )
    return NULL;
  else
    return new CLWControl( control );
}

// CDrawFuncs
CDrawFuncs::CDrawFuncs( DrawFuncs *draw_funcs)
  : DrawPixel(    *draw_funcs->drawPixel    ),
    DrawRGBPixel( *draw_funcs->drawRGBPixel ),
    DrawLine(     *draw_funcs->drawLine     ),
    DrawBox(      *draw_funcs->drawBox      ),
    DrawRGBBox(   *draw_funcs->drawRGBBox   ),
    DrawBorder(   *draw_funcs->drawBorder   ),
    TextWidth(    *draw_funcs->textWidth    ),
    DrawText(     *draw_funcs->drawText     ),
    WriteText(    *draw_funcs->drawText     ),
    DispMetrics(  *draw_funcs->dispMetrics  ),
    orig_struct(   draw_funcs ) {
  ;
}

CDrawFuncs * CDrawFuncs::New( DrawFuncs *draw_funcs ) {
  if( draw_funcs == NULL )
    return NULL;
  else
    return new CDrawFuncs( draw_funcs );
}

// CLWPanelFuncs
CLWPanelFuncs::CLWPanelFuncs( LWPanelFuncs *panel_funcs,
                              GlobalFunc   *global )
  : Destroy(     *panel_funcs->destroy     ),
    Open(        *panel_funcs->open        ),
    Handle(      *panel_funcs->handle      ),
    Draw(        *panel_funcs->draw        ),
    Close(       *panel_funcs->close       ),
    Get(         *panel_funcs->get         ),
    Set(         *panel_funcs->set         ),
    AddControl(  *panel_funcs->addControl  ),
    NextControl( *panel_funcs->nextControl ),
    user_data(    panel_funcs->user_data   ),
    Global(      *global                   ),
    orig_struct(  panel_funcs ) {
  
  DrawFuncs = new CDrawFuncs( panel_funcs->drawFuncs );
  panel_funcs->globalFun = global;
}

CLWPanelFuncs::~CLWPanelFuncs() {
  delete DrawFuncs;
}

CLWPanelFuncs * CLWPanelFuncs::New( LWPanelFuncs *panel_funcs,
                                    GlobalFunc   *global ) {
  if( panel_funcs == NULL )
    return NULL;
  else
    return new CLWPanelFuncs( panel_funcs, global );
}

