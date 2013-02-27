# Microsoft Visual C++ Generated NMAKE File, Format Version 2.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

!IF "$(CFG)" == ""
CFG=Win32 Debug
!MESSAGE No configuration specified.  Defaulting to Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "blotch.mak" CFG="Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

################################################################################
# Begin Project
# PROP Target_Last_Scanned "Win32 Debug"
MTL=MkTypLib.exe
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "WinRel"
# PROP BASE Intermediate_Dir "WinRel"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "WinRel"
# PROP Intermediate_Dir "WinRel"
OUTDIR=.\WinRel
INTDIR=.\WinRel

ALL : \lw\blotch.p .\WinRel\blotch.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /YX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /YX /O2 /I "..\..\include" /I "..\..\srclib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /YX /O2 /I "..\..\include" /I "..\..\srclib" /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /FR$(INTDIR)/ /Fp$(OUTDIR)/"blotch.pch"\
 /Fo$(INTDIR)/ /c 
CPP_OBJS=.\WinRel/
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"blotch.bsc" 
BSC32_SBRS= \
	.\WinRel\BLOTCH.SBR \
	.\WinRel\blotchUI.sbr \
	.\WinRel\GUI_HELP.SBR

.\WinRel\blotch.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DLL /MACHINE:I386 /OUT:"c:\lw\blotch.p"
# SUBTRACT LINK32 /INCREMENTAL:yes
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /DLL /INCREMENTAL:no /PDB:$(OUTDIR)/"blotch.pdb"\
 /MACHINE:I386 /DEF:"\MSVC20\Lw\INCLUDE\SERV.DEF" /OUT:"c:\lw\blotch.p"\
 /IMPLIB:$(OUTDIR)/"blotch.lib" 
DEF_FILE=\MSVC20\Lw\INCLUDE\SERV.DEF
LINK32_OBJS= \
	.\WinRel\BLOTCH.OBJ \
	.\WinRel\blotchUI.obj \
	.\WinRel\GUI_HELP.OBJ \
	\MSVC20\Lw\LIB\SERVER.lib \
	\MSVC20\Lw\LIB\SERV_W.lib

\lw\blotch.p : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "WinDebug"
# PROP BASE Intermediate_Dir "WinDebug"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "WinDebug"
# PROP Intermediate_Dir "WinDebug"
OUTDIR=.\WinDebug
INTDIR=.\WinDebug

ALL : \lw\blotch.p .\WinDebug\blotch.bsc

$(OUTDIR) : 
    if not exist $(OUTDIR)/nul mkdir $(OUTDIR)

# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /YX /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
# ADD CPP /nologo /MT /W3 /GX /Zi /YX /Od /I "..\..\include" /I "..\..\srclib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /c
CPP_PROJ=/nologo /MT /W3 /GX /Zi /YX /Od /I "..\..\include" /I "..\..\srclib"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR$(INTDIR)/ /Fp$(OUTDIR)/"blotch.pch"\
 /Fo$(INTDIR)/ /Fd$(OUTDIR)/"blotch.pdb" /c 
CPP_OBJS=.\WinDebug/
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o$(OUTDIR)/"blotch.bsc" 
BSC32_SBRS= \
	.\WinDebug\BLOTCH.SBR \
	.\WinDebug\blotchUI.sbr \
	.\WinDebug\GUI_HELP.SBR

.\WinDebug\blotch.bsc : $(OUTDIR)  $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DLL /DEBUG /MACHINE:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO /SUBSYSTEM:windows /DLL /INCREMENTAL:no /DEBUG /MACHINE:I386 /OUT:"c:\lw\blotch.p"
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /NOLOGO\
 /SUBSYSTEM:windows /DLL /INCREMENTAL:no /PDB:$(OUTDIR)/"blotch.pdb" /DEBUG\
 /MACHINE:I386 /DEF:"\MSVC20\Lw\INCLUDE\SERV.DEF" /OUT:"c:\lw\blotch.p"\
 /IMPLIB:$(OUTDIR)/"blotch.lib" 
DEF_FILE=\MSVC20\Lw\INCLUDE\SERV.DEF
LINK32_OBJS= \
	.\WinDebug\BLOTCH.OBJ \
	.\WinDebug\blotchUI.obj \
	.\WinDebug\GUI_HELP.OBJ \
	\MSVC20\Lw\LIB\SERVER.lib \
	\MSVC20\Lw\LIB\SERV_W.lib

\lw\blotch.p : $(OUTDIR)  $(DEF_FILE) $(LINK32_OBJS)
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

################################################################################
# Begin Group "Source Files"

################################################################################
# Begin Source File

SOURCE=.\BLOTCH.C

!IF  "$(CFG)" == "Win32 Release"

.\WinRel\BLOTCH.OBJ :  $(SOURCE)  $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

.\WinDebug\BLOTCH.OBJ :  $(SOURCE)  $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\MSVC20\Lw\INCLUDE\SERV.DEF
# End Source File
################################################################################
# Begin Source File

SOURCE=.\blotchUI.c
DEP_BLOTC=\
	\MSVC20\Lw\include\splug.h\
	\MSVC20\Lw\include\lwbase.h\
	\MSVC20\Lw\include\lwran.h\
	\MSVC20\Lw\include\lwpanel.h\
	\MSVC20\Lw\srclib\gui_help.h\
	\MSVC20\Lw\include\plug.h\
	\MSVC20\Lw\include\moni.h

!IF  "$(CFG)" == "Win32 Release"

.\WinRel\blotchUI.obj :  $(SOURCE)  $(DEP_BLOTC) $(INTDIR)

!ELSEIF  "$(CFG)" == "Win32 Debug"

.\WinDebug\blotchUI.obj :  $(SOURCE)  $(DEP_BLOTC) $(INTDIR)

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=\MSVC20\Lw\SrcLib\GUI_HELP.C
DEP_GUI_H=\
	\MSVC20\Lw\include\splug.h\
	\MSVC20\Lw\include\lwran.h\
	\MSVC20\Lw\include\lwpanel.h\
	\MSVC20\Lw\srclib\gui_help.h\
	\MSVC20\Lw\include\plug.h\
	\MSVC20\Lw\include\lwbase.h\
	\MSVC20\Lw\include\moni.h

!IF  "$(CFG)" == "Win32 Release"

.\WinRel\GUI_HELP.OBJ :  $(SOURCE)  $(DEP_GUI_H) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ELSEIF  "$(CFG)" == "Win32 Debug"

.\WinDebug\GUI_HELP.OBJ :  $(SOURCE)  $(DEP_GUI_H) $(INTDIR)
   $(CPP) $(CPP_PROJ)  $(SOURCE) 

!ENDIF 

# End Source File
# End Group
################################################################################
# Begin Group "Libs"

################################################################################
# Begin Source File

SOURCE=\MSVC20\Lw\LIB\SERVER.lib
# End Source File
################################################################################
# Begin Source File

SOURCE=\MSVC20\Lw\LIB\SERV_W.lib
# End Source File
# End Group
# End Project
################################################################################
