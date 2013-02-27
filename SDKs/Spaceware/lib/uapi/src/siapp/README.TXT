Build Notes for SpaceWare 8.1 Static Library
--------------------------------------------

To integrate an application with SpaceWare 8.1, no modification to the source 
code of the target application should be required. It is recommended that the 
initialization call SiInitialize be checked for error condition SPW_ERROR.
Receipt of this error could indicate that the DLL that contains the Si 
functions was not loaded properly or that the address of one of the functions
was not found. It is important to check for this error condition because Si
functions will return an error condition (if they are able) but not 
necessarily fail or cause the program to crash.

To build the the static library:

-- Build from within Visual C++ using Build|Batch Build..., selecting all
   available build targets, and then selecting the "Rebuild All" button.
   OR
   execute build.bat in the source code directory ud\lib\uapi\src\siapp.

-- Go to directory ud\lib\siapp\src and execute build.bat to combine slim and
   siapp into a single static library.
   NOTE that you may need to build slim first.

-- for all the application's build targets, add y:dev/slim/inc to the list of 
   include directories.  These are located under Build|Settings... in the 
   C/C++ tab.  In the drop box select Preprocessor and add the above directory
   to the comma-separated list.

-- In the same dialog box under the Link tab, change the Si static library to
   one of the following:

   lib\siapp\bin\win32i\debug\sthread\siappstd for debug single-threaded
   lib\siapp\bin\win32i\debug\mthread\siappstd for debug multiple-threaded
   lib\siapp\bin\win32i\release\sthread\siappst for release single-threaded
   lib\siapp\bin\win32i\release\mthread\siappst for release muliple threaded

   Note that the paths are RELATIVE to the application's own path; the 
   appropriate <..\> jump should be prepended to the above paths to 
   get to the library's path.

   Note also that for DEC Alpha machines, win32i should be changed to win32a.

   Doing this links the Si and Slim libraries into the application at link time.

-- Compile the target application.

-- Go to ud/lib/uapi/src/siappdll and run build.bat to build the DLL files.
   The resulting DLL files will be located in ud/lib/uapi/bin/siappdll/debug
   and ud/lib/uapi/bin/siappdll/release. Copy the debug version siappdld.dll 
   and the release version siappdll.dll into the Windows\System directory 
   (Windows 95) or WinNT\System32 directory (Windows NT 4.0).

-- Run the application.
