# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101
# TARGTYPE "Win32 (ALPHA) Application" 0x0601

!IF "$(CFG)" == ""
CFG=cityfly - Win32 win32i Debug
!MESSAGE No configuration specified.  Defaulting to cityfly - Win32 win32i\
 Debug.
!ENDIF 

!IF "$(CFG)" != "cityfly - Win32 win32i Debug" && "$(CFG)" !=\
 "cityfly - Win32 win32i Release" && "$(CFG)" != "cityfly - Win32 win32a Debug"\
 && "$(CFG)" != "cityfly - Win32 win32a Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "cityfly.mak" CFG="cityfly - Win32 win32i Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cityfly - Win32 win32i Debug" (based on "Win32 (x86) Application")
!MESSAGE "cityfly - Win32 win32i Release" (based on "Win32 (x86) Application")
!MESSAGE "cityfly - Win32 win32a Debug" (based on "Win32 (ALPHA) Application")
!MESSAGE "cityfly - Win32 win32a Release" (based on\
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
# PROP Target_Last_Scanned "cityfly - Win32 win32a Release"

!IF  "$(CFG)" == "cityfly - Win32 win32i Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Intel Debug"
# PROP BASE Intermediate_Dir "Intel Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32i\debug"
# PROP Intermediate_Dir "..\bin\win32i\debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\debug
INTDIR=.\..\bin\win32i\debug

ALL :   "$(OUTDIR)\cityflyd.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\dat" /I "..\res" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "_DEBUG" /D "OS_WIN32" /D "WIN32" /D "_WINDOWS" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /I "..\inc" /I "..\dat" /I "..\res"\
 /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "_DEBUG" /D "OS_WIN32" /D\
 "WIN32" /D "_WINDOWS" /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\res" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cityfly.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"..\bin\win32i\debug\cityflyd.exe"
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\debug\sthread\siappstd.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)/cityflyd.pdb" /debug\
 /machine:I386 /out:"$(OUTDIR)/cityflyd.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bwin.res" \
	"$(INTDIR)\cityfly.obj" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\cityflyd.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32i Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Intel Release"
# PROP BASE Intermediate_Dir "Intel Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32i\release"
# PROP Intermediate_Dir "..\bin\win32i\release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\release
INTDIR=.\..\bin\win32i\release

ALL :   "$(OUTDIR)\cityfly.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\dat" /I "..\res" /I "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "NDEBUG" /D "OS_WIN32" /D "WIN32" /D "_WINDOWS" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\inc" /I "..\dat" /I "..\res" /I\
 "..\..\..\inc" /I "..\..\..\lib\slim\inc" /D "NDEBUG" /D "OS_WIN32" /D "WIN32"\
 /D "_WINDOWS" /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\res" /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cityfly.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32i\release\sthread\siappst.lib\
 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/cityfly.pdb" /machine:I386 /out:"$(OUTDIR)/cityfly.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bwin.res" \
	"$(INTDIR)\cityfly.obj" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\cityfly.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "cityfly1"
# PROP BASE Intermediate_Dir "cityfly1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\debug"
# PROP Intermediate_Dir "..\bin\win32a\debug"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\debug
INTDIR=.\..\bin\win32a\debug

ALL : "$(OUTDIR)\cityflyd.exe"

CLEAN : 
	-@erase "$(INTDIR)\bwin.res"
	-@erase "$(INTDIR)\cityfly.obj"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(OUTDIR)\cityflyd.exe"
	-@erase "$(OUTDIR)\cityflyd.ilk"
	-@erase "$(OUTDIR)\cityflyd.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /GX /Zi /Od /I "..\inc" /I "..\res" /I "..\dat" /I "..\..\..\lib\slim\inc" /I "..\..\..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "ibm_pc" /D "ibm_pc_nt" /D "ibm_pc_winnt" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /Gt0 /GX /Zi /Od /I "..\inc" /I "..\res" /I "..\dat" /I\
 "..\..\..\lib\slim\inc" /I "..\..\..\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "ibm_pc" /D "ibm_pc_nt" /D "ibm_pc_winnt" /D "OS_WIN32" /Fo"$(INTDIR)/"\
 /Fd"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\inc" /i "..\res" /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\inc" /i "..\res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cityfly.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 ..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:ALPHA /out:"..\bin\win32a\debug/cityflyd.exe"
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32a\debug\sthread\siappstd.lib winmm.lib\
 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib\
 shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo\
 /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)/cityflyd.pdb" /debug\
 /machine:ALPHA /out:"$(OUTDIR)/cityflyd.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bwin.res" \
	"$(INTDIR)\cityfly.obj" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\cityflyd.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "cityfly3"
# PROP BASE Intermediate_Dir "cityfly3"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\release"
# PROP Intermediate_Dir "..\bin\win32a\release"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\release
INTDIR=.\..\bin\win32a\release

ALL : "$(OUTDIR)\cityfly.exe"

CLEAN : 
	-@erase "$(INTDIR)\bwin.res"
	-@erase "$(INTDIR)\cityfly.obj"
	-@erase "$(INTDIR)\winmain.obj"
	-@erase "$(OUTDIR)\cityfly.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /GX /O2 /I "..\inc" /I "..\res" /I "..\dat" /I "..\..\..\lib\slim\inc" /I "..\..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "ibm_pc" /D "ibm_pc_nt" /D "ibm_pc_winnt" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /Gt0 /GX /O2 /I "..\inc" /I "..\res" /I "..\dat" /I\
 "..\..\..\lib\slim\inc" /I "..\..\..\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "ibm_pc" /D "ibm_pc_nt" /D "ibm_pc_winnt" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
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
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\inc" /i "..\res" /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\inc" /i "..\res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/cityfly.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
# ADD LINK32 ..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:ALPHA
LINK32_FLAGS=..\..\..\lib\siapp\bin\win32a\release\sthread\siappst.lib\
 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/cityfly.pdb" /machine:ALPHA /out:"$(OUTDIR)/cityfly.exe" 
