# Microsoft Developer Studio Project File - Name="makefile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=makefile - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "makefile.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "makefile.mak" CFG="makefile - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "makefile - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "makefile - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "makefile - Win32 Release"

# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f makefile.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "NMAKE /f makefile.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "makefile.exe"
# PROP Bsc_Name "makefile.bsc"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "makefile - Win32 Debug"

# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f makefile.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "makefile.exe"
# PROP BASE Bsc_Name "makefile.bsc"
# PROP BASE Target_Dir ""
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "NMAKE /f makefile.mak"
# PROP Rebuild_Opt "/a"
# PROP Target_File "makefile.exe"
# PROP Bsc_Name "makefile.bsc"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "makefile - Win32 Release"
# Name "makefile - Win32 Debug"

!IF  "$(CFG)" == "makefile - Win32 Release"

!ELSEIF  "$(CFG)" == "makefile - Win32 Debug"

!ENDIF 

# Begin Group "Server Library"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\slib1.c
# End Source File
# Begin Source File

SOURCE=..\slib2.c
# End Source File
# Begin Source File

SOURCE=..\slib3.c
# End Source File
# End Group
# Begin Group "Serv_W Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\serv_w.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\makefile.vcc
# End Source File
# End Target
# End Project
