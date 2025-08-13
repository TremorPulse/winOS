# Host and target configuration (simplified for x86)
HOST?=i686-elf
HOSTARCH?=i386

# Compiler and flags
CC?=$(HOST)-gcc
AR?=$(HOST)-ar
CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

# Installation directories
DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
INCLUDEDIR?=$(PREFIX)/include
LIBDIR?=$(EXEC_PREFIX)/lib

# Kernel-specific flags
CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra -nostdlib -fno-builtin -fno-stack-protector
CPPFLAGS:=$(CPPFLAGS) -D__is_libc -Iinclude
LIBK_CFLAGS:=$(CFLAGS)
LIBK_CPPFLAGS:=$(CPPFLAGS) -D__is_libk

# Source files that will be implemented
FREEOBJS=\
stdio/printf.o \
stdio/putchar.o \
stdio/puts.o \
stdlib/abort.o \
string/memcmp.o \
string/memcpy.o \
string/memmove.o \
string/memset.o \
string/strlen.o \

# Currently no hosted objects for kernel
HOSTEDOBJS=

# All objects
OBJS=\
$(FREEOBJS) \
$(HOSTEDOBJS) \

# Kernel library objects
LIBK_OBJS=$(FREEOBJS:.o=.libk.o)

# Target binaries (only kernel library for now)
BINARIES=libk.a

.PHONY: all clean install install-headers install-libs setup-dirs help
.SUFFIXES: .o .libk.o .c .S

all: setup-dirs $(BINARIES)

# Setup directory structure
setup-dirs:
	@mkdir -p stdio stdlib string

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu11 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES) *.a
	rm -f $(OBJS) $(LIBK_OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) $(LIBK_OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

help:
	@echo "winOS Kernel Library Makefile"
	@echo "Available targets:"
	@echo "  all          - Build the kernel library"
	@echo "  clean        - Clean build artifacts"
	@echo "  install      - Install headers and libraries"
	@echo "  setup-dirs   - Create required directory structure"
	@echo ""
	@echo "Note: You need to implement the C source files in:"
	@echo "  stdio/, stdlib/, string/ directories"

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
