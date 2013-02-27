// PREVIEW.H
// Preview services for layout
// needs Lwpannel services to run
// API version 1 - 24/02/98
// Gregory Duquesne, Newtek


#ifndef	PANEL_SERVICES_NAME
	#include <lwpanel.h>
#endif

#define PREVIEW_SERVICES_NAME		"PreviewServices"
#define PREVIEW_ACCESS_NAME			"PreviewAccess"
#define PREVIEW_API_VERSION			1


/*
 *  MACRO to mimic panels control creation.  User should treat this as 
 * the whole ctl NOTE:  Must call finalize method after prev has been 
 * positioned and before pannel is opened!!
 */
 
#define PREV_CTL(prevh, prevbuf, panid) ((*prevh->Interface)(prevbuf,panid))

#ifndef LW_RAN_H
	#include "lwran.h"
#endif

typedef	struct	st_pbufLayerDesc{
	int				   dataType, dataSize, dataNumber;	// example: float[3] -> dataSize = 4; dataNumber = 3;
	int				   flag,visible;			  		        // layer flag, layer visibility (in user interface)
	const	char	*name;						  	            // layer's name for UI
}	pbufLayerDesc;

//dataTypes
#define		LDT_USHORT		0	// = unsigned	short
#define		LDT_SCALAR		1	// = float
#define		LDT_VECTOR		3	// = float[3]
#define		LDT_COLOR		  4	// = float[3]
#define		LDT_RGB			  5	// = unsigned char[3]
#define		LDT_UCHAR		  6	// = unsigned char
#define		LDT_USER		  7	// = for what you like...
//dataSizes
#define		LDS_UCHAR		sizeof(unsigned char)
#define		LDS_SHORT		sizeof(short)
#define		LDS_FLOAT		sizeof(float)
//visibility
#define		LV_VISIBLE		1
#define		LV_HIDDEN		  2

// Layer names
#define	LWPBN_POS			    "Position"
#define	LWPBN_RGB			    "RGB"
#define	LWPBN_ALPHA			  "ALPHA"
#define	LWPBN_Z			    	"ZBuffer"
#define	LWPBN_RAY_LENGTH	"RayLength"
#define	LWPBN_NORM			  "Slope"
#define	LWPBN_COLOR			  "Color"
#define	LWPBN_LUM			    "Luminosity"
#define	LWPBN_DIF			    "Diffuse"
#define	LWPBN_SPE			    "Specular"
#define	LWPBN_REF			    "Reflectivity"
#define LWPBN_TRA			    "Transparency"
#define	LWPBN_ROUGH			  "Roughness"
#define	LWPBN_ETA			    "Refraction Index"
#define	LWPBN_PARAM1		  "User parameter1"
#define	LWPBN_PARAM2		  "User parameter2"
#define	LWPBN_RT_REF		  "Reflection"
#define	LWPBN_RT_TRA		  "Transmission"
#define LWPBN_SURFID		  "SurfaceID"
#define LWPBN_RENDER		  "Render"

// Layer flags:
// shader buffers
#define LWPBF_NORM			 (1<<0)
#define LWPBF_COLOR			 (1<<1)
#define LWPBF_LUM			   (1<<2)
#define LWPBF_DIF			   (1<<3)
#define LWPBF_SPE			   (1<<4)
#define LWPBF_REF			   (1<<5)
#define LWPBF_TRA			   (1<<6)
#define LWPBF_ROUGH			 (1<<7)
#define LWPBF_ETA			   (1<<8)
#define LWPBF_POS			   (1<<9)	
#define LWPBF_RAY_LENGTH (1<<10)
#define LWPBF_SURFID		 (1<<11)
// user buffers
#define LWPBF_PARAM1		 (1<<12)
#define LWPBF_PARAM2		 (1<<13)
// rgba buffers: 
#define LWPBF_RGB			   (1<<14)
#define LWPBF_ALPHA			 (1<<15)
#define LWPBF_Z				   (1<<16)
// rt buffers
#define	LWPBF_RT_REF		 (1<<17)
#define	LWPBF_RT_TRA		 (1<<18)
// AA buffer (always used)
#define	LWPBF_RENDER		 (1<<30)
#define	LWPBF_WEIGHT		 (1<<31)
// common usefull set of flags
#define LWPBF_RGBAZ			 (1<<14 | 1<<15 | 1<<16)
#define LWPBF_SHADER		 (1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5 | 1<<6 | 1<<7 | 1<<8 | 1<<9 | 1<<10 | 1<<11)
#define LWPBF_RTBUFFERS	 (1<<17 | 1<<18) 



