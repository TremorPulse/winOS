#include "kernel/tty.h"
#include "kernel/vga.h"
#include <kernel/printk.h>
#include <kernel/kernel_levels.h>

/* We must inform ourselves when we encounter an unrecoverable exception. 
 - Clear all interrupts, inform us and halt the CPU.
*/

/* This function will implement variable args later.*/

void panic(const char* msg) {
    asm volatile("cli");

    printk(KERN_EMERG, "\n");
    printk(KERN_EMERG, "Your computer ran into a problem and needs to restart.\n");
    printk(KERN_EMERG, "We're just collecting some error info, and then we'll restart for you.\n\n");
    printk(KERN_EMERG, "KERNEL PANIC: %s\n", msg);
    printk(KERN_EMERG, "\nIf you call a support person, give them this info:\n");
    printk(KERN_EMERG, "STOP CODE: KERNEL_PANIC\n");

    while(1)
        asm volatile("hlt");
}
