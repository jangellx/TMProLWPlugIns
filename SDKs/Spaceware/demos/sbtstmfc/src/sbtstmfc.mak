# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=sbtstmfc - Win32 (ALPHA) Debug
!MESSAGE No configuration specified.  Defaulting to sbtstmfc - Win32 (ALPHA)\
 Debug.
!ENDIF 

!IF "$(CFG)" != "sbtstmfc - Win32 Intel Debug" && "$(CFG)" !=\
 "sbtstmfc - Win32 Intel Release" && "$(CFG)" !=\
 "sbtstmfc - Win32 (ALPHA) Debug" && "$(CFG)" !=\
 "sbtstmfc - Win32 (ALPHA) Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "sbtstmfc.mak" CFG="sbtstmfc - Win32 (ALPHA) Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sbtstmfc - Win32 Intel Debug" (based on "Win32 (x86) Application")
!MESSAGE "sbtstmfc - Win32 Intel Release" (based on "Win32 (x86) Application")
!MESSAGE "sbtstmfc - Win32 (ALPHA) Debug" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE "sbtstmfc - Win32 (ALPHA) Release" (based on\
 "Win32 (ALPHA) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "sbtstmfc - Win32 Intel Release"

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sbtstm0"
# PROP BASE Intermediate_Dir "sbtstm0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin/win32i/Debug"
# PROP Intermediate_Dir "../bin/win32i/Debug"
# PROP Target_Dir ""
OUTDIR=.\../bin/win32i/Debug
INTDIR=.\../bin/win32i/Debug

ALL :  "$(OUTDIR)\sbtstmfc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../inc" /I "y:\rel\spcware\v0800018\Sdk\Win32i\ME\inc\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "../inc" /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /Zi /Od /I "../inc" /I "..\..\..\inc" /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\../bin/win32i/Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../inc" /d "_DEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "../inc" /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtstmfc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 y:\rel\spcware\v0800018\Sdk\Win32i\ME\lib\msvc42b\siappst.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib\
 /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/sbtstmfc.pdb"\
 /debug /machine:I386 /out:"$(OUTDIR)/sbtstmfc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\sbDoc.obj" \
	"$(INTDIR)\sbtstmfc.obj" \
	"$(INTDIR)\sbtstmfc.res" \
	"$(INTDIR)\sbView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\sbtstmfc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sbtstm2"
# PROP BASE Intermediate_Dir "sbtstm2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin/win32i/Release"
# PROP Intermediate_Dir "../bin/win32i/Release"
# PROP Target_Dir ""
OUTDIR=.\../bin/win32i/Release
INTDIR=.\../bin/win32i/Release

ALL :  "$(OUTDIR)\sbtstmfc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "../inc" /I "y:\rel\spcware\v0800018\Sdk\Win32i\ME\inc\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../inc" /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "../inc" /I "..\..\..\inc" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\../bin/win32i/Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "../inc" /d "NDEBUG" /d "_AFXDLL"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "../inc" /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtstmfc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 y:\rel\spcware\v0800018\Sdk\Win32i\ME\lib\msvc42b\siappst.lib winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib winmm.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib\
 winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/sbtstmfc.pdb" /machine:I386 /out:"$(OUTDIR)/sbtstmfc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\sbDoc.obj" \
	"$(INTDIR)\sbtstmfc.obj" \
	"$(INTDIR)\sbtstmfc.res" \
	"$(INTDIR)\sbView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\sbtstmfc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sbtstmf"
# PROP BASE Intermediate_Dir "sbtstmf"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\Debug"
# PROP Intermediate_Dir "..\bin\win32a\Debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\Debug
INTDIR=.\..\bin\win32a\Debug

ALL : "$(OUTDIR)\sbtstmfc.exe"

CLEAN : 
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\sbDoc.obj"
	-@erase "$(INTDIR)\sbtstmfc.obj"
	-@erase "$(INTDIR)\sbtstmfc.res"
	-@erase "$(INTDIR)\sbView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\sbtstmfc.exe"
	-@erase "$(OUTDIR)\sbtstmfc.ilk"
	-@erase "$(OUTDIR)\sbtstmfc.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MDd /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OS_WIN32"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\Debug/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "_DEBUG" /alpha
# ADD MTL /nologo /D "_DEBUG" /alpha
MTL_PROJ=/nologo /D "_DEBUG" /alpha 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\inc" /d "_DEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "..\inc" /d "_DEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtstmfc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 ../../../lib/siapp/bin/win32a/debug/sthread/siappstd.lib winmm.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=../../../lib/siapp/bin/win32a/debug/sthread/siappstd.lib winmm.lib\
 /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/sbtstmfc.pdb"\
 /debug /machine:ALPHA /out:"$(OUTDIR)/sbtstmfc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\sbDoc.obj" \
	"$(INTDIR)\sbtstmfc.obj" \
	"$(INTDIR)\sbtstmfc.res" \
	"$(INTDIR)\sbView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\sbtstmfc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sbtstm0"
# PROP BASE Intermediate_Dir "sbtstm0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\Release"
# PROP Intermediate_Dir "..\bin\win32a\Release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\Release
INTDIR=.\..\bin\win32a\Release

ALL : "$(OUTDIR)\sbtstmfc.exe"

CLEAN : 
	-@erase "$(INTDIR)\MainFrm.obj"
	-@erase "$(INTDIR)\sbDoc.obj"
	-@erase "$(INTDIR)\sbtstmfc.obj"
	-@erase "$(INTDIR)\sbtstmfc.res"
	-@erase "$(INTDIR)\sbView.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(OUTDIR)\sbtstmfc.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MD /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /YX /c
# ADD CPP /nologo /MD /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MD /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "WIN32"\
 /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "OS_WIN32" /Fo"$(INTDIR)/"\
 /c 
CPP_OBJS=.\..\bin\win32a\Release/
CPP_SBRS=.\.

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

MTL=mktyplib.exe
# ADD BASE MTL /nologo /D "NDEBUG" /alpha
# ADD MTL /nologo /D "NDEBUG" /alpha
MTL_PROJ=/nologo /D "NDEBUG" /alpha 
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /i "..\inc" /d "NDEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "..\inc" /d "NDEBUG" /d\
 "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtstmfc.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 ../../../lib/siapp/bin/win32a/release/sthread/siappst.lib winmm.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=../../../lib/siapp/bin/win32a/release/sthread/siappst.lib\
 winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/sbtstmfc.pdb" /machine:ALPHA /out:"$(OUTDIR)/sbtstmfc.exe" 
LINK32_OBJS= \
	"$(INTDIR)\MainFrm.obj" \
	"$(INTDIR)\sbDoc.obj" \
	"$(INTDIR)\sbtstmfc.obj" \
	"$(INTDIR)\sbtstmfc.res" \
	"$(INTDIR)\sbView.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\sbtstmfc.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "sbtstmfc - Win32 Intel Debug"
# Name "sbtstmfc - Win32 Intel Release"
# Name "sbtstmfc - Win32 (ALPHA) Debug"
# Name "sbtstmfc - Win32 (ALPHA) Release"

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\MainFrm.cpp

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_CPP_MAINF=\
	"..\inc\MainFrm.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_CPP_MAINF=\
	"..\inc\MainFrm.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

DEP_CPP_MAINF=\
	"..\inc\MainFrm.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

DEP_CPP_MAINF=\
	"..\inc\MainFrm.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\MainFrm.obj" : $(SOURCE) $(DEP_CPP_MAINF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_CPP_STDAF=\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_CPP_STDAF=\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

DEP_CPP_STDAF=\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

DEP_CPP_STDAF=\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\StdAfx.obj" : $(SOURCE) $(DEP_CPP_STDAF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\inc\Resource.h

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sbView.cpp

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_CPP_SBVIE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbView.obj" : $(SOURCE) $(DEP_CPP_SBVIE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_CPP_SBVIE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbView.obj" : $(SOURCE) $(DEP_CPP_SBVIE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

DEP_CPP_SBVIE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbView.obj" : $(SOURCE) $(DEP_CPP_SBVIE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

DEP_CPP_SBVIE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbView.obj" : $(SOURCE) $(DEP_CPP_SBVIE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sbtstmfc.cpp

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_CPP_SBTST=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\MainFrm.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbtstmfc.obj" : $(SOURCE) $(DEP_CPP_SBTST) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_CPP_SBTST=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\MainFrm.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbtstmfc.obj" : $(SOURCE) $(DEP_CPP_SBTST) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

DEP_CPP_SBTST=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\MainFrm.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbtstmfc.obj" : $(SOURCE) $(DEP_CPP_SBTST) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

DEP_CPP_SBTST=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\MainFrm.h"\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\sbView.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbtstmfc.obj" : $(SOURCE) $(DEP_CPP_SBTST) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\sbDoc.cpp

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_CPP_SBDOC=\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbDoc.obj" : $(SOURCE) $(DEP_CPP_SBDOC) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_CPP_SBDOC=\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbDoc.obj" : $(SOURCE) $(DEP_CPP_SBDOC) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"

DEP_CPP_SBDOC=\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbDoc.obj" : $(SOURCE) $(DEP_CPP_SBDOC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"

DEP_CPP_SBDOC=\
	"..\inc\sbDoc.h"\
	"..\inc\sbtstmfc.h"\
	"..\inc\stdafx.h"\
	

"$(INTDIR)\sbDoc.obj" : $(SOURCE) $(DEP_CPP_SBDOC) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\res\sbtstmfc.rc

!IF  "$(CFG)" == "sbtstmfc - Win32 Intel Debug"

DEP_RSC_SBTSTM=\
	"..\res\sbDoc.ico"\
	"..\res\sbtstmfc.ico"\
	"..\res\sbtstmfc.rc2"\
	".\..\res\Toolbar.bmp"\
	

"$(INTDIR)\sbtstmfc.res" : $(SOURCE) $(DEP_RSC_SBTSTM) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "../inc" /i\
 "\ud\tools\sbtstmfc\res" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 Intel Release"

DEP_RSC_SBTSTM=\
	"..\res\sbDoc.ico"\
	"..\res\sbtstmfc.ico"\
	"..\res\sbtstmfc.rc2"\
	".\..\res\Toolbar.bmp"\
	

"$(INTDIR)\sbtstmfc.res" : $(SOURCE) $(DEP_RSC_SBTSTM) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "../inc" /i\
 "\ud\tools\sbtstmfc\res" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Debug"


"$(INTDIR)\sbtstmfc.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "..\inc" /i\
 "\ud\tools\sbtstmfc\res" /d "_DEBUG" /d "_AFXDLL" $(SOURCE)


!ELSEIF  "$(CFG)" == "sbtstmfc - Win32 (ALPHA) Release"


"$(INTDIR)\sbtstmfc.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/sbtstmfc.res" /i "..\inc" /i\
 "\ud\tools\sbtstmfc\res" /d "NDEBUG" /d "_AFXDLL" $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
