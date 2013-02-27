// pluginio.h - functions for Layout plugin load/save functions

#include <lwran.h>

#define SWAP(a,b)	{ a^=b; b^=a; a^=b; }
#define	BYTE	unsigned char
#define	WORD	unsigned short

#ifdef _WINDOWS
#define FILE_MARK		'\\'
#define	BSWAP_W(w)		SWAP( (((BYTE *)&w)[0]), (((BYTE *)&w)[1]) )
#define	BSWAP_L(l)		{ BSWAP_W( (((WORD *)&l)[0])); BSWAP_W( (((WORD *)&l)[1])); SWAP( (((WORD *)&l)[0]), (((WORD *)&l)[1]) ) }
#else
#define FILE_MARK		'/'
#define	BSWAP_W(w)		
#define	BSWAP_L(l)	
#endif


void  writeInt(const LWSaveState *,int);
int   readInt(const LWLoadState *);
void  writeFloat(const LWSaveState *,float);
float readFloat(const LWLoadState *);
int   readString(const LWLoadState *,char *,int);
void  writeString(const LWSaveState *,char *);

