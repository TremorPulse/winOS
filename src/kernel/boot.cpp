// boot.cpp
// ---------------------------------------------------------------------------
// The kernel entry point responsible for setting up the processor environment.
// ---------------------------------------------------------------------------

#include <stdint.h>
#define ALIGN (1 << 0)
#define MEMINFO (1 << 1)
#define FLAGS (ALIGN | MEMINFO)
#define MAGIC 0x1BADB002
#define CHECKSUM -(MAGIC + FLAGS)
#define STACK_SIZE 16384

struct multiboot_header
{
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} __attribute__((packed, section(".multiboot")));

static struct multiboot_header mb_header =
{
    MAGIC,
    FLAGS,
    CHECKSUM
};

#pragma noreturn
extern "C" void _start() __attribute__((section(".start")))
{
    // initalise the stack and load contents
    static uint8_t stack[STACK_SIZE] __attribute__((section(".bss"), aligned(16)));

    asm volatile("mov %0, %%esp" : : "r"(stack + sizeof(stack)));

    kmain();

    // we shouldn't get here if kmain() is called
    for(;;)
        asm volatile("hlt");
}



