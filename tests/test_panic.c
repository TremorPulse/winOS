#include <tests.h>
#include <kernel/printk.h>

void panict(const char* message) {
    printk("TEST_MODE: PANIC: %s\\n", message);  
    // Use QEMU's debug exit device
    asm volatile("outl %0, %1" : : "a"(0), "Nd"(0xf4));
    while(1);
}

void test_main(void) {
    serial_write_string("Testing panic function...\n");
    
    panic("Test panic message");
    TEST_PASS("panic function works");
    
    // Should never reach here
    TEST_ASSERT(0, "Panic did not halt execution");
}

