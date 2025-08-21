#include <stdio.h>
#include <kernel/kernel_levels.h>
#include <kernel/printk.h>
#include <kernel/tty.h>
#include <kernel/exceptions.h>
#include <kernel/idt.h>
#include <kernel/gdt.h>
#include <kernel/multiboot.h>
#include <kernel/serial.h>

int current_level = 0;

void kernel_main(multiboot_info_t* mbd, uint32_t magic) {
    terminal_initialize();
    serial_init();
    
    printk(KERN_NOTICE, "=====================================\n");
    printk(KERN_NOTICE, "       winOS Kernel Starting        \n");
    printk(KERN_NOTICE, "=====================================\n");
    
    printk(KERN_NOTICE, "Checking multiboot magic number...\n");
    if(magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printk(KERN_EMERG, "FATAL: Invalid magic number!\n");
        panic("invalid magic number!");
    }
    printk(KERN_NOTICE, "  Magic: 0x%x [OK]\n", magic);
    
    printk(KERN_NOTICE, "Checking memory map...\n");
    if(!(mbd->flags >> 6 & 0x1)) {
        printk(KERN_EMERG, "FATAL: Invalid memory map!\n");
        panic("invalid memory map given by GRUB bootloader");
    }
    printk(KERN_NOTICE, "  Memory map available [OK]\n");

    printk(KERN_NOTICE, "  Setting up GDT...");
    gdt_init();
    printk(KERN_NOTICE, " [OK]\n");
    
    printk(KERN_NOTICE, "  Setting up IDT...");
    idt_init();
    printk(KERN_NOTICE, " [OK]\n");
    
    printk(KERN_NOTICE, "\n=====================================\n");
    printk(KERN_NOTICE, "          System Memory Map          \n");
    printk(KERN_NOTICE, "=====================================\n");
    printk(KERN_NOTICE, "Base Address    | Length      | Type\n");
    printk(KERN_NOTICE, "-------------------------------------\n");
    
    int i;
    int available_blocks = 0;
    unsigned long total_available = 0;
    
    for(i = 0; i < mbd->mmap_length; i += sizeof(multiboot_memory_map_t)) {
        multiboot_memory_map_t* mmmt = 
            (multiboot_memory_map_t*) (mbd->mmap_addr + i);
        
        printk(KERN_INFO, "0x%x | 0x%x | ", 
               (uint32_t)mmmt->addr, (uint32_t)mmmt->len);
        
        if(mmmt->type == MULTIBOOT_MEMORY_AVAILABLE) {
            printk(KERN_INFO, "Available\n");
            available_blocks++;
            total_available += (uint32_t)mmmt->len;
        } else {
            printk(KERN_INFO, "Reserved\n");
        }
    }
    
    printk(KERN_NOTICE, "-------------------------------------\n");
    printk(KERN_NOTICE, "Available memory blocks: %d\n", available_blocks);
    printk(KERN_NOTICE, "Total available RAM: %d KB (%d MB)\n", 
           (uint32_t)(total_available / 1024), 
           (uint32_t)(total_available / (1024 * 1024)));
    
    printk(KERN_NOTICE, "\n=====================================\n");
    printk(KERN_NOTICE, "    Kernel initialization complete   \n");
    printk(KERN_NOTICE, "         System ready for use        \n");
    printk(KERN_NOTICE, "=====================================\n");
    
    printk(KERN_NOTICE, "\nKernel entering idle state...\n");
    
    while(1) {
        asm("hlt");
    }
}