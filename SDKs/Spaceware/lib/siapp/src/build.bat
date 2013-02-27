@echo off
REM
REM   This is a helper batch file to get the files for the libraries
REM   that an application needs to link in SpaceWare and build them into
REM   one library.
REM 
REM   Jeff LeBlanc
REM   October 1996
REM
REM   $Id: mathonly.bat,v 1.1 1998/03/20 18:48:33 mfarr Exp $
REM

REM  Set the Visual C++ compiler flags

IF "%1" == "clean" goto end

set COMP=%SPW_COMPILER%
echo COMPILER is %COMP%

IF "%SPWPROC%" == "" goto vars
goto begin
:vars
set SPWPROC=x86
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set SPWPROC=alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set SPWPROC=x86

call vcvars32.bat %SPWPROC%

:begin
REM  Clear all the flags, then set them as the user wishes

echo -
echo Assembling siapp
echo -

set CLEAN=False
set DEBUG=False
set RELEASE=False

REM  Figure out what type of processor we are using
REM  For Windows 95 set the default to intel

set ARCHC=win32i
set ARCH=Intel
set ROOT=..\..\..
set SLIMPATH=..\..\slim\bin

IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCH=Alpha
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCHC=win32a
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCH=Intel
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCHC=win32i

REM Create the needed subdirectories

if not exist ../bin mkdir "../bin"
if not exist ../bin/%ARCHC% mkdir "../bin/%ARCHC%"
if not exist ../bin/%ARCHC%/debug mkdir "../bin/%ARCHC%/debug"
if not exist ../bin/%ARCHC%/debug/sthread mkdir "../bin/%ARCHC%/debug/sthread"
if not exist ../bin/%ARCHC%/debug/mthread mkdir "../bin/%ARCHC%/debug/mthread"
if not exist ../bin/%ARCHC%/release mkdir "../bin/%ARCHC%/release"
if not exist ../bin/%ARCHC%/release/sthread mkdir "../bin/%ARCHC%/release/sthread"
if not exist ../bin/%ARCHC%/release/mthread mkdir "../bin/%ARCHC%/release/mthread"

:sthread_d_HE

REM Copy in the existing libraries
echo Building sthread debug

echo /OUT:..\bin\%ARCHC%\debug\sthread\siappstd.lib >lib.lst
echo %SLIMPATH%\%ARCHC%\debug\sthread\spwmath.lib >>lib.lst
echo %ROOT%\lib\uapi\bin\siapp\%ARCHC%\debug\sthread\siappd.lib >>lib.lst

REM Combine them into one library
LIB @lib.lst
del lib.lst

:sthread_r_HE

REM Copy in the existing libraries
echo Building sthread release

echo /OUT:..\bin\%ARCHC%\release\sthread\siappst.lib >lib.lst
echo %SLIMPATH%\%ARCHC%\release\sthread\spwmath.lib >>lib.lst
echo %ROOT%\lib\uapi\bin\siapp\%ARCHC%\release\sthread\siapp.lib >>lib.lst
 
REM Combine them into one library
LIB @lib.lst
del lib.lst

:mthread_d_HE

REM Copy in the existing libraries
echo Building mthread debug

echo /OUT:..\bin\%ARCHC%\debug\mthread\siappmtd.lib >lib.lst
echo %SLIMPATH%\%ARCHC%\debug\mthread\spwmath.lib >>lib.lst
echo %ROOT%\lib\uapi\bin\siapp\%ARCHC%\debug\mthread\siappd.lib >>lib.lst

REM Combine them into one library
LIB @lib.lst
del lib.lst

:mthread_r_HE

REM Copy in the existing libraries
echo Building mthread release

echo /OUT:..\bin\%ARCHC%\release\mthread\siappmt.lib >lib.lst
echo %SLIMPATH%\%ARCHC%\release\mthread\spwmath.lib >>lib.lst
echo %ROOT%\lib\uapi\bin\siapp\%ARCHC%\release\mthread\siapp.lib >>lib.lst

REM Combine them into one library
LIB @lib.lst
del lib.lst


:end
