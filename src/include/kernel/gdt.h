#pragma once

#include <stdint.h>

#define NULL_SEGMENT    0x0000000000000000ULL
#define KERNEL_CODE     0x00CF9A000000FFFFULL
#define KERNEL_DATA     0x00CF92000000FFFFULL
#define USER_CODE       0x00CFFA000000FFFFULL
#define USER_DATA       0x00CFF2000000FFFFULL

extern void gdt_flush(uint32_t gdt_ptr_addr);
void gdt_init(void);
void gdt_set_descriptor(int32_t num, uint64_t descriptor);


