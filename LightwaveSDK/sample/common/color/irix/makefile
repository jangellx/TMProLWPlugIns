#
# Makefile for IRIX 5.2 C-compiler
#
ULIB	= ../../../lib/
UINC	= ../../../include/

CFLAGS	= -I$(UINC) -D_XGL -O
LFLAGS	= -shared -exported_symbol _mod_descrip

OBJS = colortest.o gui_help.o

.c.o:
	CC $(CFLAGS) -c $*.c

all : colortest.p

colortest.p: $(OBJS)
	ld $(LFLAGS) -L$(ULIB) $(ULIB)serv_u.o $(OBJS) -o colortest.p -lserver -lm

colortest.o	:	colortest.c
gui_help.o	:	../panels/gui_help.c
	CC $(CFLAGS) -c -o gui_help.o ../panels/gui_help.c
