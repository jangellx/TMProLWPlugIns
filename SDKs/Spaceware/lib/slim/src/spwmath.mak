# Microsoft Developer Studio Generated NMAKE File, Format Version 4.20
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

!IF "$(CFG)" == ""
CFG=spwmath - Win32 win32i Debug
!MESSAGE No configuration specified.  Defaulting to spwmath - Win32 win32i\
 Debug.
!ENDIF 

!IF "$(CFG)" != "spwmath - Win32 win32i Debug" && "$(CFG)" !=\
 "spwmath - Win32 win32i Release" && "$(CFG)" != "spwmath - Win32 win32a Debug"\
 && "$(CFG)" != "spwmath - Win32 win32a Release" && "$(CFG)" !=\
 "spwmath - Win32 win32i Debug MThread" && "$(CFG)" !=\
 "spwmath - Win32 win32i Release MThread" && "$(CFG)" !=\
 "spwmath - Win32 win32a Debug MThread" && "$(CFG)" !=\
 "spwmath - Win32 win32a Release MThread"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "spwmath.mak" CFG="spwmath - Win32 win32i Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "spwmath - Win32 win32i Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "spwmath - Win32 win32i Release" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "spwmath - Win32 win32a Debug" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "spwmath - Win32 win32a Release" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "spwmath - Win32 win32i Debug MThread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "spwmath - Win32 win32i Release MThread" (based on\
 "Win32 (x86) Static Library")
