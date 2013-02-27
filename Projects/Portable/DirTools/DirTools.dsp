# Microsoft Developer Studio Project File - Name="DirTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DirTools - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DirTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DirTools.mak" CFG="DirTools - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DirTools - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DirTools - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "DirTools - Win32 Debug Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "DirTools - Win32 Release Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "DirTools - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Release\DirTools.lib"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Debug\DirTools.lib"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DirTools"
# PROP BASE Intermediate_Dir "DirTools"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DirTools"
# PROP Intermediate_Dir "DirTools"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Debug\DirTools.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\AXP\Debug\DirTools.lib"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "DirTool0"
# PROP BASE Intermediate_Dir "DirTool0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DirTool0"
# PROP Intermediate_Dir "DirTool0"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Release\DirTools.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\AXP\Release\DirTools.lib"

!ENDIF 

# Begin Target

# Name "DirTools - Win32 Release"
# Name "DirTools - Win32 Debug"
# Name "DirTools - Win32 Debug Alpha"
# Name "DirTools - Win32 Release Alpha"
# Begin Group "Source Files"

# PROP Default_Filter "*.c;*.cpp"
# Begin Source File

SOURCE=.\DirInfo.cpp

!IF  "$(CFG)" == "DirTools - Win32 Release"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\DirStrings.cpp

!IF  "$(CFG)" == "DirTools - Win32 Release"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "DirTools - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\DirInfo.h
# End Source File
# Begin Source File

SOURCE=.\DirStrings.h
# End Source File
# Begin Source File

SOURCE=.\DirTools.h
# End Source File
# End Group
# End Target
# End Project
