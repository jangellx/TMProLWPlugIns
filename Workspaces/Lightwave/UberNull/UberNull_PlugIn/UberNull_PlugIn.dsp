# Microsoft Developer Studio Project File - Name="UberNull_PlugIn" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=UberNull_PlugIn - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UberNull_PlugIn.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UberNull_PlugIn.mak" CFG="UberNull_PlugIn - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UberNull_PlugIn - Win32 Debug Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "UberNull_PlugIn - Win32 Release Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "UberNull_PlugIn - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UberNull_PlugIn - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Workspaces/Lightwave/UberNull/UberNull_PlugIn", XPBAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\AXP\Debug\UberNull.p"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

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
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
CPP=cl.exe
# ADD BASE CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\AXP\Release\UberNull.p"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UberNull"
# PROP BASE Intermediate_Dir "UberNull"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UberNull"
# PROP Intermediate_Dir "UberNull"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"../../../../Bin/Lightwave_PlugIns\AXP\Debug\UberNull.p" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:IX86 /out:"../../../../Bin/Lightwave_PlugIns\x86\Debug\UberNull.p" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UberNul0"
# PROP BASE Intermediate_Dir "UberNul0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UberNul0"
# PROP Intermediate_Dir "UberNul0"
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:IX86 /out:"../../../../Bin/Lightwave_PlugIns\AXP\Release\UberNull.p"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:IX86 /out:"../../../../Bin/Lightwave_PlugIns\x86\Release\UberNull.p"

!ENDIF 

# Begin Target

# Name "UberNull_PlugIn - Win32 Debug Alpha"
# Name "UberNull_PlugIn - Win32 Release Alpha"
# Name "UberNull_PlugIn - Win32 Debug"
# Name "UberNull_PlugIn - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp;*.c"
# Begin Source File

SOURCE=.\UberNull.c

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UberNullBase.cpp

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UberNullInterface.cpp

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UberNullLogo.cpp

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\UberNull.h
# End Source File
# Begin Source File

SOURCE=.\UberNullInterface.h
# End Source File
# Begin Source File

SOURCE=.\UberNullLogo.h
# End Source File
# End Group
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Group "Libs -- Debug x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Debug\serv_w.obj

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Debug\server.lib

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Release\serv_w.obj

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Release\server.lib

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Debug AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Debug\serv_w.obj

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Debug\server.lib

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Release\serv_w.obj

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Release\server.lib

!IF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "UberNull_PlugIn - Win32 Release"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
