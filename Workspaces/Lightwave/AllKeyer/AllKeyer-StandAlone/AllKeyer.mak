# Microsoft Developer Studio Generated NMAKE File, Based on AllKeyer.dsp
!IF "$(CFG)" == ""
CFG=AllKeyer - Win32 Debug
!MESSAGE No configuration specified. Defaulting to AllKeyer - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "AllKeyer - Win32 Release" && "$(CFG)" !=\
 "AllKeyer - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllKeyer.mak" CFG="AllKeyer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllKeyer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AllKeyer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllKeyer - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
TargetName=AllKeyer
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\AllKeyer.exe"

!ELSE 

ALL : "$(OutDir)\$(TargetName).hlp" "$(OUTDIR)\AllKeyer.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AllKeyer.obj"
	-@erase "$(INTDIR)\AllKeyer.pch"
	-@erase "$(INTDIR)\AllKeyer.res"
	-@erase "$(INTDIR)\AllKeyerDlg.obj"
	-@erase "$(INTDIR)\AllKeyerDlg_Processing.obj"
	-@erase "$(INTDIR)\MotCode.obj"
	-@erase "$(INTDIR)\ProcessScene.obj"
	-@erase "$(INTDIR)\ProcessThread.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(OUTDIR)\AllKeyer.exe"
	-@erase "$(OutDir)\$(TargetName).hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\AllKeyer.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Release/
CPP_SBRS=.
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\AllKeyer.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AllKeyer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)\AllKeyer.pdb" /machine:I386 /out:"$(OUTDIR)\AllKeyer.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AllKeyer.obj" \
	"$(INTDIR)\AllKeyer.res" \
	"$(INTDIR)\AllKeyerDlg.obj" \
	"$(INTDIR)\AllKeyerDlg_Processing.obj" \
	"$(INTDIR)\MotCode.obj" \
	"$(INTDIR)\ProcessScene.obj" \
	"$(INTDIR)\ProcessThread.obj" \
	"$(INTDIR)\StdAfx.obj"

"$(OUTDIR)\AllKeyer.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "AllKeyer - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
TargetName=AllKeyer
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OutDir)\$(TargetName).hlp"\
 "..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.exe"\
 "$(OUTDIR)\AllKeyer.pch"

!ELSE 

ALL : "$(OutDir)\$(TargetName).hlp"\
 "..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.exe"\
 "$(OUTDIR)\AllKeyer.pch"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\AllKeyer.obj"
	-@erase "$(INTDIR)\AllKeyer.pch"
	-@erase "$(INTDIR)\AllKeyer.res"
	-@erase "$(INTDIR)\AllKeyerDlg.obj"
	-@erase "$(INTDIR)\AllKeyerDlg_Processing.obj"
	-@erase "$(INTDIR)\MotCode.obj"
	-@erase "$(INTDIR)\ProcessScene.obj"
	-@erase "$(INTDIR)\ProcessThread.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.exe"
	-@erase "..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.ilk"
	-@erase "..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.pdb"
	-@erase "$(OutDir)\$(TargetName).hlp"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /Gm /GX /Zi /Od /I\
 "C:\Program Files\DevStudio\TMPro_Workspaces\AllKeyer" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /o NUL /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\AllKeyer.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\AllKeyer.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/subsystem:windows /incremental:yes\
 /pdb:"../../../../TMPro_Bin/Lightwave_StandAlone/x86/Release/AllKeyer.pdb"\
 /debug /machine:I386\
 /out:"../../../../TMPro_Bin/Lightwave_StandAlone/x86/Release/AllKeyer.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\AllKeyer.obj" \
	"$(INTDIR)\AllKeyer.res" \
	"$(INTDIR)\AllKeyerDlg.obj" \
	"$(INTDIR)\AllKeyerDlg_Processing.obj" \
	"$(INTDIR)\MotCode.obj" \
	"$(INTDIR)\ProcessScene.obj" \
	"$(INTDIR)\ProcessThread.obj" \
	"$(INTDIR)\StdAfx.obj"

