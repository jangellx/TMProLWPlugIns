/*
======================================================================
icon.c

Routines for drawing a 32 x 32 image on a LightWave panel.  Part of
the LightWave SDK BinView sample plug-in.

Ernie Wright  3 Nov 99
====================================================================== */

#include <stdio.h>
#include <lwpanel.h>
#include "binview.h"


/* 256-color palette for the image */

static unsigned char rgb[ 256 ][ 3 ] = {
   255,255,255,
     8,  8,  8,
    16, 16, 16,
    24, 24, 24,
    33, 33, 33,
    74, 74, 74,
   198,189,189,
   115,107,107,
   189,173,173,
    82, 74, 74,
   198,173,173,
   173,148,148,
    49, 41, 41,
    99, 82, 82,
   181,148,148,
   165,132,132,
   181,140,140,
   107, 82, 82,
    99, 74, 74,
    66, 49, 49,
    90, 66, 66,
    33, 24, 24,
   115, 82, 82,
   140, 99, 99,
    24, 16, 16,
   123, 82, 82,
    41, 24, 24,
    57, 33, 33,
    74, 41, 41,
    16,  8,  8,
    66, 33, 33,
    49, 24, 24,
   206, 90, 90,
    57, 24, 24,
    99, 41, 41,
    41, 16, 16,
    66, 24, 24,
    24,  8,  8,
    74, 24, 24,
    57, 16, 16,
   156, 41, 41,
    49,  0,  0,
   206, 90, 82,
   206,115,107,
   206, 99, 90,
   181, 82, 74,
   165, 74, 66,
   173, 74, 66,
   156, 66, 57,
   181, 66, 57,
   148, 49, 41,
   239,173,165,
   247,173,165,
   222,156,148,
   206,140,132,
   214,140,132,
   123, 57, 49,
   148, 66, 57,
   214, 90, 74,
   123, 49, 41,
   206, 82, 66,
   107, 41, 33,
    90, 24, 16,
   198,140,132,
   189,132,123,
   132, 82, 74,
   165, 99, 90,
   123, 74, 66,
   140, 82, 74,
   107, 57, 49,
   115, 57, 49,
    99, 49, 41,
   189, 90, 74,
   107, 49, 41,
    90, 41, 33,
    99, 41, 33,
   198, 82, 66,
   189, 74, 57,
   198, 74, 57,
   173, 66, 49,
   181, 66, 49,
   189, 66, 49,
   165, 57, 41,
   173, 57, 41,
   148, 49, 33,
    74, 24, 16,
   156, 41, 24,
    66, 16,  8,
    74, 16,  8,
   189,148,140,
   165,123,115,
   123, 82, 74,
   231,148,132,
   222,132,115,
    90, 49, 41,
   206,107, 90,
   198, 99, 82,
   181, 90, 74,
    82, 41, 33,
   173, 82, 66,
   165, 74, 57,
   173, 74, 57,
   156, 66, 49,
    82, 33, 24,
   148, 57, 41,
   132, 49, 33,
    66, 24, 16,
    57, 16,  8,
    49,  8,  0,
   222,148,132,
   198,123,107,
   107, 66, 57,
   189,115, 99,
   181,107, 90,
   239,123, 99,
   222,107, 82,
   231,107, 82,
   140, 66, 49,
   214, 99, 74,
   222, 99, 74,
   148, 66, 49,
    74, 33, 24,
   206, 90, 66,
   132, 57, 41,
   198, 82, 57,
   123, 49, 33,
   123, 41, 24,
   189,156,148,
   173,140,132,
   156,123,115,
   123, 90, 82,
   231,165,148,
   107, 74, 66,
   206,140,123,
   198,132,115,
   189,123,107,
    90, 57, 49,
   173,107, 90,
    82, 49, 41,
   239,140,115,
   214,123, 99,
   222,123, 99,
   239,132,107,
   222,115, 90,
   206,107, 82,
   214,107, 82,
   247,123, 90,
   239,115, 82,
   173, 74, 49,
   181, 74, 49,
   165, 66, 41,
   107, 41, 24,
   148, 57, 33,
    49, 16,  8,
    99, 33, 16,
   198,165,156,
   165,132,123,
   181,123,107,
   173,115, 99,
   239,156,132,
   165,107, 90,
   222,140,115,
   198,115, 90,
   123, 66, 49,
   173, 90, 66,
   231,115, 82,
   165, 82, 57,
   107, 49, 33,
   165, 74, 49,
   222, 99, 66,
   156, 66, 41,
    99, 41, 24,
   140, 57, 33,
   173,148,140,
   148,123,115,
   132,107, 99,
   231,181,165,
   222,173,156,
   214,165,148,
   206,156,140,
    99, 74, 66,
   189,140,123,
   181,132,115,
   165,115, 99,
    82, 57, 49,
   214,140,115,
   247,148,115,
   255,148,115,
   239,140,107,
   206,107, 74,
   214,107, 74,
    99, 49, 33,
    90, 41, 24,
   132, 57, 33,
   173, 74, 41,
   231,165,140,
   123, 82, 66,
   239,148,115,
    57, 33, 24,
   214,123, 90,
   189, 99, 66,
   181, 90, 57,
   222,107, 66,
   198, 90, 49,
   206,165,148,
   132, 74, 49,
   173, 90, 57,
    82, 41, 24,
    74, 33, 16,
   173,156,148,
   165,148,140,
    99, 82, 74,
   222,165,140,
   231,140, 99,
   181,107, 74,
    41, 24, 16,
    33, 16,  8,
   206, 99, 49,
    49, 16,  0,
   189,173,165,
   156,140,132,
    90, 74, 66,
   148, 99, 74,
   222,140, 99,
    74, 41, 24,
    66, 33, 16,
    24,  8,  0,
   148,132,123,
   115, 99, 90,
    74, 41, 16,
    33, 24, 16,
   156,148,140,
   148,140,132,
    49, 41, 33,
    24, 16,  8,
    49, 33, 16,
   156,156,148,
   140,140,132,
    74, 74, 66,
    24, 24, 16,
     8, 16,  8,
     0,  8,  0,
    16, 24, 24,
     8, 16, 16,
     0,  8,  8,
   123,115,123,
    90, 82, 90,
    99, 90, 99,
    24, 16, 24,
    16,  8, 16,
    49, 16, 24,
   132, 90, 99,
   140, 99,107,
    99, 41, 49,
    90, 33, 41,
     0,  0,  0
};

