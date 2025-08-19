#include <stdio.h>

#include <kernel/tty.h>

int current_console_loglevel = 4;

void kernel_main(void) {
	terminal_initialize();
	printf("Hello, kernel World!\n");
}
