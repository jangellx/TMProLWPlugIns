# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=siapp - Win32 Win32i Release sthread
!MESSAGE No configuration specified.  Defaulting to siapp - Win32 Win32i\
 Release sthread.
!ENDIF 

!IF "$(CFG)" != "siapp - Win32 Win32i Release sthread" && "$(CFG)" !=\
 "siapp - Win32 Win32i Debug sthread" && "$(CFG)" !=\
 "siapp - Win32 Win32i Release mthread" && "$(CFG)" !=\
 "siapp - Win32 Win32i Debug mthread" && "$(CFG)" !=\
 "siapp - Win32 Win32a Release sthread" && "$(CFG)" !=\
 "siapp - Win32 Win32a Release mthread" && "$(CFG)" !=\
 "siapp - Win32 Win32a Debug mthread" && "$(CFG)" !=\
 "siapp - Win32 Win32a Debug sthread"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "siapp.mak" CFG="siapp - Win32 Win32i Release sthread"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "siapp - Win32 Win32i Release sthread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "siapp - Win32 Win32i Debug sthread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "siapp - Win32 Win32i Release mthread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "siapp - Win32 Win32i Debug mthread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "siapp - Win32 Win32a Release sthread" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "siapp - Win32 Win32a Release mthread" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "siapp - Win32 Win32a Debug mthread" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "siapp - Win32 Win32a Debug sthread" (based on\
 "Win32 (ALPHA) Static Library")
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
# PROP Target_Last_Scanned "siapp - Win32 Win32i Debug mthread"

!IF  "$(CFG)" == "siapp - Win32 Win32i Release sthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "siapp___"
# PROP BASE Intermediate_Dir "siapp___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\siapp\win32i\release\sthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32i\release\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32i\release\sthread
INTDIR=.\..\..\bin\siapp\win32i\release\sthread

ALL :  "$(OUTDIR)\siapp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /W3 /GX /Od /I "../../inc" /I "../../../../inc" /D "WIN32"\
 /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32i\release\sthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\siapp\Release\siapp.lib"
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siapp.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siapp.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug sthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "siapp__0"
# PROP BASE Intermediate_Dir "siapp__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\siapp\win32i\debug\sthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32i\debug\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32i\debug\sthread
INTDIR=.\..\..\bin\siapp\win32i\debug\sthread

ALL :  "$(OUTDIR)\siappd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32i\debug\sthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\siapp\Debug\siappd.lib"
# ADD LIB32 /nologo /out:"..\..\bin\siapp\win32i\debug\sthread\siappd.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siappd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siappd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Release mthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "siapp__1"
# PROP BASE Intermediate_Dir "siapp__1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\siapp\win32i\release\mthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32i\release\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32i\release\mthread
INTDIR=.\..\..\bin\siapp\win32i\release\mthread

ALL :  "$(OUTDIR)\siapp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MT /W3 /GX /Od /I "../../inc" /I "../../../../inc" /D "WIN32"\
 /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32i\release\mthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\siapp\Release\siapp.lib"
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siapp.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siapp.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug mthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "siapp__2"
# PROP BASE Intermediate_Dir "siapp__2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\siapp\win32i\debug\mthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32i\debug\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32i\debug\mthread
INTDIR=.\..\..\bin\siapp\win32i\debug\mthread

ALL :  "$(OUTDIR)\siappd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D\
 "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32i\debug\mthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\siapp\Debug\siappd.lib"
# ADD LIB32 /nologo /out:"..\..\bin\siapp\win32i\debug\mthread\siappd.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siappd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siappd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release sthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "siapp___"
# PROP BASE Intermediate_Dir "siapp___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\siapp\win32a\release\sthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32a\release\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32a\release\sthread
INTDIR=.\..\..\bin\siapp\win32a\release\sthread

ALL : "$(OUTDIR)\siapp.lib"

CLEAN : 
	-@erase "$(INTDIR)\siapp.obj"
	-@erase "$(OUTDIR)\siapp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32a\release\sthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siapp.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siapp.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release mthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "siapp__0"
# PROP BASE Intermediate_Dir "siapp__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\bin\siapp\win32a\release\mthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32a\release\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32a\release\mthread
INTDIR=.\..\..\bin\siapp\win32a\release\mthread

ALL : "$(OUTDIR)\siapp.lib"

CLEAN : 
	-@erase "$(INTDIR)\siapp.obj"
	-@erase "$(OUTDIR)\siapp.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /I "../../inc" /I "../../../../inc" /D\
 "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32a\release\mthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siapp.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siapp.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug mthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "siapp__1"
# PROP BASE Intermediate_Dir "siapp__1"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\siapp\win32a\debug\mthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32a\debug\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32a\debug\mthread
INTDIR=.\..\..\bin\siapp\win32a\debug\mthread

ALL : "$(OUTDIR)\siappd.lib"

CLEAN : 
	-@erase "$(INTDIR)\siapp.obj"
	-@erase "$(OUTDIR)\siappd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32a\debug\mthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\siapp\win32a\debug\mthread\siappd.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siappd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siappd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug sthread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "siapp__2"
# PROP BASE Intermediate_Dir "siapp__2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\bin\siapp\win32a\debug\sthread"
# PROP Intermediate_Dir "..\..\bin\siapp\win32a\debug\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\..\bin\siapp\win32a\debug\sthread
INTDIR=.\..\..\bin\siapp\win32a\debug\sthread

ALL : "$(OUTDIR)\siappd.lib"

CLEAN : 
	-@erase "$(INTDIR)\siapp.obj"
	-@erase "$(OUTDIR)\siappd.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /c
# SUBTRACT CPP /YX
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Z7 /Od /I "../../inc" /I "../../../../inc"\
 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "OS_WIN32" /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\..\bin\siapp\win32a\debug\sthread/
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

BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/siapp.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\siapp\win32a\debug\sthread\siappd.lib"
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/siappd.lib" 
LIB32_OBJS= \
	"$(INTDIR)\siapp.obj"

"$(OUTDIR)\siappd.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "siapp - Win32 Win32i Release sthread"
# Name "siapp - Win32 Win32i Debug sthread"
# Name "siapp - Win32 Win32i Release mthread"
# Name "siapp - Win32 Win32i Debug mthread"
# Name "siapp - Win32 Win32a Release sthread"
# Name "siapp - Win32 Win32a Release mthread"
# Name "siapp - Win32 Win32a Debug mthread"
# Name "siapp - Win32 Win32a Debug sthread"

!IF  "$(CFG)" == "siapp - Win32 Win32i Release sthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug sthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Release mthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug mthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release sthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release mthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug mthread"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug sthread"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\siapp.c

!IF  "$(CFG)" == "siapp - Win32 Win32i Release sthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug sthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Release mthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32i Debug mthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release sthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Release mthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug mthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "siapp - Win32 Win32a Debug sthread"

DEP_CPP_SIAPP=\
	".\../../../../inc\si.h"\
	".\../../../../inc\spwdata.h"\
	".\../../../../inc\spwerror.h"\
	".\../../../../inc\spwmacro.h"\
	".\../../../../inc\spwplat.h"\
	".\siapp.h"\
	

"$(INTDIR)\siapp.obj" : $(SOURCE) $(DEP_CPP_SIAPP) "$(INTDIR)"


!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
