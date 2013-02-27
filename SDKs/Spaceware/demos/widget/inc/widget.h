/*  widget.h - Include file with data structs for Widget World demo
 *
 *  Written:  February 1996
 *  Author:   Jeff LeBlanc
 *
 * $Id: widget.h,v 1.3 1996/11/19 19:35:47 jeffl Exp $
 *
 *  This file contains all the data structure definitions to read in
 *  and display a simple geometry made up of points connected by edges
 *  that in turn for polygons.  Also, control structures are defined to
 *  encapsulate the standard Spaceball integration flags
 */

#ifndef _WIDGET_H
#define _WIDGET_H

#define ELIST_MAX 10       /* Maximum number of edges per polygon */

#define SPW_TRANS       1
#define SPW_SINGLE_AXIS 3

#include "si.h"
/*
 * Structure definition for Points
 */

typedef struct {          
  float Coord[3];          /* List of Point coordinates (x, y, z) */ 
} PointStr;

/*
 * Structure definition for Edges
 */

typedef struct {          
  int Start, End;          /* Start and End points of Edge        */
} EdgeStr;

/*
 * Structure definition for Polygons    
 */

typedef struct {          
  int Edges,               /* Number of Edges per polygon         */
      Color;               /* Color of polygons (color map index) */
  int EList[ELIST_MAX];    /* List of Edges                       */
  float Normal[3];         /* Normal vector for polygon           */
                           /* Shading values (between 0 and 1)    */
  float Diffuse,           /* Measure of diffuse light            */
        Specular;          /* Measure of specular reflection      */
} PolygonStr;

/*
 *  Typedefs
 */

typedef struct {
  
  /*
   * Spaceball specific variables
   */
  
  float     sbScale;             /* scale factor for data vector elements */
  int       sbAllowRotation;     /* allow Spaceball rotation elements     */
  int       sbAllowTranslation;  /* allow Spaceball translation elements  */
  int       sbDominant;          /* apply SPW_HighValuePassFilter         */
  float     sbScaleFactor[6];    /* scaling constants for base "feel"     */

#ifdef OS_WIN32
  SiHdl     devHdl;              /* Used to check for active Spaceball    */
#endif

  /*
   *  Put app specific stuff here
   */
  
  void *appData;     /* will point to a SpaceballAppDataStruct */
  
} SpaceballControlStruct;

/*
 * Modify this struct to include any app-specific data that the Spaceball
 * routines might need to get at and modify
 */

typedef struct {
  float *CTM;      /* Current transformation matrix */
  PointStr center;
  PointStr centerRot;
  int redraw;
} SpaceballAppDataStruct;

/*
 * Function prototypes
 */

void SbButtonPressEvent(int button, void *appdata);
void SbButtonReleaseEvent(int button, void *appdata);
void Render(Matrix CTM, PointStr center);
void ResetWidgetView(Matrix CTM, PointStr Center);
void MainInitialize(char *fname, 
                    SpaceballControlStruct *scs,  
                    SpaceballAppDataStruct *app,
                    float *CTM, PointStr *World);
void DrawLine(int x1, int y1, int x2, int y2);

#ifdef OS_WIN32
void DrawFilledBox(int x, int y, int width, int height, HPEN color);
void SetColor(HPEN color);
#endif

#endif


