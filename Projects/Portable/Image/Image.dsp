# Microsoft Developer Studio Project File - Name="Image" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (ALPHA) Static Library" 0x0604
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Image - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Image.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Image.mak" CFG="Image - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Image - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Image - Win32 Debug Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE "Image - Win32 Release Alpha" (based on "Win32 (ALPHA) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Image - Win32 Release"

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
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Release\Image.lib"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

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
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Debug\Image.lib"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Image___"
# PROP BASE Intermediate_Dir "Image___"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Image___"
# PROP Intermediate_Dir "Image___"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /Gt0 /W3 /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Debug\Image.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\AXP\Debug\Image.lib"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Image__0"
# PROP BASE Intermediate_Dir "Image__0"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Image__0"
# PROP Intermediate_Dir "Image__0"
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /Gt0 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\Lib\Portable\x86\Release\Image.lib"
# ADD LIB32 /nologo /out:"..\..\..\Lib\Portable\AXP\Release\Image.lib"

!ENDIF 

# Begin Target

# Name "Image - Win32 Release"
# Name "Image - Win32 Debug"
# Name "Image - Win32 Debug Alpha"
# Name "Image - Win32 Release Alpha"
# Begin Group "Docs"

# PROP Default_Filter "*.txt"
# Begin Source File

SOURCE=".\IO\HowToAddNewFileFormats-README.txt"
# End Source File
# End Group
# Begin Group "ImageIO"

# PROP Default_Filter ""
# Begin Group "ImageIO Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IO\BMP.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\BMP_RLE8.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\CSource.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\ImageData.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\ImageIO.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\ImageIO_IDs.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IO\Targa.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "ImageIO Headers Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IO\BMP.h
# End Source File
# Begin Source File

SOURCE=.\IO\BMP_RLE8.h
# End Source File
# Begin Source File

SOURCE=.\IO\CSource.h
# End Source File
# Begin Source File

SOURCE=.\IO\ImageData.h
# End Source File
# Begin Source File

SOURCE=.\IO\ImageIO.h
# End Source File
# Begin Source File

SOURCE=.\IO\ImageIO_IDs.h
# End Source File
# Begin Source File

SOURCE=.\IO\IO.h
# End Source File
# Begin Source File

SOURCE=.\IO\Targa.h
# End Source File
# End Group
# End Group
# Begin Group "Image"

# PROP Default_Filter ""
# Begin Group "Image Header Files"

# PROP Default_Filter "*.h"
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\ImageClass.h
# End Source File
# Begin Source File

SOURCE=.\ImageIDs.h
# End Source File
# End Group
# Begin Group "Image Source Files"

# PROP Default_Filter "*.cpp;*.c"
# Begin Source File

SOURCE=.\ImageClass.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ImageIDs.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# End Group
# Begin Group "ImageProcess"

# PROP Default_Filter ""
# Begin Group "ImageProcess Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Process\Checkerboard.cpp

!IF  "$(CFG)" == "Image - Win32 Release"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug"

!ELSEIF  "$(CFG)" == "Image - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "Image - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "ImageProcess Headers Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Process\Checkerboard.h
# End Source File
# Begin Source File

SOURCE=.\Process\Process.h
# End Source File
# End Group
# End Group
# End Target
# End Project