LINK32_OBJS= \
	"$(INTDIR)\bwin.res" \
	"$(INTDIR)\cityfly.obj" \
	"$(INTDIR)\winmain.obj"

"$(OUTDIR)\cityfly.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "cityfly - Win32 win32i Debug"
# Name "cityfly - Win32 win32i Release"
# Name "cityfly - Win32 win32a Debug"
# Name "cityfly - Win32 win32a Release"

!IF  "$(CFG)" == "cityfly - Win32 win32i Debug"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32i Release"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Debug"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Release"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\winmain.c

!IF  "$(CFG)" == "cityfly - Win32 win32i Debug"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\cityfly.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32i Release"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\cityfly.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Debug"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\cityfly.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Release"

DEP_CPP_WINMA=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\inc\cityfly.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	

"$(INTDIR)\winmain.obj" : $(SOURCE) $(DEP_CPP_WINMA) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\cityfly.c

!IF  "$(CFG)" == "cityfly - Win32 win32i Debug"

DEP_CPP_CITYF=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\dat\bldgs.dat"\
	"..\dat\land.dat"\
	"..\inc\cityfly.h"\
	"..\inc\icon.h"\
	"..\inc\iconmask.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	
NODEP_CPP_CITYF=\
	".\platform.h"\
	".\SPW_Bw.h"\
	".\x11input.h"\
	

"$(INTDIR)\cityfly.obj" : $(SOURCE) $(DEP_CPP_CITYF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32i Release"

DEP_CPP_CITYF=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\dat\bldgs.dat"\
	"..\dat\land.dat"\
	"..\inc\cityfly.h"\
	"..\inc\icon.h"\
	"..\inc\iconmask.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	
NODEP_CPP_CITYF=\
	".\platform.h"\
	".\SPW_Bw.h"\
	".\x11input.h"\
	

"$(INTDIR)\cityfly.obj" : $(SOURCE) $(DEP_CPP_CITYF) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Debug"

DEP_CPP_CITYF=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\dat\bldgs.dat"\
	"..\dat\land.dat"\
	"..\inc\cityfly.h"\
	"..\inc\icon.h"\
	"..\inc\iconmask.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	
NODEP_CPP_CITYF=\
	".\platform.h"\
	".\SPW_Bw.h"\
	".\x11input.h"\
	

"$(INTDIR)\cityfly.obj" : $(SOURCE) $(DEP_CPP_CITYF) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Release"

DEP_CPP_CITYF=\
	"..\..\..\inc\si.h"\
	"..\..\..\inc\spwmacro.h"\
	"..\..\..\inc\spwplat.h"\
	"..\..\..\lib\slim\inc\spwslim.h"\
	"..\dat\bldgs.dat"\
	"..\dat\land.dat"\
	"..\inc\cityfly.h"\
	"..\inc\icon.h"\
	"..\inc\iconmask.h"\
	".\..\..\..\inc\spwdata.h"\
	".\..\..\..\inc\spwerror.h"\
	
NODEP_CPP_CITYF=\
	".\platform.h"\
	".\SPW_Bw.h"\
	".\x11input.h"\
	

"$(INTDIR)\cityfly.obj" : $(SOURCE) $(DEP_CPP_CITYF) "$(INTDIR)"


!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=..\res\bwin.rc

!IF  "$(CFG)" == "cityfly - Win32 win32i Debug"

DEP_RSC_BWIN_=\
	".\..\res\icon2.ico"\
	".\..\res\s64.bmp"\
	

"$(INTDIR)\bwin.res" : $(SOURCE) $(DEP_RSC_BWIN_) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\res" /i\
 "\ud\demos\cityfly\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cityfly - Win32 win32i Release"

DEP_RSC_BWIN_=\
	".\..\res\icon2.ico"\
	".\..\res\s64.bmp"\
	

"$(INTDIR)\bwin.res" : $(SOURCE) $(DEP_RSC_BWIN_) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\res" /i\
 "\ud\demos\cityfly\res" /d "NDEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Debug"

DEP_RSC_BWIN_=\
	".\..\res\icon2.ico"\
	".\..\res\s64.bmp"\
	

"$(INTDIR)\bwin.res" : $(SOURCE) $(DEP_RSC_BWIN_) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\inc" /i "..\res" /i\
 "\ud\demos\cityfly\res" /d "_DEBUG" $(SOURCE)


!ELSEIF  "$(CFG)" == "cityfly - Win32 win32a Release"

DEP_RSC_BWIN_=\
	".\..\res\icon2.ico"\
	".\..\res\s64.bmp"\
	

"$(INTDIR)\bwin.res" : $(SOURCE) $(DEP_RSC_BWIN_) "$(INTDIR)"
   $(RSC) /l 0x409 /fo"$(INTDIR)/bwin.res" /i "..\inc" /i "..\res" /i\
 "\ud\demos\cityfly\res" /d "NDEBUG" $(SOURCE)


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
