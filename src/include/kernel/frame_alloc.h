#include <stdint.h>
#include "paging.h"

extern uint32_t endkernel;

static pageframe_t kalloc_frame_int();
pageframe_t kalloc_frame();
void kfree_frame(pageframe_t a);