!MESSAGE "spwmath - Win32 win32a Debug MThread" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "spwmath - Win32 win32a Release MThread" (based on\
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
# PROP Target_Last_Scanned "spwmath - Win32 win32i Release MThread"

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Intel Debug"
# PROP BASE Intermediate_Dir "Intel Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32i\debug\sthread"
# PROP Intermediate_Dir "..\bin\win32i\debug\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\debug\sthread
INTDIR=.\..\bin\win32i\debug\sthread

ALL : "$(OUTDIR)\spwmath.lib"

CLEAN : 
	-@erase "$(INTDIR)\arbaxtom.obj"
	-@erase "$(INTDIR)\compmat.obj"
	-@erase "$(INTDIR)\copymat.obj"
	-@erase "$(INTDIR)\crosprod.obj"
	-@erase "$(INTDIR)\dotprod.obj"
	-@erase "$(INTDIR)\getmatsc.obj"
	-@erase "$(INTDIR)\gettranm.obj"
	-@erase "$(INTDIR)\highvalf.obj"
	-@erase "$(INTDIR)\initmat.obj"
	-@erase "$(INTDIR)\invmat.obj"
	-@erase "$(INTDIR)\latupv.obj"
	-@erase "$(INTDIR)\m13x33.obj"
	-@erase "$(INTDIR)\m14x44.obj"
	-@erase "$(INTDIR)\m2lauvec.obj"
	-@erase "$(INTDIR)\m33x33.obj"
	-@erase "$(INTDIR)\m44x44.obj"
	-@erase "$(INTDIR)\matdeter.obj"
	-@erase "$(INTDIR)\mfl14x44.obj"
	-@erase "$(INTDIR)\mkidentm.obj"
	-@erase "$(INTDIR)\mtoarbax.obj"
	-@erase "$(INTDIR)\normvec.obj"
	-@erase "$(INTDIR)\orthnorm.obj"
	-@erase "$(INTDIR)\printmat.obj"
	-@erase "$(INTDIR)\printsbd.obj"
	-@erase "$(INTDIR)\printvec.obj"
	-@erase "$(INTDIR)\scalemat.obj"
	-@erase "$(INTDIR)\slim.obj"
	-@erase "$(INTDIR)\trposmat.obj"
	-@erase "$(INTDIR)\veclen.obj"
	-@erase "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/spwmath.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\debug\sthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Intel Release"
# PROP BASE Intermediate_Dir "Intel Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32i\release\sthread"
# PROP Intermediate_Dir "..\bin\win32i\release\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\release\sthread
INTDIR=.\..\bin\win32i\release\sthread

ALL : "$(OUTDIR)\spwmath.lib"

CLEAN : 
	-@erase "$(INTDIR)\arbaxtom.obj"
	-@erase "$(INTDIR)\compmat.obj"
	-@erase "$(INTDIR)\copymat.obj"
	-@erase "$(INTDIR)\crosprod.obj"
	-@erase "$(INTDIR)\dotprod.obj"
	-@erase "$(INTDIR)\getmatsc.obj"
	-@erase "$(INTDIR)\gettranm.obj"
	-@erase "$(INTDIR)\highvalf.obj"
	-@erase "$(INTDIR)\initmat.obj"
	-@erase "$(INTDIR)\invmat.obj"
	-@erase "$(INTDIR)\latupv.obj"
	-@erase "$(INTDIR)\m13x33.obj"
	-@erase "$(INTDIR)\m14x44.obj"
	-@erase "$(INTDIR)\m2lauvec.obj"
	-@erase "$(INTDIR)\m33x33.obj"
	-@erase "$(INTDIR)\m44x44.obj"
	-@erase "$(INTDIR)\matdeter.obj"
	-@erase "$(INTDIR)\mfl14x44.obj"
	-@erase "$(INTDIR)\mkidentm.obj"
	-@erase "$(INTDIR)\mtoarbax.obj"
	-@erase "$(INTDIR)\normvec.obj"
	-@erase "$(INTDIR)\orthnorm.obj"
	-@erase "$(INTDIR)\printmat.obj"
	-@erase "$(INTDIR)\printsbd.obj"
	-@erase "$(INTDIR)\printvec.obj"
	-@erase "$(INTDIR)\scalemat.obj"
	-@erase "$(INTDIR)\slim.obj"
	-@erase "$(INTDIR)\trposmat.obj"
	-@erase "$(INTDIR)\veclen.obj"
	-@erase "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/spwmath.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\release\sthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "spwmath___2"
# PROP BASE Intermediate_Dir "spwmath___2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\debug\sthread"
# PROP Intermediate_Dir "..\bin\win32a\debug\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\debug\sthread
INTDIR=.\..\bin\win32a\debug\sthread

ALL :  "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
CPP_PROJ=/nologo /MLd /Gt0 /W3 /GX /Z7 /Od /I "..\inc" /I\
 "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I\
 "y:\party3\exceed\win32a\xdk\include\X11" /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "OS_WIN32" /Fp"$(INTDIR)/spwmath.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\debug\sthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "spwmath___8"
# PROP BASE Intermediate_Dir "spwmath___8"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\release\sthread"
# PROP Intermediate_Dir "..\bin\win32a\release\sthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\release\sthread
INTDIR=.\..\bin\win32a\release\sthread

ALL :  "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
CPP_PROJ=/nologo /ML /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /I\
 "y:\party3\exceed\win32a\xdk\include" /I\
 "y:\party3\exceed\win32a\xdk\include\X11" /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "OS_WIN32" /Fp"$(INTDIR)/spwmath.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\release\sthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "spwmath_"
# PROP BASE Intermediate_Dir "spwmath_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32i\debug\mthread"
# PROP Intermediate_Dir "..\bin\win32i\debug\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\debug\mthread
INTDIR=.\..\bin\win32i\debug\mthread

ALL : "$(OUTDIR)\spwmath.lib"

CLEAN : 
	-@erase "$(INTDIR)\arbaxtom.obj"
	-@erase "$(INTDIR)\compmat.obj"
	-@erase "$(INTDIR)\copymat.obj"
	-@erase "$(INTDIR)\crosprod.obj"
	-@erase "$(INTDIR)\dotprod.obj"
	-@erase "$(INTDIR)\getmatsc.obj"
	-@erase "$(INTDIR)\gettranm.obj"
	-@erase "$(INTDIR)\highvalf.obj"
	-@erase "$(INTDIR)\initmat.obj"
	-@erase "$(INTDIR)\invmat.obj"
	-@erase "$(INTDIR)\latupv.obj"
	-@erase "$(INTDIR)\m13x33.obj"
	-@erase "$(INTDIR)\m14x44.obj"
	-@erase "$(INTDIR)\m2lauvec.obj"
	-@erase "$(INTDIR)\m33x33.obj"
	-@erase "$(INTDIR)\m44x44.obj"
	-@erase "$(INTDIR)\matdeter.obj"
	-@erase "$(INTDIR)\mfl14x44.obj"
	-@erase "$(INTDIR)\mkidentm.obj"
	-@erase "$(INTDIR)\mtoarbax.obj"
	-@erase "$(INTDIR)\normvec.obj"
	-@erase "$(INTDIR)\orthnorm.obj"
	-@erase "$(INTDIR)\printmat.obj"
	-@erase "$(INTDIR)\printsbd.obj"
	-@erase "$(INTDIR)\printvec.obj"
	-@erase "$(INTDIR)\scalemat.obj"
	-@erase "$(INTDIR)\slim.obj"
	-@erase "$(INTDIR)\trposmat.obj"
	-@erase "$(INTDIR)\veclen.obj"
	-@erase "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\spacetec\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MTd /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /D\
 "_DEBUG" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/spwmath.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\debug\mthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "spwmath0"
# PROP BASE Intermediate_Dir "spwmath0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32i\release\mthread"
# PROP Intermediate_Dir "..\bin\win32i\release\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32i\release\mthread
INTDIR=.\..\bin\win32i\release\mthread

ALL : "$(OUTDIR)\spwmath.lib"

CLEAN : 
	-@erase "$(INTDIR)\arbaxtom.obj"
	-@erase "$(INTDIR)\compmat.obj"
	-@erase "$(INTDIR)\copymat.obj"
	-@erase "$(INTDIR)\crosprod.obj"
	-@erase "$(INTDIR)\dotprod.obj"
	-@erase "$(INTDIR)\getmatsc.obj"
	-@erase "$(INTDIR)\gettranm.obj"
	-@erase "$(INTDIR)\highvalf.obj"
	-@erase "$(INTDIR)\initmat.obj"
	-@erase "$(INTDIR)\invmat.obj"
	-@erase "$(INTDIR)\latupv.obj"
	-@erase "$(INTDIR)\m13x33.obj"
	-@erase "$(INTDIR)\m14x44.obj"
	-@erase "$(INTDIR)\m2lauvec.obj"
	-@erase "$(INTDIR)\m33x33.obj"
	-@erase "$(INTDIR)\m44x44.obj"
	-@erase "$(INTDIR)\matdeter.obj"
	-@erase "$(INTDIR)\mfl14x44.obj"
	-@erase "$(INTDIR)\mkidentm.obj"
	-@erase "$(INTDIR)\mtoarbax.obj"
	-@erase "$(INTDIR)\normvec.obj"
	-@erase "$(INTDIR)\orthnorm.obj"
	-@erase "$(INTDIR)\printmat.obj"
	-@erase "$(INTDIR)\printsbd.obj"
	-@erase "$(INTDIR)\printvec.obj"
	-@erase "$(INTDIR)\scalemat.obj"
	-@erase "$(INTDIR)\slim.obj"
	-@erase "$(INTDIR)\trposmat.obj"
	-@erase "$(INTDIR)\veclen.obj"
	-@erase "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "..\inc" /I "..\..\spacetec\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MT /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /D\
 "NDEBUG" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)/spwmath.pch" /YX\
 /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32i\release\mthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "spwmath_"
