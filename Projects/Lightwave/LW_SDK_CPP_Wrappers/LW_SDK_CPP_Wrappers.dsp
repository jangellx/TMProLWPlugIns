# Microsoft Developer Studio Project File - Name="LW_SDK_CPP_Wrappers" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LW_SDK_CPP_Wrappers - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LW_SDK_CPP_Wrappers.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LW_SDK_CPP_Wrappers.mak" CFG="LW_SDK_CPP_Wrappers - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LW_SDK_CPP_Wrappers - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LW_SDK_CPP_Wrappers - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LW_SDK_CPP_Wrappers - Win32 Debug Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "LW_SDK_CPP_Wrappers - Win32 Release Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

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
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\Lib\Lightwave\x86\Release\LW_SDK_CPP_Wrappers.lib"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

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
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\Lib\Lightwave\x86\Debug\LWWrappers.lib"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LW_SDK_C"
# PROP BASE Intermediate_Dir "LW_SDK_C"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LW_SDK_C"
# PROP Intermediate_Dir "LW_SDK_C"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Lightwave\x86\Debug\LWWrappers.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Lightwave\AXP\Debug\LWWrappers.lib"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LW_SDK_0"
# PROP BASE Intermediate_Dir "LW_SDK_0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LW_SDK_0"
# PROP Intermediate_Dir "LW_SDK_0"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Lightwave\x86\Release\LW_SDK_CPP_Wrappers.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Lightwave\AXP\Release\LW_SDK_CPP_Wrappers.lib"

!ENDIF 

# Begin Target

# Name "LW_SDK_CPP_Wrappers - Win32 Release"
# Name "LW_SDK_CPP_Wrappers - Win32 Debug"
# Name "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"
# Name "LW_SDK_CPP_Wrappers - Win32 Release Alpha"
# Begin Group "Source Files"

# PROP Default_Filter "*.c;*.cpp"
# Begin Source File

SOURCE=.\Clwpanel.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

DEP_CPP_CLWPA=\
	".\Clwpanel.h"\
	
NODEP_CPP_CLWPA=\
	".\lug.h"\
	".\oni.h"\
	".\wbase.h"\
	".\wpanel.h"\
	

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Clwran.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

DEP_CPP_CLWRA=\
	".\Clwran.h"\
	
NODEP_CPP_CLWRA=\
	".\lug.h"\
	".\oni.h"\
	".\wbase.h"\
	".\wran.h"\
	

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\sample\common\panels\gui_help.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

NODEP_CPP_GUI_H=\
	".\lug.h"\
	".\oni.h"\
	".\plug.h"\
	".\ui_help.h"\
	".\wbase.h"\
	".\wpanel.h"\
	".\wran.h"\
	

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PanelDraw.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

DEP_CPP_PANEL=\
	".\Clwpanel.h"\
	".\Clwran.h"\
	".\LWWrappers.h"\
	".\PanelDraw.h"\
	".\PanelTools.h"\
	
NODEP_CPP_PANEL=\
	".\ImageClass.h"\
	".\lug.h"\
	".\oni.h"\
	".\ortable\DynArray\DynArray.h"\
	".\ortable\Image\Image.h"\
	".\ortable\Image\ImageEnums.h"\
	".\ortable\Image\IO\BMP.h"\
	".\ortable\Image\IO\ImageIOEnums.h"\
	".\ortable\Image\IO\IO.h"\
	".\ortable\Image\Process\Checkerboard.h"\
	".\ortable\Image\Process\Process.h"\
	".\ui_help.h"\
	".\wbase.h"\
	".\wpanel.h"\
	".\wran.h"\
	

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\PanelTools.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

DEP_CPP_PANELT=\
	".\PanelTools.h"\
	
NODEP_CPP_PANELT=\
	".\lug.h"\
	".\oni.h"\
	".\wbase.h"\
	".\wpanel.h"\
	

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RasterDraw.cpp

!IF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "LW_SDK_CPP_Wrappers - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Clwpanel.h
# End Source File
# Begin Source File

SOURCE=.\Clwran.h
# End Source File
# Begin Source File

SOURCE=.\LWWrappers.h
# End Source File
# Begin Source File

SOURCE=.\LWWrappers_L6.h
# End Source File
# Begin Source File

SOURCE=.\PanelDraw.h
# End Source File
# Begin Source File

SOURCE=.\PanelTools.h
# End Source File
# Begin Source File

SOURCE=.\RasterDraw.h
# End Source File
# End Group
# End Target
# End Project
