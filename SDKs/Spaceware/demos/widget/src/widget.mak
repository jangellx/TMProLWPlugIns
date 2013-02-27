# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=widget - Win32 win32a Debug
!MESSAGE No configuration specified.  Defaulting to widget - Win32 win32a\
 Debug.
!ENDIF 

!IF "$(CFG)" != "widget - Win32 win32i Debug" && "$(CFG)" !=\
 "widget - Win32 win32i Release" && "$(CFG)" != "widget - Win32 win32a Debug" &&\
 "$(CFG)" != "widget - Win32 win32a Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "widget.mak" CFG="widget - Win32 win32a Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "widget - Win32 win32i Debug" (based on "Win32 (x86) Application")
!MESSAGE "widget - Win32 win32i Release" (based on "Win32 (x86) Application")
!MESSAGE "widget - Win32 win32a Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "widget - Win32 win32a Release" (based on "Win32 (ALPHA) Application")
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
# PROP Target_Last_Scanned "widget - Win32 win32a Debug"

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "widget__"
# PROP BASE Intermediate_Dir "widget__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32i\debug"
# PROP Intermediate_Dir "..\bin\win32i\debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\debug
INTDIR=.\..\bin\win32i\debug

ALL :    "$(OUTDIR)\widget.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /I\
 "..\..\..\lib\slim\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\debug/
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/widget.res" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/widget.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\lib\siapp\bin\win32i\debug\ME\sthread\siappstd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib\
 /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/widget.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/widget.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ntgraph.obj" \
	"$(INTDIR)\widget.obj" \
	"$(INTDIR)\widget.res" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\widget.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "widget_0"
# PROP BASE Intermediate_Dir "widget_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32i\release"
# PROP Intermediate_Dir "..\bin\win32i\release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\release
INTDIR=.\..\bin\win32i\release

ALL :    "$(OUTDIR)\widget.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /I\
 "..\..\..\lib\slim\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32"\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\release/
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
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/widget.res" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/widget.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\lib\siapp\bin\win32i\release\sthread\ME\siappst.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib\
 /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/widget.pdb"\
 /machine:I386 /out:"$(OUTDIR)/widget.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ntgraph.obj" \
	"$(INTDIR)\widget.obj" \
	"$(INTDIR)\widget.res" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\widget.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "widget__"
# PROP BASE Intermediate_Dir "widget__"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\debug"
# PROP Intermediate_Dir "..\bin\win32a\debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\debug
INTDIR=.\..\bin\win32a\debug

ALL : "$(OUTDIR)\widget.exe"

CLEAN : 
	-@erase "$(INTDIR)\ntgraph.obj"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\widget.obj"
	-@erase "$(INTDIR)\widget.res"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(OUTDIR)\widget.exe"
	-@erase "$(OUTDIR)\widget.ilk"
	-@erase "$(OUTDIR)\widget.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Zi /Od /I "..\inc" /I "..\..\..\inc" /I\
 "..\..\..\lib\slim\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32"\
 /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/widget.res" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/widget.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib\
 /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/widget.pdb" /debug\
 /machine:ALPHA /out:"$(OUTDIR)/widget.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ntgraph.obj" \
	"$(INTDIR)\widget.obj" \
	"$(INTDIR)\widget.res" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\widget.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "widget_0"
# PROP BASE Intermediate_Dir "widget_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\release"
# PROP Intermediate_Dir "..\bin\win32a\release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\release
INTDIR=.\..\bin\win32a\release

ALL : "$(OUTDIR)\widget.exe"

CLEAN : 
	-@erase "$(INTDIR)\ntgraph.obj"
	-@erase "$(INTDIR)\widget.obj"
	-@erase "$(INTDIR)\widget.res"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(OUTDIR)\widget.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\inc" /I\
 "..\..\..\lib\slim\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32"\
 /Fo"$(INTDIR)/" /c 
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
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/widget.res" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/widget.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib winmm.lib /nologo /subsystem:windows /machine:ALPHA
# SUBTRACT LINK32 /pdb:none
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib ..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib\
 winmm.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/widget.pdb" /machine:ALPHA /out:"$(OUTDIR)/widget.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ntgraph.obj" \
	"$(INTDIR)\widget.obj" \
	"$(INTDIR)\widget.res" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\widget.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "widget - Win32 win32i Debug"
# Name "widget - Win32 win32i Release"
# Name "widget - Win32 win32a Debug"
# Name "widget - Win32 win32a Release"

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\winmain.c

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\widget.c

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

DEP_CPP_WIDGE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\widget.obj" : $(SOURCE) $(DEP_CPP_WIDGE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

DEP_CPP_WIDGE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\widget.obj" : $(SOURCE) $(DEP_CPP_WIDGE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

DEP_CPP_WIDGE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\widget.obj" : $(SOURCE) $(DEP_CPP_WIDGE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

DEP_CPP_WIDGE=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\widget.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\widget.obj" : $(SOURCE) $(DEP_CPP_WIDGE) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\ntgraph.c

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

DEP_CPP_NTGRA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\ntgraph.obj" : $(SOURCE) $(DEP_CPP_NTGRA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

DEP_CPP_NTGRA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\ntgraph.obj" : $(SOURCE) $(DEP_CPP_NTGRA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

DEP_CPP_NTGRA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\ntgraph.obj" : $(SOURCE) $(DEP_CPP_NTGRA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

DEP_CPP_NTGRA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\ntgraph.obj" : $(SOURCE) $(DEP_CPP_NTGRA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\res\widget.rc

!IF  "$(CFG)" == "widget - Win32 win32i Debug"

DEP_RSC_WIDGET=\
	"..\res\widget.ico"\
	

"$(INTDIR)\widget.res" : $(SOURCE) $(DEP_RSC_WIDGET) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/widget.res" /i "\ud\demos\widget\res"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "widget - Win32 win32i Release"

DEP_RSC_WIDGET=\
	"..\res\widget.ico"\
	

"$(INTDIR)\widget.res" : $(SOURCE) $(DEP_RSC_WIDGET) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/widget.res" /i "\ud\demos\widget\res"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "widget - Win32 win32a Debug"

DEP_RSC_WIDGET=\
	"..\res\widget.ico"\
	

"$(INTDIR)\widget.res" : $(SOURCE) $(DEP_RSC_WIDGET) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/widget.res" /i "\ud\demos\widget\res"\
 $(SOURCE)


!ELSEIF  "$(CFG)" == "widget - Win32 win32a Release"

DEP_RSC_WIDGET=\
	"..\res\widget.ico"\
	

"$(INTDIR)\widget.res" : $(SOURCE) $(DEP_RSC_WIDGET) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/widget.res" /i "\ud\demos\widget\res"\
 $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
