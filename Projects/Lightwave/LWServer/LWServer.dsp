# Microsoft Developer Studio Project File - Name="LWServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=LWServer - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LWServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LWServer.mak" CFG="LWServer - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LWServer - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "LWServer - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "LWServer - Win32 Debug Alpha" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE "LWServer - Win32 Release Alpha" (based on\
 "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "LWServer - Win32 Release"

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
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

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
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "LWServer"
# PROP BASE Intermediate_Dir "LWServer"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "LWServer"
# PROP Intermediate_Dir "LWServer"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "LWServe0"
# PROP BASE Intermediate_Dir "LWServe0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "LWServe0"
# PROP Intermediate_Dir "LWServe0"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "LWServer - Win32 Release"
# Name "LWServer - Win32 Debug"
# Name "LWServer - Win32 Debug Alpha"
# Name "LWServer - Win32 Release Alpha"
# Begin Group "Libs -- Debug x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Debug\serv_w.obj

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Debug\server.lib

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Release\serv_w.obj

!IF  "$(CFG)" == "LWServer - Win32 Release"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\x86\Release\server.lib

!IF  "$(CFG)" == "LWServer - Win32 Release"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Debug AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Debug\serv_w.obj

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Debug\server.lib

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Release\serv_w.obj

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\LightwaveSDK\lib\AXP\Release\server.lib

!IF  "$(CFG)" == "LWServer - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "LWServer - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
