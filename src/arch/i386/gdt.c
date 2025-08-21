/*
* Global Descriptor Table
 * This file defines the protected mode segmentation
 * for the kernel and user space.
 */
#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>

// x86 GDT entry structure
struct gdt_entry {
    uint16_t limit_low;     // Lower 16 bits of limit
    uint16_t base_low;      // Lower 16 bits of base address
    uint8_t  base_middle;   // Next 8 bits of base address
    uint8_t  access;        // Access flags
    uint8_t  granularity;   // Granularity and upper 4 bits of limit
    uint8_t  base_high;     // Upper 8 bits of base address
} __attribute__((packed));

struct gdt_ptr {
    uint16_t limit;         // Size of GDT - 1
    uint32_t base;          // Address of GDT
} __attribute__((packed));

// GDT entries: null, kernel code, kernel data, user code, user data
static struct gdt_entry gdt_entries[5];
static struct gdt_ptr gdt_ptr;

// Inline assembly function to load GDT and update segment registers
void gdt_flush(uint32_t gdt_ptr_addr) {
    asm volatile(
        "lgdt (%0)\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "ljmp $0x08, $1f\n\t"
        "1:\n\t"
        :
        : "r" (gdt_ptr_addr)
        : "eax"
    );
}

void gdt_set_descriptor(int32_t num, uint64_t descriptor) {
    gdt_entries[num].limit_low    = descriptor & 0xFFFF;
    gdt_entries[num].base_low     = (descriptor >> 16) & 0xFFFF;
    gdt_entries[num].base_middle  = (descriptor >> 32) & 0xFF;
    gdt_entries[num].access       = (descriptor >> 40) & 0xFF;
    gdt_entries[num].granularity  = (descriptor >> 48) & 0xFF;
    gdt_entries[num].base_high    = (descriptor >> 56) & 0xFF;
}

void gdt_init(void) {
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gdt_ptr.base  = (uint32_t)&gdt_entries;
    
    // Clear all entries first
    memset(&gdt_entries, 0, sizeof(struct gdt_entry) * 5);
    
    // Set up GDT entries using pre-calculated descriptor values
    // These values come from running a descriptor generator utility:
    gdt_set_descriptor(0, NULL_SEGMENT); 
    gdt_set_descriptor(1, KERNEL_CODE);
    gdt_set_descriptor(2, KERNEL_DATA);  
    gdt_set_descriptor(3, USER_CODE);   
    gdt_set_descriptor(4, USER_DATA); 
    
    // Load the GDT
    gdt_flush((uint32_t)&gdt_ptr);
}