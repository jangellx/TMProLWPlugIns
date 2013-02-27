REM @echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by ALLKEYER.HPJ. >"hlp\AllKeyer.hm"
echo. >>"hlp\AllKeyer.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\AllKeyer.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\AllKeyer.hm"
echo. >>"hlp\AllKeyer.hm"
echo // Prompts (IDP_*) >>"hlp\AllKeyer.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\AllKeyer.hm"
echo. >>"hlp\AllKeyer.hm"
echo // Resources (IDR_*) >>"hlp\AllKeyer.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\AllKeyer.hm"
echo. >>"hlp\AllKeyer.hm"
echo // Dialogs (IDD_*) >>"hlp\AllKeyer.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\AllKeyer.hm"
echo. >>"hlp\AllKeyer.hm"
echo // Frame Controls (IDW_*) >>"hlp\AllKeyer.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\AllKeyer.hm"
REM -- Make help for Project ALLKEYER


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\AllKeyer.hpj"
if errorlevel 1 goto :Error1
if not exist "hlp\AllKeyer.hlp" goto :Error2
if not exist "hlp\AllKeyer.cnt" goto :Error3
echo.
if exist Debug\nul copy "hlp\AllKeyer.hlp" Debug
if exist Debug\nul copy "hlp\AllKeyer.cnt" Debug
if exist Release\nul copy "hlp\AllKeyer.hlp" Release
if exist Release\nul copy "hlp\AllKeyer.cnt" Release
echo.
goto :done

:Error1
echo hlp\AllKeyer.hpj(1) : error: Problem encountered creating help file (1)
goto :done
:Error2
echo hlp\AllKeyer.hpj(1) : error: Problem encountered creating help file (2: .hlp)
goto :done
:Error3
echo hlp\AllKeyer.hpj(1) : error: Problem encountered creating help file (3: .cnt)
goto :done

:done
echo.