# PROP BASE Intermediate_Dir "spwmath_"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin\win32a\debug\mthread"
# PROP Intermediate_Dir "..\bin\win32a\debug\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\debug\mthread
INTDIR=.\..\bin\win32a\debug\mthread

ALL :  "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /D "HOST_TO_NETWORK" /YX /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Z7 /Od /I "..\inc" /I "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
CPP_PROJ=/nologo /MTd /Gt0 /W3 /GX /Z7 /Od /I "..\inc" /I\
 "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I\
 "y:\party3\exceed\win32a\xdk\include\X11" /D "_DEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "OS_WIN32" /Fp"$(INTDIR)/spwmath.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\debug\mthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "spwmath0"
# PROP BASE Intermediate_Dir "spwmath0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin\win32a\release\mthread"
# PROP Intermediate_Dir "..\bin\win32a\release\mthread"
# PROP Target_Dir ""
OUTDIR=.\..\bin\win32a\release\mthread
INTDIR=.\..\bin\win32a\release\mthread

ALL :  "$(OUTDIR)\spwmath.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /D "HOST_TO_NETWORK" /YX /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /I "y:\party3\exceed\win32a\xdk\include" /I "y:\party3\exceed\win32a\xdk\include\X11" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "OS_WIN32" /YX /c
CPP_PROJ=/nologo /MT /Gt0 /W3 /GX /O2 /I "..\inc" /I "..\..\..\spacetec\inc" /I\
 "y:\party3\exceed\win32a\xdk\include" /I\
 "y:\party3\exceed\win32a\xdk\include\X11" /D "NDEBUG" /D "WIN32" /D "_WINDOWS"\
 /D "OS_WIN32" /Fp"$(INTDIR)/spwmath.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\..\bin\win32a\release\mthread/
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
BSC32_FLAGS=/nologo /o"$(OUTDIR)/spwmath.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
LIB32_FLAGS=/nologo /out:"$(OUTDIR)/spwmath.lib" 
LIB32_OBJS= \
	"$(INTDIR)\arbaxtom.obj" \
	"$(INTDIR)\compmat.obj" \
	"$(INTDIR)\copymat.obj" \
	"$(INTDIR)\crosprod.obj" \
	"$(INTDIR)\dotprod.obj" \
	"$(INTDIR)\getmatsc.obj" \
	"$(INTDIR)\gettranm.obj" \
	"$(INTDIR)\highvalf.obj" \
	"$(INTDIR)\initmat.obj" \
	"$(INTDIR)\invmat.obj" \
	"$(INTDIR)\latupv.obj" \
	"$(INTDIR)\m13x33.obj" \
	"$(INTDIR)\m14x44.obj" \
	"$(INTDIR)\m2lauvec.obj" \
	"$(INTDIR)\m33x33.obj" \
	"$(INTDIR)\m44x44.obj" \
	"$(INTDIR)\matdeter.obj" \
	"$(INTDIR)\mfl14x44.obj" \
	"$(INTDIR)\mkidentm.obj" \
	"$(INTDIR)\mtoarbax.obj" \
	"$(INTDIR)\normvec.obj" \
	"$(INTDIR)\orthnorm.obj" \
	"$(INTDIR)\printmat.obj" \
	"$(INTDIR)\printsbd.obj" \
	"$(INTDIR)\printvec.obj" \
	"$(INTDIR)\scalemat.obj" \
	"$(INTDIR)\slim.obj" \
	"$(INTDIR)\trposmat.obj" \
	"$(INTDIR)\veclen.obj"

