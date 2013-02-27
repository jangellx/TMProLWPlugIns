cd blotch
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\superblotch
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\lazypnts
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\motdump
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\negative
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\zor
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..
