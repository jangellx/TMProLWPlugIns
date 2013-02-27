//
// Checkerboard.h
// 
// Draws a checkerboard of the given grid size and color into
// the given image buffer.
//

#ifndef HEADER_IMAGE_CHECKERBOARD
#define HEADER_IMAGE_CHECKERBOARD

#include "..\imageclass.h"

enum CheckMask {
  CHECKMASK_EVEN = 1,    // Do just the even checks
  CHECKMASK_ODD  = 2,    // Do just the odd checks
  CHECKMASK_BOTH = 3,    // Do both the even and odd checks (CHECKMASK_EVEN | CHECKMASK_ODD)

  NUM_CHECKMASK          // Number of check mask modes
};

class Checkerboard {
public:
  static bool Draw( ImageClass *image, int x, int y, // "Smart" draw;  determines which mode the image is in and draws the checkerboard
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,
                    IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b,
                    int check_mask = CHECKMASK_BOTH );


  static bool DrawRGBA( ImageClass *image, int x, int y,
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,  IMG_BYTE odd_a,
                    IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b,  IMG_BYTE even_a,
                    int check_mask = CHECKMASK_BOTH );

  static bool DrawRGB( ImageClass *image, int x, int y,
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_r,  IMG_BYTE odd_g,  IMG_BYTE odd_b,
                    IMG_BYTE even_r, IMG_BYTE even_g, IMG_BYTE even_b,
                    int check_mask = CHECKMASK_BOTH );

  static bool DrawAlpha( ImageClass *image, int x, int y,
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_a,
                    IMG_BYTE even_a,
                    int check_mask = CHECKMASK_BOTH );

  static bool DrawIndexed( ImageClass *image, int x, int y,
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_index,
                    IMG_BYTE even_index,
                    int check_mask = CHECKMASK_BOTH );

  static bool DrawGrey( ImageClass *image, int x, int y,
                    int width, int height, int check_w, int check_h,
                    IMG_BYTE odd_value,
                    IMG_BYTE even_value,
                    int check_mask = CHECKMASK_BOTH );
};

#endif