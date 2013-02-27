//
// IO.h
//
// Includes all the headers needed by the image
// loaders and savers
//

#ifndef IMAGEIO_HEADER
#define IMAGEIO_HEADER

extern const char *ImageLoaderErrors;
extern const char *ImageSaverErrors;

#include "ImageIO_IDs.h"
#include "ImageIO.h"
#include "ImageData.h"

// Insert headers for file formats here
#include "BMP.h"
#include "Targa.h"
#include "CSource.h"

#endif