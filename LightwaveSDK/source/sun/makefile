#
# Server library and startup module -- IRIX
#
UINC    = ../include
ULIB    = ../lib

CFLAGS  = -I$(UINC) -D_XGL -DRELEASE -O -c

.c.o:
	cc $(CFLAGS) $*.c


SERVLIB_O = slib3.o slib2.o slib1.o


all : $(ULIB)/libserver.a $(ULIB)/serv_u.o

$(ULIB)/libserver.a : $(SERVLIB_O)
	-rm $(ULIB)/libserver.a
	ar r $(ULIB)/libserver.a $(SERVLIB_O)

$(ULIB)/serv_u.o : serv_u.o
	cp serv_u.o $(ULIB)/serv_u.o
