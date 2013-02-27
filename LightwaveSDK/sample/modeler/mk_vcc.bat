cd layerset
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\spherize
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\spikey
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..\testcmd
nmake -f Win32\makefile /a /nologo
del *.obj
cd ..
