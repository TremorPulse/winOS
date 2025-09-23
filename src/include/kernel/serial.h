#pragma once

#include <stdint.h>

#define SERIAL_BASE        0x3F8
#define DATA_PORT(base)    (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)
#define SERIAL_LINE_ENABLE_DLAB         0x80


void serial_init(void);
void serial_write_char(char c);
void serial_write_string(const char* str);
int serial_is_transmit_fifo_empty(uint16_t com);
