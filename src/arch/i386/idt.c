#include <stdint.h>
#include <string.h>
#include <kernel/idt.h>
#include <kernel/exceptions.h>

// Forward declarations of your exception handlers
extern void isr0(void);   // Division error
extern void isr13(void);  // General protection fault
extern void isr14(void);  // Page fault

// x86 IDT entry structure 
struct idt_entry {
    uint16_t base_low;    // Lower 16 bits of handler address
    uint16_t selector;    // Kernel segment selector (usually 0x08)
    uint8_t  always0;     // Always 0
    uint8_t  flags;       // Type and attributes
    uint16_t base_high;   // Upper 16 bits of handler address
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;       // Size of IDT - 1
    uint32_t base;        // Address of IDT
} __attribute__((packed));

static struct idt_entry idt_entries[256];
static struct idt_ptr idt_ptr;

void idt_set(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    
    idt_entries[num].selector = selector;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void idt_init(void) {
    // Set up IDT pointer
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;
    
    // Clear all entries first
    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);
    
    // Set up essential exception handlers
    // Flags: 0x8E = present, ring 0, 32-bit interrupt gate
    idt_set(0, (uint32_t)isr0, 0x08, 0x8E);    // Division error
    idt_set(13, (uint32_t)isr13, 0x08, 0x8E);  // General protection
    idt_set(14, (uint32_t)isr14, 0x08, 0x8E);  // Page fault
    
    // Load the IDT
    asm volatile("lidt %0" : : "m"(idt_ptr));
}