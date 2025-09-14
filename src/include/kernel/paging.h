#pragma once
#include <stdint.h>

extern void loadPageDirectory(uint32_t* page_directory);
extern void enablePaging(void);

void zero_page_table(uint32_t* page_directory);
void fill_page_table(uint32_t* first_page_table);
void init_page_table(void);