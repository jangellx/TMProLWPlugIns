
PLUG_NAME = montest

ULIB    = ../../../lib
UINC    = ../../../include

CC = cc
LD = cc

DFLAGS = -Olimit 1500
CFLAGS = -I$(UINC) -I. $(OPT) -D_XGL
LFLAGS = -L$(ULIB) -shared -exported_symbol _mod_descrip

OBJS = $(PLUG_NAME).o \
       $(ULIB)/serv_u.o

LLIBS   = -lserver -lm -lc

.c.o:
	$(CC) $(CFLAGS) -c -o $*.o $*.c

all :  $(PLUG_NAME).p

clean:
	rm *.o
	rm x*

$(PLUG_NAME).p:    $(OBJS)
	$(LD) $(LFLAGS) -o $(PLUG_NAME).p $(OBJS) $(LLIBS)

