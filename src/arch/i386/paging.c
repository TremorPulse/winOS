/**
 * Page Table
 */

#include <stdint.h>
#include <kernel/paging.h>

uint32_t page_directory[1024] __attribute((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

/* Sets the following flags to the pages:
    - Supervisor: Only kernel-mode can access them.
    - Write Enabled: It can be both read from and written to.
    - Not Present: The page table is not present.
*/ 
void zero_page_table(uint32_t* page_directory)
{
    for(volatile int i = 0; i < 1024; i++)
        page_directory[i] = 0x00000002;
}

/* Here we map the physical address where we want to start mapping these pages to 
in this case, we want to map these pages to the very beginning of memory.
*/
void fill_page_table(uint32_t* first_page_table)
{   // Fill all 1024 entries in the table, mapping 4 megabytes.
    for(volatile int i = 0; i < 1024; i++)
    {    // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes 
        first_page_table[i] = ((volatile int) i * 0x1000) | 3; // supervisor level, read/write, present.
    }
}

void init_page_table()
{   zero_page_table(page_directory);
    fill_page_table(first_page_table);

    page_directory[0] = ((unsigned int)first_page_table) | 3;

    loadPageDirectory(page_directory);
    enablePaging();
}