/* image data for a 32 x 32, indexed-color image */

static unsigned char icon[ 32 ][ 32 ] = {
   152,170,150, 83, 81, 78,122, 58,119,165,143,146,187,187,187,159,
   186,197,188,200, 88, 62,126, 84, 97, 86, 40, 82, 82,148,203,190,
   152,152,170,152, 81,124,122,118,119,116,146,146,187,186,188,187,
   186,139,197,144, 94,208,154,154,196, 71, 69,253,163,102,203,217,
   117,104,170, 82, 80, 49, 77, 76,189,115,143,114,114,188,186,186,
   197,223,223,197,222,192, 87,192,156,180,111, 18,163,117, 77,119,
   172,104,170,170, 80, 80, 77, 76,122,145,143,114,213,141,188,197,
   188,143,199,223,113,205,192, 62,128,132,111,132,163,120, 77,118,
    46,104,170,150, 47, 77, 77,124,122,145,143,115,114,142,188,114,
   114,143,141,143,110, 97, 57,167,128,180, 22, 22,111,117,201,190,
   181, 63,170,112,179,178, 95,201, 76, 93,161,115,159, 51, 51,139,
   143,115,161,109, 72, 45,135,183, 16,220,236,209, 65,123, 80, 93,
   172, 63,166,204, 49,162, 55,124,122, 93,161,145, 51,118,141,109,
   118,190,185, 43,148,101,135,157,155,219, 91,175,129, 59,168,185,
   172, 64,164,155, 80, 72,109, 76, 76,185,161,118, 51,190,141,195,
   189,190,161,185,101,168,157,157,155,219, 91,175,156, 59,148,185,
   172, 64, 97,178, 83, 72, 55, 76,109,109,161,118,176,190,140,212,
   189,177,212,185,206,181,182,137,204, 10,196, 23, 15,123, 64,133,
   158,127,150, 64,134, 54,112, 77, 96,177, 93,122,133, 55,179,110,
   201,162,177,110,120,164, 89,160,127,173, 16, 14, 23, 73, 46,179,
    66,102, 84, 82,214, 97,148, 77, 76, 44, 44, 42, 32, 43,110, 72,
    97,166,113,110,100,102, 45, 46,155, 66,156,129, 69, 70,172, 97,
   105,152,104,170,152,170,150,201, 49, 76, 76, 42, 45, 99, 99,166,
   164,164,166,166,166,102,170,172, 89, 91,196, 65,111, 70,172, 82,
   105,105,104,104,152,152,170,101,101, 49, 47, 72, 97,164,164,166,
   164,214,214,166,120, 57, 57,123,127, 25, 91, 91,111, 70, 50,170,
   125,105,172,123,104,104,102,102, 48, 48, 46, 46, 97, 46,100,168,
   168,168,168,168,117,123, 59,125,127,130, 91, 91,136, 73,123,104,
   167,125,193,193,193,104,120,117,120, 57, 57, 57, 46, 50, 48, 45,
   100, 82,101, 99, 67, 56,167,192,173, 91,211, 91,136, 71,123,104,
   151,167,167,167,126,125, 56,123, 56,163, 56, 56, 57, 56, 46,158,
    45, 68, 97, 45, 68, 65, 70, 71,173, 18,180,132, 20, 71,125,123,
    75,171,171,151,151,125, 59, 73, 61, 75, 74,192, 33,216, 39, 36,
    85,253,111, 74,111,111,111,136, 11,211,180,  9,136, 94, 61,193,
   171,171,171,192,151, 59, 56, 61, 74,103, 87,108,240,244,241,241,
    21,228,220,228, 87,107,107,107,231,211,221,221,136, 94,167,123,
    98, 98, 98, 98,253,254, 98, 30, 38,184, 30,255,  3,  2,  4,  4,
   255, 24,245,  6,251,250,216,216,232, 13,238, 18,136, 94,191,167,
   207, 98, 30,121, 38, 98, 30, 33, 27, 27, 27, 21,  4,  3,  3,  3,
     2,255,  4,  7,252, 33,216,215,237,  9,211,  9,136,138,171,191,
    74,121, 36,106,235, 36, 35, 31, 19, 19,  4, 26,242,  2,255,  1,
    19,248,255,247,252, 27,230,230,231,  9,184,211,138, 94,191,167,
    74,121,106,106,235,235,226, 26, 13,  9,  3,  2,  2,  1,  2,  2,
     5, 19,255,  7,228, 27,230,234,232,  9,238, 18,184, 98,191, 73,
    98,121, 36,106, 31,235,255,215,  9, 19, 29,240,  3,  2,  4,  4,
    19,  4, 29,  7,184, 26,230,234,237,211, 20, 18,136, 94, 73, 73,
    74, 98, 98,121,198, 33,216,230,  4,230,  1,239,242,  1,  3,  2,
    29, 29, 24,246, 27, 24,230,234,231,  9, 20, 18,136, 94, 73, 73,
   191, 94,254, 98, 30,198, 31, 35,216, 37, 24, 21,244,  2,  3,249,
    29,249, 24,  4,230,234, 26, 21,231,211,180,211,136, 94, 73,167,
    94, 94,136,136,184, 98, 36, 31,215, 31, 26,230,240,  3, 29,  1,
    21,233,  4, 24, 24, 29, 21,233,236, 13, 20, 22,136, 94,191,167,
    75,191,191,191, 75, 74, 74,207,207,225,218,218,216,216,216,216,
   216,216,216,216,224, 36,224, 98,210, 17, 18, 18,136, 94, 74,191,
   191,191,191,191,191, 71, 71, 71, 74, 98,207,224, 30, 27, 30,198,
   198,198, 31,198,198,224,198,224, 11,211, 18, 18,136, 74,171,191,
   191,191,191, 74, 94,191, 71, 94, 94,136, 94, 94,136,184,138,138,
    28, 98, 98, 30,224, 36,224, 36,210, 18, 18, 18,136,138, 74, 74,
   191, 74,192, 74,103, 30,121, 98, 98, 98,138,138,138,138,138, 30,
    30, 30, 36, 36, 36,235,224, 36,210, 20,136,136,138, 98, 74,192,
   192,192,192,207,121, 38,225, 39,106,198,235, 36,106, 36,106, 39,
   106, 39, 39, 39,225, 36, 36,225,227,227,174,174,227, 94,207, 98,
   208,208,208,208,121, 36,106, 39,153,153,153,153, 35, 39, 35, 39,
   235, 39, 39, 39,235, 36,225, 36, 98,136, 94,136,136,103, 30,103,
};

