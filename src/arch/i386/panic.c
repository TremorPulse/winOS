#include "kernel/tty.h"
#include "kernel/vga.h"
#include <kernel/printk.h>
#include <kernel/kernel_levels.h>
#define VGA_BASE 0xB8000

/* We must inform ourselves when we encounter an unrecoverable exception. */

/* This function will implement variable args later.*/
void panic(const char* msg)
{
    // TODO: Disable interrupts.
    printk(KERN_EMERG, "An unrecoverable exception has occured. Rebooting.");
    // TODO: Dump stack and other debug info.
    // TODO: SET A REBOOT TIMER AFTER %d seconds
    asm("jmp 0xFFFF");
}