"..\..\..\..\TMPro_Bin\Lightwave_StandAlone\x86\Release\AllKeyer.exe" : \
"$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(CFG)" == "AllKeyer - Win32 Release" || "$(CFG)" ==\
 "AllKeyer - Win32 Debug"
SOURCE=.\AllKeyer.cpp
DEP_CPP_ALLKE=\
	".\AllKeyer.h"\
	".\AllKeyerDlg.h"\
	".\AllKeyerDlg_Processing.h"\
	".\ProcessScene.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AllKeyer.obj" : $(SOURCE) $(DEP_CPP_ALLKE) "$(INTDIR)"\
 "$(INTDIR)\AllKeyer.pch"


SOURCE=.\hlp\AllKeyer.hpj

!IF  "$(CFG)" == "AllKeyer - Win32 Release"

OutDir=.\Release
ProjDir=.
TargetName=AllKeyer
InputPath=.\hlp\AllKeyer.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

!ELSEIF  "$(CFG)" == "AllKeyer - Win32 Debug"

OutDir=.\Debug
ProjDir=.
TargetName=AllKeyer
InputPath=.\hlp\AllKeyer.hpj

"$(OutDir)\$(TargetName).hlp"	 : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	call "$(ProjDir)\makehelp.bat"

!ENDIF 

SOURCE=.\AllKeyer.rc
DEP_RSC_ALLKEY=\
	".\icon1.ico"\
	".\res\AllKeyer.rc2"\
	".\res\OpenFile.bmp"\
	

"$(INTDIR)\AllKeyer.res" : $(SOURCE) $(DEP_RSC_ALLKEY) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\AllKeyerDlg.cpp
DEP_CPP_ALLKEYE=\
	".\AllKeyer.h"\
	".\AllKeyerDlg.h"\
	".\ProcessScene.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AllKeyerDlg.obj" : $(SOURCE) $(DEP_CPP_ALLKEYE) "$(INTDIR)"\
 "$(INTDIR)\AllKeyer.pch"


SOURCE=.\AllKeyerDlg_Processing.cpp
DEP_CPP_ALLKEYER=\
	".\AllKeyer.h"\
	".\AllKeyerDlg_Processing.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\AllKeyerDlg_Processing.obj" : $(SOURCE) $(DEP_CPP_ALLKEYER)\
 "$(INTDIR)" "$(INTDIR)\AllKeyer.pch"


SOURCE=.\MotCode.cpp

"$(INTDIR)\MotCode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AllKeyer.pch"


SOURCE=.\ProcessScene.cpp

"$(INTDIR)\ProcessScene.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\AllKeyer.pch"


SOURCE=.\ProcessThread.cpp
DEP_CPP_PROCE=\
	".\AllKeyer.h"\
	".\ProcessScene.h"\
	".\ProcessThread.h"\
	".\StdAfx.h"\
	

"$(INTDIR)\ProcessThread.obj" : $(SOURCE) $(DEP_CPP_PROCE) "$(INTDIR)"\
 "$(INTDIR)\AllKeyer.pch"


SOURCE=.\StdAfx.cpp
DEP_CPP_STDAF=\
	".\StdAfx.h"\
	

!IF  "$(CFG)" == "AllKeyer - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 "_AFXDLL" /Fp"$(INTDIR)\AllKeyer.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AllKeyer.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "AllKeyer - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /Gm /GX /Zi /Od /I\
 "C:\Program Files\DevStudio\TMPro_Workspaces\AllKeyer" /D "WIN32" /D "_DEBUG"\
 /D "_WINDOWS" /D "_AFXDLL" /Fp"$(INTDIR)\AllKeyer.pch" /Yc"stdafx.h"\
 /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\AllKeyer.pch" : $(SOURCE) $(DEP_CPP_STDAF)\
 "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

