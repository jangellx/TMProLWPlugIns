@ECHO OFF
REM   $Id: mthonly.bat,v 1.1 1998/03/20 18:47:29 mfarr Exp $
REM  Set the Visual C++ compiler flags

if "%SPWPROC%" == "" goto vars

goto begin

:vars
set SPWPROC=x86
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set SPWPROC=alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set SPWPROC=x86
@call vcvars32.bat %SPWPROC%


:begin

REM  Figure out what type of processor we are using
REM  For Windows 95 set the default to intel

set ARCH=win32i
set ARCHC=Intel
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCH=win32a
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCHC=Alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCH=win32i
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCHC=Intel

mkdir ..\bin
mkdir ..\bin\%ARCH%
mkdir ..\bin\%ARCH%\debug
mkdir ..\bin\%ARCH%\release

echo Building: %ARCHC% Debug
nmake /c /f "spwmath.mak" CFG="spwmath - Win32 %ARCH% Debug" ALL
nmake /c /f "spwmath.mak" CFG="spwmath - Win32 %ARCH% Debug MThread" ALL

echo Building: %ARCHC% Release
nmake /c /f "spwmath.mak" CFG="spwmath - Win32 %ARCH% Release" ALL
nmake /c /f "spwmath.mak" CFG="spwmath - Win32 %ARCH% Release MThread" ALL

:end
set ARCH=
set ARCHC=
