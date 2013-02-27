SpaceWare SDK 1.0 for Windows.  

INTRODUCTION

The SpaceWare SDK 1.0 provides a single interface to the driver 
software that gives application software access to Spaceball data.  
Once the SDK library code is compiled into the application code, 
future upgrades to SpaceWare will only require the replacement 
of a DLL in the system directory, with no recompilation required.

IMPORTANT NOTES

This version of the SDK has been successfully compiled under 
Microsoft Visual C++ versions 4.2 and 5.0 on both Intel and Alpha 
platforms.

The directory structure for this SDK is placed under the directory 
in which it is unzipped.  

Once unzipped, the DLLs must be moved to their home. As unzipped, 
they are located in the extraction directory under 
ud\lib\uapi\bin\siappdll\debug or release. This is definitely OS 
dependent and somewhat machine dependent. For Windows 95, the DLLs 
should be placed in the Windows\System directory or its equivalent.  
For Windows NT, the DLLs should be placed in the WinNT\System32
directory or its equivalent. Note that these directory names may 
not be the same on all systems.


FILE LOCATIONS

With DLLs installed, everything should run correctly with SpaceWare 
8.0 installed. Demos are located in ud\demos. There are four: 
Widget, CityFly, sbtest, and sbtstmfc.  

The executables are located in ud\demos\cityfly\bin\win32i\release 
or debug and ud\demos\widget\bin\win32i\release or debug.

The static library code is contained in two places, ud\lib\slim and
ud\lib\uapi\src\siapp. Slim is the math library used by the driver
software and is composed of two parts: spwmath and spwutil.  

Spwmath code is provided; these are the routines that are used most 
of the time. Spwutil is provided as a binary only. Upai's siapp 
contains the code for the static library that should be linked into 
application code. It provides stubs for driver routines that live in 
the DLL. Both of these libraries are combined into a single library 
by the build script in ud\lib\siapp\src.

COMPILER NOTES
Before recompiling, make sure that the development environment is set up 
properly. In particular, the paths must be properly set up to find the 
appropriate Microsoft libraries used by nmake. For Visual C++ 4.2, the 
development environment is usually set up in the directory msdev on the 
main drive (usually C:\). Assuming this, for Windows NT 4.0, the 
following environment variables should be set in Control Panels|System 
under the Environment tab, in the User Variables section:

lib set to C:\msdev\lib,C:\msdev\mfc\lib
include set to C:\msdev\include,C:\msdev\mfc\include
add to path: C:\msdev\bin

Adjust these drives and directories to your own situation as 
appropriate.

If the compiler being used is Visual C++ 5.0, these variables are 
similar. The difference is that Visual C++ became part of Visual Studio 
and therefore the paths to get to the binaries and libraries are 
completely different. Go to the same control panel tab and set up the 
some enviroment variables as follows:

lib set to C:\Program Files\DevStudio\Vc\lib,C:\Program Files\DevStudio\Vc\mfc\lib
include set to C:\Program Files\DevStudio\Vc\include,C:\Program Files\DevStudio\Vc\mfc\include
add to path: C:\Program Files\DevStudio\Vc\bin

Note that when loading projects in Visual C++ 5.0 on the Alpha platform,
you must manually set the active configuration to an Alpha configuration.

Note that under Windows 95, only the path variable in the autoexec.bat 
file need be changed. Using Visual C++ 4.2 add the following line:

PATH=%PATH%;C:\MSDEV\LIB;C:\MSDEV\MFC\LIB;C:\MSDEV\INCLUDE;C:\MSDEV\MFC\INCLUDE;C:\MSDEV\BIN

With Visual C++ 5.0 the following lines are required:

PATH=%PATH%;C:\PROGRA~1\DEVSTU~1\VC\BIN
PATH=%PATH%;C:\PROGRA~1\DEVSTU~1\VC\LIB;C:\PROGRA~1\DEVSTU~1\VC\MFC\LIB
PATH=%PATH%;C:\PROGRA~1\DEVSTU~1\VC\INCLUDE;C:\PROGRA~1\DEVSTU~1\VC\MFC\INCLUDE

In order to compile from the command line in Windows 95, you must set 
MS-DOS window environment memory to the maximum allowed value (4096). 

BUILD NOTES

To initiate a build, open a MS-DOS window and go to the directory that
contains the extracted files. Start the build by typing "build". This 
will rebuild the entire tree under either Visual C++ 4.2 or 5.0 (that 
which is present). As it finishes it inserts the DLLs into the 
appropriate system directory. In the last few lines of the build.bat 
script, this copy operation is performed. If this copy operation does 
not complete correctly, the directories must be correctly specified in 
that section.

When building an application, the SpaceWare libraries must be linked in.
In Visual C++ 4.2 this is done by selecting Build|Settings, select the 
Link tab, and then add one of the following files to the Object/Library
module listing: 

for single threaded applications:

ud\lib\uapi\siapp\win32i\sthread\release\siappst.lib
ud\lib\uapi\siapp\win32i\sthread\debug\siappstd.lib

for multithreaded applications:

ud\lib\uapi\siapp\win32i\mthread\release\siappmt.lib
ud\lib\uapi\siapp\win32i\mthread\debug\siappmtd.lib

The only difference under Visual C++ 5.0 is to add under the 
Project|Settings menu.

Note that it is IMPORTANT to match single-threaded applications to the 
single-threaded static library and multi-threaded applications to 
multi-threaded static library.


ADDITIONAL INFORMATION

SiInitialize now returns SPW_DLL_LOAD_ERROR in addition to SPW_ERROR 
and SPW_NO_ERROR.
