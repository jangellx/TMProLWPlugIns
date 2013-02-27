# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=zor - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to zor - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "zor - Win32 Release" && "$(CFG)" != "zor - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "zor.mak" CFG="zor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "zor - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "zor - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Target_Last_Scanned "zor - Win32 Debug"
RSC=rc.exe
CPP=cl.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "zor - Win32 Release"

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

ALL : ".\zor.p"

CLEAN : 
	-@erase "$(INTDIR)\zor.obj"
	-@erase "$(OUTDIR)\zor.exp"
	-@erase "$(OUTDIR)\zor.lib"
	-@erase ".\zor.p"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/zor.pch" /YX /Fo"$(INTDIR)/" /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/zor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /machine:I386 /nodefaultlib:"libc" /out:"zor.p"
# SUBTRACT LINK32 /incremental:yes
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /incremental:no\
 /pdb:"$(OUTDIR)/zor.pdb" /machine:I386 /nodefaultlib:"libc"\
 /def:"\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def" /out:"zor.p"\
 /implib:"$(OUTDIR)/zor.lib" 
DEF_FILE= \
	"..\..\..\..\..\include\serv.def"
LINK32_OBJS= \
	"$(INTDIR)\zor.obj" \
	"..\..\..\..\..\lib\serv_w.obj" \
	"..\..\..\..\..\lib\server.lib"

".\zor.p" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "zor - Win32 Debug"

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

ALL : ".\zor.p"

CLEAN : 
	-@erase "$(INTDIR)\vc40.idb"
	-@erase "$(INTDIR)\vc40.pdb"
	-@erase "$(INTDIR)\zor.obj"
	-@erase "$(OUTDIR)\zor.exp"
	-@erase "$(OUTDIR)\zor.lib"
	-@erase "$(OUTDIR)\zor.pdb"
	-@erase ".\zor.ilk"
	-@erase ".\zor.p"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/zor.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/zor.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /debug /machine:I386 /nodefaultlib:"libc" /out:"zor.p"
# SUBTRACT LINK32 /incremental:no
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib libcmt.lib /nologo /subsystem:windows /dll /incremental:yes\
 /pdb:"$(OUTDIR)/zor.pdb" /debug /machine:I386 /nodefaultlib:"libc"\
 /def:"\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def" /out:"zor.p"\
 /implib:"$(OUTDIR)/zor.lib" 
DEF_FILE= \
	"..\..\..\..\..\include\serv.def"
LINK32_OBJS= \
	"$(INTDIR)\zor.obj" \
	"..\..\..\..\..\lib\serv_w.obj" \
	"..\..\..\..\..\lib\server.lib"

".\zor.p" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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

# Name "zor - Win32 Release"
# Name "zor - Win32 Debug"

!IF  "$(CFG)" == "zor - Win32 Release"

!ELSEIF  "$(CFG)" == "zor - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\sample\layout\zor\zor.c
DEP_CPP_ZOR_C=\
	{$(INCLUDE)}"\lwbase.h"\
	{$(INCLUDE)}"\lwran.h"\
	{$(INCLUDE)}"\moni.h"\
	{$(INCLUDE)}"\plug.h"\
	{$(INCLUDE)}"\splug.h"\
	

"$(INTDIR)\zor.obj" : $(SOURCE) $(DEP_CPP_ZOR_C) "$(INTDIR)"
   $(CPP) $(CPP_PROJ) $(SOURCE)


# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\lib\server.lib

!IF  "$(CFG)" == "zor - Win32 Release"

!ELSEIF  "$(CFG)" == "zor - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\lib\serv_w.obj

!IF  "$(CFG)" == "zor - Win32 Release"

!ELSEIF  "$(CFG)" == "zor - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\lw\DEV\lwsdk\PLUGSRC\sdks\include\serv.def

!IF  "$(CFG)" == "zor - Win32 Release"

!ELSEIF  "$(CFG)" == "zor - Win32 Debug"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
