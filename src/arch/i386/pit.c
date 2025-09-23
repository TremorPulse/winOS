#include <stdint.h>
#include <kernel/pit.h>

bool pit_command_handler(struct pit_command *pit_command)
{
    // (0-2 are valid channels, 3 is read-back command)
    if (pit_command->channel > 3) {
        return false;
    }

    // must be one of the defined values
    if (pit_command->access_mode != PIT_ACCESS_LATCH && 
        pit_command->access_mode != PIT_ACCESS_LOBYTE && 
        pit_command->access_mode != PIT_ACCESS_HIBYTE && 
        pit_command->access_mode != PIT_ACCESS_LOHI) {
        return false;
    }

    if (pit_command->operating_mode > 5) {
        return false;
    }

    // BCD mode should be 0 (binary) for x86 systems
    // But we'll allow both 0 and 1 for compatibility
    if (pit_command->bcd_mode > 1) {
        return false;
    }

    return true;
}

void operating_mode_handler(struct pit_command *pit_command)
{
    switch(pit_command->operating_mode) {
        case PIT_MODE_0: // Interrupt on Terminal Count
            // Output goes low immediately when mode/command register is written
            pit_set_output_low(pit_command->channel);
            break;
            
        case PIT_MODE_1: // Hardware Re-triggerable One-shot
            // Output goes high immediately when mode/command register is written
            pit_set_output_high(pit_command->channel);
            break;
            
        case PIT_MODE_2: // Rate Generator
            // Output goes high immediately when mode/command register is written
            pit_set_output_high(pit_command->channel);
            break;
            
        case PIT_MODE_3: // Square Wave Generator
            // Output goes high immediately when mode/command register is written
            pit_set_output_high(pit_command->channel);
            break;
            
        case PIT_MODE_4: // Software Triggered Strobe
            // Output goes high immediately when mode/command register is written
            pit_set_output_high(pit_command->channel);
            break;
            
        case PIT_MODE_5: // Hardware Triggered Strobe
            // Output goes high immediately when mode/command register is written
            pit_set_output_high(pit_command->channel);
            break;
    }
    
    // Reset all internal logic in the selected PIT channel
    pit_reset_channel_logic(pit_command->channel);
}

static void pit_set_output_low(uint8_t channel)
{
    // Implementation depends on your hardware abstraction layer
    // This would typically involve setting hardware registers
}

static void pit_set_output_high(uint8_t channel)
{
    // Implementation depends on your hardware abstraction layer
    // This would typically involve setting hardware registers
}

static void pit_reset_channel_logic(uint8_t channel)
{
    // Reset internal counters, states, and logic for the specified channel
    // This is required by the PIT specification when mode/command register is written
}

