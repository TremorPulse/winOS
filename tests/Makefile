HOST := $(shell ../default-host.sh)
CC := $(HOST)-gcc
ARCHDIR := ../src/arch/i386
CFLAGS := -O2 -g -ffreestanding -Wall -Wextra -I../src/include
KERNEL_OBJS := ../src/arch/i386/tty.o ../src/arch/i386/printk.o
LIBS := -nostdlib -L../sysroot/usr/lib -lk -lgcc
TEST_DIR = .
TEST_BUILD_DIR = build
TEST_SOURCES = $(wildcard test_*.c)
TEST_TARGETS = $(patsubst %.c,$(TEST_BUILD_DIR)/%.bin,$(TEST_SOURCES))
QEMU_TEST_FLAGS = -device isa-debug-exit,iobase=0xf4,iosize=0x04 -serial stdio -display none
TEST_CFLAGS = $(CFLAGS) -DTEST_MODE -I./include
TEST_CPPFLAGS = $(CPPFLAGS)
TEST_LIBS = $(LIBS)

# Add assembly generation flags
ASM_CFLAGS = $(CFLAGS) -S -fverbose-asm -masm=intel

.PHONY: test tests clean install-headers install assembly kernel.s

# Add rule to generate kernel.s
kernel.s: kernel.c
	@echo "Generating assembly output for kernel.c"
	$(CC) $(ASM_CFLAGS) -o kernel.s kernel.c

# Alternative rule if you want AT&T syntax instead of Intel syntax
kernel-att.s: kernel.c
	@echo "Generating assembly output (AT&T syntax) for kernel.c"
	$(CC) $(CFLAGS) -S -fverbose-asm -o kernel-att.s kernel.c

# Rule to generate assembly for any C file
%.s: %.c
	@echo "Generating assembly output for $<"
	$(CC) $(ASM_CFLAGS) -o $@ $<

# Add assembly target to generate all assembly files
assembly: kernel.s
	@echo "Assembly generation complete"

test: tests
	@echo "Running all tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		qemu-system-i386 $(QEMU_TEST_FLAGS) -kernel $$test || true; \
	done

tests: $(TEST_TARGETS)

$(TEST_BUILD_DIR)/%.bin: %.c ./serial.c | $(TEST_BUILD_DIR)
	@echo "Building test: $@"
	$(CC) -T ./linker.ld -o $@ $(TEST_CFLAGS) \
		$(ARCHDIR)/crti.o \
		$(ARCHDIR)/crtbegin.o \
		$(ARCHDIR)/boot.o \
		./tests.c \
		./serial.c \
		$< \
		$(filter-out kernel/kernel.o, kernel/kernel.s $(KERNEL_OBJS)) \
		$(TEST_LIBS) \
		$(ARCHDIR)/crtend.o \
		$(ARCHDIR)/crtn.o

$(TEST_BUILD_DIR):
	mkdir -p $(TEST_BUILD_DIR)

clean:
	rm -rf $(TEST_BUILD_DIR)
	rm -f *.s  # Also clean up assembly files

test-%: $(TEST_BUILD_DIR)/%.bin
	@echo "Running test: $*"
	qemu-system-i386 $(QEMU_TEST_FLAGS) -kernel $<

install-headers:
	@echo "No headers to install for tests"

install:
	@echo "Tests don't need installation"