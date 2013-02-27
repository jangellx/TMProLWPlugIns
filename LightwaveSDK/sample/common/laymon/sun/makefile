# Makefile template for Sun systems

# Set this value to the basename of your projects plug-in
#
BASE    = montest

# Set these values to the location of your LightWave SDK installation
#
ULIB    = ../../../lib
UINC    = ../../../include

CC = cc
LD = cc

# Optimization flags (to employ the '-fast' switch on large source files,
# you must have a LOT of swap space available)
#
#DFLAGS = -O -xtarget=native
DFLAGS = -O -fast -xtarget=native

# Compiler flags
#
CFLAGS = -D_XGL $(DFLAGS) -I$(UINC) -I. -I/usr/include $(OPT)

# Linker flags
#
LFLAGS = -L/usr/lib -L$(ULIB) -G -B symbolic -e _mod_descrip

# Place a list of your project's object files in this value.  (Be sure
# to retain the last line that links in 'serv_u.o')
#
OBJS = $(BASE).o \
       $(ULIB)/serv_u.o

# The list of libraries we will need to link with
#
LLIBS   = -lserver -lm

# Some build rules
#
.c.o:
	$(CC) $(CFLAGS) -c $*.c

# Targets for your build.  Replace '$(BASE)' with your plug-in's base name
#
all :  $(BASE).p

$(BASE).p:    $(OBJS)
	$(LD) $(LFLAGS) -o $(BASE).p $(OBJS) $(LLIBS)

# Update/add to these dependencies for each source file in your project.
#
$(BASE).o	:	montest.c
	$(CC) $(CFLAGS) -c montest.c -o montest.o

