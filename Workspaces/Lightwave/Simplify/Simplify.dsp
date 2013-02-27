# Microsoft Developer Studio Project File - Name="Simplify" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=Simplify - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Simplify.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simplify.mak" CFG="Simplify - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simplify - Win32 Debug Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Simplify - Win32 Release Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "Simplify - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Simplify - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug Alpha"
# PROP BASE Intermediate_Dir "Debug Alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug Alpha"
# PROP Intermediate_Dir "Debug Alpha"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"../../../Bin/Lightwave_PlugIns/AXP/Debug/Simplify.p"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release Alpha"
# PROP BASE Intermediate_Dir "Release Alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release Alpha"
# PROP Intermediate_Dir "Release Alpha"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /Zm400 /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"../../../Bin/Lightwave_PlugIns/AXP/Release/Simplify.p"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Simplify"
# PROP BASE Intermediate_Dir "Simplify"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Simplify"
# PROP Intermediate_Dir "Simplify"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"../../../Bin/Lightwave_PlugIns/AXP/Debug/Simplify.p" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"../../../Bin/Lightwave_PlugIns/x86/Debug/Simplify.p" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Simplif0"
# PROP BASE Intermediate_Dir "Simplif0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Simplif0"
# PROP Intermediate_Dir "Simplif0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:IX86 /out:"../../../Bin/Lightwave_PlugIns/AXP/Release/Simplify.p"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:IX86 /out:"../../../Bin/Lightwave_PlugIns/x86/Release/Simplify.p"

!ENDIF 

# Begin Target

# Name "Simplify - Win32 Debug Alpha"
# Name "Simplify - Win32 Release Alpha"
# Name "Simplify - Win32 Debug"
# Name "Simplify - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp;*.c"
# Begin Source File

SOURCE=.\Simplify.c

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_Config.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_Dongle.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_Generic.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_GlobalControl_Interface.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep_Instance.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep_Interface.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep_InterfaceKeys.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SimplifyLogo.cpp

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Simplify.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_Config.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_Dongle.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_Generic.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_GlobalControl_Interface.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep_Instance.h
# End Source File
# Begin Source File

SOURCE=.\Simplify_ObjRep_Interface.h
# End Source File
# Begin Source File

SOURCE=.\SimplifyLogo.h
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Group "Libs -- Debug x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Debug\serv_w.obj

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Debug\server.lib

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Release\serv_w.obj

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Release\server.lib

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Debug AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Debug\serv_w.obj

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Debug\server.lib

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Release\serv_w.obj

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Release\server.lib

!IF  "$(CFG)" == "Simplify - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "Simplify - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "Simplify - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
