int current_level = 0;  
#include <tests.h>

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile("outb %0, %1" :: "a"(value), "Nd"(port));
}

void test_init(void) {
    serial_init();
    serial_write_string("\n\nStarting test...\n");
}

void test_exit(int exit_code) {
    serial_write_string("Test finished with code: ");
    
    if (exit_code == TEST_SUCCESS) {
        serial_write_string("SUCCESS");
    } else {
        serial_write_string("FAILURE");
    }
    serial_write_string("\n");
    
    outb(QEMU_EXIT_PORT, exit_code);
    
    while(1) {
        asm volatile("hlt");
    }
}

void kernel_main(void) {
    test_init();
    test_main();
    
    // If we reach here without explicit pass/fail, assume failure
    TEST_ASSERT(0, "Test completed without explicit result");
}