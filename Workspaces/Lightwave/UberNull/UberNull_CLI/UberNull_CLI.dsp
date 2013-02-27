# Microsoft Developer Studio Project File - Name="UberNull_CLI" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103
# TARGTYPE "Win32 (ALPHA) Console Application" 0x0603

CFG=UberNull_CLI - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UberNull_CLI.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UberNull_CLI.mak" CFG="UberNull_CLI - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UberNull_CLI - Win32 Release Alpha" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UberNull_CLI - Win32 Debug Alpha" (based on "Win32 (ALPHA) Console Application")
!MESSAGE "UberNull_CLI - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "UberNull_CLI - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Workspaces/Lightwave/UberNull/UberNull_CLI", KOBAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "UberNull_CLI - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UberNull_CLI"
# PROP BASE Intermediate_Dir "UberNull_CLI"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UberNull_CLI"
# PROP Intermediate_Dir "UberNull_CLI"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /pdb:"UberNull_CLI_AXP_Release/UberNull_CLI.pdb" /machine:ALPHA /out:"../../../../Bin/Lightwave_StandAlone\AXP\Release\UberNull.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "UberNul0"
# PROP BASE Intermediate_Dir "UberNul0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "UberNul0"
# PROP Intermediate_Dir "UberNul0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /debug /machine:ALPHA
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:console /pdb:"UberNull_CLI_Debug_AXP/UberNull_CLI.pdb" /debug /machine:ALPHA /out:"../../../../Bin/Lightwave_StandAlone\AXP\Debug\UberNull.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"UberNull_CLI_Debug_AXP/UberNull_CLI.pdb" /debug /machine:IX86 /out:"../../../../Bin/Lightwave_StandAlone\AXP\Debug\UberNull.exe" /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"UberNull_CLI_Debug_AXP/UberNull_CLI.pdb" /debug /machine:IX86 /out:"../../../../Bin/Lightwave_StandAlone\x86\Debug\UberNull.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none /incremental:no

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "UberNul1"
# PROP BASE Intermediate_Dir "UberNul1"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "UberNul1"
# PROP Intermediate_Dir "UberNul1"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"UberNull_CLI_AXP_Release/UberNull_CLI.pdb" /machine:IX86 /out:"../../../../Bin/Lightwave_StandAlone\AXP\Release\UberNull.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /pdb:"UberNull_CLI_AXP_Release/UberNull_CLI.pdb" /machine:IX86 /out:"../../../../Bin/Lightwave_StandAlone\x86\Release\UberNull.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "UberNull_CLI - Win32 Release Alpha"
# Name "UberNull_CLI - Win32 Debug Alpha"
# Name "UberNull_CLI - Win32 Debug"
# Name "UberNull_CLI - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp;*.c"
# Begin Source File

SOURCE=.\UberNull_CLI.cpp

!IF  "$(CFG)" == "UberNull_CLI - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Debug"

!ELSEIF  "$(CFG)" == "UberNull_CLI - Win32 Release"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# End Group
# End Target
# End Project
