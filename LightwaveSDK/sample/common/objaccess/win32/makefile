#
# Makefile for Microsoft Visual C 2.0 for IX86.
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


# Output *.p modules are made directly from C source files to keep this
# example simple.
#
.c.p:
	cl $(CFLAGS) $*.c
	link $(LFLAGS) -out:$@ $*.obj $(LLIBS)

all : objtest.p
	del *.exp *.lib
