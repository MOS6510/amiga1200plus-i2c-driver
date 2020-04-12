# Main makefile

PROJ_ROOT = $(PWD)

CCPATH							   = /opt/Amiga

CC                            = $(CCPATH)/bin/m68k-amigaos-gcc
CXX                           = $(CCPATH)/bin/m68k-amigaos-g++
AR                            = $(CCPATH)/bin/m68k-amigaos-ar
NM                            = $(CCPATH)/bin/m68k-amigaos-nm
RANLIB							   = $(CCPATH)/bin/m68k-amigaos-ranlib
LD                       		= $(CCPATH)/bin/m68k-amigaos-gcc
SHELL                         = sh
XDFTOOL								= /usr/local/bin/xdftool
AMIGA_EXPLORER                = $(PWD)/tools/lxamiga.pl

ADFIMAGE								:= $(PROJ_ROOT)/build/Amiga1200+i2c.adf

CFLAGS								+= -Os \
										-I../include \
										-fstrength-reduce \
									 	$(INC_SRCH_PATH) \
										-Wstrict-prototypes \
                              -noixemul \
                              -Wall \
                              -m68$(ARCH) \
										-fomit-frame-pointer \
                              -msoft-float

LDFLAGS 								+= -Llibs -noixemul

#If not given via command line, build for "68000"
ifeq ($(ARCH),)
	ARCH=000
endif

BUILDDIR								:= build

export CCPATH CC LD CXX CFLAGS LDFLAGS RANLIB AR LD AOS_INCLUDES OS_INCLUDES CFLAGS ARCH PROJ_ROOT XDFTOOL AMIGA_EXPLORER NM ADFIMAGE

.PHONY: install clean all debug createDistribution

# Release version: make
all: 	CFLAGS += -s
all: 	LDFLAGS += -s
all:
	@$(MAKE) -C library
	@#@$(MAKE) createDistribution

# Debug version: make debug
debug:  CFLAGS += -DDEBUG -g
debug:
	@$(MAKE) -C library

install:
	@$(MAKE) -C library install

clean:
	@$(MAKE) -C library  clean
	rm -f $(ADFIMAGE)

createDistribution:
	#@$(XDFTOOL) $(ADFIMAGE) write DistributionTemplate/install
	#@$(XDFTOOL) $(ADFIMAGE) write DistributionTemplate/install.info
	#@$(XDFTOOL) $(ADFIMAGE) list
