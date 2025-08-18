// tty.h
// ---------------------------------------------------------------------------
// Header definitions for the VGA buffer.
// ---------------------------------------------------------------------------

#pragma once
#include "stddef.h"
#include <stdint.h>

enum VGA_colour { BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, 
    RED = 4, MAGENTA = 5, 
    BROWN = 6, LGREY = 7, DGREY = 8, WHITE = 9};

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}