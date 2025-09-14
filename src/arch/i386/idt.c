/* Interrupt Descriptor Table
 * This file defines the protected and long mode
 * counterpart to the real mode Interrupt Vector Table - the IDT.
 * IDT entries are called gates. Entries either contain interrupt gates, task gates and trap gates.
 * - An interrupt gate specifies an ISR.
 * - A trap gate handles exceptions.
 * - A trap gate is specific to 32 bits system used for hardware task switching.
 * - This is slow, and doesn't exist on modern processors (x86-64).
 */

#include <stdint.h>
#include <string.h>
#include <kernel/idt.h>
#include <kernel/exceptions.h>
#include <kernel/interrupts.h>

/* On 32-bit systems, entries form a table like this.
 * The 64-bit table just adds an ist variable to hold the Interrupt Stack offset
 * and three offset bit variables to go from 32..64.
 */
struct idt_entry {
    uint16_t base_low;   /* Lower 16 bits of handler function address */
    uint16_t selector;   /* Kernel segment selector (0x08 for kernel code segment) */
    uint8_t  always0;    /* Reserved field, must always be zero */
    uint8_t  flags;      /* Type and attributes byte */
    uint16_t base_high;  /* Upper 16 bits of handler function address */
} __attribute__((packed));

/* IDT pointer structure - used by LIDT instruction */
struct idt_ptr {
    uint16_t limit;     /* Size of IDT table in bytes minus 1 */
    uint32_t base;      /* Linear address of the IDT table in memory */
} __attribute__((packed));

/* Global IDT table - 256 entries to cover all possible interrupts (0-255) */
static struct idt_entry idt_entries[256];

/* IDT pointer that will be loaded into IDTR register */
static struct idt_ptr idt_ptr;

/**
 * Set up an entry in the Interrupt Descriptor Table
 * @param num      - Interrupt number (0-255)
 * @param base     - Address of the interrupt handler function
 * @param selector - Code segment selector (usually 0x08 for kernel code segment)
 * @param flags    - Type and attribute flags for this entry
 */
void idt_set(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    /* Split the handler address into low and high 16-bit parts */
    idt_entries[num].base_low  = base & 0xFFFF;         /* Extract bits 0-15 */
    idt_entries[num].base_high = (base >> 16) & 0xFFFF; /* Extract bits 16-31 */
    
    /* Set the code segment selector */
    idt_entries[num].selector = selector;
    
    /* Reserved field must always be zero */
    idt_entries[num].always0 = 0;
    
    /* Set type and attribute flags */
    idt_entries[num].flags = flags;
}

/**
 * Initialize the Interrupt Descriptor Table
 */
void idt_init(void) {
    /* Configure the IDT pointer structure */
    idt_ptr.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_ptr.base  = (uint32_t)&idt_entries;
    
    /* Clear all IDT entries to zero */
    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

    /* Set up essential CPU exception handlers */
    /* All use flags 0x8E which means:
     * - Present bit set (1)
     * - Descriptor Privilege Level 0 (kernel only)
     * - 32-bit interrupt gate type
     * - Code segment selector 0x08 (kernel code segment from GDT)
     */
    
    idt_set(0,  (uint32_t)isr0,  0x08, 0x8E);  // Division error
    idt_set(13, (uint32_t)isr13, 0x08, 0x8E);  // General protection fault
    idt_set(14, (uint32_t)isr14, 0x08, 0x8E);  // Page fault
    
    /* Load the IDT into the CPU's IDTR register using the LIDT instruction
     * The LIDT instruction tells the CPU where to find our interrupt table
     * From now on, when interrupts occur, CPU will look up handlers in this table
     */
    asm volatile("lidt %0" : : "m"(idt_ptr));
}