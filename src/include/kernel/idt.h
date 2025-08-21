#pragma once

#include <stdint.h>

void idt_init(void);
void idt_set(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
