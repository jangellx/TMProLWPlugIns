
// Clwpanel.h
//
// This file contains a series of classes which encapsulate the structures
// used by the Lightwave SDK.  It's purpose is to make those wacky function
// pointers into function references, so they can be used just like any other
// function.  Otherwise, they're the same.
//
// This file encapsulates the structures found lwpanel.h
//
// Currently featured in this file:
//
//    C++ Class                  LW Structure
//   CLWControl                 LWControl
//   CDrawFuncs                 DrawFuncs
//   CLWPanelFuncs              LWPanelFuncs
//

#pragma once

#include <stdlib.h>

#ifdef LW_SDK_CPP_WRAPPERS_L6_HEADER
  #include <lwpanel.h>
  #define DisplayMetrics LWDisplayMetrics
#else 
  #include <lwbase.h>
  #include <lwpanel5.h>
#endif

// Classes
class CLWControl;
class CDrawFuncs;
class CLWPanelFuncs;

// CLWControl
class CLWControl {
public:
  CLWControl( LWControl *control );

  // Use New() to create a new instance of this class
  static CLWControl * CLWControl::New( LWControl *control );

	void		(&Draw) (LWControlID, DrMode);           // Tell the control to draw
	void		(&Get)  (LWControlID, cTag, LWValue *);  // Get a tag from a control
	void		(&Set)  (LWControlID, cTag, LWValue *);  // Set a tag in a control

public:  
	void	* &priv_data;	

  LWControl * orig_struct;  // The original structure used to create this class
};

// CDrawFuncs
class CDrawFuncs {
public:
  CDrawFuncs( DrawFuncs *draw_funcs );

  // Use New() to create a new instance of this class
  static CDrawFuncs * CDrawFuncs::New( DrawFuncs *control );

	void	                 (&DrawPixel)    (LWPanelID, int, int, int);	 	                 	// color,x,y
	void	                 (&DrawRGBPixel) (LWPanelID, int, int, int, int, int);        	  // r,g,b,x,y
	void	                 (&DrawLine)     (LWPanelID, int, int, int, int, int);          	// color,x,y,x2,y2
	void	                 (&DrawBox)      (LWPanelID, int, int, int, int, int);         	  // color,x,y,w,h
	void	                 (&DrawRGBBox)   (LWPanelID, int, int, int, int, int, int, int);	// r,g,b,x,y,w,h
	void	                 (&DrawBorder)   (LWPanelID, int, int, int, int, int);            // indent,x,y,w,h		(h==0 -> divider)
	int	                   (&TextWidth)    (LWPanelID, char *);                             // text
	void	                 (&DrawText)     (LWPanelID, char *, int, int, int);              // text,color,x,y
	void	                 (&WriteText)    (LWPanelID, char *, int, int, int);              // text,color,x,y
	const DisplayMetrics * (&DispMetrics)();

  DrawFuncs * orig_struct;  // The original structure used to create this class
};

// CLWPanelFuncs
class CLWPanelFuncs {
public:
  CLWPanelFuncs( LWPanelFuncs *panel_funcs,
                 GlobalFunc   *global );

  // Use New() to create a new instance of this class
  static CLWPanelFuncs * CLWPanelFuncs::New( LWPanelFuncs *panel_funcs,
                                             GlobalFunc   *global );
  ~CLWPanelFuncs();

public:
	LWPanelID	    Create(char *title) {                    // Create a panel with orig_struct as the user data
    return( (*orig_struct->create)( title, orig_struct ) ); }
	LWPanelID	    Create(char *title, void *_user_data) {  // Create a panel with user-defined user data
    return( (*orig_struct->create)( title, _user_data ) ); }
	void			  (&Destroy)(LWPanelID);           // Destroy a panel
	int			    (&Open)(LWPanelID,int);	         // Open a previously craeted panel.  flags int (see flag bits  PANF_ etc.)
	int			    (&Handle)(LWPanelID, int);	     // Handle event for a panel.  process input manually after non-blocking open, use EVNT_ bits to process async. or synchronously
	void			  (&Draw)(LWPanelID, DrMode);      // Draw a panel
	void			  (&Close)(LWPanelID);             // Close a panel (does not destroy)
	void			  (&Get)(LWPanelID, pTag, void *); // Set a value in a panel
	void			  (&Set)(LWPanelID, pTag, void *); // Get a value in a panel
	LWControl	* (&AddControl)(LWPanelID pan, char *type, ControlDesc *data, char *label);  // Add a control to a panel
	LWControl	* (&NextControl)(LWPanelID pan, LWControlID ctl); // Go to the next control in  a panel

public:
	CDrawFuncs  * DrawFuncs;                     // Drawing Functions

	void			  * &user_data;	                   // do whatcha like
	GlobalFunc	  &Global;	                     // please set this to your GlobalFunction pointer.. at least for file req.

  LWPanelFuncs * orig_struct;  // The original structure used to create this class
};
