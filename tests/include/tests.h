#include "serial.h"

// QEMU isa-debug-exit device
#define QEMU_EXIT_PORT 0xf4

// Exit codes for QEMU
#define TEST_SUCCESS 0x10  // QEMU will exit with code (0x10 << 1) | 1 = 33
#define TEST_FAILURE 0x11  // QEMU will exit with code (0x11 << 1) | 1 = 35

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            serial_write_string("FAIL: "); \
            serial_write_string(message); \
            serial_write_string("\n"); \
            test_exit(TEST_FAILURE); \
        } \
    } while(0)

#define TEST_ASSERT_EQ(expected, actual, message) \
    TEST_ASSERT((expected) == (actual), message)

#define TEST_PASS(message) \
    do { \
        serial_write_string("PASS: "); \
        serial_write_string(message); \
        serial_write_string("\n"); \
        test_exit(TEST_SUCCESS); \
    } while(0)

void test_init(void);
void test_exit(int exit_code);
void test_main(void); 