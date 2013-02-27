cd envaccess
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\image\gradload
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\imgsaver
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\..\laymon
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\objaccess
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\vsformat
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\color
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\gradient
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..
