#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <kernel/kernel_levels.h>
#include <kernel/serial.h>

extern int current_level;

static bool print(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++) {
        // Output to serial (serial handles \n properly)
        serial_write_char(bytes[i]);
        
        // Output to VGA terminal with \r\n conversion
        if (bytes[i] == '\n') {
            // For VGA, send carriage return first, then newline
            if (putchar('\r') == EOF) return false;
            if (putchar('\n') == EOF) return false;
        } else {
            if (putchar(bytes[i]) == EOF) return false;
        }
    }
    return true;

}

// Helper function to convert number to string
static void print_number(unsigned int num, int base) {
    char digits[] = "0123456789ABCDEF";
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        serial_write_char('0');
        putchar('0');
        return;
    }
    
    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }
    
    // Print in reverse order to both serial and VGA
    while (i > 0) {
        char c = buffer[--i];
        serial_write_char(c);
        putchar(c);
    }
}

int printk(int level, const char* restrict format, ...) {
    if(level < current_level)
        return 0;
        
    va_list parameters;
    va_start(parameters, format);
    int written = 0;
    
    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;
        
        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%')
                amount++;
            if (maxrem < amount) {
                va_end(parameters);
                return EOVERFLOW;
            }
            if (!print(format, amount)) {
                va_end(parameters);
                return -1;
            }
            format += amount;
            written += amount;
            continue;
        }
        
        const char* format_begun_at = format++;
        
        if (*format == 'c') {
            format++;
            char c = (char) va_arg(parameters, int);
            if (!maxrem) {
                va_end(parameters);
                return EOVERFLOW;
            }
            if (!print(&c, sizeof(c))) {
                va_end(parameters);
                return -1;
            }
            written++;
        } else if (*format == 's') {
            format++;
            const char* str = va_arg(parameters, const char*);
            size_t len = strlen(str);
            if (maxrem < len) {
                va_end(parameters);
                return EOVERFLOW;
            }
            if (!print(str, len)) {
                va_end(parameters);
                return -1;
            }
            written += len;
        } else if (*format == 'd') {
            format++;
            int num = va_arg(parameters, int);
            if (num < 0) {
                serial_write_char('-');
                putchar('-');
                num = -num;
                written++;
            }
            print_number((unsigned int)num, 10);
            // Rough estimate of digits written
            written += (num == 0) ? 1 : 10;
        } else if (*format == 'x') {
            format++;
            unsigned int num = va_arg(parameters, unsigned int);
            print_number(num, 16);
            // Rough estimate of digits written
            written += 8;
        } else {
            format = format_begun_at;
            size_t len = strlen(format);
            if (maxrem < len) {
                va_end(parameters);
                return -1;
            }
            if (!print(format, len)) {
                va_end(parameters);
                return -1;
            }
            written += len;
            format += len;
        }
    }
    
    va_end(parameters);
    return written;
}