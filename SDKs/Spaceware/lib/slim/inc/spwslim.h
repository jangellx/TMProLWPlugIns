/*-----------------------------------------------------------------------------
 *
 *  spw_slim.h: SpaceWare Library Integration Module (SLIM) public definitions 
 *
 *  $Id: spwslim.h,v 1.7 1998/03/27 10:31:59 prichard Exp $
 *
 *-----------------------------------------------------------------------------
 *
 * (C) 1997 Spacetec IMC Corporation ("Spacetec").
 *
 * Permission to use, copy, modify, and distribute this software for all
 * purposes and without fees is hereby granted provided that this copyright
 * notice appears in all copies. Permission to modify this software is granted
 * and Spacetec will support such modifications only if said modifications are
 * approved by Spacetec.
 *
 */

#ifndef SPW_SLIM_H
#define SPW_SLIM_H

static char SpwSlimCvsId[] = "(C) 1997 Spacetec IMC Corporation: $Id: spwslim.h,v 1.7 1998/03/27 10:31:59 prichard Exp $";

#include "spwmacro.h"
#include "spwplat.h"

#define SPW_SLIM_MAJOR    3
#define SPW_SLIM_MINOR    0
#define SPW_SLIM_UPDATE   0
#define SPW_SLIM_BUILD    7
#define SPW_SLIM_VERSION  "SLIM version 3.0"
#define SPW_SLIM_DATE     "March 27, 1998"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef EXPORT_FUNCTIONS
#define DLL_EXPORT __declspec(dllexport)
#define CALL_TYPE __stdcall
#else
#define DLL_EXPORT
#define CALL_TYPE
#endif

/* PUBLIC VARIABLES & DEFINES */

#if !defined(__GL_GL_H__) && !defined(SPW_NO_MATRIX)
typedef float Matrix[4][4];
#endif

DLL_EXPORT void SPW_ArbitraryAxisToMatrix (Matrix, float[3], float);
DLL_EXPORT int SPW_CompareMatrices (Matrix, Matrix);
DLL_EXPORT float SPW_DotProduct (float [3], float [3]);
DLL_EXPORT float SPW_GetMatrixScale (Matrix);
DLL_EXPORT void SPW_InvertMatrix (Matrix, Matrix);      
DLL_EXPORT void SPW_LookAtUpVectorToMatrix (Matrix, float [3], float [3], 
                                            float [3]);
DLL_EXPORT void SPW_MatrixToArbitraryAxis (float [3], float *, Matrix);
DLL_EXPORT float SPW_MatrixDeterminant (Matrix);
DLL_EXPORT void SPW_MatrixToLookAtUpVector (float [3], float [3], float [3], 
                                            Matrix);
DLL_EXPORT void SPW_Mult44x44 (Matrix, Matrix, Matrix);
DLL_EXPORT void SPW_MultFull14x44 (float [3], float [3], Matrix);
DLL_EXPORT void SPW_Mult14x44 (float [3], float [3], Matrix);
DLL_EXPORT void SPW_Mult33x33 (Matrix, Matrix, Matrix);
DLL_EXPORT void SPW_Mult13x33 (float [3], float [3], Matrix);
DLL_EXPORT int SPW_OrthoNormMatrix (Matrix);
DLL_EXPORT float SPW_VectorLength (float [3]);


DLL_EXPORT void SPW_TransposeMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_CopyMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_ScaleMatrix (Matrix, Matrix, float);
DLL_EXPORT void SPW_GetTranslationMatrix (Matrix, Matrix);
DLL_EXPORT void SPW_InitializeMatrix (Matrix, float, float, float, float,
                                      float, float, float, float,
                                      float, float, float, float,
                                      float, float, float, float);
DLL_EXPORT void SPW_MakeIdentityMatrix (Matrix);
DLL_EXPORT void SPW_NormalizeVector (float [3], float [3]);
DLL_EXPORT void SPW_CrossProduct (float [3], float [3], float [3]);
DLL_EXPORT void SPW_PrintMatrix (char *, Matrix);
DLL_EXPORT void SPW_PrintVector (char *, float [3]);
DLL_EXPORT void SPW_PrintSpaceballData (char *, float [7]);
DLL_EXPORT void SPW_HighValuePassFilter (float *, int);

#ifdef __cplusplus
}
#endif

#endif /* SPW_SLIM_H */






