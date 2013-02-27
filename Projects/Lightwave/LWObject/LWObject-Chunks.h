//
// LWObject-Chunks.h
//

#ifndef HEADER_LWOBJECT_CHUNKS
#define HEADER_LWOBJECT_CHUNKS

// Header Chunks
const unsigned long LWObjectChunk_FORM = ('F'<<24) | ('O'<<16) | ('R'<<8) | 'M';
const unsigned long LWObjectChunk_LWOB = ('L'<<24) | ('W'<<16) | ('O'<<8) | 'B';
const unsigned long LWObjectChunk_LWO2 = ('L'<<24) | ('W'<<16) | ('O'<<8) | '2';

// Primary Chunks
const unsigned long LWObjectChunk_TAGS = ('T'<<24) | ('A'<<16) | ('G'<<8) | 'S';
const unsigned long LWObjectChunk_LAYR = ('L'<<24) | ('A'<<16) | ('Y'<<8) | 'R';
const unsigned long LWObjectChunk_PNTS = ('P'<<24) | ('N'<<16) | ('T'<<8) | 'S';
const unsigned long LWObjectChunk_BBOX = ('B'<<24) | ('B'<<16) | ('O'<<8) | 'X';
const unsigned long LWObjectChunk_POLS = ('P'<<24) | ('O'<<16) | ('L'<<8) | 'S';
const unsigned long LWObjectChunk_PTAG = ('P'<<24) | ('T'<<16) | ('A'<<8) | 'G';
const unsigned long LWObjectChunk_SURF = ('S'<<24) | ('U'<<16) | ('R'<<8) | 'F';
const unsigned long LWObjectChunk_VMAP = ('V'<<24) | ('M'<<16) | ('A'<<8) | 'P';
const unsigned long LWObjectChunk_VMAD = ('V'<<24) | ('M'<<16) | ('A'<<8) | 'D';

// Surface Subchunks
const unsigned long LWObject_SURFChunk_COLR = ('C'<<24) | ('O'<<16) | ('L'<<8) | 'R';
const unsigned long LWObject_SURFChunk_DIFF = ('D'<<24) | ('I'<<16) | ('F'<<8) | 'F';
const unsigned long LWObject_SURFChunk_SHRP = ('S'<<24) | ('H'<<16) | ('R'<<8) | 'P';
const unsigned long LWObject_SURFChunk_LUMI = ('L'<<24) | ('U'<<16) | ('M'<<8) | 'I';
const unsigned long LWObject_SURFChunk_SPEC = ('S'<<24) | ('P'<<16) | ('E'<<8) | 'C';
const unsigned long LWObject_SURFChunk_GLOS = ('G'<<24) | ('L'<<16) | ('O'<<8) | 'S';
const unsigned long LWObject_SURFChunk_REFL = ('R'<<24) | ('E'<<16) | ('F'<<8) | 'L';
const unsigned long LWObject_SURFChunk_TRAN = ('T'<<24) | ('R'<<16) | ('A'<<8) | 'N';
const unsigned long LWObject_SURFChunk_TRNL = ('T'<<24) | ('R'<<16) | ('N'<<8) | 'L';
const unsigned long LWObject_SURFChunk_BUMP = ('B'<<24) | ('U'<<16) | ('M'<<8) | 'P';
const unsigned long LWObject_SURFChunk_SIDE = ('S'<<24) | ('I'<<16) | ('D'<<8) | 'E';
const unsigned long LWObject_SURFChunk_SMAN = ('S'<<24) | ('M'<<16) | ('A'<<8) | 'N';
const unsigned long LWObject_SURFChunk_RFOP = ('R'<<24) | ('F'<<16) | ('O'<<8) | 'P';
const unsigned long LWObject_SURFChunk_TROP = ('T'<<24) | ('R'<<16) | ('O'<<8) | 'P';
const unsigned long LWObject_SURFChunk_ADTR = ('A'<<24) | ('D'<<16) | ('T'<<8) | 'R';
const unsigned long LWObject_SURFChunk_CLRH = ('C'<<24) | ('L'<<16) | ('R'<<8) | 'H';
const unsigned long LWObject_SURFChunk_CLRF = ('C'<<24) | ('L'<<16) | ('R'<<8) | 'F';
const unsigned long LWObject_SURFChunk_ALPH = ('A'<<24) | ('L'<<16) | ('P'<<8) | 'H';


// Polygon Type IDs
const unsigned long LWObject_PolygonType_FACE = ('F'<<24) | ('A'<<16) | ('C'<<8) | 'E';
const unsigned long LWObject_PolygonType_CURV = ('C'<<24) | ('U'<<16) | ('R'<<8) | 'V';
const unsigned long LWObject_PolygonType_PTCH = ('P'<<24) | ('T'<<16) | ('C'<<8) | 'H';
const unsigned long LWObject_PolygonType_MBAL = ('M'<<24) | ('B'<<16) | ('A'<<8) | 'L';
const unsigned long LWObject_PolygonType_BONE = ('B'<<24) | ('O'<<16) | ('N'<<8) | 'E';

// VMap Type IDs
const unsigned long LWObject_VMapType_PICK = ('P'<<24) | ('I'<<16) | ('C'<<8) | 'K';
const unsigned long LWObject_VMapType_WGHT = ('W'<<24) | ('G'<<16) | ('H'<<8) | 'T';
const unsigned long LWObject_VMapType_MNVP = ('M'<<24) | ('N'<<16) | ('V'<<8) | 'P';
const unsigned long LWObject_VMapType_TXUV = ('T'<<24) | ('X'<<16) | ('U'<<8) | 'V';
const unsigned long LWObject_VMapType_RGB  = ('R'<<24) | ('G'<<16) | ('B'<<8) | ' ';
const unsigned long LWObject_VMapType_RGBA = ('R'<<24) | ('G'<<16) | ('B'<<8) | 'A';
const unsigned long LWObject_VMapType_MORF = ('M'<<24) | ('O'<<16) | ('R'<<8) | 'F';
const unsigned long LWObject_VMapType_SPOT = ('S'<<24) | ('P'<<16) | ('O'<<8) | 'T';


// Surface Presets Chunks (SRF File)
const unsigned long LWObjectChunk_Preset_PST_ = ('P'<<24) | ('S'<<16) | ('T'<<8) | '_';
const unsigned long LWObjectChunk_Preset_POWN = ('P'<<24) | ('O'<<16) | ('W'<<8) | 'N';
const unsigned long LWObjectChunk_Preset_PSUB = ('P'<<24) | ('S'<<16) | ('U'<<8) | 'B';
const unsigned long LWObjectChunk_Preset_PDAT = ('P'<<24) | ('D'<<16) | ('A'<<8) | 'T';

#endif