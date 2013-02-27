#
# Makefile for Solaris C-compiler
#
ULIB	= ../../../lib/
UINC	= ../../../include/

CFLAGS	= -I$(UINC) -D_XGL -O
LFLAGS	= -G -B symbolic -e _mod_descrip


# Output *.p modules are made directly from C source files to keep this
# example simple.
#
.c.p:
	cc $(CFLAGS) -c $*.c
	ld $(LFLAGS) -L$(ULIB) $(ULIB)serv_u.o $*.o -o $@ -lserver -lm


all : vsformat.p