"$(OUTDIR)\spwmath.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 

################################################################################
# Begin Target

# Name "spwmath - Win32 win32i Debug"
# Name "spwmath - Win32 win32i Release"
# Name "spwmath - Win32 win32a Debug"
# Name "spwmath - Win32 win32a Release"
# Name "spwmath - Win32 win32i Debug MThread"
# Name "spwmath - Win32 win32i Release MThread"
# Name "spwmath - Win32 win32a Debug MThread"
# Name "spwmath - Win32 win32a Release MThread"

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\copymat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_COPYM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\copymat.obj" : $(SOURCE) $(DEP_CPP_COPYM) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\crosprod.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_CROSP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\crosprod.obj" : $(SOURCE) $(DEP_CPP_CROSP) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\dotprod.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_DOTPR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\dotprod.obj" : $(SOURCE) $(DEP_CPP_DOTPR) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\highvalf.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_HIGHV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\highvalf.obj" : $(SOURCE) $(DEP_CPP_HIGHV) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\initmat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_INITM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\initmat.obj" : $(SOURCE) $(DEP_CPP_INITM) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\invmat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_INVMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\invmat.obj" : $(SOURCE) $(DEP_CPP_INVMA) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\latupv.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_LATUP=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\latupv.obj" : $(SOURCE) $(DEP_CPP_LATUP) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m13x33.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_M13X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m13x33.obj" : $(SOURCE) $(DEP_CPP_M13X3) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m14x44.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_M14X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m14x44.obj" : $(SOURCE) $(DEP_CPP_M14X4) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m2lauvec.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_M2LAU=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m2lauvec.obj" : $(SOURCE) $(DEP_CPP_M2LAU) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m33x33.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_M33X3=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m33x33.obj" : $(SOURCE) $(DEP_CPP_M33X3) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\m44x44.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_M44X4=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\m44x44.obj" : $(SOURCE) $(DEP_CPP_M44X4) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mfl14x44.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_MFL14=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mfl14x44.obj" : $(SOURCE) $(DEP_CPP_MFL14) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mkidentm.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_MKIDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mkidentm.obj" : $(SOURCE) $(DEP_CPP_MKIDE) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\mtoarbax.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_MTOAR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\mtoarbax.obj" : $(SOURCE) $(DEP_CPP_MTOAR) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\normvec.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_NORMV=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\normvec.obj" : $(SOURCE) $(DEP_CPP_NORMV) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\arbaxtom.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_ARBAX=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\arbaxtom.obj" : $(SOURCE) $(DEP_CPP_ARBAX) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printvec.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_PRINT=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printvec.obj" : $(SOURCE) $(DEP_CPP_PRINT) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\orthnorm.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_ORTHN=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\orthnorm.obj" : $(SOURCE) $(DEP_CPP_ORTHN) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printmat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_PRINTM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printmat.obj" : $(SOURCE) $(DEP_CPP_PRINTM) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\printsbd.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_PRINTS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\printsbd.obj" : $(SOURCE) $(DEP_CPP_PRINTS) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\trposmat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_TRPOS=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\trposmat.obj" : $(SOURCE) $(DEP_CPP_TRPOS) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\veclen.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_VECLE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\veclen.obj" : $(SOURCE) $(DEP_CPP_VECLE) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\matdeter.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_MATDE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\matdeter.obj" : $(SOURCE) $(DEP_CPP_MATDE) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\compmat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_COMPM=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\compmat.obj" : $(SOURCE) $(DEP_CPP_COMPM) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\gettranm.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_GETTR=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\gettranm.obj" : $(SOURCE) $(DEP_CPP_GETTR) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\getmatsc.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_GETMA=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\getmatsc.obj" : $(SOURCE) $(DEP_CPP_GETMA) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\scalemat.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_SCALE=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\scalemat.obj" : $(SOURCE) $(DEP_CPP_SCALE) "$(INTDIR)"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\slim.c

!IF  "$(CFG)" == "spwmath - Win32 win32i Debug"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Debug MThread"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32i Release MThread"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Debug MThread"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"

!ELSEIF  "$(CFG)" == "spwmath - Win32 win32a Release MThread"

DEP_CPP_SLIM_=\
	"..\..\..\spacetec\inc\spwmacro.h"\
	"..\..\..\spacetec\inc\spwplat.h"\
	"..\inc\spwslim.h"\
	

"$(INTDIR)\slim.obj" : $(SOURCE) $(DEP_CPP_SLIM_) "$(INTDIR)"

!ENDIF 

# End Source File
# End Target
# End Project
################################################################################
