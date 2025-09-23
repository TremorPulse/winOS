#pragma once

#include <stdint.h>
#include <stdbool.h>

#define CHANNEL_0 0x40
#define CHANNEL_1 0x41
#define CHANNEL_2 0x42
#define COMMAND_REG 0x43

// PIT operating modes
#define PIT_MODE_0 0  // Interrupt on terminal count
#define PIT_MODE_1 1  // Hardware re-triggerable one-shot
#define PIT_MODE_2 2  // Rate generator
#define PIT_MODE_3 3  // Square wave generator
#define PIT_MODE_4 4  // Software triggered strobe
#define PIT_MODE_5 5  // Hardware triggered strobe

// Access modes
#define PIT_ACCESS_LATCH 0
#define PIT_ACCESS_LOBYTE 1
#define PIT_ACCESS_HIBYTE 2
#define PIT_ACCESS_LOHI 3

struct pit_command {
    uint8_t bcd_mode : 1;        // Bit 0: 0=binary, 1=BCD
    uint8_t operating_mode : 3;  // Bits 1-3: Operating mode (0-5)
    uint8_t access_mode : 2;     // Bits 4-5: Access mode
    uint8_t channel : 2;         // Bits 6-7: Channel select
} __attribute__((packed));