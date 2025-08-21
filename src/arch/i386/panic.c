#include "kernel/tty.h"
#include "kernel/vga.h"
#include <kernel/printk.h>
#include <kernel/kernel_levels.h>

/* We must inform ourselves when we encounter an unrecoverable exception. 
 - Clear all interrupts , inform us and halt the CPU.
*/

/* This function will implement variable args later.*/

void panic(const char* msg) {
    asm volatile("cli");
    printk(KERN_EMERG, "KERNEL PANIC: %s", msg);
    while(1)
        asm volatile("hlt");
}
