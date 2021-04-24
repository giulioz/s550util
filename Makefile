MACHINE=$(shell uname -m)
OS=$(shell uname -s)
ifeq ($(MACHINE),x86)
	CPU=$(MACHINE)
else
	CPU=$(shell uname -p)
endif
# Set byte order to big endian on powerpc
ifeq ($(CPU), powerpc)
	CFLAGS+= -DIS_BIG_ENDIAN
endif
ifeq ("$(OS)", "SunOS")
	ifeq ($(CPU),sparc)
		CFLAGS+= -DIS_BIG_ENDIAN
	endif
else
	CFLAGS+= -DNEED_STDINT_H
endif
.PHONY: all clean tar
all:	s550util
clean: 
	-rm s550util
s550util:	s550util.c
ifdef DEBUG
		gcc $(CFLAGS) -DDEBUG -o $@ $<
else
		gcc $(CFLAGS) -o $@ $<
endif
tar:
	-rm s550util.tar
	tar cf s550util.tar Makefile README *.[ch] s2img.sh

