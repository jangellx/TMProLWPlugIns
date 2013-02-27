# Microsoft Developer Studio Project File - Name="pfstream" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pfstream - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pfstream.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pfstream.mak" CFG="pfstream - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pfstream - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "pfstream - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "pfstream - Win32 Debug Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "pfstream - Win32 Release Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "pfstream - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Release\pfstream.lib"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Debug/pfstream.lib"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "pfstream"
# PROP BASE Intermediate_Dir "pfstream"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "pfstream"
# PROP Intermediate_Dir "pfstream"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Debug/pfstream.lib"
# ADD LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Debug/pfstream.lib"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "pfstrea0"
# PROP BASE Intermediate_Dir "pfstrea0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "pfstrea0"
# PROP Intermediate_Dir "pfstrea0"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Release\pfstream.lib"
# ADD LIB32 /nologo /out:"../../../TMPro_Lib/Portable/x86/Release\pfstream.lib"

!ENDIF 

# Begin Target

# Name "pfstream - Win32 Release"
# Name "pfstream - Win32 Debug"
# Name "pfstream - Win32 Debug Alpha"
# Name "pfstream - Win32 Release Alpha"
# Begin Group "Source Files"

# PROP Default_Filter "*.cpp"
# Begin Source File

SOURCE=.\EOL_Manager\eol_manager.cpp

!IF  "$(CFG)" == "pfstream - Win32 Release"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pifstream\pifstream.cpp

!IF  "$(CFG)" == "pfstream - Win32 Release"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\pofstream\pofstream.cpp

!IF  "$(CFG)" == "pfstream - Win32 Release"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "pfstream - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\EOL_Manager\eol_manager.h
# End Source File
# Begin Source File

SOURCE=.\pfstream.h
# End Source File
# Begin Source File

SOURCE=.\pifstream\pifstream.h
# End Source File
# Begin Source File

SOURCE=.\pofstream\pofstream.h
# End Source File
# End Group
# End Target
# End Project
