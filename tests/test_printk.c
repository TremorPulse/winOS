#include <tests.h>
#include <kernel/printk.h>
#include <kernel/kernel_levels.h>

void test_main(void) {
    serial_write_string("Testing printk function...\n");
    
    printk(KERN_INFO, "Test message from printk\n");
    printk(KERN_DEBUG, "Debug message: %d\n", 42);
    
    TEST_PASS("printk function work");
}