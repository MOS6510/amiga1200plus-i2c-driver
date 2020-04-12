#
# Amiga1200+ i2c library driver
#
# (c) 2020 Heiko Prüssing <www.heiko-pruessing.de>
#

LIBRARY_NAME   = i2c.library

# Build subdirectory for every supported microprocessor (68000-68080)
BUILDDIR=build/build-$(ARCH)

SRC     += $(wildcard *.c)
HDR     += $(wildcard *.h)

#Make sure "devinit.o" is the first object file during linking!
FIRST_LINK_OBJECT = libinit.c
SRC_TMP := $(SRC)
SRC = $(filter-out $(FIRST_LINK_OBJECT), $(SRC_TMP))
SRC := $(FIRST_LINK_OBJECT) $(SRC)

#Create List of objects from list of sources...
OBJS     = $(patsubst %.c, $(BUILDDIR)/%.o, $(SRC))

LDOPTS += -noixemul -nostartfiles -lnix -lamiga -ldebug -lstubs
LDOPTS += -Wl,-Map,$@.map -Wl,--cref
CFLAGS += -Wno-pointer-sign
CFLAGS += -DLIBRARY_NAME=\"$(LIBRARY_NAME)\"

.PHONY: all distribution builddir clean

all: builddir $(BUILDDIR)/$(LIBRARY_NAME).$(ARCH)

addToAdf:
	@echo "Add to ADF file:"
	@$(XDFTOOL) $(ADFIMAGE) format "Amiga1200+Tools" + makedir "L" || true
	@$(XDFTOOL) $(ADFIMAGE) delete "L/"$(LIBRARY_NAME).$(ARCH) || true
	@$(XDFTOOL) $(ADFIMAGE) write  $(BUILDDIR)/$(LIBRARY_NAME).$(ARCH) "L"

# Installs library on real Amiga via "Amiga Explorer"
install:
		$(AMIGA_EXPLORER) -u L:$(LIBRARY_NAME) || true
		@sleep 2
		$(AMIGA_EXPLORER) -s $(BUILDDIR)/$(LIBRARY_NAME).$(ARCH) L:$(LIBRARY_NAME)

builddir:
	@echo "Create build dir..."
	@mkdir -p $(BUILDDIR)

# Creates distribution (and build all architechtures and LHA file at the end)
distribution:
	@make clean all     ARCH=000
	@make clean library ARCH=020
	@./build-installation.sh

$(BUILDDIR)/$(LIBRARY_NAME).$(ARCH):	$(OBJS)
		@echo "Linking       " $@
		@$(LD) $^ -o $@ $(LDOPTS)
		@strings $@ | grep '$$VER'
		@#@$(MAKE) addToAdf
		@#@ls -la $@

clean:
		rm -rf -d *.o *.s *.aobj *.library *.map mapfile $(BUILDDIR)/*.o $(BUILDDIR)/*.library
		rm -fr build/  $(BUILDDIR)

$(BUILDDIR)/%.o:%.c
	@echo "Compiling C   " $< " (68"$(ARCH)")"
	@$(CC) -c $(CFLAGS) -o $@ $<

.c.s: $(HDR)
	@echo "Compiling C (only to ASM) " $< " (68"$(ARCH)")"
	@$(CC) -S $(CFLAGS) -o $@ $<
