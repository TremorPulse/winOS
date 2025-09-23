#pragma once
#include <stdint.h>

extern void loadPageDirectory(uint32_t* page_directory);
extern void enablePaging(void);

typedef struct page_frame {
    uint32_t present    : 1;  // Page present in memory
    uint32_t rw         : 1;  // Read/write
    uint32_t user       : 1;  // User/supervisor
    uint32_t accessed   : 1;  // Accessed
    uint32_t dirty      : 1;  // Dirty
    uint32_t unused     : 7;  // Unused/reserved
    uint32_t frame_addr : 20; // Frame address (shifted right 12 bits)
} pageframe_t;

void zero_page_table(uint32_t* page_directory);
void fill_page_table(uint32_t* first_page_table);
void init_page_table(void);