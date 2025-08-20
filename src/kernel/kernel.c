#include <stdio.h>
#include <kernel/kernel_levels.h>
#include <kernel/printk.h>

int current_level = 0;

void kernel_main(void) {
	terminal_initialize();
	panic("Help!!");
}
