#
# Amiga 1200+ i2c
#
# 2020 by Heiko Prüssing
#

SRC      := $(wildcard *.c)
HDR 	   := $(wildcard *.h) 
SRC_MAIN := main.c

#If not given via command line build for "MC68000"
ifeq ($(ARCH),)
	ARCH=000
endif	

BUILDDIR = build/build-$(ARCH)
OS_INCLUDES                   = -Iincludes -Iincludes/sana -Iincludes/libnix                                                    
LINKER_OPTS                	= -Llibs -nostdlib -noixemul -lnix -lamiga -lstubs 
#All objects:
OBJ       							= $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))
#All objects of the linker lib:
OBJ_LIB    							= $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC_LIB))
#All objects of the cnsole program:
OBJ_MAIN								= $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC_MAIN))

LDFLAGS += -L$(BUILDDIR)
CFLAGS += -I../library/build


.PHONY: all distribution builddir install clean

all: builddir $(BUILDDIR)/unittest

builddir:
	@echo "Create build directories..."
	@mkdir -p $(BUILDDIR)
		
$(BUILDDIR)/unittest:	$(OBJ_MAIN)
		@echo "Linking       " $@
		@$(LD) $^ $(LDFLAGS) -o $@ -Wl,-Map,$@.map 
				
# Installs device on real Amiga via "Amiga Explorer"
install:
		$(AMIGA_EXPLORER) -l || true
		@sleep 1
		$(AMIGA_EXPLORER) -u RAD:/unittest || true
		@sleep 1
		$(AMIGA_EXPLORER) -s $(BUILDDIR)/unittest RAD:/unittest	
		
clean:
		rm -r -f -d $(BUILDDIR) *.o *.s *.aobj *.map mapfile

#.c.o:
$(BUILDDIR)/%.o:%.c $(HDR)
	@echo "Compiling C   " $< " (cpu=68"$(ARCH)")"
	$(CC) -c $(CFLAGS) -o $@ $< 

#.c.s:
$(BUILDDIR)/%.s:%.c $(HDR)
	@$(CC) -S $(CFLAGS) -o $@ $< 
	