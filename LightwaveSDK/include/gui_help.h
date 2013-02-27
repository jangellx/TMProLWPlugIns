// Gui_Help.h -- prototypes for utility functions

#pragma once

int maxLabel(LWPanelFuncs *,LWPanelID );            // ??
int maxWidth(LWPanelFuncs *,LWPanelID );            // ??
void ctlCenter(int ,LWControl *);                   // Center X/Y?
void ctlVAlign(int ,LWControl *);                   // Align controls vertically (first control marks top?)
void ctlStack(LWControl *,LWControl *);             // Stack controls (vertically?)
void ctlOneLine(LWControl *,LWControl *,LWControl *,LWControl *,LWControl *,LWControl *);  // Put all controls on the same line (line start designated by first control?)
void ctlOutLine(LWControl *,LWControl *,LWControl *,LWControl *,LWControl *,LWControl *);  // ???
void ctlMarkUp(LWControl *);                        // Move control up a notch?
void panelMarkUp(LWPanelFuncs *, LWPanelID);        // Move panel up a notch?

/*
 * Contributed by Steve Day / Linear Designs
 * 8th May 1998
 */

#define VALIGN_TOP  0
#define VALIGN_MIDDLE 1
#define VALIGN_BOTTOM 2

void ctrlAlignH(int, LWControl *,LWControl *,LWControl *,LWControl *,LWControl *,LWControl *);  // Align controls horizontally (first control marks left edge?)
