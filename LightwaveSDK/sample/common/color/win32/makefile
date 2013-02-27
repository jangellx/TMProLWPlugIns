#
# Makefile for Microsoft Visual C++
#
ULIB	= ..\..\..\lib\ 
UINC	= ..\..\..\include\ 

# Define different tokens for the different possible machine types.
#
!IF "$(PROCESSOR_ARCHITECTURE)" == "x86"
CFMACH	= -D_X86_=1
!ENDIF

!IF "$(PROCESSOR_ARCHITECTURE)" == "MIPS"
CFMACH	= -D_MIPS_=1
!ENDIF

!IF "$(PROCESSOR_ARCHITECTURE)" == "ALPHA"
CFMACH	= -D_ALPHA_=1
!ENDIF

# If the include files are already be on the include path, the /I option
# can be left off.
#
CFLAGS	= -c -W3 $(CFMACH) -D_WIN32 /Ox /I $(UINC)
LFLAGS	= -dll -export:_mod_descrip /NODEFAULTLIB:libc
LLIBS	= $(ULIB)serv_w.obj $(ULIB)server.lib libcmt.lib kernel32.lib

OBJS = colortest.obj gui_help.obj

.c.obj:
	cl $(CFLAGS) $*.c

all:    colortest.p

colortest.p : $(OBJS)
    link $(LFLAGS) -out:colortest.p $(OBJS) $(LLIBS)
	del *.exp *.lib

colortest.obj   : colortest.c
gui_help.obj    : ..\panels\gui_help.c
    cl $(CFLAGS) /Fogui_help.obj ..\panels\gui_help.c
