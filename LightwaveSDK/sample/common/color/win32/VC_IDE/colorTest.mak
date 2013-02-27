# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=colorTest - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to colorTest - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "colorTest - Win32 Release" && "$(CFG)" !=\
 "colorTest - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "colorTest.mak" CFG="colorTest - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "colorTest - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE "colorTest - Win32 Debug" (based on\
 "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "colorTest - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "colorTest - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\colorTest.p"

CLEAN : 
	-@erase "$(INTDIR)\colortest.obj"
	-@erase "$(INTDIR)\gui_help.obj"
	-@erase "$(OUTDIR)\colorTest.exp"
	-@erase "$(OUTDIR)\colorTest.lib"
	-@erase ".\colorTest.p"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/colorTest.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/colorTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc" /out:"colorTest.p"
# SUBTRACT LINK32 /incremental:yes /debug
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/colorTest.pdb" /machine:I386 /nodefaultlib:"libc"\
 /def:"\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def" /out:"colorTest.p"\
 /implib:"$(OUTDIR)/colorTest.lib" 
DEF_FILE= \
	"..\..\..\..\..\include\serv.def"
LINK32_OBJS= \
	"$(INTDIR)\colortest.obj" \
	"$(INTDIR)\gui_help.obj" \
	"..\..\..\..\..\lib\serv_w.obj" \
	"..\..\..\..\..\lib\server.lib"

".\colorTest.p" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "colorTest - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\colorTest.p"

CLEAN : 
	-@erase "$(INTDIR)\colortest.obj"
	-@erase "$(INTDIR)\gui_help.obj"
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(OUTDIR)\colorTest.exp"
	-@erase "$(OUTDIR)\colorTest.lib"
	-@erase "$(OUTDIR)\colorTest.pdb"
	-@erase ".\colorTest.ilk"
	-@erase ".\colorTest.p"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/colorTest.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.\.
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/colorTest.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc" /out:"colorTest.p"
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/colorTest.pdb" /debug /machine:I386 /nodefaultlib:"libc"\
 /def:"\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def" /out:"colorTest.p"\
 /implib:"$(OUTDIR)/colorTest.lib" 
DEF_FILE= \
	"..\..\..\..\..\include\serv.def"
LINK32_OBJS= \
	"$(INTDIR)\colortest.obj" \
	"$(INTDIR)\gui_help.obj" \
	"..\..\..\..\..\lib\serv_w.obj" \
	"..\..\..\..\..\lib\server.lib"

".\colorTest.p" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

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

################################################################################
# Begin Target

# Name "colorTest - Win32 Release"
# Name "colorTest - Win32 Debug"

!IF  "$(CFG)" == "colorTest - Win32 Release"

!ELSEIF  "$(CFG)" == "colorTest - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\sample\common\color\colortest.c
DEP_CPP_COLOR=\
	{$(INCLUDE)}"\gcolorpicker.h"\
	{$(INCLUDE)}"\gui_help.h"\
	{$(INCLUDE)}"\lwbase.h"\
	{$(INCLUDE)}"\lwpanel.h"\
	{$(INCLUDE)}"\lwran.h"\
	{$(INCLUDE)}"\moni.h"\
	{$(INCLUDE)}"\plug.h"\
	{$(INCLUDE)}"\splug.h"\
	

"$(INTDIR)\colortest.obj" : $(SOURCE) $(DEP_CPP_COLOR) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\sample\common\panels\gui_help.c
DEP_CPP_GUI_H=\
	{$(INCLUDE)}"\gui_help.h"\
	{$(INCLUDE)}"\lwbase.h"\
	{$(INCLUDE)}"\lwpanel.h"\
	{$(INCLUDE)}"\lwran.h"\
	{$(INCLUDE)}"\moni.h"\
	{$(INCLUDE)}"\plug.h"\
	{$(INCLUDE)}"\splug.h"\
	

"$(INTDIR)\gui_help.obj" : $(SOURCE) $(DEP_CPP_GUI_H) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\lib\server.lib

!IF  "$(CFG)" == "colorTest - Win32 Release"

!ELSEIF  "$(CFG)" == "colorTest - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\lib\serv_w.obj

!IF  "$(CFG)" == "colorTest - Win32 Release"

!ELSEIF  "$(CFG)" == "colorTest - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def

!IF  "$(CFG)" == "colorTest - Win32 Release"

!ELSEIF  "$(CFG)" == "colorTest - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
