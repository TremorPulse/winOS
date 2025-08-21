#include <stdio.h>
#include <kernel/kernel_levels.h>
#include <kernel/printk.h>
#include <kernel/tty.h>
#include <kernel/exceptions.h>
#include <kernel/idt.h>

int current_level = 0;

void kernel_main(void) {
	terminal_initialize();
	printk(KERN_NOTICE, "Starting kernel...\n");
	idt_init();
	printk(KERN_NOTICE, "IDT OK.\n");

	while(1);
}
