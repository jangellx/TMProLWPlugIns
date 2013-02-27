@ECHO OFF

REM   $Id: build.bat,v 1.2 1996/12/19 21:01:11 prichard Exp $

REM  Set the Visual C++ compiler flags

IF "%SPWPROC%" == "" goto vars
goto begin
:vars
set SPWPROC=x86
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set SPWPROC=alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set SPWPROC=x86
call vcvars32.bat %SPWPROC%

:begin
REM  Clear all the flags, then set them as the user wishes

set CLEAN=False
set DEBUG=False
set RELEASE=False

REM  Figure out what type of processor we are using
REM  For Windows 95 set the default to intel

set ARCH=win32i
set ARCHC=Intel
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCH=win32a
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCHC=Alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCH=win32i
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCHC=Intel

REM  See which if any command line arguments have been sent.

IF "%1" == "clean" set CLEAN=True
IF "%1" == "debug" set DEBUG=True
IF "%1" == "release" set RELEASE=True

IF "%2" == "clean" set CLEAN=True
IF "%2" == "debug" set DEBUG=True
IF "%2" == "release" set RELEASE=True

IF "%3" == "clean" set CLEAN=True
IF "%3" == "debug" set DEBUG=True
IF "%3" == "release" set RELEASE=True

REM  Now figure out what the user left to defaults

IF "%DEBUG%" == "True" goto config
IF "%RELEASE%" == "True" goto config
set DEBUG=True
set RELEASE=True

:config
REM  Tell user what we are going to do.

REM  If they asked us to clean, then clean the directories, otherwise
REM  skip it

if "%CLEAN%" == "False" goto build


:clean
:cleand
if "%DEBUG%" == "False" goto cleanr
echo Cleaning: ..\bin\%ARCH%\Debug
nmake /c /f "widget.mak" CFG="widget - Win32 %ARCH% Debug" CLEAN
:cleanr
if "%RELEASE%" == "False" goto build
echo Cleaning: ..\bin\%ARCH%\Release
nmake /c /f "widget.mak" CFG="widget - Win32 %ARCH% Release" CLEAN
goto end

:build
REM  Now we go and build the selected parts of SpaceWare

if not exist ../bin mkdir "../bin"
if not exist ../bin/%ARCH% mkdir "../bin/%ARCH%"
if not exist ../bin/%ARCH%/debug mkdir "../bin/%ARCH%/debug"
if not exist ../bin/%ARCH%/release mkdir "../bin/%ARCH%/release"

:build
:buildd
if "%DEBUG%" == "False" goto buildr
echo Building: %ARCHC% Debug
nmake /c /f "widget.mak" CFG="widget - Win32 %ARCH% Debug" ALL
:buildr
if "%RELEASE%" == "False" goto end
echo Building: %ARCHC% Release
nmake /c /f "widget.mak" CFG="widget - Win32 %ARCH% Release" ALL

:end

set ARCH=
set ARCHC=
set CLEAN=
set DEBUG=
set RELEASE=
