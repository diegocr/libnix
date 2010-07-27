# Generated automatically from Makefile.in by configure.
#### Start of system configuration section. ####

srcdir =	.

CC =		m68k-amigaos-gcc
CPP =		m68k-amigaos-gcc -E
AS =		m68k-amigaos-as

AR =		m68k-amigaos-ar
RANLIB =	m68k-amigaos-ranlib
AWK =		gawk
V	=	VER: libnix 3.0 (29.11.2009)
CFLAGS	=	-Wall -m68020-60 -O2 -msoft-float -funroll-loops -fomit-frame-pointer -noixemul -I../headers -g

#### End system configuration section ####

ifneq ($(TARGET),clean)
include ../../sources/nix/filelist
endif

CURDIR = $(shell pwd)

OPTIONS=-I$(srcdir)/../headers $(CFLAGS)

REDEF=-D_DOSBase=___DOSBase \
      -D_UtilityBase=___UtilityBase \
      -D_MathIeeeSingBasBase=___MathIeeeSingBasBase \
      -D_MathIeeeSingTransBase=___MathIeeeSingTransBase \
      -D_MathIeeeDoubBasBase=___MathIeeeDoubBasBase \
      -D_MathIeeeDoubTransBase=___MathIeeeDoubTransBase \
      -D_LocaleBase=___LocaleBase

%.o: ../../sources/nix/%.c
	$(CC) $(OPTIONS) -S $^ -o $*.S 2>&1|tee $*.err
	$(CPP) -traditional $(REDEF) $*.S -o $*2.S
	$(CC) $(OPTIONS) $*2.S -c -o $*.o
	-rm $*.S $*2.S
	-if test ! -s $*.err; then rm $*.err; fi

.PHONY: all clean veryclean

all: libnix.a

clean:
	-rm -f */*.o *.a

filelists:
	rm -fv filelist
	cd ../ && make filelists

$(foreach f,$(SUBDIRS),$(CURDIR)/$(f)):
	mkdir $@

libnix.a: $(foreach f,$(SUBDIRS),$(CURDIR)/$(f)) $(OBJECTS) ../../sources/nix/Makefile ../../sources/nix/filelist
	-rm -f $@
	$(AR) -q $@ $(OBJECTS)
	$(RANLIB) $@
	echo "\$$$(V)" >>$@
