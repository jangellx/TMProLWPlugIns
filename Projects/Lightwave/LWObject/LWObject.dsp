# Microsoft Developer Studio Project File - Name="LWObject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LWObject - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LWObject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LWObject.mak" CFG="LWObject - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LWObject - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LWObject - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LWObject - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LWObject - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "LWObject - Win32 Release"
# Name "LWObject - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\LWbifstream.cpp
# End Source File
# Begin Source File

SOURCE=".\LWObject-Color.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Errors.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Layer.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Point.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Polygon.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Surface.cpp"
# End Source File
# Begin Source File

SOURCE=".\LWObject-VMap.cpp"
# End Source File
# Begin Source File

SOURCE=.\LWObject.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\LWbifstream.h
# End Source File
# Begin Source File

SOURCE=".\LWObject-Chunks.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Color.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Errors.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Layer.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Point.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Polygon.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-Surface.h"
# End Source File
# Begin Source File

SOURCE=".\LWObject-VMap.h"
# End Source File
# Begin Source File

SOURCE=.\LWObject.h
# End Source File
# End Group
# End Target
# End Project
