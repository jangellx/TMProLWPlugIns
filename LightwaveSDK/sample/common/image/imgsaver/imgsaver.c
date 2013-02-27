/* ImgSaver.c -- ImageSaver plugin example */
/* This example ImageSaver class plugin writes out a bunch of little marks, with info about the image... 
   almost as useless, but far more annoying is the testload loader example, which ia named AAnounce 
   in the plugin list, to popup, tell you what image it is loading, then fail, so other loaders get a shot.
*/
#include <splug.h>
#include <moni.h>
#include <lwmod.h>
#include <lwran.h>
#include <image.h>
#include <stdio.h>
#include <stdlib.h>


typedef struct {
	FILE	*td_file;
	char	*td_name;
	int	td_type; 
} testdata;

XCALL_(static void)  TestSetSize(testdata *dat,int w,int h,int flags) {
	if(dat->td_file = fopen(dat->td_name,"w")) {
		fprintf(dat->td_file,"Image: %s\n\t %d x %d\n",dat->td_name,w,h);
	}
}

XCALL_(static int) TestSendLine(testdata  *dat,int line,const ImageValue *rgbline,const ImageValue *alphaline) {
	if(dat->td_file) {
		fprintf(dat->td_file,".~. ");
	}
	return AFUNC_OK;
}

XCALL_(static int) TestDone(testdata  *dat,int line) {
	if(dat->td_file) {
		fprintf(dat->td_file,"\nDONE!\t %d \n",line);	
		fclose(dat->td_file);
	}
	return AFUNC_OK;
}

XCALL_(static int) TestSaver( long version, GlobalFunc *global, ImSaverLocal *local, void *serverData ) {
	testdata	dat;
	ImageProtocol prot;

	XCALL_INIT;

	dat.td_name = (char *)local->filename;
	dat.td_type = local->type;
	prot.type = IMG_RGB24;
	prot.color.priv_data = &dat;
	prot.color.setSize = TestSetSize;
	prot.color.sendLine = TestSendLine;
	prot.color.done = TestDone;
	(*local->sendData) (local->priv_data, &prot, IMGF_ALPHA);

	return AFUNC_OK;
}

#include <string.h>
XCALL_(static int) TestLoader( long version, GlobalFunc *global, ImLoaderLocal *local, void *serverData ) {
	MessageFuncs		*message;
	char mes[81];
	int f;
	XCALL_INIT;
	if(message = (*global) ("Info Messages", GFUSE_TRANSIENT)) {
		(*message->info) ("Asked to load:   ",local->filename);
	}
 	local->result = IPSTAT_NOREC;		
	return AFUNC_OK;
}

ServerRecord ServerDesc[] = {
	{ "ImageSaver", 		"TestSaver(.txt)",	TestSaver },	
	{ "ImageLoader", 		"AAnnounceLoader",	TestLoader },	
	{ NULL }
};
