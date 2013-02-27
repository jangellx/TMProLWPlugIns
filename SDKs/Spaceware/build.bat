@echo off

REM
REM Batch file to build all source code into executables in Phase II zip files
REM
REM Matt Farr
REM
REM 3 Feb 1998
REM
REM $Id: buildsrc.bat,v 1.4.4.2 1998/05/26 15:07:47 equerze Exp $

cd .\lib\slim\src\
call build.bat
cd ..\..\..\

cd .\lib\uapi\src\siapp\
call build.bat
cd ..\..\..\..\

cd .\lib\siapp\src\
call build.bat
cd ..\..\..\

cd .\demos\cityfly\src\
call build.bat
cd ..\..\..\

cd .\demos\widget\src\
call build.bat
cd ..\..\..\

cd .\demos\sbtest32\src\
call build.bat
cd ..\..\..\

cd .\demos\sbtstmfc\src\
call build.bat
cd ..\..\..\


REM  Figure out what type of processor we are using
REM  For Windows 95 set the default to intel

set ARCH=Win32i
set ARCHC=Intel
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCH=Win32a
IF "%PROCESSOR_ARCHITECTURE%" == "ALPHA" set ARCHC=Alpha
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCH=Win32i
IF "%PROCESSOR_ARCHITECTURE%" == "x86" set ARCHC=Intel

if not "%OS%"=="Windows_NT" goto win
copy .\lib\uapi\bin\siappdll\%ARCH%\release\siappdll.dll %SystemRoot%\system32\
copy .\lib\uapi\bin\siappdll\%ARCH%\debug\siappdld.dll %SystemRoot%\system32\
goto done

:win
copy .\lib\uapi\bin\siappdll\%ARCH%\release\siappdll.dll %winbootdir%\system\
copy .\lib\uapi\bin\siappdll\%ARCH%\debug\siappdld.dll %winbootdir%\system\

:done
set ARCH=
set ARCHC=