typedef	struct	st_pbufLayer {
	void			    *data;
	pbufLayerDesc	 desc;
	float			     min, max; // useful to rescale values at display
}	pbufLayer;

#define		LWP_MAX_BUFFERS		19

typedef	struct	st_prevBuffer{
	pbufLayer	layerList[LWP_MAX_BUFFERS];			             // list of layers
	double		raySource[3], zoomFactor, fovAlpha, fovBeta; // camera position, zoom Factor and fov angles
	int			  initialised, nLayers, flags;			           // init flags,number of layers,layer flags

	int				     rx, ry, activeLayer;		                 // UI parameters
	double		     dx, dy, ox, oy;
	unsigned char	*bitmap;						                     // last display
	LWControl		  *ctl, *preview_window;			             // on which controls the preview is displayed
	LWControl		  *mode, *load, *save;
	LWPanelID		   panID;							                     // panel ID for this preview
}	prevBuffer;

typedef	struct	st_prevBufferAccess{
	// pixel info
	int					    sx,sy;
	// shader info	
	unsigned	short	surfID;			       // surface id
	float				    color[3];
	float				    pos[3],norm[3];	   // point position and normal on the surface
	float				    lum, dif, spe, ref, tra, eta, rough;
	float				    rayLength;		     // distance to viewer
	// pixel filter info
	unsigned	char	rgba[4];		       // resultant of rendering: r,g,b and alpha components
	float				    z;				         // z value
	// other info
	float		        t_rgb[3],r_rgb[3]; // RGB components from reflection and transparency
	void		       *param1,*param2;		 // 2 params for the 2 user layers....
}	prevBufferAccess;

// Standard buffers are created by the create function regarding the flags passed by user
// The 2 user layers can be created via the addLayer function, passing their layer descriptor.
typedef	struct	st_PrevBufferHandler{
	prevBuffer	*(*createPrev) (int	rx,int	ry,int	flags);
	int			(*destroy) (prevBuffer	*buf);
	int			(*addLayer)(prevBuffer	*buf,pbufLayerDesc	*desc);
	int			(*init)(prevBuffer	*buf,LWTime	time,int resx,int resy);
	int			(*load)(prevBuffer	*buf,char	*fileName);
	int			(*save)(prevBuffer	*buf,char	*fileName);
	// UI stuff
	LWControl	*(*Interface)(prevBuffer	*buf, LWPanelID	panID);
	void		(*refresh)(prevBuffer	*buf);
	void		(*blitToScreen)(prevBuffer		*buf);
	void 		(*finalizePrev)(prevBuffer	*buf);		// MUST be called after prev ctl is positioned (before open)
	void		(*renderPixel) (prevBuffer	*buf,int x,int y,unsigned char rgb[3]);
	void		(*renderPixelOffscreen)(prevBuffer	*buf,int	x,int	y,unsigned	char	rgb[3]);
	// save and get data functions...
	void		(*savePreviewSt) (prevBuffer	*buf,prevBufferAccess	*pba,int	flags);
	void		(*getPreviewSt) (prevBuffer	*buf,prevBufferAccess	*pba,int	flags);
	// flags set the kind of data you want to save/get
	short		apiVersion;
}	PrevBufferHandler;