static LWRasterID ras = NULL;


/*
======================================================================
init_icon()

Initialize the image raster.

INPUTS
   rasf     raster functions returned by the Raster Services global

RESULTS
   Creates a raster and draws the icon image on it.

This should be called once, before calling draw_icon().  The raster
created by this function should be freed by calling free_icon() when
it's no longer needed.
====================================================================== */

static void init_icon( LWRasterFuncs *rasf )
{
   int i, j;

   ras = rasf->create( 32, 32, 0 );
   if ( !ras ) return;

   for ( j = 0; j < 32; j++ )
      for ( i = 0; i < 32; i++ )
         rasf->drawRGBPixel( ras,
            rgb[ icon[ j ][ i ] ][ 0 ],
            rgb[ icon[ j ][ i ] ][ 1 ],
            rgb[ icon[ j ][ i ] ][ 2 ], i, 31 - j );
}


/*
======================================================================
draw_icon()

Draw the icon image on a LightWave panel window.

INPUTS
   x, y     position of the image's upper left corner
   rasf     raster functions returned by the Raster Services global
   panel    panel to draw on

RESULTS
   Blits the image to the panel window.

The (x, y) pixel coordinates are relative to the top left corner of
the panel window.
====================================================================== */

void draw_icon( int x, int y, LWRasterFuncs *rasf, LWPanelID panel )
{
   if ( !ras ) init_icon( rasf );
   if ( !ras ) return;

   rasf->blitPanel( ras, 0, 0, panel, x, y, 32, 32 );
}


/*
======================================================================
free_icon()

Frees resources allocated by init_icon().

INPUTS
   rasf     raster functions returned by the Raster Services global

RESULTS
   Frees the icon image raster.
====================================================================== */

void free_icon( LWRasterFuncs *rasf )
{
   if ( ras ) {
      rasf->destroy( ras );
      ras = NULL;
   }
}