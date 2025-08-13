// tty.h
// ---------------------------------------------------------------------------
// Header definitions for the VGA buffer.
// ---------------------------------------------------------------------------
#pragma once

#include "stddef.h"
#include <stdint.h>

#define VGA_WIDTH 80;
#define VGA_HEIGHT 25;
#define VGA_BASE = 0xB8000;

enum VGA_colour { BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, 
    RED = 4, MAGENTA = 5, 
    BROWN = 6, LGREY = 7, DGREY = 8, WHITE = 9};
