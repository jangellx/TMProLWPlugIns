# Microsoft Developer Studio Project File - Name="OpenGLTools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=OpenGLTools - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OpenGLTools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OpenGLTools.mak" CFG="OpenGLTools - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OpenGLTools - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "OpenGLTools - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "OpenGLTools - Win32 Release Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "OpenGLTools - Win32 Debug Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "OpenGLTools - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OpenGLTools___Win32_Debug"
# PROP BASE Intermediate_Dir "OpenGLTools___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OpenGLTools___Win32_Debug"
# PROP Intermediate_Dir "OpenGLTools___Win32_Debug"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "OpenGLTools___Win32_Release"
# PROP BASE Intermediate_Dir "OpenGLTools___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "OpenGLTools___Win32_Release"
# PROP Intermediate_Dir "OpenGLTools___Win32_Release"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "OpenGLTools___Win32_Release_Alpha"
# PROP BASE Intermediate_Dir "OpenGLTools___Win32_Release_Alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "OpenGLTools___Win32_Release_Alpha"
# PROP Intermediate_Dir "OpenGLTools___Win32_Release_Alpha"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "OpenGLTools___Win32_Debug_Alpha"
# PROP BASE Intermediate_Dir "OpenGLTools___Win32_Debug_Alpha"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "OpenGLTools___Win32_Debug_Alpha"
# PROP Intermediate_Dir "OpenGLTools___Win32_Debug_Alpha"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /Gt0 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "OpenGLTools - Win32 Debug"
# Name "OpenGLTools - Win32 Release"
# Name "OpenGLTools - Win32 Release Alpha"
# Name "OpenGLTools - Win32 Debug Alpha"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\OpenGLTools-Draw2D.cpp"

!IF  "$(CFG)" == "OpenGLTools - Win32 Debug"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Debug Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\OpenGLTools-Fonts.cpp"

!IF  "$(CFG)" == "OpenGLTools - Win32 Debug"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Debug Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\OpenGLTools-LWIcons.cpp"

!IF  "$(CFG)" == "OpenGLTools - Win32 Debug"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Release Alpha"

!ELSEIF  "$(CFG)" == "OpenGLTools - Win32 Debug Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\OpenGLTools-Draw2D.h"
# End Source File
# Begin Source File

SOURCE=".\OpenGLTools-Fonts.h"
# End Source File
# Begin Source File

SOURCE=".\OpenGLTools-LWIcons.h"
# End Source File
# Begin Source File

SOURCE=.\OpenGLTools.h
# End Source File
# End Group
# End Target
# End Project
