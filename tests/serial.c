#include <serial.h>

static inline void outb(uint16_t port, uint8_t value)
{
    asm volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
    return ret;
}

void serial_init(void) {
    // Disable all interrupts
    outb(SERIAL_BASE + 1, 0x00);
    
    // Enable DLAB (set baud rate divisor)
    outb(SERIAL_LINE_COMMAND_PORT(SERIAL_BASE), SERIAL_LINE_ENABLE_DLAB);
    
    // Set divisor to 3 (lo byte) 38400 baud
    outb(DATA_PORT(SERIAL_BASE), 0x03);
    outb(SERIAL_BASE + 1, 0x00); // (hi byte)
    
    // 8 bits, no parity, one stop bit
    outb(SERIAL_LINE_COMMAND_PORT(SERIAL_BASE), 0x03);
    
    // Enable FIFO, clear them, with 14-byte threshold
    outb(SERIAL_FIFO_COMMAND_PORT(SERIAL_BASE), 0xC7);
    
    // IRQs enabled, RTS/DSR set
    outb(SERIAL_MODEM_COMMAND_PORT(SERIAL_BASE), 0x0B);
    
    // Test serial chip (send byte 0xAE and check if serial returns same byte)
    outb(SERIAL_BASE, 0xAE);
    
    // Check if serial is faulty (i.e: not same byte as sent)
    if(inb(SERIAL_BASE) != 0xAE) {
        // Serial is faulty
        return;
    }
    
    // If serial is not faulty set it in normal operation mode
    // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
    outb(SERIAL_MODEM_COMMAND_PORT(SERIAL_BASE), 0x0F);
}

int serial_is_transmit_fifo_empty(uint16_t com) {
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

void serial_write_char(char c) {
    while (serial_is_transmit_fifo_empty(SERIAL_BASE) == 0);
    outb(SERIAL_BASE, c);
}

void serial_write_string(const char* str) {
    while (*str) {
        serial_write_char(*str);
        str++;
    }
}

