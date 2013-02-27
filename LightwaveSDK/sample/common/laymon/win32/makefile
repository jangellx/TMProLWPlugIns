ULIB    = ..\..\..\lib
UINC    = ..\..\..\include

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

CFLAGS  = -c -W3 $(CFMACH) -DWIN32 -D_WIN32 /O2 /I $(UINC) /I . $(OPT)
LFLAGS  = /INCREMENTAL:NO -dll -def:$(UINC)\serv.def /NODEFAULTLIB:libc

OBJS = montest.obj

LLIBS   = $(ULIB)\serv_w.obj $(ULIB)\server.lib libcmt.lib kernel32.lib

.cpp.p:
	cl $(CFLAGS) $*.cpp

.c.p:
	cl $(CFLAGS) $*.c

all :  montest.p

montest.p:    $(OBJS)
    link $(LFLAGS) -out:montest.p $(OBJS) $(LLIBS)

montest.obj     : montest.c
