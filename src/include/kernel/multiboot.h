#pragma once

#include <stdint.h>

#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

#define MULTIBOOT_MEMORY_AVAILABLE          1    
#define MULTIBOOT_MEMORY_RESERVED           2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE   3
#define MULTIBOOT_MEMORY_NVS                4
#define MULTIBOOT_MEMORY_BADRAM             5

#define MULTIBOOT_INFO_MEMORY           0x00000001
#define MULTIBOOT_INFO_BOOTDEV          0x00000002
#define MULTIBOOT_INFO_CMDLINE          0x00000004
#define MULTIBOOT_INFO_MODS             0x00000008
#define MULTIBOOT_INFO_AOUT_SYMS        0x00000010
#define MULTIBOOT_INFO_ELF_SHDR         0x00000020
#define MULTIBOOT_INFO_MEM_MAP          0x00000040
#define MULTIBOOT_INFO_DRIVE_INFO       0x00000080
#define MULTIBOOT_INFO_CONFIG_TABLE     0x00000100
#define MULTIBOOT_INFO_BOOT_LOADER_NAME 0x00000200
#define MULTIBOOT_INFO_APM_TABLE        0x00000400
#define MULTIBOOT_INFO_VBE_INFO         0x00000800

typedef struct multiboot_memory_map {
    uint32_t size;          // Size of this entry (minus this field)
    uint64_t addr;          // Base address
    uint64_t len;           // Length of memory region
    uint32_t type;          // Type of memory region
} __attribute__((packed)) multiboot_memory_map_t;

typedef struct multiboot_mod_list {
    uint32_t mod_start;     // Start address of module
    uint32_t mod_end;       // End address of module
    uint32_t cmdline;       // Command line for this module
    uint32_t pad;           // Padding
} __attribute__((packed)) multiboot_module_t;

typedef struct multiboot_info {
    uint32_t flags;         // Multiboot info version flags

    // Available memory from BIOS
    uint32_t mem_lower;    
    uint32_t mem_upper;     
    uint32_t boot_device;  

    uint32_t cmdline; 

    // Boot modules
    uint32_t mods_count;    
    uint32_t mods_addr;     // Address of module list

    // ELF section header info (union with a.out symbols)
    union {
        struct {
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t reserved;
        } elf_sec;
        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } aout_sym;
    } u;

    // Memory mapping buffer
    uint32_t mmap_length;  
    uint32_t mmap_addr;     

    // Drive info
    uint32_t drives_length; // Length of drive info
    uint32_t drives_addr;   // Address of drive info

    // ROM configuration table
    uint32_t config_table;  // ROM configuration table
    uint32_t boot_loader_name; 
    uint32_t apm_table;  

    // VBE video info
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed)) multiboot_info_t;
