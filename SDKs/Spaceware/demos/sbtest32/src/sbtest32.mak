# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=sbtest32 - Win32 Intel Debug
!MESSAGE No configuration specified.  Defaulting to sbtest32 - Win32 Intel\
 Debug.
!ENDIF 

!IF "$(CFG)" != "sbtest32 - Win32 Intel Debug" && "$(CFG)" !=\
 "sbtest32 - Win32 Intel Release" && "$(CFG)" != "sbtest32 - Win32 Alpha Debug"\
 && "$(CFG)" != "sbtest32 - Win32 Alpha Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "sbtest32.mak" CFG="sbtest32 - Win32 Intel Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sbtest32 - Win32 Intel Debug" (based on "Win32 (x86) Application")
!MESSAGE "sbtest32 - Win32 Intel Release" (based on "Win32 (x86) Application")
!MESSAGE "sbtest32 - Win32 Alpha Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "sbtest32 - Win32 Alpha Release" (based on\
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
# PROP Target_Last_Scanned "sbtest32 - Win32 Alpha Release"

!IF  "$(CFG)" == "sbtest32 - Win32 Intel Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sbtest30"
# PROP BASE Intermediate_Dir "sbtest30"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin/win32i/Debug"
# PROP Intermediate_Dir "../bin/win32i/Debug"
# PROP Target_Dir ""
OUTDIR=.\../bin/win32i/Debug
INTDIR=.\../bin/win32i/Debug

ALL : "$(OUTDIR)\sbtest32.exe"

CLEAN : 
	-@erase "$(INTDIR)\sbtest32.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\sbtest32.exe"
	-@erase "$(OUTDIR)\sbtest32.ilk"
	-@erase "$(OUTDIR)\sbtest32.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\inc" /I "y:\rel\spcware\v0800018\SDK\Win32i\ME\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
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
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtest32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 y:\rel\spcware\v0800018\Sdk\Win32i\ME\lib\msvc42b\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/sbtest32.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/sbtest32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\sbtest32.obj"

"$(OUTDIR)\sbtest32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Intel Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sbtest31"
# PROP BASE Intermediate_Dir "sbtest31"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin/win32i/Release"
# PROP Intermediate_Dir "../bin/win32i/Release"
# PROP Target_Dir ""
OUTDIR=.\../bin/win32i/Release
INTDIR=.\../bin/win32i/Release

ALL : "$(OUTDIR)\sbtest32.exe"

CLEAN : 
	-@erase "$(INTDIR)\sbtest32.obj"
	-@erase "$(OUTDIR)\sbtest32.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\inc" /I "y:\rel\spcware\v0800018\SDK\Win32i\ME\inc\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "WIN32" /D\
 "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/sbtest32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 y:\rel\spcware\v0800018\Sdk\Win32i\ME\lib\msvc42b\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib\
 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/sbtest32.pdb" /machine:I386 /out:"$(OUTDIR)/sbtest32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\sbtest32.obj"

"$(OUTDIR)\sbtest32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "sbtest31"
# PROP BASE Intermediate_Dir "sbtest31"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\debug"
# PROP Intermediate_Dir "..\bin\win32a\debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\debug
INTDIR=.\..\bin\win32a\debug

ALL :   "$(OUTDIR)\sbtest32.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/"\
 /c 
CPP_OBJS=.\..\bin\win32a\debug/
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
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"sbtest32.bsc"
BSC32_FLAGS=/nologo /o"sbtest32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 ..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/sbtest32.pdb" /debug\
 /machine:ALPHA /out:"$(OUTDIR)/sbtest32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\sbtest32.obj"

"$(OUTDIR)\sbtest32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "sbtest33"
# PROP BASE Intermediate_Dir "sbtest33"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\release"
# PROP Intermediate_Dir "..\bin\win32a\release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\release
INTDIR=.\..\bin\win32a\release

ALL :   "$(OUTDIR)\sbtest32.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX /Yc /Yu
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "NDEBUG"\
 /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\release/
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
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"sbtest32.bsc"
BSC32_FLAGS=/nologo /o"sbtest32.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 ..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib\
 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/sbtest32.pdb" /machine:ALPHA /out:"$(OUTDIR)/sbtest32.exe" 
LINK32_OBJS= \
	"$(INTDIR)\sbtest32.obj"

"$(OUTDIR)\sbtest32.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "sbtest32 - Win32 Intel Debug"
# Name "sbtest32 - Win32 Intel Release"
# Name "sbtest32 - Win32 Alpha Debug"
# Name "sbtest32 - Win32 Alpha Release"

!IF  "$(CFG)" == "sbtest32 - Win32 Intel Debug"

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Intel Release"

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Debug"

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\sbtest32.c

!IF  "$(CFG)" == "sbtest32 - Win32 Intel Debug"

DEP_CPP_SBTES=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbtest32.h"\
	

"$(INTDIR)\sbtest32.obj" : $(SOURCE) $(DEP_CPP_SBTES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Intel Release"

DEP_CPP_SBTES=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbtest32.h"\
	

"$(INTDIR)\sbtest32.obj" : $(SOURCE) $(DEP_CPP_SBTES) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Debug"

DEP_CPP_SBTES=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbtest32.h"\
	

"$(INTDIR)\sbtest32.obj" : $(SOURCE) $(DEP_CPP_SBTES) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "sbtest32 - Win32 Alpha Release"

DEP_CPP_SBTES=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwdata.h"\
	"..\..\..\inc\spwerror.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\inc\sbtest32.h"\
	

"$(INTDIR)\sbtest32.obj" : $(SOURCE) $(DEP_CPP_SBTES) "$(INTDIR)"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
