/* Global Saver Plugin Header */
/* 9/12/95 */
// 4/23/97 -- FORMAT List
#define GLOBAL_SAVER_NAME	"ImageSaver"

typedef struct st_svimage {
	int		im_w, im_h;
	unsigned char *im_rgb, *im_a;
} svImage;

typedef struct st_globalsaverdata {
	ImageProtocol 	*(*open)(GlobalFunc *);
	int				(*setType)(ImageProtocol *, char *, char *, int);
	void 			(*close)(ImageProtocol *);
	int				(*saveImage)(GlobalFunc *, svImage *, char *);
} GlobalSaverData;

#define FORMAT_LIST_NAME	"ImageSaveFormats"
