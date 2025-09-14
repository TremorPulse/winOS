/**
 * Global Descriptor Table
 * This file defines the protected mode segmentation for the kernel and user space.
 * It contains entries telling the CPU about memory segments.
 *
 * Most ISRs and memory management is controlled through tables like this one.
 * i.e before loading a new value to the segment register, a lot of checks are done
 * to see if we're entitled to that region. Then they get cached in invisible registers on the CPU.
 * These tables are placed once in memory at boot but can be modified later.
 */

#include <stdint.h>
#include <string.h>
#include <kernel/gdt.h>

/* Global Descriptor Table Entry Structure
 * Each entry is 8 bytes and describes a memory segment's properties
 * The structure is split oddly due to x86 compatibility reasons
 */
struct gdt_entry {
    uint16_t limit_low;    /* Bits 0-15 of segment limit (segment size - 1) */
    uint16_t base_low;     /* Bits 0-15 of segment base address */
    uint8_t  base_middle;  /* Bits 16-23 of segment base address */
    uint8_t  access;       /* Access flags byte:
                            * Bit 7: Present bit (1 = valid segment)
                            * Bits 6-5: Descriptor Privilege Level (0 = kernel, 3 = user)
                            * Bit 4: Descriptor type (1 = code/data, 0 = system)
                            * Bit 3: Executable bit (1 = code segment, 0 = data segment)
                            * Bit 2: Direction bit (0 = grows up, 1 = grows down)
                            * Bit 1: Readable/Writable bit (1 = readable code/writable data)
                            * Bit 0: Accessed bit (set by CPU when segment is accessed) */
    uint8_t  granularity;  /* Granularity and flags byte:
                            * Bit 7: Granularity (1 = 4KB blocks, 0 = 1 byte blocks)
                            * Bit 6: Size flag (1 = 32-bit, 0 = 16-bit)
                            * Bit 5: Long mode (1 = 64-bit code segment, 0 = not 64-bit)
                            * Bit 4: Available for system use
                            * Bits 3-0: Upper 4 bits of segment limit (bits 16-19) */
    uint8_t  base_high;    /* Bits 24-31 of segment base address */
} __attribute__((packed));

/* GDT Pointer Structure - used by LGDT instruction */
struct gdt_ptr {
    uint16_t limit;        /* Size of GDT table in bytes minus 1 */
    uint32_t base;         /* Linear address of the GDT table in memory */
} __attribute__((packed));

/* Global Descriptor Table - 5 entries:
 * 0: Null descriptor (required by x86 architecture)
 * 1: Kernel code segment (ring 0)
 * 2: Kernel data segment (ring 0) 
 * 3: User code segment (ring 3)
 * 4: User data segment (ring 3)
 */
static struct gdt_entry gdt_entries[5];

/* GDT pointer that will be loaded into GDTR register */
static struct gdt_ptr gdt_ptr;

/**
 * Load the GDT and update all segment registers
 * 1. Load the GDT using LGDT instruction
 * 2. Update all data segment registers to point to kernel data segment (0x10)
 * 3. Perform a far jump to reload the code segment register with kernel code segment (0x08)
 * 
 * @param gdt_ptr_addr - Address of the GDT pointer structure
 */
void gdt_flush(uint32_t gdt_ptr_addr) {
    asm volatile(
        "lgdt (%0)\n\t"          /* Load GDT from pointer */
        "mov $0x10, %%ax\n\t"    /* Load kernel data segment selector (entry 2 * 8 = 0x10) */
        "mov %%ax, %%ds\n\t"     /* Update data segment register */
        "mov %%ax, %%es\n\t"     /* Update extra segment register */
        "mov %%ax, %%fs\n\t"     /* Update F segment register */
        "mov %%ax, %%gs\n\t"     /* Update G segment register */
        "mov %%ax, %%ss\n\t"     /* Update stack segment register */
        "ljmp $0x08, $1f\n\t"    /* Far jump to reload CS with kernel code segment (entry 1 * 8 = 0x08) */
        "1:\n\t"                 /* Label for jump destination */
        :                        /* No output operands */
        : "r" (gdt_ptr_addr)     /* Input: GDT pointer address in any general register */
        : "eax"                  /* Clobbered register: we modify EAX */
    );
}

/**
 * Set a GDT descriptor entry from a pre-calculated 64-bit descriptor value
 * This function takes a 64-bit descriptor and splits it into the GDT entry structure
 * Turn to page 2-4 for Vol. 3A Intel® 64 and IA-32 Manual.
 * @param num        - GDT entry index (0-4)
 * @param descriptor - Pre-calculated 64-bit descriptor value containing all segment info
 */
void gdt_set_descriptor(int32_t num, uint64_t descriptor) {
    /* Extract and set the segment limit (bits 0-15 and 48-51) */
    gdt_entries[num].limit_low = descriptor & 0xFFFF;                    /* Bits 0-15 */
    
    /* Extract and set the base address (bits 16-31, 32-39, 56-63) */
    gdt_entries[num].base_low    = (descriptor >> 16) & 0xFFFF;         /* Bits 16-31 */
    gdt_entries[num].base_middle = (descriptor >> 32) & 0xFF;           /* Bits 32-39 */
    gdt_entries[num].base_high   = (descriptor >> 56) & 0xFF;           /* Bits 56-63 */
    
    /* Extract and set access flags (bits 40-47) */
    gdt_entries[num].access      = (descriptor >> 40) & 0xFF;           /* Bits 40-47 */
    
    /* Extract and set granularity/flags and upper limit (bits 48-55) */
    gdt_entries[num].granularity = (descriptor >> 48) & 0xFF;           /* Bits 48-55 */
}

/**
 * Initialize the Global Descriptor Table
 * Sets up a basic flat memory model with separate kernel and user segments
 */
void gdt_init(void) {
    /* Configure the GDT pointer structure */
    gdt_ptr.limit = (sizeof(struct gdt_entry) * 5) - 1;  /* Total size of GDT minus 1 */
    gdt_ptr.base  = (uint32_t)&gdt_entries;               /* Address of our GDT table */
    
    /* Clear all GDT entries to zero (creates null descriptors) */
    memset(&gdt_entries, 0, sizeof(struct gdt_entry) * 5);
    
    /* Set up GDT entries using pre-calculated descriptor values
     * These values are defined as constants (NULL_SEGMENT, KERNEL_CODE, etc.)
     * and contain all the necessary flags for each segment type:
     * 
     * Entry 0: NULL_SEGMENT  - Required null descriptor (CPU generates exception if used)
     * Entry 1: KERNEL_CODE   - Ring 0 executable segment covering all 4GB of memory
     * Entry 2: KERNEL_DATA   - Ring 0 readable/writable segment covering all 4GB
     * Entry 3: USER_CODE     - Ring 3 executable segment for user applications
     * Entry 4: USER_DATA     - Ring 3 readable/writable segment for user applications
     */
    gdt_set_descriptor(0, NULL_SEGMENT);   
    gdt_set_descriptor(1, KERNEL_CODE);    
    gdt_set_descriptor(2, KERNEL_DATA);   
    gdt_set_descriptor(3, USER_CODE);      
    gdt_set_descriptor(4, USER_DATA);      
    
    /* Load the GDT into the CPU and update all segment registers
     * After this point, all memory accesses will go through our GDT
     * The CPU will use selectors 0x08 and 0x10 for kernel code and data
     */
    gdt_flush((uint32_t)&gdt_ptr);
}