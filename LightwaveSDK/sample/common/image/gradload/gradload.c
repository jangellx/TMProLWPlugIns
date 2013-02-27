/* Test Image Loader: 
	by Arnie Cachelin 
	Loads 'Gradient' files like: 
LWGRAD
320 200
255 255 255
0 255 0
0 0 0
0 0 255
 */

#include <splug.h>
#include <moni.h>
#include <lwmod.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN	255
static char LineBuff[LINE_LEN+1],lmes[120];
static Monitor		*monitor;
  
typedef struct {
	int	gr_type;
	int	gr_width;
	int	gr_height;
	ImageValue	gr_col1[3];
	ImageValue	gr_col2[3];
	ImageValue	gr_col3[3];
	ImageValue	gr_col4[3];
} Gradient;

int	ReadGradient(char *file,Gradient *grad)
{
	FILE	*fp=NULL;
	int	r,g,b;
	if(!(fp=fopen(file,"r"))) 
		return(0);
	fgets(LineBuff,LINE_LEN,fp);
	if(strncmp(LineBuff,"LWGRAD",6))
	{
		fclose(fp);
		return(0); 
	}
	fscanf(fp,"%d",&(grad->gr_type));
	fscanf(fp,"%d, %d",&(grad->gr_width),&(grad->gr_height));
	fscanf(fp,"%d, %d, %d",&r,&g,&b);
	grad->gr_col1[0] = r;
	grad->gr_col1[1] = g;
	grad->gr_col1[2] = b;
	fscanf(fp,"%d, %d, %d",&r,&g,&b);
	grad->gr_col2[0] = r;
	grad->gr_col2[1] = g;
	grad->gr_col2[2] = b;
	fscanf(fp,"%d, %d, %d",&r,&g,&b);
	grad->gr_col3[0] = r;
	grad->gr_col3[1] = g;
	grad->gr_col3[2] = b;
	fscanf(fp,"%d, %d, %d",&r,&g,&b);
	grad->gr_col4[0] = r;
	grad->gr_col4[1] = g;
	grad->gr_col4[2] = b;
	fclose(fp);
	return(grad->gr_width*grad->gr_height); 
}	

XCALL_(static int) GradLoad(
	long			 version ,
	GlobalFunc		 *global ,
	ImLoaderLocal 	*local ,
	void			*serverData )
{
	Gradient	mygrad;
	ImageProtocolID ip;
	ColorProtocol *prot;
	ImageValue *line,*aline,col[3],lcol[3];
	long w,h=0,alpha=0,channels=3,bpc=1,i,j,c;
	float dy2[3],dx[3],dy[3];
	XCALL_INIT;

	if (version != 1)
		return AFUNC_BADVERSION;

	monitor = local->monitor;
	if(!ReadGradient((char *)local->filename,&mygrad))
	{
		local->result = IPSTAT_NOREC;
		return AFUNC_OK;
	}
	w=	mygrad.gr_width;
	h=	mygrad.gr_height;
	local->result = IPSTAT_OK;
	if(!(ip=(*local->begin)(local->priv_data,IMG_RGB24)))
	{
		return AFUNC_OK;
	}
	if(monitor) MON_INIT(monitor,h);
	prot = &ip->color;
	IP_SETSIZE(prot,w,h,alpha ? IMGF_ALPHA:0);
	line=malloc(3*w*sizeof(ImageValue));
	aline=malloc(w*sizeof(ImageValue));
	if(!line || !aline)
	{
		if(line) free(line);
		if(aline) free(aline);
		return AFUNC_OK;
	}
	for(c=0;c<3;c++)
	{
		dy[c] = ((float)(mygrad.gr_col3[c] - mygrad.gr_col1[c]))/(mygrad.gr_height-1);
		dy2[c] = ((float)(mygrad.gr_col4[c] - mygrad.gr_col2[c]))/(mygrad.gr_height-1);
	}
	for(j=0; j<h; j++)
	{
		for(c=0;c<3;c++)
		{
			lcol[c] = mygrad.gr_col1[c] + j*dy[c]; 
			col[c] = mygrad.gr_col2[c] + j*dy2[c]; 
			dx[c] = ((float)(lcol[c] - col[c]))/(mygrad.gr_width-1);
		}
		for(i=0; i<w; i++)
			for(c=0;c<3;c++)
				line[3*i+c] = lcol[c] + i*dx[c];
		if(bpc=IP_SENDLINE(prot,j,line,aline))
		{
			break;
		}
		if(monitor) MON_STEP(monitor);
	}
	if(bpc=IP_DONE(prot,(local->result!=IPSTAT_OK)))
	{
		local->result = IPSTAT_FAILED;
	}
	if(monitor) MON_DONE(monitor);
	local->result = IPSTAT_OK;
	(*local->done)(local->priv_data,ip);
	free(aline);
	free(line);
	return AFUNC_OK;
}

ServerRecord		ServerDesc[] = {
	{ "ImageLoader", "GradLoader",		GradLoad },	 
	{ NULL }
};
