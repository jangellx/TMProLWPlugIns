# Microsoft Developer Studio Project File - Name="FreeKeyPlugin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (ALPHA) Dynamic-Link Library" 0x0602

CFG=FreeKeyPlugin - Win32 Debug Alpha
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FreeKeyPlugin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FreeKeyPlugin.mak" CFG="FreeKeyPlugin - Win32 Debug Alpha"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FreeKeyPlugin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FreeKeyPlugin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FreeKeyPlugin - Win32 Debug Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE "FreeKeyPlugin - Win32 Release Alpha" (based on "Win32 (ALPHA) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEKEYPLUGIN_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEKEYPLUGIN_EXPORTS" /YX /FD /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib opengl32.lib glu32.lib glaux.lib /nologo /dll /machine:I386 /out:"../../../../Bin/Lightwave_PlugIns\x86\Release\FreeKey.p"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEKEYPLUGIN_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FREEKEYPLUGIN_EXPORTS" /YX /FD /GZ /c
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib opengl32.lib glu32.lib glaux.lib /nologo /dll /debug /machine:I386 /out:"../../../../Bin/Lightwave_PlugIns\x86\Debug\FreeKey.p" /pdbtype:sept

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FreeKeyPlugin___Win32_Debug_Alpha"
# PROP BASE Intermediate_Dir "FreeKeyPlugin___Win32_Debug_Alpha"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "FreeKeyPlugin___Win32_Debug_Alpha"
# PROP Intermediate_Dir "FreeKeyPlugin___Win32_Debug_Alpha"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
CPP=cl.exe
# ADD BASE CPP /nologo /MTd /Gt0 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /Gt0 /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
RSC=rc.exe
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\x86\Debug\FreeKey.p" /pdbtype:sept
# SUBTRACT BASE LINK32 /incremental:no
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /dll /debug /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\AXP\Debug\FreeKey.p" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "FreeKeyPlugin___Win32_Release_Alpha"
# PROP BASE Intermediate_Dir "FreeKeyPlugin___Win32_Release_Alpha"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "FreeKeyPlugin___Win32_Release_Alpha"
# PROP Intermediate_Dir "FreeKeyPlugin___Win32_Release_Alpha"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
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
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\x86\Release\FreeKey.p"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib winmm.lib opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /dll /machine:ALPHA /out:"../../../../Bin/Lightwave_PlugIns\AXP\Release\FreeKey.p"

!ENDIF 

# Begin Target

# Name "FreeKeyPlugin - Win32 Release"
# Name "FreeKeyPlugin - Win32 Debug"
# Name "FreeKeyPlugin - Win32 Debug Alpha"
# Name "FreeKeyPlugin - Win32 Release Alpha"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\FreeKey-Interface.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Controls-Main.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Controls-Tools.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Dongle.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Group.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-ItemListDrawData.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Keys.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-KeySelect.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-OpenGLLister.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Options.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Process.cpp"

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FreeKeyInterface_Lister.cpp

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FreeKeyInterface_Select.cpp

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FreeKeyLogo.cpp

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FreeKeyLogo_Small.cpp

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\FreeKeyPlugin.cpp

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\FreeKey-Interface.h"
# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-Dongle.h"
# End Source File
# Begin Source File

SOURCE=".\FreeKeyInterface-ItemListDrawData.h"
# End Source File
# Begin Source File

SOURCE=.\FreeKeyLogo.h
# End Source File
# Begin Source File

SOURCE=.\FreeKeyLogo_Small.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Libs"

# PROP Default_Filter ""
# Begin Group "Libs -- Debug x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Debug\serv_w.obj

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Debug\server.lib

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release x86"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Release\serv_w.obj

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\x86\Release\server.lib

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Debug AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Debug\serv_w.obj

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Debug\server.lib

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Libs -- Release AXP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Release\serv_w.obj

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\LightwaveSDK\lib\AXP\Release\server.lib

!IF  "$(CFG)" == "FreeKeyPlugin - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Debug Alpha"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "FreeKeyPlugin - Win32 Release Alpha"

!ENDIF 

# End Source File
# End Group
# End Group
# End Target
# End Project
